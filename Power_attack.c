// hamming distance power attack model for boron block cipher
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include<stdint.h>
#include<time.h>

#define NSAMPLE 2000
#define NPOINT 25
#define NKEY 16
//#define SBOX 13
#define FROM_HEX(x) ((x) < 10? (x) : (x) - 7)
#define MIN(x,y) ((x) > (y)? (y) : (x))

int inverseSBox[] = {0xa,0x3,0x9,0xe,0x1,0xd,0xf,0x4,0xc,0x5,0x7,0x2,0x6,0x8,0x0,0xb};

int p[] = {0,16,32,48,49,1,17,33,34,50,2,18,19,35,51,3,4,20,36,52,53,5,21,37,38,54,6,22,23,39,55,7,8,
24,40,56,57,9,25,41,42,58,10,26,27,43,59,11,12,28,44,60,61,13,29,45,46,62,14,30,31,47,63,15};

int SBOX;
int ReadSample(int);
int PrintKey(double keyBias[], int biasIndex[]);
int HD(int v1, int v2);
int HDS(char line[], char prevLine[]);
void strncopy(char des[], char src[], int n);

long trace[NPOINT][NSAMPLE]={0};
int hConsumedPower[NKEY][NSAMPLE]={0};
double result[NKEY][NPOINT]={0};
double meanH[NKEY] = {0};
double meanTrace[NPOINT] = {0};

int windowSize;


int main()
{
	int i, j, k, index,key;
	double temph, tempt, sumHT, sumSqT, sumSqH;
	FILE *fp;
	char fileName[50];
	double keyBias[NKEY], bias;
	int biasIndex[NKEY];
	unsigned long int finalkey=0, subkey;	
	clock_t start, end;
	double cpu_time_used;

	start = clock();
	for(SBOX=0;SBOX<16;SBOX++)
	{ReadSample(SBOX);

	for(i = 0; i < NKEY; i++)
	{
		//printf("Running for key %d\n", i);
		for(j = 0; j < windowSize; j++)
		{
			sumHT = 0;
			sumSqH = 0;
			sumSqT = 0;
			
			for(k = 0; k < NSAMPLE; k++)
			{
				temph = hConsumedPower[i][k] - meanH[i];
				tempt = trace[j][k] - meanTrace[j];
				sumHT += temph*tempt;
				sumSqH += temph*temph;
				sumSqT += tempt*tempt;
			}

			result[i][j] = sumHT/sqrt(sumSqH*sumSqT);
		}
	}

	if(!(fp = fopen("HammingDistance/Result.txt", "w")))
		printf(" Cannot open file Result.txt");
	else
	{
		for(i = 0; i < NKEY; i++)
		{
			for(j = 0; j < windowSize; j++)
				fprintf(fp, "%e\t", result[i][j]);
			fprintf(fp, "\n");
		}

		fclose(fp);
	}

	for(i = 0; i < NKEY; i++)
	{
		bias = 0;
		for(j = 0; j < windowSize; j++)
	//if(fabs(bias) < fabs(result[i][j])) 
	if((bias) < (result[i][j])) 
			{
				bias = result[i][j];
				index = j;
			}
		keyBias[i] = bias;
		biasIndex[i] = index;
	}
	
	subkey=PrintKey(keyBias, biasIndex);
	finalkey|= (subkey&0xf)<<((15-SBOX)*4);
	printf("\n--------------------------------------------\n");		
	
}
		 end = clock();
	cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC; 
printf("\n*************** The retrieved sub-key is %016lx ***************\n",finalkey);
printf("\n*** The execution time  taken for DPA on BORON is %f seconds ***\n\n", cpu_time_used);
//system("xdg-open HammingDistance/SortedKeys.txt");
}


/*unsigned long int Inv_PBox(unsigned long int a)
{
	unsigned long int b;
	unsigned int i;
	b=0;
    for (i=0;i<64;i++)
    {
		 b |= ((a>>i)&0x1)<<p[i];
    }
	 return(b);
}*/

/*unsigned long int Inv_PBox(unsigned long int a)
{
	unsigned long int b;
	unsigned int i;
	b=0;
    for (i=0;i<64;i++)
    {
		 b |= ((a>>i)&0x1)<<p[p[i]];
    }
	
	 return(b);
}*/

