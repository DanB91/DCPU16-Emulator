#include <iostream>
#include <ncurses.h>
#include "cpuExterns.hpp"


#define MIN_ARGS 2
#define MAX_ARGS 2

namespace CPU{
	void startExecutionOfProgram(char*);
}

void printUsageAndExit()
{
    std::cout << "Usage: dcpu16emu assembled_program\n";
    exit(1);
}


void parseArguments(int argc, char **argv)
{
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-c") == 0)
		{

		}
	}
}

int main(int argc, char **argv)
{


	if(argc < MIN_ARGS || argc > MAX_ARGS)
	{
		printUsageAndExit();
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
