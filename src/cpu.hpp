#pragma once
#include "DCPU16Exception.hpp"
#include "opcodes.hpp"
#include "types.hpp"
#include <array>
#include <unordered_map>
#include <functional>





namespace CPU{  

    const int RamSize = 65536;

    std::array<dcpu_word, RamSize> RAM;

    dcpu_word A, B, C, X, Y, Z, I, J; //registers
    dcpu_word programCounter = 0, stackPointer = 0, interruptAddress = 0, excess = 0;

    unsigned int totalCycles = 0;
    unsigned int cyclesSinceLastInstruction = 0;

    const int clockSpeed = 10000;
    
        


   void startExecutionOfProgram(char*); 


}
