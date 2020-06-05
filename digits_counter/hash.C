#include <stdio.h>
#include <stdlib.h>

#define SIZE 29

struct node{
	unsigned int counter;
	unsigned int argNum;
	struct  node *next;
};/* node in linked list of occurences counters in files */

typedef struct node Node;

struct hashTableItem{
	Node *list;
};
typedef struct hashTableItem hashItem;
/* Declairing global variables */
hashItem *hashArray[SIZE]; 
unsigned int currentArg;

/* placeholders for functions */
int readFile(FILE *fp,char fileName[]);
int increaseOccurenceOfNum(int num, int key);
int addToHash(int num,char filename[]);
void startArray();
void printArray(char** argv);

int main(int argc, char** argv) {
	FILE *fp;
	if(argc<2) puts("No arguments provided, exiting...\n");
	startArray();
	for (currentArg=1;currentArg<(unsigned)argc;currentArg++){
		if ((fp = fopen(argv[currentArg], "rt"))==NULL) {
			fprintf(stderr,"Cannot open file %s.\n",argv[currentArg]);
			exit(1);	
			}
		if(readFile(fp,argv[currentArg])!=0){
		fprintf(stderr,"Wrong format in file %s.\n",argv[currentArg]);
		exit(1);	
		}
	if(fclose(fp)==EOF){
		fprintf(stderr,"Cannot close file %s.\n",argv[currentArg]);
		exit(1);
		}
	}/*end of for*/
	printArray(argv); /* print the readen results */
}/* End of main(). */

/* Function prints global hash array. */
void printArray(char** argv){
Node * p;
int i;
for(i=0;i<SIZE;i++){
	if(&hashArray[i]->list!=(Node**)0x0 && hashArray[i]->list!=(Node*)0x0){
		p=hashArray[i]->list;
		if(p->counter==0) {/*printf("key %d counter %d file number %d \n",i,p->counter,p->argNum);*/continue;}
		else{
		printf("%d appears in file %s - %d time",i,argv[p->argNum],p->counter);
		if(p->counter>1) printf("s");
		}
		while(p->next!=NULL){
			p=p->next;
			printf(", file %s - %d time",argv[p->argNum],p->counter);
			if(p->counter>1) printf("s");
			}
		printf(".\n");
		}
	}
}/*end of printArray()*/

/* Function allocates memory for hash table array pointers */
void startArray(){
*hashArray=(hashItem*)malloc(SIZE*sizeof(hashItem));
}

/* Function reads decimal values in range [0...28] separated with white characters from file. 
Exit codes: 0-OK, 1-wrong format.*/
int readFile(FILE *fp, char fileName[]){
int c,i=0,test,whiteCharSkipped=0, numToAdd=0;
char number[3];
number[2]='\0';
while((c=fgetc(fp))!=EOF){
	while((c!=EOF)&&((c==' ')||(c=='\n')||(c=='\t'))){/*skip white characters and reset i*/
		c=fgetc(fp);
		whiteCharSkipped=1;
		}
	if(whiteCharSkipped==1){/*if there was white character read number from array*/
		if(i==1){
		if((number[0]-'0'>9)||(number[0]-'0'<0)) {printf("Wrong character in 1 digit num.\n");return 1;}/*case wrong character*/
		if(addToHash(number[0]-'0',fileName)==1){
			fprintf(stderr,"Error in \"add to hash\" function.\n");
			exit(1);
			}
		whiteCharSkipped=0;
		i=0;
		} 
		else 
		{
		test=sscanf(number,"%d",&numToAdd);
		if(test==EOF){
			fprintf(stderr,"Error in reading \"number\" array.\n");
			exit(1);
			}
		if((numToAdd>28)||(numToAdd<0)) return 1;/*case number is not in range*/
		if(addToHash(numToAdd,fileName)==1){
			fprintf(stderr,"Error in add to hash");
			exit(1);
			}
		whiteCharSkipped=0;
		i=0;
		}
	}
	/* read number to array*/
	if(i>1)return 1;
	if(whiteCharSkipped!=1 && i<2){
		number[i]=(char)c;
		i++;
		}
	}/* end of while */
return 0;
}

/* Function adds occurence of number to hash table. 
Exit codes: 0-OK, 1-error. */
int addToHash(int key, char fileName[]){
	Node *p, *newNode;
	if(hashArray[key]==NULL){/*case first entry for current key. */
	hashArray[key]=(hashItem*)malloc(sizeof(hashItem));
	hashArray[key]->list = (Node*)malloc(sizeof(Node));
	hashArray[key]->list->next=NULL;
	hashArray[key]->list->counter=1;
	hashArray[key]->list->argNum=currentArg;
	return 0;
	}
	p=hashArray[key]->list;
	/* printf("current arg %d\n",currentArg);*/
	while(p->next!=NULL){/* check if there is entry of current arg already */
	if(p->argNum==currentArg){
		p->counter++;
		/*printf("number %d for file %s: counter increased\n",key,fileName); */
		return 0;
		}
	else	p=p->next;
   	}
	/*case last entry */
	if(p->argNum==currentArg){
		p->counter++;
		/*printf("number %d for file %s: counter increased\n",key,fileName); */
		return 0;
		}
	/* if got there so no entry of current arg */
	newNode=(Node*)malloc(sizeof(Node));
	newNode->counter=1;
	newNode->argNum=currentArg;
	p->next=newNode;
	return 0;
}
