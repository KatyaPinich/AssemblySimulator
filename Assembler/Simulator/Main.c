#include <stdio.h>
#include "commons.h"

#define NUM_OF_REGISTERS 16
#define HEX_WORD_LENGTH 8

void decodeInstruction(int instructionToDecode, Instruction* decodedInstruction)
{
	int opcodeMask = 0Xf0000000;
	int rdMask = 0x0f000000;
	int rsMask = 0x00f00000;
	int rtMask = 0x000f0000;
	int rmMask = 0x0000f000;
	int immMask = 0x00000fff;

	decodedInstruction->opcode = instructionToDecode & opcodeMask;
	decodedInstruction->rd = instructionToDecode & rdMask;
	decodedInstruction->rs = instructionToDecode & rsMask;
	decodedInstruction->rt = instructionToDecode & rtMask;
	decodedInstruction->rm = instructionToDecode & rmMask;
	decodedInstruction->imm = instructionToDecode & immMask;
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