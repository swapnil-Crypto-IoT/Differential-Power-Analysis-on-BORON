/*****************************************************************************************
		Subroutine for the masked sbox implementation
******************************************************************************************/

#include<stdio.h>
#include<stdlib.h>

unsigned long long int msbox(unsigned long long int masks, unsigned long long int c)
{
	unsigned long long int i,j[16]={0},a=0,t[16]={0};
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

		for(i=0;i<16;i++)
		{
			t[15-i] = (masks>>(i*4))&0xf;
		}
		
		/*printf("\n");
		for(i=0;i<16;i++)
		{
			printf(" %llx ",t[i]);
		}
		printf("\n");*/
	a=0;
   	for (i=0;i<64;i=i+4)     
	{
	 j[((i*1)/4)] = mask_table1[t[((i*1)/4)]][((c>>(60-i)&0xf))];
	 a |= ((((j[((i*1)/4)])&0xf)<<(60-i)));
	}
//	printf("\n in function %llx", a);
	return(a);
}

