include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
INCLUDE := -I../
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS += -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := -lutils_main -lutils -pthread


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

custom:$(OUT_DIR)/printer

$(OUT_DIR)/printer:$(OBJ_DIR)/print.o $(OBJ_DIR)/printd.o $(OBJ_DIR)/util.o $(OBJ_DIR)/print_test.o
	$(build-executable)

custom-clean:
	rm -rf $(OUT_DIR)/printer
