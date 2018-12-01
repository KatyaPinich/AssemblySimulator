#include <stdio.h>
#include "commons.h"
#include "instruction_executer.h"

#define NUM_OF_REGISTERS 16
#define HEX_WORD_LENGTH 8

void decodeInstruction(int instructionToDecode, Instruction* decodedInstruction)
{
	int immMask = 0x00000fff;
	int mask = 0xf;
	
	decodedInstruction->imm = instructionToDecode & immMask;
	decodedInstruction->rm = (instructionToDecode >> 12) & mask;
	decodedInstruction->rt = (instructionToDecode >> 16) & mask;
	decodedInstruction->rs = (instructionToDecode >> 20) & mask;
	decodedInstruction->rd = (instructionToDecode >> 24) & mask;
	decodedInstruction->opcode = (instructionToDecode >> 28) & mask;
}

void runSimulator(char* inputMemoryFilename)
{
	int registers[NUM_OF_REGISTERS];
	for (int i = 0; i < NUM_OF_REGISTERS; i++)
	{
		registers[i] = 0;
	}

	FILE* inputMemoryFile = fopen(inputMemoryFilename, "r");
	if (inputMemoryFile == NULL)
	{
		printf("Error opening file %s", inputMemoryFilename);
		exit(1);
	}

	int instructionToDecode = 0;
	Instruction decodedInstruction;
	while (fscanf(inputMemoryFile, "%x", &instructionToDecode) == 1)
	{
		decodeInstruction(instructionToDecode, &decodedInstruction);
		executeInstruction(&decodedInstruction, registers);
	}

	fclose(inputMemoryFile);

	// Fetch
	// Decode
	// Execute
}

int main(int argc, char *args[])
{
	char* inputMemoryFilename = args[1];
	runSimulator(inputMemoryFilename);
}