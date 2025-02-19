#include "CPU.h"

void CPU::loadNextInstruction() {
	unsigned __int16 OStemp = mainMemory.loadWord(Registers[RegisterType::PC] + 1);
	InstructionSpecifier = mainMemory.loadByte(Registers[RegisterType::PC]);
	OprandSpecifier = OStemp;
}

void CPU::decodeInstruction() {

	struct Split413 {
		unsigned int rightThreeBits : 3;
		unsigned int middleOneBit : 1;
		unsigned int leftFourBits : 4;
	};

	struct Split53 {
		unsigned int rightThreeBits : 3;
		unsigned int leftFiveBits : 5;
	};

	struct Split71 {
		unsigned int rightOneBit : 1;
		unsigned int leftSevenBits : 7;
	};

	union SplitUnion {
		unsigned __int8 number;
		Split413 split413;
		Split53 split53;
		Split71 split71;
	};

	SplitUnion splitUnion = { InstructionSpecifier };

	if (is413Instruction(splitUnion.split413.leftFourBits)) {
		switch (splitUnion.split413.leftFourBits) {
		case 15:
			currentInstruction = InstructionList::STOREB;
			break;
		case 14:
			currentInstruction = InstructionList::STOREW;
			break;
		case 13:
			currentInstruction = InstructionList::LOADB;
			break;
		case 12:
			currentInstruction = InstructionList::LOADW;
			break;
		case 10:
			currentInstruction = InstructionList::BTWORr;
			break;
		case 9: 
			currentInstruction = InstructionList::BTWANDr;
			break;
		case 8:
			currentInstruction = InstructionList::SUBr;
			break;
		case 7: 
			currentInstruction = InstructionList::ADDr;
			break;
		}
		switch (splitUnion.split413.middleOneBit) {
		case ACCUMULATOR:
			currentRegisterType = RegisterType::A;
			break;
		case INDEX_REGISTER:
			currentRegisterType = RegisterType::X;
			break;
		}
		switch (splitUnion.split413.rightThreeBits) {
		case DIRECT:
			currentAddressMode = AddressMode::d;
			break;
		case IMMEDIATE:
			currentAddressMode = AddressMode::i;
			break;
		}
	} else if (is53Instruction(splitUnion.split53.leftFiveBits)) {
		switch (splitUnion.split53.leftFiveBits) {
		case 10:
			currentInstruction = InstructionList::COUT;
			break;
		case 9:
			currentInstruction = InstructionList::CIN;
			break;
		case 7:
			currentInstruction = InstructionList::DOT;
			break;
		case 6:
			currentInstruction = InstructionList::DIT;
			break;
		}
		switch (splitUnion.split53.rightThreeBits) {
		case DIRECT:
			currentAddressMode = AddressMode::d;
			break;
		case IMMEDIATE:
			currentAddressMode = AddressMode::i;
			break;
		}
		currentRegisterType = RegisterType::NA;
	} else if (is71Instruction(splitUnion.split71.leftSevenBits)) {
		switch (splitUnion.split71.leftSevenBits) {
		case 17:
			currentInstruction = InstructionList::RRr;
			break;
		case 16:
			currentInstruction = InstructionList::RLr;
			break;
		case 15:
			currentInstruction = InstructionList::ASRr;
			break;
		case 14:
			currentInstruction = InstructionList::ASLr;
			break;
		case 12:
			currentInstruction = InstructionList::BTWIr;
			break;
		}
		switch (splitUnion.split71.rightOneBit) {
		case ACCUMULATOR:
			currentRegisterType = RegisterType::A;
			break;
		case INDEX_REGISTER:
			currentRegisterType = RegisterType::X;
			break;
		}
		currentAddressMode = AddressMode::NA2;
	} else {
		currentInstruction = InstructionList::STOP;
		currentAddressMode = AddressMode::NA2;
		currentRegisterType = RegisterType::NA;
	}
}

bool CPU::is413Instruction(unsigned int intValueOfLeftMostFourBits) {
	return intValueOfLeftMostFourBits > 6;
}

bool CPU::is53Instruction(unsigned int intValueOfLeftMostFiveBits) {
	return intValueOfLeftMostFiveBits == 6 ||
		intValueOfLeftMostFiveBits == 7 ||
		intValueOfLeftMostFiveBits == 9 ||
		intValueOfLeftMostFiveBits == 10;
}

bool CPU::is71Instruction(unsigned int intValueOfLeftMostSevenBits) {
	return intValueOfLeftMostSevenBits == 12 ||
		intValueOfLeftMostSevenBits == 14 ||
		intValueOfLeftMostSevenBits == 15 ||
		intValueOfLeftMostSevenBits == 16 ||
		intValueOfLeftMostSevenBits == 17;
}

