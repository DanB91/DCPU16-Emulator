
.SUFFIXES: .cpp .o


CC = g++-fsf-4.7
OBJ = main.o
INC =
DYLIB =   
CFLAGS =-std=c++11  -g
%.o: %.cpp
	$(CC) $(INC) $(CFLAGS) -c $<
all: dcpu16emu
dcpu16emu: $(OBJ)
	$(CC) $(OBJ) $(DYLIB) -o dcpu16emu

main.o: cpu.hpp DCPU16Exception.hpp opcodes.hpp
clean:
	rm -f tests/*.o *.o core
