#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 501
#define MAX_LABEL_LENGTH 51
#define OPPS_REGS_TOTAL_LENGTH 180
#define MEMORY_REGISTER_SIZE 13		
#define MEMORY_SIZE 4098			
#define NUMBER_OF_LEGAL_ARGS 5
#define LABELS_AMOUNT 40

typedef struct label
{
	char Name[MAX_LABEL_LENGTH];
	int Line;
	struct label *next;
} label_t;


int  strToInt(char* adress)
{
	int mult = 1;
	int result = 0;
	int len = strlen(adress);
	for (int i = len - 1; i >= 0; i--)
	{
		result = result + ((int)adress[i] - 48)*mult;
		mult = mult * 10;
	}
	return result;
}

void upperString(char temp[]) {
	int c = 0;

	while (temp[c] != '\0')
	{
		if (temp[c] >= 'a' && temp[c] <= 'z')
		{
			temp[c] = temp[c] - 32;
		}
		c++;
	}
}
 
int findIndex(char *string, int size, char target)
{
	int i = 0;
	while ((i < size) && (string[i] != target)) i++;
	return (i < size) ? (i) : (-1);
}

int findRegNumber(char *regs[], char* target)
{
	int i = 0;
	char *pos = regs[i];
	while (pos != '\0')
	{
		if (strcmp(pos, target) == 0)
			return i;
		i++;
		pos = regs[i];
		if (i == 16)
			return -1;
	}

	//while ((i < OPPS_REGS_TOTAL_LENGTH) && (strcmp(target, *(opps[i])) != 0)) i++;
	//return (i < OPPS_REGS_TOTAL_LENGTH) ? (i) : (-1);
}

int findOppCode(char *opps[], char* target)
{
	int i = 0;
	char *pos = opps[i];
	while (pos!= '\0')
	{
		if (strcmp(pos,target)==0)
			return i;
		i++;
		pos = opps[i];
		if (i == 16)
			return -1;
	}

	//while ((i < OPPS_REGS_TOTAL_LENGTH) && (strcmp(target, *(opps[i])) != 0)) i++;
	//return (i < OPPS_REGS_TOTAL_LENGTH) ? (i) : (-1);
}

label_t* addLable(label_t *head, char *assemblyToken, int tokenLength, int locationCounter)
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

label_t* firstPass(char* fileName, label_t* labels)
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
		assemblyToken = strtok(assemblyLine, " ,-\t");
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
	return labels;
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

int isNumber(char* assemblyToken)
{
	// returns (-1) IF it's a number, ELSE: the first index of the non-number char in assemblyToken
	int i;
	int length;
	length = strlen(assemblyToken);
	if ((assemblyToken[0] == '0')&(assemblyToken[1] == 'x'))
	{
		assemblyToken = strtok(NULL, "0x");
		return -2; //hexa number flag
	}

	for (i = 0; i < length; i++)
	{
		if (!isdigit(assemblyToken[i]))
			return i; //number is possibly corrupted ->>illegal command
	}
	return -1; // dec number flag
}

int isWord(char* assemblyToken)
{
	if (strcmp(assemblyToken, ".word") == 0)
		return TRUE;
	return FALSE;

}

void illegalCommand()
{
	printf("ERROR: illegal command!");
	exit(1);
}

void wordSubCase(char *assemblyToken, char **memory[])//assemblyToken has already tokened once
{
	char *adress[5];
	int resultAdress;
	char *value [13];
	memset(adress, '\0', sizeof(adress));
	memset(value, '\0', sizeof(value));
	assemblyToken = strtok(NULL, " ,-\t");		 //Re-tokening
	strcpy(adress, assemblyToken);
	assemblyToken = strtok(NULL, " ,-\t\n");		 //Re-tokening
	strcpy(value, assemblyToken);
	resultAdress = strToInt(adress);
	if (resultAdress > 4096) //|| (strlen(value) > 12))
	{
		illegalCommand();
	}
//	memory[resultAdress] = value;
}


