#include <stdio.h>
#include "commons.h"
#include "command_line_arguments.h"
#include "instruction_executer.h"
#include "files_handler.h"

/*
** Summary:
**		Decodes an instruction.
**		Each instruction is decoded using a mask to extract the bits of each component.
**		Each component of the instruction is then assigned to the appropriate field in
**		the decodedInstruction struct.
** Parameters:
**		instructionToDecode - an integer representing the instruction from memory to decode
**		decodedInstruction - a struct representing the decoded instruction
*/
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

/*
** Summary:
**		Executes the instructions written in the memin.txt file.
**		Each instruction is decoded then executed.
**		In the end, it outputs the memory, registers and instruction count to files.
** Parameters:
**		commandLineArgs - an array containing the command line arguments passed
*/
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
	writeCountToFile(commandLineArgs[COUNT_FILENAME_ARG], instructionCounter);
}