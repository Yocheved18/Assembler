#include "errors.h"
int line_handle_error(char *line,char ic_cur, char *file_name)
{
	int i = 0;/*Initialize loop counter i*/
	int instruct_type=-1;/*Variable to store whether the line is an instruction*/
	int func_type=-1;/*Variable to store whether the line is a command*/
	int labelFlag = 0;/*Initialize labelFlag to 0 (false)*/
	char line_cpy[MAX_LINE];
	char *seperated_line[MAX_LINE_LENGTH];/*Array to store the separated words of the line*/
	char line_cpy_whole[MAX_LINE];
	int num_of_parts=0;
	int num_of_words=0;
	count_num_of_words_in_line(line_cpy_whole);/* Count the number of words in the input line */
	strcpy(line_cpy_whole,line);
	num_of_words=count_num_of_words_in_line(line_cpy_whole);
	/* Split the input line into separate words and store them in the "seperated_line" array */
	if((num_of_parts=count_split_line_correctness(seperated_line,line))==-1)
	{
		fprintf(stderr,"Memory error!\n");
		return ALOCATION_ERROR;
	}
	if(ic_cur==-1)/*If ic_cur is -1, set it to 0*/
	{
		ic_cur=0;
	}
	if((num_of_parts=count_split_line_correctness(seperated_line,line))==-1)/*This line calls the split_line_correctness() function to split the input line into separate words and store them in the seperated_line array.*/
	{
		fprintf(stderr,"Memory error!\n");/*prints an error message*/
		return ALOCATION_ERROR;/*return an allocation error code*/
	}/*If the function returns a false value, indicating an error in the splitting process, an error message is printed and the function returns 1.*/
	/*checks if it is a label. If it is, sets the "labelFlag" variable to true.*/
	strcpy(line_cpy,*seperated_line);
	line_cpy[strlen(line_cpy)-ONE_SPOT]='\0';
	/* Check if it is a label. If it is, set the "labelFlag" variable to true */
	if((is_label(line_cpy)) && seperated_line[0][strlen(*seperated_line)-ONE_SPOT]==':')
	{
		labelFlag = ON;
		i++;
	}
	/*If it is not a label and "labelFlag" is still false, checks if the line is an instruction or command.*/
	/*If it is a command, calls the command_line_correctness() function.*/
	if((func_type=is_command(seperated_line[i]))>-1)
	{
		if(command_line_correctness(file_name,line,ic_cur,num_of_words,num_of_parts,labelFlag,seperated_line, func_type))
		{
			free_split_line(seperated_line);/*Free memory allocated for separated_line*/
			/*fprintf(stderr, "Memory error!\n");prints an error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}
		free_split_line(seperated_line);/*Free memory allocated for separated_line*/
		return VALID;/*return a success code*/
	}
	else if((instruct_type=instructer_type(seperated_line[i]))!=-1)
	{
		if(instruction_line_correctness(file_name,line,ic_cur, num_of_words,labelFlag,seperated_line,instruct_type))
		{
			free_split_line(seperated_line);/*Free memory allocated for separated_line*/
			/*fprintf(stderr, "ERROR\n");prints an error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}
		free_split_line(seperated_line);/*Free memory allocated for separated_line*/
		return VALID;/*return a success code*/
	}
	else
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s invalid word for begining of line!\n",ic_cur,file_name);
		free_split_line(seperated_line);
		return TEXT_ERROR;
	}
	free_split_line(seperated_line);/* Check if it is a label. If it is, set the "labelFlag" variable to true */
	return VALID;/*return a success code*/
}

int command_line_correctness(char * file_name,char * line,int ic_cur,int num_of_words,int num_of_parts,int labelFlag,char **seperated_line,int func_type)
{
	if(func_table[func_type].type_operands==JUMP_TYPE)/*Check command line according to the type of operands*/
	{
		if(jump_type_command(file_name,line,ic_cur,num_of_words,num_of_parts,labelFlag,seperated_line,func_type))
		{
			return TEXT_ERROR;
		}
		return VALID;
	}
	else if(func_table[func_type].type_operands==TOW_OPERAND_TYPE)
	{
		if(tow_operand_type_command(file_name,line,ic_cur,num_of_words,num_of_parts,labelFlag,seperated_line,func_type))
		{
			return TEXT_ERROR;
		}
		return VALID;
	}
	else if(func_table[func_type].type_operands==ONE_OPERAND_TYPE)
	{
		if(one_operand_type_command(file_name,line,ic_cur,num_of_words,num_of_parts,labelFlag,seperated_line,func_type))
		{
			return TEXT_ERROR;
		}
		return VALID;
	}
	else
	{
		if(num_of_words!=(1+labelFlag))/*Check for none operand type command line*/
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s wrong amount of parameters for none operand func type line!\n",ic_cur,file_name);
			return TEXT_ERROR;
		}
		else if(strstr(line,","))
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s Ther should be no commas in none operand line!\n",ic_cur,file_name);
			return TEXT_ERROR;
		}
	}
	return VALID;
}


