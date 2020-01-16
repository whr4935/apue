include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
CFLAGS  +=  -pthread
CXXFLAGS  +=
INCLUDE :=
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib" -Wl,-rpath=$(LIBS_DIR)
EXECUTABLE_LIBS := 


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

########################
custom:$(OUT_DIR)/dlopenTest

$(OUT_DIR)/dlopenTest:$(OBJ_DIR)/dlopenTest.o $(LIBS_DIR)/libdlopentest1.so $(LIBS_DIR)/libdlopentest2.so
	@echo "  BUILD   \033[1m\033[32m `basename $@` \033[0m"""
	$(CXX) $(LDFLAGS) $(EXECUTABLE_LDFLAGS) -o$@ $(filter %.o, $^) $(LIBS) $(EXECUTABLE_LIBS) -ldlopentest1 -ldlopentest2 -ldl

$(LIBS_DIR)/libdlopentest1.so:$(OBJ_DIR)/dlopentest1.o
	$(Q)echo "  LINK    \033[1m\033[32m`basename $@`\033[0m"
	$(Q)$(CXX) -shared $(LDFLAGS) -o$@ $^ $(LIBS)

$(LIBS_DIR)/libdlopentest2.so:$(OBJ_DIR)/dlopentest2.o
	$(Q)echo "  LINK    \033[1m\033[32m`basename $@`\033[0m"
	$(Q)$(CXX) -shared $(LDFLAGS) -o$@ $^ $(LIBS)

custom-clean:
	rm -rf $(OUT_DIR)/dlopenTest
	rm -rf $(LIBS_DIR)/libdlopentest1.so
	rm -rf $(LIBS_DIR)/libdlopentest2.so



