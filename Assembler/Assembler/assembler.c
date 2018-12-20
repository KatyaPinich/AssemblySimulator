#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "commons.h"

enum instructionComponents
{
	Opcode = 0,
	Rd = 1,
	Rs = 2,
	Rt = 3,
	Rm = 4,
	Immediate = 5
};

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
}

int findOppCode(char *opps[], char* target)
{
	int i = 0;
	char *pos = opps[i];
	while (pos != '\0')
	{
		if (strcmp(pos, target) == 0)
			return i;
		i++;
		pos = opps[i];
		if (i == 16)
			return -1;
	}
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

int isLabelLineEmpty(char* assemblyToken)
{
	assemblyToken = strtok(NULL, " ,\t");
	return ((assemblyToken == NULL) || (assemblyToken[0] == '#') || (assemblyToken[0] == '\n'));
}

label_t* firstPass(char* assemblyFile, label_t* labels)
{
	// marks each label and keeps their adress as instructed (location, name) in a linked list//
	char assemblyLine[MAX_LENGTH];
	char *assemblyToken;
	int locationCounter = 0;
	int tokenLength = 0;
	int containsEndCommand = FALSE;
	FILE* fptr;
	if ((fptr = fopen(assemblyFile, "r")) == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}

	while (fgets(assemblyLine, MAX_LENGTH, fptr))		//while not EOF
	{
		assemblyToken = strtok(assemblyLine, " ,\t");
		if (assemblyToken == NULL)
			break;
		else if ((strcmp(assemblyToken, "\n") == 0) || (strcmp(assemblyToken, ".word") == 0))
			continue;		//new assembly line
		else if (assemblyToken[0] != '#') 		// Check line is not a comment
		{
			tokenLength = strlen(assemblyToken);		//checks if it's a Label
			if ((assemblyToken[tokenLength - 1] == ':') || (assemblyToken[tokenLength - 2] == ':'))
			{
				labels = addLable(labels, assemblyToken, tokenLength, locationCounter);
				if (isLabelLineEmpty(assemblyToken))
					continue;
			}
			else if (strcmp(assemblyToken, "halt") == 0)
			{
				containsEndCommand = TRUE;
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
	return labels;
}

int isLabel(label_t* head, char* x)
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
	int i = 0;
	int length = 0;
	length = strlen(assemblyToken);
	if ((assemblyToken[0] == '0') && (assemblyToken[1] == 'x'))
	{
		assemblyToken += 2;
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

int parseAdressValue(char *assemblyToken)
{
	int resultAdressOrValue;
	char adressOrValue[13] = "";
	int isNegative = FALSE;
	assemblyToken = strtok(NULL, " ,\t\n");
	strcpy(adressOrValue, assemblyToken);
	if (assemblyToken[0] == '-')
	{
		isNegative = TRUE;
		assemblyToken++;
	}
	if (isNumber(assemblyToken) == -2) //hexa value
	{
		resultAdressOrValue = (int)strtoul(adressOrValue, NULL, 16);
	}
	else if (isNumber(assemblyToken) == -1)
	{
		resultAdressOrValue = strToInt(adressOrValue);
	}
	else
	{
		illegalCommand();
	}
	if (isNegative)
	{
		resultAdressOrValue *= (-1);
	}
	return resultAdressOrValue;
}

void wordSubCase(char *assemblyToken, int memory[])//assemblyToken has already tokened once
{
	int resultAdress;
	int resultValue;
	resultAdress = parseAdressValue(assemblyToken);
	resultValue = parseAdressValue(assemblyToken);
	if (resultAdress > 4096)
	{
		illegalCommand();
	}
	memory[resultAdress + 1] = resultValue;
}

char* parseNumber(char *assemblyToken, label_t* labels)
{
	int isLabelName = 0;
	int isNumericValue = 0;
	int numericValue = 0;
	char hexString[9] = "";
	char outputImmediate[9] = "";
	char tempString[9];
	if ((isLabelName = isLabel(labels, assemblyToken)) == -1) //not a label
	{
		isNumericValue = isNumber(assemblyToken);
		if (isNumericValue > -1)
		{
			illegalCommand();
		}
		if (isNumericValue == -1)
		{
			numericValue = atoi(assemblyToken);
			sprintf(hexString, "%X", numericValue);
		}
		if (isNumericValue == -2)
		{
			strcpy(tempString, (assemblyToken + 2));
			if (strlen(tempString) == 1)
				sprintf(hexString, "00%s", tempString);
			else if (strlen(tempString) == 2)
				sprintf(hexString, "0%s", tempString);
			//numericValue = atoi(assemblyToken+2);
			//itoa(numericValue, hexString, 10); //already hex
		}
	}
	else
	{
		itoa(isLabelName, hexString, 16); //from dec to hex
		sprintf(hexString, "%3X", isLabelName);
	}

	return hexString;
}

void chopFirstFiveIndexes(char *hexString)
{
	char *temp = hexString;
	temp += 5;
	strcpy(hexString, temp);
}

char* parseImmediate(char *assemblyToken, label_t* labels)
{
	int isLabelName = 0;
	int isNumericValue = 0;
	int numericValue = 0;
	char hexString[9];
	char tempString[9];
	int isNegative = FALSE;

	// TODO: Handle negative numbers
	if ((isLabelName = isLabel(labels, assemblyToken)) == -1) //not a label
	{
		if (assemblyToken[0] == '-')
		{
			isNegative = TRUE;
			assemblyToken++;
		}

		isNumericValue = isNumber(assemblyToken);
		if (isNumericValue > -1)
		{
			illegalCommand();
		}
		if (isNumericValue == -1)
		{
			numericValue = atoi(assemblyToken);
			if (isNegative)
			{
				numericValue *= (-1);
			}
			sprintf(hexString, "%03X", numericValue);
			if (isNegative)
				chopFirstFiveIndexes(hexString);
		}
		if (isNumericValue == -2)
		{
			strcpy(tempString, (assemblyToken + 2));
			if (strlen(tempString) == 1)
				sprintf(hexString, "00%s", tempString);
			else if (strlen(tempString) == 2)
				sprintf(hexString, "0%s", tempString);
			else
				sprintf(hexString, "%s", tempString);
			//numericValue = atoi(assemblyToken+2);
			//itoa(numericValue, hexString, 10); //already hex
		}
	}
	else
	{
		itoa(isLabelName, hexString, 16); //from dec to hex
		sprintf(hexString, "%03X", isLabelName);
	}

	return hexString;
}

int parseAssemblyCommand(char *assemblyToken, label_t* labels, char *opps[], char * regs[])
{
	int counter = 0;
	int oppCode = 0;
	char temp[9] = "";
	char instruction[9] = "";
	char *immediateValue;
	int tempValue;
	long outputNum = 0;

	while (assemblyToken != NULL)
	{
		if ((counter > NUMBER_OF_LEGAL_ARGS) || ((counter < NUMBER_OF_LEGAL_ARGS) && (assemblyToken == NULL)))
		{
			illegalCommand();
		}
		switch (counter)
		{
		case (Opcode):
		{
			if (strcmp(assemblyToken, "\n") == 0)
				return -1;
			oppCode = findOppCode(opps, assemblyToken);
			if (oppCode == -1)
			{
				illegalCommand();
			}
			sprintf(temp, "%X", oppCode);
			strcpy(instruction, temp);
			counter++;
			assemblyToken = strtok(NULL, " ,\t");		 //Re-tokening
			break;
		}
		case (Immediate):
		{
			immediateValue = parseImmediate(assemblyToken, labels);
			strcat(instruction, immediateValue);
			counter++;
			assemblyToken = strtok(NULL, " ,\t");		 //Re-tokening
			break;
		}
		case(Rm): //break is in the condition in order to move next to default if it's not a number
		{
			tempValue = isNumber(assemblyToken);
			if (tempValue == -1)
			{
				immediateValue = parseNumber(assemblyToken, labels);
				strcat(instruction, immediateValue);
				counter++;
				assemblyToken = strtok(NULL, " ,\t");		 //Re-tokening
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
			strcat(instruction, temp);
			counter++;
			assemblyToken = strtok(NULL, " ,\t");
			//Re-tokening
			break;
		}
		}
	}
	outputNum = (long)strtoul(&instruction, NULL, 16);
	return outputNum;
}

void parseAssemblyLine(label_t* labels, char *assemblyLine, char *regs[], char *opps[], int memory[], int* memoryCounter)
{
	char *assemblyToken;
	int tempValue;
	int outputNum = 0;
	int commentIndex = 0;
	int size = strlen(assemblyLine);
	memory[*memoryCounter] = 0;//doesnt have to 
	commentIndex = findIndex(assemblyLine, size, '#');
	if (commentIndex != -1)
	{
		assemblyLine[commentIndex] = '\0';
	}

	assemblyToken = strtok(assemblyLine, " ,-\t:");		 //tokening
	if (assemblyToken == NULL)
	{
		return;
	}
	if (isLabel(labels, assemblyToken) != -1)
	{
		assemblyToken = strtok(NULL, " ,-\t:");		 //tokening
		tempValue = findOppCode(opps, assemblyToken);
		if ((assemblyToken != NULL) && (tempValue != -1))
		{
			outputNum = parseAssemblyCommand(assemblyToken, labels, opps, regs);
			if (outputNum != -1)
			{
				memory[*memoryCounter] = outputNum;
				(*memoryCounter)++;
			}
		}
	}
	else if (isWord(assemblyToken))
	{
		//(*memoryCounter)++;
		wordSubCase(assemblyLine, memory);
	}
	else
	{
		outputNum = parseAssemblyCommand(assemblyToken, labels, opps, regs);
		if (outputNum != -1)
		{
			memory[*memoryCounter] = outputNum;
			(*memoryCounter)++;
		}
	}
}

void secondPass(char* assemblyFile, label_t* labels, char* regs[], char *opps[], int memory[]) //TODO: adding memin
{
	FILE* fptr;
	char assemblyLine[MAX_LENGTH];
	int memoryCounter = 1;
	memset(assemblyLine, '\0', sizeof(assemblyLine));
	if ((fptr = fopen(assemblyFile, "r")) == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}

	while (fgets(assemblyLine, MAX_LENGTH, fptr))		//while not EOF
	{
		parseAssemblyLine(labels, assemblyLine, regs, opps, memory, &memoryCounter);
	}
	fclose(fptr);
}

void meminWrite(int memory[], char *meminFile)
{
	char lineForMemin = "";
	int i = 0; //writing to memory from the 1st line
	int maxIndex = 0;
	FILE *memin;
	if ((memin = fopen(meminFile, "w")) == NULL)
	{
		printf("Error! opening file");
		exit(1);
	}
	while (i < MEMORY_SIZE)
	{
		if (memory[i] != NULL)
			maxIndex = i;
		i++;
	}
	i = 1;
	while (i <= maxIndex)
	{
		if (memory[i] != NULL)
		{
			fprintf(memin, "%08X\n", memory[i]);
		}
		else
		{
			fprintf(memin, "%s", "00000000\n");
		}
		i++;
	}
	fclose(memin);
}