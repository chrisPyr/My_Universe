CXX=g++
CXXFLAGS=-std=c++17 -O3 -Wall -Wextra -fPIC -shared -m64 -fopenmp -g
INCLUDE=-Iinclude/
PYTHON=python3
SRC=lib/*


.PHONY:clean all test target
all:target


test:target
	python3 -m pytest -v -s


target: $(SRC).cpp
		$(CXX) $(CXXFLAGS) $(INCLUDE) `$(PYTHON) -m pybind11 --includes` $^ -o _system`$(PYTHON)-config --extension-suffix` -I /usr/include/python3.8
clean:
		$(RM) -r $(TARGET) *.so *.txt __pycache__* ../__pycache__*
