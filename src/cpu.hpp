#pragma once
#include "DCPU16Exception.hpp"
#include "opcodes.hpp"
#include "types.hpp"
#include <array>
#include <unordered_map>
#include <functional>


#define RAM_SIZE 65536



namespace CPU{  

    std::array<dcpu_word, RAM_SIZE> RAM;

    dcpu_word A, B, C, X, Y, Z, I, J; //registers
    dcpu_word programCounter = 0, stackPointer = 0, interruptAddress = 0, excess = 0;

    const std::unordered_map<int, OpCode> basicOpCodeFunctionMap({
            {SET, OpCode(1, [](dcpu_word &b, dcpu_word a){b = a;})},
            
            {ADD, OpCode(2, [&](dcpu_word &b, dcpu_word a){
            excess = int(b + a) >> 16;
            b += a;
            })}

            });

    


    void runNextInstruction();
    size_t loadProgramIntoRAM(char *);



}
