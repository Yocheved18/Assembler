#include "first_pass.h"
int first_pass(FILE *f,char * file_name,unsigned short **commands,unsigned short **data,int *ic,int *dc,int *sum_symbols,symbol_details *** symbol_list,int * is_extern,int * is_entery)
{
	int i=0;/*a variable representing the current index in the current line being processed.*/
	int errors=0;/*a variable representing if theres errors or nights*/
	char c;/*a variable representing the current character being read from the input file*/
	char curr_line[MAX_LINE];/*a variable holding the current line being processed.*/
	char line_cpy[MAX_LINE];/*a variable holding a copy of the current line being processed to check for errors.*/
	rewind(f);/*reset the file pointer to the beginning of the file*/
	while((c=getc(f))!=EOF)/*loop through each character in the file*/
	{
		if(i<MAX_LINE_LENGTH && c!= '\n')/*if the line is shorter than 80 characters and the character is not a newline, add the character to the current line*/
		{
			curr_line[i]=c;
			i++;
		}
		else if(c!='\n')/*if the line is longer than 80 characters and the character is not a newline, print an error message and skip to the next line*/
		{
			curr_line[i]='\n';
			fprintf(stderr,"ERROR!\nLine is longer then valid line length!\n");
			while((c=fgetc(f))!='\n');
			i=0;
		}
		else/*if the character is a newline, process the current line*/
		{
			curr_line[i]='\0';
			i=0;
			strcpy(line_cpy,curr_line);/*make a copy of the current line and check if it contains any errors*/
			if(strlen(curr_line)>ONE_SPOT)/*This checks if the current line being read is not empty.*/
			{
				if(line_handle_error(line_cpy,*ic,file_name))/*This checks for errors in the current line using the line_handle_error() function.*/
				{
					/*fprintf(stderr,"ERROR WAS FOUND IN LINE!\n");prints error message*/
					errors=ALOCATION_ERROR;
				}
				if(errors==VALID)/*if there are no errors, handle the line based on its type*/
				{
					if(line_handling(f,commands,data,ic,dc,curr_line,sum_symbols,symbol_list,is_extern,is_entery))/*This processes the current line based on its type using the line_handling() function.*/
					{
						fprintf(stderr,"ERROR IN ALLOCATION WAS FOUND IN LINE!\n");/*prints error message*/
						return ALOCATION_ERROR;
					}
				}
				else
				{
					(*ic)++;
				}
			}
		}
	}
	if(errors)/*if there were errors in the input file, print an error message and stop the first pass*/
	{	
		fprintf(stderr,"Stopping!\n");/*prints error message*/
		return ALOCATION_ERROR;
	}
	return VALID;/*if there were no errors, return 0 to indicate that the first pass was successful*/
}