int instruction_line_correctness(char * file_name,char * line,int ic_cur,int num_of_words,int labelFlag,char **seperated_line,int instruct_type)
{
	if(instruct_type==0)/*If the instruction type is a data instruction*/
	{
		if(data_line_correctness(line,seperated_line+ONE_SPOT+labelFlag,ic_cur,file_name,labelFlag))/*Check the correctness of the data line and handle error*/
		{
			/*fprintf(stderr,"ERROR!\n");prints an error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}
	}
	else if(instruct_type==1)/*If the instruction type is a string instruction*/
	{
		if(string_instructions(file_name,line,ic_cur,num_of_words,labelFlag,seperated_line))
		{
			return TEXT_ERROR;
		}
		return VALID;
	}
	else if(instruct_type==2)/*If the instruction type is an extern instruction*/
	{
		if(labelFlag)
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s In a extern instrution there shouldn't be a label before the instruction!\n",ic_cur,file_name);
			return TEXT_ERROR;
		}
		if(extern_entery_instructions(file_name,line,ic_cur,num_of_words, seperated_line,"extern"))
		{
			return TEXT_ERROR;
		}
		return VALID;
	}
	else if(instruct_type==3)/*If the instruction type is an entry instruction*/
	{
		if(labelFlag)
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s In a entery instrution there shouldn't be a label before the instruction!\n",ic_cur,file_name);
			return TEXT_ERROR;
		}
		if(extern_entery_instructions(file_name,line,ic_cur,num_of_words, seperated_line,"entery"))
		{
			return TEXT_ERROR;
		}
		return VALID;
	}
	return VALID;
}

