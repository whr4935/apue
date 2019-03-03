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

########################
custom:$(OUT_DIR)/a $(OUT_DIR)/b

$(OUT_DIR)/a:$(OBJ_DIR)/a.o
	$(build_exectuable)

$(OUT_DIR)/b:$(OBJ_DIR)/b.o
	$(build_exectuable)

custom-clean:
	rm -rf $(OUT_DIR)/a
	rm -rf $(OUT_DIR)/b
