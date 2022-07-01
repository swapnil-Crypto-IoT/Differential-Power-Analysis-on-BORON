/************************************************************************************************
		Inverse cross shuffle code in boron block cipher
************************************************************************************************/
#include<stdio.h>
#include<stdlib.h>


unsigned long int invshuffle(unsigned long int x)
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