int data_line_correctness(char* line,char ** seperated_line_with_commas,int ic_cur,char * file_name,int labelFlag)
{
	int i=0,j=0;/*Initialize variables i, j to zero*/
	int first_pos=0;/*Initialize first_pos variable to zero*/
	char current_num_str[MAX_LABEL];/*Declare an array to store the current number string*/
	char *seperated_line[MAX_LINE_LENGTH];/*Declare an array of pointers to store separated words in the line*/
	char testing_line[MAX_LINE];/*Declare an array to store a testing line*/
	memset(testing_line,0,MAX_LINE);/*Initialize the testing_line array with zeros*/
	memset(current_num_str,0,MAX_LABEL);/*Initialize the current_num_str array with zeros*/
	first_pos=jumpSpace(line,first_pos);/*Skip spaces in the line*/	
	if(labelFlag)/*Check if the label flag is set*/
	{
		first_pos=jumpWord(line,first_pos);/*Skip the label*/
		first_pos=jumpSpace(line,first_pos);/*Skip spaces after the label*/
	}
	first_pos+=LENGTH_OF_DATA;/*Move first_pos to the first character after the ".data" directive*/
	first_pos=jumpSpace(line,first_pos);/*Skip spaces after the directive*/
	if(split_line(seperated_line, line))/*Split the line into words*/
	{
		/*fprintf(stderr,"Error!!!\n");prints an error message*/
		return ALOCATION_ERROR;/*return an allocation error code*/
	}
	if(seperated_line_with_commas[0][0] == ',')/*Check for comma before the first number*/
	{
		free_split_line(seperated_line);/*Free memory allocated for separated_line*/
		fprintf(stderr, "ERROR: in line: %d in file: %s There should be no comma before the first number\n", ic_cur,file_name);/*prints an error message*/
		return ALOCATION_ERROR;/*return an allocation error code*/
	}
	for(i = 0; strcmp(seperated_line[i+ONE_SPOT+labelFlag], "")!=0; i++)/*Iterate through separated_line, checking for valid integers*/
	{
		strcpy(current_num_str,seperated_line[i+ONE_SPOT+labelFlag]);
		/* Check that each word contains only valid digits, and optional '+' or '-' signs */
		if(!is_digit_data(current_num_str))
		{
			free_split_line(seperated_line);/*Free memory allocated for separated_line*/
			fprintf(stderr,"ERROR: in line: %d in file: %s Data values must be valid integers\n", ic_cur,file_name);/*prints an error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}
	}
	while(first_pos<strlen(line))/*Remove spaces from the line and store it in testing_line*/
	{
		if(!isspace(line[first_pos]))
		{
			testing_line[j]=line[first_pos];
			j++;
		}
		first_pos++;
	}
	first_pos=0;/*Reset first_pos variable to zero
*/
	while(first_pos<strlen(testing_line)-ONE_SPOT)/*Check for invalid characters and consecutive commas in testing_line*/
	{
		while(isdigit(testing_line[first_pos]) || testing_line[first_pos]=='+' || testing_line[first_pos]=='-')/*Skip valid digits and signs*/
		{
			first_pos++;
		} 
		if(testing_line[first_pos]==',')/*Check for consecutive commas*/
		{
			if(testing_line[first_pos+ONE_SPOT]==',')
			{
				free_split_line(seperated_line);/*Free memory allocated for separated_line*/
				fprintf(stderr,"ERROR: In line: %d in file: %s In a data line there shouldn't be more then one comma in a row!\n",ic_cur,file_name);/*prints an error message*/
				return ALOCATION_ERROR;/*return an allocation error code*/
			}
			first_pos++;/*Increment first_pos to move past the comma*/
		}
		else if(first_pos<strlen(testing_line)-ONE_SPOT)
		{
			free_split_line(seperated_line);/*Free memory allocated for separated_line*/
			fprintf(stderr,"ERROR: In line: %d in file: %s In a data line there should be only digits or commas!\n",ic_cur,file_name);/*prints an error message*/
			return ALOCATION_ERROR;/*return an allocation error code*/
		}	
	}
	if(seperated_line[i][strlen(seperated_line[i])] == ',') /* Check the last character of the string */
	{
		free_split_line(seperated_line);/*Free memory allocated for separated_line*/
		fprintf(stderr, "ERROR: In line: %d in file: %s In data line there should be no comma after the last number\n",ic_cur,file_name);/*prints an error message*/
		return ALOCATION_ERROR;/*return an allocation error code*/
	}
	free_split_line(seperated_line);/*Free the memory allocated for separated_line*/
	return VALID;/*return a success code*/	
}

int string_instructions(char * file_name,char * line,int ic_cur,int num_of_words,int labelFlag,char **seperated_line)
{
	int i=1;
	int length;
	length=strlen(seperated_line[1+labelFlag])-1;
	if(num_of_words!=(2+labelFlag))/*The line does not contain just a set of letters*/
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s In a string instrution there should be just a set of letters!\n",ic_cur,file_name);
		return TEXT_ERROR;
	}
	if(strstr(line,","))/*The line contains commas*/
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s In a string instrution there should be no commas!\n",ic_cur,file_name);
		return TEXT_ERROR;
	}
	if(seperated_line[1+labelFlag][0] != '\"' || seperated_line[1+labelFlag][length] != '\"')/*The string is not enclosed in double quotes*/
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s\nString line must be enclosed in double quotes\n", ic_cur,file_name);
		return TEXT_ERROR;
	} 
	while(i<length)/*Check that each character in the string is a valid letter*/
	{
		if(!isalpha(seperated_line[1+labelFlag][i]))
		{
		    	fprintf(stderr,"ERROR: In line: %d in file: %s\nString value must contain valid leters\n", ic_cur,file_name);
		    	return TEXT_ERROR;
		}
		i++;
	}
	return VALID;/*Return a success code*/
}

