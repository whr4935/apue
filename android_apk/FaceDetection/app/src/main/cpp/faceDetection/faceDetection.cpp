#include <android/log.h>
#include <pthread.h>
#include <stdlib.h>

#ifdef USE_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#endif

#include "faceDetection.h"
#include "detectorBase.h"

#include "genderclassifier.hpp"
#include "ageclassifier.hpp"

#define LOG_NDEBUG 0
#define LOG_TAG "faceDetecton"

#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, ##__VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, ##__VA_ARGS__)
#define ALOGE(...) \
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##__VA_ARGS__)

#ifdef USE_OPENCV
using namespace cv;
#endif
using std::string;



// -----------------------------------------------
class FaceDetection::DetectResultFilter
{
public:
    enum {
        kHasFaceThresHold = 1,
        kFacePositionThresHold = 1,
        kDelta = 5,
    };

    DetectResultFilter();
    ~DetectResultFilter();

#ifdef USE_OPENCV
    void filterResult(const std::vector<Rect>& faces);
#endif
    bool hasResult() const;
    const std::vector<int> result();
#ifdef USE_OPENCV
    const std::vector<Rect>& getResult();
#endif

private:
#ifdef USE_OPENCV
    std::vector<Rect> mResult;
    std::vector<Rect> mLastResult;
#endif

    int mHasFaceCount;
    int mFacePositionCount;

    bool mHasFace;
    bool mResultUpdated;
    bool mShouldUpdateResult;
};

FaceDetection::DetectResultFilter::DetectResultFilter()
    : mHasFaceCount(0)
    , mFacePositionCount(0)
    , mHasFace(false)
    , mResultUpdated(false)
    , mShouldUpdateResult(false)
{

}

FaceDetection::DetectResultFilter::~DetectResultFilter()
{

}


#ifdef USE_OPENCV
void FaceDetection::DetectResultFilter::filterResult(const std::vector<Rect>& faces)
{
    mLastResult = mResult;
    mResult = faces;

    if (mLastResult.size() != mResult.size()) {
        mHasFaceCount = 0;
        mResultUpdated = false;
        mHasFace = false;
    }

    if (mHasFaceCount < kHasFaceThresHold) {
        ++mHasFaceCount;
    } else {
        if (!mResultUpdated) {
            mShouldUpdateResult = true;
        }

        if (mResult.size() > 0)
            mHasFace = true;
    }

    if (mHasFace && mLastResult.size()==mResult.size()) {
        for (int i = 0; i != mResult.size(); ++i) {
            if (abs(mResult[i].x - mLastResult[i].x) >= kDelta ||
                abs(mResult[i].y - mLastResult[i].y) >= kDelta ||
                abs(mResult[i].width - mLastResult[i].width) >= kDelta ||
                abs(mResult[i].height - mLastResult[i].height) >= kDelta) {
                mFacePositionCount = 0;
                mResultUpdated = false;
            } else {
                mResult[i] = mLastResult[i];
            }
        }

        if (mFacePositionCount < kFacePositionThresHold) {
            ++mFacePositionCount;
        } else {
            if (!mResultUpdated) {
                mShouldUpdateResult = true;
            }
        }
    }
}
#endif

inline bool FaceDetection::DetectResultFilter::hasResult() const
{
    return mShouldUpdateResult;
}

inline const std::vector<int> FaceDetection::DetectResultFilter::result()
{
    std::vector<int> result;

#ifdef USE_OPENCV
    for (int i = 0; i < mResult.size(); ++i) {
       result.push_back(mResult[i].x);
       result.push_back(mResult[i].y);
       result.push_back(mResult[i].width);
       result.push_back(mResult[i].height);
    }

    mShouldUpdateResult = false;
    mResultUpdated = true;
#endif

    return result;
}

#ifdef USE_OPENCV
const std::vector<Rect>& FaceDetection::DetectResultFilter::getResult() {
    return mResult;
}
#endif

// -----------------------------------------------
char* FaceDetection::sModelsDir = NULL;

