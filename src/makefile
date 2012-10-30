
.SUFFIXES: .cpp .o

PRECFLAGS = -g -std=c++0x 
PREDYLIB = -lncurses -pthread

ifeq ($(shell uname),Darwin)
	CFLAGS = $(PRECFLAGS) -stdlib=libc++
	DYLIB = $(PREDYLIB) -stdlib=libc++
	CC = clang++
else
	CFLAGS = $(PRECFLAGS)
	DYLIB = $(PREDYLIB) 
	CC = g++
endif

OBJ = main.o cpu.o
INC =
%.o: %.cpp
	$(CC) $(INC) $(CFLAGS) -c $<
all: dcpu16emu
test: dcpu16emu
	./dcpu16emu examples/test.o

dcpu16emu: $(OBJ)
	$(CC) $(OBJ) $(DYLIB) -o dcpu16emu

main.o: cpu.hpp DCPU16Exception.hpp types.hpp
cpu.o: cpu.hpp DCPU16Exception.hpp types.hpp
clean:
	rm -f tests/*.o *.o core
