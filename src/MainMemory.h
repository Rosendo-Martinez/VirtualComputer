#pragma once
#include <string>
#include "Utilities.h"

using namespace std;

class MainMemory
{
private:
	unsigned __int8 memory[100] = {0};
	int size = 100;
public:
	void initializeMemory(string hexadecimalCode);
	unsigned __int16 loadWord(int address);
	unsigned __int8 loadByte(int address);
	void storeWord(int address, unsigned __int16 word);
	void storeByte(int address, unsigned __int8 byte);
	string toString();
};

