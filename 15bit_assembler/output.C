#include <assembler.h>

/* Placeholders */
char * get_filename(char filename[], int flag);
void create_object_file(FILE *fp);
void create_extern_file(FILE *fp);
void create_entry_file(FILE *fp);
char * to_base_8(WORD * word);
char * get_address(unsigned int num);

/*
Function will create and write:
object, external and entry files.
*/
int build_files(char filename[])
{
	FILE *fp;
	char *new_filename=NULL;
	Error_Flag=0;//reset error flag just in case
	/* 1. Create objects file. */
	new_filename=get_filename(filename,1);
	if ((fp = fopen(new_filename, "w"))==NULL)//create file for obj writing 
		{
			fprintf(stderr,"ERROR: Cannot create file %s.\n",new_filename);
			Error_Flag=1;
		}
	create_object_file(fp);
	if(fclose(fp)==EOF)//close file
		{
			fprintf(stderr,"ERROR: Cannot close file %s.\n",new_filename);
			Error_Flag=1;
		}
		printf("File \"%s\" created.\n",new_filename);
	if(have_extern)//if flag of extern labels enabled
	{
		free(new_filename);
		new_filename=get_filename(filename,3);
		if ((fp = fopen(new_filename, "w"))==NULL)//create file for obj writing 
			{
				fprintf(stderr,"ERROR: Cannot create file %s.\n",new_filename);
				Error_Flag=1;
			}
			create_extern_file(fp);
		if(fclose(fp)==EOF)//close file
		{
			fprintf(stderr,"ERROR: Cannot close file %s.\n",new_filename);
			Error_Flag=1;
		}
		printf("File \"%s\" created.\n",new_filename);
	}
	if(have_entry)//if entry labels flag enabled
	{
		free(new_filename);
		new_filename=get_filename(filename,2);
		if ((fp = fopen(new_filename, "w"))==NULL)//create file for obj writing 
			{
				fprintf(stderr,"ERROR: Cannot create file %s.\n",new_filename);
				Error_Flag=1;
			}
			create_entry_file(fp);
		if(fclose(fp)==EOF)//close file
		{
			fprintf(stderr,"ERROR: Cannot close file %s.\n",new_filename);
			Error_Flag=1;
		}
		printf("File \"%s\" created.\n",new_filename);
	}
	free(new_filename);
	return Error_Flag;
}
/* Help functions */

/* Function to create object file. */
void create_object_file(FILE *fp)
{
	unsigned int i;
	Data_Node *pointer=NULL;
	char *sentence=NULL, *sentence2=NULL;
	fprintf(fp,"  %d %d\n",IC,DC);//print instructions counter and data counter
	for(i=0; i<IC; i++)
	{
		sentence=get_address(i+100);
		sentence2=to_base_8(code[i]);
		fprintf(fp,"%s %s\n",sentence,sentence2);
	}
	pointer=data_head;
	while(pointer!=NULL)
	{
		sentence=get_address(pointer->mem_num);
		sentence2=to_base_8(pointer->data);
		fprintf(fp,"%s %s\n",sentence,sentence2);
		pointer=pointer->next;
	}	
}

/*Function to create extern file. */
void create_extern_file(FILE *fp)
{
	Extern_Line_Node *pointer=NULL;
	pointer=externs_head;
	while(pointer!=NULL)
	{
		fprintf(fp,"%s %d\n",pointer->line, pointer->address);
		pointer=pointer->next;
	}
}

/* Function to create entry file. */
void create_entry_file(FILE *fp)
{
	Label_Node *pointer=NULL;
	pointer=labels_head;
	while(pointer!=NULL)
	{
		if(pointer->is_entry)
			fprintf(fp,"%s %u\n",pointer->name, pointer->address);
		pointer=pointer->next;
	}
}

/* Help function to return string with base 8 coding of given WORD. */
char * to_base_8(WORD * word)
{
	char *pointer=NULL;
	int decimal = 0, i = 0;
	pointer=(char*) calloc(6,sizeof(char));
	for(i=14; i>=0;i--)
	{
		if(i!=14 && (i+1)%3==0)
		{
			pointer[4-((i+1)/3)]=decimal+48;
			decimal=0;
		}
		decimal+=getBit(word,i)*pow(2,i%3);
	}
	pointer[4]=decimal+48; 
    return pointer;
}

/* Help function to return four-chars string with given decimal number. */
char * get_address(unsigned int num)
{
	int i;
	char * pointer=NULL;
	pointer=(char*) calloc(5,sizeof(char));
	for(i=0; i<4; i++)
	{
		pointer[3-i]=(char)(num%10+'0');
		num=num/10;
	}
	pointer[4]='\0';
	return pointer;
}

/* Function to return filename wit extention according to flag:
		1 - *.o b (object)
		2 - *.ent (entry labels)
		3 - *.ext (external label)
*/
char * get_filename(char filename[], int flag)
{
	char *pointer=NULL;
	int i, temp;
	for(i=strlen(filename)-1;i>=0;i--)
	{//Calculate length of extention
		if(filename[i]=='.')
		{
			break;
		}
	}/* End of for */
	if(i<=0)//case no extention found
		pointer=strdup(filename);
	else
	{
		//allocate space for filename
		if(flag==1)
			pointer=(char*) calloc(i+3,sizeof(char));
		else
			pointer=(char*) calloc(i+4,sizeof(char));
		//copy first i chars to pointer
		temp=i;
		while(temp>=0)
		{
			pointer[temp]=filename[temp];
			temp--;
		}
		if(flag==1)//if *.ob
		{
			pointer[i+1]='o';
			pointer[i+2]='b';
			pointer[i+3]='\0';
		}
		else//if *.ext *.ent
		{
			pointer[i+1]='e';
			if(flag==2)
				pointer[i+2]='n';
			else
				pointer[i+2]='x';
			pointer[i+3]='t';
			pointer[i+4]='\0';
		}
	}
	return pointer;	
}
