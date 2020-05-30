include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
INCLUDE := -I../
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS += 
EXECUTABLE_LIBS := -lutils -lutils_main -lADT -lsort


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk
########################
LEGACY_SEARCH_OBJ := test_search.o elementary_search.o
LEGACY_TARGET := test_search
CUSTOM_OBJ := $(filter-out $(LEGACY_SEARCH_OBJ), $(SRC_OBJ))
custom_target := $(basename $(addprefix $(OUT_DIR)/, $(notdir $(CUSTOM_OBJ))))
custom:$(custom_target) $(OUT_DIR)/$(LEGACY_TARGET)

$(OUT_DIR)/$(LEGACY_TARGET):$(addprefix $(OBJ_DIR)/, $(LEGACY_SEARCH_OBJ)) $(LIBS_DIR)/libADT.a $(LIBS_DIR)/libsort.a
	$(build-executable)

$(OUT_DIR)/%:$(OBJ_DIR)/%.o $(LIBS_DIR)/libADT.a $(LIBS_DIR)/libsort.a
	$(build-executable)

custom-clean:
	rm -rf $(custom_target) $(OUT_DIR)/$(LEGACY_TARGET)