FaceDetection::FaceDetection(int width, int height, PixelFormat pix_fmt, FrameListener *frameListener)
    : mStarted(false),
      mWidth(width),
      mHeight(height),
      mPixelFormat(pix_fmt),
      mListener(NULL),
      mFrameListener(frameListener),
      mWorkThreadId(-1),
      mRequestExit(false),
      mSem(kMaxDetectedQueueDepth),
      mFilter(new DetectResultFilter),
      mGenderClassifiler(NULL),
      mGCEnabled(false),
      mAgeClassifier(NULL),
      mACEnabled(false)
{
    mDetector = DetectorBase::createDetector(this);

    pthread_mutex_init(&mLock, NULL);
    pthread_cond_init(&mFrameCond, NULL);
    pthread_mutex_init(&mPoolLock, NULL);
}

FaceDetection::~FaceDetection()
{
    if (mStarted) {
        stop();
    }

    if (mFrameListener) {
        delete mFrameListener;
        mFrameListener = NULL;
    }

    if (mListener) {
        delete mListener;
        mListener = NULL;
    }

    if (mFilter) {
        delete mFilter;
        mFilter = NULL;
    }

    if (mDetector) {
        delete mDetector;
        mDetector = NULL;
    }

    if (mGenderClassifiler) {
        delete mGenderClassifiler;
        mGenderClassifiler = NULL;
    }

    if (mAgeClassifier) {
        delete mAgeClassifier;
        mAgeClassifier = NULL;
    }

    pthread_mutex_destroy(&mLock);
    pthread_mutex_destroy(&mPoolLock);
    pthread_cond_destroy(&mFrameCond);

    ALOGI("dtor FaceDetection");
}

int FaceDetection::start()
{
    int err = -1;

    if (mStarted) {
        ALOGI("FaceDetection already started!");
        return 0;
    }

    err = mDetector->start();
    if (err < 0) {
        ALOGE("mDetector start failed!");
        return -1;
    }

    ALOGI("starting faceDetection...");
    err = pthread_create(&mWorkThreadId, NULL, wrapper_thread, this);
    if (err != 0) {
        ALOGE("create work_thread failed! err= %d", err);

        return -1;
    }

    mStarted = true;
    ALOGI("faceDetection started!");
    return 0;
}

int FaceDetection::stop()
{
    if (!mStarted)
        return 0;

    ALOGI("stoping faceDetection...");
    pthread_mutex_lock(&mLock);
    mRequestExit = true;
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mFrameCond);
    pthread_join(mWorkThreadId, NULL);

    mDetector->stop();

    mStarted = false;
    ALOGI("faceDetection stoped!");
    return 0;
}

// -----------------------------------------------
void FaceDetection::setModelsDir(const char* dir)
{
    if (sModelsDir) {
        free(sModelsDir);
        sModelsDir = NULL;
    }

    sModelsDir = strdup(dir);
}

void FaceDetection::enableGenderClassifier(bool enable) {
    pthread_mutex_lock(&mLock);
    mGCEnabled = enable;
    pthread_mutex_unlock(&mLock);
}
void FaceDetection::enableAgeClassifier(bool enable) {
    pthread_mutex_lock(&mLock);
    mACEnabled = enable;
    pthread_mutex_unlock(&mLock);
}

int FaceDetection::sendFrame(uint8_t *data, int64_t timestamp, void *cookie)
{
    int sem = __sync_fetch_and_or(&mSem, 0);
    if (!mStarted || sem ==0) {
        if (mFrameListener) {
            mFrameListener->onFrameReleased(cookie);
        }
        return 0;
    }

    Frame *f = acqurireFrame();
    f->mData = data;
    f->mTimeStamp = timestamp;
    f->cookie = cookie;

    pthread_mutex_lock(&mLock);
    mFrames.push_back(f);
    pthread_mutex_unlock(&mLock);
    pthread_cond_signal(&mFrameCond);

    __sync_sub_and_fetch(&mSem, 1);

    return 0;
}

void FaceDetection::setListener(FaceDetectionListener *listener)
{
    if (mStarted) {
        ALOGE("we have started, can not set listener now!");
        return;
    }

    mListener = listener;
}

void *FaceDetection::wrapper_thread(void *arg)
{
    FaceDetection *d = static_cast<FaceDetection *>(arg);

    d->threadLoop();

    return (void *)0;
}

