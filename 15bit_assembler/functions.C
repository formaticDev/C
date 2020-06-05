#include <assembler.h>


/* String analysing help functions below. */

/* Function trims all leading and following white space characters and returnes the string. Returns NULL in case there are only white characters or string is empty. */
char* trim_white_chars(char token[])
{
	int i=0,j,temp=0;
	char *pointer=NULL;
	if(token==NULL) return NULL;
	if(strlen(token)==0) return token;
	j=strlen(token)-1;
	while((token[i]==' ' || token[i]=='\n' || token[i]=='\t')&&i<(int)strlen(token)) i++;
	while((token[j]==' ' || token[j]=='\n' || token[j]=='\t')&&j>-1) j--;
	if(j<i) return NULL;
	pointer=(char*)calloc((j-i+2),sizeof(char));
	for(temp=0;temp<=(j-i);temp++)
	{
		pointer[temp]=token[temp+i];
	}
	pointer[j-i+1]='\0';
	return pointer;
}

/* Function determinates if the given token can be label and validates it. Returns 1 if token, 0 if not and -1 if there is an error. */
unsigned int is_label(char *token, unsigned int should_have_semicolon)
{
	unsigned int i=0, is_long=0, illegal_first_char=0, illegal_char=0, is_register=0,have_semicolon=0;
	if(token == NULL) return -1; /* Got null token. */
	token=trim_white_chars(token);
	if( token==NULL || strcmp(token,"") == 0) return -1;/* Got token with white chars only or empty token. */
	if(find_command(token)!=-1) return 0; /* Case line starts with instruction. */
	if(strcmp(token,".data")==0 || strcmp(token,".extern")==0 || strcmp(token,".entry")==0 || strcmp(token,".string")==0)
	return 0; /* Case line starts with data command. */
	have_semicolon=token[strlen(token)-1]==':';/* if last symbol is ':' the token could be label */
	if(!have_semicolon && should_have_semicolon) 
	{
	    fprintf(stderr,"ERROR in line %d: syntax error, lable name in the beggining of line don't have semicolon.\n",CL);
	    Error_Flag=1;
	}
	/* Case lable name is as register name. */
	if((strlen(token)==2 || strlen(token)==3) && token[0]=='r' && (token[1]-'0')<8 && (token[1]-'0')>=0) 
	{
		fprintf(stderr,"ERROR in line %d: r0-r7 names are reserved for registers.\n",CL);
		Error_Flag=1;
		is_register=1;
	}
	if(!isalpha(token[i])) illegal_first_char=1; /* Not started with letter */
	while(token[i]!='\0')/* move to the end of the token */
	{
		if (!(isalpha(token[i])||isdigit(token[i]))&&i!=(strlen(token)-1))
		{
			illegal_char=1;
		}
		i++;
	}
	if(i!=0 && should_have_semicolon && i-1>MAX_LABEL_LENGTH) is_long=1; /* Label is longer then possible. */
	if(i!=0 && !should_have_semicolon&&i>MAX_LABEL_LENGTH) is_long=1; /* Label is longer then possible. */
	if(is_long)
	{
		fprintf(stderr,"ERROR in line %d: Illegal length of label, should be less or equal %d.\n",CL,MAX_LABEL_LENGTH);
		Error_Flag=1;		
	}
	if(illegal_first_char)
	{
		fprintf(stderr,"ERROR in line %d: Illegal first character in label, should be letter.\n",CL);
		Error_Flag=1;
	}
	if(illegal_char)
	{
		fprintf(stderr,"ERROR in line %d: Illegal character in label.\n",CL);
		Error_Flag=1;
	}
	return (!is_long && !illegal_first_char && !illegal_char && !is_register);
}
/* Function finds command in commands array and returns command number if found or -1 if not. */
int find_command(char *token)
{
	unsigned int i,j,flag=0;
	for(i=0;i<16;i++)
	{
		if(strlen(commands[i])==strlen(token))
		{
			for(j=0;j<strlen(commands[i]);j++)
			{
				//Check every char in both upper and regular cases.
				if(commands[i][j]!=tolower(token[j])) flag=1;
			}
			if (flag==0) return i; 
			else flag=0;
		}
	}
	return-1;
}