int line_handling(FILE *f,unsigned short **commands,unsigned short **data,int *ic,int *dc,char curr_line[],int *sum_symbols,symbol_details ***symbol_list,int * is_extern,int * is_entery)
{
	char *seprated_line[MAX_LINE];/*An array of strings holding the line split into separate words.*/
	char line_cpy[MAX_LINE];/*A string holding a copy of the current line being processed.*/
	int symbol_define_flag=0;/* A flag indicating whether a symbol is defined in the current line.*/
	int command_type;/*An integer indicating the type of command in the current line.*/
	int i=0;/* An integer representing the current index in the seprated_line array*/
	int num_words=0;/* An integer representing the number of words in the current line.*/
	strcpy(line_cpy,curr_line);
	num_words=count_num_of_words_in_line(line_cpy);
	split_line(seprated_line,curr_line);
	strcpy(line_cpy,*seprated_line);
	line_cpy[strlen(line_cpy)-ONE_SPOT]='\0';
	if((is_label(line_cpy)) && seprated_line[0][strlen(*seprated_line)-ONE_SPOT]==':')/*This block of code checks if the current line contains a label definition. If so, symbol_define_flag is set to ON and i is incremented.*/
	{
		i++;
		symbol_define_flag=ON;
	}
	command_type=is_command(seprated_line[i]);/*checks if the line is a command type*/
	if(command_type>-1)/*the line is a command type*/
	{
		if(command_line_dealing(i,num_words,symbol_define_flag,ic,sum_symbols,seprated_line,commands,command_type,symbol_list))
		{
			return ALOCATION_ERROR;/*error in allocation*/
		}
	}
	else if(strcmp(seprated_line[i],".extern")==0)/*the line is a extern directer type*/
	{
		if(i!=0)/*there was a symbol in the biginning of the line*/
		{
			fprintf(stderr,"ERROR!!\nBefore an extern statment there should be nothing but spaces!\n");/*prints error message*/
			return TEXT_ERROR;
		}
		*is_extern = ON;	
		if(add_defined_symbol_to_list(seprated_line[i+ONE_SPOT],0,sum_symbols,"ext",symbol_list))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*error in allocation*/
			return ALOCATION_ERROR;
		}
	}
	else if(strcmp(seprated_line[i],".entry")==0)/*the line is a entry directer type*/
	{
		if(i!=0)/*there was a symbol in the biginning of the line*/
		{
			fprintf(stderr,"ERROR!!\nBefore an entry statment there should be nothing but spaces!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		*is_entery=ON;
	}
	else if(strcmp(seprated_line[i],".string")==0)/*the line is a string directer type*/
	{
		if(string_line_dealing(i,symbol_define_flag,dc,sum_symbols,seprated_line,data,symbol_list))
		{
			return ALOCATION_ERROR;/*error in allocation*/
		}
	}
	else/*the line is a data directer type*/
	{
		if(data_line_dealing(i,num_words,symbol_define_flag,dc,sum_symbols,seprated_line,data,symbol_list))
		{
			return ALOCATION_ERROR;/*error in allocation*/
		}
	}
	return VALID;
}

int command_line_dealing(int i,int num_words,int symbol_flag,int *ic,int *sum_symbols,char *seprated_line[],unsigned short **commands,int command_type,symbol_details ***symbol_list)
{
	if(*ic==-1)/*If it is the first command in the program, set the instruction counter to 0 and allocate memory for the commands array. */
	{
		printf("ic = -1\n");
		*ic=0;
		(*commands)=(unsigned short *)malloc(sizeof(short));
		if((*commands)==NULL)
		{
			fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	else/*If it is not the first command in the program, increment the instruction counter and reallocate memory for the commands array.*/
	{
		(*ic)++;
		if(reallocate_memory_coding(commands,(sizeof(short)*(*ic+ONE_SPOT))))
		{
			fprintf(stderr,"ERROR!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	if(symbol_flag)/*If the current line has a symbol, add it to the symbol table with the current instruction counter.*/
	{
		if(add_defined_symbol_to_list(seprated_line[i-ONE_SPOT],*ic,sum_symbols,"func",symbol_list))/*Add the command opcode to the commands array.*/
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	*((*commands)+*ic)=func_table[command_type].opcode;/*Encode the command operands and update the instruction counter.*/
	if(coding_memory(num_words,ic,sum_symbols,seprated_line,commands,i+ONE_SPOT,command_type,symbol_list))
	{
		fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
		return ALOCATION_ERROR;
	}
	free_split_line(seprated_line);/*Free the memory allocated for the separated line.*/
	return VALID;
}

int string_line_dealing(int i,int symbol_flag,int *dc,int *sum_symbols,char *seprated_line[],unsigned short **data,symbol_details ***symbol_list)
{
	int j=1;
	if(*dc == -1)/* Check if this is the first data line.*/
	{
		*dc=0;
	}
	i++;/*ignore .string - move to the next word*/
	if(*dc==0)/*first data line*/
	{
		(*data)=(unsigned short *)calloc(strlen(seprated_line[i])-ONE_SPOT,sizeof(short));/*If this is the first data line, allocate memory for it.*/
		if((*data)==NULL)
		{
			fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	else/*not first data line*/
	{
		(*dc)++;
		if(reallocate_memory_coding(data,sizeof(short)*(strlen(seprated_line[i])-ONE_SPOT+*dc)))/*If this is not the first data line, reallocate memory for it.*/
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	if(symbol_flag)/*If there is a symbol declared before the ".string" directive, add it to the symbol list.*/
	{
		if(add_defined_symbol_to_list(seprated_line[i-TOW_SPOTS],*dc,sum_symbols,"data",symbol_list))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	while(seprated_line[i][j]!='"')/*inputs all the letters*/
	{
		*((*data)+*dc)=seprated_line[i][j];
		j++;
		(*dc)++;
	}
	*((*data)+*dc)=0;/*puts a white line after the word*/
	free_split_line(seprated_line);/*Free the memory allocated for the separated line.*/
	return VALID;
}

int data_line_dealing(int i,int num_words,int symbol_flag,int *dc,int *sum_symbols,char *seprated_line[],unsigned short **data,symbol_details ***symbol_list)
{
	/* Check if this is the first data line */
	if((*dc) == -1)
	{
		(*dc)=0;
	}
	if((*dc)==0)/*first data line*/
	{
		(*data)=(unsigned short *)calloc(num_words-ONE_SPOT-symbol_flag,sizeof(short));/*Allocate memory for the data*/
		if((*data)==NULL)
		{
			fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	else/*not first data line*/
	{
		(*dc)++;
		/* Reallocate memory for the data */
		if(reallocate_memory_coding(data,sizeof(short)*(num_words-ONE_SPOT-symbol_flag+*dc)))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	/* Add symbol to symbol table if symbol_flag is set */
	if(symbol_flag)
	{
		if(add_defined_symbol_to_list(seprated_line[i-ONE_SPOT],*dc,sum_symbols,"data",symbol_list))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	i++;/*ignore .data - move to the next word*/
	while(strcmp(seprated_line[i],"")!=0)/* Read data and convert it to a number */
	{
		*((*data)+*dc)=switch_digit(seprated_line[i]);
		(*dc)++;
		i++;
	}
	(*dc)--;/* Decrease dc by 1 */
	free_split_line(seprated_line);/*Free the memory allocated for the separated line.*/
	return VALID;
}

int coding_memory(int num_words,int *ic,int *sum_symbols,char *seprated_line[],unsigned short **commands,int i,int command_type,symbol_details ***symbol_list)
{
	if(func_table[command_type].type_operands==JUMP_TYPE)/* If the command is a jump type, it has a label operand and an additional number operand */
	{
		int num_operands=1;
		if(num_words>=NUM_OF_WORDS_FOR_JUMP_TYPE)
		{
			(*((*commands)+*ic))+=JUMP_ADRESS;/* Increase the command opcode with the address of the label operand */
			num_operands=coding_memory_for_jmping_funcs(ic,seprated_line,commands,i+ONE_SPOT,num_operands,1,symbol_list,sum_symbols);/* Encode the label operand */
			if(num_operands==-1)
			{
				fprintf(stderr,"ERROR!");/*prints error message*/
				return ALOCATION_ERROR;
			}
			num_operands=coding_memory_for_jmping_funcs(ic,seprated_line,commands,i+TOW_SPOTS,num_operands,AMOUNT_FOR_DIVIDING_FOR_SECOND_JUMP_TYPE,symbol_list,sum_symbols);/* Encode the number operand */
			if(num_operands==-1)
			{
				fprintf(stderr,"ERROR!");/*prints error message*/
				return ALOCATION_ERROR;
			}
			/* Add a white line after the second operand */
			*((*commands)+*ic+ONE_SPOT)=0;
			(*ic)+=num_operands;
			return VALID;
		}
		else
		{
			(*((*commands)+*ic))+=LABEL_ADRESS_DEST_OPERAND;/* If there are not enough words in the line, add only the label operand to the command opcode */
			if(reallocate_memory_coding(commands,sizeof(short)*(*ic+ONE_SPOT)))/* Add a white line after the label operand */
			{
				fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
				return ALOCATION_ERROR;
			}
			*((*commands)+*ic+ONE_SPOT)=0;
			(*ic)+=ONE_SPOT;
		}
		return VALID;
	}
	else if(func_table[command_type].type_operands==TOW_OPERAND_TYPE) /* If the command has two operands */
	{
		if(coding_memory_for_two_operands(ic,seprated_line,commands,i,symbol_list,sum_symbols))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		return VALID;
	}
	else if(func_table[command_type].type_operands==ONE_OPERAND_TYPE) /* If the command has one operand */
	{
		if(coding_memory_for_one_operand(ic,seprated_line,commands,i,0,symbol_list,sum_symbols))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");
			return ALOCATION_ERROR;
		}
		return VALID;
	}
	return VALID;
}

int coding_memory_for_two_operands(int *ic,char *seprated_line[],unsigned short **commands,int i,symbol_details ***symbol_list,int *sum_symbols)
{
	int num_operands=0;
	if(is_label(seprated_line[i]))/*Check if the first operand is a label*/
	{
		if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS)))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		(*((*commands)+*ic))+=LABEL_ADRESS_SOURCE_OPERAND;/*Set the source operand address to label address*/
		num_operands++;
		*((*commands)+*ic+num_operands)=0;/* Initialize destination operand with 0 */
	}
	else if(is_register(seprated_line[i]))/* Check if the first operand is a register */
	{
		if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS)))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		(*((*commands)+*ic))+=REGISTER_ADRESS_SOURCE_OPERAND;/* Set the source operand address to register address */
		num_operands++;
		*((*commands)+*ic+num_operands)=atoi(*(seprated_line+i)+ONE_SPOT);/* Set the register number in the destination operand */
		*((*commands)+*ic+num_operands)=(*((*commands)+*ic+num_operands))<<PLACE_FOR_REGISTER_AS_SOURCE_OPERAND;/* Shift the register number to the correct place in the destination operand */
	}
	else if(is_digit(*(seprated_line+i))) /* Check if the first operand is a digit */
	{
		if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS)))
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		num_operands++;
		*((*commands)+*ic+num_operands)=switch_digit(seprated_line[i]+ONE_SPOT);/* Set the digit in the destination operand */
		*((*commands)+*ic+num_operands)=(*((*commands)+*ic+num_operands))<<SPACE_FOR_TOW_FIRST_BITS;/* Shift the digit to the correct place in the destination operand */
	}
	if(coding_memory_for_one_operand(ic,seprated_line,commands,i+ONE_SPOT,num_operands,symbol_list,sum_symbols)) /* Call coding_memory_for_one_operand to handle the second operand */
	{
		fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
		return ALOCATION_ERROR;
	}
	return VALID;
}


int coding_memory_for_one_operand(int *ic,char *seprated_line[],unsigned short **commands,int i,int num_operands,symbol_details ***symbol_list,int *sum_symbols)
{
	int temp_regstr=0;
	if(is_label(*(seprated_line+i)))/*If the operand is a label*/
	{
		if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS+num_operands))) /* Allocate memory for the command and the operand */
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		/* Add the label address to the command */
		(*((*commands)+*ic))+=LABEL_ADRESS_DEST_OPERAND;
		num_operands++;
		(*ic)+=num_operands;
		return VALID;
	}
	else if(is_register(seprated_line[i])) /* If the operand is a register */
	{
		if(num_operands!=0 && is_register(seprated_line[i-ONE_SPOT]))/* If the previous operand is also a register, add the register value to the first operand's command */

		{
			temp_regstr=atoi(*(seprated_line+i)+ONE_SPOT);
			temp_regstr=temp_regstr<<SPACE_FOR_TOW_FIRST_BITS;
			*((*commands)+*ic+num_operands)=(*((*commands)+*ic+num_operands))|temp_regstr;
		}
		else
		{
			if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS+num_operands)))/* Allocate memory for the command and the operand */
			{
				fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
				return ALOCATION_ERROR;
			}
			num_operands++;
			*((*commands)+*ic+num_operands)=atoi(*(seprated_line+i)+ONE_SPOT); /* Add the register address to the command */
			*((*commands)+*ic+num_operands)=(*((*commands)+*ic+num_operands))<<SPACE_FOR_TOW_FIRST_BITS;
		}
		  /* Add the register address destination operand to the command */
		(*((*commands)+*ic))+=REGISTER_ADRESS_DEST_OPERAND;
		(*ic)+=num_operands;
		return VALID;
	}
	else if(is_digit(*(seprated_line+i))) /* If the operand is a digit */
	{
		if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS+num_operands))) /* Allocate memory for the command and the operand */
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		num_operands++;
		/* Add the digit to the command */
		*((*commands)+*ic+num_operands)=switch_digit(seprated_line[i]+ONE_SPOT);
		*((*commands)+*ic+num_operands)=(*((*commands)+*ic+num_operands))<<SPACE_FOR_TOW_FIRST_BITS;
		(*ic)+=num_operands;
		return VALID;
	}
	return VALID;
}


