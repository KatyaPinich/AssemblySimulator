#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "commons.h"
#include "assembler.h"
//TODO: Check parse number is correct 

int main(int argc, char *args[])
{
	int memory[MEMORY_SIZE];
	int i = 0;
	const char *regs[] = { "$zero", "$at", "$v0", "$a0", "$a1", "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2", "$gp","$sp", "$fp", "$ra" };
	const char *opps[] = { "add", "sub", "and", "or", "sll", "sra", "mac", "branch", "in", "out", "res", "jal", "lw", "sw", "jr", "halt" };
	char* assemblyFlie = args[1];
	char* meminFile = args[2];
	label_t* labels = NULL;
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}
	labels = firstPass(assemblyFlie, labels);
	secondPass(assemblyFlie, labels, regs, opps, memory);
	meminWrite(memory, meminFile); 
	exit(0);
}