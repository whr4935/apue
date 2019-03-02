MAKE := make --no-print-directory
CC := gcc
CXX := g++
CFLAGS:= -I. -g -std=c99 -Wall -pthread
CXXFLAGS := -I. -g -std=c++11 -Wall -pthread

LIBS := 

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

FilterOut := Utilities out .git scripts
SRC_DIR := $(filter-out $(FilterOut), $(basename $(patsubst ./%,%,$(SRC_DIR))))
SRC_DIR += .

SRC := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
SRC += $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))

TARGET := $(addprefix out/, $(basename $(notdir $(patsubst ./%, %, $(SRC)))))

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

all:out $(LIBS) $(TARGET)

out:
		@mkdir $@

$(LIBS):force
		@$(MAKE) -C Utilities

$(TARGET):$(LIBS)

-include $(addsuffix .dep, $(TARGET))

out/%:%.c
		$(CC) $(CFLAGS) -o $@ $< $(LIBS) -MD -MF $(@:=.dep)

out/%:%.cpp
		$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS) -MD -MF $(@:=.dep)

clean:
		-rm -rf out

test:
		@echo $(SRC_DIR)

force:;

.PHONY:all clean force

