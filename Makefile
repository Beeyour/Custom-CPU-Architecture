CXX      = g++
CXXFLAGS = -std=c++17 -Wall

SRC = software/assembler/assembler.cpp
ASM = programs/program.asm
HEX = programs/memory.hex
BIN = bin/assembler

.PHONY: all clean

all: $(HEX)


$(BIN): $(SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)


$(HEX): $(BIN) $(ASM)
	./$(BIN) $(ASM) -o $(HEX)

clean:
	rm -rf bin $(HEX)