void FaceDetection::threadLoop()
{
    if (mListener != NULL) {
        mListener->onFrameSize(mWidth, mHeight);
    }

#ifdef USE_OPENCV
    std::vector<Rect> faces;
#endif
    for (;;) {
        pthread_mutex_lock(&mLock);
        while (!mRequestExit && mFrames.empty()) {
            pthread_cond_wait(&mFrameCond, &mLock);
        }
        if (mRequestExit) {
            mRequestExit = false;
            doExitLoop();
            pthread_mutex_unlock(&mLock);
            break;
        }

        Frame *f = *mFrames.begin();
        mFrames.pop_front();

#ifdef USE_OPENCV
        if (mGCEnabled) {
            if (!mGenderClassifiler && mListener != NULL) {
                std::string dir(sModelsDir);
                dir.append("/fisher64eq.yml");
                mGenderClassifiler = new GenderClassifier(dir);
            }
        } else {
            if (mGenderClassifiler != NULL) {
                delete mGenderClassifiler;
                mGenderClassifiler = NULL;
            }
        }

        if (mACEnabled) {
            if (!mAgeClassifier && mListener != NULL) {
                std::string dir(sModelsDir);
                dir.append("/age_cropped.yml");
                mAgeClassifier = new AgeClassifier(dir);
            }
        } else {
            if (mAgeClassifier != NULL) {
                delete mAgeClassifier;
                mAgeClassifier = NULL;
            }
        }
#endif

        pthread_mutex_unlock(&mLock);

        // do some work here
        // ALOGI("get a frame:%p, timestamp:%lld, cookie:%p", f->mData, f->mTimeStamp, f->cookie);

#ifdef USE_OPENCV
        if (mDetector) {
            cv::Mat gray;
            mDetector->detect(f, faces, gray);

            mFilter->filterResult(faces);
            if (mFilter->hasResult()) {
                std::vector<int> msg;
                const std::vector<Rect>& result = mFilter->getResult();
                for (int i = 0; i < result.size(); ++i) {
                    double confidence;
                    int gender = -1;
                    if (mGenderClassifiler) {
                        cv::Mat face = gray(result[i]);
                        gender = mGenderClassifiler->classify(face, confidence);
                    }
                    int age = -1;
                    if (mAgeClassifier) {
                        cv::Mat face = gray(result[i]);
                        age = mAgeClassifier->classify(face, confidence);
                    }

                    msg.push_back(result[i].x);
                    msg.push_back(result[i].y);
                    msg.push_back(result[i].width);
                    msg.push_back(result[i].height);
                    msg.push_back(gender);
                    msg.push_back(age);
                }

                if (mListener != NULL)
                    mListener->onUpdateFacePosition(msg);
            }
        }
#endif

        __sync_add_and_fetch(&mSem, 1);

        // finished
        recycleFrame(f, mFrameListener);
    }

}

void FaceDetection::recycleFrame(Frame *f, FrameListener *listener)
{
    if (listener) {
        listener->onFrameReleased(f->cookie);
    }

    // ALOGI("release frame:%p", f->mData);
    releaseFrame(f);
}

void FaceDetection::doExitLoop()
{
    for (std::list<Frame *>::iterator it = mFrames.begin();
         it != mFrames.end(); ++it) {
        recycleFrame(*it, mFrameListener);
    }
    mSem = kMaxDetectedQueueDepth;

    pthread_mutex_lock(&mPoolLock);
    for (std::list<Frame *>::iterator it = mFramePool.begin();
         it != mFramePool.end(); ++it) {
        delete *it;
    }
    mFramePool.clear();
    pthread_mutex_unlock(&mPoolLock);
}

FaceDetection::Frame *FaceDetection::acqurireFrame()
{
    Frame *f = NULL;
    pthread_mutex_lock(&mPoolLock);
    if (!mFramePool.empty()) {
        f = *mFramePool.begin();
        mFramePool.pop_front();
    }
    pthread_mutex_unlock(&mPoolLock);

    if (f == NULL) {
        f = new Frame();
    }

    return f;
}

void FaceDetection::releaseFrame(Frame *f)
{
    pthread_mutex_lock(&mPoolLock);
    mFramePool.push_back(f);
    pthread_mutex_unlock(&mPoolLock);
}
