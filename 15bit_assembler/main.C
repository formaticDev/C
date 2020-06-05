#include <assembler.h>
/*Definition of global variables. */
unsigned int IC; //instructions counter
unsigned int DC; //data counter
unsigned int CL;//current line counter
Data_Node *data_head=NULL;//data list head
WORD* code[MAX_CODE_LENGTH];//code array
Label_Node *labels_head=NULL;//labels list head
Extern_Line_Node *externs_head=NULL;//list of lines in output *.ext
unsigned int Error_Flag=0; /* Flag is being enabled in case there are erorrs to prevent creation of the files. */
const char *commands[16]={
    "mov",
    "cmp",
    "add",
    "sub",
    "lea",
    "clr",
    "not",
    "inc",
    "dec",
    "jmp",
    "bne",
    "red",
    "prn",
    "jsr",
    "rts",
    "stop"
};//array of commands

/* Flags for output function. */
unsigned int have_extern;//extern labels were declared
unsigned int have_entry;//entry labels were declared

/*Functions to reset variables before each file reading.*/
void resetData();
void resetLabels();
void resetCode();
void resetExterns();

int main(int argc, char** argv) {
	FILE *fp;
	int currentArg=0, temp=0;
	if(argc<2) puts("No arguments provided, exiting...\n");
	for (currentArg=1;currentArg<(unsigned)argc;currentArg++)
	{
		/* Reset variables for next run. */
		if(currentArg>1)
		{
			resetData();
			resetLabels();
			resetCode();
			resetExterns();
		}
		Error_Flag=0;
		/* Set output flags. */
		have_extern=0;
		have_entry=0;
		/* Starring to read files from arguments. */
		/* First run */
		if ((fp = fopen(argv[currentArg], "rt"))==NULL) 
		{
			fprintf(stderr,"ERROR: Cannot open file %s.\n",argv[currentArg]);
			continue;				
		}
		printf("\nProceeding with file \"%s\":\n",argv[currentArg]);
		first_pass(fp);
		if(fclose(fp)==EOF)
		{
			fprintf(stderr,"ERROR: Cannot close file %s.\n",argv[currentArg]);
			continue;
		}
		/* Second run */
		if ((fp = fopen(argv[currentArg], "rt"))==NULL)
		{
			fprintf(stderr,"ERROR: Cannot open file %s.\n",argv[currentArg]);
			continue;				
		}
		if(Error_Flag==0) 
		{
			/* uncomment for debug prints
			print_bin_code(IC+3);
			print_bin_data();
			print_labels();*/
			temp=second_pass(fp);
		}
		if(Error_Flag==0 && temp)
		{
		/* Uncomment for debug prints
			print_bin_code(IC+3);
			print_bin_data();
			print_labels();	*/
		/* 10. Build output files. */
			build_files(argv[currentArg]);
		}
		if(fclose(fp)==EOF)
		{
			fprintf(stderr,"ERROR: Cannot close file %s.\n",argv[currentArg]);
			continue;
		}
		if(Error_Flag==0) printf("Success!\n");
		else printf("Error in proceeding file \"%s\".\n",argv[currentArg]);
	}/*end of for*/
}

void resetData()
{
	Data_Node *pointer=NULL;
		while(data_head!=NULL)
		{
			pointer=data_head;
			free(data_head->data);
			data_head=data_head->next;
			free(pointer);
		}
	if(data_head!=NULL) free(data_head);
	data_head=NULL;
}

void resetLabels()
{
	Label_Node *pointer=NULL;
		while(labels_head!=NULL)
		{
			pointer=labels_head;
			free(labels_head->name);
			labels_head=labels_head->next;
			free(pointer);
		}
	if(labels_head!=NULL) free(labels_head);
	labels_head=NULL;
}

void resetCode()
{
	int i;
	for(i=0;i<MAX_CODE_LENGTH;i++)
	{
		if(code[i]!=NULL)
		{
			free(code[i]);
		}
		code[i]=NULL;
	}
}

void resetExterns()
{
	 Extern_Line_Node *pointer=NULL;
	 while(externs_head!=NULL)
	 {
	 	pointer=externs_head;
	 	free(pointer->line);
	 	externs_head=externs_head->next;
	 	free(pointer);
	 }
	 if(externs_head!=NULL) free(externs_head);
	 externs_head=NULL;
}
