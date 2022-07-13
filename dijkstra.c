#include<float.h>
#include<stdio.h>
#include<stdlib.h>
#include"dijkstra.h"

struct graph G;		/* the graph */
struct node *heap=NULL; /* the binomial heap */

struct node *merge(struct node *a,struct node *b) {  /* merge the heaps a and b; the result will be a, while b will be destroyed */
	struct node *head=NULL,*cur=NULL;

	while(a||b) { /* so long as a or b is non-empty */
		struct node **n=(!a||(b&&b->order<a->order))?&b:&a,*next;	/* do we need to advance a or b? */

		next=(*n)->next;	/* make a note of the place where we need to advance *n */
		(*n)->next=NULL;	/* append *n to cur */
		if(!cur) {head=cur=*n; (*n)->pre=NULL;} /* are we right at the beginning? */
		else {(*n)->pre=cur; cur=cur->next=*n;} /* otherwise append *n after cur */
		*n=next;	/* advance *n */
	}
	return head;
}

struct node *unite(struct node *a,struct node *b) {  /* add the heap b to the heap a; b will cease to be valid */
	struct node *p,*c;

	a=b=merge(a,b);	/* merga a and b; b is going to store the return pointer */
	while(a&&a->next) {/* walk across a; during the union process, THREE trees of the same order may occur */
		if(a->order==a->next->order) {/* take action if we see the same order twice */
			if(a->next->next&&a->next->next->order==a->order) a=a->next; /* if we see the same order thrice, advance a one step*/

			/* now add a->next as a child to a, or vice versa, depending on key values */
			if(a->v->key<a->next->v->key) { /* p is the parent-to-be, c the child */
				p=a;	/* if a's key value is smaller, a is going to be parent of a->next */
				c=a->next;	
				p->next=c->next; /* if the parent is a, we need to update its next pointer  */
				if(p->next) p->next->pre=p; /* as well as the pre pointer of c->next */
			} else {
				p=a->next;	/* otherwise a->next is going to be the parent of a */
				c=a;
				p->pre=c->pre; /* as the parent is a->next, we need to update its pre pointer  */
				if(p->pre) p->pre->next=p; /* as well as the next pointer of p->pre */
				else b=p; /* update the pointer to the united heap if necessary */
			} 
			c->parent=p; /* append c as a the leftmost child to p */
			c->pre=NULL;	/* c is the first child */
			c->next=p->child;	/* insert c at the head of the list of children of p */
			if(p->child) p->child->pre=c;
			p->child=c;	/* update p's child pointer */
			p->order++; /* increase the order of the parent */
			a=p;	/* advance a to the new united tree p */
		}
		else a=a->next;	/* otherwise just move on */
	}
	return b; /* b is utilised to memorise the pointer to the union */
}

struct node *insert(struct vertex *v) { /* insert a vertex into the heap */
	struct node *new=malloc(sizeof(struct node));

	if(!new) return NULL; /* out of memory */
	new->v=v; /* otherwise initialise new as a singleton heap */
	v->node=new;
	new->parent=new->child=new->pre=new->next=NULL;
	new->order=0;
	heap=unite(heap,new);	/* take the union */
	return new;
}

struct vertex *findMin(void) { /* find the min weight vertex in the heap */
	struct node *next;
	struct vertex *min=NULL;

	/* to find the min just iterate over the list of roots */
	for(next=heap;next;next=next->next) if(!min||next->v->key<min->key) min=next->v;
	return min;
}

struct vertex *extractMin(void) { /* extract the min from the tree */
	struct node *r,*next,*pre,*last;
	struct vertex *v=findMin(); /* find the min vertex */
	
	if(!v||!(r=v->node)) return NULL;	/* let r be the root that stores the min vertex */

	if(r->pre) r->pre->next=r->next; /* extract r from the root list */
	else heap=r->next;
	if(r->next) r->next->pre=r->pre;

	if(r->child) {	/* turn the children into a heap and take the union with heap */
		for(next=r->child;next->next;next=next->next) next->parent=NULL; /* find the last child and set the parent pointers to NULL */
		next->parent=NULL; /* also do the last one */
		for(last=next;next;next=pre) { /* store the last childe in last, then move from right to left over the children */
			pre=next->pre; /* make a note of the pre pointer */
			next->pre=next->next; /* swap the direction of the pointers */
			next->next=pre;
		}
		heap=unite(last,heap); /* now form the union */
	}

	v->node=NULL;	/* delete the node pointer from v */
	return v;
}

void reduce_key(struct vertex *v,double key) { /* reduce the key value of v to key, which must be smaller than or equal to v->key */
	struct node *a;

	if(key>=v->key) return; /* do nothing if the key value is invalid */
	v->key=key; /* set the new key */
	for(a=v->node;a->parent&&key<a->parent->v->key;a=a->parent) { /* move up the tree so long as the key is smaller */
		a->v=a->parent->v; /* swap the vertices represented by the parent and the child, but leave everything else in order */
		a->v->node=a;
		a->parent->v=v;
		v->node=a->parent;
	}
}

struct node *build_heap(void) {
	int i;

	for(i=0;i<G.n;i++) if(!insert(G.v+i)) return NULL;
	return heap;
}

void dijkstra(void) { /* the Dijkstra algorithm */
	struct vertex *u;
	struct edge *e;

	while((u=extractMin())!=NULL) { /* find the vertex of min key in Q */
		for(e=u->adj;e;e=e->next) {	/* update the keys of the adjacent vertices */
			if(e->v->node) { /* if v is in Q */
			double d=u->key+e->len;

			if(d<e->v->key) reduce_key(e->v,d);
			}
		}
	}
}

int main(int argc,char **argv) {
	if(argc>2) { /* if called with arguments, generate and output a random graph with random edge weights */
		int n=atoi(argv[1]),m=atoi(argv[2]);

		if(n>1&&m>0) {random_graph(n,m); return 0;}
		else {fputs("Invalid parameters",stderr); return 1;}
	}

	if(read_input()) {fputs("Error reading input",stderr); return 1;}
	if(!build_heap()) {fputs("Error building heap",stderr); return 1;}
	dijkstra();
	print_dist();
	return 0;
}
