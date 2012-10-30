#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <unordered_map>
#include <future>
#include "cpu.hpp"


#define MIN_ARGS 2
#define MAX_ARGS 4
#define DEFAULT_CLOCKSPEED "10000"


static void printUsageAndExit()
{
    std::cout << "Usage: dcpu16emu assembled_program\n";
    exit(1);
}

static void drawScreen(const CPU *cpu)
{
	initscr();
	
	while(!cpu->hasExited()){
		printw("%s", cpu->getStatusOfCPU().c_str());

		move(0,0);
		refresh();
	}

	endwin();
}


static std::unordered_map<std::string, std::string> parseArguments(int argc, char **argv)
{
	std::unordered_map<std::string, std::string> arguments;
	arguments["clockSpeed"] = DEFAULT_CLOCKSPEED;
	for(int i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
				case 'c':
					if(i + 1 < argc)
						 arguments["clockSpeed"] = argv[i+1];	
					break;
				default:
					printUsageAndExit();
			}
		}
		
	}

	return arguments;
}

int main(int argc, char **argv)
{
	auto arguments = parseArguments(argc, argv);
	CPU *cpu = new CPU(atoi(arguments["clockSpeed"].c_str()));

	if(argc < MIN_ARGS || argc > MAX_ARGS)
	{
		printUsageAndExit();
	}
	
	std::thread t(drawScreen, cpu);
	
	
	try{
		cpu->startExecutionOfProgram(argv[1]);
	}catch(DCPU16Exception &e){
		std::cerr << e.what() << '\n';
	}

	t.join();
	delete cpu;


	return 0;

}
