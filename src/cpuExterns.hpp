#pragma once
#include <array>
#include <unordered_map>
#include <functional>
#include "DCPU16Exception.hpp"
#include "types.hpp"
#include "opcodes.hpp"


#define RAM_SIZE 65536




namespace CPU{

    extern std::array<dcpu_word, RAM_SIZE> RAM;

    extern dcpu_word A, B, C, X, Y, Z, I, J; //registers
    extern dcpu_word programCounter, stackPointer, interruptAddress, excess;


    extern const std::unordered_map<int, OpCode> basicOpCodeFunctionMap;

    void runNextInstruction();
    size_t loadProgramIntoRAM(char *);


}
