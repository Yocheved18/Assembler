#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "text_correctness.h"
#include "globals.h"




typedef struct  MacroList * ptrMacro;/*This line defines a new type ptrMacro as a pointer to a struct MacroList*/

/*This defines a structure called macronode that represents a node in a linked list. The macronode structure has four fields:*/
typedef struct MacroList {
	char *macroName;/*name stores the name of the macro*/
	char *content;/*content is an array of strings that stores the content of the macro*/
	int index;/*keep track of the current position in the content array where the next character should be added when a macro definition is being read and stored*/
	ptrMacro next;/*next is a pointer to the next node in the linked list*/
} MacroList;

bool macroRetire(FILE *file_as, FILE *file_am);/*It reads lines from the input file, checks if they define a macro, and if so, stores the macro name and content in a linked list. If a line does not define a macro, it writes the line to the output file, possibly substituting any defined macro names with their corresponding macro content.*/

void writesFile(FILE *, char *);/*this function takes a pointer to a file and a string as input, and writes the string to the specified file- opens the file(".am") and inserts a line into it*/

bool addToMacroList(char *, ptrMacro *);/*this function adds a new macro to a linked list of macros. It takes a string that represents the macro name and a pointer to the head of the macro list. It creates a new node for the macro and inserts it at the beginning of the list. The content of the macro is stored in the node's content array.*/

void freeMacroList(ptrMacro *);/*This function frees the memory used by the linked list of macros. It takes a pointer to a pointer that points to the head of the list as its input. The function iterates through the list, freeing the memory allocated for each macro and updating the next pointer of each node until the end of the list is reached. Finally, it sets the head of the list to NULL to indicate that it has been freed.*/

ptrMacro sameMacro(char *,  ptrMacro);/* this function searches a linked list of macros for a macro with a given name. It takes two arguments: a pointer to a string containing the name of the macro to search for, and a pointer to the head of the linked list. If a macro with the given name is found, the function returns a pointer to that macro; otherwise, it returns NULL.*/



#endif

