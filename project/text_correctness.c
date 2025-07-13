#include "text_correctness.h"
int split_line(char *split_line[],char line[])/*splites given line into a array of words!*/
{
	char *token;
	int i=0;
	char lincpy[MAX_LINE];
	strcpy(lincpy,line);
	token=strtok(lincpy," \t\n");/*skips first spaces and tabs!*/
	for(;i<MAX_LINE && token!=NULL; i++)
	{
		split_line[i]=malloc(strlen(token)+ONE_SPOT);/*inputs the word into a array*/
		if(split_line[i]==NULL)
		{
			printf("ERROR!\nMemory allocation failled!\n");
			return 1;
		}
		strcpy(split_line[i],token);
		token=strtok(NULL," \t\n,()");/*seperates the given line by seperating every time reaches a space, tab, a comma, or parenthesis!*/
	}
	split_line[i]=malloc(ONE_SPOT);
	if(split_line[i]==NULL)
	{
		printf("ERROR!\nMemory allocation failled!\n");
		return ALOCATION_ERROR;
	}
	strcpy(split_line[i],"");
	return VALID;
}

void free_split_line(char **split_line)/*frees the memory allocated for spliting given line to previes func!*/
{
	int i=0;
	for(;i<MAX_LINE_LENGTH && strcmp(split_line[i],""); i++)/*runs threw words of array till reaches empty word!*/
	{
		free(*(split_line+i));
	}
}

int jumpSpace(char * line, int index)/*jumps space till next word - returns the index of first letter of word!*/
{
	char c;
	while((c = line[index]) != '\n' && isspace(c))/*runs through the each line untill the first charcter*/
		index++;
	return index;/*returns the index of the first character*/
}


int jumpWord(char * line, int index)/*jumps curr word till next space - returns the index of space!*/
{
	char c;
	c = line[index];
	while(isalpha(c) || c==':')
	{
		index++;
		c = line[index];
	}
	return index;/*returns the index of the first character*/
}


int count_num_of_words_in_line(char *s)/*counts num of words in line*/
{
	char *token;
	int i=0;
	token=strtok(s," \t");
	while(token!=NULL)
	{
		token=strtok(NULL," \t,()");/*counts the given line by adding every time reaches a space, tab, a comma, or parenthesis!*/
		i++;
	}
	return i;
}

int is_empty_line(char *s)/*checks if line is an empty line*/
{
	char *token;
	token=strtok(s," \t\n");
	if(token==NULL)
	{
		return 1;
	}
	return 0;
}

int is_comment_line(char *s)/*checks if line is a comment line*/
{
	char c=';';/*checks if line starts with semicolon*/
	if(*s==c)
	{
		return 1;
	}
	return 0;
}

int is_command(char *word)/*checks if word is a command*/
{
	int i=0;
	while(i<SUM_OF_FUN_TYPES)/*runs threw array of funcs and compares the sent word to each fun name - returns the place in the array of the found func - else returns -1!*/
	{
		if(strcmp(word,func_table[i].func_name)==0)
		{
			return i;
		}
		i++;
	}
	return -1;
}

int is_instructer(char *word)/*checks if word is a instructer*/
{
	if(strcmp(word,".data")==0 || strcmp(word,".string")==0 || strcmp(word,".extern")==0 || strcmp(word,".entry")==0)
	{
		return 1;
	}
	return 0;
}

int instructer_type(char *word)
{
	int i;
	char *directive[] = {".data", ".string", ".extern", ".entry"};/*declares an array of four character pointers named directive and initializes it with four string literals.*/
	for(i = 0; i < SUM_OF_INSTRUCTERS; i++)
	{
		if(strcmp(word,(directive[i])) == 0)
		{
			return i;
		}
	}/*These lines define a nested loop that iterates through each element of the directive array and each word in the seperated_line array. If a word in seperated_line matches a directive in directive, the function returns the index of that directive in the directive array.*/
	return -1;
} 

int is_instructer_name(char *word)/*checks if word is a instructer name*/
{
	if(strcmp(word,"data")==0 || strcmp(word,"string")==0 || strcmp(word,"extern")==0 || strcmp(word,"entry")==0)
	{
		return 1;
	}
	return 0;
}

int is_register(char *word)/*checks if word is a register*/
{
	if(word[0]=='r' && (word[1]>=ASCI_CODE_FOR_ZERO && word[1]<=ASCI_CODE_FOR_SEVEN))/* checks that first letter is 'r' and the number is between 0 - 7!*/
	{
		return 1;
	}
	return 0;
}

