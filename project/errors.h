#ifndef ERRORS_H
#define ERRORS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "text_correctness.h"
#include "globals.h"
#define LENGTH_OF_DATA 5


/*This function processes a given line in an assembly language program, checking if it is an instruction, a command, or a label. It splits the line into separate words and validates its structure. If the line is a command or an instruction, the function calls the appropriate correctness-checking functions for further validation. If successful, it returns a success code, otherwise, it returns an error code, along with an error message indicating the issue.*/
int line_handle_error(char *line,char ic_cur, char *file_name);

/*This function validates the structure and operands of a given assembly command line. It checks the command type, number of operands, and the presence of commas, ensuring the line adheres to the expected format. If the line is valid, it returns a success code; otherwise, it returns an error code along with a descriptive error message. This function is particularly helpful in detecting and reporting syntax errors in assembly language programs.*/
int command_line_correctness(char * file_name,char * line,int ic_cur,int num_of_words,int num_of_parts,int labelFlag,char **seperated_line,int func_type);

/*This function checks the correctness of an instruction line based on the given instruction type. If the instruction type is .data, it calls the data_line_correctness function to check the correctness of the data line. If the instruction type is .string, it calls the string_line_correctness function to check the correctness of the string line. If the instruction type is .extern or .entry, it calls the extern_entry_line_correctness function to check the correctness of the extern/entry line. If any error occurs, the function prints an error message and returns an allocation error code. If no errors occur, the function returns a success code.*/
int instruction_line_correctness(char * file_name,char * line,int ic_cur,int num_of_words,int labelFlag,char **seperated_line,int instruct_type);

/*This function checks the correctness of a line with the ".data" directive. It first checks that there are no spaces before the directive and no comma before the first number. Then, it splits the line into words and checks that each word contains only valid digits or signs. It also checks that there are no consecutive commas or invalid characters in the line. Finally, it checks that there is no comma after the last number. If any error is found, the function prints an error message and returns an allocation error code. Otherwise, it returns a success code.*/
int data_line_correctness(char* line,char ** seperated_line_with_commas,int ic_cur,char * file_name,int labelFlag);

/*This function checks the correctness of a string line in the assembly code. It takes the line, a string array of the separated elements in the line, the current IC value, and the file name as parameters. The function checks that the string is enclosed in double quotes and that it contains valid ASCII characters. If any error is found, the function prints an error message and returns an allocation error code. Otherwise, the function returns a success code.*/
int string_instructions(char * file_name,char * line,int ic_cur,int num_of_words,int labelFlag,char **seperated_line);

/*This function checks the correctness of an extern/entry line in the assembly code. It checks that there is no label flag set for the line and that there are exactly two operands (a label and an instruction). It also checks that the label operand is a valid label. If any errors are found, it prints an error message and returns an allocation error code. Otherwise, it returns a success code.*/
int extern_entery_instructions(char * file_name,char * line,int ic_cur,int num_of_words,char **seperated_line,char * type);

/*This function validates the structure and operands of a given assembly command line with one operand. It checks the command type, number of operands, and the presence of commas, ensuring the line adheres to the expected format. If the line is valid, it returns a success code; otherwise, it returns an error code along with a descriptive error message. This function is particularly helpful in detecting and reporting syntax errors in assembly language programs.*/
int one_operand_type_command(char * file_name,char * line,int ic_cur,int num_of_words,int num_of_parts,int labelFlag,char **seperated_line,int type);

/*This function validates the structure and operands of a given assembly command line with tow operands. It checks the command type, number of operands, and the presence of commas, ensuring the line adheres to the expected format. If the line is valid, it returns a success code; otherwise, it returns an error code along with a descriptive error message. This function is particularly helpful in detecting and reporting syntax errors in assembly language programs.*/
int tow_operand_type_command(char * file_name,char * line,int ic_cur,int num_of_words,int num_of_parts,int labelFlag,char **seperated_line,int type);

/*This function checks the correctness of the source and destination operand in an assembly command for a given command type. It checks if the operand is a register, label, or digit, based on the command's allowed destination operand types, and prints an error message and returns an error code if the operand is not valid or not allowed for the specific command type. If the operand is valid, it returns a success code.*/
int checks_every_word(char *split_line[],char * file_name,int ic_cur,char * operand_type1,char * operand_type2);

/*The function split_line receives a string of characters and splits it into substrings delimited by white spaces, and parentheses. The substrings are stored in an array of strings. The function uses the strtok function to tokenize the input string and then allocates memory for each token in the output array. Finally, it returns the number of substrings if the operation was successful or -1 if there was a memory allocation error. This function is commonly used in compilers and assemblers to parse and analyze source code.*/
int count_split_line_correctness(char *split_line[],char line[]);

int jump_type_command(char * file_name,char * line,int ic_cur,int num_of_words,int num_of_parts,int labelFlag,char **seperated_line,int type);

/*The function split_line receives a string of characters and splits it into substrings delimited by commas, and parentheses. The substrings are stored in an array of strings. The function uses the strtok function to tokenize the input string and then allocates memory for each token in the output array. Finally, it returns the number of substrings if the operation was successful or -1 if there was a memory allocation error. This function is commonly used in compilers and assemblers to parse and analyze source code.*/
int split_line_jmp(char *split_line[],char line[]);

/*The function split_line receives a string of characters and splits it into substrings delimited by white spaces, commas, and parentheses. The substrings are stored in an array of strings. The function uses the strtok function to tokenize the input string and then allocates memory for each token in the output array. Finally, it returns the number of substrings if the operation was successful or -1 if there was a memory allocation error. This function is commonly used in compilers and assemblers to parse and analyze source code.*/
int count_split_line(char *split_line[],char line[]);

#endif

