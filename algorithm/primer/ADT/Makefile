include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
INCLUDE := -I../utils
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS += 
EXECUTABLE_LIBS := -lutils 


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

########################
my-target:=$(OUT_DIR)/list $(OUT_DIR)/skiplist
custom:$(my-target)

$(OUT_DIR)/list:$(OBJ_DIR)/list.o $(LIBS_DIR)/libutils.a
	$(build_exectuable)


$(OUT_DIR)/skiplist:$(OBJ_DIR)/skiplist.o $(LIBS_DIR)/libutils.a
	$(build_exectuable)

custom-clean:
	rm -rf $(my-target)
