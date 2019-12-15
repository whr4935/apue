include $(BUILD_DIR)/Header.mk

MODULE  := ffplay
SRC_DIR :=libavcodec libavformat libavutil
DEFINE  :=
INCLUDE :=
LDFLAGS :=
LIBS    := -lSDL -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := 


$(call build-target, executable)

include $(BUILD_DIR)/Footer.mk