void CPU::BTWIrM() {
	// change 1s to 0s and 0s to 1s
	// on 16 bit value
	Registers[currentRegisterType] = ~Registers[currentRegisterType];
}

void CPU::ASLrM() {
	// shift all bits 1 to the left, right most bit gets 0
	carryBit = (Registers[currentRegisterType] & 0x8000) == 0x8000;
	Registers[currentRegisterType] = Registers[currentRegisterType] << 1;
}

void CPU::ASRrM() {
	// shift all bits 1 to the right, left most bit stays the same
	carryBit = (Registers[currentRegisterType] & 0x0001) == 0x0001;
	bool isLeftMostBitOne = (Registers[currentRegisterType] >> 15) == 1;
	if (isLeftMostBitOne) {
		// // shift all bits 1 to the right, left most bit set to 1
		Registers[currentRegisterType] = (Registers[currentRegisterType] >> 1) | 0x8000;
	}
	else {
		// right shift operation sets left most value to 0
		Registers[currentRegisterType] = Registers[currentRegisterType] >> 1;
	}
}

void CPU::RLrM() {
	bool isLeftMostBitOne = (Registers[currentRegisterType] >> 15) == 1;
	if (carryBit) {
		Registers[currentRegisterType] = (Registers[currentRegisterType] << 1) | 0x0001;
	}
	else {
		Registers[currentRegisterType] = (Registers[currentRegisterType] << 1);
	}
	carryBit = isLeftMostBitOne;
}

void CPU::RRrM() {
	bool isRightMostBitOne = (Registers[currentRegisterType] & 0x0001) == 0x0001;
	if (carryBit) {
		Registers[currentRegisterType] = (Registers[currentRegisterType] >> 1) | 0x8000;
	}
	else {
		Registers[currentRegisterType] = (Registers[currentRegisterType] >> 1);
	}
	carryBit = isRightMostBitOne;
}

void CPU::DITM() {
	string intStr;
	cout << "Input Integer: ";
	cin >> intStr;
	__int16 inputNum = stoi(intStr);
	mainMemory.storeWord(OprandSpecifier, inputNum);
	cout << "------------->\n";
}

void CPU::DOTM() {
	__int16 number;
	if (currentAddressMode == AddressMode::d) {
		number = mainMemory.loadWord(OprandSpecifier);
	}
	else {
		number = OprandSpecifier;
	}
	cout << "Integer Output: " << number << '\n';
	cout << "------------->\n";
}

void CPU::CINM() {
	char inputChar;
	cout << "Input Character: ";
	cin >> inputChar;
	mainMemory.storeByte(OprandSpecifier,inputChar);
	cout << "------------->\n";
}

void CPU::COUTM() {
	char outputChar;
	if (currentAddressMode == AddressMode::d) {
		outputChar = mainMemory.loadByte(OprandSpecifier);
	}
	else {
		outputChar = OprandSpecifier;
	}
	cout << "Character Output: " << outputChar << '\n';
	cout << "------------->\n";
}

void CPU::ADDrM() {
	__int16 int1 = Registers[currentRegisterType];
	__int16 int2;
	if (currentAddressMode == AddressMode::d) {
		int2 = mainMemory.loadWord(OprandSpecifier);
	}
	else {
		int2 = OprandSpecifier;
	}
	__int16 sum = int1 + int2;
	carryBit = ((unsigned)sum <= max((unsigned)int1, (unsigned)int2));
	Registers[currentRegisterType] = sum;
}

void CPU::SUBrM() {
	unsigned __int16 int1 = Registers[currentRegisterType];
	unsigned __int16 int2;
	if (currentAddressMode == AddressMode::d) {
		int2 = mainMemory.loadWord(OprandSpecifier);
	}
	else {
		int2 = OprandSpecifier;
	}
	unsigned __int16 int2TwosComp = (~int2) + 1;
	unsigned __int16 sum = int1 + int2TwosComp;
	carryBit = (sum <= max(int1, int2TwosComp));
	Registers[currentRegisterType] = sum;
}

void CPU::BTWANDrM() {
	__int16 oprand;
	if (currentAddressMode == AddressMode::d) {
		oprand = mainMemory.loadWord(OprandSpecifier);
	}
	else {
		oprand = OprandSpecifier;
	}
	Registers[currentRegisterType] = Registers[currentRegisterType] & oprand;
}

void CPU::BTWORrM() {
	__int16 oprand;
	if (currentAddressMode == AddressMode::d) {
		oprand = mainMemory.loadWord(OprandSpecifier);
	}
	else {
		oprand = OprandSpecifier;
	}
	Registers[currentRegisterType] = Registers[currentRegisterType] | oprand;
}

