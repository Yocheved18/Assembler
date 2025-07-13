#include "second_pass.h"

int second_pass(FILE * f,char * file_name,int *ic,int *dc,int *sum_symbols,unsigned short **commands,unsigned short **data, symbol_details ***symbol_list,int  is_extern,int  is_entery)
{
	char line[MAX_LINE];/*Buffer to store each line from the file*/
	int ic_second=0;/*Variable to store the second IC value*/
	int i=0;/*initialize variable i to zero*/
	char c;/*Character variable to store each character from the file*/
	char *extern_file_name = NULL;/*String to store the name of the external file*/
	FILE * file_ext;/*File pointer for the external file*/
	if(is_extern)/*Check if the file contains external symbols*/
	{
		extern_file_name = (char *)calloc(FILENAME_MAX - FILENAME_EXT_LEN , sizeof(char));/*Allocate memory for the external file name*/
		if(extern_file_name ==NULL)/*Check for memory allocation failure*/
		{
			fprintf(stderr, "Error: Memory allocation failed!!\n");/*print an error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}
		strcpy(extern_file_name, file_name);/*Copy the original file name to the external file name*/
		while(i<FILENAME_EXT_LEN)/*Remove the file extension*/
		{
			extern_file_name[strlen(extern_file_name)-i]='\0';	
			i++;/*increment the loop counter*/
		}
		i=0;/*initialize variable i to zero*/
		strcat(extern_file_name, ".ext");/*Add the .ext extension to the file name*/
		file_ext = fopen(extern_file_name, "w");/*Open the external file for writing*/
		if (!file_ext) /*Check if the file opening failed*/
		{
			free(extern_file_name);
			fprintf(stderr, "Error: could not open entry file %s for writing.\n", extern_file_name);/*prints error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}
	}
	while((c=getc(f))!=EOF)/*Read the file character by character*/
	{
		if(i<MAX_LINE_LENGTH && c!= '\n')/*Check if the line is within the maximum line length and not a newline character*/
		{
			line[i]=c;/*Add the character to the line buffer*/
			i++;/*increment the loop counter*/
		}
		else
		{
			line[i]='\0';/*Terminate the line with a null character*/
			i=0;/*Reset the loop counter*/
			if(strlen(line)>1)/*Check if the line has more than one character*/
			{
				if(second_pass_handeling(line,file_ext,*ic+1,&ic_second,symbol_list,sum_symbols,commands))/*Process the line using the second_pass_handeling function*/
				{
					fprintf(stderr,"Error!!!\n");/*print error message*/
					return ALOCATION_ERROR;/*return an allocation error code*/
				}	
			}
		}
	}
	if(is_entery)/*Check if the file contains entry symbols*/
	{
		if(entry_label_file(symbol_list, *sum_symbols, file_name,*ic+1))/*Process the entry labels*/
		{
			fprintf(stderr,"Error!!!\n");/*print error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}
	}
	if(create_obj_file(file_name,ic,dc,commands,data))/*Create the .obj file*/
	{
		fprintf(stderr,"Error!!!\n");/*print error message*/
		return ALOCATION_ERROR;/*return an allocation error code*/
	}
	if(is_extern)/* Check if the file contains extern symbols*/
	{
		fclose(file_ext);/**/
		free(extern_file_name);/**/
	}
	return VALID;/*return a success code*/
}

int second_pass_handeling(char *line,FILE * file_ext,int ic,int * ic_second, symbol_details ***symbol_list, int *sum_symbols,unsigned short **commands)
{
	int i = 0;/*initialize variable i to zero*/
	int num = 0;/*initialize variable num to zero*/
	char line_cpy[MAX_LINE];/*initialize a character array for a copy of the line*/
	int command_type;/*initialize variable command_type*/
	char *seperated_line[MAX_LINE_LENGTH];/*initialize an array of strings for the separated line*/
	if(split_line(seperated_line, line))/*split the line into separate strings*/
	{
		fprintf(stderr,"Error!!!\n");/*print an error message if splitting the line failed*/
		return ALOCATION_ERROR;/*return an allocation error code*/
	}
	num = count_num_of_words_in_line(line);/*count the number of words in the line*/
	strcpy(line_cpy,*seperated_line);/*copy the line to the line copy variable*/
	line_cpy[strlen(line_cpy)-ONE_SPOT]='\0';/*remove the newline character at the end of the line*/
	if((is_label(line_cpy)) && seperated_line[0][strlen(*seperated_line)-ONE_SPOT]==':')/*checks if there is a label declaration*/
	{
		i++;/*increment the loop counter*/
	}
	if(instructer_type(seperated_line[i])==DATA_CASE || instructer_type(seperated_line[i])==STRING_CASE)/*check if the instruction is a guidance instruction*/		
	{
		i=num;/*set the loop counter to the number of words in the line*/
		free_split_line(seperated_line);/*free the memory allocated for the separated line*/
		return VALID;/*return a success code*/
	}
	else if((command_type=is_command(seperated_line[i]))>-1)/*check if the instruction is a command instruction*/
	{
		i++;/*increment the loop counter*/
		if(func_table[command_type].type_operands==JUMP_TYPE)/*handle the instruction based on the number of operands it takes*/
		{
			if(command_jump_operand(seperated_line+i,num,file_ext,ic,ic_second,symbol_list,*sum_symbols,commands))/*handle the command with three operands*/
			{
				free_split_line(seperated_line);/*free the memory allocated for the separated line*/
				return ALOCATION_ERROR;/*return an allocation error code*/
			}
			free_split_line(seperated_line);/*free the memory allocated for the separated line*/
			return VALID;/*return a success code*/
		}
		else if(func_table[command_type].type_operands==TOW_OPERAND_TYPE)/*handle the command with two operands*/
		{
			if(command_double_operand(seperated_line+i,file_ext,ic,ic_second,symbol_list,*sum_symbols,commands))
			{
				free_split_line(seperated_line);/*free the memory allocated for the separated line*/
				return ALOCATION_ERROR;/*return an allocation error code*/
			}
			free_split_line(seperated_line);/*free the memory allocated for the separated line*/
			return VALID;/*return a success code*/
		}
		else if(func_table[command_type].type_operands==ONE_OPERAND_TYPE)/*If the command has one operand
*/
		{
			if(command_single_operand(seperated_line+i,file_ext,ic,ic_second,symbol_list,*sum_symbols,commands))
			{
				free_split_line(seperated_line);/*free the memory allocated for the separated line*/
				return ALOCATION_ERROR;/*return an allocation error code*/
			}
			free_split_line(seperated_line);/*free the memory allocated for the separated line*/
			return VALID;/*return a success code*/
		}
		free_split_line(seperated_line);/*free the memory allocated for the separated line*/
		return VALID;/*return a success code*/
	}
	else if(instructer_type(seperated_line[i])==ENTERY_CASE)/*check if the line is an ENTRY directive*/
	{
		if(add_apearing_symbol_to_list(seperated_line[i+1],*sum_symbols,symbol_list))
		{
			free_split_line(seperated_line);/*free the memory allocated for the separated line*/
			fprintf(stderr,"Error!\n");/*print error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}
		free_split_line(seperated_line);/*free the memory allocated for the separated line*/
		return VALID;/*return a success code*/
	}
	free_split_line(seperated_line);/*free the memory allocated for the separated line*/
	return VALID;/*return a success code*/
}

int command_jump_operand(char **seperated_line,int num_of_words,FILE * file_ext,int ic,int * ic_second, symbol_details ***symbol_list, int sum_symbols,unsigned short **commands)
{
	short label_adress=-1;/*initialize variable for the label's address*/
	int i=0;/*initialize variable i to zero*/
	label_adress=return_label_adress(seperated_line[i],file_ext,sum_symbols,symbol_list,ic,*ic_second+ONE_SPOT);/*get the label's address using the function return_label_adress*/
	if(label_adress==-1)/*if the label was not found*/
	{
		fprintf(stderr,"ERROR!\nLabel not found in list!!\n");/*print an error message*/
		return TEXT_ERROR;/*return an error code*/
	}
	*((*commands)+*ic_second+ONE_SPOT)=label_adress;/*add the label's address to the command array at the correct index*/
	if(num_of_words>=NUM_OF_WORDS_FOR_JUMP_TYPE)/*if the number of words in the line is greater than or equal to the file name extension length*/
	{
		if(is_register(seperated_line[i+ONE_SPOT]) && is_register(seperated_line[i+TOW_SPOTS]))/* if the operands are two registers*/
		{
			*ic_second+=THREE_SPOTS;/*increment the second instruction counter by three*/
		}
		else/*if the operands are not two registers*/
		{
			if(is_label(seperated_line[i+ONE_SPOT]))/*if the first operand is a label*/
			{
				label_adress=return_label_adress(seperated_line[i+ONE_SPOT],file_ext,sum_symbols,symbol_list,ic,*ic_second+TOW_SPOTS);/*get the label's address*/
				if(label_adress==-1)/*if the label was not found*/
				{
					fprintf(stderr,"ERROR!\nLabel not found in list!!\n");/*print an error message*/
					return TEXT_ERROR;/*return an error code*/
				}
				*((*commands)+*ic_second+TOW_SPOTS)=label_adress;/*add the label's address to the command array at the correct index*/
			}
			if(is_label(seperated_line[i+TOW_SPOTS]))/*if the second operand is a label*/
			{
				label_adress=return_label_adress(seperated_line[i+TOW_SPOTS],file_ext,sum_symbols,symbol_list,ic,*ic_second+THREE_SPOTS);/*get the label's address*/
				if(label_adress==-1)/*if the label was not found*/
				{
					fprintf(stderr,"ERROR!\nLabel not found in list!!\n");/*print an error message*/
					return TEXT_ERROR;/*return an error code
                }*/
				}
				*((*commands)+*ic_second+THREE_SPOTS)=label_adress;/*add the label's address to the command array at the correct index
            }*/
			}
			*ic_second+=FOUR_SPOTS;/*increment the second instruction counter by the length of the file name extension*/
		}	
		return VALID;/*return a success code*/
	}
	else/*if the number of words in the line is less than the file name extension length*/
	{
		*ic_second+=TOW_SPOTS;/*increment the second instruction counter by the function label shift*/
		return VALID;/*return a success code*/
	}
	return VALID;/*return a success code*/
}

int command_double_operand(char **seperated_line,FILE * file_ext,int ic,int * ic_second, symbol_details ***symbol_list, int sum_symbols,unsigned short **commands)
{
	short label_adress=-1;/**/
	int i=0;/*initialize variable i to zero*/
	if(is_register(seperated_line[i]) && is_register(seperated_line[i+ONE_SPOT]))/*check if both operands are registers
	{*/
	{
		*ic_second+=TOW_SPOTS;/*increment the second instruction counter*/
		return VALID;/*return a success code*/
	}
	else/*if at least one operand is not a register*/
	{
		if(is_label(seperated_line[i]))/*check if the first operand is a label*/
		{
			label_adress=return_label_adress(seperated_line[i],file_ext,sum_symbols,symbol_list,ic,*ic_second+ONE_SPOT);/*get the label's address using the function return_label_adress
			if(label_adress==-1) // if the label was not found*/
			if(label_adress==-1)/*if the label was not found*/
			{
				fprintf(stderr,"ERROR!\nLabel not found in list!!\n");/*print an error message*/
				return TEXT_ERROR;/*return an error code*/
			}
			*((*commands)+*ic_second+ONE_SPOT)=label_adress;/*add the label's address to the command array at the correct index*/
		}
		if(is_label(seperated_line[i+ONE_SPOT]))/*check if the second operand is a label*/
		{
			label_adress=return_label_adress(seperated_line[i+ONE_SPOT],file_ext,sum_symbols,symbol_list,ic,*ic_second+TOW_SPOTS);/*get the label's address using the function return_label_adress*/
			if(label_adress==-1)/*if the label was not found*/
			{
				fprintf(stderr,"ERROR!\nLabel not found in list!!\n");/*print an error message*/
				return TEXT_ERROR;/*return an error code*/
			}
			*((*commands)+*ic_second+TOW_SPOTS)=label_adress;/*add the label's address to the command array at the correct index
		}*/
		}
		*ic_second+=THREE_SPOTS;/*increment the second instruction counter*/
		return VALID;/*return a success code*/
	}
	return VALID;/*return a success code*/
}

int command_single_operand(char **seperated_line,FILE * file_ext,int ic,int * ic_second, symbol_details ***symbol_list, int sum_symbols,unsigned short **commands)
{
	short label_adress=-1;/*default value of address if label not found */
	int i=0;/*initialize variable i to zero*/
	if(is_label(seperated_line[i]))/*check if the operand is a label*/
	{
		label_adress=return_label_adress(seperated_line[i],file_ext,sum_symbols,symbol_list,ic,*ic_second+ONE_SPOT);/*get the label's address*/
		if(label_adress==-1)/*if the label was not found*/
		{
			fprintf(stderr,"ERROR!\nLabel not found in list!!\n");/*print an error message*/
			return TEXT_ERROR;/*return an error code*/
		}
		*((*commands)+*ic_second+ONE_SPOT)=label_adress;/*add the label's address to the command*/
	}
	*ic_second+=TOW_SPOTS;/*increment the second instruction counter*/
	return VALID;/*return a success code*/
}

int add_apearing_symbol_to_list(char *name,int sum_symbol,symbol_details ***symbol_list)
{
	int i=0;/*initialize variable i to zero*/
	if(sum_symbol==0)/*check if the symbol list is empty*/
	{
		fprintf(stderr,"ERROR!\nNo labels in list\n");/*print an error message*/
		return ALOCATION_ERROR;/*return an error code*/
	}
	else
	{
		while(i<sum_symbol)/*loop through the symbol list to find the symbol with the given name*/
		{
			if(strcmp((*(*symbol_list+i))->symbol_name,name)==0)/*check if the current symbol's name matches the given name*/
			{
				strcat((*(*symbol_list+i))->symbol_type,"ent");/*add the "ent" type to the symbol's type*/
				return VALID;/*return a success code*/
			}
			i++;/* increment the loop counter*/
		}
	}
	fprintf(stderr,"ERROR!\nLabels not found in list\n");/* print an error message*/
	return ALOCATION_ERROR;/*return an error code*/
}

short return_label_adress(char * name,FILE * extern_file_name,int sum_symbols,symbol_details ***symbol_list,int ic,int ic_second)
{
	int i=0;/*initialize variable i to zero*/
	short val=-1;/* default value if label not found */
	/*loop through symbol list to find label*/
	while(i<sum_symbols)
	{
		/*check if symbol name matches requested label name*/
		if(strcmp((*(*symbol_list+i))->symbol_name,name)==0)/**/
		{
			val=(*(*symbol_list+i))->symbol_val;/*get symbol value*/
			/* check symbol type to determine address encoding */
			if(strstr((*(*symbol_list+i))->symbol_type,"func")!=NULL)/**/
			{
				/*function label - shift and encode address as 10*/
				val+=IC_START_ADDRESS;/*add instruction counter offset*/
				val=val<<SPACE_FOR_TOW_FIRST_BITS;
				val+=SPACE_FOR_TOW_FIRST_BITS;/*10 encoding*/
				return val;
			}
			else if(strstr((*(*symbol_list+i))->symbol_type,"data")!=NULL)/**/
			{
				/* data label - shift and encode address as 01 */
				val+=(IC_START_ADDRESS+ic);/* add instruction counter and data counter offsets */
				val=val<<SPACE_FOR_TOW_FIRST_BITS;
				val+=SPACE_FOR_TOW_FIRST_BITS;/*01 encoding*/
				return val;
			}
			else if(strstr((*(*symbol_list+i))->symbol_type,"ext")!=NULL)/**/
			{
				val=1;/* external label - write to extern file and return 0 */
				fprintf(extern_file_name, "%s \t%d\n",name,ic_second +  IC_START_ADDRESS);/* write label name and address to file */
				return val;/*returns value*/
			}
			else
			{
				val=-1;/*unsupported label type - return default value*/
				return val;
			}
		}
		i++; /*increment the loop counter*/
	}
	return val;/*returns value*/
}

int entry_label_file(symbol_details ***symbol_list, int sum_symbols, char * file_name,int ic)
{
	int i=0;/*initialize variable i to zero*/
	char *entry_file_name = NULL;/*declare a pointer to a char and initialize it to NULL*/
	int val=0;/* initialize variable val to zero*/
	FILE* file_ent;/*declare a pointer to a FILE struct*/
	entry_file_name = (char *)calloc(FILENAME_MAX - FILENAME_EXT_LEN, sizeof(char));/* allocate memory for entry_file_name*/
	if(entry_file_name ==NULL)/*check if memory allocation was successful*/
	{
		fprintf(stderr, "Error: Memory allocation failed!!\n");/*prints an error message to standard error output*/
		return ALOCATION_ERROR;/*return 1 to indicate failure*/
	}
	strcpy(entry_file_name, file_name);/*copy file_name into entry_file_name*/
	while(i<NUM_OF_WORDS_FOR_JUMP_TYPE)/*loop while i is less than the constant FILENAME_EXT_LEN*/
	{
		entry_file_name[strlen(entry_file_name)-i]='\0';/*set the last i characters of entry_file_name to null character*/	
		i++;/*increment i by one*/
	}
	strcat(entry_file_name, ".ent");/*concatenate ".ent" to the end of entry_file_name*/
	file_ent = fopen(entry_file_name, "w");/*open entry_file_name for writing*/
	if (!file_ent)/*checks if opening the file was successful*/
	{
		free(entry_file_name);/*free the memory allocated for entry_file_name*/
		fprintf(stderr, "Error: could not open entry file %s for writing.\n", entry_file_name);/*print an error message to standard error output*/
		return ALOCATION_ERROR;/*return 1 to indicate failure*/
	}
	/* Write entries to file */
	for (i = 0; i < sum_symbols; i++)/*loop through each symbol in symbol_list*/
	{
		if((strstr((*(*symbol_list+i))->symbol_type,"ent"))!=NULL)/*check if the symbol is an entry symbol*/
		{
			val=(*(*symbol_list+i))->symbol_val;/*get the symbol's value*/
			if((strstr((*(*symbol_list+i))->symbol_type,"func")))/*check if the symbol is a function symbol*/
			{
				val+=IC_START_ADDRESS;/*add the instruction counter (IC) start address to the symbol's value*/
			}
			if((strstr((*(*symbol_list+i))->symbol_type,"data")))/*check if the symbol is a data symbol*/
			{
				val+=(IC_START_ADDRESS+ic);/*add the IC start address and the number of data items to the symbol's value*/
			}
			else if((strstr((*(*symbol_list+i))->symbol_type,"ext"))!=NULL)/*check if the symbol is an external symbol*/
			{
				free(entry_file_name);/*free the memory allocated for entry_file_name*/
				fclose(file_ent);/*close the file pointer*/
				return TEXT_ERROR;/*return TEXT_ERROR to indicate that an external symbol was found*/
			}
			fprintf(file_ent, "%s \t%d\n",(*(*symbol_list+i))->symbol_name,val);/*write the symbol's name and value to the file*/
		}
	}
	fclose(file_ent);/*close the file pointer*/
	free(entry_file_name);/*free the memory allocated for entry_file_name*/
	return VALID;/*return a success code*/
}

int create_obj_file(char *file_name, int *ic, int *dc,unsigned short **commands,unsigned short **data)
{
	int i=0;/*initialize variable i to zero*/
	char *object_file_name = NULL;
	char * convert;
	FILE* file_obj;
	/*allocates memory for the object file name.*/
	object_file_name = (char *)calloc(FILENAME_MAX - FILENAME_EXT_LEN, sizeof(char));
	if(object_file_name ==NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed!!\n");
		return ALOCATION_ERROR;
	}
	/*copies the input file name to the object file name, and remove the file extension.*/
	strcpy(object_file_name, file_name);
	while(i<NUM_OF_WORDS_FOR_JUMP_TYPE)
	{
		object_file_name[strlen(object_file_name)-i]='\0';	
		i++;/*increment the loop counter*/
	}
	/*appends the object file extension.*/
	strcat(object_file_name, ".ob");
	/*tries to open the object file for writing.*/
	if(!(file_obj = fopen(object_file_name,"w+"))) 
	{
		free(object_file_name);
		fprintf(stderr, "Error: could not open object file %s for writing.\n", object_file_name);
		return ALOCATION_ERROR;
	}
	/*writes the machine code of the program to the object file, in the unique base-2 format.*/
	fprintf(file_obj, "%d \t%d\n", *ic+1, *dc+1);
	if(*ic!=-1)
	{
		for (i = 0; i <= *ic; i++) 
		{
			/*converts the command word to the unique base-2 format.*/
			if((convert=ConvertToBase2Uniqe(*((*commands)+i)))==NULL)
			{
				free(object_file_name);
				printf("Error: Memory allocation failed.\n");
				return ALOCATION_ERROR;
			}
			/*writes the command word to the object file, along with its address.*/
			fprintf(file_obj, "%d \t%s\n", IC_START_ADDRESS + i,convert);
			free(convert);
		}
	}
	if(*dc!=-1)
	{
		for (i = 0; i <= *dc; i++) 
		{
			/*converts the data word to the unique base-2 format.*/
			if((convert=ConvertToBase2Uniqe(*((*data)+i)))==NULL)
			{
				free(object_file_name);
				printf("Error: Memory allocation failed.\n");
				return ALOCATION_ERROR;
			}
			/*writes the data word to the object file, along with its address.*/
			fprintf(file_obj, "%d \t%s\n", IC_START_ADDRESS + i + *ic+1, convert);
			free(convert);
		}
	}
	/*closes the object file and free the allocated memory.*/
	fclose(file_obj);
	free(object_file_name);
	return VALID;/*return a success code*/
}

char * ConvertToBase2Uniqe(int address)
{
	int num = address;/*initializes a variable with the address value.*/
	int i=BITS_IN_ADDRESS - 1;/*initialize the index for the base-2 address string.*/
	char * base2Address;/*declare a string to store the base-2 address.*/
	base2Address = malloc(sizeof(char) * SIZE_OF_SHORT);/*allocate memory for the string.*/
	if (base2Address == NULL) /*check if the allocation was successful*/
	{
		fprintf(stderr,"Error: Memory allocation failed.\n");
		return NULL;/*return NULL if the allocation failed.*/
	}
	/*This block of code converts the binary representation of the input address into the string with the unique base-2 format (using dots and slashes) and stores the resulting string in 'base2Address'..*/
	for (; i >-1; i--) /*iterate through all the bits of the address.*/
	{
		if((num % 2) == 1)/*if the current bit is 1.*/
		{
			base2Address[i] = '1';/*set the current bit in the base-2 address string to '1'.*/
		} 
		else/*if the current bit is 0.*/
		{
			base2Address[i] = '0';/*sets the current bit in the base-2 address string to '0'.*/
		} 
		address =  address / SPACE_FOR_TOW_FIRST_BITS;/*divides the address by 2 to shift to the next bit.*/
		num = address;/*update the variable holding the current address value.*/
	}
	for (i = BITS_IN_ADDRESS - 1; i >-1; i--) /*iterate through all the bits of the base-2 address string.*/
	{
		if(base2Address[i] == '0')/*if the current bit is '0'.*/
		{
			base2Address[i]='.';/*replace the current bit with '.'.*/
		}
		else/*if the current bit is '1'.*/
		{
			base2Address[i]='/';/*replace the current bit with '/'.*/
		}
	}
	base2Address[BITS_IN_ADDRESS]='\0';/*adds the null-terminating character to the end of the string.*/
	return base2Address;/*returns a pointer to the base-2 address string.*/
}


