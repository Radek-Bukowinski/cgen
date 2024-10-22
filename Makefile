CXX      = clang++
CXXFLAGS = -std=c++2b -Wall
SRC      = ./src/main.cpp
OUT      = ./bin/out

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)

.PHONY: all clean
