CXXFLAGS += -Wall -Wextra -std=c++14

-include main.d

main: main.cpp
	$(CXX) $^ -o $@