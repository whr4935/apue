include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
CFLAGS  +=  -pthread
CXXFLAGS  +=
INCLUDE :=
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := 


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

########################
custom:$(OUT_DIR)/simpleSection $(OUT_DIR)/ManualNameMangling $(OUT_DIR)/pt $(OUT_DIR)/pt_multithread

$(OUT_DIR)/simpleSection:$(OBJ_DIR)/simpleSection.o
	$(build_exectuable)

$(OUT_DIR)/ManualNameMangling:$(OBJ_DIR)/ManualNameMangling.o
	$(build_exectuable)

$(OUT_DIR)/pt:$(OBJ_DIR)/thread.o
	$(build_exectuable)

$(OUT_DIR)/pt_multithread:$(OBJ_DIR)/thread.o
	$(CXX) $(LDFLAGS) $(EXECUTABLE_LDFLAGS) -o$@ $^ -pthread $(LIBS) $(EXECUTABLE_LIBS)
	@echo "  BUILD   \033[1m\033[32m `basename $@` \033[0m"""

custom-clean:
	rm -rf $(OUT_DIR)/simpleSection
	rm -rf $(OUT_DIR)/ManualNamgling
	rm -rf $(OUT_DIR)/pt
	rm -rf $(OUT_DIR)/pt_multithread



