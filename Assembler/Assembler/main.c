#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//### Max lablel length: 50 , Max commandline: 500 therfore: ###
#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 501
#define MAX_LABEL_LENGTH 51

typedef struct label
{
	char Name[MAX_LABEL_LENGTH];
	int Line;
	struct label *next;
} label_t;

int findIndex(char *string, int size, char target)
{
	int i = 0;
	while ((i < size) && (string[i] != target)) i++;

	return (i < size) ? (i) : (-1);
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

void firstPass(char* fileName , label_t* labels )
{
	// marks each label and keeps their adress as instructed//
	int num;
	char assemblyLine[500] ;
	char *assemblyToken;
	int locationCounter = 0;
	int tokenLength = 0;
	int containsEndCommand = FALSE;
	FILE* fptr;
	if ((fptr = fopen(fileName, "r")) == NULL) {
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
			if( (assemblyToken[tokenLength - 1] == ':')||(assemblyToken[tokenLength - 2] == ':'))
			{
				// If it is a label record it and location counter
				// TODO: Check the label does not exist
				// TODO: Check label name is valis - starts with a character
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

	//strcpy(assemblyLine, newstring);
}


int main(int argc, char *args[])
{
	char* fileName = args[1];
	label_t* labels = NULL;
	firstPass(fileName, labels) ;
	
}