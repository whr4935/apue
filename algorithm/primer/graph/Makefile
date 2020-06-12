include $(BUILD_DIR)/Header.mk

MODULE  := graph
SRC_DIR :=
DEFINE  :=
INCLUDE := -I../
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS += 
EXECUTABLE_LIBS := -lutils  -lutils_main


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk
########################
LEGACY_SEARCH_OBJ := 
CUSTOM_OBJ := $(filter-out $(LEGACY_SEARCH_OBJ), $(SRC_OBJ))
custom_target := $(basename $(addprefix $(OUT_DIR)/, $(notdir $(CUSTOM_OBJ))))
custom:$(custom_target)

$(OUT_DIR)/%:$(OBJ_DIR)/%.o
	$(build-executable)

custom-clean:
	rm -rf $(custom_target)
