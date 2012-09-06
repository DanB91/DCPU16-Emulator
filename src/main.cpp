#include <iostream>
#include <ncurses.h>
#include "cpuExterns.hpp"


#define MIN_ARGS 2
#define MAX_ARGS 2

void printUsage()
{
    std::cout << "Usage: dcpu16emu assembled_program\n";
}




int main(int argc, char **argv)
{

   
    if(argc < MIN_ARGS || argc > MAX_ARGS)
    {
        printUsage();
        exit(1);
    }

    initscr();
    

    try{
        CPU::startExecutionOfProgram(argv[1]);
    }catch(DCPU16Exception &e){
        std::cerr << e.what() << '\n';
    }

    endwin();
    
    return 0;

}
