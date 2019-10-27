TARGET := cpp_bin
OUT_DIR := out
SRC_DIR := $(shell find . -maxdepth 1 -type d)
SRC_DIR := $(basename $(patsubst ./%, %, $(SRC_DIR)))
SRC_DIR := $(filter-out $(OUT_DIR) include, $(SRC_DIR))
$(info SRC_DIR=$(SRC_DIR))
CXXFLAGS:=-Iinclude -std=c++11 -g
LDFLAGS:= -pthread

vpath %.cpp $(SRC_DIR)
SRC := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
SRC += $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
OBJ := $(addprefix $(OUT_DIR)/, $(notdir $(OBJ)))



all:$(OUT_DIR)/$(TARGET) 

-include $(OBJ:.o=.dep)

$(OUT_DIR):
	mkdir $(OUT_DIR)


$(OUT_DIR)/$(TARGET):$(OBJ)
	@echo build $(TARGET)
	@g++ -o $@ $(LDFLAGS) $^


$(OUT_DIR)/%.o:%.cpp |$(OUT_DIR)
	g++ $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

clean:
	-rm -f $(OUT_DIR) -r

test:
	@echo $(SRC)
