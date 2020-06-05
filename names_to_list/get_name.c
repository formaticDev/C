#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define SIZE 30*21
#define MAX 21

struct node{
	char data[MAX];
	struct  node *next;
};

typedef struct node Node;
typedef struct node* NodePointer;
/* Declairing global variables */
char names[SIZE];
NodePointer names_list;

/* placeholders for functions */
int insertIntoLinkedList(char [], NodePointer *);
void displayLinkedList(NodePointer);
int compare(char[],char[]);
int random_in_range();
char* get_name();

int main()
{
char word[MAX];
int i=0,j=0;
NodePointer head = NULL;
/* Start random */
srand(time(0));
printf("\nEnter 30 names in a row with whitecharacters between them:\n");
gets(names);
printf("\nEntered values: %s.\n",names);
/* Split input to names and add to list. */
while (names[i]!=0)
	{
		if((names[i]==' ')||(names[i]=='\0'))
		{
			word[j+1]='\0';
			if(insertIntoLinkedList(word,&head)==1)
			{
				printf("Fatal error: double name found.\n");
				return 0;
			}
			j=0;
			i++;
		}
		else 
		{
			word[j]=names[i];
			i++;
			j++;
		}
	}/* end of while */
	names_list=head;
	printf("\nA random name from list: %s.\n",get_name());
	displayLinkedList(head);
	return 0;
}/* End of Main */

char* get_name()
{
	int num=random_in_range(), i=0;
	NodePointer current = names_list;
	while(i<num)
	{
		current=current->next;
		i++;
	}
	return current->data;
}

/* Function inserts new node with given char sequence to the end of list. */
int insertIntoLinkedList(char word2[], NodePointer *head2)
{
	/* Pointer to a new node to be inserted into list */
	NodePointer newNode = NULL;
	/* Pointer to the current node in the linked List */
	NodePointer current=*head2;
	/* Pointer to previous node */
	NodePointer previous=NULL;
	/* Create a node on the heap */
	newNode=malloc(sizeof(Node));
	if(NULL != newNode)
	{
		strcpy(newNode->data,word2);
		while(current!=NULL) 
		{
			if(compare(current->data,word2)==1)
			{
				return 1;
			}
			previous=current;
			current=current->next;
		}
		if (NULL == previous)
		{
			*head2=newNode;
		}
		else
		{
			previous->next=newNode;
		}
	}/*end of if*/	
return 0;
/* printf("\n"); */	
}/*end of insertToLinkedList*/

void displayLinkedList(NodePointer current)
{
/* If empty */
if(NULL == current)
{
	printf("Empty list, nothing to display.\n");
	return;
}
printf("\nEntered names: ");
while(NULL != current)
{
	printf("%s, ",current->data);
	current=current->next;
}/* end of while */
printf(".\n");
}/* end of displayLinkedList */

/* Function to compare char sequences according to question */
int compare (char string1[], char string2[])
{
int i, res=1;
	if(strlen(string1)!=strlen(string2)) return 0;
	while((string1[i]!='\0')&&(string2[i]!='\0'))
	{
		if (tolower(string1[i])!=tolower(string2[i]))
		{
			return 0;
		}
		i++;
	}
return res;
}/* end of compare */

/* function returns random number from 1 to 30 */
int random_in_range()
{
return (rand()%31)+1;
}
