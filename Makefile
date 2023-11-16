FLAGS=-Wall -Wextra -pedantic -std=c++17

stamp: main.cpp
	mkdir -p build
	$(CXX) $(FLAGS) $^ -o build/$@