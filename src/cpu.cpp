#include "cpu.hpp"
#include <atomic>
#include <ctime>
#include <unistd.h>
#include <ncurses.h>

namespace CPU{


	static dcpu_word &nonLiteralValue(dcpu_word valueRepresentation, bool isFirstArgumentInInstruction)
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
				cyclesSinceLastInstruction++;
				return RAM[A + RAM[++programCounter]];
			case 0x11:
				cyclesSinceLastInstruction++;
				return RAM[B + RAM[++programCounter]];
			case 0x12:
				cyclesSinceLastInstruction++;
				return RAM[C + RAM[++programCounter]];
			case 0x13:
				cyclesSinceLastInstruction++;
				return RAM[X + RAM[++programCounter]];
			case 0x14:
				cyclesSinceLastInstruction++;
				return RAM[Y + RAM[++programCounter]];
			case 0x15:
				cyclesSinceLastInstruction++;
				return RAM[Z + RAM[++programCounter]];
			case 0x16:
				cyclesSinceLastInstruction++;
				return RAM[I + RAM[++programCounter]];
			case 0x17:
				cyclesSinceLastInstruction++;
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
				cyclesSinceLastInstruction++;
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
			default:
				throw DCPU16Exception("Illegal argument passed in");

		}

	}




	static void runSpecialInstruction()
	{

	}

	static void runBasicInstruction()
	{
		const OpCode &op = basicOpCodeFunctionMap.at(RAM[programCounter] & 0x1F); 
		dcpu_word firstInstructionArgument = (RAM[programCounter] >> 5) & 0x1F;
		dcpu_word secondInstructionArgument = RAM[programCounter] >> 10;

		//we are assigning to a literal value, silently fail here
		if(firstInstructionArgument > 0x1F){
			programCounter++;
			return; 
		}

		dcpu_word &firstArgumentValue = nonLiteralValue(firstInstructionArgument, true);
		dcpu_word secondArgumentValue = (secondInstructionArgument < 0x20) ? nonLiteralValue(secondInstructionArgument, false) :
			0;

		programCounter++; 

		op(firstArgumentValue, secondArgumentValue);
		cyclesSinceLastInstruction += op.cycleCount;




	}

	static void runNextInstruction()
	{


		if((RAM[programCounter] & 0x1F) != 0)
			runBasicInstruction();
		else
			runSpecialInstruction();

		totalCycles += cyclesSinceLastInstruction;


	}

	static size_t loadProgramIntoRAM(char *programFileName)
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

	static int getRealTimeCPUClock(){
		static int totalCyclesInOneSecond = 0;
		static clock_t startTime = clock();

		int returnValue = -1;

		if((clock() - startTime) / CLOCKS_PER_SEC < 1)
			totalCyclesInOneSecond += cyclesSinceLastInstruction;
		else{
			returnValue = totalCyclesInOneSecond;
			startTime = clock();
			totalCyclesInOneSecond = 0; 
		}

		return returnValue;




	}

	static void drawScreen()
	{

	
		printw("A: %x\tB: %x\tC: %x\tX: %x\tY: %x\tZ: %x\tI: %x\tJ: %x\n\n", A, B, C, X, Y, Z, I, J);
		printw("PC: %x\tEX: %x, Cycle Count: %d, Clock Speed: %d", programCounter, excess, totalCycles, realTimeClockSpeed);

		move(0,0);
		refresh();


	}




	/*
	 * runs next instruction and updates the real time clock speed. Hopefully sleeps an accurate ammount of time 
	 * to reflect the clockspeed
	 */
	static void step(){

		clock_t timeBeforeInstruction = clock();

		runNextInstruction();
		drawScreen();


		
		double secondsSinceLastInstruction = double(clock() - timeBeforeInstruction) / CLOCKS_PER_SEC;

		double microSecondsToSleep = double(cyclesSinceLastInstruction) / clockSpeed * 1000000 - (secondsSinceLastInstruction * 1000000);

		if(microSecondsToSleep <= 0)
			microSecondsToSleep = 0;

		usleep(microSecondsToSleep);

		unsigned int rtcp = getRealTimeCPUClock(); //real time clock speed

		if(rtcp != -1)
			realTimeClockSpeed = rtcp;



		totalCycles += cyclesSinceLastInstruction;
		cyclesSinceLastInstruction = 0;



	}

	void startExecutionOfProgram(char *programFileName)
	{
		size_t lengthOfProgramInWords = loadProgramIntoRAM(programFileName);

		while(programCounter < lengthOfProgramInWords){

			step(); 


		}

		
	}

	



}
