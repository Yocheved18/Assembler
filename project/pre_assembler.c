#include "pre_assembler.h"

bool macroRetire(FILE *file_as, FILE *file_am)
{
	/*char command[MAX_LINE_LENGTH];*/
	int i = 0, row = 0, index=0;
	char buffer[MAX_LINE_LENGTH]/*contains a line at a time*/, firstWord[MAX_LINE_LENGTH];
	ptrMacro pt, headMacroList = NULL;
	bool error = false;
	bool macro_flag = false;/*a variable indicating whether the code is currently inside a macro or not.*/
	rewind(file_as);
	while (fgets(buffer, MAX_LINE_LENGTH, file_as)!= NULL)/*reads a line from file_as until there are no more lines */
	{
		row++;/*counts the lines*/
		index = 0;/*resets the index*/
		i = 0;/*resets i*/
		if (!feof(file_as))/*returns true if the end-of-file indicator is not set, and false otherwise.*/
		{
			index = jumpSpace(buffer, index);/*jumps the pointer to the first position in the row.*/
			if (/*buffer[index] == '\0' || */buffer[index] == '\n')/*checks if the charecter is empty or there is a new line*/ 
			{
				continue; /*skip empty lines*/
			}
			while (index < MAX_LINE_LENGTH && !isspace(buffer[index])  ) {	/*Collecting the first word*/
		        	firstWord[i] = buffer[index];/*copies the first charecter to the firstword array*/
		        	i++;
		        	index++;
		    	}
			if(is_comment_line(firstWord))
			{
				continue;
			}
			if(is_empty_line(firstWord))
			{
				continue;
			}
			if (firstWord[index - ONE_SPOT] == ':') 
			{
				/*Line starts with a label, so just write it to the output file*/
				writesFile(file_am, buffer);
				continue;
			}
			firstWord[i] = '\0';/*adds null to the string to turn it into a valid string*/
			if (!strcmp(firstWord, "mcr"))/*checks if the first word is a macro*/
			{
				if (macro_flag == false) /*checks if is in a macro*/
				{/*Not a nested macro*/
				    	macro_flag = true;
				    	i = 0;
				   	index = jumpSpace(buffer, index);
				    	while (!isspace(buffer[index]) && i < MAX_LINE_LENGTH) 
					{	/*Collecting the second word*/
						firstWord[i] = buffer[index];/*copies the second charecter to the firstword array*/
						i++;
						index++;
				    	}
					firstWord[i] = '\0';
					if(/*is_label(firstWord)||*/ is_command(firstWord)!=-1)
					{
						printf("error in line %d index %d-Defining a macro in the name of a reserved word in the language",row, index);
		                		error = true;
					}
					if (!strcmp(firstWord, ""))/*checks that the macro name is not empty*/
					{
						printf("error in line %d - macro statement without name.\n", row);
						error = true;
				    	}
				}
				else if (sameMacro(firstWord,headMacroList))/*Checks if the macro name has already been defined in the macro lis*/
				{
					printf("error in line %d index %d - Conflict in the macro type, \na macro with the same name was previously defined",row, index);
					error = true;
				}
				index = jumpSpace(buffer, index);
				if (buffer[index] != '\n') 
				{
					printf("error in line %d index %d - excess characters after the macro name", row, index);
					error = true;
				}
				if(!error)
				{
				        addToMacroList(firstWord,&headMacroList);/*if no errors add it to the macro list*/

				}
				else
		        	{
					printf("error in line %d index %d - set up a nested macro.", row, index);/*if thers errors*/
					/*exit(0);*/
					return true;
				}
			}
			else if (!strcmp(firstWord, "endmcr"))							/*checks if the first word in the line is the end of a macro definition.*/
			{
				if (macro_flag == false) /*checks that we did not find more macro endings than macro definitions. If so, we will print an error.*/
				{
					printf("error in line %d index %d - attempt to close undeclared macro.", row, index);
					error=true;
				}
				else 
				{
					index = jumpSpace(buffer, index);
					if (buffer[index] != '\n') 
					{
						printf("error in line %d index %d - excess characters after the endmacro.\n", row, i);
						error=true;
					}
					else
					{
					/*finish writing the macro content*/
						headMacroList->content[headMacroList->index] = '\0';
						headMacroList->index = headMacroList->index + ONE_SPOT;
						macro_flag = false;/*
variable reset*/
					}
				}
			}
			else if (macro_flag == true )/*checks if we are currently inside a macro.*/
			{
				if(!error) 
				{
					i = 0;
					while(i < MAX_LINE_LENGTH && buffer[i] != '\n' && headMacroList->index < MAX_LINE_LENGTH)
					{/*finish writing the macro content*/
						headMacroList->content[headMacroList->index] = buffer[i];
						i++;
						headMacroList->index = headMacroList->index + ONE_SPOT;
					}
					headMacroList->content[headMacroList->index] = '\n';
					headMacroList->index = headMacroList->index + ONE_SPOT;
				}
			}
			else
			{
				pt = sameMacro(firstWord, headMacroList);
				if (pt)/*checks if the macro name exists in the macro list*/
					writesFile(file_am, pt->content);/*writs the contents of the macro to the promoted file*/
				else
					fputs(buffer, file_am);/*If not, writes the original line to the promoted file.
*/
			}

		}
		if(i == MAX_LINE_LENGTH && buffer[index-ONE_SPOT] != '\n')
		{
			printf("error in line %d - The line is longer than the allowed length\n", row);
			error=true;
		}
	}
	fputc('\n', file_am);
	freeMacroList(&headMacroList);
	return error;
}