int extern_entery_instructions(char * file_name,char * line,int ic_cur,int num_of_words,char **seperated_line,char * type)
{
	char * token=NULL;
	char *linecpy;
	if(num_of_words!=2)/*Error if there is no label in the instruction*/
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s In a %s instrution there should be just a label!\n",ic_cur,file_name,type);
			return TEXT_ERROR;
	}
	else if(!is_label(seperated_line[1]))/*Error if the second word in the instruction is not a label*/
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s In a %s instrution there should be a label!\n",ic_cur,file_name,type);
			return TEXT_ERROR;
	}
	linecpy=(char *)malloc(strlen(line));
	if(linecpy==NULL)/*emory allocation error*/
	{
		fprintf(stderr,"Memory error!\n");
		return ALOCATION_ERROR;
	}
	strcpy(linecpy,line);
	token=strtok(linecpy,",()+_-=*&!");
	if(strcmp(token,line)!=0)/*Error if the instruction contains invalid signs*/
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s In a %s instrution there should be no signs!\n",ic_cur,file_name,type);
		token=NULL;		
		free(linecpy);
		return TEXT_ERROR;
	}
	token=NULL;
	free(linecpy);
	return VALID;
}

int one_operand_type_command(char * file_name,char * line,int ic_cur,int num_of_words,int num_of_parts,int labelFlag,char **seperated_line,int type)
{
	if(num_of_words==(ONE_OPERAND_TYPE+1+labelFlag))/*no perenthsis*/
	{
		if(strstr(line,","))
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s Ther should be no commas in one operand line!\n",ic_cur,file_name);
			return TEXT_ERROR;
		}
		if(checks_every_word(seperated_line+1+labelFlag,file_name,ic_cur,"",func_table[type].dest_operands))
		{
			return TEXT_ERROR;
		}
		return 0;
	}
	else
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s wrong amount of parameters for one operand func type line!\n",ic_cur,file_name);
		return TEXT_ERROR;
	}
	return 0;
}

