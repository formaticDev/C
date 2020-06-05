#include <stdio.h>
#define size 120
/*
Programm fetches char sequence in format "%c %s" from stdin.
Main function prints all subsequences from the string %s starting
with symbol %c and their ammount with function suffix(char str[], char c).
*/

/* 
Function suffix prints all substrings from char sequence
str[] which startes with char c.
Returns number of substrings found in string.
*/
int suffix(char str[], char c)
{
	int i=0,j=0,counter=0;
	while (str[i]!=0)/*Search for given char in given char sequence.*/
	{
		if(str[i]==c)/*If char found print all rest of sequence and increment counter.*/
		{
			counter++;
			j=i;
			printf("\n");
			while(str[j]!=0)
			{
				printf("%c",str[j]);
				++j;	
			}
		}/*Continue searching on string.*/
		++i;
	}
	printf("\n");
	return counter;/*Return ammount of substrings found.*/
}
/*
Main function:
Friendly requests input from user, print
parsed char and char sequence.
Than runs suffix(char str[], char c) on 
the input and prints returned value.
*/
int main()
{
	char input[size],symbol;
	int i=0;
	printf("Please enter a char and string with whitespace between them: ");
	scanf("%c %s",&symbol,input);/*Parse requiered pattern: char, whitespace, char sequence.*/
	printf("\nChosen symbol: %c, provided string: ",symbol);/*Print parsed values.*/
	for (i=0;input[i]!=0;i++) printf("%c",input[i]);
	printf("There are %d substrings starting with %c in a given string.\n",suffix(input,symbol),symbol);/*Print results.*/
	return 0;
}