int is_label(char *s)
{
	int i=1;/*initialize a counter variable i to 1*/
	if(strlen(s)>MAX_LABEL || !isalpha(s[0]) || is_register(s) || is_command(s)!=-1 || is_instructer(s) || is_instructer_name(s)) /* check if the length of the string is greater than 30 characters, or if the first character is not a letter, or if the string is a register, a command, an instruction, or an instruction name */
	{
		return 0;/*return 0 to indicate that the string is not a label*/
	}
	while(i<(strlen(s)-1))/*iterate over the characters in the string*/
	{
		if(!isalpha(s[i]) && !isdigit(s[i])) /*check if the character is not a letter or a digit*/
		{
			return 0;/*return 0 to indicate that the string is not a label*/
		}
		i++;/*increment the counter variable i*/
	}
	return 1;/*return 1 to indicate that the string is a label*/
}

int switch_digit(char *s)
{
	int convert=0;
	if(s[0]=='+')/*positive sign before number*/
	{
		convert=atoi(s+ONE_SPOT);
	}
	else if(s[0]=='-')/*negative sign before number*/
	{
		convert=MAX_COMMAND_VALUE;
		convert-=(atoi(s+ONE_SPOT)-1);
	}
	else/*no sign before number*/
	{
		convert=atoi(s);
	}
	return convert;/*return the value of convert*/
}


int reallocate_memory_coding(unsigned short **commands,int size)
{
	unsigned short * temp;
	temp=(unsigned short *)realloc(*commands,size);/*reallocate memory for the commands array*/
	if(temp==NULL)/*check if the memory allocation failed*/
	{
		fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");/*print an error message to stderr*/
		return ALOCATION_ERROR;/*return 1 to indicate an error*/
	}
	*commands=temp;
	return VALID;/*return 0 to indicate success*/
}

int split_line_correctness(char *split_line[],char line[])
{
	char *token;/*declare a pointer variable token*/
	int i=0;/*initialize a counter variable i to 0*/
	char lincpy[MAX_LINE];/*declare a character array lincpy with a size of 81*/
	strcpy(lincpy,line);/*copy the contents of the string line to the character array lincpy*/
	token=strtok(lincpy," \t\n");/*split the string lincpy into tokens separated by spaces, tabs, and newlines using the strtok function*/
	for(;i<MAX_LINE_LENGTH && token!=NULL; i++)/*iterate over the tokens and allocate memory for each token*/
	{
		split_line[i]=malloc(strlen(token)+ONE_SPOT);/*allocate memory for the token plus one extra character for the null terminator*/
		if(split_line[i]==NULL)/*check if the memory allocation failed*/
		{
			printf("ERROR!\nMemory allocation failled!\n");/*print an error message to stdout*/
			return ALOCATION_ERROR;/* return 1 to indicate an error*/
		}
		strcpy(split_line[i],token);/*copy the token to the split_line array*/
		token=strtok(NULL," \t\n");/*get the next token from the string*/
	}
	split_line[i]=malloc(ONE_SPOT);/*allocate memory for the last element in the split_line array*/
	if(split_line[i]==NULL)/*check if the memory allocation failed*/
	{
		printf("ERROR!\nMemory allocation failled!\n");/*print an error message to stdout*/
		return ALOCATION_ERROR;/*return 1 to indicate an error*/
	}
	strcpy(split_line[i],"");/*set the last element of the split_line array to an empty string*/
	return VALID;/*return 0 to indicate success*/
}

