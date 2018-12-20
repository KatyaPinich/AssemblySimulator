#include <stdio.h>
#include "commons.h"

/*
** Summary:
**		Reads the program memory from file and fills the memory array.
**		If the memory file contains less then MEMORY_SIZE entries, 
**		the rest of the entries are considered zero.
** Parameters:
**		inputMemoryFilename - the name of the file containing the memory
**		memory - an array representing the memory to fill
*/
void loadMemory(char* inputMemoryFilename, int memory[])
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

/*
** Summary:
**		Writes the trace file.
**		Each line in the trace file contains the location of the PC,
**		the instruction to be executed, and the state of the registers before
**		the execution.
** Parameters:
**		traceFilename - the name of the file for the trace output
**		pc - an integer representing the location of the PC
**		instruction - an integer representing the instruction that is to be executed
**		registers - an array representing the registers in the program
*/
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

/*
** Summary:
**		A utility function that writes an array to file as hexadecimal entries.
** Parameters:
**		filename - the name of the file for the output
**		arraySize - the size of the array to write
**		values - an array containing the values to write to the file
*/
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

/*
** Summary:
**		Writes the memory array to the specified file
** Parameters:
**		memoutFilename - the name of the file for the output
**		memorySize - the size of the array to write
**		memory - an array representing the memory of the program
*/
void writeMemoryToFile(char* memoutFilename, int memorySize, int memory[])
{
	writeHexadecimalArrayToFile(memoutFilename, memorySize, memory);
}

/*
** Summary:
**		Writes the registers array to the specified file
** Parameters:
**		registersFilename - the name of the file for the output
**		numberOfRegisters - the size of the array to write
**		registers - an array representing the registers in the program
*/
void writeRegistersToFile(char* registersFilename, int numberOfRegisters, int registers[])
{
	writeHexadecimalArrayToFile(registersFilename, numberOfRegisters, registers);
}

/*
** Summary:
**		Writes the instruction counter to a file
** Parameters:
**		countFilename - the name of the file for the output
**		instructionCount - an integer representing the number of 
**		instructions executed by the simulator
*/
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