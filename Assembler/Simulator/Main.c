#include <stdio.h>
#include "commons.h"
#include "command_line_arguments.h"
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
	decodedInstruction->rd = (instructionToDecode >> 24) & mask;	
	decodedInstruction->opcode = (instructionToDecode >> 28) & mask;
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

void writeTrace(char* traceFilename, int pc, int instruction, int registers[])
{
	FILE* traceFile = fopen(traceFilename, "a");
	if (traceFile == NULL)
	{
		printf("Error opening file %s", traceFilename);
		exit(1);
	}

	char hex[HEX_WORD_LENGTH + 2];

	//sprintf(hex, "%08X", pc);
	//fputs(strcat(hex, " "), traceFile);
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

void runSimulator(char *commandLineArgs[])
{
	int registers[NUM_OF_REGISTERS];
	for (int i = 0; i < NUM_OF_REGISTERS; i++)
	{
		registers[i] = 0;
	}

	int memory[MEMORY_SIZE];
	loadMemory(commandLineArgs[MEMIN_FILENAME_ARG], memory, MEMORY_SIZE);

	ExecutionState executionState;
	executionState.pc = 0;
	executionState.isHaltExecuted = 0;

	int instructionCounter = 0;
	Instruction decodedInstruction;
	while (!executionState.isHaltExecuted)
	{
		writeTrace(commandLineArgs[TRACE_FILENAME_ARG], executionState.pc, memory[executionState.pc], registers);
		decodeInstruction(memory[executionState.pc], &decodedInstruction);
		executeInstruction(&decodedInstruction, memory, registers, &executionState);
		instructionCounter++;
	}

	writeMemoryToFile(commandLineArgs[MEMOUT_FILENAME_ARG], MEMORY_SIZE, memory);
	writeRegistersToFile(commandLineArgs[REGOUT_FILENAME_ARG], NUM_OF_REGISTERS, registers);
}

int main(int argc, char *argv[])
{
	checkCommandLineArguments(argc);
	runSimulator(argv);
}