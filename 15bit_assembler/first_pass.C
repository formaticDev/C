#include <assembler.h>

int first_pass(FILE *fp)
{
	/* 0. Declare variables needed for run. */
	char line[MAX_LINE_LENGTH]={0}, *label=NULL, *token=NULL;
	int label_definition_flag,temp, command_number=99;
	/* 1. Initiate CL, DC and IC to 0. */
	DC=0;
	IC=0;
	CL=0;
	/* 2. Read next line in file */
	while(fgets(line, MAX_LINE_LENGTH,fp)){
		CL++;
		label_definition_flag=0;
		free(label);
		label=NULL;
		token=strtok(line," ");
		token=trim_white_chars(token);
		if(token==NULL) continue; //case empty line
		/* Handle comments. */
		if(token[0]==';') continue;
		/* 3. Is the first field in line a label? if not go to 5 */
		temp=is_label(token,1);
		if(temp==1)
		{
			/* 4. Enable label definition flag */
			label_definition_flag=1;
			token[strlen(token)-1]='\0';/* Remove semicolon from the label name. */
			label=(char*)malloc((strlen(token)+1)*sizeof(char));
			strcpy(label,token);/* Save label for further use. */
			label[strlen(token)]='\0';
			token=strtok(NULL," ");
			token=trim_white_chars(token);
		}
		if(token==NULL)
		{
			fprintf(stderr,"ERROR in line %d: syntax error, no arguments provided after label name.\n",CL);
			Error_Flag=1;
			continue;
		}
		/* 5. Is this instruction to add new data? (.data or .string) If not go to 8. */
		temp=strcmp(token,".data");
		if (temp==0 || strcmp(token,".string")==0)
		{
			/* 6. If it is label definition insert label to data table with .data property. Set value to DC. */
			if(temp==0)/* Token is data */
			{
				token=strtok(NULL,"");
				add_label(label, token, ".data");		/* step 7 inside, but continue there */
				continue;/* Go to step 2. */
			}
			else /* Token is string */
			{
				token=strtok(NULL,"");
				add_label(label,token,".string"); 		/* step 7 inside, but continue there */
				continue; /* Go to step 2. */
			}
		}/* End of data\string insertion */
		/* 8. Is this .extern or .entry? If not go to 11 */
		if(strcmp(token,".extern")==0) /* If extern */
		{
			token=strtok(NULL,"");
			add_label(label,token,".extern"); /* 10. Insert operands to data table with no values but with property .external and go to 2. */
			continue; /* Go to step 2. */
		}
		if(strcmp(token,".entry")==0) /* If entry */
		{
			continue; /* 9. Entry is relevant only in second pass, go to 2. */
		}
		/* 11. If got there - line is instruction or illegal. If label_definition_flag on add label to data table with .code property. */
		if(label_definition_flag)
		{
			add_label(label,token,".code");
			//token=strtok(NULL," ");//move to instruction
		}
		/* 12. Find instruction in instructions list, if not found display error. */
		command_number=find_command(token);
		if(command_number<0)
		{
			fprintf(stderr,"ERROR in line %d: syntax error, wrong command name.\n",CL);
			Error_Flag=1;
			continue; //Unclear what to do, proceed to next line.
		}
		/* 13. Analyze operands and determinate how many words need to reserve in memory.*/
		token=strtok(NULL,"");
		token=trim_white_chars(token);
		if(token==NULL && command_number<14 && command_number>-1)
		{
			fprintf(stderr,"ERROR in line %d: no operands provided for command %s.\n",CL,commands[command_number]);
			Error_Flag=1;
			continue;//Process next line since nothing to do with this line.
		}
		temp=count_words(token);
		if((temp>2) || (command_number>4 && temp>1) || (command_number>13 && temp!=0))
		{
			fprintf(stderr,"ERROR in line %d: to many operands after instruction.\n",CL);
			Error_Flag=1;
			continue; //Unclear what to do, proceed to next line.
		}
		if((command_number<5 && temp<2) || (command_number<14&&command_number>4&&temp<1))
		{
			fprintf(stderr,"ERROR in line %d: more operands expected for the instruction.\n",CL);
			Error_Flag=1;
			continue; //Unclear what to do, proceed to next line.
		}
		temp=fp_process_instruction(command_number, token,1);//actual words ammount returned from there
		if(temp==-1) continue;
		IC=IC+temp; //15. update instructions counter and go to 2.
	}
	/* 16. If there are errors stop there. */
	if (Error_Flag!=0) return -1;
	/* 17. Update data labels addresses, add IC+100. */
	update_data_labels();
	/* 17.1 Update data adresses with IC+100 */
	update_data_addresses();
	/* 18. Start second pass. */
	return 0;
}
