#ifndef INSTRUCTION_EXECUTER_H
#define INSTRUCTION_EXECUTER_H

#include "commons.h"

/*
** Summary:
**		Represents the execution state of the program.
** Fields:
**		pc - an integer representing the PC, which 
**			points to the instruction to be executed.
**		isHaltExecuted - true if the HALT command 
**			has been executed, false otherwise
*/
struct executionState {
	int pc;
	int isHaltExecuted;
};
typedef struct executionState ExecutionState;

void executeInstruction(Instruction* instruction, int memory[], int registers[], ExecutionState* state);

#endif
