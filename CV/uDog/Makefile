TARGET := silentdream
BUILD_DIR := build

#add source directories here
SRC_DIR := src 
INC_DIR := -Isrc/include  -Isrc/modules
CFLAGS := -g -Werror -std=c99 $(INC_DIR)
CXXFLAGS:= -g -Werror -std=c++11 $(INC_DIR)
LDFLAGS:= -pthread

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)
SRC := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
SRC += $(wildcard *.c)
SRC += $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
SRC += $(wildcard *.cpp)

OBJ := $(SRC:.c=.o)
OBJ := $(OBJ:.cpp=.o)
OBJ := $(addprefix $(BUILD_DIR)/, $(notdir $(OBJ)))

all:$(BUILD_DIR)/$(TARGET)

-include $(OBJ:.o=.dep)
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET):$(OBJ)
	g++ -o $@ $(LDFLAGS) $^

$(BUILD_DIR)/%.o:%.c |$(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BUILD_DIR)/%.o:%.cpp |$(BUILD_DIR)
	g++ $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

clean:
	-rm -rf $(BUILD_DIR)

test:
	@echo $(OBJ)

kill:
	-@kill -9 `ps aux|grep '$(TARGET)$$'|grep -v grep|awk '{print $$2}'`

install:
	cp $(BUILD_DIR)/$(TARGET) /usr/local/bin
	if [ ! -d /etc/silentdream ];then \
	    mkdir /etc/silentdream; \
    fi
	cp conf/* /etc/silentdream -rf

uninstall:
	rm /usr/local/bin/$(TARGET)
