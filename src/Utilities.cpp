#include "Utilities.h"

unsigned __int8 Utilities::ConvertToUnsignedInt8Byte(string hexadecimalByte) {
	unsigned int leftMostFourBits = Utilities::HexadecimalCharacterToUInt(hexadecimalByte[0]);
	unsigned int rightMostFourBits = Utilities::HexadecimalCharacterToUInt(hexadecimalByte[1]);
	unsigned __int8 unsignedInt8Byte = Utilities::CreateUnsignedInt8Byte(leftMostFourBits, rightMostFourBits);
	return unsignedInt8Byte;
}

unsigned int Utilities::HexadecimalCharacterToUInt(char hexadecimalChar) {
	unsigned int toReturn;
	bool characterIsNumber = (hexadecimalChar - '0' < 10 && hexadecimalChar - '0' > -1);
	if (characterIsNumber) {
		toReturn = hexadecimalChar - '0';
	}
	else {
		toReturn = hexadecimalChar - 'A' + 10;
	}
	return toReturn;
}

unsigned __int8 Utilities::CreateUnsignedInt8Byte(int leftMostFourBits, int rightMostFourBits) {

	struct TwoFourBitInts {
		unsigned int firstFourBits : 4;
		unsigned int lastFourBits : 4;
	};

	union UnionOfTwoFourBitsInts {
		TwoFourBitInts twoFourBitInts;
		unsigned int combined;
	};

	TwoFourBitInts twoFourBitInts = { rightMostFourBits, leftMostFourBits };
	UnionOfTwoFourBitsInts unionOfTwoFourBitsInts = { twoFourBitInts };
	
	return unionOfTwoFourBitsInts.combined;
}

string Utilities::UnsignedInt8ByteToHexadicmalByte(unsigned __int8 unsignedInt8Byte) {
	struct ByteSplitUp {
		unsigned int rightMostFourBits: 4;
		unsigned int leftMostFourBits: 4;
	};

	union Union {
		unsigned __int8 unsignedInt8Byte;
		ByteSplitUp byte;
	};

	Union myUnion = { unsignedInt8Byte };
	string hexadicmalByte;
	hexadicmalByte += ConvertToHexadicmalChar(myUnion.byte.leftMostFourBits);
	hexadicmalByte += ConvertToHexadicmalChar(myUnion.byte.rightMostFourBits);
	return hexadicmalByte;
}

char Utilities::ConvertToHexadicmalChar(unsigned int number) {
	bool isInteger = (number < 10);
	char hexadicmalValue;
	if (isInteger) {
		hexadicmalValue = number + '0';
	}
	else {
		hexadicmalValue = (number - 10) + 'A';
	}
	return hexadicmalValue;
}

string Utilities::UnsignedInt16WordToHexadicmalWord(unsigned __int16 word) {
	struct WordSplitUp {
		unsigned __int8 rightMostByte : 8;
		unsigned __int8 leftMostByte : 8;
	};

	union Union {
		unsigned __int16 word;
		WordSplitUp wordSplitUp;
	};

	Union myUnion = { word };
	string hexadicmalWord;
	hexadicmalWord += UnsignedInt8ByteToHexadicmalByte(myUnion.wordSplitUp.leftMostByte) + ' ';
	hexadicmalWord += UnsignedInt8ByteToHexadicmalByte(myUnion.wordSplitUp.rightMostByte);
	return hexadicmalWord;
}


unsigned __int16 Utilities::BytesToWord(unsigned __int8 leftMostByte, unsigned __int8 rightMostByte) {
	struct Word {
		unsigned __int8 rightMostByte;
		unsigned __int8 leftMostByte;
	};

	union WordUnion {
		unsigned __int16 wordAsUINT16;
		Word word;
	};

	Word word = { rightMostByte, leftMostByte };
	WordUnion wordUnion;
	wordUnion.word = word;
	
	return wordUnion.wordAsUINT16;
}

void Utilities::WordToBytes(unsigned __int16 word, unsigned __int8& leftMostByte, unsigned __int8& rightMostByte) {
	struct Word {
		unsigned __int8 rightMostByte;
		unsigned __int8 leftMostByte;
	};

	union WordUnion {
		unsigned __int16 wordAsUINT16;
		Word word;
	};

	WordUnion wordUnion;
	wordUnion.wordAsUINT16 = word;
	leftMostByte = wordUnion.word.leftMostByte;
	rightMostByte = wordUnion.word.rightMostByte;
}

string Utilities::removeNonHexadicmalCharacters(string str) {
	string toReturn;
	for (int i = 0; i < str.length(); i++) {
		char c = str[i];
		if (c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f') {
			toReturn += c;
		}
		else if (c >= '0' && c <= '9') {
			toReturn += c;
		}
	}
	return toReturn;
}
