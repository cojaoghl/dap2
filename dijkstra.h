/* header file for Dijkstra/linked list implementation */

#define INFTY DBL_MAX
#define NINFTY DBL_MIN

/* the graph is stored as an adjacency list */
/* for each vertex we allocate a list of "edges" which represent entries in the adjacency list */
/* every edge of the graph is stored twice: once in the adjacency list of either vertex */
/* the weights of the edges are stored as doubles and are assumed to be non-negative */
/* the start vertex for Dijkstra is always the first vertex (id 0) */

struct vertex;
struct node;

struct edge {
	double len;		/* length of the edge */
	struct vertex *v;	/* the vertex on the other side of the edge */
	struct edge *next;	/* pointer to the next entry of the adjacency list */
};

struct vertex {
	int id;	/* id of the vertex */
	struct edge *adj;	/* first entry of the adjacency list */
	double key;	/* key value used by Dijkstra */
	struct node *node; /* entry of the vertex in the binomial heap */
};

struct graph {
	int n;	/* number of vertices */
	struct vertex *v;	/* pointer to the first vertex */
};

/* binomial heap data structure */

struct node {
	int order;	/* order of the binomial heap */
	struct vertex *v; /* pointer to the vertex that the node represents */	
	struct node *pre; /* pointer to the previous node on the same level */
	struct node *next; /* pointer to the next node on the same level */
	struct node *parent; /* pointer to the parent node (or NULL if none) */
	struct node *child; /* pointer to the leftmost child (or NULL if none) */
};

int read_input(void); /* read input from stdin */
void print_dist(void); /* print the key values of all the vertices */
void random_graph(int n,int m); /* print a random graph with n vertices and about m edges and random edge weights to stdout */
void print_heap(struct node *next); /* print a heap */

