BUILD = ./build.out
SRC = ./src

COMPILER = g++
CXXFLAGS = -Wno-long-long -O0 -ggdb

all: clean compile

compile: 
	$(COMPILER) $(SRC)/*.cpp $(CXXFLAGS) -o $(BUILD)

mem: 
	valgrind $(BUILD)

run: 
	$(BUILD)

#Support
count:
	wc -l $(SRC)/*.cpp

countall:
	wc -l $(SRC)/*.cpp $(SRC)/*.h

clean: clearbin

clearbin:
	rm -fr $(BIN)

hello:
	echo "Makefile Check"