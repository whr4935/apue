include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
INCLUDE := -I../
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS += -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := -lutils_main -lutils -pthread


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

custom:$(OUT_DIR)/misc $(OUT_DIR)/test_getopt

$(OUT_DIR)/misc:$(OBJ_DIR)/crc.o $(OBJ_DIR)/misc.o
	$(build-executable)

$(OUT_DIR)/test_getopt:$(OBJ_DIR)/test_getopt.o
	$(Q)$(CXX) $(LDFLAGS) $(EXECUTABLE_LDFLAGS) -o$@ $(filter %.o, $^) $(LIBS)
	@echo "  BUILD   \033[1m\033[32m `basename $@` \033[0m"

custom-clean:
	rm -rf $(OUT_DIR)/misc
	rm -rf $(OUT_DIR)/test_getopt
