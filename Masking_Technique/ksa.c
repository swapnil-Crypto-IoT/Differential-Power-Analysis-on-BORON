/*****************************************************************************************
				Key Scheduling Algorithm Present - 128
*****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
unsigned long long int val1[32]={0};
unsigned long long int* key_read()
{
	FILE *fp;
	unsigned char i;
//	unsigned long long int grp ;//val1[32]={0},grp;
	fp=fopen("key_data.txt","r");
	for(i=0;i<32;i++)
	{
		fscanf(fp,"%1llx",&val1[i]); 
	}
	fclose(fp);
	return val1;
}

unsigned long long int keyupdate(unsigned long long *key_high, unsigned long long int *key_low , unsigned long long int *subkey, unsigned long long int *subkey1)
{
	unsigned long long int temp,m,i;
	unsigned int i1;
	unsigned char s[16] = {0xe,0x4,0xb,0x1,0x7,0x9,0xc,0xa,0xd,0x2,0x0,0xf,0x8,0x5,0x3,0x6};

//	subkey[0]=*key_high;
	subkey1[0]=*key_low;
	for (i=1;i<25;i++)
	{		
		temp=*key_high;
		*key_high = (temp<<13)|((*key_low)>>(64-13));
		*key_low = ((*key_low)<<13)|(temp>>(64-13)); // 128 bit key left circular shift by 13

		//Sbox	
			m=0;	
			for(i1=0; i1<2; i1++)
			{
				m|=(s[(((*key_low>>0)&0xff)>>(i1*4))&0xf]<<i1*4)&0xff;
			//printf("\n key low and  m is %lx & %lx \n",*key_low,m);
			}
			*key_low &= (0Xffffffffffffff00);  
			*key_low |= (((m<<00)&0x00000000000000ff));
			*key_low ^= ( ( (i-1) & 0x1f ) << 59 );			
		//	*key_low ^= ((i+1)&0x1f);//>> 2);
	 subkey[i]=*key_high;
	 subkey1[i]=*key_low;
	
	}

//	for (i=0;i<32;i++)
//	{		
//	printf("\n the value of subkey at round %02lld is %016llx(H) %016llx(L)\n",i,subkey[i],subkey1[i]);  
//	}
	
}
