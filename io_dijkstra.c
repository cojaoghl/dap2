#include<float.h>
#include<stdio.h>
#include<stdlib.h>
#include"dijkstra.h"

extern struct graph G;		/* the graph */

/* this function reads the input from stdin */
/* the first line of the input file details the number of vertices */
/* each of the following lines contains an edge represented by the ids of two vertices in the range 0,...,n-1 */
/* followed by a double, which is the non-negative length of the edge */

int read_input(void) {	
	int i,x,y;
	double len;

	if(scanf("%d",&G.n)!=1) return 1;	/* read the number of vertices */
	if(G.n<1) return 1; /* abort if fewer than one vertex */
	if(!(G.v=malloc(sizeof(struct vertex)*G.n))) return 1;	/* allocate vertices; abort if out of memory */
	for(i=0;i<G.n;i++) G.v[i].id=i,G.v[i].adj=NULL,G.v[i].key=INFTY;	/* initialise the ids, adjacency lists and key values */
	G.v[0].key=0;

	while(scanf("%d %d %lf",&x,&y,&len)==3) { /* so long as input is available, repeat the following */
		struct edge *e;

		if(x<0||x>=G.n||y<0||y>=G.n||x==y||len<0.0) return 1; /* abort on nonsensical input */

		if(!(e=malloc(sizeof(struct edge)))) return 1; /* allocate memory for an edge */
		e->len=len; /* set the length of e */
		e->v=G.v+y;	/* let e point to vertex y */
		e->next=G.v[x].adj;	/* insert e into the adjacency list of x */
		G.v[x].adj=e;

		if(!(e=malloc(sizeof(struct edge)))) return 1; /* allocate memory for another edge */
		e->len=len; /* set the length of e */
		e->v=G.v+x;	/* let e point to vertex x */
		e->next=G.v[y].adj;	/* insert e into the adjacency list of y */
		G.v[y].adj=e;
	}
	return 0;
}


void print_dist(void) { /* print the key values of all the vertices */
	int i;

	for(i=0;i<G.n;i++) 
		if(G.v[i].key==INFTY) printf("%4d: oo\n",G.v[i].id);
		else printf("%4d: %lf\n",G.v[i].id,G.v[i].key);
}

void print_graph(void) { /* dump the graph to stdout */
	int i;
	struct edge *e;

	for(i=0;i<G.n;i++) {
		printf("v%d\n",G.v[i].id);
		for(e=G.v[i].adj;e;e=e->next) {
			printf("\t%d %lf\n",e->v->id,e->len);
		}
	}
}

void print_node(struct node *next) {
	if(!next) return;
	if(next->parent) printf("(%d,%d),",next->v->id,next->parent->v->id);
	if(next->child) printf("(%d,%d),",next->v->id,next->child->v->id);
	if(next->next) printf("(%d,%d),",next->v->id,next->next->v->id);
	if(next->pre) printf("(%d,%d),",next->v->id,next->pre->v->id);
	print_node(next->child);
	print_node(next->next);
}

//void print_heap(struct node *h) {
//	printf("G=DiGraph([");
//	print_node(h);
//	printf("])\nG.show()\n");
//}

void print_heap(struct node *h) {
	if(!h) return;
	printf("%d: parent=%d child=%d pre=%d next=%d\n",h->v->id,
				h->parent?h->parent->v->id:-1,
				h->child?h->child->v->id:-1,
				h->pre?h->pre->v->id:-1,
				h->next?h->next->v->id:-1);
	print_heap(h->child);
	print_heap(h->next);
}
