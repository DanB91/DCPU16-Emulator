#include <iostream>
#include <ncurses.h>
#include "cpuExterns.hpp"
#include <thread>
#include <unistd.h>

#define MIN_ARGS 2
#define MAX_ARGS 2

void printUsage()
{
    std::cout << "Usage: dcpu16emu assembled_program\n";
}


void drawScreen()
{
    while(!CPU::hasProgramFinished()){
        printw("A: %x\tB: %x\tC: %x\tX: %x\tY: %x\tZ: %x\tI: %x\tJ: %x\n\n", CPU::A, CPU::B, CPU::C, CPU::X, CPU::Y, CPU::Z, CPU::I, CPU::J);
        printw("PC: %x\tEX: %x, Cycle Count: %d, Clock Speed: %f", CPU::programCounter, CPU::excess, CPU::totalCycles, CPU::realTimeClockSpeed);
        
        move(0,0);
        refresh();
        usleep(1000);

    }
}

int main(int argc, char **argv)
{

   
    if(argc < MIN_ARGS || argc > MAX_ARGS)
    {
        printUsage();
        exit(1);
    }

    initscr();
    std::thread printingOutputToScreenThread(drawScreen);


    try{
        CPU::startExecutionOfProgram(argv[1]);
    }catch(DCPU16Exception &e){
        std::cerr << e.what() << '\n';
    }

    printingOutputToScreenThread.join();

    endwin();
    
    return 0;

}
