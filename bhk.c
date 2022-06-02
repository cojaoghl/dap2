#include<stdio.h>	/* include commands for reading/writing to standard input/output */
#include<stdlib.h>	/* standard library */
#include<ctype.h>	/* functions such as isspace, isdigit for processing input */
#include<float.h>	/* mathematical functions */
#include"bhk.h"		/* custom declarations */


int n=0;	/* number of cities */
dist_t *dist=NULL;	/* pointer to the distance values */
int T_size=0; /* size of the dynamic programming table */
dist_t *T=NULL; /* dynamic programming table */

extern int read_distances(void); /* process input, read cities and compute distances */

dist_t get_distance(int i,int j) { /* look up distance of cities i and j */
	return dist[i*n+j];
}

/* we represent a subset U of the vertex set [n] by the corresponding bit vector, with city i corresponding to the 1<<i bit */
/* with cities numbered 0,...,n-1, the start city s is always city n-1 */

dist_t *T_entry(int u,int U) { /* return pointer to table entry of pair (u,U); assume 0<=u<n-1 and U<(1<<(n-1)) */
	return T+u+U*(n-1);
}

void iter_subsets(int m,int k,int u,int U) { /* generate k-subsets of 0,...,m that do not contain u with elements m+1,...,n prescribed by U */
	if(k>m+1 || m+1<0 || k<0) return; /* if no subsets exist, cancel */
	if(k==m+1) {	/* if all remaining elements are required */
		U|=(1<<m+1)-1;	/* set them to one in U */
		if(!(U&1<<u)) {	/* check if the forbidden element is contained; if not, perform action */
			int v;

			for(v=0;v<n-1;v++) /* iterate on v */
				if(U&1<<v) { /* if v is in U */
					dist_t val=*T_entry(v,U&~(1<<v));	/* look up T(v,U\{v}) */

					val+=get_distance(v,u); /* add the distance from v to u */
					if(*T_entry(u,U)>val) *T_entry(u,U)=val; /* update min value if val is smaller than the current value */
				}
		}
		return;
	}
	if(u!=m) iter_subsets(m-1,k-1,u,U|1<<m);	/* recurse on smaller subsets as in Pascal's triangle */
	iter_subsets(m-1,k,u,U);
}

dist_t bhk(void) { /* the BHK algorithm; returns min TSP length */
	dist_t opt=DIST_MAX;
	int N,u;

	T_size=(n-1)*(1<<(n-1)); /* set the size of the dynamic programming table */
	if(!(T=malloc(sizeof(dist_t)*T_size))) {fputs("Error: out of memory!",stderr); return 0;} /* return error if out of memory */
	for(N=0;N<T_size;N++) T[N]=DIST_MAX; /* set all entries in T to +oo */
	for(N=0;N<n-1;N++) *T_entry(N,0)=get_distance(N,n-1); /* initialise the singleton entries of T */

	for(N=1;N<n-1;N++)  /* for all sets of sizes i=1,...,n-2 */
		for(u=0;u<n-1;u++)  /* for all vertices other than s=n-1 */
			iter_subsets(n-2,N,u,0);	/* iterate over the respective subsets */
	for(u=0;u<n-1;u++) { /* final minimisation */
		dist_t val=*T_entry(u,((1<<(n-1))-1)&~(1<<u));	/* look up T(v,U\{v}) */
		val+=get_distance(u,n-1);
		if(val<opt) opt=val;
	}
	return opt;	
}

int main(int argc,char **argv) {
	dist_t opt;

	if(read_distances()) return 1; /* attempt reading input; in the event of an error, abort */
	if(n>MAX_CITIES) {fputs("Error: too many cities!",stderr); return 1;}

/*    	print_cities(); print_distances();*/
	opt=bhk();
	if(opt>0) printf("%d\n",(int)opt);
	
	return 0; /* report "success" */
}
