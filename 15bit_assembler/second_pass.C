#include <assembler.h>


/* Function will complete the first pass script. */
int second_pass(FILE *fp)
{
	char line[MAX_LINE_LENGTH]={0}, *token;
	int temp;
	unsigned int command_number;
	/* 1. Init IC =0. */ 
	IC=0;
	CL=0;
	/* 2. Read next line in source file. If there is no line left go to 9. */
	while(fgets(line, MAX_LINE_LENGTH,fp))
	{
		CL++;
		token=strtok(line," ");
		token=trim_white_chars(token);
		/* Handle comments. */
		if(token!=NULL && token[0]==';') continue;
		/* 3. If the first field in the line is lable-skip it. */
		temp=is_label(token,1);
		if(temp) //If token is label
		{
			token=strtok(NULL," ");
			token=trim_white_chars(token);
		}
		if(temp==-1)
		{/* If got there line is empty. */
			continue;
		}
		/* 4. If this is .data, .string or .extern go to 2. */
		if(strcmp(token,".data")==0 || strcmp(token,".extern")==0 || strcmp(token,".string")==0) continue;
		/* 5. If this is not .entry go to 7. */
		if(strcmp(token,".entry")==0)
		{
		/* 6. Add is_entry to every label in this line. If lable not found print error. */
			token=strtok(NULL," ");
			token=trim_white_chars(token);
			if(token==NULL)
			{
				fprintf(stderr,"ERROR in line %d: No operands provided after \".entry\".\n",CL);
				Error_Flag=1;
				continue; //Unclear what to do, proceed to next line.
			}
			else 
			{
				token=strtok(token,",");
				token=trim_white_chars(token);
				if(token==NULL)
				{
					fprintf(stderr,"ERROR in line %d: white characters were provided as operand to \".entry\".\n",CL);
					Error_Flag=1;
				}
				while (token!=NULL)
				{
					if(is_label(token,0))
					{
						temp=set_is_entry(token);
						if(!temp)
						{
							fprintf(stderr,"ERROR in line %d: lable \"%s\" was not declared.\n",CL,token);
							Error_Flag=1;
						}
					}
					token=strtok(NULL,",");
				}
				continue; // Go to 2.
			}
		}
		/* 7. Finish encoding label operands.  */ //if got there it should be command name
		command_number=find_command(token);//assuming it is valid since we are in second pass
		token=strtok(NULL,"");//get operands
		token=trim_white_chars(token);
		temp=fp_process_instruction(command_number, token,0);//actual words ammount returned from there
		if(temp==-1) continue;
		/* 8. Update instruction counter and go to 2. */
		IC=IC+temp; 
	}
	/* 9. Source file read comletely. If there were errors stop there. */
	if(Error_Flag) return 0;
	else return 1;
}
