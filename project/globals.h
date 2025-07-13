#ifndef GLOBALS_H
#define GLOBALS_H

#define as_end_file_name ".as"/*sets the input file*/
#define am_end_file_name ".am"/*sets the output file*/
#define MAX_LINE_LENGTH 80/*sets the maximum length for a line in the file*/
#define MAX_LINE 81
#define MAX_LABEL 31
#define MAX_LABEL_TYPE 10
#define MAX_FUNC_NAME 5
#define TEXT_ERROR 2
#define ALOCATION_ERROR 1
#define VALID 0
#define ON 1
#define JUMP_TYPE 3
#define TOW_OPERAND_TYPE 2
#define ONE_OPERAND_TYPE 1
#define NO_OPERAND_TYPE 0
#define FOUR_SPOTS 4
#define THREE_SPOTS 3
#define TOW_SPOTS 2
#define ONE_SPOT 1
#define NUM_OF_WORDS_FOR_JUMP_TYPE 4
#define LABEL_ADRESS 3
#define DATA_CASE 0
#define STRING_CASE 1
#define EXTERN_CASE 2
#define ENTERY_CASE 3
#define SPACE_FOR_TOW_FIRST_BITS 2
#define DIGIT 0
#define LABEL 1
#define JUMP 2
#define REGISTER 3

#define ONE 1
#define TWO 2
#define ZERO 0
#define MOV_OP 0
#define CMP_OP 64
#define ADD_OP 128
#define SUB_OP 192
#define NOT_OP 256
#define CLR_OP 320
#define LEA_OP 384
#define INC_OP 448
#define DEC_OP 512
#define JMP_OP 576
#define BNE_OP 640
#define RED_OP 704
#define PRN_OP 768
#define JSR_OP 832
#define RTS_OP 896
#define STOP_OP 960
#define NOT_VALID_OP -1

typedef enum {false,true}bool;

/*Definition of a struct to store details about symbols in the assembly program*/
typedef struct {
	char symbol_name[MAX_LABEL];
	int symbol_val;
	char symbol_type[MAX_LABEL_TYPE];
	}symbol_details;

/*Definition of a struct to store details about each assembly instruction, including its opcode and operand types*/
typedef struct {
	char func_name[MAX_FUNC_NAME];
	int opcode;
	char *source_operands;
	char *dest_operands;
	int type_operands;
	}func_elements;

/*Definition of a static array of struct elements to store information about each instruction in the assembly language, including its name, opcode, and operand types.*/
static const func_elements func_table[] ={{"mov",MOV_OP,"13","013",TOW_OPERAND_TYPE},
				    {"cmp",CMP_OP,"013","013",TOW_OPERAND_TYPE},
				    {"add",ADD_OP,"013","13",TOW_OPERAND_TYPE},
				    {"sub",SUB_OP,"013","13",TOW_OPERAND_TYPE},
				    {"lea",LEA_OP,"1","13",TOW_OPERAND_TYPE},
				    {"clr",CLR_OP,NULL,"13",ONE_OPERAND_TYPE},
				    {"not",NOT_OP,NULL,"13",ONE_OPERAND_TYPE},
				    {"inc",INC_OP,NULL,"13",ONE_OPERAND_TYPE},
				    {"dec",DEC_OP,NULL,"13",ONE_OPERAND_TYPE},
				    {"jmp",JMP_OP,NULL,"12",JUMP_TYPE},
				    {"bne",BNE_OP,NULL,"12",JUMP_TYPE},
				    {"jsr",JSR_OP,NULL,"12",JUMP_TYPE},
				    {"red",RED_OP,NULL,"13",ONE_OPERAND_TYPE},
				    {"prn",PRN_OP,NULL,"013",ONE_OPERAND_TYPE},
				    {"rts",RTS_OP,NULL,NULL,NO_OPERAND_TYPE},
				    {"stop",STOP_OP,NULL,NULL,NO_OPERAND_TYPE},
				    {"0", NOT_VALID_OP,"-1","-1",-1}};


#endif