int tow_operand_type_command(char * file_name,char * line,int ic_cur,int num_of_words,int num_of_parts,int labelFlag,char **seperated_line,int type)
{
	char *ptr=NULL;
	char *split_line1[8];
	if(num_of_words==(TOW_OPERAND_TYPE+1+labelFlag))/*no perenthsis*/
	{
		if(num_of_parts==(TOW_OPERAND_TYPE+labelFlag))
		{
			if((ptr=strstr(seperated_line[1+labelFlag],","))==NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s Missing comma between operands!\n",ic_cur,file_name);
				return TEXT_ERROR;
			}
			else if(strstr(ptr+1,",")!=NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s Too many commas between operands!\n",ic_cur,file_name);
				ptr=NULL;
				return TEXT_ERROR;
			}
			else
			{
				ptr=NULL;
				if(count_split_line(split_line1,seperated_line[1+labelFlag])!=TOW_OPERAND_TYPE)
				{
					fprintf(stderr,"ERROR: In line: %d in file: %s\n",ic_cur,file_name);
					return TEXT_ERROR;
				}
				if(checks_every_word(split_line1,file_name,ic_cur,func_table[type].source_operands,func_table[type].dest_operands))
				{
					free_split_line(split_line1);
					return TEXT_ERROR;
				}
				free_split_line(split_line1);
				return VALID;
			}
			return 0;
		}
		else if(num_of_parts==(TOW_OPERAND_TYPE+1+labelFlag))
		{
			if(seperated_line[1+labelFlag][0]==',')
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s No commas should apear before first operand!\n",ic_cur,file_name);
				return TEXT_ERROR;
			}
			if((ptr=strstr(seperated_line[1+labelFlag],","))==NULL)
			{
				if((ptr=strstr(seperated_line[2+labelFlag],","))==NULL)
				{
					fprintf(stderr,"ERROR: In line: %d in file: %s Missing comma between operands!\n",ic_cur,file_name);
					ptr=NULL;
					return TEXT_ERROR;
				}
				else if(strstr(ptr+1,",")!=NULL)
				{
					fprintf(stderr,"ERROR: In line: %d in file: %s Too many commas between operands!\n",ic_cur,file_name);
					ptr=NULL;
					return TEXT_ERROR;
				}
				else if(seperated_line[2+labelFlag][0]!=',')
				{
					fprintf(stderr,"ERROR: In line: %d in file: %s A commas should apear between the operands!\n",ic_cur,file_name);
					ptr=NULL;
					return TEXT_ERROR;
				}
				ptr=NULL;
			}
			else if(strstr(ptr+1,",")!=NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s Too many commas between operands!\n",ic_cur,file_name);
				ptr=NULL;
				return TEXT_ERROR;
			}
			else if((ptr=strstr(seperated_line[2+labelFlag],","))!=NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s Too many commas between operands!\n",ic_cur,file_name);
				ptr=NULL;
				return TEXT_ERROR;
			}
			if(split_line(split_line1,line))
			{
				return ALOCATION_ERROR;
			}
			if(checks_every_word(split_line1+1+labelFlag,file_name,ic_cur,func_table[type].source_operands,func_table[type].dest_operands))
			{
				free_split_line(split_line1);
				return TEXT_ERROR;
			}
			ptr=NULL;
			free_split_line(split_line1);
			return VALID;
		}
		else if(num_of_parts==(TOW_OPERAND_TYPE+2+labelFlag))
		{
			if(strstr(seperated_line[1+labelFlag],",")!=NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s wrong place for comma!\n",ic_cur,file_name);
				return TEXT_ERROR;
			}
			else if((ptr=strstr(seperated_line[2+labelFlag],","))!=NULL)
			{
				if(strstr(ptr+1,",")!=NULL)
				{
					fprintf(stderr,"ERROR: In line: %d in file: %s Extra commas!\n",ic_cur,file_name);
					ptr=NULL;
					return TEXT_ERROR;
				}
				if((ptr=strstr(seperated_line[3+labelFlag],","))!=NULL)
				{
					fprintf(stderr,"ERROR: In line: %d in file: %s Too many commas between operands!\n",ic_cur,file_name);
					ptr=NULL;
					return TEXT_ERROR;
				}
				ptr=NULL;
			}
			else if((ptr=strstr(seperated_line[3+labelFlag],","))!=NULL)
			{
					fprintf(stderr,"ERROR: In line: %d in file: %s Extra commas!\n",ic_cur,file_name);
					ptr=NULL;
					return TEXT_ERROR;
			}
			ptr=NULL;
			if(split_line(split_line1,line))
			{
				return ALOCATION_ERROR;
			}
			if(checks_every_word(split_line1+1+labelFlag,file_name,ic_cur,func_table[type].source_operands,func_table[type].dest_operands))
			{
				free_split_line(split_line1);
				return TEXT_ERROR;
			}
			free_split_line(split_line1);
			return VALID;
		}
		else
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s Too many commas in tow operands func type line!\n",ic_cur,file_name);
			return TEXT_ERROR;
		}
		return VALID;
	}
	else
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s wrong amount of parameters in tow operands func type line!\n",ic_cur,file_name);
		return TEXT_ERROR;
	}
	return VALID;
}

