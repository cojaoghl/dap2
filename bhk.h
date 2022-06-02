/* length of the equator  */
#define CIRCUMFERENCE	40000.0

/* max number of cities */
#define MAX_CITIES	32

/* max distance */
#define DIST_MAX 65535

typedef unsigned short dist_t; /* define type for distance measurements */

struct city {	/* data structure to represent a "city" */
	char *name; /* name of the city */
	double longitude, latitude; /* coordinates on the sphere */
};



