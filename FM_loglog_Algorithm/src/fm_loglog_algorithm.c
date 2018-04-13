#include "fm_loglog_algorithm/fm_loglog_algorithm.h"



#include"stdio.h"
#include"stdlib.h"
#include"math.h"
#include"time.h"
#define ulong unsigned long
#define k 32768		//32768-16384-8196-4096-2048
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
ulong BinTreeNodeNum(BinTree *top)
{
	if(NULL==top)
		return 0;
	return 1+BinTreeNodeNum(top->lchild)+BinTreeNodeNum(top->rchild);
}
ulong TrueAnswer()
{
	FILE *fp;
	ulong value;
	BinTree *top=NULL;
	fp=fopen("./data/stream_for_fm.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{
		if(search(top,value))
			continue;
		else
			top=creat(top,value);
	}
	fclose(fp);
	return BinTreeNodeNum(top);
}
int CountTail(ulong hashValue)
{
	ulong temp=hashValue;
	int count=0;
	while(temp!=0)
	{
		if(temp%2==0)
			count++;
		else
			break;
		temp=temp/2;
	}
	return count;
}
int FMAlgorithm()
{
	ulong a,b,value,hashValue;	
	const ulong N=pow(2,25);
	int r=0,t;
	FILE *fp;
	a=(double)rand()/RAND_MAX*N; 
	b=(double)rand()/RAND_MAX*N; 
	fp=fopen("./data/stream_for_fm.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{	
		hashValue=(a*value+b)%N;	
		t=CountTail(hashValue);
		if(t>r)
			r=t;
	}
	fclose(fp);
	return r;
}
double CombiningEstimates_FM(int m,int l )
{
	int i,j;
	double temp;
	double *R=(double *)malloc(m*sizeof(double));
	//Group average
	for(i=0;i<m;i++)
	{
		R[i]=0;
		for(j=0;j<l;j++)
		{
			R[i]+=(double)FMAlgorithm();
		}
		R[i]=R[i]/l;
	}	
	//median
	for(i=0;i<m-1;i++)
		for(j=i+1;j<m;j++)
		{
			if(R[j]<R[i])
			{
				temp=R[i];
				R[i]=R[j];
				R[j]=temp;
			}
		}
	if(m%2==0)
		return (R[m/2-1]+R[m/2])/2;
	else
		return R[m/2];
}
double AverageDeviation_FM(ulong x,int n,int m,int l)
{
	int64_t sum=0,s=0;
	ulong *p=(ulong *)malloc(n*sizeof(ulong));
	int i;
	for(i=0;i<n;i++)
	{
		p[i]=(ulong)pow((double)2,CombiningEstimates_FM(m,l));
		printf("Combining Estimates number of unique elements (FM) : %ld\n",p[i]);
		if(p[i]>x)
			s=(int64_t)(p[i]-x);
		else
			s=(int64_t)(x-p[i]);
		sum+=s*s/n;
	}
	//sum=sum/n;
	return sqrt(sum);
}
int loglogAlgorithm()
{
	int *p=(int *)malloc(k*sizeof(int));
	ulong a,b,value,hashValue;
	ulong sig;
	int id,t=0,i,s=0;
	const ulong N=pow(2,25);
	FILE *fp;
	
	for(i=0;i<k;i++)
		p[i]=0;
	a=1+(double)rand()/RAND_MAX*N;
	b=1+(double)rand()/RAND_MAX*N;
	fp=fopen("./data/stream_for_fm.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{
		hashValue=(a*value+b)%N;	
		id=hashValue%k;
		sig=hashValue/k;
		t=CountTail(sig)+1;
		if(t>p[id])
			p[id]=t;
	}
	fclose(fp);
	for(i=0;i<k;i++)
		s+=p[i];
	s=s/k;
	return s;
}
double CombiningEstimates_loglog(int m )
{
	int i,j;
	double temp;
	double *s=(double *)malloc(m*sizeof(double));
	for(i=0;i<m;i++)		
		s[i]=loglogAlgorithm();	
	//median
	for(i=0;i<m-1;i++)
		for(j=i+1;j<m;j++)
		{
			if(s[j]<s[i])
			{
				temp=s[i];
				s[i]=s[j];
				s[j]=temp;
			}
		}
	if(m%2==0)
		return (s[m/2-1]+s[m/2])/2;
	else
		return s[m/2];
}
double AverageDeviation_loglog(ulong x,int n,int m )
{
	int64_t sum=0,s=0;
	ulong *p=(ulong *)malloc(n*sizeof(ulong));
	int i;
	for(i=0;i<n;i++)
	{
		p[i]=0.39701*k*pow(2,CombiningEstimates_loglog(m));
		printf("Combining Estimates number of unique elements (loglog) : %ld\n",p[i]);
		if(p[i]>x)
			s=(int64_t)(p[i]-x);
		else
			s=(int64_t)(x-p[i]);
		sum+=s*s/n;
	}
	//sum=sum/n;
	return sqrt(sum);
}
int main()
{
	srand((unsigned)time(NULL));
	ulong trueanswer=TrueAnswer();
	int m=4,l=4,n=20;
	printf("True number of unique elements : %ld\n\n",trueanswer);
	printf("Standard Deviation (  FM  ) : %.3f\n\n",AverageDeviation_FM(trueanswer,n,m,l));
	printf("Standard Deviation (loglog) : %.3f\n\n",AverageDeviation_loglog(trueanswer,n,m));
	getchar();
	return 0;
}
