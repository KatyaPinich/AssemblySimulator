#include <stdio.h>
#include "commons.h"

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

void writeTrace(char* traceFilename, int pc, int instruction, int registers[])
{
	FILE* traceFile = fopen(traceFilename, "a");
	if (traceFile == NULL)
	{
		printf("Error opening file %s", traceFilename);
		exit(1);
	}

	char hex[HEX_WORD_LENGTH + 2];

	fprintf(traceFile, "%08X %08X ", pc, instruction);

	for (int i = 0; i < NUM_OF_REGISTERS; i++)
	{
		if (i == NUM_OF_REGISTERS - 1)
			fprintf(traceFile, "%08X\n", registers[i]);
		else
			fprintf(traceFile, "%08X ", registers[i]);
	}

	fclose(traceFile);
}

void writeHexadecimalArrayToFile(char* filename, int arraySize, int values[])
{
	FILE* file = fopen(filename, "w");
	if (file == NULL)
	{
		printf("Error opening file %s", filename);
		exit(1);
	}

	for (int i = 0; i < arraySize; i++)
	{
		fprintf(file, "%08X\n", values[i]);
	}

	fclose(file);
}

void writeMemoryToFile(char* memoutFilename, int memorySize, int memory[])
{
	writeHexadecimalArrayToFile(memoutFilename, memorySize, memory);
}

void writeRegistersToFile(char* registersFilename, int numberOfRegisters, int registers[])
{
	writeHexadecimalArrayToFile(registersFilename, numberOfRegisters, registers);
}

void writeCountToFile(char* countFilename, int instructionCount)
{
	FILE* file = fopen(countFilename, "w");
	if (file == NULL)
	{
		printf("Error opening file %s", countFilename);
		exit(1);
	}

	fprintf(file, "%d\n", instructionCount);

	fclose(file);
}