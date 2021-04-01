TARGET=Sudoku
<<<<<<< HEAD
CPPFLAGS=-std=c++17 -O3
all: $(TARGET)

%: %.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@
=======

all: $(TARGET)

%: %.cpp
	$(CXX) -o $@ $^
>>>>>>> e91fb5f5f8a6570de267cd95ec34bfbe11e3058c

.PHONY: clean
clean:
	$(RM) $(TARGET)
<<<<<<< HEAD
=======

>>>>>>> e91fb5f5f8a6570de267cd95ec34bfbe11e3058c
