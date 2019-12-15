include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR := nuplayer
CXXFLAGS += -Wno-multichar
DEFINE  := -D__unused=
INCLUDE := -I../include -I/usr/local/include/monkeyKing/ \
	-I../libstagefright/include \
	-I../libstagefright/mpeg2ts \
	-I../libstagefright/rtsp \
	-I../libstagefright/httplive \
	-I../libstagefright/timedtext \

LDFLAGS += 
LIBS    := -lmonkeyKing -lstagefright -lavformat -lavcodec -lavutil -lavfilter -lswresample -lswscale -lSDL

$(call build-target, executable)


include $(BUILD_DIR)/Footer.mk