int checks_every_word(char *split_line[],char * file_name,int ic_cur,char * operand_type1,char * operand_type2)
{
	int i=0,j=0;
	char *op_ty[2];
	char *operand[]={"source","dest"};
	op_ty[0]=(char *)malloc(sizeof(operand_type1));
	if(op_ty[0]==NULL)
		return ALOCATION_ERROR;
	op_ty[1]=(char *)malloc(sizeof(operand_type2));
	if(op_ty[1]==NULL)
		return ALOCATION_ERROR;
	strcpy(op_ty[0],operand_type1);
	strcpy(op_ty[1],operand_type2);
	if(strcmp(op_ty[0],"")==0)
	{
		j=1;
	}
	while(strcmp(split_line[i],"")!=0)
	{
		if(is_register(split_line[i]))
		{
			if(!(strstr(op_ty[j],"3")))
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s , A register can't be the %s operand in this command!\n",ic_cur,file_name,operand[j]);
				free(op_ty[0]);
				free(op_ty[1]);
				return ALOCATION_ERROR;
			}
		}
		else if(is_label(split_line[i]))
		{
			if(strstr(op_ty[j],"1")==NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s , A lable can't be the %s operand in this command!\n",ic_cur,file_name,operand[j]);
				free(op_ty[0]);
				free(op_ty[1]);
				return ALOCATION_ERROR;
			}
		}
		else if(is_digit(split_line[i]))
		{
			if(strstr(op_ty[j],"0")==NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s , A digit can't be the %s operand in this command!\n",ic_cur,file_name,operand[j]);
				free(op_ty[0]);
				free(op_ty[1]);
				return ALOCATION_ERROR;
			}
		}
		else
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s ,The %s operand is not a valid word!\n",ic_cur,file_name,operand[j]);
			free(op_ty[0]);
			free(op_ty[1]);
			return ALOCATION_ERROR;
		}
		i++;
		j++;
	}
	free(op_ty[0]);
	free(op_ty[1]);
	return VALID;
}

int count_split_line_correctness(char *split_line[],char line[])
{
	char *token;
	int i=0;
	char lincpy[MAX_LINE];
	strcpy(lincpy,line);
	token=strtok(lincpy," \t\n");
	for(;i<MAX_LINE && token!=NULL; i++)
	{
		split_line[i]=malloc(strlen(token)+ONE_SPOT);
		if(split_line[i]==NULL)
		{
			printf("Memory error!\n");
			return -1;
		}
		strcpy(split_line[i],token);
		token=strtok(NULL," \t\n");
	}
	split_line[i]=malloc(ONE_SPOT);
	if(split_line[i]==NULL)
	{
		printf("Memory error!\n");
		return -1;
	}
	strcpy(split_line[i],"");
	return i;
}

