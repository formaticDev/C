#include <stdio.h>
#define size 120
/*
Main function perform following:
1) Read number from user input to char array with get_input(char array[])
2) Call translate_dec_hex(int input[]) to print given number in base 2.
*/

/* Function get_input(char array[]) stores number given by user in char array.*/
void get_input(char array[])
{
	printf("\nEnter number to put in char array:\t");
	/*Get user input as string and store in char array.*/
	scanf("%s",array);
}

/* Function converts char array with number stored as char sequence in it to integer number, and returns integer.*/
int get_int_from_char_array(char input[])
{
	/*Declare variable "decimal" for integer number converted from char array.*/
	int decimal;
	sscanf(input, "%d", &decimal);/*Parse numbers from input to integer variable.*/
/* Uncomment following to print recieved values with indexes. Also declare "i" integer variable.*/
/*	printf("\nArray: \n");
	for(i=0;(input[i]>='0')&&(input[i]<='9');i++)
		{
			printf("Cell:%d, value:%c\n",i,input[i]);
			
		}*/
	return decimal;
}

/* Function converts given char array with number stored as
char sequence to integer with get_int_from_char_array(char array[]),
and calculates its base 2 equivalent.*/
void translate_dec_hex(char input[])
{
/*Declaring variable to store input as integer, 
and another variable which is copy of same number to calculate binary. */
	int number=get_int_from_char_array(input),number_copy=number,i;
/*Calculation of base 2 form. Write number mod 2 to bit and divide until become 0.*/
	for(i=0;i<size;i++)
	{
		if(number_copy!=0)
		{
			input[i]=number_copy%2+'0';/*Store calculated bit in original array*/
			number_copy=number_copy/2;/*Using copy of base 10 number to perform calculation.*/
		}
		else break;
	}
	printf("\nNumber %d in base 2: ",number);
	if(i==0) printf("%d",i); /*Case 0 provided as input.*/
	else /*Print calculated bits in right order, starting from last calculated index.*/
	{
		--i;
		while(i>=0)
		{
			printf("%c",input[i]);
			--i;
		}
	}
	printf("\n\n");
}

/*Main function.*/
int main()
{
	/*Declaring char array at max line size to store user input.*/
	char input[size];
	/*Calling function to get user input.*/
	get_input(input);
	/*Calling function to print given number in base 2.*/
	translate_dec_hex(input);
	return 0;
}


