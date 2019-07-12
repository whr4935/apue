include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
INCLUDE :=
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := 


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

custom:$(OUT_DIR)/h_enc $(OUT_DIR)/h_dec


$(OUT_DIR)/h_enc:$(OBJ_DIR)/h_enc.o
	$(build-executable)


$(OUT_DIR)/h_dec:$(OBJ_DIR)/h_dec.o
	$(build-executable)

custom-clean:
	-rm -rf $(OUT_DIR)/h_enc
	-rm -rf $(OUT_DIR)/h_dec
