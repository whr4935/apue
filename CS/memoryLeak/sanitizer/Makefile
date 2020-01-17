include $(BUILD_DIR)/Header.mk

MODULE  := test_sanitizer
SRC_DIR :=
DEFINE  :=
#CXXFLAGS += 
INCLUDE :=
LDFLAGS := -fsanitize=leak  #only useful for link
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := 


$(call build-target, executable)

include $(BUILD_DIR)/Footer.mk
