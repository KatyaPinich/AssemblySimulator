#ifndef INSTRUCTION_EXECUTER_H
#define INSTRUCTION_EXECUTER_H

#include "commons.h"

struct executionState {
	int pc;
	int isHaltExecuted;
};
typedef struct executionState ExecutionState;

void executeInstruction(Instruction* instruction, int memory[], int registers[], ExecutionState* state);

#endif
