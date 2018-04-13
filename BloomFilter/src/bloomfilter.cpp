#include "bloomfilter/bloomfilter.h"


#include"stdio.h"
#include"stdlib.h"
#include"math.h"
#include"time.h"
typedef unsigned long ulong;
typedef struct bnode
{
	ulong value;
	struct bnode *lchild,*rchild;
}BinTree;
BinTree *creat(BinTree *top,ulong value)
{
	if(NULL==top)
	{
		top=(BinTree *)malloc(sizeof(BinTree));
		top->value=value;
		top->lchild=NULL;
		top->rchild=NULL;
	}
	else if(value<=top->value)
		top->lchild=creat(top->lchild,value);
	else
		top->rchild=creat(top->rchild,value);
	return top;
}
int search(BinTree *top,ulong value)
{
	if(NULL==top)
		return 0;
	if(value==top->value)
		return 1;
	if(value<top->value)
		return search(top->lchild,value);
	return search(top->rchild,value);
}
typedef struct
{
	ulong a;
	ulong b;
}hash;
int SaveToBitsArray(int  *&s,ulong ArraySize,hash  *&h,ulong hashNum)
{
	FILE *fp;
	ulong i;
	ulong value;
	s=(int *)malloc(ArraySize*sizeof(int));
	for(i=0;i<ArraySize;i++)
		s[i]=0;
	h=(hash *)malloc(hashNum*sizeof(hash));
	for(i=0;i<hashNum;i++)
	{
		h[i].a=(double)rand()/RAND_MAX*pow(10,7);
		h[i].b=(double)rand()/RAND_MAX*pow(10,7);
	}

	fp=fopen("./data/stream_for_bm.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{
		for(i=0;i<hashNum;i++)
		{
			s[(h[i].a*value+h[i].b)%ArraySize]=1; 
		}
	}
	fclose(fp);
	return 1;
}
ulong BloomFilter(int * s,ulong ArraySize,hash * h,ulong hashNum)
{
	FILE *fp;
	ulong i,sum=0;
	int flag;
	ulong value;
	fp=fopen("./data/stream_for_query.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{
		flag=1;
		for(i=0;i<hashNum;i++)
		{
			if(s[(h[i].a*value+h[i].b)%ArraySize]!=1)
			{
				flag=0;
				break;
			}					
		}
		if(flag==1)
			sum++;
	}
	fclose(fp);
	return sum;
}
ulong getTrueNumInS()
{
	FILE *fp;
	ulong value;
	BinTree *bt=NULL;		
	ulong sum=0;

	fp=fopen("./data/stream_for_bm.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{								
		bt=creat(bt,value);				
	}
	fclose(fp);							

	fp=fopen("./data/stream_for_query.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{								
		if(search(bt,value))
			sum++;
	}
	fclose(fp);						

	return sum;
}
int main()
{
	srand((unsigned)time(NULL));
	ulong ArraySize=50000,hashNum=4,opt_hashNum,insNum,insNum_bf,insNum_bf_opt;
	opt_hashNum=log(2)*(double)ArraySize/10000+0.5;  //printf("***** %ld \n",opt_hashNum);
	int *s;
	hash *h;
	
	insNum=getTrueNumInS();
	printf("insNum_true     %ld \n",insNum);

	SaveToBitsArray(s,ArraySize,h,opt_hashNum);
	insNum_bf_opt=BloomFilter(s,ArraySize,h,opt_hashNum);
	printf("insNum_bf_opt   %ld \n",insNum_bf_opt);

	SaveToBitsArray(s,ArraySize,h,hashNum);
	insNum_bf=BloomFilter(s,ArraySize,h,hashNum);
	printf("insNum_bf   %ld \n",insNum_bf);

	printf("   %.3f \n\n",(double)(insNum_bf-insNum)/insNum);

	


	/*int i;
	for(i=0;i<ArraySize;i++)
	{
		if(s[i]!=1)
		printf("%ld \n",s[i]);
	}
	for(i=0;i<hashNum;i++)
	{
		
		printf("%ld %ld\n",h[i].a,h[i].b);
	}*/
	return 0;
}
