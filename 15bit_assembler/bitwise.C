#include <assembler.h>


/* Bit operations with words. */

void setBit (WORD * a, unsigned int k)
{
  unsigned int flag = 1;	//flag = 0000...00001
  flag = flag << k;		// flag = 0000...010...000   (shifted k positions)
  a->word = a->word | flag;
}

unsigned int getBit (WORD * a, unsigned int k)
{
  unsigned int flag = 1;	// flag = 0000.....00001
  flag = flag << k;		// flag = 0000...010...000   (shifted k positions)
  return (a->word & flag) ? 1 : 0;
}

void unsetBit (WORD * a, unsigned int k)
{
  unsigned int flag = 1;	// flag = 0000.....00001
  flag = flag << k;		// flag = 0000...010...000   (shifted k positions)
  flag = ~flag;			// flag = 1111...101..111
  a->word = a->word & flag;	// RESET the bit at the k-th position
}

/* Function initiates new word and returns it. */
WORD *init_word ()
{
	int i;
  WORD *a;
  a=(WORD*) calloc (1,sizeof (WORD));
  for(i=0;i<15;i++)
  {
  	unsetBit(a,i);
  }
  return a;
}

/* Function to set given integer number in binary in given WORD. */
void set_to_binary (WORD * word, int k)
{
  int i, flag = 0;
  for (i = 0; i < 15; i++)	//unset the original word
    {
      unsetBit (word, i);
    }
  if (k < 0)
    {
      flag = 1;
      k = k * -1;
    }
  for (i = 0; i < 14; i++)
    {
      if (k == 0)
	break;
      if (k % 2)
	setBit (word, i);
      k = k / 2;
    }
  if (k > 0)
    {
      fprintf (stderr,
	       "ERROR in line %d: too big decimal passed as operand.\n", CL);
      Error_Flag = 1;
      return;
    }
  if (flag)			//if negative decimnal
    {
				for (i = 0; i < 15; i++)	//flip every bit, implement one's complement
					{
						if (getBit (word, i))
						unsetBit (word, i);
						else
						setBit (word, i);
					}
				for (i = 0; i < 14; i++)	//flip all ones till' catch 0 and break
					{
						if (getBit (word, i))
						unsetBit (word, i);
						else
						{
							setBit (word, i);
							break;
						}
					}
				if(i==-1)
					{
						fprintf (stderr,"ERROR in line %d: too small negative decimal passed as operand.\n", CL);
								Error_Flag = 1;
								return;
					}
    }
}

/* Function to set opcode to instruction word. */
int set_opcode (WORD *a, unsigned int k)
{
	unsigned int i;
	for(i=11;i<15;i++)unsetBit(a,i); //reset opcode in case there was one
  for (i = 11; i < 15; i++)
  {
    if (k == 0)
	break;
    if (k % 2)
	setBit (a, i);
    k = k / 2;
  }
  if (k > 0)
  {
    fprintf (stderr,"ERROR in line %d: invalid opcode passed to set_opcode.\n", CL);
    Error_Flag = 1;
    return -1;
  }
  return 1;
}

/* Returns binary value of given WORD. */
char *toString (WORD * a)
{
  int i = 0;
  char *pointer, temp[16];
  pointer =(char*) calloc (16,sizeof (char));
  while (i < 15)
    {
      temp[i] = getBit (a, 14 - i) + 48;
      i++;
    }
  temp[16] = '\0';
  strcpy (pointer, temp);
  return pointer;
}
