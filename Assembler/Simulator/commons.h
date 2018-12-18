#ifndef COMMONS_H
#define COMMONS_H

#define FALSE = 0
#define TRUE = 1

#define NUM_OF_REGISTERS 16
#define HEX_WORD_LENGTH 8
#define MEMORY_SIZE 4096

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
