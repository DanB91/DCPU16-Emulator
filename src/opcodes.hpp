#pragma once
#include <functional>
#include <unordered_map>
#include "types.hpp"
#include "DCPU16Exception.hpp"
#include "cpuExterns.hpp"


namespace CPU{

    struct OpCode{

        const int cycleCount;
        std::function<void(dcpu_word&, dcpu_word)> opCodeFunction;


        OpCode(int cycleCount,  std::function<void(dcpu_word&, dcpu_word)> opCodeFunction)
            :cycleCount(cycleCount), opCodeFunction(opCodeFunction)
        {}
        OpCode(): cycleCount(-1){throw DCPU16Exception("Don't use this constructor");}

        void operator()(dcpu_word& b, dcpu_word a) const{ opCodeFunction(b, a);}
    };



    const std::unordered_map<int, OpCode> basicOpCodeFunctionMap({
            {0x1, OpCode(1, [](dcpu_word &b, dcpu_word a){b = a;})}, //set

            {0x2, OpCode(2, [&](dcpu_word &b, dcpu_word a){ //add
                int result = int(b) + int(a);
                excess = result >> 16;
                b = result;
                })},

            {0x3, OpCode(2, [&](dcpu_word &b, dcpu_word a){ //subtract
                int result = int(b) - int(a);
                excess = result >> 16;
                b = result;
                })},

            {0x4, OpCode(2, [&](dcpu_word &b, dcpu_word a){ //unsigned multiplication
                unsigned int result =  (unsigned int)(b) * (unsigned int)(a);
                excess = result >> 16;
                b = result;
                })},

            {0x5, OpCode(2, [&](dcpu_word &b, dcpu_word a){//signed multiplication
                    int result = int(b) * int(a);
                    excess = result >> 16;
                    b = result;

                    })},

            {0x6, OpCode(3, [&](dcpu_word &b, dcpu_word a){//unsigned division 
                    if(a == 0){
                    b = 0;
                    excess = 0;
                    }
                    else{

                    excess =  ((unsigned int)(b) << 16) / (unsigned int)(a) ;
                    b /= a;
                    }

                    })},

            {0x7, OpCode(3, [&](dcpu_word &b, dcpu_word a){ //signed division

                    if(a == 0){

                    b = 0;
                    excess = 0;
                    }
                    else{

                    excess =  (int(b) << 16) / int(a) ;
                    b = short(b) / short(a);
                    }

                    })},

            {0x8, OpCode(3, [](dcpu_word &b, dcpu_word a){ //unsigned modulous

                    if(a == 0)
                    b = 0;
                    else
                    b %= a;
                    })},

            {0x9, OpCode(3, [](dcpu_word &b, dcpu_word a){ //signed modulous

                    if(a == 0)
                    b = 0;
                    else
                    b = short(b) % short(a);
                    })},

            {0xA, OpCode(1, [](dcpu_word &b, dcpu_word a){b &= a;})}, //and

            {0xB, OpCode(1, [](dcpu_word &b, dcpu_word a){b |= a;})}, //or

            {0xC, OpCode(1, [](dcpu_word &b, dcpu_word a){b ^= a;})}, //exclusive or

            {0xD, OpCode(1, [&](dcpu_word &b, dcpu_word a){ //logical right shift

                    excess = short(b << 16) >> a;
                    b >>= a;

                    })},

            {0xE, OpCode(1, [&](dcpu_word &b, dcpu_word a){ //arithmetic right shift

                    excess = (b << 16) >> a;
                    b = short(b) >> a;

                    })},

            {0xF, OpCode(1, [&](dcpu_word &b, dcpu_word a){ //logical left shift

                    excess = short(b << 16) >> a;
                    b <<= a;

                    })},

            {0x10, OpCode(2, [&](dcpu_word &b, dcpu_word a){            
                    if(!(b&a)){

                    while((RAM[programCounter] & 0x1F) < 0x17 && (RAM[programCounter] & 0x1F) > 0x10){
                    programCounter++;
                    cyclesSinceLastInstruction++;
                    }

                    programCounter++;
                    cyclesSinceLastInstruction++;
                    }


                    })},

            {0x11, OpCode(2, [&](dcpu_word &b, dcpu_word a){            
                    if(b&a){

                    while((RAM[programCounter] & 0x1F) < 0x17 && (RAM[programCounter] & 0x1F) > 0x10){
                    programCounter++;
                    cyclesSinceLastInstruction++;
                    }

                    programCounter++;
                    cyclesSinceLastInstruction++;
                    }


                    })}, 
            {0x12, OpCode(2, [&](dcpu_word &b, dcpu_word a){            
                    if(b!=a){

                    while((RAM[programCounter] & 0x1F) < 0x17 && (RAM[programCounter] & 0x1F) > 0x10){
                    programCounter++;
                    cyclesSinceLastInstruction++;
                    }

                    programCounter++;
                    cyclesSinceLastInstruction++;
                    }


                    })}, 
            
            {0x13, OpCode(2, [&](dcpu_word &b, dcpu_word a){            
                    if(b==a){

                    while((RAM[programCounter] & 0x1F) < 0x17 && (RAM[programCounter] & 0x1F) > 0x10){
                    programCounter++;
                    cyclesSinceLastInstruction++;
                    }

                    programCounter++;
                    cyclesSinceLastInstruction++;
                    }


                    })}

            









    });


}
