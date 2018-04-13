#include "reservoirsample-algorithm/reservoirsample-algorithm.h"


#include"stdio.h"
#include"stdlib.h"
//#include"windows.h"
#include"time.h"
#include"math.h"
//#include"conio.h"
//for linux 
typedef unsigned long DWORD;
unsigned long GetTickCount()  
{  
    struct timespec ts;  
  
    clock_gettime(CLOCK_MONOTONIC, &ts);  
  
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);  
}  

int prob(DWORD a,DWORD b)
{
	double p=(double)a/b,r;
	r=(double)rand()/RAND_MAX;
	if(r<p)
		return 1;
	return 0;
}
void replace(int *s,unsigned long N,int value)
{
	DWORD i;
	i=(double)rand()/RAND_MAX*N;
	s[i]=value;
}
void main()
{
	FILE *fp;
	DWORD i=0;
	int64_t ssum=0,sum=0;
	int value=-1;
	DWORD t=0,N=0;
	int *sample;
	DWORD end,start;
	srand((unsigned)time(NULL));

	printf("Input Sample size :\t");
	scanf("%d",&N);
	start=GetTickCount();
	sample=(int *)malloc(N*sizeof(int));
	fp=fopen("./data/stream.txt","r");
	while(fscanf(fp,"%d\n",&value)!=EOF)
	{
		t++;
		if(t<=N)
		{
			sample[t-1]=value;
		}
		else 
		{
			if(prob(N,t))
			{
				//keep and replace
				replace(sample,N,value);
			}
			else
			{
				//discard
			}
		}
		sum+=value;
	}
	fclose(fp);
	for(i=0;i<N;i++)
	{
		ssum+=sample[i];
	}
	printf("\nSample average   %.1f\t",(float)ssum/N);
	printf("\nStream average   %.1f\t",(float)sum/t);
	end=GetTickCount();
	printf("\n\t\t\t\t%.2f %c\t\t%lld ms\n",((float)ssum/N-(float)sum/t)/((float)sum/t)*100,'%',end-start);
	
	printf("\n\n");
}

