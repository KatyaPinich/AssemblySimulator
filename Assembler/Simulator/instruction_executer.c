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

void add(Instruction* instruction, int registers[], ExecutionState* state)
{
	registers[instruction->rd] = registers[instruction->rs] + registers[instruction->rt] + signExtend(instruction->imm);
	state->pc++;
}

void sub(Instruction* instruction, int registers[], ExecutionState* state)
{
	registers[instruction->rd] = registers[instruction->rs] - registers[instruction->rt] - signExtend(instruction->imm);
	state->pc++;
}

void and(Instruction* instruction, int registers[], ExecutionState* state)
{
	registers[instruction->rd] = registers[instruction->rs] & registers[instruction->rt] & signExtend(instruction->imm);
}

void branch(Instruction* instruction, int registers[], ExecutionState* state)
{
	int rmValue = instruction->rm;
	int rsRegisterValue = registers[instruction->rs];
	int rtRegisterValue = registers[instruction->rt];
	if ((rmValue == 0 && (rsRegisterValue == rtRegisterValue)) ||
		(rmValue == 1 && (rsRegisterValue != rtRegisterValue)) ||
		(rmValue == 2 && (rsRegisterValue > rtRegisterValue)) ||
		(rmValue == 3 && (rsRegisterValue < rtRegisterValue)) ||
		(rmValue == 4 && (rsRegisterValue >= rtRegisterValue)) ||
		(rmValue == 5 && (rsRegisterValue <= rtRegisterValue)))
	{
		state->pc = instruction->imm;
	}
	else
	{
		state->pc++;
	}
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
			add(instruction, registers, state);
			break;
		case Sub:
			sub(instruction, registers, state);
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
}