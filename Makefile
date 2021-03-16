TARGET=Sudoku

all: $(TARGET)

%: %.cpp
	$(CXX) -o $@ $^

.PHONY: clean
clean:
	$(RM) $(TARGET)

