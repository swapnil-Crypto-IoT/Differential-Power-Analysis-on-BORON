/*********************************************************************************
				XOR layer in Boron block cipher
*********************************************************************************/

#include<stdio.h>
#include<stdlib.h>

unsigned long int xor_layer(unsigned long int x)
{
	unsigned int x1,x2,x3,x4,y1,y2,y3,y4;
	
	x1= ((x&0xffff000000000000)>>48)&0xffff;
	x2= ((x&0x0000ffff00000000)>>32)&0xffff;
	x3= ((x&0x00000000ffff0000)>>16)&0xffff;
	x4= ((x&0x000000000000ffff)>>0)&0xffff;

	y2 = x2^x4;
	y1 = x1^y2;
	y3 = x1^x3;
	y4 = y3^x4;
	
	x = ((unsigned long)y1<<48) | ((unsigned long)y2<<32) | (y3<<16) |(y4) ;
	return(x);
}
