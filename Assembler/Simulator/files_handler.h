#ifndef FILES_HANDLER_H
#define FILES_HANDLER_H

void loadMemory(char* inputMemoryFilename, int memory[], int memorySize);

void writeTrace(char* traceFilename, int pc, int instruction, int registers[]);

void writeMemoryToFile(char* memoutFilename, int memorySize, int memory[]);

void writeRegistersToFile(char* registersFilename, int numberOfRegisters, int registers[]);

void writeCountToFile(char* countFilename, int instructionCount);

#endif
