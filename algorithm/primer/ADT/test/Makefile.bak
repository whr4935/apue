include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
INCLUDE := -I../../
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS += -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := -lutils_main -lutils -lADT -pthread


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

custom:$(OUT_DIR)/test_list $(OUT_DIR)/test_skiplist $(OUT_DIR)/test_fifo

$(OUT_DIR)/test_list:$(OBJ_DIR)/test_list.o
	$(build-executable)

$(OUT_DIR)/test_skiplist:$(OBJ_DIR)/test_skiplist.o
	$(build-executable)

$(OUT_DIR)/test_fifo:$(OBJ_DIR)/test_fifo.o $(LIBS_DIR)/libADT.a
	$(build-executable)

custom-clean:
	rm -rf $(OUT_DIR)/test_list
	rm -rf $(OUT_DIR)/test_skiplist
	rm -rf $(OUT_DIR)/test_fifo
