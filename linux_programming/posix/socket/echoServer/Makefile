include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
INCLUDE := -I../../
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS += -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := -lutils_main -lutils -pthread


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

custom:$(OUT_DIR)/echoServer $(OUT_DIR)/echoClient

$(OUT_DIR)/echoServer:$(OBJ_DIR)/echoserver.o
	$(build-executable)

$(OUT_DIR)/echoClient:$(OBJ_DIR)/echoclient.o
	$(build-executable)

custom-clean:
	rm -rf $(OUT_DIR)/echoServer
	rm -rf $(OUT_DIR)/echoClient
