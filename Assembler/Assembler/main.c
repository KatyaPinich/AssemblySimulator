#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 501
#define MAX_LABEL_LENGTH 51
#define OPP_AMOUNT 18
#define MEMORY_SIZE 4096
#define NUMBER_OF_LEGAL_ARGS 5
#define LABELS_AMOUNT 40

typedef struct label
{
	char Name[MAX_LABEL_LENGTH];
	int Line;
	struct label *next;
} label_t;

char* initializeOppcodesTable()
{
	char *opps[OPP_AMOUNT] = { "add", "sub", "and", "or", "sll", "sra", "mac", "branch", "res", "res", "res", "jal", "lw", "sw", "jr", "halt" };
	return opps;
};
char *initializeRegistersTable()
{
	char *opps[OPP_AMOUNT] = { "add", "sub", "and", "or", "sll", "sra", "mac", "branch", "res", "res", "res", "jal", "lw", "sw", "jr", "halt" };
	return opps;
};


int findIndex(char *string, int size, char target)
{
	int i = 0;
	while ((i < size) && (string[i] != target)) i++;
	return (i < size) ? (i) : (-1);
}

int findRegNumber(char *regs[], char* target)
{
	int i = 0;
	while ((i < OPP_AMOUNT) && (strcmp(target, regs[i])!=0)) i++;
	return (i < OPP_AMOUNT) ? (i) : (-1);
}

int findOppCode(char *opps[], char* target)
{
	int i = 0;
	while ((i < OPP_AMOUNT) && (strcmp(target, opps[i]) != 0)) i++;
	return (i < OPP_AMOUNT) ? (i) : (-1);
}

label_t* addLable(label_t *head, char* assemblyToken, int tokenLength, int locationCounter)
{
	char tempString[MAX_LABEL_LENGTH];
	label_t *newLabel = (label_t*)malloc(sizeof(label_t));		//allocation
	char target;
	int targetIndex = 0;
	if (newLabel == NULL)	//exception//
		return NULL;
	targetIndex = findIndex(assemblyToken, tokenLength, ':');	//index of ':'
	strcpy(tempString, assemblyToken);
	tempString[targetIndex] = '\0';		//removes ':'
	strcpy(newLabel->Name, tempString);		//construct//
	newLabel->Line = locationCounter;
	newLabel->next = head;
	return newLabel;
}

void firstPass(char* fileName, label_t* labels )
{
	// marks each label and keeps their adress as instructed (location, name) in a linked list//
	char assemblyLine[MAX_LENGTH];
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
				labels = addLable(labels, assemblyToken, tokenLength, locationCounter);
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

int isLabel(label_t* head, char* x[])
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
	if ((assemblyToken[0] == '0')&(assemblyToken[1] == 'x'))
		return -2; //hexa number flag
	for (i = 0; i < length; i++)
	{
		if (!isdigit(assemblyToken[i]))
			return i; //number is possibly corrupted
	}
	return -1; // dec number flag
}

void illegalCommand()
{
	printf("ERROR: illegal command!");
	exit(1);
}

void wordSubCase(char *assemblyToken, char *memory[])
{

}

char* parseAssemblyLine(label_t* labels, char *assemblyLine, char *regs[], char *opps[] )
{
	char *assemblyToken;
	char* temp;
	int counter = 0;
	int tempValue;
	char* outputString;
	int commentIndex = 0;
	int size = strlen(assemblyLine);
	commentIndex = findIndex(assemblyLine,  size,  '#');
	assemblyLine[commentIndex] = '\0';
	assemblyToken = strtok(assemblyLine, " ,-\t");		 //tokening
	if (isLabel(labels, assemblyToken)!=-1)
		assemblyToken = strtok(assemblyLine, " ,-\t");		 //tokening
	while (assemblyToken != NULL)
	{
		if ((counter > NUMBER_OF_LEGAL_ARGS) || ((counter < NUMBER_OF_LEGAL_ARGS) && (assemblyToken == NULL)))
		{	
			illegalCommand();
		}
		switch (counter)
		{
			case (0):
			{
				tempValue = findOppCode(opps, assemblyToken);
				if (tempValue == -1)
				{
					illegalCommand();
				}
				sprintf(temp, '%x', tempValue);
				break;
			}
			case (5):
			{
				if ((tempValue = isLabel(labels, assemblyToken)) == -1) //not a label
				{
					tempValue = isNumber(assemblyToken);
					if (tempValue == -1)
					{
						if (assemblyToken[tempValue] == '#') //incase there are 2 '#' somehow
						{
							assemblyToken[tempValue] = '\0'; //chopping the undesured part
						}
						else
						{
							illegalCommand();
						}
							
					}
					if (tempValue == -2)
					{
						strcpy(temp, (assemblyToken+2));
					}
				}
				sprintf(temp, '%x', tempValue); //number or label
				break;
				
			}
			default:
			{
				tempValue = findRegNumber(regs, assemblyToken);
				if (tempValue == -1)
				{
					illegalCommand();
				}
				sprintf(temp, '%x', tempValue);
				break;
			}
		}
		strcat(outputString, temp);
		temp[0] = '\0';
		assemblyToken = strtok(assemblyLine, " ,-\t");		 //tokening
		counter++;
	}
	return outputString;
}


void secondPass(char* fileName, label_t* labels, char* regs[], char *opps []) //TODO: adding memin
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
	char* memory[MEMORY_SIZE];

	if ((fptr = fopen(fileName, "r")) == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}
	while (fgets(assemblyLine, MAX_LENGTH, fptr))		//while not EOF
	{
		strcpy(outputString, parseAssemblyLine( labels, assemblyLine, regs, opps));
		locationCounter++;		//new assembly line
		
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
	char *regs;
	char *opps;
	char* fileName = args[1];
	label_t* labels = NULL;
	firstPass(fileName, labels);
	//firstPass(fileName, labels);
	regs = initializeRegistersTable();
	opps = initializeOppcodesTable();
	secondPass(fileName, labels, regs, opps);
	//secondPass(fileName, labels, regs, opps);
	exit(0);
}