/* Function will add new lable to labels list according to provided parameters */
void add_label(char label[], char token[], const char type[])
{
	unsigned int i,number=0;
	WORD *new_word;
	char *pointer=NULL;
	if(strcmp(type,".data")==0) /* Handle new data with the lable */
	{
		if(label!=NULL) 
			insert_label(label,0,0);
		pointer=strtok(token,",");
		pointer=trim_white_chars(pointer);
		if(pointer==NULL)
		{
			fprintf(stderr,"ERROR in line %d: syntax error, unexpected format of .data operands.\n",CL);
			Error_Flag=1;
		}
		while(pointer!=NULL)
		{
			if((pointer[0]=='+') || (pointer[0]=='-')) i=1;
			else i=0;
			number=0;
			while(i<strlen(pointer))
			{
				if(!isdigit(pointer[i]))
				{
					fprintf(stderr,"ERROR in line %d: non-digit character provided in arguments to .data lable.",CL);
					Error_Flag=1;
					break;
				}
				number=(number*10)+((unsigned int)(pointer[i]-'0'));
				i++;
			}
			if(pointer[0]=='-') number=-1*number;
			new_word=init_word();
			set_to_binary(new_word,number);
			insert_data(new_word);
			pointer=strtok(NULL,",");
			pointer=trim_white_chars(pointer);
		}
	}/* End of data handling. */
	if(strcmp(type,".string")==0)/* Handle .string. */
	{
		insert_label(label,0,0);
		pointer=trim_white_chars(token);
		if(pointer[0]!='"' || pointer[strlen(pointer)-1]!='"')
		{
			fprintf(stderr,"ERROR in line %d: syntax error, string provided in unexpected format.\n",CL);
			Error_Flag=1;
		}
		for(i=1; i<(strlen(pointer)-1);i++)
		{
			new_word=init_word();
			set_to_binary(new_word,(unsigned int)pointer[i]);
			insert_data(new_word);
		}
		new_word=init_word();
		insert_data(new_word);//insert \0 finishing string
	}/* End of .string handling. */
	if(strcmp(type,".extern")==0)/* Extern lable handling. */
	{
		pointer=strtok(token,",");
		pointer=trim_white_chars(pointer);
		while(pointer!=NULL)
		{
			if(is_label(pointer,0)) insert_label(pointer,0,1);
			else
			{
				fprintf(stderr,"ERROR in line %d: illegal lable name provided as external lable.\n",CL);
				Error_Flag=1;
			}
			pointer=strtok(NULL,",");
			pointer=trim_white_chars(pointer);
		}
	}/* End of extern handling. */
	if(strcmp(type,".code")==0) /* Handle code lable. */
	{
		insert_label(label,1,0);
	}
}

/* Function will count and return operands number by ',' delimiter. */
unsigned int count_words(char token[])
{
	unsigned int counter=0;
	char *pointer=NULL;
	if(token==NULL) return counter;
	pointer=(char*)calloc(strlen(token),sizeof(char));
	strcpy(pointer,token);
    pointer[strlen(token)]='\0';
	pointer=strtok(pointer,",");
	pointer=trim_white_chars(pointer);
	while(pointer!=NULL)
	{
		counter++;
		pointer=strtok(NULL,",");
	}
	return counter;
}

