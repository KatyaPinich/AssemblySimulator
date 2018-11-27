#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_LENGTH 501
#define MAX_LABLE_LENGTH 51

typedef struct label
{
	char Name[MAX_LABLE_LENGTH];
	int Line;
	struct label *next;
} label_t;

label_t* addLable(label_t *head, char* assemblyToken, int tokenLength, int locationCounter)
{
	label_t *newLabel = (label_t*)malloc(sizeof(label_t));
	if (newLabel == NULL)
	{
		return 1;
	}
	//assemblyToken[-1] = '\0';
	strcpy(newLabel->Name, assemblyToken);
	newLabel->Line = locationCounter;
	newLabel->next = head;
	return newLabel;
}

void firstPass(char* fileName , label_t* labels )
{
	int num;
	//char newstring[500] = "";
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
	while (fgets(assemblyLine, MAX_LENGTH, fptr))
	{
		//assemblyLine[0] = '\0';//memset
		//fgets(assemblyLine, MAX_LENGTH, fptr); //this one is corrupting
		assemblyToken = strtok(assemblyLine, " \t");

		if (assemblyToken == NULL) //beacause an exaption was thrown otherwise
			break;
		// Check line is not a comment
		if (assemblyToken[0] != '#')
		{
			// Is this a label
			tokenLength = strlen(assemblyToken);
			if (assemblyToken[tokenLength - 1] == ':')
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
			if (assemblyToken == NULL)
			{
				break;
			}
			locationCounter++;
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