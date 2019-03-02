TARGET := cpp_bin
BUILD_DIR := build
SRC_DIR := class expression func misc std_lib template
CXXFLAGS:=-Iinclude -std=c++11 -g
LDFLAGS:= -pthread

vpath %.cpp $(SRC_DIR)
SRC := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp))
SRC += $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
OBJ := $(addprefix $(BUILD_DIR)/, $(notdir $(OBJ)))



all:$(BUILD_DIR)/$(TARGET) 

-include $(OBJ:.o=.dep)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)


$(BUILD_DIR)/$(TARGET):$(OBJ)
	@echo build $(TARGET)
	@g++ -o $@ $(LDFLAGS) $^


$(BUILD_DIR)/%.o:%.cpp |$(BUILD_DIR)
	g++ $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

clean:
	-rm -f $(BUILD_DIR) -r

test:
	@echo $(SRC)
