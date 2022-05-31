#include<stdio.h>	/* include commands for reading/writing to standard input/output */
#include<stdlib.h>	/* standard library */
#include<ctype.h>	/* functions such as isspace, isdigit for processing input */

char *input=NULL;	/* pointer to input (text) */

struct vertex {	/* data structure to represent a vertex */
	struct entry *first;	/* pointer to the first entry of the adjacency list */
	int comp;	/* id of the component of the vertex */
	enum colour {green,yellow,red} col; /* color: green, yellow, red */
	struct vertex *p; /* p-field for DFS */
};

struct entry {	/* entry of the adjacency list */
	struct vertex *v;		/* pointer to the adjacent vertex  */
	struct entry *next;		/* pointer to the next entry */
};

struct graph {
	int n;	/* number of vertices */
	struct vertex *v;	/* pointer to the vertices */
};

struct graph G;	/* the input graph */

int vertex_number(struct vertex *u) {return u?u-G.v:-1;} /* return the number of a vertex; ranges from 0 to n-1 */

int read_input(void) {	/* this function reads standard input and stores the contents under the input pointer */
	int len=0;		/* len is the size of memory block */
	int cur=0;		/* cur is the number of memory cells currently occupied */
	int c;			/* variable to hold the next character read */
	
	while((c=getchar())!=EOF) {	/* read the next character while there is one */
		if(cur>=len-1) {			/* enlarge the memory block if necessary */
			int i,newlen;
			char *buffer;

			newlen=2*(len+1);		/* size of the enlarged memory block */
			buffer=(char *)malloc(sizeof(char)*newlen);	/* allocate memory */
			if(!buffer) return -1;	/* if not enough memory available, report error */
			for(i=0;i<len;i++) buffer[i]=input[i];	/* copy old contents over to new memory block */
			if(input) free(input);	/* free the old memory block if necessary */
			input=buffer;
			len=newlen;
		}
		input[cur++]=(char)c;	/* store the input character */
	}
	input[cur]=0; /* store a zero character to mark the end of file */
	return cur;	/* return the number of characters read */
}

char *convert_to_int(char *str,int *num) {	/* convert the characters under str to an integer, return the position after the chars processed; in case of an error, return NULL */
	*num=0;	/* number of integers read */

	while(isdigit(*str)) { /* so long as we're reading digits */
		(*num)*=10;	/* shift current value one decimal digit to the left */
		(*num)+=*str-'0';	/* add value of the current digit */
		str++;
	}

	if(isspace(*str) || *str==0) return str; /* if the next char is space or EOF, return the pointer to the next unprocessed symbol  */
	return NULL;	/* otherwise return "error" */
}

/* input file format: first line contains the number of vertices */
/* the following lines contain the edges in the format v1 v2 */
/* here v1,v2 are vertex numbers 0,...,n-1 */

char *skip_whitespace(char *str) { /* skip over non-newline whitespace */
	while(*str && isspace(*str)) str++;
	return str;
}