unsigned long long int Inv_sbox(unsigned long long int c)
{

	unsigned long long int i,j[16],a=0;
	a=0;
   	for (i=0;i<64;i=i+4)     
	{
	 j[((i*1)/4)]= inverseSBox[((c>>(60-i)&0xf))];
	 a |= ((((j[((i*1)/4)])&0xf)<<(60-i)));
	}
	return(a);
}


unsigned long int xor_layer_i(unsigned long int x) // boron inverse xor layer
{
	unsigned int x1,x2,x3,x4,y1,y2,y3,y4;
	
	y1= ((x&0xffff000000000000)>>48)&0xffff;
	y2= ((x&0x0000ffff00000000)>>32)&0xffff;
	y3= ((x&0x00000000ffff0000)>>16)&0xffff;
	y4= ((x&0x000000000000ffff)>>0)&0xffff;

	x1 = y2^y1;
	x3 = y3^x1;
	x4 = y4^y3;
	x2 = y2^x4;
	
	x = ((unsigned long)x1<<48) | ((unsigned long)x2<<32) | (x3<<16) |(x4) ;
	return(x);
}

unsigned long int LCS_i(unsigned long int x) // boron inverse LCS layer
{
	unsigned int x1,x2,x3,x4;
	
	x1= ((x&0xffff000000000000)>>48)&0xffff;
	x1 = ((x1>>9)|(x1<<(16-9)))&0xffff;

	x2= ((x&0x0000ffff00000000)>>32)&0xffff;
	x2 = ((x2>>7)|(x2<<(16-7)))&0xffff;

	x3= ((x&0x00000000ffff0000)>>16)&0xffff;
	x3 = ((x3>>4)|(x3<<(16-4)))&0xffff;

	x4= ((x&0x000000000000ffff)>>0)&0xffff;
	x4 = ((x4>>1)|(x4<<(16-1)))&0xffff;

	//printf("\nthe value of x1-x2-x3-x4 in LCS is %x-%x-%x-%x\n",x1,x2,x3,x4);
	
	x = ((unsigned long)x1<<48) | ((unsigned long)x2<<32) | (x3<<16) |(x4) ;
	
	//printf("\n the value of final x in Lcs %lx\n",x);
	
	return(x);
	
}

unsigned long int shuffle_i(unsigned long int x) // boron inverse shuffle layer
{
	unsigned long int temp1,temp2,xl,xr;	
	xl = (x&0xffffffff00000000)>>32;
	xr = (x&0xffffffff);
//	printf("\n the value in shuffle is %lx %lx\n",xl,xr);

	temp1 = (((xl&0x00ff00ff)<<8)&0xffffffff) | (((xl&0xff00ff00)>>8)&0xffffffff);
//	printf("\n the value of temp1 is %08lx\n",temp1);
	temp2 = (((xr&0x00ff00ff)<<8)&0xffffffff) | (((xr&0xff00ff00)>>8)&0xffffffff);
//	printf("\n the value of temp2 is %08lx\n",temp2);
	x = (temp1<<32)|(temp2);
	return(x);
}


