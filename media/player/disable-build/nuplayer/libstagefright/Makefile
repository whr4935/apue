include $(BUILD_DIR)/Header.mk

LOCAL_PATH := $(shell pwd)
$(info $(LOCAL_PATH))

MODULE  :=  stagefright
SRC_DIR := httplive id3 mpeg2ts rtsp
CXXFLAGS += -Wno-multichar
DEFINE  := -D__unused=
INCLUDE := -I$(LOCAL_PATH)/../include -I/usr/local/include/monkeyKing/ \
	-I$(LOCAL_PATH)/include \
	-I$(LOCAL_PATH)/mpeg2ts/ \
	-I$(LOCAL_PATH)/ \

LDFLAGS := 
LIBS    := 


$(call build-target, static_library)


include $(BUILD_DIR)/Footer.mk
