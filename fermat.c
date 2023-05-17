#include<stdio.h>	
#include<stdlib.h>
#include<ctype.h>
#include<math.h>

long fermat(long n) {
	long x,y,r;

	r=(long)sqrt(n);
	x=2*r+1;
	y=1;
	r*=r;
	r-=n;

	while(r!=0) {
		r+=x;
		x+=2;
		do {
			r-=y;
			y+=2;
		} while(r>0);
	}
	return (x-y)/2;
}

int main(int argc,char **argv) {
	long n;

	if(argc<2) return -1;
	n=atol(argv[1]);	
	if(n<=1) return -1;
	
	n=fermat(n);
	if(n<=0) return -1;
	printf("%ld\n",n);

	return 0;
}
