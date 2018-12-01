#include <stdio.h>

#define NUM_OF_REGISTERS 16

void runSimulator(char* inMemoryFilename)
{
	int registers[NUM_OF_REGISTERS];
	for (int i = 0; i < NUM_OF_REGISTERS; i++)
	{
		registers[i] = 0;
	}


	// Fetch
	// Decode
	// Execute
}

int main(int argc, char *args[])
{
	char* inMemoryFilename = args[1];
}