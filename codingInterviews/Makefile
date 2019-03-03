MAKE := make --no-print-directory
CC := gcc
CXX := g++
CFLAGS:= -I. -g -std=c99 -Wall -Werror -pthread
CXXFLAGS := -I. -g -std=c++11 -Wall -Werror -pthread

LIBS := build/libUtilities.a

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

FilterOut := Utilities build .git scripts
SRC_DIR := $(filter-out $(FilterOut), $(basename $(patsubst ./%,%,$(SRC_DIR))))
SRC_DIR += .

SRC := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
SRC += $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))

TARGET := $(addprefix build/, $(basename $(notdir $(patsubst ./%, %, $(SRC)))))

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

all:build $(LIBS) $(TARGET)

build:
	@mkdir build

$(LIBS):force
	@$(MAKE) -C Utilities

$(TARGET):$(LIBS)

-include $(addsuffix .dep, $(TARGET))

build/%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -MD -MF $(@:=.dep)

build/%:%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS) -MD -MF $(@:=.dep)

clean:
	-rm -rf build

test:
	@echo $(SRC_DIR)

force:;

.PHONY:all clean force
