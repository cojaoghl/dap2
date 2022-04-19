#include<stdio.h>	/* include commands for reading/writing to standard input/output */
#include<stdlib.h>	/* standard library */
#include<ctype.h>	/* functions such as isspace, isdigit for processing input */

char *input=NULL;	/* pointer to input (text) */
int size=0;			/* number of charcters read as input */

int *nums=NULL;		/* pointer to array (converted to numbers) */
int numbers=0;		/* number of entries in nums */

int read_input(void) {
	int len=0;		/* len is the size of memory block */
	int cur=0;		/* cur is the number of memory cells currently occupied */
	int c;			/* variable to hold the next character read */
	
	while((c=getchar())!=EOF) {	/* read the next character while there is one */
		if(cur>=len) {			/* enlarge the memory block if necessary */
			int i,newlen;
			char *buffer;

			newlen=2*len+1;		/* size of the enlarged memory block */
			buffer=(char *)malloc(sizeof(char)*newlen);	/* allocate memory */
			if(!buffer) return -1;	/* if not enough memory available, report error */
			for(i=0;i<len;i++) buffer[i]=input[i];	/* copy old contents over to new memory block */
			if(input) free(input);	/* free the old memory block if necessary */
			input=buffer;
			len=newlen;
		}
		input[cur++]=(char)c;	/* store the input character */
	}
	return cur;	/* return the number of characters read */
}

int convert_to_int(void) {
	int count;	/* number of integers read */
	int state;	/* stated: 0 means "reading whitespace"; 1 means "reading digits" */
	int i;

	/* the purpose of the following lines is to determine the number of integers contained in the input */
	for(i=state=count=0;i<size;i++) { /* pass over input file */
		char c=input[i];	/* c=next character */
		if(state==0 && !isspace(c)) { /* if state is zero but c is not a whitespace character */
			if(isdigit(c)) state=1,count++; else return -1; /* check if c is a digit; then set state to one and increase counter; otherwise return "error" and abort */
		}
		else if(state==1 && !isdigit(c)) { /* if state is one but c is not a digit */
			if(isspace(c)) state=0; else return -1; /* check if c is a whitespace; in that case, set state to zero; otherwise, abort and report error */
		}
	}

  	if(!count) return 0; /* if there are nu numbers to be read, abort */

	nums=(int *)malloc(count*sizeof(int)); /* otherwise reserve memory */
	if(!nums) return -1; /* if not enough memory available, abort */

	/* now we pass over the input again; this time we convert the text to integers */
	/* the mechanics of the state variable is just as before */
	for(i=state=count=0;i<size;i++) { 
		char c=input[i];
		if(state==0 && !isspace(c)) {
			if(isdigit(c)) {
				state=1;
				nums[count++]=c-'0'; /* set the number to the character code of c minus the code for '0' and increase the counter */
			}
			else return -1;
		}
		else if(state==1 && !isdigit(c)) {
			if(isspace(c)) state=0; else return -1;
		}
		else if(state==1) {	/* if we're reading the next integer */
			nums[count-1]*=10; /* then shift the number left by one decimal digit */
			nums[count-1]+=c-'0'; /* and add c minus the code for '0' as the lest significant decimal digit */
		}
	}
	return count; /* return the number of integers read */
}

void print_list(void) { /* this function just prints the integers stored in nums */
	int i;

	for(i=0;i<numbers;i++) printf("%d\n",nums[i]);
}

void quicksort(int *a,int s,int t) { /*  sort a[s],...,a[t] */
	int i=s,j=s,k=t,temp,pivot=a[s];

	/* this is finally quicksort proper */
	/* we use the Dijkstra partitioning method */
	if(s>=t) return;
	while(j<=k) {
		if(a[j]<pivot) {
			temp=a[i]; a[i]=a[j]; a[j]=temp;
			i++; j++;
		}
		else if(a[j]>pivot) {
			temp=a[j]; a[j]=a[k]; a[k]=temp;
			k--;
		}
		else j++;
	}
	quicksort(a,s,i); /* recurse on the smaller/greater sub-lists */
	quicksort(a,k+1,t);
}

int main(int argc,char **argv) {
	size=read_input();	/* attempt to read input */
	if(size<=0) {fputs("Error: could not read input",stderr); return 1;} /* if reading the input fails, report error and abort */
	numbers=convert_to_int();	/* attempt conversion to integers */
	if(numbers<0) {fputs("Error: no numbers read",stderr); return 1;} /* if an error is reported back, abort */

	quicksort(nums,0,numbers-1); /* perform quicksort on nums */
	print_list();	/* output the sorted list */

	return 0; /* report "success" */
}
