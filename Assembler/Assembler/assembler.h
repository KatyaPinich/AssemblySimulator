#ifndef ASSEMBLER_H
#define ASSEMBLER_H

label_t* firstPass(char* assemblyFile, label_t* labels);
void secondPass(char* assemblyFile, label_t* labels, char* regs[], char *opps[], int memory[]);
void meminWrite(int memory[], char *meminFile);


#endif
