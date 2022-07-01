#include<stdio.h>
#include<stdlib.h>

/*****************************************************************************************
		Subroutine for the sbox implementation
******************************************************************************************/
unsigned long long int sbox(unsigned long long int c)
{

unsigned char s[16] = {0xe,0x4,0xb,0x1,0x7,0x9,0xc,0xa,0xd,0x2,0x0,0xf,0x8,0x5,0x3,0x6};
unsigned long long int i,j[16],a=0;

	a=0;
   	for (i=0;i<64;i=i+4)     
	{
	 j[((i*1)/4)]= s[((c>>(60-i)&0xf))];
	 a |= ((((j[((i*1)/4)])&0xf)<<(60-i)));
	}
	return(a);
}

