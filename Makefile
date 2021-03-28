TARGET=Sudoku
CPPFLAGS=-std=c++17 -O3
all: $(TARGET)

%: %.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET)
