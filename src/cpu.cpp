#include "cpu.hpp"

namespace CPU{

   
    int currentCycleCount = 0;

    dcpu_word &nonLiteralValue(dcpu_word valueRepresentation, bool isFirstArgumentInInstruction)
    {
        switch(valueRepresentation)
        {
            //register
            case 0:
                return A;
            case 1:
                return B;
            case 2:
                return C;
            case 3:
                return X;
            case 4:
                return Y;
            case 5:
                return Z;
            case 6:
                return I;
            case 7:
                return J;
                //[register]
            case 8:
                return RAM[A];
            case 9:
                return RAM[B];
            case 0xA:
                return RAM[C];
            case 0xB:
                return RAM[X];
            case 0xC:
                return RAM[Y];
            case 0xD:
                return RAM[Z];
            case 0xE:
                return RAM[I];
            case 0xF:
                return RAM[J];
                //[register + next word]
            case 0x10:
                currentCycleCount++;
                return RAM[A + RAM[++programCounter]];
            case 0x11:
                currentCycleCount++;
                return RAM[B + RAM[++programCounter]];
            case 0x12:
                currentCycleCount++;
                return RAM[C + RAM[++programCounter]];
            case 0x13:
                currentCycleCount++;
                return RAM[X + RAM[++programCounter]];
            case 0x14:
                currentCycleCount++;
                return RAM[Y + RAM[++programCounter]];
            case 0x15:
                currentCycleCount++;
                return RAM[Z + RAM[++programCounter]];
            case 0x16:
                currentCycleCount++;
                return RAM[I + RAM[++programCounter]];
            case 0x17:
                currentCycleCount++;
                return RAM[J + RAM[++programCounter]];
            case 0x18:
                //push
                if(isFirstArgumentInInstruction)
                    return RAM[--stackPointer];
                //pop
                else
                    return RAM[stackPointer++];
            case 0x19:
                //[stack pointer]
                return RAM[stackPointer];
            case 0x1A:
                //pick
                currentCycleCount++;
                return RAM[stackPointer + RAM[++programCounter]];
            case 0x1B:
                return stackPointer;
            case 0x1C:
                return programCounter;
            case 0x1D:
                return excess;
            case 0x1E:
                return RAM[RAM[++programCounter]];
            case 0x1F:
                return RAM[++programCounter];
           


        }

    }




    void runSpecialInstruction()
    {

    }

    void runBasicInstruction()
    {
       dcpu_word opcode = RAM[programCounter] & 0x1F;
       dcpu_word firstInstructionArgument = (RAM[programCounter] >> 5) & 0x1F;
       dcpu_word secondInstructionArgument = RAM[programCounter] >> 10;

       
       if(firstInstructionArgument > 0x1F)
           return;  //we are assigning to a literal value, silently fail here


       dcpu_word &firstArgumentValue = nonLiteralValue(firstInstructionArgument, true);
       dcpu_word secondArgumentValue = (secondInstructionArgument < 0x20) ? nonLiteralValue(secondInstructionArgument, false) :
           0;

       programCounter++;
       
       basicOpCodeFunctionMap.at(opcode)(firstArgumentValue, secondArgumentValue);


       

    }

    void runNextInstruction()
    {
       

        if((RAM[programCounter] & 0x1F) != 0)
            runBasicInstruction();
        else
            runSpecialInstruction();

       
    }

    size_t loadProgramIntoRAM(char *programFileName)
    {

        FILE *assembledProgramFile;
        size_t lengthOfProgramInWords;

        if(!(assembledProgramFile = fopen(programFileName, "r")))
            throw DCPU16Exception("Unable to open file");

        lengthOfProgramInWords = fread(RAM.data(), sizeof(dcpu_word), RAM.size(), assembledProgramFile);

        if(ferror(assembledProgramFile))
            throw DCPU16Exception("Error loading program");

        fclose(assembledProgramFile);

        return lengthOfProgramInWords;

    }



}
