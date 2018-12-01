#include "commons.h"
#include "instruction_executer.h"

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

void jumpAndLink(Instruction* instruction, int registers[], ExecutionState* state)
{
	registers[15] = (state->pc + 1) & 0xfff;
	state->pc = instruction->imm;
	state->pcModified = 1;
}

void saveWord(Instruction* instruction, int memory[], int registers[])
{
	int memoryAddress = (registers[instruction->rs] + signExtend(instruction->imm)) & 0xfff;
	memory[memoryAddress] = registers[instruction->rd];
}

void executeInstruction(Instruction* instruction, int memory[], int registers[], ExecutionState* state)
{
	switch (instruction->opcode)
	{
		case Add:
			executeAdd(instruction, registers);
			break;
		case Sub:
		case And:
		case Or:
		case Sll:
		case Sra:
		case Mac:
		case Branch:
		case Jal:
			jumpAndLink(instruction, registers, state);
			break;
		case Lw:
			loadWord(instruction, memory, registers);
			break;
		case Sw:
			saveWord(instruction, memory, registers);
		case Jr:
		case Halt:
			state->isHaltExecuted = 1;
		default:
			break;
	}

	if (!state->pcModified)
	{
		state->pc += 1;
	}
	else
	{
		state->pcModified = 0;
	}
}