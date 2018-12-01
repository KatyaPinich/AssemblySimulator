#include <stdio.h>
#include "commons.h"
#include "instruction_executer.h"

#define NUM_OF_REGISTERS 16
#define HEX_WORD_LENGTH 8
#define MEMORY_SIZE 4096

void decodeInstruction(int instructionToDecode, Instruction* decodedInstruction)
{
	int immMask = 0x00000fff;
	int mask = 0xf;
	
	decodedInstruction->imm = instructionToDecode & immMask;
	decodedInstruction->rm = (instructionToDecode >> 12) & mask;
	decodedInstruction->rt = (instructionToDecode >> 16) & mask;
	decodedInstruction->rs = (instructionToDecode >> 20) & mask;
	decodedInstruction->rd = (instructionToDecode >> 24) & mask;	decodedInstruction->opcode = (instructionToDecode >> 28) & mask;
}

void loadMemory(char* inputMemoryFilename, int memory[], int memorySize)
{
	FILE* inputMemoryFile = fopen(inputMemoryFilename, "r");
	if (inputMemoryFile == NULL)
	{
		printf("Error opening file %s", inputMemoryFilename);
		exit(1);
	}

	int instruction = 0;
	int counter = 0;
	while (fscanf(inputMemoryFile, "%x", &instruction) == 1)
	{
		memory[counter] = instruction;
		counter++;
	}

	fclose(inputMemoryFile);

	if (counter < MEMORY_SIZE)
	{
		for (int i = counter; i < MEMORY_SIZE; i++)
		{
			memory[i] = 0;
		}
	}
}

void runSimulator(char* inputMemoryFilename)
{
	int registers[NUM_OF_REGISTERS];
	for (int i = 0; i < NUM_OF_REGISTERS; i++)
	{
		registers[i] = 0;
	}

	int memory[MEMORY_SIZE];
	loadMemory(inputMemoryFilename, memory, MEMORY_SIZE);

	int pc = 0;
	int isHaltExecuted = 0;
	Instruction decodedInstruction;
	while (!isHaltExecuted)
	{
		decodeInstruction(memory[pc], &decodedInstruction);
		executeInstruction(&decodedInstruction, memory, registers, &pc);
	}
}

int main(int argc, char *args[])
{
	char* inputMemoryFilename = args[1];
	runSimulator(inputMemoryFilename);
}