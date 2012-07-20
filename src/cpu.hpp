#pragma once
#include "DCPU16Exception.hpp"
#include "opcodes.hpp"
#include <array>
#include <unordered_map>
#include <functional>


#define RAM_SIZE 65536


typedef uint_least16_t udcpu_word;
typedef int_least16_t dcpu_word;

namespace CPU{

    std::array<udcpu_word, RAM_SIZE> RAM;

    dcpu_word A, B, C, X, Y, Z, I, J; //registers
    udcpu_word programCounter, stackPointer, interruptAddress, excess;


    std::unordered_map<udcpu_word, std::function<void(dcpu_word&, dcpu_word)>> basicOpCodeFunctionMap({
            {SET, [](dcpu_word &b, dcpu_word a){b = a;}},
            {ADD, [&](dcpu_word &b, dcpu_word a){
                excess = int(b + a) >> 4;
                b += a;
            }}

    });





}
