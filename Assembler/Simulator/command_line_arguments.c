#include <string.h>
#include "command_line_arguments.h"

#define NUM_OF_ARGUMENTS 6

void checkCommandLineArguments(int argc)
{
	if (argc != NUM_OF_ARGUMENTS)
	{
		printf("error: not enough arguments");
		printf("usage: sim.exe <memin.txt> <memout.txt> <regout.txt> <trace.txt> <count.txt>");
		exit(1);
	}
}