/*********************************************************************************
			Inverse XOR layer in Boron block cipher
*********************************************************************************/

#include<stdio.h>
#include<stdlib.h>

unsigned long int invxor_layer(unsigned long int x)
{
	unsigned int x1,x2,x3,x4,y1,y2,y3,y4;
	
	y1= ((x&0xffff000000000000)>>48)&0xffff;
	y2= ((x&0x0000ffff00000000)>>32)&0xffff;
	y3= ((x&0x00000000ffff0000)>>16)&0xffff;
	y4= ((x&0x000000000000ffff)>>0)&0xffff;

	x1= y1^y2;
	x4= y3^y4;
	x3= x1^y3;
	x2= x4^y2;

	x = ((unsigned long)x1<<48) | ((unsigned long)x2<<32) | (x3<<16) |(x4) ;
	return(x);
}