/* Function to process operands passed as string and take actions accordingly. Returns number of additional words in instructions tabel.
Function will process only non-lable operands for first pass. */
int fp_process_instruction(unsigned int command_number,char token[],unsigned int is_first_pass)
{
	char *pointer=NULL;//For strtok
	WORD *instruction=NULL, *operand1=NULL, *operand2=NULL;//max 3 words to insert
	unsigned int is_register=0, words_counter=1, is_source=1;
	int temp, i;
	if(is_first_pass)
	{
		instruction=init_word();
		set_opcode(instruction,command_number);
		setBit(instruction,2);//Enable 'A' bit for instruction word
	}
	pointer=strtok(token,",");//divide operands by delimiter
	if((command_number==14 || command_number==15) && pointer!=NULL)
	{
		fprintf(stderr,"ERROR in line %d: redundant operand provided with command %s.\n",CL,commands[command_number]);
		Error_Flag=1;
		return -1;
	}
	// Handle case opcode is 14 or 15 (rts, stop)
	if(command_number==14 || command_number==15)
	{
		if(is_first_pass) code[IC]=instruction;
		return words_counter;//Only 1 word required to store rts or stop
	}
	if(pointer==NULL)
	{
		fprintf(stderr,"ERROR in line %d: white characters or empty token passed as operand to instruction %s.\n",CL,commands[command_number]);
		Error_Flag=1;
		return -1;
	}
	if(command_number>4) is_source=0;//expecting target operand in this case since no source operand allowed
	if(pointer[0]=='#')//case LBA 0
	{
		if(is_first_pass)//in second pass all checks are done and operands from lba 0 encoded, so just increment words counter
		{ 
			if(command_number==4)
			{
				fprintf(stderr,"ERROR in line %d: command \"%s\" should have lable name as source operand.\n",CL,commands[4]);
				Error_Flag=1;
				return -1;
			}
			if(!is_source && command_number!=1 && command_number != 12)
			{
				fprintf(stderr,"ERROR in line %d: command \"%s\" can't have immediate number as target operand.\n",CL,commands[command_number]);
				Error_Flag=1;
				return -1;
			}
			temp=0;
			i=1;
			if(pointer[1]=='-' || pointer[1]=='+') i=2;
			while(i<(int)strlen(pointer))//parse the number from string
			{
				temp=temp*10;//move to next digit
				if(isdigit(pointer[i]))
				{
					temp+=pointer[i]-'0';//add current digit
				}
				else 
				{
					fprintf(stderr,"ERROR in line %d: non-digit character passed in immediate number operand. \n",CL);
					Error_Flag=1;
					return -1;
				}
				i++;
			}
			if(pointer[1]=='-') temp=temp*-1;
			if(temp > 1024 || temp < -1024) 
			{
				fprintf(stderr,"ERROR in line %d: immediate number passed as operand is out of range(-1024 to 1024).\n",CL);
				Error_Flag=1;
				return -1;
			}
			operand1=init_word();		
			set_to_binary(operand1,temp);
			operand1->word=operand1->word << 3;//shift 3 left to insert ARE
			setBit(operand1,2);//Enable 'A' bit for immediate number word
			if(is_source)
			{
				setBit(instruction,7);//enable source operand zero LBA
			}
			else setBit(instruction,3);//enable target operand zero LBA
		}
		words_counter++;
	}
	else if(pointer[0]=='*')//case LBA 2
	{
		if(is_first_pass)//in second pass all checks are done and operands from lba 2 encoded, so just increment words counter
		{
			if(command_number==4)
			{
				fprintf(stderr,"ERROR in line %d: command \"%s\" should have lable name as source operand.\n",CL,commands[4]);
				Error_Flag=1;
				return -1;
			}
			if(strlen(pointer)>3 || pointer[1]!='r' || pointer[2]>'7' || pointer[2]<'0')
			{
				fprintf(stderr,"ERROR in line %d: '*' char in begginning of operand should be followed with register name.\n",CL);
				Error_Flag=1;
				return -1;
			}
			temp=pointer[2]-'0';
			operand1=init_word();
			set_to_binary(operand1,temp);
			if(is_source)
			{
				setBit(instruction,9);//enable source operand LBA 2
				operand1->word=operand1->word << 6;//shift 6 left to set number of source register
			}
			else 
			{
				setBit(instruction,5);//enable target operand LBA 2
				operand1->word=operand1->word << 3;//shift 3 left to set number of target register
			}
			setBit(operand1,2);//Enable 'A' bit for register word
		}
		is_register=1;//enable register flag to write next register operand to same word
		words_counter++;
	}
	else if(strlen(pointer)==2 && pointer[0]=='r' && (pointer[1]<='7' && pointer[1]>='0')) //case LBA 3
	{
		if(is_first_pass)//in second pass all checks are done and operands from lba 2 encoded, so just increment words counter
		{
			if(is_source && command_number>3)
			{
				fprintf(stderr,"ERROR in line %d: command \"%s\" can't have register as source operand.\n",CL,commands[command_number]);
				Error_Flag=1;
				return -1;
			}
			if(!is_source && (command_number==9 || command_number==10 || command_number==13))
			{
				fprintf(stderr,"ERROR in line %d: command \"%s\" can't have register as target operand.\n",CL,commands[command_number]);
				Error_Flag=1;
				return -1;
			}
			temp=pointer[1]-'0';
			operand1=init_word();
			set_to_binary(operand1,temp);
			if(is_source)
			{
				setBit(instruction,10);//enable source operand LBA 3
				operand1->word=operand1->word << 6;//shift 6 left to set number of source register
			}
			else 
			{
				setBit(instruction,6);//enable target operand LBA 3
				operand1->word=operand1->word << 3;//shift 3 left to set number of target register
			}
			setBit(operand1,2);//Enable 'A' bit for register word
		}
		is_register=1;//enable register flag to write next register operand to same word
		words_counter++;
	}
	else //if got there operand is from LBA 1
	{
		if(!is_label(pointer,0))//validate LBA 1 and continue if valid.
		{
			fprintf(stderr,"ERROR in line %d: illegal first operand provided to instruction \"%s\".\n",CL,commands[command_number]);
			Error_Flag=1;
			return -1;
		}
		else 
		{
			if(is_first_pass==0)//Encode lable operand in case of second pass
			{
				operand1=init_word();
				temp=get_label_address(pointer);
				if(temp==-2) 
				{
					setBit(operand1,0);
					insert_externs_node(IC+101,pointer);
				}
				else if(temp>=0)
				{
					if(temp<4097) set_to_binary(operand1,temp);
					else 
					{
						fprintf(stderr,"ERROR in line %d: lable address overflow, lable %s have address %d(>4097).\n",CL,pointer,temp);
						Error_Flag=1;
						return -1;
					}
					operand1->word=operand1->word << 3;
					setBit(operand1,1);					
				}
				else if(temp == -1)
				{
					fprintf(stderr,"ERROR in line %d: label %s not found in second pass!.\n",CL,pointer);
					Error_Flag=1;
					return -1;
				}
			} 
			else if(is_source)
			{
				setBit(instruction,8);//Set source operand LBA 1
			}
			else
			{
				setBit(instruction,4);//Set target operand LBA 1
			}
			words_counter++;
		}
	}
	pointer=strtok(NULL,",");//move to next operand
	pointer=trim_white_chars(pointer);
	if(is_source && pointer==NULL)
	{
		fprintf(stderr,"ERROR in line %d: target operand not provided.\n",CL);
		Error_Flag=1;
		return -1;
	}
	if(!is_source && pointer!=NULL)
	{
		fprintf(stderr,"ERROR in line %d: redundant operand provided with command %s.\n",CL,commands[command_number]);
		Error_Flag=1;
		return -1;
	}
	if(!is_source) //insert words to code table and return ammount
	{
		if(is_first_pass) 
			code[IC]=instruction;//no need to change instruction word in second pass
		if(operand1!=NULL) //case in first pass operand was label and wasn't encoded at this point
			code[IC+1]=operand1;
		return words_counter; 
	}
//**********************************************************If got there command_number is under 5 and this is target operand
	is_source=0;//setting flag of target operand
	if(pointer[0]=='#')//case LBA 0 
	{
		if(is_first_pass)//in second pass all checks are done and operands from lba 1 encoded, so just increment words counter
		{
			if(!is_source && command_number!=1)
			{
				fprintf(stderr,"ERROR in line %d: command \"%s\" can't have immediate number as target operand.\n",CL,commands[command_number]);
				Error_Flag=1;
				return -1;
			}
			temp=0;
			i=1;
			if(pointer[1]=='-' || pointer[1]=='+') i=2;
			while(i<(int)strlen(pointer))//parse the number from string
			{
				temp=temp*10;//move to next digit
				if(isdigit(pointer[i]))
				{
					temp+=pointer[i]-'0';//add current digit
				}
				else 
				{
					fprintf(stderr,"ERROR in line %d: non-digit character passed in immediate number operand. \n",CL);
					Error_Flag=1;
					return -1;
				}
				i++;
			}
			if(pointer[1]=='-') temp=temp*-1;
			if(temp>1024||temp<-1024) 
			{
				fprintf(stderr,"ERROR in line %d: immediate number passed as operand is out of range(-1024 to 1024).\n",CL);
				Error_Flag=1;
				return -1;
			}
			operand2=init_word();
			set_to_binary(operand2,temp);
			operand2->word=operand2->word << 3;//shift 3 left to insert ARE
			setBit(operand2,2);//Enable 'A' bit for immediate number word
			if(is_source)
			{
				setBit(instruction,7);//enable source operand zero LBA
			}
			else setBit(instruction,3);//enable target operand zero LBA
		}
		words_counter++;
	}
	else if(pointer[0]=='*')//case LBA 2, if it is second register-operand write to operand 1 and don't increase words counter
	{
		if(is_first_pass)//in second pass all checks are done and operands from lba 2 encoded, so just increment words counter
		{
			if(strlen(pointer)>3 || pointer[1]!='r' || pointer[2]>'7' || pointer[2]<'0')
			{
				fprintf(stderr,"ERROR in line %d: '*' char in begginning of operand should be followed with register name.\n",CL);
				Error_Flag=1;
				return -1;
			}
			if(!is_register)//case single register operand
			{
				temp=pointer[2]-'0';
				operand2=init_word();
				set_to_binary(operand2,temp);
				operand2->word=operand2->word << 3;//shift 3 left to set number of target register
				setBit(operand2,2);//Enable 'A' bit for register word
				if(is_source)
				{
					setBit(instruction,9);//enable source operand LBA 2
				}
				else setBit(instruction,5);//enable target operand LBA 2
			}
			else
			{
				temp=pointer[2]-'0';
				operand2=init_word();
				set_to_binary(operand2,temp);
				operand2->word=operand2->word << 3;//shift 3 left to set number of target register
				operand1->word=operand1->word | operand2->word;//merge information from both words to write registers numbers to same word
				if(is_source)
				{
					setBit(instruction,9);//enable source operand LBA 2
				}
				else setBit(instruction,5);//enable target operand LBA 2
			}
		}
		if(!is_register) words_counter++;//case second operand was first register in line
	}
	else if(strlen(pointer)==2 && pointer[0]=='r' && (pointer[1]<='7' && pointer[1]>='0')) //case LBA 3
	{
		if(is_first_pass)//in second pass all checks are done and operands from lba  encoded, so just increment words counter
		{
			if(!is_source && (command_number==9 || command_number==10 || command_number==13))
			{
				fprintf(stderr,"ERROR in line %d: command \"%s\" can't have register as target operand.\n",CL,commands[command_number]);
				Error_Flag=1;
				return -1;
			}
			if(!is_register)
			{
				temp=pointer[1]-'0';
				operand2=init_word();		
				set_to_binary(operand2,temp);
				operand2->word=operand2->word << 3;//shift 3 left to set number of taget register
				if(is_source)
				{
					setBit(instruction,10);//enable source operand LBA 3
				}
				else setBit(instruction,6);//enable target operand LBA 3
				setBit(operand2,2);//Enable 'A' bit for register word
			}
			else
			{
				temp=pointer[1]-'0';
				operand2=init_word();		
				set_to_binary(operand2,temp);
				operand2->word=operand2->word << 3;//shift 3 left to set number of taget register
				operand1->word=operand1->word | operand2->word;//merge information from both words to write registers numbers to same word
				if(is_source)
				{
					setBit(instruction,10);//enable source operand LBA 3
				}
				else setBit(instruction,6);//enable target operand LBA 3
			}
		}
		if(!is_register) words_counter++;//case second operand was first register in line
	}
	else //if got there operand is from LBA 1
	{
		if(!is_label(pointer,0))//validate LBA 1 and continue if valid.
		{
			fprintf(stderr,"ERROR in line %d: illegal second operand provided to instruction \"%s\".\n",CL,commands[command_number]);
			Error_Flag=1;
			return -1;
		}
		else 
		{
			
			if(is_first_pass==0)//Encode lable operand in case of second pass
			{
				operand2=init_word();
				temp=get_label_address(pointer);
				if(temp==-2) 
				{
					setBit(operand2,0);
					insert_externs_node(IC+102,pointer);
				}
				else if(temp>=0)
				{
					if(temp<4097) set_to_binary(operand2,temp);
					else 
					{
						fprintf(stderr,"ERROR in line %d: lable address overflow, lable %s have address %d(>4097).\n",CL,pointer,temp);
						Error_Flag=1;
						return -1;
					}
					operand2->word=operand2->word << 3;
					setBit(operand2,1);					
				}
				else if(temp == -1)
				{
					fprintf(stderr,"ERROR in line %d: label %s not found in second pass!.\n",CL,pointer);
					Error_Flag=1;
					return -1;
				}
			}
			else if(is_source)
			{
				setBit(instruction,8);//Set source operand LBA 1
			}
			else
			{
				setBit(instruction,4);//Set target operand LBA 1
			}
			words_counter++;
		}
	}
//*************************************************If got there all operands should be encoded and validated. 
//In second pass pointers which were not encoded to labels will remain null and will not be added.
	if(is_first_pass) code[IC]=instruction;//no need to add instruction in second pass
	if(operand1!=NULL) code[IC+1]=operand1;
	if(operand2!=NULL && words_counter==3) code[IC+2]=operand2;
	return words_counter; 
}