int read_graph(char *str) { /* read graph from str; returns -1 in event of error, and 0 otherwise */
	int i,m;
	char *tmp;
	struct entry *edges;

	/* determine the number of vertices and allocate enough space */
	str=skip_whitespace(str);	/* skip over any leading whitespace */
	if((str=convert_to_int(str,&G.n))==NULL) return -1; /* read number of vertices; report error unless correct number read */
	G.v=malloc(sizeof(struct vertex)*G.n); /* allocate memory for the vertices */
	if(G.v==NULL) return -1; /* if there's not enough memory available, return an error */
	for(i=0;i<G.n;i++) G.v[i].first=NULL; /* initialise adjacency list pointers to NULL */

	/* now count the number of remaining lines of the input file, which equals the number of edges */
	for(tmp=str,m=0;*tmp;tmp++) if(*tmp=='\n') m++;

	edges=malloc(2*(1+m)*sizeof(struct entry));	/* allocate memory for the edges */
	if(edges==NULL) return -1; /* if we're out of memory, return error code */
	if(m==0) return 0; /* if there's no edges to be read, we're done */
	for(m=0;*str;m+=2) { /* now read the edges one by one */
		int v,w;
		
		str=skip_whitespace(str); /* skip over any leading whitespace */
		if(!*str) break; /* if we're at the end of the file, stop */
		if((str=convert_to_int(str,&v))==NULL) return -1; /* read number of the first vertex; abort in case of syntax error */
		str=skip_whitespace(str); /* skip over any whitespace */
		if((str=convert_to_int(str,&w))==NULL) return -1; /* read number of the second vertex; abort in case of syntax error */
		str=skip_whitespace(str); /* skip over any whitespace */
		if(v<0||v>=G.n) return -1; /* if the first vertex number doesn't make sense, abort */
		if(w<0||w>=G.n||w==v) return -1; /* if the second vertex number doesn't make sense, abort */

		edges[m].v=G.v+w; /* set the vertex that the edge points to */
		edges[m+1].v=G.v+v; /* set the vertex that the next edge points to */
		edges[m].next=G.v[v].first; /* insert the edge at the begining of the adjacency list of v */
		G.v[v].first=edges+m;
		edges[m+1].next=G.v[w].first; /* insert the edge at the begining of the adjacency list of w */
		G.v[w].first=edges+m+1;
	}
	return 0;	/*  report success */
}

void print_graph(void) { /* print the graph in the form of an adjacency list */
	struct entry *next;
	int i,*comps,total=0;

	if(!(comps=calloc(G.n,sizeof(int)))) return; /* reservere memory for the component size counters */

	printf("n=%d\n",G.n); /* print the total number of vertices */
	for(i=0;i<G.n;i++) {
		comps[G.v[i].comp]++; /* increase the respecitive component counter */
		printf("v%d, c%d, p%d: ",i,G.v[i].comp,vertex_number(G.v[i].p));	/*  for each vertex print its number and the id of its component */
		for(next=G.v[i].first;next;next=next->next)	/* iterate over the adjacency list */
			printf("%d ",vertex_number(next->v));	/* print the number of the neighbour */
		puts("");	/* output a newline */
	}
	puts("Component sizes");	/* write out component sizes */
	for(i=0;i<G.n;i++)
		if(comps[i]) {	/* online write out non-empty components */
			printf("c%d: %d\n",i,comps[i]);
			total++;
		}
	printf("%d\n",total); /* print total number of components */
}

void dfs_loop(struct vertex *v,int j) { /* expects a pointer to a vertex of G and the number of a component j=0,...,n-1 */
	struct entry *u;

	v->col=yellow; /*  set vertex colour to yellow */
	v->comp=j; /* set component number to j */
	for(u=v->first;u;u=u->next) { /* iterate over adj list of v */
		if(u->v->col==green) { /* if current neighbour green  */
			u->v->p=v;	/* set parent pointer to v */
			dfs_loop(u->v,j); /* recurse on u */
		}
	}
	v->col=red;	/* colour v red */
}

void dfs(void) { /* run dfs on G */
	int i,j;

	for(i=0;i<G.n;i++) G.v[i].comp=-1,G.v[i].col=green,G.v[i].p=NULL; /*  initialise component numbers, colours, parents */
	for(i=j=0;i<G.n;i++)	/* iterate over all vertices */
		if(G.v[i].col==green)	/* if green, then call dfs_loop */
			dfs_loop(G.v+i,j++);	/* increase component counter */
}

int main(int argc,char **argv) {
	if(read_input()<=0) {fputs("Error: could not read input",stderr); return 1;} /* if reading the input fails, report error and abort */
	if(read_graph(input)<0) {fputs("Error: graph syntax not correct",stderr); return 1;} /* if reading the graph fails, report error and abort */
	dfs();
	print_graph();
	
	return 0; /* report "success" */
}
