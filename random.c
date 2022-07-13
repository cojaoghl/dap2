#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define RAND 0
/*  0=use timer to see random number generator; 1=use /dev/random */

extern struct graph G;

void randSeed(void) { /*  seed random number generator */
	if(!RAND) {time_t t; srand((unsigned) time(&t));}
	else {
		FILE *h=fopen("/dev/random","rb");
		unsigned int a=0; size_t r;

		if(!h) return;
		r=fread(&a,sizeof(unsigned int),1,h); fclose(h);
		if(!r) return;
		srand(a);
		fclose(h);
	}
}

double randUnit(void) {return ((double) rand())/(1.0+RAND_MAX);}
	/* return a random number in [0,1] */

void random_graph(int n,int m) {
	int i,j;
	double p=(2.0*m)/n/(n-1);	/* any two vertices are connected with probability p */

	randSeed();	/* seed the random number generator */
	printf("%d\n",n);	/* print the number of vertices */
	for(i=0;i<n;i++)
		for(j=i+1;j<n;j++)
			if(randUnit()<p) printf("%d %d %lf\n",i,j,randUnit());	/* create the random edges, with random lengths */
}
