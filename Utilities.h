#pragma once
#include <string>
#include <iostream>

using namespace std;

class Utilities
{
public:
	static unsigned __int8 ConvertToUnsignedInt8Byte(string hexadecimalByte);
	static unsigned int HexadecimalCharacterToUInt(char hexadicmalChar);
	static unsigned __int8 CreateUnsignedInt8Byte(int leftMostFourBits, int rightMostFourBits);
	static string UnsignedInt8ByteToHexadicmalByte(unsigned __int8 byte);
	static char ConvertToHexadicmalChar(unsigned int number);
	static string UnsignedInt16WordToHexadicmalWord(unsigned __int16 word);
	static unsigned __int16 BytesToWord(unsigned __int8 leftMostByte, unsigned __int8 rightMostByte);
	static void WordToBytes(unsigned __int16 word, unsigned __int8& leftMostByte, unsigned __int8& rightMostByte);
	static string removeNonHexadicmalCharacters(string str);
};

