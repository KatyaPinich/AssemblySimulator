#include <stdio.h>
#include "commons.h"
#include "command_line_arguments.h"
#include "simulator.h"

/*
** Summary: 
**		Main entry point to the program. It takes a memin.txt file as input 
**		and simulates the fetch-decode-execute loop of a processor to execute the 
**		input program.
** Parameters:
**		argc - number of command line arguments passed
**		argv - an array containing the command line arguments passed
*/
int main(int argc, char *argv[])
{
	checkCommandLineArguments(argc);
	runSimulator(argv);
}