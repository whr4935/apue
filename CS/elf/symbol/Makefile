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
custom:$(OUT_DIR)/simpleSection $(OUT_DIR)/simpleSection_v2 \
	$(OUT_DIR)/ManualNameMangling $(OUT_DIR)/pt $(OUT_DIR)/pt_multithread

$(OUT_DIR)/simpleSection:$(OBJ_DIR)/simpleSection.o $(OBJ_DIR)/test_simpleSection.o
	$(build-executable)

$(OUT_DIR)/simpleSection_v2:$(OBJ_DIR)/test_simpleSection_v2.o $(LIBS_DIR)/libsymbol_v2.so
	@echo "  BUILD   \033[1m\033[32m `basename $@` \033[0m"""
	$(CXX) $(LDFLAGS) $(EXECUTABLE_LDFLAGS) -o$@ $(filter %.o, $^) $(LIBS) $(EXECUTABLE_LIBS) -lsymbol_v2 -ldl

$(LIBS_DIR)/libsymbol_v2.so:$(OBJ_DIR)/simpleSection.o
	$(Q)$(CXX) -shared $(LDFLAGS) -o$@ $^ $(LIBS)

$(OUT_DIR)/ManualNameMangling:$(OBJ_DIR)/ManualNameMangling.o
	$(build-executable)

$(OUT_DIR)/pt:$(OBJ_DIR)/thread.o
	$(build-executable)

$(OUT_DIR)/pt_multithread:$(OBJ_DIR)/thread.o
	$(CXX) $(LDFLAGS) $(EXECUTABLE_LDFLAGS) -o$@ $^ -pthread $(LIBS) $(EXECUTABLE_LIBS)
	@echo "  BUILD   \033[1m\033[32m `basename $@` \033[0m"""

custom-clean:
	rm -rf $(OUT_DIR)/simpleSection
	rm -rf $(OUT_DIR)/ManualNamgling
	rm -rf $(OUT_DIR)/pt
	rm -rf $(OUT_DIR)/pt_multithread



