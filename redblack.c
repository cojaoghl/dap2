/* a good way to invoke is : seq 32 | shuf | (while read x; do echo i $x; done; echo d 1; echo d 2; echo d 3) | ./redblack | sage */


#include<stdio.h>
#include<stdlib.h>

enum colour {red,black};

typedef int key_t;

struct node {
	key_t key;
	struct node *parent,*left,*right;
	enum colour col;
};

struct node nil={.key=0,.parent=NULL,.left=NULL,.right=NULL,.col=black};
struct node *root=&nil;

void rotate_left(struct node *x) { /* left rotation around x */
	struct node *y;

	if(x==&nil) return; /* abort if x points to nil */
	if((y=x->right)==&nil) return; /* abort if x->right points to nil */

	x->right=y->left;	/* make y's left child the right child of x */
	if(y->left!=&nil) y->left->parent=x;

	y->parent=x->parent;
	if(x==root) root=y; /* if x is the root, make y the root */
	else if(x==x->parent->left) x->parent->left=y; /* if x is a left child, set its parent's left pointer to y */
	else x->parent->right=y; /* accordingly if x is a right child */

	y->left=x;	/* make x the left child of y */
	x->parent=y;
}

void rotate_right(struct node *y) { /* right rotation around x */
	struct node *x;

	if(y==&nil) return; /* abort if y points to nil */
	if((x=y->left)==&nil) return; /* abort if y->left points to nil */

	y->left=x->right;	/* make x's right child the left child of y */
	if(x->right!=&nil) x->right->parent=y;

	x->parent=y->parent;
	if(y==root) root=x; /* if y is the root, make x the root */
	else if(y==y->parent->left) y->parent->left=x; /* if y is a left child, set its parent's left pointer to x */
	else y->parent->right=x; /* accordingly if y is a right child */

	x->right=y;	/* make y the right child of x */
	y->parent=x;
}

/* insertion */

void insert_cleanup(struct node *z) {
	struct node *y;

	while(z->parent->col==red) {
		if(z->parent==z->parent->parent->left) { /* if the parent of z is a left child */
			y=z->parent->parent->right;	/* y points to z's aunt */
			if(y->col==red) { /* red aunt */
				z->parent->col=y->col=black;	/* make y and z's parent black */
				z=z->parent->parent; /* set z to the grandparent */
				z->col=red; /* ...and colour it red */
			}
			else {
				if(z==z->parent->right) rotate_left(z=z->parent);  /* if z is a right child, perform a left rotation around parent */
				z->parent->col=black;	/* colour the parent black */
				z->parent->parent->col=red; /*  colour grandparent red */	
				rotate_right(z->parent->parent); /* right rotation */
			}
		}
		else {	/* z is a right child; do same as above but with left/right interchanged */
			y=z->parent->parent->left;
			if(y->col==red) {	
				z->parent->col=y->col=black;
				z=z->parent->parent;
				z->col=red;
			}
			else {
				if(z==z->parent->left) rotate_right(z=z->parent);
				z->parent->col=black;
				z->parent->parent->col=red;
				rotate_left(z->parent->parent);
			}
		}
	}
	root->col=black;
}

struct node *insert(key_t key) {
	struct node *x,*y,*z;

	if(!(z=malloc(sizeof(struct node)))) return NULL;	/* allocate memory; failure if we are out of memory */
	z->key=key;	/* set key */
	z->left=z->right=&nil; /* new node z has no children */
	z->col=red;	/* new node is red */

	/* now set z->parent appropriately; x=comparison point; y=node to be the parent of z */
	for(x=root,y=&nil;x!=&nil;x=(key<x->key)?x->left:x->right) y=x;	/* mimic search for key */
	z->parent=y;	/* set parent of z */	
	if(y==&nil) root=z;	/* make z the root if parent happens to be nil */
	else if(key<y->key) y->left=z;	/* otherwise set the left/right pointer of y to z */
	else y->right=z;
	
	insert_cleanup(z); /* call cleanup subroutine */
	return z;
}


/* deletion */

void transplant(struct node *u,struct node *v) {	/* put v into u's place */
	v->parent=u->parent; /* update v's parent pointer */
	if(u->parent==&nil) root=v; /* if u has no parent, make u the root */
	else if(u->parent->left==u) u->parent->left=v;	/* redirect the appropriate pointer of u's parent to v */
	else u->parent->right=v;
}


