/*******************************************************************************
			 left circular shift in boron block cipher
*******************************************************************************/



#include<stdio.h>
#include<stdlib.h>

unsigned long int LCS(unsigned long int x)
{
	unsigned int x1,x2,x3,x4;
	
	x1= ((x&0xffff000000000000)>>48)&0xffff;
	x1 = ((x1<<9)|(x1>>(16-9)))&0xffff;

	x2= ((x&0x0000ffff00000000)>>32)&0xffff;
	x2 = ((x2<<7)|(x2>>(16-7)))&0xffff;

	x3= ((x&0x00000000ffff0000)>>16)&0xffff;
	x3 = ((x3<<4)|(x3>>(16-4)))&0xffff;

	x4= ((x&0x000000000000ffff)>>0)&0xffff;
	x4 = ((x4<<1)|(x4>>(16-1)))&0xffff;

	//printf("\nthe value of x1-x2-x3-x4 in LCS is %x-%x-%x-%x\n",x1,x2,x3,x4);
	
	x = ((unsigned long)x1<<48) | ((unsigned long)x2<<32) | (x3<<16) |(x4) ;
	
	//printf("\n the value of final x in Lcs %lx\n",x);
	
	return(x);
	
}

