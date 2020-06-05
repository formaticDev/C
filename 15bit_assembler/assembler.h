#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAX_LINE_LENGTH 256 /* Assuming max line length in assembly code is 256 */
#define MAX_LABEL_LENGTH 31 /* Max label length is 31 */
#define MAX_CODE_LENGTH 400 /* Assume max code cells ammount is 400, adjust when needed */

/*
MMN 14 programm -assembler.
Will translate assembler to machine code for 15 bit processor as described in mmn 14 2020a.
Verifies corectness of code and in case no errors found produces machine code file and extern\entry file if trere were extern\entry labels.
*/

/* Structures and types. */
struct bitwise{
    unsigned int word:15;
};
typedef struct bitwise WORD;
typedef struct DataNode
{
	unsigned int mem_num;
	WORD *data;
	DataNode *next;
} Data_Node;
typedef struct LabelNode
{
	unsigned int address;
	char *name;
	unsigned int is_instruction;
	unsigned int is_external;
	unsigned int is_entry;
	LabelNode *next;
} Label_Node;
typedef struct external_output_line
{
	char *line;
	int address;
	external_output_line *next;
} Extern_Line_Node;

/* Define global variables */
extern unsigned int IC; //instructions counter
extern unsigned int DC; //data counter
extern unsigned int CL;//current line counter
extern Data_Node *data_head;//data list head
extern WORD* code[MAX_CODE_LENGTH];//code array
extern Label_Node *labels_head;//labels list head
extern unsigned int Error_Flag; /* Flag is being enabled in case there are erorrs to prevent creation of the files. */
extern const char *commands[];//commands names array
extern unsigned int have_extern;//indicates if there is extern lables for output func
extern unsigned int have_entry;//indicates if there is entry lables for output func
extern Extern_Line_Node *externs_head;//list of lines in output *.ext

/* Functions placeholders, description of functions in the source files near the functions.*/
void setBit (WORD * a, unsigned int k);
unsigned int getBit (WORD * a, unsigned int k);
void unsetBit (WORD * a, unsigned int k);
WORD *init_word ();
void set_to_binary (WORD * word, int k);
int set_opcode (WORD *a, unsigned int k);
char *toString (WORD * a);
void insert_data(WORD *word);
Data_Node* getLastDataNode();
void insert_data(WORD *word);
Label_Node* getLastLabelNode();
void insert_label(char * label_name,unsigned int isInst, unsigned int isExt);
void insert_externs_node(int address, char labelname[]);
int get_label_address(char label[]);
void update_data_labels();
void update_data_addresses();
unsigned int set_is_entry(char label[]);
unsigned int is_label(char *token, unsigned int should_have_semicolon);
char* trim_white_chars(char token[]);
int find_command(char *token);
void add_label(char label[], char token[], const char type[]);
unsigned int count_words(char token[]);
int fp_process_instruction(unsigned int command_number,char token[],unsigned int is_first_pass);
void init_code_arr();
void print_bin_code(unsigned int x);
void print_bin_data();
void print_labels();
int build_files(char filename[]);
int first_pass(FILE *fp);
int second_pass(FILE *fp);
