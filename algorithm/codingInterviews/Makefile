MAKE := make --no-print-directory
CC := gcc
CXX := g++
CFLAGS:= -I. -g -std=c99 -Wall -Werror -pthread
CXXFLAGS := -I. -g -std=c++11 -Wall -Werror -pthread
MY_OUT_DIR:=out

LIBS := $(MY_OUT_DIR)/libUtilities.a

OS_VER := Linux
ifeq ($(OS), Windows_NT)
	OS_VER := Win
endif


SRC_DIR :=
ifeq ($(OS_VER), Linux)
SRC_DIR := $(shell find . -maxdepth 1 -type d)
else
SHELL = cmd
SRC_DIR := $(shell dir /ad /b)
endif

FilterOut := Utilities $(MY_OUT_DIR) .git scripts
SRC_DIR := $(filter-out $(FilterOut), $(basename $(patsubst ./%,%,$(SRC_DIR))))
SRC_DIR += .

SRC := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
SRC += $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))

MY_TARGET := $(addprefix $(MY_OUT_DIR)/, $(basename $(notdir $(patsubst ./%, %, $(SRC)))))

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

build_target all:$(MY_OUT_DIR) $(LIBS) $(MY_TARGET)

$(MY_OUT_DIR):
	@mkdir $@

$(LIBS):force
	@$(MAKE) -C Utilities MY_OUT_DIR=$(MY_OUT_DIR)

$(MY_TARGET):$(LIBS)

-include $(addsuffix .dep, $(MY_TARGET))

$(MY_OUT_DIR)/%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -MD -MF $(@:=.dep)

$(MY_OUT_DIR)/%:%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS) -MD -MF $(@:=.dep)

clean:
	-rm -rf $(MY_OUT_DIR)

test:
	@echo $(SRC_DIR)

force:;

.PHONY:all clean force
