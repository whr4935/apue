CC := gcc
CXX := g++
AR := ar
CFLAGS := -g -ICommon
CXXFLAGS := -g -ICommon
LIBS := -L. -lmyes -lGLESv2 -lEGL -lm -lX11

libmyes := libmyes.a

SRC_DIR :=$(shell find . -maxdepth 1 -type d)
SRC_DIR := $(patsubst ./%, %, $(SRC_DIR))
SRC_DIR := $(filter-out Common out, $(SRC_DIR))
SRC :=$(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
SRC +=$(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
TARGET := $(addprefix out/, $(notdir $(basename $(SRC))))

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

all:out $(libmyes) $(TARGET)

out:
	@mkdir $@

out/%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

out/%:%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

#################################
libmyes_src :=$(wildcard Common/*.c)
libmyes_obj :=$(libmyes_src:.c=.o)
	
$(libmyes):$(libmyes_obj)
	$(AR) -r $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -rf $(TARGET)

distclean:
	-rm -rf $(libmyes_obj) $(libmyes) $(TARGET)

