#include "Utilities.h"
#include "CPU.h"
#include <fstream>

int main() {

	CPU cpu;
	string code;

	cout << "Input program code: ";
	getline(cin, code);
	code = Utilities::removeNonHexadicmalCharacters(code);

	cpu.initializeMainMemory(code);
	cpu.startVonNuemanCycle();

	return 0;
}