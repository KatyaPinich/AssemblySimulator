#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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
	// marks each label and keeps their adress as instructed (location, name) in a linked list//
	char assemblyLine[500] ;
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
			if( (assemblyToken[tokenLength - 1] == ':')||(assemblyToken[tokenLength - 2] == ':'))
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

int isLabel(label_t* head, char* x)
{
// Checks whether the value x is present in linked list */
	label_t* current = head;  // Initialize current 
	while (current != NULL)
	{
		if (strcmp(current->Name, x)==0)
			return TRUE;
	current = current->next;
	}
	return FALSE;
}


//TODO: assemble data stracture for opcodes
//TODO: assemble data stracture for registers
void secondPass(char* fileName, label_t* labels)
{
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
		printf("test: Splitting string \"%s\" into tokens:\n", assemblyLine);
		assemblyToken = strtok(assemblyLine, " ,.-");
		while (assemblyToken != NULL) 	
		{
			//TODO: parse the command line
			//TODO: translate each command line
			printf("%s\n", assemblyToken);
			assemblyToken = strtok(NULL, " ,.-");
			//TODO: write each command line in Memin as
		}
		assemblyToken = strtok(assemblyLine, " \t");
		if (assemblyToken == NULL)		//beacause an exaption was thrown otherwise
			break;
		if (assemblyToken[0] == '#') 		// Check line is not a comment
			continue;
	}
	fclose(fptr);
}


int main(int argc, char *args[])
{
	char* fileName = args[1];
	label_t* labels = NULL;
	firstPass(fileName, labels) ;
	//TODO: second pass
	//TODO: free() for every pointer/ linked list - or just write exit(0)
}