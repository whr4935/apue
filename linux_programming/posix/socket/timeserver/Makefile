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

custom:$(OUT_DIR)/timeserver $(OUT_DIR)/timeclient

$(OUT_DIR)/timeserver:$(OBJ_DIR)/timeserver.o
	$(build-executable)

$(OUT_DIR)/timeclient:$(OBJ_DIR)/timeclient.o
	$(build-executable)

custom-clean:
	rm -rf $(OUT_DIR)/timeserver
	rm -rf $(OUT_DIR)/timeclient
