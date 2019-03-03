LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# comment this line disable opencv
# USE_OPENCV=1

ifdef USE_OPENCV
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
include $(LOCAL_PATH)/../opencv3.x/jni/OpenCV.mk
endif

LOCAL_SRC_FILES:= \
   faceDetection.cpp \
   detectorBase.cpp \
   cascadeDetector.cpp \
   genderclassifier.cpp \
   ageclassifier.cpp


LOCAL_C_INCLUDES += 
ifdef USE_OPENCV
LOCAL_CFLAGS += -DUSE_OPENCV
endif

LOCAL_LDLIBS +=-llog

LOCAL_MODULE:= libfaceDetection


# include $(BUILD_SHARED_LIBRARY)



