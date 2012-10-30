#include "cpu.hpp"
#include <string>
#include <atomic>
#include <ctime>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <future>




//gets the reference to the variable that non literal argument of an instruction represents
dcpu_word &CPU::nonLiteralValue(dcpu_word valueRepresentation, bool isFirstArgumentInInstruction)
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



void CPU::runBasicInstruction()
{
#define firstArg (RAM[programCounter] >> 5) & 0x1F //gets first arg of instruction
#define secondArg RAM[programCounter] >> 10 //gets secondArg of instruction
	try{
		int selectedOpCode = (1 << (RAM[programCounter] & CPU::OPCODE_MASK));
		dcpu_word &firstArgumentValue = nonLiteralValue(firstArg, true);
		dcpu_word secondArgumentValue = (secondArg < 0x20) ? nonLiteralValue(secondArg, false) :0;
		
		int temp = secondArgumentValue;



		programCounter++;

		if(0x4000004 & selectedOpCode){
			temp = (int)firstArgumentValue + (int)secondArgumentValue;
			cyclesSinceLastInstruction += 1;

		}
		if(0xC00000C & selectedOpCode) excess = temp >> 16;
		if(0xCC00FFFE & selectedOpCode) {//set b to cacluated value
			firstArgumentValue = temp;
			cyclesSinceLastInstruction += 1;
		}
	}
	//an exception is thrown if we are trying to assign a value to a literal
	catch(DCPU16Exception &){
		
		programCounter++;
	}





/*
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
*/



}

void CPU::runNextInstruction()
{


	if((RAM[programCounter] & CPU::OPCODE_MASK) != 0)
		runBasicInstruction();
	else
		runSpecialInstruction();

	totalCycles += cyclesSinceLastInstruction;


}

size_t CPU::loadProgramIntoRAM(char *programFileName)
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

int CPU::calculateRealTimeCPUClock(){
	int cyclesBeforeTimer = totalCycles;
	sleep(1);

	return totalCycles - cyclesBeforeTimer;




}
/*
static void drawScreen()
{


	printw("A: %x\tB: %x\tC: %x\tX: %x\tY: %x\tZ: %x\tI: %x\tJ: %x\n\n", A, B, C, X, Y, Z, I, J);
	printw("PC: %x\tEX: %x, Cycle Count: %d, Clock Speed: %d", programCounter, excess, totalCycles, realTimeClockSpeed);

	move(0,0);
	refresh();


}
*/



/*
 * runs next instruction and updates the real time clock speed. Hopefully sleeps an accurate ammount of time 
 * to reflect the clockspeed
 */
void CPU::step(){

	clock_t timeBeforeInstruction = clock();

	runNextInstruction();

	double secondsSinceLastInstruction = double(clock() - timeBeforeInstruction) / CLOCKS_PER_SEC;

	double microSecondsToSleep = double(cyclesSinceLastInstruction) / clockSpeed * 1000000 - (secondsSinceLastInstruction * 1000000);

	if(microSecondsToSleep <= 0)
		microSecondsToSleep = 0;

	usleep(microSecondsToSleep);

	totalCycles += cyclesSinceLastInstruction;
	cyclesSinceLastInstruction = -1;



}



void CPU::startExecutionOfProgram(char *programFileName)
{
	size_t lengthOfProgramInWords = loadProgramIntoRAM(programFileName);
	auto futureRealTimeClockSpeed = std::async(std::launch::async, &CPU::calculateRealTimeCPUClock, this);

	while(programCounter < lengthOfProgramInWords){

		
		if(futureRealTimeClockSpeed.wait_for(std::chrono::seconds(0)) == std::future_status::ready){
			realTimeClockSpeed = futureRealTimeClockSpeed.get();
			futureRealTimeClockSpeed = std::async(std::launch::async, &CPU::calculateRealTimeCPUClock, this); 
		}
		
		step(); 


	}

	_hasExited = true;


}

std::string CPU::getStatusOfCPU() const
{
	char retStr[CPU::STATUS_STRING_LEN];

	sprintf(retStr, "A: %x\tB: %x\tC: %x\tX: %x\tY: %x\tZ: %x\tI: %x\tJ: %x\n\n"
			"PC: %x\tEX: %x, Cycle Count: %d, Clock Speed: %d", A, B, C, X, Y, Z, I, J, programCounter, excess, totalCycles, realTimeClockSpeed); 

	return retStr;
	
}






