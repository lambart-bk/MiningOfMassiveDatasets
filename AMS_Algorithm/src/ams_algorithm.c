#include "ams_algorithm/ams_algorithm.h"


#include"stdio.h"
#include"stdlib.h"
#include"math.h"
#include"time.h"
#define ulong unsigned long
ulong N;
typedef struct 
{
	ulong value;
	ulong element;
}variable;
typedef struct bnode
{
	ulong value;
	ulong times;
	struct bnode *lchild,*rchild;
}BinTree;
BinTree *creat(BinTree *top,ulong value)
{
	if(NULL==top)
	{
		top=(BinTree *)malloc(sizeof(BinTree));
		top->value=value;
		top->times=1;
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
	{
		top->times+=1;
		return 1;
	}
	if(value<top->value)
		return search(top->lchild,value);
	return search(top->rchild,value);
}
int64_t BinTreeNodetimes(BinTree *top)
{
	if(NULL==top)
		return 0;
	return top->times*top->times+BinTreeNodetimes(top->lchild)+BinTreeNodetimes(top->rchild);
}
int64_t TrueAnswer(ulong *n)
{
	FILE *fp;
	ulong value,i=0;
	BinTree *top=NULL;
	fp=fopen("./data/stream_for_ams.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{
		i++;
		if(search(top,value))
			continue;
		else
			top=creat(top,value);
	}
	fclose(fp);
	*n=i;
	return BinTreeNodetimes(top);
}
int64_t AMS_Algorithm()
{
	FILE *fp;
	variable var;
	ulong value;
	ulong pos,i=0;
	fp=fopen("./data/stream_for_ams.txt","r");
	pos=((double)rand())/RAND_MAX*N+1;	//printf("%ld  \n",pos);
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{
		i++;
		if(i<pos)
			continue;
		else if(i==pos)
		{
			var.element=value;
			var.value=1;   
		}
		else
		{
			if(value==var.element)
				var.value++;
		}
	}
	fclose(fp);		//printf("%ld  \n",N*(2*var.value-1));
	return N*(2*var.value-1);
}
int64_t CombiningEstimates_AMS(int m,int l)
{
	int i,j;
	int64_t temp;
	int64_t *R=(int64_t *)malloc(m*sizeof(int64_t));
	//Group average
	for(i=0;i<m;i++)
	{
		R[i]=0;
		for(j=0;j<l;j++)
		{
			R[i]+=AMS_Algorithm();	
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
double StandardDeviation(int64_t x,int n,int m,int l)
{
	int64_t sum=0,s=0;
	int64_t *p=(int64_t *)malloc(n*sizeof(int64_t));
	int i;
	for(i=0;i<n;i++)
	{
		p[i]=CombiningEstimates_AMS(m,l);
		printf("Combining Estimates   : %lld\n",p[i]);
		if(p[i]>x)
			s=(p[i]-x);
		else
			s=(x-p[i]);
		sum+=s*s/n;
	}
	//sum=sum/n;
	return sqrt(sum);
}
int prob(ulong a,ulong b)
{
	double p=(double)a/b,r;
	r=(double)rand()/RAND_MAX;
	if(r<p)
		return 1;
	return 0;
}
int64_t StreamNeverEnd_AMS(ulong samplesize,ulong times)
{
	FILE *fp;
	ulong value,t=0;
	int64_t sum=0;
	variable *sample=(variable *)malloc(samplesize*sizeof(variable));
	ulong i,temp;
	for(i=0;i<samplesize;i++)
		sample[i].value=-1;
	fp=fopen("./data/stream_for_ams.txt","r");
	while(fscanf(fp,"%ld\n",&value)!=EOF)
	{
		t++;
		for(i=0;i<samplesize;i++)
		{
			if(-1!=sample[i].value)
			{
				if(value==sample[i].element)
					sample[i].value++;
			}	
		}
		if(t<=samplesize)
		{
			sample[t-1].element=value;
			sample[t-1].value=1;
		}
		else 
		{
			if(prob(samplesize,t))
			{
				temp=(double)rand()/RAND_MAX*samplesize;
				sample[temp].element=value;
				sample[temp].value=1;
			}
		}
		if(t==times)
		{
			temp=0;
			for(i=0;i<samplesize;i++)
			{
				if(sample[i].value==-1)
					continue;
				sum+=t*(2*sample[i].value-1);	//printf("%ld\t",2*sample[i].value-1);
				temp++;							
			}
			sum/=temp;							printf("%ld\t",temp);
			return sum;
		}
	}
	fclose(fp);	
	
	return -99;
}
int main()
{
	srand((unsigned)time(NULL));
	int m=10,l=10,n=20;
	ulong time;
	int64_t trueanswer=TrueAnswer(&N);	//printf("%ld  \t",N);

	printf("trueanswer  %lld  \n",trueanswer);

	printf("StandardDeviation   %.3f  \n",StandardDeviation(trueanswer,n,m,l));
	while(1)
	{
		printf("Input times :\t");
		scanf("%ld",&time);
		printf("StreamNeverEnd_AMS: %lld\n",StreamNeverEnd_AMS(100,time));
	}
	getchar();
	return 0;
}