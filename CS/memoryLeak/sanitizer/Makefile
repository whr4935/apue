include $(BUILD_DIR)/Header.mk

MODULE  := test_sanitizer
SRC_DIR :=
DEFINE  :=
CXXFLAGS += -fsanitize=leak
INCLUDE :=
LDFLAGS := -fsanitize=leak
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := 


$(call build-target, executable)

include $(BUILD_DIR)/Footer.mk
