/*****************************************************************************************
Author : Swapnil Ashok Sutar
Date : 5th September 2019
Code :  Implement the masking technique in Boron block cipher (128 bit)
******************************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#include"ksa.c"

#include"sbox.c"
#include"shuffle.c"
#include"lcs.c"
#include"xor.c"

#include"invsbox.c"
#include"invshuffle.c"
#include"invlcs.c"
#include"invxor.c"

#include"invmsbox.c"
#include"mask_sbox.c"
/*****************************************************************************************
				Main function 
*****************************************************************************************
Plaintext : 64 bit plaintext data
Ciphertext : 64 bit ciphered data
mask :  64 bit value used for masking
******************************************************************************************/
int main()
{
unsigned long long int Plaintext = 0x6b8b45676b8b4567, Ciphertext, mask, *keyval, k[2], t1,t2,t3, x1;
unsigned int i;
unsigned long long int subkey[32]={0};
unsigned long long int subkey1[32]={0};
/*****************************************************************************************
				Read the keys
*****************************************************************************************/
	keyval = key_read();
	k[0]=0;
	for (i=0;i<16;i++)
	{
		k[0]|=(keyval[i]&0xf)<<(((16-1)-i)*4);
	}	
	
	k[1]=0;	
	for (i=16;i<32;i++)
	{
		k[1]|=(keyval[i]&0xf)<<(((32-1)-i)*4);
	}
	keyupdate(&k[0],&k[1],subkey,subkey1); // key update
	printf("\n--------------------------------------------------\n");
	for (i=0;i<32;i++)
	{		
	printf("\n the value of subkey at round %02d is %016llx(H) %016llx(L)\n",i,subkey[i],subkey1[i]);  
	}
/*****************************************************************************************/

 
/*****************************************************************************************
			Input the mask from the user
*****************************************************************************************/
mask=0xed245481234af168;
/*printf("\n Enter the Mask (M): -->");
scanf("%016llx",&mask);*/
/*****************************************************************************************/

/*****************************************************************************************
	Normal Boron encryption
*****************************************************************************************/
Plaintext = 0x6b8b45676b8b4567;
Ciphertext = 0x00;
x1=0;
printf("\n Encryption starts \n original plaintext-- %llx \n",Plaintext);
	//temp = player(mask);
	//printf("\n temp--%llx  \n",temp);
	//x1 = Plaintext^mask;
	x1 = Plaintext;
	for(i=0;i<25;i++)
	{		
	x1^=subkey1[i];
	printf("\n the value of pt after subkey is %016llx\n",x1);		
	x1=sbox(x1);
	printf("\n the value of pt after sbox is %016llx\n",x1);
	x1 = shuffle(x1);
	printf("\nthe value of pt after shuffle is %016llx\n",x1);
	x1=LCS(x1);
	printf("\n the value of pt after LCS is %016llx\n",x1);
	x1=xor_layer(x1);
	printf("\n the value of pt after xor layer %016llx\n",x1);
	}
	x1 ^= subkey1[25];   
	Ciphertext = x1;
printf("\n Encryption end \n original Ciphertext-- %llx \n",Ciphertext);

printf("\n####################################################################################################\n");

/*****************************************************************************************
	Normal Boron decryption
*****************************************************************************************/
x1=0;
printf("\n Decryption starts \n original Ciphertext-- %llx \n",Ciphertext);
	x1 = Ciphertext^subkey[25];
	for(i=0;i<25;i++)
	{
	 
	x1 = invxor_layer(x1);
	x1 = invlcs(x1);
	x1 = invshuffle(x1);
	printf("\n the value of pt after sbox is %016llx\n",x1);	
	x1 = invsbox(x1);
	x1 ^= subkey1[24-i];
	//printf("\n keyxor state--%llx  key--%llx \n",x1,subkey[i]);
	}
	Plaintext = 0;	
	Plaintext = x1;
printf("\n Decryption end \n original Plaintext-- %llx \n",Plaintext);
printf("\n####################################################################################################\n");
/*****************************************************************************************
	Masked boron encryption
*****************************************************************************************/
Plaintext = 0x6b8b45676b8b4567;
Ciphertext = 0x00;
x1=0;

printf("\n masked Encryption starts \n original plaintext-- %llx \n",Plaintext);
	t1 = shuffle(mask);
	t2 = LCS(mask);
	t3 = xor_layer(mask);
	//printf("\n temp--%llx  \n",temp);
	x1 = Plaintext^mask; //mask apply
	for(i=0;i<25;i++)
	{
	 x1 ^= subkey1[i];
	 printf("\n mask keyxor state--%llx  key--%llx \n",x1,subkey[i]);
	 
	 x1 = msbox(mask,x1);
	 printf("\n mask xbox--%llx  \n",x1);
	 
//	 x1 = player(x1)^temp^mask;
//	 printf("\n mask playerout--%llx\n",x1);	 
	
	x1 = shuffle(x1)^t1^mask;
	printf("\nthe value of pt after shuffle is %016llx\n",x1);
	x1=LCS(x1)^t2^mask;
	printf("\n the value of pt after LCS is %016llx\n",x1);
	x1=xor_layer(x1)^t3^mask;
	printf("\n the value of pt after XOR layer is %016llx\n",x1);	
 	}
	x1 = x1 ^ mask;
	x1 ^= subkey1[25];   
	Ciphertext = x1;
printf("\n Masked Encryption end \n original Ciphertext-- %llx \n",Ciphertext);
printf("\n####################################################################################################\n");

/*****************************************************************************************
	Masked boron decryption
*****************************************************************************************/
x1=0;
t1=t2=t3=0;
Plaintext=0;
printf("\n Masked Decryption starts \n original plaintext-- %llx \n",Ciphertext);
	//temp = player(mask);
	//printf("\n temp--%llx  \n",temp);
	//x1 = Plaintext^mask;
	x1 = Ciphertext;
	x1^= subkey1[25];
	x1 = x1^mask;
	t1 = invxor_layer(mask);
	t2 = invlcs(mask);
	t3 = invshuffle(mask);
	for(i=0;i<25;i++)
	{
	 
	x1 = invxor_layer(x1)^t1^mask;
	//printf("\n mask playerout--%llx\n",x1);	 
	 
	x1 = invlcs(x1)^t2^mask;
	x1 = invshuffle(x1)^t3^mask;

	x1 = invmsbox(mask,x1);
	printf("\n mask xbox--%llx  \n",x1);
	 

	x1 = x1 ^ subkey1[24-i];
//	printf("\n keyxor state--%llx  key--%llx \n",x1,subkey[31-i]);
	}
	x1=x1^mask;
	Plaintext = x1;
printf("\n Masked Decryption end \n original Plaintext-- %llx \n",Plaintext);
printf("\n####################################################################################################\n");	
}
