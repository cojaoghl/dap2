#include<stdio.h>	
#include<stdlib.h>	
#include<ctype.h>	

#define SMALL 5

char *input=NULL;	
int size=0;			

int *nums=NULL;		
int numbers=0;		

int read_input(void) {
	int len=0;		
	int cur=0;		
	int c;			
	
	while((c=getchar())!=EOF) {	
		if(cur>=len) {			
			int i,newlen;
			char *buffer;

			newlen=2*len+1;		
			buffer=(char *)malloc(sizeof(char)*newlen);	
			if(!buffer) return -1;	
			for(i=0;i<len;i++) buffer[i]=input[i];	
			if(input) free(input);	
			input=buffer;
			len=newlen;
		}
		input[cur++]=(char)c;	
	}
	return cur;	
}

int convert_to_int(void) {
	int count;	
	int state;	
	int i;

	for(i=state=count=0;i<size;i++) { 
		char c=input[i];	
		if(state==0 && !isspace(c)) { 
			if(isdigit(c)) state=1,count++; else return -1; 
		}
		else if(state==1 && !isdigit(c)) { 
			if(isspace(c)) state=0; else return -1; 
		}
	}

  	if(!count) return 0; 

	nums=(int *)malloc(count*sizeof(int)); 
	if(!nums) return -1; 

	for(i=state=count=0;i<size;i++) { 
		char c=input[i];
		if(state==0 && !isspace(c)) {
			if(isdigit(c)) {
				state=1;
				nums[count++]=c-'0'; 
			}
			else return -1;
		}
		else if(state==1 && !isdigit(c)) {
			if(isspace(c)) state=0; else return -1;
		}
		else if(state==1) {	
			nums[count-1]*=10; 
			nums[count-1]+=c-'0'; 
		}
	}
	return count; 
}

void partition(int *a,int pivot,int *i,int *j,int *k) { /* this is the Dijkstra partition routine separated out */
	int temp;

	while(*j<=*k) {
		if(a[*j]<pivot) {
			temp=a[*i]; a[*i]=a[*j]; a[*j]=temp;
			(*i)++; (*j)++;
		}
		else if(a[*j]>pivot) {
			temp=a[*j]; a[*j]=a[*k]; a[*k]=temp;
			(*k)--;
		}
		else (*j)++;
	}
}

void quicksort(int *a,int s,int t) { /* quicksort calls Dijkstra partitioning */
	int i=s,j=s,k=t,pivot=a[s];

	if(s>=t) return;
	partition(a,pivot,&i,&j,&k);
	quicksort(a,s,i-1);
	quicksort(a,k+1,t);
}

int quantile(int *a,int s,int t,int q) {	/* compute the q-th quantile; q ranges from 1 to t-s+1; q=1 returns the min of a[s],...,a[t] */
	int i,j,k,*m,mm;

	if(s>t||q<0||q>t-s+1) return -1; /* if there is nothing to do, abort and return error code */
	if(t<=s+SMALL) {		/* if there's fewer than six elements, use just quicksort */
		quicksort(a,s,t);
		return a[s+q-1];
	}

	if(!(m=malloc(sizeof(int)*(1+(t-s+1)/5)))) return -1;	/* reserve memory for medians */
	for(i=s,k=0;i<=t;i+=SMALL) {	/* compute the medians of sub-arrays of length (at most) SMALL */
		int j=i+SMALL-1;
		if(j>t) j=t;
		m[k++]=quantile(a,i,j,1+(j-i)/2);
	}
	mm=quantile(m,0,k-1,k/2);	/* recurse to compute the median of medians */
	free(m);	/* array m is no longer needed */

	i=j=s; k=t;
	partition(a,mm,&i,&j,&k);	/* invoke Dijkstra partitioning */
	if(i-s+1>=q) return quantile(a,s,i,q);	/* q is in the smaller part */
	else if(k+1-s<q) return quantile(a,k+1,t,q+s-k-1); /* q is in the greater part */
	else return mm;	/* q happens to be the pivot */
}

void super_quicksort(int *a,int s,int t) { /* expressly pivot on the median */
	int i=s,j=s,k=t,pivot;

	if(s>=t) return;
	pivot=quantile(a,s,t,1+(t-s)/2);
	partition(a,pivot,&i,&j,&k);
	super_quicksort(a,s,i);
	super_quicksort(a,k+1,t);
}

void print_list(void) { /* this function just prints the integers stored in nums */
	int i;

	for(i=0;i<numbers;i++) printf("%d\n",nums[i]);
}

int main(int argc,char **argv) {
	size=read_input();
	if(size<=0) {fputs("Error: could not read input",stderr); return 1;}
	numbers=convert_to_int();
	if(numbers<0) {fputs("Error: no numbers read",stderr); return 1;}

  	printf("%d\n",quantile(nums,0,numbers-1,1+numbers/2));
/*  	super_quicksort(nums,0,numbers-1);
	print_list();*/
	return 0;
}
