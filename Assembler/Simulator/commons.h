#ifndef COMMONS_H
#define COMMONS_H

struct instruction
{
	int opcode;
	int rd;
	int rs;
	int rt;
	int rm;
	int imm;
};
typedef struct instruction Instruction;

#endif
