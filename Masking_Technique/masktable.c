#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include<stdint.h>

unsigned char s[16] = {0xe,0x4,0xb,0x1,0x7,0x9,0xc,0xa,0xd,0x2,0x0,0xf,0x8,0x5,0x3,0x6}; //global sbox layer




unsigned long long int sbox(unsigned long long int c)
{

	unsigned long long int i,j[16],a=0;
	a=0;
   	for (i=0;i<64;i=i+4)     
	{
	 j[((i*1)/4)]= s[((c>>(60-i)&0xf))];
	 a |= ((((j[((i*1)/4)])&0xf)<<(60-i)));
	}
	return(a);
}

unsigned long long int msbox(unsigned int masks, unsigned long long int c)
{



unsigned long long int i,j[16]={0},a=0;
unsigned long long int mask_table1[16][16]={
{0xe, 0x4, 0xb, 0x1, 0x7, 0x9, 0xc, 0xa, 0xd, 0x2, 0x0, 0xf, 0x8, 0x5, 0x3, 0x6, },
{0xf, 0x5, 0xa, 0x0, 0x6, 0x8, 0xd, 0xb, 0xc, 0x3, 0x1, 0xe, 0x9, 0x4, 0x2, 0x7, },
{0xc, 0x6, 0x9, 0x3, 0x5, 0xb, 0xe, 0x8, 0xf, 0x0, 0x2, 0xd, 0xa, 0x7, 0x1, 0x4, },
{0xd, 0x7, 0x8, 0x2, 0x4, 0xa, 0xf, 0x9, 0xe, 0x1, 0x3, 0xc, 0xb, 0x6, 0x0, 0x5, },
{0xa, 0x0, 0xf, 0x5, 0x3, 0xd, 0x8, 0xe, 0x9, 0x6, 0x4, 0xb, 0xc, 0x1, 0x7, 0x2, },
{0xb, 0x1, 0xe, 0x4, 0x2, 0xc, 0x9, 0xf, 0x8, 0x7, 0x5, 0xa, 0xd, 0x0, 0x6, 0x3, },
{0x8, 0x2, 0xd, 0x7, 0x1, 0xf, 0xa, 0xc, 0xb, 0x4, 0x6, 0x9, 0xe, 0x3, 0x5, 0x0, },
{0x9, 0x3, 0xc, 0x6, 0x0, 0xe, 0xb, 0xd, 0xa, 0x5, 0x7, 0x8, 0xf, 0x2, 0x4, 0x1, },
{0x6, 0xc, 0x3, 0x9, 0xf, 0x1, 0x4, 0x2, 0x5, 0xa, 0x8, 0x7, 0x0, 0xd, 0xb, 0xe, },
{0x7, 0xd, 0x2, 0x8, 0xe, 0x0, 0x5, 0x3, 0x4, 0xb, 0x9, 0x6, 0x1, 0xc, 0xa, 0xf, },
{0x4, 0xe, 0x1, 0xb, 0xd, 0x3, 0x6, 0x0, 0x7, 0x8, 0xa, 0x5, 0x2, 0xf, 0x9, 0xc, },
{0x5, 0xf, 0x0, 0xa, 0xc, 0x2, 0x7, 0x1, 0x6, 0x9, 0xb, 0x4, 0x3, 0xe, 0x8, 0xd, },
{0x2, 0x8, 0x7, 0xd, 0xb, 0x5, 0x0, 0x6, 0x1, 0xe, 0xc, 0x3, 0x4, 0x9, 0xf, 0xa, },
{0x3, 0x9, 0x6, 0xc, 0xa, 0x4, 0x1, 0x7, 0x0, 0xf, 0xd, 0x2, 0x5, 0x8, 0xe, 0xb, },
{0x0, 0xa, 0x5, 0xf, 0x9, 0x7, 0x2, 0x4, 0x3, 0xc, 0xe, 0x1, 0x6, 0xb, 0xd, 0x8, },
{0x1, 0xb, 0x4, 0xe, 0x8, 0x6, 0x3, 0x5, 0x2, 0xd, 0xf, 0x0, 0x7, 0xa, 0xc, 0x9, },
};
	a=0;
   	for (i=0;i<64;i=i+4)     
	{
	 j[((i*1)/4)] = mask_table1[masks][((c>>(60-i)&0xf))];
	 a |= ((((j[((i*1)/4)])&0xf)<<(60-i)));
//	printf("\n in function %llx", mask_table1[1][0]);
	}
	printf("\n in function %llx", a);
	return(a);
}

int main()
{
 
 int i;
 char mask;
unsigned long long int mask_table[16][16]={0};
/* important logic to generate mask table dont delete it.*/ 

	printf("{");
	for (mask=0;mask<16;mask++)
	{
//	 printf("\n mask %x==> ",mask);
		printf("{");	 
	for (i=0;i<16;i++)
		{

		 mask_table[mask][i^mask] = s[i^mask]^mask;
		 printf("0x%02llx, ", mask_table[mask][i^mask]);

		}		printf("},");
	printf("\n");
	}
	printf("}");

	printf("\n the mask value is %llx",msbox(0xf,0x123456789abcdef));
	printf("\n");
}


