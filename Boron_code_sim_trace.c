#include<stdio.h>
#include <stdlib.h>

#define keylen 32
#define NSAMPLE 2000

FILE *fpo;

unsigned long long int pt,ct,state;

//***************************globle variable declaration***********************
unsigned char s[16] = {0xe,0x4,0xb,0x1,0x7,0x9,0xc,0xa,0xd,0x2,0x0,0xf,0x8,0x5,0x3,0x6};

//*****************************************************************************
unsigned long long int val1[32]={0};

//***************************function declaration area*************************
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

unsigned long int shuffle(unsigned long int x)
{
	unsigned long int temp1,temp2,xl,xr;	
	xl = (x&0xffffffff00000000)>>32;
	xr = (x&0xffffffff);
//	printf("\n the value in shuffle is %lx %lx\n",xl,xr);

	temp1 = (((xl&0xff00ff00)>>8)&0xffffffff) | (((xl&0x00ff00ff)<<8)&0xffffffff);
//	printf("\n the value of temp1 is %08lx\n",temp1);
	temp2 = (((xr&0xff00ff00)>>8)&0xffffffff) | (((xr&0x00ff00ff)<<8)&0xffffffff);
//	printf("\n the value of temp2 is %08lx\n",temp2);
	x = (temp1<<32)|(temp2);
	return(x);
}


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

unsigned long long int* key_read()
{
	FILE *fp;
	unsigned char i;
	unsigned long long int grp ;//val1[32]={0},grp;
	fp=fopen("key_data.txt","r");
	for(i=0;i<keylen;i++)
	{
		fscanf(fp,"%01llx",&val1[i]); 
	}
	fclose(fp);
	return val1;
}


void PrintState(unsigned long long pt)
{
	fprintf(fpo, "%016llx", pt);
	fprintf(fpo, "\n");
	
}


unsigned long int subkey[32]={0};
unsigned long int subkeyl[32]={0};          //k[0]                      //k[1]
unsigned long int keyupdate(unsigned long *key_high, unsigned long int *key_low)
{
	unsigned long int temp,m,i;
	unsigned int i1;
	
//	subkey[0]=*key_high;
	subkeyl[0]=*key_low;
	for (i=1;i<=25;i++)
	{		
		
		temp=*key_high;
		// 128 bit key left circular shift by 13
		*key_high = (temp<<13)|((*key_low)>>(64-13));
		*key_low = ((*key_low)<<13)|(temp>>(64-13)); 

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
	 subkeyl[i]=*key_low;
	}

	for (i=0;i<=25;i++)
	{		
	printf("\n the value of subkey at round %ld is %016lx <<%016lx>>\n",i,subkey[i],subkeyl[i]);  
	}	
}
unsigned long int k[2]={0x0,0x0};
unsigned long int cipher(unsigned long int pt)
{
	unsigned int round;
	
	for(round=0;round<25;round++)
	{		
//	PrintState(pt);		
	pt^=subkeyl[round];
//		printf("\n the value of pt after subkey is %016lx\n",pt);		
//	PrintState(pt);		
		pt=sbox(pt);
//		printf("\n the value of pt after sbox is %016lx\n",pt);
//	PrintState(pt);			
		
		pt = shuffle(pt);
//		printf("\nthe value of pt after shuffle is %016lx\n",pt);
//	PrintState(pt);		
		pt=LCS(pt);
//		printf("\n the value of pt after LCS is %016lx\n",pt);
//	PrintState(pt);	
		pt=xor_layer(pt);
//		printf("\n the value of pt after xor layer %016lx\n",pt);
//		printf("\nthe value of pt is %016lx\n",pt);
	PrintState(pt);	
	}

//	pt^=subkeyl[25];	
	//printf("\nthe value of ciphertext is %016lx\n",pt);	
	ct=pt;
	return pt;

}

//*****************************************************************************



//**************************main start here************************************
int main()
{
	unsigned long int temp,pt=0x0;
	unsigned int i,i1,r_c;
	unsigned long long int *val1,r[16];    

	int ch,j = 0, size, t;
	unsigned char length[3] ;
	unsigned char *line = NULL;
	FILE *fpt, *fpc;
	unsigned char outputFile[50];
//*******************key schedule input read from file*********************************
	val1 = key_read();
	k[0]=0;
	
	for (i=0;i<16;i++)
	{
		k[0]|=(val1[i]&0xf)<<(((16-1)-i)*4);
	}	
	
	k[1]=0;	
	for (i=16;i<32;i++)
	{
		k[1]|=(val1[i]&0xf)<<(((32-1)-i)*4);
	}



//****************************************************	
//	printf("\nthe value of pt is %016lx\n",pt);
//	printf("\nthe value of key is %016lx %016lx \n", k[0],k[1]);

	keyupdate(&k[0],&k[1]); // key update 

	if(!(fpt = fopen("PT_file.txt", "r"))) //reading the 20000 sample plaintext from PT_file.txt
	{
		printf("Cannot open file PT_file.txt\n");
		exit(1);
	}

	if(!(fpc = fopen("CT_file.txt", "w"))) // writing the corrosponding Ciphertext in CT_file.txt
	{
		printf("Cannot open file CT_file.txt\n");
		exit(1);
	}
	//getline(&line, &size, fpt);
	//puts(line);	


//************************* generating intermidiate state in partialEnc file with numbering******
	for(i = 0; i < NSAMPLE; i++)
	{
		sprintf(outputFile, "Output/partialEnc%d.txt", i);
		if(!(fpo = fopen(outputFile, "w")))
		{
			printf("Cannot open file %s!\n", outputFile);
			exit(1);
		}


			pt=0;
			for (i1=0;i1<16;i1++)
			{
				fscanf(fpt,"%01llx",&r[i1]);	// reading the file PT_file.txt
				pt|=(r[i1]&0xf)<<((15-i1)*4);
			//	printf("\n %lx \n",pt);
			}

			pt=cipher(pt);
	
			fprintf(fpc, "%016llx\n", ct);   // printing the ciphertext to CT_file.txt
			//printf("\nthe value of ct is---final output-- %llx\n",ct);
			fclose(fpo);
	}
			//free(line);
			fclose(fpc);
			fclose(fpt);
}
	

//**************************main ends here*************************************