void init_code_arr()//debug purposes
{
    unsigned int i;
    for (i=0;i<MAX_CODE_LENGTH;i++)
    {
        code[i]=NULL;
    }
}

void print_bin_code(unsigned int x)//to print all the code table
{
    unsigned int i;
    printf("Code array:\n");
    for (i=0;i<x;i++)
    {
        if(code[i]!=NULL)
        {
            printf("code[%d]: %s\n",(i+100),toString(code[i]));
        }
        else printf("code[%d]: NULL\n",(i+100));
    }
    printf("\n");    
}

/* Debug functions below. */

void init_array() // Initiate commads array, debug purposes
{
	WORD *word;
	unsigned int i;
	for(i=0;i<MAX_CODE_LENGTH;i++)
	{
		word=init_word();
		code[i]=word;
	}
}

void print_bin_data()//print data table
{
	Data_Node *pointer;
	pointer=data_head;
	printf("Data table:\n");
	while (pointer!=NULL)
	{
		printf("data[%d]: %s\n",pointer->mem_num,toString(pointer->data));
		pointer=pointer->next;
	}
	printf("\n");
}

void print_labels()//print labels table
{
	Label_Node *pointer;
	pointer=labels_head;
	printf("Labels table:\n");
	while(pointer!= NULL)
	{
		printf("Label name: %s, is_external: %d, is_entry: %d, is_instruction: %d, address: [%u].\n",pointer->name,pointer->is_external,pointer->is_entry,pointer->is_instruction,pointer->address);
		pointer=pointer->next;
	}
	printf("\n");
}

