include $(BUILD_DIR)/Header.mk

MODULE  :=
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

.PHONY:build_target
build_target:$(DIRS)

$(eval $(set-subdirs-target))

$(DIRS):utils

include $(BUILD_DIR)/Footer.mk

