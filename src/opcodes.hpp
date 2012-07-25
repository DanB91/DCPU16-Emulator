#pragma once
#include <functional>
#include "types.hpp"
#include "DCPU16Exception.hpp"


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


#define SET 0x1
#define ADD 0x2
}
