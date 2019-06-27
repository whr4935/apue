include $(BUILD_DIR)/Header.mk

MODULE  := threadpool
SRC_DIR :=
DEFINE  :=
INCLUDE :=
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := -pthread -lgtest -lgtest_main


$(call build-target, executable)

include $(BUILD_DIR)/Footer.mk
