include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
INCLUDE := -I../
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS += -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := -lutils_main -lutils -pthread -lrt


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

custom:$(OUT_DIR)/process

$(OUT_DIR)/process:$(OBJ_DIR)/process.o $(OBJ_DIR)/term.o $(OBJ_DIR)/thread.o  $(OBJ_DIR)/ipc.o
	$(build-executable)

custom-clean:
	rm -rf $(OUT_DIR)/process
