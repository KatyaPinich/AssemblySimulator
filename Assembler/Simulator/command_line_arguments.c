#include <string.h>
#include "command_line_arguments.h"

#define NUM_OF_ARGUMENTS 5

CommandLineArguments* getCommandLineArgs(int argc, char* argv[])
{
	if (argc != NUM_OF_ARGUMENTS + 1)
	{
		printf("Number of arguments does not match.");
		exit(1);
	}

	CommandLineArguments commandLineArguments;
	commandLineArguments.inputMemoryFilename = strdup(argv[1]);
	commandLineArguments.outputMemoryFilename = strdup(argv[2]);
	commandLineArguments.registersFilename = strdup(argv[3]);
	commandLineArguments.traceFilename = strdup(argv[4]);
	commandLineArguments.countFilename = strdup(argv[5]);

	return &commandLineArguments;
}