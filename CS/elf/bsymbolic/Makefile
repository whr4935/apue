include $(BUILD_DIR)/Header.mk

MODULE  := 
SRC_DIR :=
DEFINE  :=
CFLAGS  +=  -pthread
CXXFLAGS  +=
INCLUDE :=
LDFLAGS :=
#LDFLAGS := -Wl,-Bsymbolic
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := 


$(call build-target, custom)

include $(BUILD_DIR)/Footer.mk

########################
custom:$(OUT_DIR)/bsymbolic_main $(LIBS_DIR)/libbsymbolic_so.so $(LIBS_DIR)/libbsymbolic_so.a

$(OUT_DIR)/bsymbolic_main: $(OBJ_DIR)/bsymbolic_main.o $(LIBS_DIR)/libbsymbolic_liveplayer.so
	@echo "  BUILD   \033[1m\033[32m `basename $@` \033[0m"""
	$(CXX) $(LDFLAGS) $(EXECUTABLE_LDFLAGS) -o$@ $(filter %.o, $^) $(LIBS) $(EXECUTABLE_LIBS) -lbsymbolic_liveplayer -ldl

$(LIBS_DIR)/libbsymbolic_liveplayer.so:$(OBJ_DIR)/bsymbolic_liveplayer.o $(LIBS_DIR)/libbsymbolic_so.a
	$(Q)$(CXX) -shared $(LDFLAGS) -o$@ $^ $(LIBS) -Wl,--version-script=bsymbolic_liveplayer.map.txt

$(LIBS_DIR)/libbsymbolic_so.so:$(OBJ_DIR)/bsymbolic_so.o
	$(Q)$(CXX) -shared $(LDFLAGS) -o$@ $^ $(LIBS)

$(LIBS_DIR)/libbsymbolic_so.a:$(OBJ_DIR)/bsymbolic_so.o
	$(Q)$(AR) -r $@ $^

custom-clean:
	rm -rf $(OUT_DIR)/bsymbolic_main