int is_digit(char * word)
{
	int i=1;/*initialize a counter variable i to 1*/
	int check=0;/*initialize a variable check to 0*/
	int pos=0;/*initialize a variable pos to 0*/
	int neg=0;/*initialize a variable neg to 0*/
	/*check if the first character in the string is a hash symbol*/
	if(word[0]!='#')
	{
        	return 0; /*if it's not a hash symbol, return 0 to indicate an error*/
	}
	/*check if the second character in the string is a minus sign or a plus sign*/
	if(word[i]=='-')
	{
		neg=ON; /*if it's a minus sign, set the variable neg to 1*/
		i++; /*increment the counter variable i by 1*/
	}
	else if(word[i]=='+')
	{
		pos=ON; /*if it's a plus sign, set the variable pos to 1*/
		i++; /*increment the counter variable i by 1*/
	}
	/*iterate over all the characters in the string from the checked character to the last character in the string*/
	for(;i<strlen(word);i++)
	{
		/*check if the current character in the string is a digit using the isdigit function*/
		if(!isdigit(word[i]))
		{
			return 0; /*if it's not a digit, return 0 to indicate an error*/
		}
	}
	/*if the string starts with a plus sign*/
	if(pos)
	{
		check=atoi(word+TOW_SPOTS); /*convert the string to an integer starting from the third character and store it in the variable check*/
		if(check>MAX_COMMAND_VALUE) /*check if the integer value is greater than the maximum allowed value*/
		{
			fprintf(stderr,"ERROR!\nThe number is out of boundaries!\n"); /*print an error message to stderr*/
			return 0; /*return 0 to indicate an error*/
		}
	}
	/*if the string starts with a minus sign*/
	else if(neg)
	{
 		check=atoi(word+TOW_SPOTS); /*convert the string to an integer starting from the third character and store it in the variable check*/
		if(check>MIN_COMMAND_VALUE) /*check if the integer value is less than the minimum allowed value*/
		{
			fprintf(stderr,"ERROR!\nThe number is out of boundaries!\n"); /*print an error message to stderr*/
			return 0; /*return 0 to indicate an error*/
		}
	}
	/*if the string starts with a digit or a hash symbol*/
	else
	{
		check=atoi(word+ONE_SPOT); /*convert the string to an integer starting from the second character and store it in the variable check*/
		if(check>MAX_COMMAND_VALUE) /*check if the integer value is greater than the maximum allowed value*/
		{
			fprintf(stderr,"ERROR!\nThe number is out of boundaries!\n");/*print an error message to stderr*/
			return 0; /*return 0 to indicate an error*/
		}
	}
	return 1; /*if the function reaches this point, the string contains only digits and is within the allowed range, so return 1 to indicate success*/
}

int is_digit_data(char * word)
{
	int i=0;/*initialize a counter variable i to 0*/
	int check=0;/*initialize a variable check to 0*/
	int pos=0;/*initialize a variable pos to 0*/
	int neg=0;/*initialize a variable neg to 0*/
	while(i<strlen(word))/*iterate over each character in the string*/
	{
		if(word[i]==',')/*check if the character is a comma*/
		{
			while(word[i+ONE_SPOT]!='\0')/*move all the characters after the comma one position to the left*/
			{
				word[i]=word[i+ONE_SPOT];
			}
		}
		i++;/*increment the counter variable i by 1*/
	}
	i=0;/*reset the counter variable i to 0*/
	if(word[i]=='-')/*check if the first character in the string is a minus sign*/
	{
		neg=ON;/*if it's a minus sign, set the variable neg to 1*/
		i++;/*increment the counter variable i by 1*/
	}
	else if(word[i]=='+')/*check if the first character in the string is a plus sign*/
	{
		pos=ON;/*if it's a plus sign, set the variable pos to 1*/
		i++;/*increment the counter variable i by 1*/
	}/*These lines initialize a variable i to 0 and check if the first character in the string is a minus sign or a plus sign. If so, the variable i is incremented by 1 in order to start checking from the first non-positive or non-negative character.*/
	for(;i<strlen(word);i++)/*iterate over all the characters in the string from the checked character to the last character in the string*/
	{
		if(!isdigit(word[i]))/*check if the current character in the string is a digit using the isdigit function*/
		{
	   		return 0;
		}
	}
	if(pos)/*if the string starts with a plus sign*/
	{
		check=atoi(word+ONE_SPOT);/*convert the string to an integer and store it in the variable check*/
		if(check>MAX_DATA_VALUE)/*check if the integer value is greater than the maximum allowed value*/
		{
	   		fprintf(stderr,"ERROR!\nThe number is out of boundries!\n");/*print an error message to stderr*/
	  		 return 0;/*return 0 to indicate an error*/
		}
	}
	else if(neg)/*if the string starts with a minus sign*/
	{
		check=atoi(word+ONE_SPOT);/*convert the string to an integer and store it in the variable check*/
		if(check>MIN_DATA_VALUE)/*check if the integer value is less than the minimum allowed value*/
		{
	  		 fprintf(stderr,"ERROR!\nThe number is out of boundries!\n");/*print an error message to stderr*/
	  		 return 0;/* return 0 to indicate an error*/
		}
	}/*This for loop iterates over all the characters in the string from the checked character to the last character in the string. In each iteration, it checks if the current character in the string is a digit using the isdigit function. If the character is not a digit, the function immediately returns 0, indicating that the string does not contain only digits.*/
	else
	{
		check=atoi(word);
		if(check>MAX_DATA_VALUE)
		{
	   		fprintf(stderr,"ERROR!\nThe number is out of boundries!\n");
	  		 return 0;
		}
	}
	return 1; /*if the function reaches this point, the string contains only digits and is within the allowed range, so return 1 to indicate success*/ 
}

