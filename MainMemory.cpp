#include "MainMemory.h"

void MainMemory::initializeMemory(string hexadecimalCode) {
	// makes code have even number of characters 
	string hexadecimalCodeEven;
	if (hexadecimalCode.length() % 2 != 0) {
		hexadecimalCodeEven = hexadecimalCode + '0';
	}
	else {
		hexadecimalCodeEven = hexadecimalCode;
	}

	// loads code array as uint8 representations of a byte
	int memoryIndex = 0;
	for (int i = 1; i < hexadecimalCodeEven.length(); i += 2) {
		string hexadecimalByte;
		hexadecimalByte += hexadecimalCodeEven[i - 1];
		hexadecimalByte += hexadecimalCodeEven[i];
		unsigned __int8 unsignedInt8Byte = Utilities::ConvertToUnsignedInt8Byte(hexadecimalByte);
		memory[memoryIndex] = unsignedInt8Byte;
		memoryIndex++;
	}

	for (; memoryIndex < size; memoryIndex++) {
		memory[memoryIndex] = 0;
	}
}

string MainMemory::toString() {
	string str;
	// truncate MM
	// find position of first non-zero value
		// truncate 0's to right of it, starting on 3rd 0
	int firstNonZeroIndex = size - 1;
	for (; firstNonZeroIndex > -1; firstNonZeroIndex--) {
		if (memory[firstNonZeroIndex] != 0) {
			break;
		}
	}
	str = "00...00]";
	for (; firstNonZeroIndex > -1; firstNonZeroIndex--) {
		str = Utilities::UnsignedInt8ByteToHexadicmalByte(memory[firstNonZeroIndex]) + " " + str;
	}
	str = "[" + str;
	return str;
}

unsigned __int16 MainMemory::loadWord(int address) {
	unsigned __int8 rightMostByte = memory[address + 1];
	unsigned __int8 leftMostByte = memory[address];
	return Utilities::BytesToWord(leftMostByte, rightMostByte);
}

unsigned __int8 MainMemory::loadByte(int address) {
	return memory[address];
}

void MainMemory::storeWord(int address, unsigned __int16 word) {
	Utilities::WordToBytes(word, memory[address], memory[address + 1]);
}

void MainMemory::storeByte(int address, unsigned __int8 byte) {
	memory[address] = byte;
}