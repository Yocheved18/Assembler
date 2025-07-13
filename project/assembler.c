#include "assembler.h"

int main(int argc, char * argv[])
{
	int fl=1;
	printf("\n*************************************\n\n");
	fprintf(stdout,"Welcome to the assembler program!\n");
	if(argc<2)
	{
		fprintf(stderr,"ERORR!\nNO FILES FOUND!\n\n");
		fprintf(stdout,"Assembler program has stoped!\tTo continue restart the program and input files!\n");
	fprintf(stdout,"Assembler program is going to sleep for now!\t$$$$$$$$!\n");
		return ALOCATION_ERROR;
	}
	for(;fl<argc;fl++)
	{
		fprintf(stdout,"We are now doing the assembler on  - %s.as - file!\n",argv[fl]);
		file_handling(argv[fl]);
	}
	fprintf(stdout,"Assembler program has stoped!\tTo continue restart the program and input files!\n");
	fprintf(stdout,"Assembler program is going to sleep for now!\t$$$$$$$$!\n");
	return 0;
}

int file_handling(char *file_name)
{
	char * file_name_as=NULL;
	char * file_name_am=NULL;
	FILE *file_as, *file_am;
	int ic=-1, dc=-1,j=0;
	int sum_symbols=0;
	char c=0;
	unsigned short **commands=NULL;
	unsigned short **data=NULL;
	int is_extern=0;
	int is_entery=0;
	symbol_details ** symbol_list=NULL;
	/*Allocate memory for the command and data arrays*/
	commands=(unsigned short **)malloc(sizeof(short *));
	if(commands==NULL)
	{
		fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");
		return ALOCATION_ERROR;
	}
	data=(unsigned short **)malloc(sizeof(short *));
	if(data==NULL)
	{
		fprintf(stderr,"ERROR!!\nMemory allocation failed!\n");
		return ALOCATION_ERROR;
	}
	/*Allocate memory for the file names*/
	file_name_as=(char *)calloc(FILENAME_MAX-3,sizeof(char));
	if(file_name_as==NULL)         
	{
		fprintf(stderr,"ERROR!\nNo space for allocation\n");
		return 1;
	}
	strcpy(file_name_as,file_name);
	strcat(file_name_as,as_end_file_name);
	if(!(file_as = fopen(file_name_as,"r")))/*opens the input file*/
	{
		fprintf(stderr,"File: %s does not exist\n",file_name_as);
		return TEXT_ERROR;
	}
	if((c=fgetc(file_as))==EOF)/*cheks if the file is empty*/
	{
		fprintf(stderr,"The file: %s - is empty!!\n",file_name_as);
		return TEXT_ERROR;
	}
	/*Allocate memory for the output file name*/
	file_name_am=(char *)calloc(strlen(file_name_as),sizeof(char));
	if(file_name_am==NULL)
	{
		fprintf(stderr,"ERROR!\nNo space for allocation\n");
		return ALOCATION_ERROR;
	}
	strcpy(file_name_am,file_name);
	strcat(file_name_am,am_end_file_name);
	if(!(file_am = fopen(file_name_am,"w+")))/*opens the input file*/
	{
		fprintf(stderr,"File: %s does not exist\n",file_name_am);
		return TEXT_ERROR;
	}
	/*Perform preprocessing using the macroRetire function*/
	if(macroRetire(file_as, file_am))
	{
		fprintf(stderr,"ERRORS FOUND IN PRE ASSEMBLER!\n");
		return ALOCATION_ERROR;
	}
	rewind(file_am);/*sets the file position indicator to the beginning of the file file_am.*/
	if(first_pass(file_am,file_name_am,commands,data,&ic,&dc,&sum_symbols,&symbol_list,&is_extern,
	&is_entery))/*Call the first_pass function*/
	{
		fprintf(stderr,"Error in file: %s - in first pass!\n",file_name);
		if(sum_symbols>0)
		{
			while(j<=sum_symbols)		
			{
				free(*(symbol_list+j));
				j++;
			}
		}
		free(symbol_list);
		free(*commands);
		free(commands);
		free(*data);
		free(data);
		free(file_name_am);
		free(file_name_as);
		fclose(file_as);
		fclose(file_am);
		return 1;
	}
	rewind(file_am);/*sets the file position indicator to the beginning of the file file_am.*/
	if(second_pass(file_am,file_name_am,&ic,&dc,&sum_symbols,commands,data,&symbol_list,is_extern,
	is_entery))/*Call the first_pass function*/
	{
		fprintf(stderr,"Error %s\n",file_name);
		if(sum_symbols>0)
		{
			while(j<=sum_symbols)		
			{
				free(*(symbol_list+j));
				j++;
			}
		}
		free(symbol_list);
		free(*commands);
		free(commands);
		free(*data);
		free(data);
		free(file_name_am);
		free(file_name_as);
		fclose(file_as);
		fclose(file_am);
		return ALOCATION_ERROR;
	}
   	if(sum_symbols>0)
	{
		while(j<=sum_symbols)		
		{
			free(*(symbol_list+j));
			j++;
		}
	}
	free(symbol_list);
	free(*commands);
	free(commands);
	free(*data);
	free(data);
	free(file_name_am);
	free(file_name_as);
	fclose(file_as);
	fclose(file_am);
	fprintf(stdout,"We are now DONE the assembler on  - %s.as - file!\n\n",file_name);
	printf("*************************************\n\n");
	return 0;
}

