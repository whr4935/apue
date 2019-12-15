include $(BUILD_DIR)/Header.mk

MODULE  :=  testSurface
SRC_DIR := 
DEFINE  :=
INCLUDE := 
LDFLAGS :=  -pthread
LIBS    := -L$(LIBS_DIR) -lrenderer -lGLESv2 -lEGL -lX11 


$(call build-target, executable)

$(BUILD_DIR)/$(MODULE):$(LIBS_DIR)/librenderer.a

include $(BUILD_DIR)/Footer.mk
