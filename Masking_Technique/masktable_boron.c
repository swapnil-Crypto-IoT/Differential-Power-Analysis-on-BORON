#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include<stdint.h>

unsigned char s[16] ={0xe,0x4,0xb,0x1,0x7,0x9,0xc,0xa,0xd,0x2,0x0,0xf,0x8,0x5,0x3,0x6}; //global sbox layer




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
unsigned long long int mask_table1[16][16]={{0x0e, 0x04, 0x0b, 0x01, 0x07, 0x09, 0x0c, 0x0a, 0x0d, 0x02, 0x00, 0x0f, 0x08, 0x05, 0x03, 0x06, },
{0x05, 0x0f, 0x00, 0x0a, 0x08, 0x06, 0x0b, 0x0d, 0x03, 0x0c, 0x0e, 0x01, 0x04, 0x09, 0x07, 0x02, },
{0x09, 0x03, 0x0c, 0x06, 0x0e, 0x08, 0x05, 0x0b, 0x02, 0x0d, 0x0f, 0x00, 0x01, 0x04, 0x0a, 0x07, },
{0x02, 0x08, 0x07, 0x0d, 0x09, 0x0f, 0x0a, 0x04, 0x0c, 0x03, 0x01, 0x0e, 0x05, 0x00, 0x06, 0x0b, },
{0x03, 0x0d, 0x08, 0x0e, 0x0a, 0x00, 0x0f, 0x05, 0x0c, 0x01, 0x07, 0x02, 0x09, 0x06, 0x04, 0x0b, },
{0x0c, 0x02, 0x0f, 0x09, 0x01, 0x0b, 0x04, 0x0e, 0x00, 0x0d, 0x03, 0x06, 0x07, 0x08, 0x0a, 0x05, },
{0x0a, 0x0c, 0x01, 0x0f, 0x0d, 0x07, 0x08, 0x02, 0x05, 0x00, 0x0e, 0x03, 0x06, 0x09, 0x0b, 0x04, },
{0x0d, 0x0b, 0x0e, 0x00, 0x06, 0x0c, 0x03, 0x09, 0x01, 0x04, 0x02, 0x0f, 0x08, 0x07, 0x05, 0x0a, },
{0x05, 0x0a, 0x08, 0x07, 0x00, 0x0d, 0x0b, 0x0e, 0x06, 0x0c, 0x03, 0x09, 0x0f, 0x01, 0x04, 0x02, },
{0x0b, 0x04, 0x06, 0x09, 0x0c, 0x01, 0x0f, 0x0a, 0x0d, 0x07, 0x08, 0x02, 0x00, 0x0e, 0x03, 0x05, },
{0x0a, 0x05, 0x07, 0x08, 0x09, 0x0c, 0x02, 0x0f, 0x01, 0x0b, 0x04, 0x0e, 0x06, 0x00, 0x0d, 0x03, },
{0x04, 0x0b, 0x09, 0x06, 0x0d, 0x08, 0x0e, 0x03, 0x0a, 0x00, 0x0f, 0x05, 0x01, 0x07, 0x02, 0x0c, },
{0x04, 0x09, 0x0f, 0x0a, 0x01, 0x0e, 0x0c, 0x03, 0x0b, 0x05, 0x00, 0x06, 0x02, 0x08, 0x07, 0x0d, },
{0x08, 0x05, 0x0b, 0x0e, 0x0f, 0x00, 0x02, 0x0d, 0x04, 0x0a, 0x07, 0x01, 0x09, 0x03, 0x0c, 0x06, },
{0x0d, 0x08, 0x06, 0x0b, 0x0e, 0x01, 0x03, 0x0c, 0x02, 0x04, 0x09, 0x07, 0x05, 0x0f, 0x00, 0x0a, },
{0x09, 0x0c, 0x0a, 0x07, 0x00, 0x0f, 0x0d, 0x02, 0x05, 0x03, 0x06, 0x08, 0x0e, 0x04, 0x0b, 0x01, },
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


