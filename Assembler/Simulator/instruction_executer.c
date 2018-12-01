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

int signExtend(int number)
{
	return (number & 0x800) == 0x800 ? 0xFFFFF000 | number : number;
}

void executeAdd(Instruction* instruction, int registers[])
{
	registers[instruction->rd] = registers[instruction->rs] + registers[instruction->rt] + signExtend(instruction->imm);
}

void loadWord(Instruction* instruction, int memory[], int registers[])
{
	int memoryAddress = (registers[instruction->rs] + signExtend(instruction->imm)) & 0xfff;
	registers[instruction->rd] = memory[memoryAddress];
}

void executeInstruction(Instruction* instruction, int memory[], int registers[], int* pc)
{
	switch (instruction->opcode)
	{
	case Add:
		executeAdd(instruction, registers);
		break;
	case Lw:
		break;
	default:
		break;
	}
}