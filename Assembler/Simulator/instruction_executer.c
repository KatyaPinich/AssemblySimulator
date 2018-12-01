#include "commons.h"

enum Opcodes
{
	Add = 0,
	Sub = 1,
	And = 2,
	Or = 3,
	Sll = 4,
	Sra = 5,
	Mac = 6,
	Branch = 7,
	Jal = 11,
	Lw = 12,
	Sw = 13,
	Jr = 14,
	Halt = 15
};

void executeInstruction(Instruction* instruction, int registers[])
{
	switch (instruction->opcode)
	{
	case 0:
		break;
	default:
		break;
	}
}