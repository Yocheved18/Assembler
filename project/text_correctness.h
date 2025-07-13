#ifndef TEXT_CORRECTNESS_H
#define TEXT_CORRECTNESS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#define MAX_SHORT_VALU 65535
#define MIN_DATA_VALUE 16384
#define MAX_DATA_VALUE 16383
#define MIN_COMMAND_VALUE 4096
#define MAX_COMMAND_VALUE 4095
#define SUM_OF_FUN_TYPES 16
#define ASCI_CODE_FOR_ZERO 48
#define ASCI_CODE_FOR_SEVEN 55
#define SUM_OF_INSTRUCTERS 4

/*The function split_line receives a string of characters and splits it into substrings delimited by white spaces, commas, and parentheses. The substrings are stored in an array of strings. The function uses the strtok function to tokenize the input string and then allocates memory for each token in the output array. Finally, it returns 0 if the operation was successful or 1 if there was a memory allocation error. This function is commonly used in compilers and assemblers to parse and analyze source code.*/
int split_line(char *split_line[],char line[]);

/*The function free_split_line is responsible for freeing the memory allocated for the split_line array, which was previously created by the split_line function. The function iterates over the array and frees each element using the free function. It stops when it reaches an empty element (i.e., a null string). This function ensures that there is no memory leak in the program by freeing the memory that is no longer needed.*/
void free_split_line(char **split_line);

/*This function receives a line of code and an index and returns the index of the first non-space character in the line, starting from the given index. It does so by iterating through the line, starting from the given index, and checking each character until it finds the first non-space character. If the end of the line is reached before finding a non-space character, the function returns the index of the end of the line. The function uses the standard library function isspace() to check if a character is a whitespace character.*/
int jumpSpace(char *, int index);

/*This function receives a string line and an index index as arguments. The function iterates through the characters of the string starting from the character at the given index until it encounters a non-alphabetic character or a colon character. It returns the index of the first non-alphabetic character encountered, or the index of the end of the string if no such character is found. Essentially, this function "jumps" over any word that is found in the given string.*/
int jumpWord(char * line, int index);

/*This function takes a string (a line of text) as input and counts the number of words in it. It does this by first using strtok to split the string into separate words using space, tab, comma, and parentheses as delimiters. It then counts the number of resulting tokens and returns that count.*/
int count_num_of_words_in_line(char *s);

/*This function checks if a given string is an empty line, which means that it only contains white spaces (spaces, tabs, newlines) or is a comment line starting with ';' or '#'. It returns 1 if the line is empty, and 0 otherwise.*/
int is_empty_line(char *s);

/*This function receives a string s and checks if it starts with a semicolon character ;, which indicates that it is a comment line. If the line is indeed a comment line, the function returns 1. Otherwise, it returns 0.*/
int is_comment_line(char *s);

/*The function checks if a given string is a valid label. It first checks if the length of the string is greater than 30 characters, if the first character is not an alphabetical letter, or if the string is a register, command, directive, or instruction name. If any of these conditions are met, it returns 0, indicating that the string is not a valid label. If none of these conditions are met, it then checks that all of the characters in the string are either alphabetical letters or digits. If this is the case, it returns 1, indicating that the string is a valid label.*/
int is_label(char *s);

/*This function checks if a given string (word) is a register. A register is represented by a lowercase 'r' followed by a digit between 0 and 7. The function first checks if the first character of the word is 'r' and then checks if the second character is a digit between 0 and 7. If both conditions are met, the function returns 1 (true), indicating that the word is indeed a register.*/
int is_register(char *word);

/*The function receives a string word and checks if it is a valid assembly command. It does so by iterating through an array of structures called func_table that holds information about each command, such as its name and opcode. If the string matches the name of one of the commands in the table, the function returns the index of the command in the table. If the string does not match any command, the function returns -1.*/
int is_command(char *word);

/*This function checks if a given word is an assembler directive (instruction). It receives a string as an input, representing a word, and compares it to the four possible directives: ".data", ".string", ".extern", and ".entry". If the given word matches one of the directives, the function returns 1; otherwise, it returns 0.*/
int is_instructer(char *word);

/*The function instructer_type takes a string as input and checks if it is an instruction directive (.data, .string, .extern, or .entry). If it is, the function returns the index of that directive in an array of directive strings. If the input string is not a directive, the function returns -1. The function is useful in parsing and identifying the type of instruction in an assembly language program.*/
int instructer_type(char *word);

/*The is_instructer_name function takes a string as input and checks whether it represents a valid assembler directive. The function checks if the string matches one of the four valid directives: "data", "string", "extern", or "entry". If the input string matches one of these directives, the function returns 1. Otherwise, it returns 0.*/
int is_instructer_name(char *word);

/*The function takes a string s as input and converts it to a signed 15-bit integer. If the string starts with a positive sign (+), it converts the rest of the string to an integer using the atoi function. If the string starts with a negative sign (-), it converts the rest of the string to an integer using atoi, subtracts it from the maximum value of a signed 15-bit integer (MAX_SHORT_VALU), and subtracts 1 from the result. If the string does not start with a sign, it converts the entire string to an integer using atoi. The function returns the converted integer.*/
int switch_digit(char *s);

/*This function reallocates memory for the commands array, which is an array of unsigned short integers. The size of the array is passed as a parameter. The function uses the realloc function to change the size of the memory allocated to the commands array. If the reallocation is successful, the function returns 0. Otherwise, it returns 1 and prints an error message.*/
int reallocate_memory_coding(unsigned short **commands,int size);

/*This is a function declaration for a function called split_line_correctness. It takes two parameters, an array of strings called split_line and a string called line. The purpose of this function is to split the string line into words and store each word in the split_line array. The function returns an integer value, which is used to indicate whether the operation was successful or not.*/
int split_line_correctness(char *split_line[],char line[]);

/*This function checks if a given string is a valid digit representation, which means it starts with the '#' character followed by an integer value. The function first checks if the first character is the '#' symbol, then it checks if the string contains valid digits and ignores the optional '+' or '-' sign at the beginning. The function also checks if the integer value is within the range of valid values for our assembly program. If the given string is a valid digit representation, the function returns 1, otherwise, it returns 0.*/
int is_digit(char * word);

/*The function checks if a given string represents a valid number for .data directive. It first checks if the second character of the string is a minus sign or a plus sign, and if so, it increments i to start checking from the first non-positive or non-negative character. Then, it iterates over all the characters in the string from the checked character to the last character in the string. In each iteration, it checks if the current character in the string is a digit using the isdigit function. If the character is not a digit, the function immediately returns 0, indicating that the string does not contain only digits. If the string contains only digits, the function then checks the value of the number against the maximum and minimum allowed values for .data directive, and returns 1 if the value is within the valid range, or 0 otherwise.*/
int is_digit_data(char * word);


#endif