void CPU::LOADWM() {
	__int16 oprand;
	if (currentAddressMode == AddressMode::d) {
		oprand = mainMemory.loadWord(OprandSpecifier);
	}
	else {
		oprand = OprandSpecifier;
	}
	Registers[currentRegisterType] = oprand;
}

void CPU::LOADBM() {
	// byte loaded to right side of register, left byte of register is not changed
	__int8 oprand;
	if (currentAddressMode == AddressMode::d) {
		oprand = mainMemory.loadByte(OprandSpecifier);
	}
	else {
		oprand = OprandSpecifier;
	}
	unsigned __int8 leftByte;
	unsigned __int8 rightByte;
	Utilities::WordToBytes(Registers[currentRegisterType], leftByte, rightByte);
	Registers[currentRegisterType] = Utilities::BytesToWord(leftByte, oprand);
}

void CPU::STOREWM() {
	mainMemory.storeWord(OprandSpecifier, Registers[currentRegisterType]);
}

void CPU::STOREBM() {
	// stores right byte of register
	unsigned __int8 leftByte;
	unsigned __int8 rightByte;
	Utilities::WordToBytes(Registers[currentRegisterType], leftByte, rightByte);
	mainMemory.storeByte(OprandSpecifier, rightByte);
}

void CPU::STOPM(bool& stop) {
	stop = true;
}

void CPU::runInstruction(bool &stop) {
	switch (currentInstruction)
	{
	case InstructionList::STOP:
		STOPM(stop);
		break;
	case InstructionList::BTWIr:
		BTWIrM();
		break;
	case InstructionList::ASLr:
		ASLrM();
		break;
	case InstructionList::ASRr:
		ASRrM();
		break;
	case InstructionList::RLr:
		RLrM();
		break;
	case InstructionList::RRr:
		RRrM();
		break;
	case InstructionList::DIT:
		DITM();
		break;
	case InstructionList::DOT:
		DOTM();
		break;
	case InstructionList::CIN:
		CINM();
		break;
	case InstructionList::COUT:
		COUTM();
		break;
	case InstructionList::ADDr:
		ADDrM();
		break;
	case InstructionList::SUBr:
		SUBrM();
		break;
	case InstructionList::BTWANDr:
		BTWANDrM();
		break;
	case InstructionList::BTWORr:
		BTWORrM();
		break;
	case InstructionList::LOADW:
		LOADWM();
		break;
	case InstructionList::LOADB:
		LOADBM();
		break;
	case InstructionList::STOREW:
		STOREWM();
		break;
	case InstructionList::STOREB:
		STOREBM();
		break;
	}
}

void CPU::incrementProgramCounter() {
	bool isCurrentInstructionUrnary = currentAddressMode == AddressMode::NA2;
	if (isCurrentInstructionUrnary) {
		Registers[RegisterType::PC] += 1;
	}
	else {
		Registers[RegisterType::PC] += 3;
	}
}

void CPU::startVonNuemanCycle() {

	cout << "Initial Main Memory: " << mainMemory.toString() << '\n';
	cout << "------------->\n";
	Registers[RegisterType::PC] = 0;
	Registers[RegisterType::SP] = 0;
	bool STOP = false;
	do {
		loadNextInstruction();
		decodeInstruction();
		incrementProgramCounter();
		runInstruction(STOP);
		cout << toString();
	} while (!STOP);
	cout << "Final Main Memory: " << mainMemory.toString() << '\n';
}

string CPU::toString() {
	string str;
	str += "A  : " + Utilities::UnsignedInt16WordToHexadicmalWord(Registers[RegisterType::A]) + '\n';
	str += "X  : " + Utilities::UnsignedInt16WordToHexadicmalWord(Registers[RegisterType::X]) + '\n';
	str += "PC : " + Utilities::UnsignedInt16WordToHexadicmalWord(Registers[RegisterType::PC]) + '\n';
	str += "SP : " + Utilities::UnsignedInt16WordToHexadicmalWord(Registers[RegisterType::SP]) + '\n';
	str += "IS : " + Utilities::UnsignedInt8ByteToHexadicmalByte(InstructionSpecifier) + " " + Utilities::UnsignedInt16WordToHexadicmalWord(OprandSpecifier) + '\n';
	str += "------------->\n";
	return str;
}

void CPU::initializeMainMemory(string code) {
	mainMemory.initializeMemory(code);
}

void CPU::restart() {
	carryBit = false;
	InstructionSpecifier = 0;
	OprandSpecifier = 0;
	for (int i = 0; i < 4; i++) {
		Registers[i] = 0;
	}
}