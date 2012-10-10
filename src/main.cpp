#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include "cpuExterns.hpp"


#define MIN_ARGS 2
#define MAX_ARGS 4

namespace CPU{
	void startExecutionOfProgram(char*);
	extern int clockSpeed;
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
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'c':
					if(i + 1 < argc)
						CPU::clockSpeed = atoi(argv[i+1]);	
					break;
				default:
					printUsageAndExit();
			}
		}
	}
}

int main(int argc, char **argv)
{


	if(argc < MIN_ARGS || argc > MAX_ARGS)
	{
		printUsageAndExit();
	}
	parseArguments(argc, argv);
	initscr();


	try{
		CPU::startExecutionOfProgram(argv[1]);
	}catch(DCPU16Exception &e){
		std::cerr << e.what() << '\n';
	}

	endwin();

	return 0;

}
