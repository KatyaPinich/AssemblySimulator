#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 501
#define MAX_LABEL_LENGTH 51
#define OPP_AMOUNT 32

typedef struct label
{
	char Name[MAX_LABEL_LENGTH];
	int Line;
	struct label *next;
} label_t;

void construckHashTable(char *regOppArr[OPP_AMOUNT])
{
	char *regOppArr[] = { "add", "sub", "and", "or", "sll", "sra", "mac", "branch", "res", "res", "res", "jal", "lw", "sw", "jr", "halt" , "$zero", "$at", "$v0", "$a0", "$a1", "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2", "$gp","$sp", "$fp", "$ra"};
}

int findIndex(char *string, int size, char target)
{
	int i = 0;
	while ((i < size) && (string[i] != target)) i++;
	return (i < size) ? (i) : (-1);
}

int findRegOpp(char *regAndOpp[OPP_AMOUNT], char* target)
{
	int i = 0;
	while ((i < OPP_AMOUNT) && (strcmp(target, regAndOpp[i])!=0)) i++;
	return (i < OPP_AMOUNT) ? (i) : (-1);
}


label_t* addLable(label_t *head, char* assemblyToken, int tokenLength, int locationCounter)
{
	char tempString[MAX_LABEL_LENGTH];
	label_t *newLabel = (label_t*)malloc(sizeof(label_t));		//allocation
	char target;
	int targetIndex = 0;
	if (newLabel == NULL)	//exception//
		return 1;
	targetIndex = findIndex(assemblyToken, tokenLength, ':');	//index of ':'
	strcpy(tempString, assemblyToken);
	tempString[targetIndex] = '\0';		//removes ':'
	strcpy(newLabel->Name, tempString);		//construct//
	newLabel->Line = locationCounter;
	newLabel->next = head;
	return newLabel;
}

void firstPass(char* fileName, label_t* labels)
{
	// marks each label and keeps their adress as instructed (location, name) in a linked list//
	char assemblyLine[500];
	char *assemblyToken;
	int locationCounter = 0;
	int tokenLength = 0;
	int containsEndCommand = FALSE;
	FILE* fptr;
	if ((fptr = fopen(fileName, "r")) == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}
	while (fgets(assemblyLine, MAX_LENGTH, fptr))		//while not EOF
	{
		locationCounter++;		//new assembly line
		assemblyToken = strtok(assemblyLine, " \t");
		if (assemblyToken == NULL)		//beacause an exaption was thrown otherwise
			break;
		if (assemblyToken[0] != '#') 		// Check line is not a comment
		{
			tokenLength = strlen(assemblyToken);		//checks if it's a Label
			if ((assemblyToken[tokenLength - 1] == ':') || (assemblyToken[tokenLength - 2] == ':'))
			{
				//? TODO: Check the label does not exist - What do you mean??
				if (assemblyToken[0] != '#') // TODO: Check label name is valis - starts with a character
				{
					labels = addLable(labels, assemblyToken, tokenLength, locationCounter);
				}

			}
			else if (strcmp(assemblyToken, "halt") == 0)
			{
				containsEndCommand = TRUE;
			}
		}
	}

	fclose(fptr);

	if (!containsEndCommand)
	{
		printf("ERROR: halt command is missing from code.");
		exit(1);
	}
}

int isLabel(label_t* head, char* x[MAX_LABEL_LENGTH])
{
	// Checks whether the value x is present in linked list */
	label_t* current = head;  // Initialize current 
	while (current != NULL)
	{
		if (strcmp(current->Name, x) == 0)
			return current->Line;
		current = current->next;
	}
	return -1;
}
//TODO: take care of ox numbers, "zero"
int isNumber(char* assemblyToken)
{
	// returns (-1) IF it's a number, ELSE: the first index of the non-number char in assemblyToken
	int i;
	int length;
	length = strlen(assemblyToken);
	for (i = 0; i < length; i++)
	{
		if (!isdigit(assemblyToken[i]))
			return i;
	}
	return -1;
}


// TODO: if the first token is a label and later there is a comment of '\n' : ignore?
char* translateToken(label_t* labels, char* assemblyToken, char *regOppArr[OPP_AMOUNT])
//takes a token checks which opcode or register it is
{
	char *numString[MAX_LABEL_LENGTH];
	int temp = -1;
	int regOppNum = 0;
	int tokenLength = 0;
	char* tempString[MAX_LABEL_LENGTH];
	strcpy(tempString, assemblyToken);
	if (assemblyToken[0] == '#')
		return NULL;
	tokenLength = strlen(assemblyToken);
	if (tempString[tokenLength - 1] == '#') 		// remove '#' from potential opp/reg/number
		tempString[tokenLength] = '\0';
	if (isNumber(tempString) == -1)				//is it a number
		return tempString;
	temp = isLabel(labels, tempString);		//it is a label so:
	if (temp != -1)
	{
		itoa(temp, numString, 10);
		//TODO: the label is the only one in line - ignore?
		return numString;					//replace the label in label->line
	}
	//isRegister : starts with $ , in regHashTable
	regOppNum = findRegOpp(regOppArr, tempString);
	if (regOppNum >= 0)
	{
		if (regOppNum < 16)
			sprintf(numString, "%x", regOppNum);
		else
		{
			sprintf(numString, "%x", (regOppNum-16));
		}
		return numString;
	}
	if ((strcmp(tempString, ".word")==0))
		return 
								//isOpcode : in opHashTable
}
//TODO: take care of '#': remove, move to next assemblyLine
//TODO: assemble data stracture for opcodes
//TODO: assemble data stracture for registers
void secondPass(char* fileName, label_t* labels, char* regOppArr[OPP_AMOUNT]) //TODO: adding memin
{
	FILE* fptr;
	//FILE* memin
	char assemblyLine[500];
	char *temp[MAX_LABEL_LENGTH];
	char *outputString[MAX_LENGTH] ;
	char *assemblyToken;
	int locationCounter = 0;
	int tokenLength = 0;
	int containsEndCommand = FALSE;
	int counter = 0;

	outputString[0] = '\0';
	if ((fptr = fopen(fileName, "r")) == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}
	while (fgets(assemblyLine, MAX_LENGTH, fptr))		//while not EOF
	{
		locationCounter++;		//new assembly line
		while (assemblyToken != NULL)
		{
			assemblyToken = strtok(assemblyLine, " ,-\t");		 //tokening
			strcpy(temp, translateToken(labels, assemblyToken, regOppArr));		//translating
			if (temp == NULL)		//if it is a label that is alone in line?
				break;
			if ((strcmp(temp, ".word")) == 0)
			{
				while ((assemblyToken != NULL)&&(counter <=2))
				{
					assemblyToken = strtok(assemblyLine, " ,-\t");
					//TODO: 1 - adress (isNumber) , 2 - value (isNumber)
				}
			}
			strcat(outputString, temp);
		}
		if (assemblyToken == NULL)		//beacause an exaption was thrown otherwise
		{
			//TODO:	write outputstring to memin + '\n'
			//TODO: memset to outputstring '\0'
			continue; //add a test print later
		}
	}
	fclose(fptr);
	// TODO : close memin
}


int main(int argc, char *args[])
{
	char *regOppArr[32];
	char* fileName = args[1];
	label_t* labels = NULL;
	firstPass(fileName, labels);
	construckHashTable(regOppArr);
	secondPass(fileName, labels, regOppArr);
	//TODO: second pass
	//TODO: free() for every pointer/ linked list - or just write exit(0)
}