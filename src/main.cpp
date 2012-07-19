#include <iostream>

#define MIN_ARGS 1
#define MAX_ARGS 1

void printUsage()
{
    std::cout << "Usage: dcpu16emu program\n";
}


int main(int argc, char **argv)
{
    if(argc < MIN_ARGS || argc > MAX_ARGS)
    {
        printUsage();
        exit(1);
    }
}
