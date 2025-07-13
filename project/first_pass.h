#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "text_correctness.h"
#include "error.h"
#include "globals.h"
#define SIZE_OF_SYMBOL sizeof(symbol_details)
#define SIZE_OF_SYMBOL_POINTER sizeof(symbol_details*)
#define MAX_SHORT_VALU 65535
#define AMOUNT_FOR_DIVIDING_FOR_SECOND_JUMP_TYPE 4
#define SPACE_BEFOR_REGISTER_PARAMETER 8
#define JUMP_ADRESS 8
#define LABEL_ADRESS_DEST_OPERAND 4
#define LABEL_ADRESS_SOURCE_OPERAND 16
#define REGISTER_ADRESS_SOURCE_OPERAND 48
#define REGISTER_ADRESS_DEST_OPERAND 12
#define PLACE_FOR_REGISTER_AS_SOURCE_OPERAND 8
#define LABEL_ADRESS_PARAMETER 4096
#define LABEL_ADRESS_SECOND_PARAMETER 1024
#define REGISTER_ADRESS_PARAMETER 12288
#define TOW_OPERAND_FOR_JUMP_TYPE 2


/*This function performs the first pass of the assembly process. It reads each line from the input file, handles errors in the line, and passes valid lines to the line handling function. It also prints relevant information to the console for debugging purposes. If an error is encountered during the line handling process, it returns an error code. Otherwise, it updates the instruction and data counters, symbol list, and other relevant variables.*/
int first_pass(FILE *f,char * file_name,unsigned short **commands,unsigned short **data,int *ic,int *dc,int *sum_symbols,symbol_details *** symbol_list,int * is_extern,int * is_entery);

/*The function is responsible for processing a single line of code in the input file. It receives the line as a string, splits it into separate words, and determines the type of instruction based on the first word. It then calls different helper functions to handle the different types of instructions (such as command, data, string, entry, or extern). These helper functions are responsible for allocating memory for the relevant data structures (such as the command or data arrays), adding symbols to the symbol table, and updating the instruction counter (ic) and data counter (dc) as needed. The function returns 0 if the line was processed successfully, or 1 if an error occurred during the processing of the line.*/
int line_handling(FILE *f,unsigned short **commands,unsigned short **data,int *ic,int *dc,char curr_line[],int *sum_symbols,symbol_details ***symbol_list,int * is_extern,int * is_entery);

/*This function handles the case where the input line is a string directive type. It takes the input line, splits it into separate words, checks if there is a symbol defined, and if so, adds it to the symbol table. Then, it allocates memory for the data array (if it's the first data line) or reallocates memory (if it's not the first line) and inputs all the letters of the string to the data array. Finally, it adds a white line after the word and frees the memory of the split line. The function returns 0 if everything went smoothly and 1 if there was an error with memory allocation.*/
int command_line_dealing(int i,int num_words,int symbol_flag,int *ic,int *sum_symbol,char *seprate_line[],unsigned short **commands,int command_type,symbol_details ***symbol_list);

/*The function handles a line of code that contains a string directive. It checks if there is a symbol defined at the beginning of the line, allocates memory for the string, and copies the characters between the double quotes into the allocated memory. If it is the first string line, it allocates memory using calloc, otherwise, it uses realloc. Finally, it frees the memory allocated for the split line.*/
int string_line_dealing(int i,int symbol_flag,int *dc,int *sum_symbol,char *seprate_line[],unsigned short **data,symbol_details ***symbol_list);

/*This function handles the encoding of the operands for the current command. It first checks the type of operands for the command, then calls the relevant function to encode them. For type 3 commands, it also handles the encoding of the label's address.*/
int data_line_dealing(int i,int num_words,int symbol_flag,int *dc,int *sum_symbols,char *seprated_line[],unsigned short **data,symbol_details ***symbol_list);

/*The function is responsible for generating the machine code of each instruction in the input assembly file. It uses the type of the instruction operands and calls the corresponding function to generate the code accordingly. If the instruction is a jump type, it also calculates the offset between the current instruction and the target instruction, and adds it to the opcode. The function updates the value of the instruction counter (ic) and the list of symbols as needed.*/
int coding_memory(int num_words,int *ic,int *sum_symbol,char *seprate_line[],unsigned short **commands,int i,int command_type,symbol_details ***symbol_list);

/*This function handles the coding of commands with two operands. It checks the first operand and determines its type - label, register, or number. If it is a label, it allocates memory for two shorts and sets the command's opcode to 16. If it is a register, it allocates memory for two shorts, sets the opcode to 48, and updates the register value in the commands. If it is a number, it allocates memory for two shorts and updates the number value in the commands. It then calls the function coding_memory_for_one_operand to handle the second operand.*/
int coding_memory_for_two_operands(int *ic,char *seprate_line[],unsigned short **commands,int i,symbol_details ***symbol_list,int *sum_symbol);

/*The function is responsible for encoding commands with a single operand, which can be a label, a register, or a numerical value. If the operand is a label, the function sets the appropriate bits in the command and stores the address of the label in the next word. If the operand is a register, the function sets the appropriate bits and stores the register number in the command. If the operand is a numerical value, the function stores the value in the command. In all cases, the function updates the instruction counter ic and returns 0 if successful, or 1 if there was an error.*/
int coding_memory_for_one_operand(int *ic,char *seprate_line[],unsigned short **commands,int i,int num_operands,symbol_details ***symbol_list,int *sum_symbol);

/*The function handles the encoding of commands with two operands or a single operand that requires jumping to a function. It determines the type of the operand(s) and encodes the relevant bits into the command array. The function also handles cases where the operand is a label, a register, or a number. It returns the number of encoded operands.*/
int coding_memory_for_jmping_funcs(int *ic,char *seprate_line[],unsigned short **commands,int i,int num_operands,int j,symbol_details ***symbol_list,int *sum_symbol);

/*This function allocates memory for the next symbol in the symbol list. It first reallocates memory for the symbol list to increase its size by one. It then allocates memory for the new symbol element and initializes it with empty values. The function returns 0 if the memory allocation was successful and 1 otherwise.*/
int allocate_memory_for_next_symbol(symbol_details ***symbol_list,int *sum_symbol);

/*This function adds a new defined symbol to a list of symbols. It receives the name, value, type, and the symbol list, and then allocates memory for the new symbol. If it is the first symbol, it allocates memory for the list as well. The function checks if the symbol is already defined in the list, and if so, it prints an error message and returns 2. If the symbol is not defined, it adds it to the list and allocates memory for the next symbol. Finally, the function returns 0 to indicate successful execution.*/
int add_defined_symbol_to_list(char *name, int valu,int *sum_symbol, char type[],symbol_details ***symbol_list);


#endif
