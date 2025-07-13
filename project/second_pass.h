#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "text_correctness.h"
#include "globals.h"
#define IC_START_ADDRESS 100
#define SIZE_OF_SHORT 15
#define BITS_IN_ADDRESS 14
#define FILENAME_EXT_LEN 4


/*This function performs the second pass of the assembly process. It reads each line of the file, calls the second_pass_handling function for each line to process it, and updates the instruction counter (ic) accordingly. It also creates an .ext file if necessary and calls the entry_label_file function if an .entry directive was found in the file. Finally, it calls the create_obj_file function to create the object file from the data collected during the first and second passes. The function returns 0 if the process is successful and 1 otherwise.*/
int second_pass(FILE * f,char * file_name,int *ic,int *dc,int *sum_symbols,unsigned short **commands,unsigned short **data, symbol_details ***symbol_list,int  is_extern,int is_entery);

/*This function is responsible for handling the second pass of the assembly process. It receives a line of code, reads it and extracts the necessary information in order to generate the machine code. It uses several other functions such as split_line, is_label, is_command, instructer_type, is_register, add_apearing_symbol_to_list, and return_label_adress. The function checks the type of instruction, identifies the operands and calculates their addresses. It also updates the command array and adds any appearing symbols to the symbol list. The function returns an error code if any errors occur during the process.*/
int second_pass_handeling(char *line,FILE * file_ext,int ic,int * ic_second, symbol_details ***symbol_list, int *sum_symbols,unsigned short **commands);

/*This function, handles a jump command in an assembly language program. It processes the operands (labels and registers) and updates the instruction counter accordingly. If a label is used as an operand, it retrieves the label's address and updates the command array with this address. The function returns a success code if the processing is successful, otherwise, it returns an error code.*/
int command_jump_operand(char **seperated_line,int num_of_words,FILE * file_ext,int ic,int * ic_second, symbol_details ***symbol_list, int sum_symbols,unsigned short **commands);

/*This function processes a command with two operands in an assembly language program. It checks if the operands are registers or labels, and updates the instruction counter accordingly. If an operand is a label, it retrieves the label's address and updates the command array with this address. The function returns a success code if the processing is successful, otherwise, it returns an error code.*/
int command_double_operand(char **seperated_line,FILE * file_ext,int ic,int * ic_second, symbol_details ***symbol_list, int sum_symbols,unsigned short **commands);

/*This function processes a command with one operand in an assembly language program. It checks if the operand is a label and, if so, retrieves the label's address. If the label is found, the function updates the command array with the label's address and increments the instruction counter accordingly. The function returns a success code if the processing is successful, otherwise, it returns an error code.*/
int command_single_operand(char **seperated_line,FILE * file_ext,int ic,int * ic_second, symbol_details ***symbol_list, int sum_symbols,unsigned short **commands);


/*This function receives a symbol name, a number of symbols in a symbol list, and a pointer to a pointer to an array of pointers to symbol_details structures. The function checks if the symbol name already exists in the symbol list and if so, adds the string "ent" to the end of the symbol's type string. If the symbol is not found in the list, an error message is printed and the function returns 1. If the operation succeeds, the function returns 0.*/
int add_apearing_symbol_to_list(char *name,int sum_symbol,symbol_details ***symbol_list);

/*This function takes a label name, the sum of symbols in the symbol list, the symbol list, the IC counter, and the IC_second counter as arguments. It then searches the symbol list for the label and returns its address. If the label is a function, it adds the IC_START_ADDRESS to the address and multiplies it by 4, and if it's data, it adds the IC_START_ADDRESS and the IC to the address and multiplies it by 4. If the label is an external symbol, it writes the label and the IC_second + IC_START_ADDRESS to an external file and returns 1. If the label is not found in the symbol list, it returns -1.*/
short return_label_adress(char * name,FILE * file_ext,int sum_symbols,symbol_details ***symbol_list,int ic,int ic_second);

/*This function receives a list of symbols, their total number, the name of the file and the instruction counter as arguments. Its purpose is to write to an entry file all the symbols that are marked as "entry" in the symbol list. The function creates a file with the extension ".ent" based on the name of the input file. It then iterates through the symbol list and checks if the symbol is marked as "entry". If it is, the function calculates the value of the symbol according to its type (function or data) and adds the instruction counter and IC_START_ADDRESS accordingly. Finally, the function writes the symbol's name and value to the entry file. If the function encounters an error, it returns a non-zero value.*/
int entry_label_file(symbol_details ***symbol_list, int sum_symbols, char * file_name,int ic);

/*This function is responsible for creating the object file, which will hold the final binary code of the program. The function receives as input the file name, the final instruction counter (IC), the final data counter (DC), and the command and data arrays that hold the binary code.
First, the function creates a file with the ".obj" extension and writes the IC and DC values in the first line of the file. Then, the function loops through the command and data arrays and writes their corresponding binary values in the file, along with their respective addresses. The binary values are converted to a unique base-2 string using a helper function called ConvertToBase2Uniqe().
Finally, the function closes the object file and frees the memory allocated for the object file name. If an error occurs during the process, the function returns an error code.*/
int create_obj_file(char *file_name, int *ic, int *dc,unsigned short **commands,unsigned short **data);

/*The function ConvertToBase2Uniqe takes an integer address and returns a unique base-2 representation of that address as a string. The resulting string uses dots (.) and slashes (/) instead of the standard binary digits (0 and 1) to create a unique format that can be easily distinguished from standard binary representations. The function first converts the integer into a binary representation using standard binary digits, and then replaces each 0 with a dot and each 1 with a slash. Finally, the function adds a null-terminating character to the end of the resulting string and returns a pointer to the string. If there is a memory allocation failure during the execution of the function, it will return NULL.*/
char * ConvertToBase2Uniqe(int address);

#endif
