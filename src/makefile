
.SUFFIXES: .cpp .o


CC = g++-fsf-4.7
OBJ = main.o cpu.o
INC =
DYLIB = -lncurses
CFLAGS =-std=c++11  -g
%.o: %.cpp
	$(CC) $(INC) $(CFLAGS) -c $<
all: dcpu16emu
dcpu16emu: $(OBJ)
	$(CC) $(OBJ) $(DYLIB) -o dcpu16emu

main.o: cpuExterns.hpp DCPU16Exception.hpp opcodes.hpp types.hpp
cpu.o: cpu.hpp DCPU16Exception.hpp opcodes.hpp types.hpp
clean:
	rm -f tests/*.o *.o core
