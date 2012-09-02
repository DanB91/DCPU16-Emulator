
.SUFFIXES: .cpp .o

PRECFLAGS = -g -std=c++11
PREDYLIB = -lncurses 


ifeq ($(shell uname),Darwin)
	CFLAGS = $(PRECFLAGS) -stdlib=libc++
	DYLIB = $(PREDYLIB) -stdlib=libc++
else
	CFLAGS = $(PRECFLAGS)
	DYLIB = $(PREDYLIB) 

endif

CC = clang++ 
OBJ = main.o cpu.o
INC =
%.o: %.cpp
	$(CC) $(INC) $(CFLAGS) -c $<
all: dcpu16emu
dcpu16emu: $(OBJ)
	$(CC) $(OBJ) $(DYLIB) -o dcpu16emu

main.o: cpuExterns.hpp DCPU16Exception.hpp opcodes.hpp types.hpp
cpu.o: cpu.hpp DCPU16Exception.hpp opcodes.hpp types.hpp
clean:
	rm -f tests/*.o *.o core