char* parseAssemblyLine(label_t* labels, char *assemblyLine, char *regs[], char *opps[] )
{
	char *assemblyToken;
	char temp[MAX_LABEL_LENGTH];
	int counter = 0;
	int tempValue;
	int tempValue2;
	char* outputString[100];
	int commentIndex = 0;
	int size = strlen(assemblyLine);
	char *labelFlag = "L";
	char *wordFlag = "W";
	char *emptyLineFlag = "N";
	commentIndex = findIndex(assemblyLine,  size,  '#');
	assemblyLine[commentIndex] = '\0';
	assemblyToken = strtok(assemblyLine, " ,-\t:");		 //tokening
	if (isLabel(labels, assemblyToken) != -1)
	{
		assemblyToken = strtok(NULL, " ,-\t:");		 //tokening
		if ((assemblyToken == NULL)||(tempValue = findOppCode(opps, assemblyToken)==-1))
		{
			return labelFlag;
		}
	}
	if (isWord(assemblyToken))
	{
		return wordFlag;
	}
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
				if (strcmp(assemblyToken, "\n") == 0)
					return emptyLineFlag;
				tempValue = findOppCode(opps, assemblyToken);
				if (tempValue == -1)
				{
					illegalCommand();
				}
				sprintf(temp, "%X", tempValue);
				strcpy(outputString, temp);
				counter++;
				assemblyToken = strtok(NULL, " ,-\t");		 //Re-tokening
				break;
			}
			case (5):
			{
				if ((tempValue = isLabel(labels, assemblyToken)) == -1) //not a label
				{
					tempValue = isNumber(assemblyToken);
					if (tempValue > -1)
					{
						illegalCommand();
					}
					if (tempValue == -1)
					{
						tempValue2 = atoi(assemblyToken);

						itoa(tempValue2, temp, 16); //from dec to hex
					}
					if (tempValue == -2)
					{
						tempValue2 = atoi(assemblyToken);
						itoa(tempValue2, temp, 10); //already hex
					}
				}
				strupr(temp);//BUGGED
				strcat(outputString, temp);
				counter++;
				assemblyToken = strtok(NULL, " ,-\t");		 //Re-tokening
				break;
			}
			case(4): //break is in the condition in order to move next to default if it's not a number
			{
				tempValue = isNumber(assemblyToken);
				if (tempValue == -1)
				{
					tempValue2 = atoi(assemblyToken);
					itoa(tempValue2, temp, 16); //from dec to hex
					upperString(temp);
					strcat(outputString, temp);
					counter++;
					assemblyToken = strtok(NULL, " ,-\t");		 //Re-tokening
					break;
				}

			}
			default:
			{
				tempValue = findRegNumber(regs, assemblyToken);
				if (tempValue == -1)
				{
					illegalCommand();
				}
				sprintf(temp, "%X", tempValue);
				strcat(outputString, temp);
				counter++;
				assemblyToken = strtok(NULL, " ,-\t");		 //Re-tokening
				break;
			}
		}
	}
	return outputString;
}

void secondPass(char* fileName, label_t* labels, char* regs[], char *opps [], char **memory[]) //TODO: adding memin
{
	FILE* fptr;
	//FILE* memin
	char *assemblyLine[500];
	char *outputString;
	char *assemblyToken;
	int locationCounter = 0;
	int tokenLength = 0;
	int containsEndCommand = FALSE;
	int counter = 0;
	if ((fptr = fopen(fileName, "r")) == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}
	while (fgets(assemblyLine, MAX_LENGTH, fptr))		//while not EOF
	{
		outputString = parseAssemblyLine( labels, assemblyLine, regs, opps);
		if (strcmp(outputString, "L")==0)
		{
			//locationCounter++;//doesnt write to memory
			//labelSubCase(memory)?
			continue;
		}
		if (strcmp(outputString, "N") == 0)
		{
			locationCounter++;//doesnt write to memory
			continue;
		}
		else if (strcmp(outputString, "W") == 0)
		{
			locationCounter++;
			wordSubCase(assemblyLine, memory); //write to memory in "adress"
			continue;
		}
		else
		{
			memory[locationCounter] = outputString; //char vs register?
			locationCounter++;		//new assembly line
		}
	}
	//free(assemblyLine);
	fclose(fptr);

	// TODO : close memin
}

void meminWrite(char **memory)
{
	int i = 1; //writing to memory from the 1st line
	FILE *memin;
	memin = fopen("memin.txt", "w");
	while(i <= MEMORY_SIZE)
	{
		fprintf(memin, "%s\n", memory[i++]);
	}
	fclose(memin);
}

int main(int argc, char *args[])
{
	char **memory[MEMORY_SIZE] = {NULL};
	const char *regs[] = { "$zero", "$at", "$v0", "$a0", "$a1", "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2", "$gp","$sp", "$fp", "$ra" };
	const char *opps[] = { "add", "sub", "and", "or", "sll", "sra", "mac", "branch", "res", "res", "res", "jal", "lw", "sw", "jr", "halt" };
	char* fileName = args[1];
	label_t* labels = NULL;
	labels = firstPass(fileName, labels);
	secondPass(fileName, labels, regs, opps, memory);
	meminWrite(memory);
	exit(0);
}