int coding_memory_for_jmping_funcs(int *ic,char *seprated_line[],unsigned short **commands,int i,int num_operands,int j,symbol_details ***symbol_list,int *sum_symbols)
{
	int temp_regstr=0;
	if(is_label(seprated_line[i]))/* If the operand is a label */
	{
		if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS+num_operands)))/* Allocate memory for the command and the operand */
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		/* Add the label address to the command */
		(*((*commands)+*ic))+=LABEL_ADRESS_PARAMETER/j;
		num_operands++;
		*((*commands)+*ic+num_operands)=0;
	}
	else if(is_register(seprated_line[i])) /* If the operand is a register */
	{
		/* If the previous operand is also a register, add the register value to the first operand's command */
		if(num_operands==TOW_OPERAND_FOR_JUMP_TYPE && is_register(seprated_line[i-ONE_SPOT]))/*checks if first parameter was a register*/
		{
			temp_regstr=atoi(*(seprated_line+i)+ONE_SPOT);
			temp_regstr=temp_regstr<<SPACE_FOR_TOW_FIRST_BITS;
			*((*commands)+*ic+num_operands)=(*((*commands)+*ic+num_operands))|temp_regstr;
		}
		else
		{
			if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS+num_operands)))/* Allocate memory for the command and the operand */
			{
				fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
				return ALOCATION_ERROR;
			}
			num_operands++;
			 /* Add the register address to the command */
			*((*commands)+*ic+num_operands)=atoi(*(seprated_line+i)+ONE_SPOT);
			*((*commands)+*ic+num_operands)=(*((*commands)+*ic+num_operands))<<(SPACE_BEFOR_REGISTER_PARAMETER/j);
		}
		 /* Add the register address parameter to the command */
		(*((*commands)+*ic))+=REGISTER_ADRESS_PARAMETER/j;
		
	}
	else if(is_digit(*(seprated_line+i))) /* If the operand is a digit */
	{
		if(reallocate_memory_coding(commands,sizeof(short)*(*ic+TOW_SPOTS+num_operands))) /* Allocate memory for the command and the operand */
		{
			fprintf(stderr,"ERROR!\nFOULT WITH MEMORY!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		num_operands++;
		/* Add the digit to the command */
		*((*commands)+*ic+num_operands)=switch_digit(seprated_line[i]+ONE_SPOT);
		*((*commands)+*ic+num_operands)=(*((*commands)+*ic+num_operands))<<SPACE_FOR_TOW_FIRST_BITS;
	}
	return num_operands;
}

int allocate_memory_for_next_symbol(symbol_details ***symbol_list,int *sum_symbols)
{
	(*symbol_list)=(symbol_details **)realloc(*symbol_list,SIZE_OF_SYMBOL_POINTER*(*sum_symbols+ONE_SPOT));/* Allocate memory for a new symbol pointer */
	if(!(*symbol_list))
	{
		fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");/*prints error message*/
		return ALOCATION_ERROR;
	}
	(*(*symbol_list+(*sum_symbols)))=(symbol_details *)malloc(SIZE_OF_SYMBOL);/* allocate the empty element(symbol)*/
	if(!(*(*symbol_list+(*sum_symbols))))
        {
        	fprintf(stderr,"ERROR!!\nMemory allocation failed! here\n");/*prints error message*/
		return ALOCATION_ERROR;
        }
	/*Initialize the new symbol element*/
	strcpy((*(*symbol_list+(*sum_symbols)))->symbol_name,"");
	(*(*symbol_list+(*sum_symbols)))->symbol_val=0;
	strcpy((*(*symbol_list+(*sum_symbols)))->symbol_type,"");
	return VALID;
}

int add_defined_symbol_to_list(char *name, int valu,int *sum_symbols, char type[],symbol_details ***symbol_list)
{
	int i=0;
	/* Remove the colon character from the label name (if exists) */
	if(name[strlen(name)-ONE_SPOT] == ':')
	{
		name[strlen(name)-ONE_SPOT]='\0';
	}
	/* If this is the first symbol, allocate memory for the symbol list */
	if(*sum_symbols==0)/*check if this is the first symbol*/
	{
		(*symbol_list)=(symbol_details **)malloc(SIZE_OF_SYMBOL_POINTER);
		if(!(*symbol_list))
		{
			fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		(**symbol_list)=(symbol_details *)malloc(SIZE_OF_SYMBOL);
		if(!(**symbol_list))
		{
			fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
		 /* Add the symbol details to the first element of the symbol list */
		strcpy((**symbol_list)->symbol_name,name);
		(**symbol_list)->symbol_val=valu;
		strcpy((**symbol_list)->symbol_type,type);
		(*sum_symbols)++;
		  /* Allocate memory for the next symbol */
		if(allocate_memory_for_next_symbol(symbol_list,sum_symbols))
		{
			fprintf(stderr,"reallocation didn't work!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	else
	{
		/* Check if the label was already defined */
		while(i<*sum_symbols)
		{
			if(strcmp((*(*symbol_list+(i/**SIZE_OF_SYMBOL_POINTER*/)))->symbol_name,name)==0)
			{
				fprintf(stderr,"ERROR!\nDefining the same label more then once!!\n");/*prints error message*/
				return TEXT_ERROR;  /*leave the hole function*/
			}
			i++;
		}
		/* Add the symbol details to the next element in the symbol list */
		strcpy((*(*symbol_list+*sum_symbols))->symbol_name,name);
		(*(*symbol_list+*sum_symbols))->symbol_val=valu;
		strcpy((*(*symbol_list+*sum_symbols))->symbol_type,type);
		(*sum_symbols)++;
		 /* Allocate memory for the next symbol */
		if(allocate_memory_for_next_symbol(symbol_list,sum_symbols))
		{
			fprintf(stderr,"reallocation didn't work!\n");/*prints error message*/
			return ALOCATION_ERROR;
		}
	}
	return VALID;
}
