#include <iostream>
#include <ncurses.h>
#include "cpuExterns.hpp"


#define MIN_ARGS 2
#define MAX_ARGS 2

void printUsage()
{
    std::cout << "Usage: dcpu16emu assembled_program\n";
}


void drawScreen()
{
    printw("A: %x\tB: %x\tC: %x\tX: %x\tY: %x\tZ: %x\tI: %x\tJ: %x\n\n", CPU::A, CPU::B, CPU::C, CPU::X, CPU::Y, CPU::Z, CPU::I, CPU::J);
    printw("PC: %x", CPU::programCounter);
    move(0,0);
    refresh();
}

int main(int argc, char **argv)
{

    size_t lengthOfProgramInWords;

    if(argc < MIN_ARGS || argc > MAX_ARGS)
    {
        printUsage();
        exit(1);
    }

    try{
        initscr();
        lengthOfProgramInWords = CPU::loadProgramIntoRAM(argv[1]);

        while(CPU::programCounter < lengthOfProgramInWords){
            drawScreen();
            CPU::runNextInstruction();
        }
        
    }catch(DCPU16Exception &e){
        std::cerr << e.what() << '\n';
    }

    endwin();
    
    return 0;

}