int jump_type_command(char * file_name,char * line,int ic_cur,int num_of_words,int num_of_parts,int labelFlag,char **seperated_line,int type)
{
	char *ptr=NULL;
	char *split_line1[8];
	if(num_of_words==(JUMP_TYPE-1+labelFlag))/*no perenthsis*/
	{
		if(num_of_parts==(2+labelFlag))
		{
			if(strstr(line,",")!=NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s Unnecessary comma!\n",ic_cur,file_name);
				return TEXT_ERROR;
			}
			if(!is_label(seperated_line[1+labelFlag]))
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s , A jump func has to have a label parameter!\n",ic_cur,file_name);
				return TEXT_ERROR;
			}
			return 0;
		}
		else
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s wrong amount of parameters in jump type line!\n",ic_cur,file_name);
			return TEXT_ERROR;
		}
	}
	else if(num_of_words==(JUMP_TYPE+1+labelFlag))/*with perenthsis*/
	{
		if(num_of_parts==(2+labelFlag))
		{
			if((ptr=strstr(seperated_line[1+labelFlag],","))==NULL)
			{
				fprintf(stderr,"ERROR: In line: %d in file: %s Missing comma between parameters in jump type line!\n",ic_cur,file_name);
				return TEXT_ERROR;
			}
			else if(strstr(ptr+1,",")!=NULL)
			{
				ptr=NULL;
				fprintf(stderr,"ERROR: In line: %d in file: %s Too many comma between parameters in jump type line!\n",ic_cur,file_name);
				return TEXT_ERROR;
			}
			else
			{
				ptr=NULL;
				if(split_line_jmp(split_line1,seperated_line[1+labelFlag])!=JUMP_TYPE)
				{
					fprintf(stderr,"ERROR: In line: %d in file: %sttt\n",ic_cur,file_name);
					free_split_line(split_line1);
					return TEXT_ERROR;
				}
				if(!is_label(split_line1[0]))
				{
					fprintf(stderr,"ERROR: In line: %d in file: %s , A jump func has to have a label parameter!\n",ic_cur,file_name);
					free_split_line(split_line1);
					return TEXT_ERROR;
				}
				if(checks_every_word(&split_line1[1],file_name,ic_cur,"013","013"))
				{
					free_split_line(split_line1);
					return TEXT_ERROR;
				}
				if(seperated_line[1+labelFlag][strlen(seperated_line[1+labelFlag])-1]!=')')
				{
					free_split_line(split_line1);
					fprintf(stderr,"ERROR: In line: %d in file: %s , A jump func has to close its parameter with parenthesis!\n",ic_cur,file_name);
					return TEXT_ERROR;
				}
				free_split_line(split_line1);
				return VALID;
			}
		}
		else
		{
			fprintf(stderr,"ERROR: In line: %d in file: %s In a jump command there should be no spaces between the parameters!\n",ic_cur,file_name);
			return TEXT_ERROR;
		}
	}
	else
	{
		fprintf(stderr,"ERROR: In line: %d in file: %s wrong amount of parameters in jump type line!\n",ic_cur,file_name);
		return TEXT_ERROR;
	}
	return VALID;
}

int split_line_jmp(char *split_line[],char line[])
{
	char *token;
	int i=0;
	char lincpy[MAX_LINE];
	strcpy(lincpy,line);
	token=strtok(lincpy,"(),");
	for(;i<MAX_LINE && token!=NULL; i++)
	{
		split_line[i]=malloc(strlen(token)+ONE_SPOT);
		if(split_line[i]==NULL)
		{
			printf("Memory error!\n");
			return -1;
		}
		strcpy(split_line[i],token);
		token=strtok(NULL,"(),");
	}
	split_line[i]=malloc(ONE_SPOT);
	if(split_line[i]==NULL)
	{
		printf("Memory error!\n");
		return -1;
	}
	strcpy(split_line[i],"");
	return i;
}

int count_split_line(char *split_line[],char line[])
{
	char *token;
	int i=0;
	char lincpy[MAX_LINE];
	strcpy(lincpy,line);
	token=strtok(lincpy," ,()\t\n");
	for(;i<MAX_LINE && token!=NULL; i++)
	{
		split_line[i]=malloc(strlen(token)+ONE_SPOT);
		if(split_line[i]==NULL)
		{
			printf("Memory error!\n");
			return -1;
		}
		strcpy(split_line[i],token);
		token=strtok(NULL," \t\n,()");
	}
	split_line[i]=malloc(ONE_SPOT);
	if(split_line[i]==NULL)
	{
		printf("Memory error!\n");
		return -1;
	}
	strcpy(split_line[i],"");
	return i;
}

