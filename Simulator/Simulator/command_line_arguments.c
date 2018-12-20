#include <string.h>
#include "command_line_arguments.h"

#define NUM_OF_ARGUMENTS 6

/*
** Summary:
**		Checks whether the amount of command line arguments passed matches
**		the number of command line arguments expected.
** Parameters:
**		argc - number of command line arguments passed
*/
void checkCommandLineArguments(int argc)
{
	if (argc != NUM_OF_ARGUMENTS)
	{
		printf("error: not enough arguments");
		printf("usage: sim.exe <memin.txt> <memout.txt> <regout.txt> <trace.txt> <count.txt>");
		exit(1);
	}
}