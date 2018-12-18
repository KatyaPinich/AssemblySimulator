#include <stdio.h>
#include "commons.h"
#include "command_line_arguments.h"
#include "simulator.h"

int main(int argc, char *argv[])
{
	checkCommandLineArguments(argc);
	runSimulator(argv);
}