bool addToMacroList(char * name, ptrMacro *head)
{
	ptrMacro new;/*declare a new pointer to macroLine*/
	new = (ptrMacro)malloc(sizeof(MacroList));/*allocate memory for the new macro*/	
	if(!new)/*check if the allocation was successful*/
	{ 
		printf("\n cannot allocate memory for a new macro \n");/**/
		/*exit(0);*/
		return true;/*exit the program if the allocation was not successful*/
	}
	new->macroName = (char*) malloc(strlen(name) + ONE_SPOT); /* Allocate memory for the macro name */
    	if (!new->macroName) 
	{
        	printf("\nCannot allocate memory for the macro name\n");
		/*exit(0);*/
		return true;/*exit the program if the allocation was not successful*/
	}
    	strcpy(new->macroName, name); /* Copy the macro name to the new macro */
    	new->content = (char*) malloc(MAX_LINE_LENGTH); /* Allocate memory for the macro content */
    	if (!new->content) 
	{
        	printf("\nCannot allocate memory for the macro content\n");
		/*exit(0);*/
		return true;/*exit the program if the allocation was not successful*/
	}
	strcpy(new-> macroName, name);/*copy the name of the macro to the new macro*/		
	new->next = *head;/*Add the new macro to the top of the list*/
   	*head = new;/* Update the head of the list to point to the new macro*/
	new -> index = 0;/*Set the new macro's index to 0*/
	return false;
}

ptrMacro sameMacro(char * macro_name, ptrMacro head )/*takes a string pointer and a pointer to a macroLine struct, and returns a pointer to a macroLine struct.*/
{
	while(head)/*While the head pointer is not NULL*/					
	{
		if(!strcmp(macro_name ,head -> macroName))/*If the macro name is already defined*/
			return head; /*return a pointer to the current node.*/
		head = head-> next;/*Advance head to the next node in the linked list.*/
	}
	return NULL;/*If no matching node is found in the linked list, return NULL.*/
}

void freeMacroList(ptrMacro *head)/*frees the memory allocated for a linked list of macro lines*/
{
	ptrMacro p;/*declare a pointer to a macroLine struct*/
	while(*head)/*while the head pointer is not NULL*/
	{
		p = *head;/*stores the head pointer in p*/
		*head = p->next;/*sets the head pointer to the next node in the list*/
		free(p);/*free the memory allocated for the current node*/
	}
}

void writesFile(FILE * file_am, char * line)/*copy lines*/
{
	int i = 0;
	while( line[i]!='\0')/*goes through the file until it reads all lines*/
	{
		fputc(line[i], file_am);/*writes the character i of the string line to the file*/
		i++;
	}
}




