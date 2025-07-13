#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler.h"
#include "text_correctness.h"
#include "first_pass.h"
#include "second_pass.h"
#include "globals.h"

/*This function handles the entire process of assembling an assembly file. It receives a file name, reads the input file, performs pre-assembly processing using the macroRetire function, and then performs a two-pass assembly process using the first_pass and second_pass functions. If an error is encountered during the assembly process, the function prints an error message and returns a non-zero value. If the assembly is successful, it writes the output to a new file with the same name as the input file and a .am extension, and returns a value of 0. The function also manages memory allocation and deallocation for various data structures used during the assembly process.*/
int file_handling(char *file_name);

#endif
