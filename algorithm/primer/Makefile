include $(BUILD_DIR)/Header.mk

MODULE  :=  libalgorithm.a
SRC_DIR :=
DEFINE  :=
INCLUDE :=
LDFLAGS :=
LIBS    := -L$(LIBS_DIR)
EXECUTABLE_LDFLAGS := -Wl,-rpath="out/lib"
EXECUTABLE_LIBS := 

#$(call build-target, custom)
$(eval $(cur-subdirs))
DIRS:=$(subdirs)
DIRS:=$(filter-out utils, $(DIRS))

libs := $(addprefix $(LIBS_DIR)/lib, $(DIRS))
libs := $(addsuffix .a, $(libs))
#$(info $(subdirs))
#$(info $(libs))

.PHONY:build_target
build_target:$(LIBS_DIR)/$(MODULE)

$(LIBS_DIR)/$(MODULE):$(subdirs)
	$(AR) -r $(LIBS_DIR)/$(MODULE) $(libs)

$(eval $(set-subdirs-target))

$(DIRS):utils

include $(BUILD_DIR)/Footer.mk

