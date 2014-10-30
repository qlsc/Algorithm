#include <time.h>

#define MAX_CACHE_LINES 32
#define MAX_ROWS 256
#define MAX_SETS 32

#define HIT  1
#define MISS 0

typedef struct {
    int valid;
    time_t time;
    int tag;
    int numCacheLines;
    int data[MAX_CACHE_LINES];
} Row;

typedef struct {
    int numRows;
    Row rows[MAX_ROWS];
} Set;

typedef struct {
    int s; // size
    int n; // n-way
    int l; // line-size
    Set sets[MAX_SETS];
} Cache;
struct row { 
	int valid; 
	int tag; 
	struct row *prev 
};
  struct row *firstRow = NULL;
  struct row cache[5000];
  int maxCacheIndex;