void delete_cleanup(struct node *x) { /* cleanup helper for delete */
	struct node *w;

	while(x!=root&&x->col==black) {
		if(x==x->parent->left) { /* if x is a left child */
			w=x->parent->right; /* x's sibling */	
			if(w->col==red) { /* case 1 */
				w->col=black; /* colour w black */
				x->parent->col=red; /* colour x's parent red */
				rotate_left(x->parent); /* rotate x's parent left */
				w=x->parent->right; /* update w */
			}
			if(w->left->col==black&&w->right->col==black) /* case 2 */
			{
				w->col=red,x=x->parent;} /* colour w red and move on to x's parent */
			else {
				if(w->right->col==black) { /* case 3 */
					w->left->col=black; /*  color w's left child black */
					w->col=red; /* color w red */
					rotate_right(w);	/* right rotation */
					w=x->parent->right; /* update w */
				}
				/* case 4 */
				w->col=x->parent->col; /* update w's colour */
				x->parent->col=black; /* colour x's parent black */
				w->right->col=black; /* colour w's right child black */
				rotate_left(x->parent); /* rotate left around x's parent */
				x=root; /* make x the root and thus stop the while loop */
			}
		}
		else { /* otherwise do the same with 'left' and 'right' flipped */
			w=x->parent->left; /* x's sibling */	
			if(w->col==red) { /* case 1 */
				w->col=black; /* colour w black */
				x->parent->col=red; /* colour x's parent red */
				rotate_right(x->parent); /* rotate x's parent right */
				w=x->parent->left; /* update w */
			}
			if(w->right->col==black&&w->left->col==black) /* case 2 */
			{w->col=red,x=x->parent;
			} /* colour w red and move on to x's parent */
			else {
				if(w->left->col==black) { /* case 3 */
					w->right->col=black; /*  color w's right child black */
					w->col=red; /* color w red */
					rotate_left(w);	/* left rotation */
					w=x->parent->left; /* update w */
				}
				/* case 4 */
				w->col=x->parent->col; /* update w's colour */
				x->parent->col=black; /* colour x's parent black */
				w->left->col=black; /* colour w's left child black */
				rotate_right(x->parent); /* rotate right around x's parent */
				x=root; /* make x the root and thus stop the while loop */
			}
		}
	}
	x->col=black;
}

struct node *delete(key_t key) {
	struct node *x,*y,*z;
	enum colour col;

	for(z=root;z!=&nil&&z->key!=key;z=(z->key>key)?z->left:z->right); /* search for key in the tree */
	if(z==&nil) return NULL; /* if key is not present, abort */
	col=z->col;	/* set col to z's colour */

	if(z->left==&nil) /* if z has no left child */
		transplant(y=z,x=z->right); /* transplant z's right child (which may be nil) for z */
	else if(z->right==&nil) /* if z has no right child */
		transplant(y=z,x=z->left); /* transplant z's left child for z */
	else { /* z has two children */
		for(x=y=z->right;x!=&nil;x=x->left) y=x; /* find min y in the right subtree */
		col=y->col; /* make a note of y's original colour */
		x=y->right;	 /* x takes y's place */
		if(y!=z->right) { /* unless y is the child of z */
			transplant(y,x); /* let y's right child x take y's place */
			y->right=z->right;	/* let y take z's place */
			y->right->parent=y;
		}
		else x->parent=y; /* this is necessary because x may be nil! */
		transplant(z,y); /* put y in z's place */
		y->left=z->left; /* y gets z's left child */
		y->left->parent=y;
		y->col=z->col; /* y gets z's colour */
	}

	if(col==black) delete_cleanup(x); /* if y was black originally, cleanup */
	return z; /* return pointer to the deleted node; do not free memory */
}

void print_node(struct node *v) { /* recurive procedure for printing nodes */
	if(v==NULL||v==&nil) return; /* don't print the dummy node */
	if(v->left!=&nil) printf("(%d,%d),",v->key,v->left->key); /* if v has a left child, print the edge */
	if(v->right!=&nil) printf("(%d,%d),",v->key,v->right->key); /* if v has a right child, print the edge */
	print_node(v->left);	/* proceed recursively */
	print_node(v->right);
}

void print_colors(struct node *v,enum colour col) { /* print nodes with given color */
	if(v==NULL||v==&nil) return; /* don't print the dummy node */
	if(v->col==col&&v!=root) printf("%d,",v->key); /* if the colour matches and v is not the root, print its key */
	print_colors(v->left,col);	/* recurse */
	print_colors(v->right,col);
}

void print_tree(void) { /* export tree in sagemath format */
	if(root==&nil) return; /* nothing to do if the tree is empty */
	printf("g=Graph([ ");
	print_node(root);
	printf("])\n");
	printf("cols={");
	printf("'#808080': ["); /* #808080 is the RGB code for grey (so we can read the labels) */
	print_colors(root,black); /* list black vertices */
	printf("],");
	printf("'#FF8080': ["); /* #FF8080 is the RGB code for light red */
	print_colors(root,red); /* list red vertices */
	printf("],");
	printf("'#8080FF': [%d]}\n",root->key); /* light blue colour for the root */
	printf("g.plot(vertex_colors=cols)\n");
}

void read_instructions(void) { /* read instructions from standard input; instructions are 'i <key>' and 'd <key>' for insertion/deletion and 'p' for print */
	size_t n;
	key_t key;
	char *line;

	while(1) {
		line=NULL;
		if(getline(&line,&n,stdin)<=0) break; /* read a line of input; break if no more input available */
		sscanf(line+1,"%d",&key);	/* read the key value */
		switch(*line) {	/* the first character determines the command to be run */
			case 'i': if(!insert(key)) fprintf(stderr,"Insert key %d: error!\n",key); break;	/* perform insertion */
			case 'd': if(!delete(key)) fprintf(stderr,"Delete key %d: error!\n",key); break;	/* perform deletion */
		}
	} 
}

int main(int argc,char **argv) {
	read_instructions();
	print_tree();
	return 0;
}
