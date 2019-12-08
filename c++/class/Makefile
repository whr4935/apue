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
custom_target := $(basename $(addprefix $(OUT_DIR)/, $(notdir $(SRC_OBJ))))
custom:$(custom_target)

$(OUT_DIR)/%:$(OBJ_DIR)/%.o
	$(build-executable)

custom-clean:
	rm -rf $(custom_target)
