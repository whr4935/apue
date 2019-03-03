#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_whr_facedetection_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    Mat m =imread("bike.jpg");

    return env->NewStringUTF(hello.c_str());
}
