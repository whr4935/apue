TARGET := threadpool
CXXFLAGS := -std=c++11

all:$(TARGET)

-include testThreadPool.d

$(TARGET):testThreadPool.o
	$(CXX) -o $@ $^ -pthread -lgtest -lgtest_main

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< -MMD -MF $(@:.o=.d)

clean:
	rm $(TARGET) testThreadPool.o testThreadPool.d
