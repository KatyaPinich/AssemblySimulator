#ifndef COMMAND_LINE_ARGUMENTS_H
#define COMMAND_LINE_ARGUMENTS_H

typedef struct
{
	char *inputMemoryFilename;
	char *outputMemoryFilename;
	char *registersFilename;
	char *traceFilename;
	char *countFilename;
} CommandLineArguments;

CommandLineArguments *getCommandLineArgs(int argc, char* argv[]);

#endif
