include $(BUILD_DIR)/Header.mk

MODULE  :=  
SRC_DIR := 
DEFINE  :=
INCLUDE := 
LDFLAGS :=  -pthread
LIBS    := -L$(LIBS_DIR)/ -lrenderer -lasound


$(call build-target, executable)
$(BUILD_DIR)/testAlsaPlay:$(LIBS_DIR)/librenderer.a

include $(BUILD_DIR)/Footer.mk
