#ifndef COMMONS_H
#define COMMONS_H

#define FALSE = 0
#define TRUE = 1

struct instruction
{
	int opcode;
	unsigned int rd;
	int rs;
	int rt;
	int rm;
	int imm;
};
typedef struct instruction Instruction;

#endif
