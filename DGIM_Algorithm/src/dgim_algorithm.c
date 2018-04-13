#include "dgim_algorithm/dgim_algorithm.h"

#include"stdio.h"
#include"stdlib.h"
#include"time.h"

//#include"windows.h"
//for linux 
typedef unsigned long DWORD;
unsigned long GetTickCount()  
{  
    struct timespec ts;  
  
    clock_gettime(CLOCK_MONOTONIC, &ts);  
  
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);  
}  
//bucket struct
typedef struct bkt
{
	int timestamp;
	int size;
	struct bkt *next;
}bucket;

bucket *Add(bucket *head,int time,int size)
{
	bucket *p;
	p=(bucket *)malloc(sizeof(bucket));
	p->next=NULL;
	p->timestamp=time;
	p->size=size;
	if(head==NULL)
		head=p;
	else
	{
		p->next=head;
		head=p;
	}
	return head;
}

bucket *UpdateWindow(bucket *head,int curTime,int WindowSize)
{
	bucket *p,*q;
	p=q=head;
	while(p->next!=NULL)
	{
		q=p;
		p=p->next;
	}
	if((curTime-p->timestamp+1)>WindowSize)
	{
		q->next=NULL;
		free(p);
	}
	return head;
}

bucket *UpdateBucket(bucket *head,int *flag,int r)
{
	bucket *t,*q,*p;
	t=head;
	int count;
	while(t->next!=NULL)
	{
		q=p=t;
		count=0;
		while(p->next!=NULL)
		{
			q=p;
			p=p->next;
			if(q->size==p->size)
			{
				count++;
				if(count>=r)
				{
					q->size+=p->size;
					q->next=p->next;
					free(p);
					*flag=1;
					return head;
				}
			}
			else
				break;
		}
		t=t->next;
	}
	*flag=0;
	return head;
	/*bucket *p,*q,*t=NULL;
	p=q=head;
	while(p->next!=NULL)
	{
		q=p->next;
		if((p->size==q->size)&&(q->next!=NULL))
		{
			t=q->next;
			if(q->size==t->size)
			{
				q->size+=t->size;
				q->next=t->next;			
				free(t);
				*flag=1;//head=UpdateBucket(head);
				return head;
			}
		}
		p=p->next;
	}
	*flag=0;
	return head;*/
}
//output bucket
void print(bucket *head)
{
	bucket *p;
	p=head;
	while(p!=NULL)
	{
		printf("Size: %5d\tTimeStamp: %10d\n",p->size,p->timestamp);
		p=p->next;
	}
}
int Search(bucket *head,double error,int searchtime) 
{
	bucket *p=head;
	int count=0;
	while(p->next!=NULL)
	{
		count+=p->size;
		p=p->next;
	}
	count+=(int )p->size*error;
	return count;
}
//groundtruth
void TrueAnswer(unsigned long searchtime ,int N)
{
	unsigned long t=0;
	int i,count=0;
	char *p,value;
	DWORD start,end;
	FILE *fp=fopen("./data/01stream.txt","r");
	t=0;
	count=0;
	i=0;
	p=(char *)malloc(N*sizeof(char));
	start=GetTickCount();
	while(fscanf(fp,"%c",&value)!=EOF)
	{
		if(value!='0'&&value!='1')
			continue;		
		t++;
		p[i]=value;		
		i=(++i)%N;
		if(t==searchtime)
		{			
			for(i=0;i<N;i++)
			{
				if(p[i]=='1')
					count++;
			}
			printf("\n\nTrue Answer is %d\n",count);
			end=GetTickCount();
			printf("\nTotalTime(True): %ld ms\n",end-start);
			break;
		}
	}
	fclose(fp);

}
int main(int argc,char**argv)
{
  /*char *a=(char*)malloc(5*sizeof(char));
  char b[5]={'1','2','3','4','5'};int i=0;
  for(i=0;i<5;i++)
  {
    a[i]=b[i];
    printf("%c%c ",a[0],a[0]);
  }*/
  

	FILE *fp=fopen("./data/01stream.txt","r");
	char value;
	int N=1000;
	unsigned long t=0,searchtime;
	double error=0.5;
	int flag;
	bucket *head=NULL;
	DWORD start,end;
	//int i,count;
	char *p=NULL;
//	time_t rawtime_s,rawtime_e;
//	struct tm *timeinfo;


	printf("Please Input a search time :\t");
	scanf("%ld",&searchtime);

	//////////////DGIM
	start=GetTickCount();		//time(&rawtime_s);
	while(fscanf(fp,"%c",&value)!=EOF)
	{
		if(value!='0'&&value!='1')
			continue;
		t++;
		flag=1;
		if(value=='1')
		{
			head=Add(head,t,1);
			//print(head);
			head=UpdateWindow(head,t,N);
			//print(head);
			while(flag)
				head=UpdateBucket(head,&flag,2);//10%ʱȡ10,
		}
		if(t==searchtime)
		{
			printf("\nCurrent Window has %d 1-bit\n",Search(head,error,searchtime));
			end=GetTickCount();
			printf("\nTotalTime(DGIM): %ld ms\n",end-start);
			break;
			/*time(&rawtime_e);
			printf("start time:  %s\n",asctime(localtime(&rawtime_s)));
			printf("end time:  %s\n",asctime(localtime(&rawtime_e)));		
			printf("total time:  %.0f\n",difftime(rawtime_e,rawtime_s));*/
		}
	}
	fclose(fp);
	//print(head);


	TrueAnswer(searchtime,N);

	return 0;
}
