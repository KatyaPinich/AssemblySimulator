#ifndef COMMONS_H
#define COMMONS_H

#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 501
#define MAX_LABEL_LENGTH 51
#define OPPS_REGS_TOTAL_LENGTH 180
#define MEMORY_REGISTER_SIZE 13		
#define MEMORY_SIZE 4097			
#define NUMBER_OF_LEGAL_ARGS 5
#define LABELS_AMOUNT 40

typedef struct label
{
	char Name[MAX_LABEL_LENGTH];
	int Line;
	struct label *next;
} label_t;


#endif
