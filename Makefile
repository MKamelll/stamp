FLAGS=-Wall -Wextra -pedantic

stamp: main.cpp
	mkdir -p build
	$(CXX) $(FLAGS) $^ -o build/$@