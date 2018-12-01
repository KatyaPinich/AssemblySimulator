#ifndef INSTRUCTION_EXECUTER_H
#define INSTRUCTION_EXECUTER_H

#include "commons.h"

void executeInstruction(Instruction* instruction, int memory[], int registers[], int* pc);

#endif
