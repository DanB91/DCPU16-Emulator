#include <iostream>
#include "cpu.hpp"

#define MIN_ARGS 2
#define MAX_ARGS 2

void printUsage()
{
    std::cout << "Usage: dcpu16emu assembled_program\n";
}

void loadProgramIntoRAM(char *programFileName)
{
    FILE *assembledProgramFile;
    
    if(!(assembledProgramFile = fopen(programFileName, "r")))
        throw DCPU16Exception("Unable to open file");

    fread(CPU::RAM.data(), sizeof(udcpu_word), CPU::RAM.size(), assembledProgramFile);
    
    fclose(assembledProgramFile);

}


int main(int argc, char **argv)
{

    if(argc < MIN_ARGS || argc > MAX_ARGS)
    {
        printUsage();
        exit(1);
    }
    try{
        loadProgramIntoRAM(argv[1]);
        
    }catch(DCPU16Exception &e){
        std::cerr << e.what() << '\n';
    }

    return 0;

}
