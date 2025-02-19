#pragma once

#include "MainMemory.h"

class CPU
{
private:
	static const unsigned int ACCUMULATOR = 0b0;
	static const unsigned int INDEX_REGISTER = 0b1;
	static const unsigned int IMMEDIATE = 0b000;
	static const unsigned int DIRECT = 0b001;
	unsigned __int16 Registers[4] = {0};
	bool carryBit = false;
	unsigned __int8 InstructionSpecifier = 0;
	unsigned __int16 OprandSpecifier = 0;
	bool is413Instruction(unsigned int intValueOfLeftMostFourBits);
	bool is53Instruction(unsigned int intValueOfLeftMostFiveBits);
	bool is71Instruction(unsigned int intValueOfLeftMostSevenBits);
	void BTWIrM();
	void ASLrM();
	void ASRrM();
	void RLrM();
	void RRrM();
	void DITM();
	void DOTM();
	void CINM();
	void COUTM();
	void ADDrM();
	void SUBrM();
	void BTWANDrM();
	void BTWORrM();
	void LOADWM();
	void LOADBM();
	void STOREWM();
	void STOREBM();
	void STOPM(bool &stop);
	MainMemory mainMemory;
	void loadNextInstruction();
	void decodeInstruction();
	void runInstruction(bool &stop);
	void incrementProgramCounter();
	string toString();
	
	enum InstructionList {
		STOP=0, BTWIr, ASLr, ASRr, RLr, RRr, DIT, DOT, CIN, COUT, ADDr, SUBr, BTWANDr, BTWORr, LOADW, LOADB, STOREW, STOREB
	}; 
	string ISStrings[18] = {"STOP", "BTWIr", "ASLr", "ASRr", "RLr", "RRr", "DIT", "DOT", "CIN", "COUT", "ADDr", "SUBr", "BTWANDr", "BTWORr", "LOADW", "LOADB", "STOREW", "STOREB"};
	enum RegisterType {
		A=0, X=1, PC=2, SP=3, NA
	};
	string RTStrings[3] = { "A", "X", "NA" };
	enum AddressMode {
		d=0, i, NA2
	};
	string AMStrings[3] = { "d", "i", "NA" };
	InstructionList currentInstruction;
	RegisterType currentRegisterType;
	AddressMode currentAddressMode;

public:
	void initializeMainMemory(string code);
	void startVonNuemanCycle();
	void restart();
};

