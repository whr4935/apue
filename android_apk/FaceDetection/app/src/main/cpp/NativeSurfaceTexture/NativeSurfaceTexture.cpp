//
// Created by whr on 18-5-18.
//

#include <jni.h>
#include <android/log.h>
#include "NativeSurfaceTexture.h"

#define LOG_NDEBUG 0
#define LOG_TAG "Renderer"

#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, ##__VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, ##__VA_ARGS__)
#define ALOGE(...) \
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##__VA_ARGS__)

namespace {
    JavaVM *g_JVM;
    jclass g_jSurfaceTextureClazz;
    struct fields_t {
        jfieldID  surfaceTexture;
        jmethodID ctor;
        jmethodID updateTexImage;
        jmethodID getTransformMatrix;
        jmethodID getTimestamp;
    };
    fields_t fields;
}

static inline void setNativeSurfaceTextureObj(JNIEnv* env, jobject thiz, NativeSurfaceTexture* surfaceTexture)
{
    env->SetLongField(thiz, fields.surfaceTexture, (jlong)surfaceTexture);
}

static inline NativeSurfaceTexture* getNativeSurfaceTextureObj(JNIEnv* env, jobject thiz)
{
    return (NativeSurfaceTexture*)env->GetLongField(thiz, fields.surfaceTexture);
}

/////////////////////////////////////////////////////////////////////
NativeSurfaceTexture::NativeSurfaceTexture(int texName) {
    JNIEnv* env = nullptr;

    int ret = g_JVM->AttachCurrentThread(&env, nullptr);
    if (ret != JNI_OK) {
        ALOGE("jvm attach current thread failed!");
        return;
    }

    mJSurfaceTexture = (void*)env->NewObject(g_jSurfaceTextureClazz, fields.ctor, texName);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        env->ThrowNew(env->FindClass("java/lang/Exception"), "JNI Exception!");
    }

    setNativeSurfaceTextureObj(env, (jobject )mJSurfaceTexture, this);

    g_JVM->DetachCurrentThread();
}

NativeSurfaceTexture::~NativeSurfaceTexture() {

}

void NativeSurfaceTexture::setFrameAvailableListener(OnFrameAvailableListener *listener) {
    mFrameAvailableListener = listener;
}

void NativeSurfaceTexture::onFrameAvailable() {
    if (mFrameAvailableListener) {
        mFrameAvailableListener->onFrameAvailable(this);
    }
}

int NativeSurfaceTexture::updateTexImage() const {
    JNIEnv* env;
    g_JVM->AttachCurrentThread(&env, nullptr);
    env->CallVoidMethod((jobject)mJSurfaceTexture, fields.updateTexImage);

    g_JVM->DetachCurrentThread();
}

void NativeSurfaceTexture::getTransformMatrix(float *mtx) {
    JNIEnv* env;
    g_JVM->AttachCurrentThread(&env, nullptr);
    jfloatArray jmtx = env->NewFloatArray(16);
    env->CallVoidMethod((jobject)mJSurfaceTexture, fields.updateTexImage, jmtx);
    env->GetFloatArrayRegion(jmtx, 0, 16, mtx);
    env->DeleteLocalRef(jmtx);

    g_JVM->DetachCurrentThread();
}

int64_t NativeSurfaceTexture::getTimestamp() {
    JNIEnv* env;
    g_JVM->AttachCurrentThread(&env, nullptr);
    jlong timestamp = env->CallLongMethod((jobject)mJSurfaceTexture, fields.getTimestamp);
    g_JVM->DetachCurrentThread();

    return timestamp;
}

///////////////////////////////////////////////////////
static void native_init(JNIEnv* env)
{
    jclass clazz;

    clazz = env->FindClass("com/example/whr/facedetection/JSurfaceTexture");
    if (clazz == nullptr) {
        ALOGE("native_init findClass failed!");
        return;
    }
    g_jSurfaceTextureClazz = (jclass)env->NewGlobalRef(clazz);
    fields.surfaceTexture = env->GetFieldID(clazz, " mNativeSurfaceTexture", "J");
    fields.ctor = env->GetMethodID(clazz, "<init>", "(J)V");
    fields.updateTexImage = env->GetMethodID(clazz, "updateTexImage", "()V");
    fields.getTransformMatrix = env->GetMethodID(clazz, "getTransformMatrix", "([F)V");
    fields.getTimestamp = env->GetMethodID(clazz, "getTimestamp", "()J");
}

static void native_finalize(JNIEnv* env, jobject thiz)
{
    NativeSurfaceTexture* surfaceTexture = getNativeSurfaceTextureObj(env, thiz);
    delete surfaceTexture;
}

static void native_onFrameAvailable(JNIEnv* env, jobject thiz, jlong surfaceTexture)
{
    NativeSurfaceTexture* nativeSurfaceTexture = (NativeSurfaceTexture*)surfaceTexture;

    nativeSurfaceTexture->onFrameAvailable();
}

static JNINativeMethod gMethods[] = {
        {"native_init",
            "()V",
            (void*)native_init,
        },

        {"native_onFrameAvailable",
         "(J)V",
        (void*)native_onFrameAvailable
        },

        {
        "native_finalize",
        "()V",
        (void*)native_finalize,
        }
};

#define NELM(a)     (sizeof(a)/sizeof(*a))

int register_com_example_whr_jsurfaceTexture(JNIEnv* env)
{
    jclass  clazz;
    clazz = env->FindClass("com/example/whr/facedetection/JSurfaceTexture");
    if (clazz == nullptr) {
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
            env->ThrowNew(env->FindClass("java/lang/Exception"), "JNI Exception!");
        }
        return -1;
    }

    if (env->RegisterNatives(clazz, gMethods, NELM(gMethods)) < 0) {
        return -1;
    }

    return 0;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv *env = NULL;
    jclass clazz = NULL;
    jint result = -1;

    if (g_JVM != NULL)
        return JNI_VERSION_1_4;

    g_JVM = vm;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "GetEnv failed");
        goto exit;
    }

    if (register_com_example_whr_jsurfaceTexture(env) < 0) {
        ALOGE("native registeration RendererHelper failed!");
        goto exit;
    }

    result = JNI_VERSION_1_4;
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "JNI_OnLoad sucess");

    exit:
    return result;
}