int ReadSample(SBOX)
{
	int i, j, dig, cipher, scipher,i1;
	double prevTrigger, curTrigger;
	double voltageValue;
	int windowStart;
	char fileName[100], line[50], prevLine[50]; unsigned long int r1;
	unsigned long int ct,val;
	FILE *fps1, *fpc1, *fpo1; 

	if(!(fpc1 = fopen("CT_file.txt", "r")))
	{
		printf("Cannot open file: CT_file.txt");
		exit(0);
	}

	windowSize = NPOINT;
	windowStart = NPOINT - windowSize;

	for(i = 0; i < NSAMPLE; i++)

	{

if(i<NSAMPLE-1)		
		printf(" \r please wait sample is reading.....%f%%",(i+1)*0.05);
else
{
printf("\r.........sample read successfully for SBOX-%02d.....%f%% \n",SBOX,(i+1)*0.05);
}	

		//printf("Reading sample %d", i);
		memset(prevLine, 0, 16*sizeof(char));
		fscanf(fpc1, "%s", line);
//		puts(line);
		
/*		sscanf(line,"%lx",&r1);
	
			ct=0;
			for (i1=0;i1<16;i1++)
			{
					// reading the file PT_file.txt
				ct|=((r1>>(i1*4))&0xf)<<((i1)*4);
				
			}
		ct = Inv_PBox(ct);		
		ct = Inv_sbox(ct);
		sprintf(line,"%lx",ct);
*/		//puts(line);	
		
		dig = line[SBOX] - '0';
		cipher = FROM_HEX(dig);
		cipher = cipher&0xf;
	
		sscanf(line,"%lx",&r1);
		ct=0;
			for (i1=0;i1<16;i1++)
			{
			// reading the file CT_file.txt
			ct|=((r1>>(i1*4))&0xf)<<((i1)*4);
			}
		ct = xor_layer_i(ct);
		ct = LCS_i(ct);
		ct = shuffle_i(ct);
		sprintf(line,"%lx",ct);
	//puts (line);

		

	//	printf("\n dig value %x\n",cipher);
		//dig = line[SBOX+1] - '0';
		//cipher = (cipher << 4) + FROM_HEX(dig);

		dig = line[SBOX] - '0';
		scipher = FROM_HEX(dig);

	//	dig = line[SBOX+1] - '0';
	//	scipher = (scipher<<4)+FROM_HEX(dig);

		scipher = scipher&0xf;

		//printf("\n the cipher value is %d %x\n",SBOX,scipher);
	//	dig = line[SBOX+1] - '0';
	//	scipher = (scipher << 4) + FROM_HEX(dig);
//printf("\n scipher value %x\n",scipher);
		sprintf(fileName, "Output/partialEnc%d.txt", i);
		if(!(fps1 = fopen(fileName, "r")))
		{
			printf("Cannot open file: %s\n", fileName);
			exit(0);
		}

		for(j = 0; j < windowSize; j++)
		{	//fscanf(fps1, "%ld", &val); for actual trace model
			fscanf(fps1, "%s", line);
			trace[j][i] = HDS(line, prevLine);
			meanTrace[j] += trace[j][i];
			strncopy(prevLine, line, 32);
		}
			
		for(j = 0; j < NKEY; j++)
		{
//			hConsumedPower[j][i] = HD(cipher, inverseSBox[scipher^j]);
			hConsumedPower[j][i] = HD(cipher^j, inverseSBox[scipher]);
			meanH[j] += hConsumedPower[j][i];
		}

		fclose(fps1);
	}

	for(i = 0; i < windowSize; i++)
		meanTrace[i] = meanTrace[i]/NSAMPLE;
	for(i = 0; i < NKEY; i++)
		meanH[i] = meanH[i]/NSAMPLE;

	fclose(fpc1);
//	fclose(fpl);

/*	fpo1 = fopen("Output/SampleOut.txt", "w");
	for(i = 0; i < NCIPHER; i++)
	{
		fprintf(fpo1, "Cipher: %02x\tCount: %3d\t", i, freqSample[i]);
		for(j = 0; j < 50; j++)
			fprintf(fpo1, "%10lf ", sample[i][j]);
		fprintf(fpo1, "\n");
	}
	fclose(fpo1);*/
}

int PrintKey(double keyBias[], int biasIndex[])
{
	int i, j, key;
	double temp;
	FILE *fp;

	if(!(fp = fopen("HammingDistance/SortedKeys.txt", "w")))
	{
		printf("Cannot open file SortedKeys.txt!\n");
		exit(0);
	}

	for(i = 0; i < NKEY; i++)
	{
		temp = 0;
		for(j = 0; j < NKEY; j++)
			if((keyBias[j] != 0) && (fabs(keyBias[j]) > fabs(temp)))
			{
				temp = keyBias[j];
				key = j;
			}
		keyBias[key] = 0;
		fprintf(fp, "Key = %x\tBias = %e\tBias Index = %4d\n", key, temp, biasIndex[key]);
		if(i==0)
		{
		printf("The recovered %d nibble of the key is %x\n",15-SBOX,key);
		return(key);
		}
		
	}
	
	fclose(fp);
}

int HD(int v1, int v2)
{
	int count = 0;

	v1 ^= v2;
	while(v1)
	{
		v1 = v1 & (v1-1);
		count++;
	}

	return count;
}

int HDS(char line[], char prevLine[])
{
	int count = 0;
	int i, j;
	int num1, num2;
	for(i = 0; i < 16; i++)
	{
		num1 = FROM_HEX(line[i]-'0');
		num2 = FROM_HEX(prevLine[i]-'0');
		num1 = num1^num2;
		
		for(j = 0; j < 4; j++)
		{
			count += num1&1;
			num1 >>= 1;
		}
	}
	return count;
}

void strncopy(char des[], char src[], int n)
{
	int i;

	for(i = 0; i < n; i++)
		des[i] = src[i];
}
