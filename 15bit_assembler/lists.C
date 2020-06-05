#include <assembler.h>


/* List operations and other data handling. */

/* Last node in data list getter */
Data_Node* getLastDataNode()
{
	Data_Node *pointer;
	if(data_head==NULL) return data_head;
	pointer=data_head;
	while(pointer->next!=NULL)
	{
		pointer=pointer->next;
	}
	return pointer;
}

/* insert new data to data table */
void insert_data(WORD *word)
{
	Data_Node *pointer, *last_node;
	if(data_head==NULL)
	{
		data_head=(Data_Node*)calloc(1,sizeof(Data_Node));
		data_head->mem_num=DC;
		data_head->data=word;
		DC++;
	}
	else
	{
		pointer=(Data_Node*)calloc(1,sizeof(Data_Node));
		pointer->mem_num=DC;
		pointer->data=word;
		DC++;
		last_node=getLastDataNode();
		last_node->next=pointer;
	}
}

/* Last node in label list getter. */
Label_Node* getLastLabelNode()
{
	Label_Node *pointer;
	if(labels_head==NULL) return labels_head;
	pointer=labels_head;
	while(pointer->next!=NULL)
	{
		pointer=pointer->next;
	}
	return pointer;
}
/* Function will find given lable in list and enable is_entry flag. If lable will not be found 0 will be returned. */
unsigned int set_is_entry(char label[])
{
	Label_Node *pointer;
	pointer=labels_head;
	while(pointer!=NULL)
	{
		if(strcmp(pointer->name,label)==0)
		{
			pointer->is_entry=1;
			have_entry=1;//set flag of entry labels for output file creation
			return 1;
		}
		pointer=pointer->next;
	}
	return 0;
}

/* Function will return address of lable with name as given token or -1 in case not found and -2 in case lable is extern(since address isn't real). */
int get_label_address(char label[])
{
	Label_Node *pointer;
	pointer=labels_head;
	while(pointer!=NULL)
	{
		if(strcmp(pointer->name,label)==0)
		{
			if(pointer->is_external) return -2;
			return pointer->address;
		}
		pointer=pointer->next;
	}
	return -1;
}

/* Insert new label to labels list. */
void insert_label(char * label_name,unsigned int isInst, unsigned int isExt)
{
	Label_Node *pointer=NULL, *last_node=NULL;
	if(label_name==NULL) 
	{// case no lable name provided.
		fprintf(stderr,"ERROR in line %d: no label name provided when adding data.",CL);
		Error_Flag=1;
	}
	if(labels_head==NULL)//Case list wasn't started
	{
		labels_head=(Label_Node*)calloc(1,sizeof(Label_Node));
		labels_head->name=strdup(label_name);
		labels_head->is_instruction=isInst;
		labels_head->is_external=isExt;
		labels_head->is_entry=0;
		if(isExt)
		{
			labels_head->address=0;
			have_extern=1;//enable flag of extern labels
		}
		else if(isInst) 
			labels_head->address=IC+100;
		else labels_head->address=DC;
		labels_head->next=NULL;
	}
	else
	{
		last_node=labels_head;
		while(last_node!=NULL)
		{
			if(strcmp(label_name,last_node->name)==0) //Validate uniqness of lable name
			{
				fprintf(stderr,"ERROR in line %d: non-unique lable name.\n",CL);
				Error_Flag=1;
				return;
			}
			last_node=last_node->next; 
		}
		last_node=getLastLabelNode();//if got there lable is valid
		//construct new node
		pointer=(Label_Node*)calloc(1,sizeof(Label_Node));
		if(isExt)
		{
			pointer->address=0;
			have_extern=1;//enable flag of extern labels 
		}
		else if(isInst) pointer->address=IC+100;
		else pointer->address=DC;
		pointer->name=strdup(label_name);
		pointer->is_instruction=isInst;
		pointer->is_external=isExt;
		pointer->is_entry=0;
		last_node->next=pointer;//add new lable to list
	}
}

/* Function will update the data labels with IC+100 in adress field of each. */
void update_data_labels()
{
	Label_Node *pointer=NULL;
	pointer=labels_head;
	while(pointer!=NULL)
	{
		if(pointer->is_instruction==0 && pointer->is_external==0)
		{
			pointer->address=pointer->address+IC+100;
		}
		pointer=pointer->next;
	}
}

/* 17.1 Update data adresses with IC+100 */
void update_data_addresses()
{
	Data_Node *pointer=NULL;
	pointer=data_head;
	while(pointer!=NULL)
	{
		pointer->mem_num=pointer->mem_num+IC+100;
		pointer=pointer->next;
	}
}

/* Prepare new line to output file *.ext . */
void insert_externs_node(int address, char labelname[])
{
	Extern_Line_Node *new_node, *pointer;
	new_node=(Extern_Line_Node*)calloc(1, sizeof(Extern_Line_Node));
	new_node->line=strdup(labelname);	
	new_node->address=address;
	new_node->next=NULL;
	pointer=externs_head;
	if(externs_head==NULL)
	{
		externs_head=new_node;
	}
	else
	{
		while(pointer->next!=NULL)
		{
			pointer=pointer->next;
		}
		pointer->next=new_node;
	}
}
