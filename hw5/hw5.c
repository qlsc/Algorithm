//
//  main.c
//  cs3650
//
//  Created by Andrew Barba on 10/24/14.
//  Copyright (c) 2014 Andrew Barba. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#define MAX_CACHE_LINES 32
#define MAX_ROWS 256
#define MAX_SETS 32

#define HIT  1
#define MISS 0

//#define powf(x,y)     (float)pow((double)(x),(double)(y))
//#define log2f(x)     ((float)log((double)(x)))/(log((double)2))

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

Cache buildCache(int s, int n, int l)
{
	int i,j;
    Cache cache = {s,n,l};
    for (i = 0; i < n; i++) {
        int rows = s / ( n * l );
        Set set = {rows};
        for (j = 0; j < set.numRows; j++) {
            Row row = {0, 0, -1, l};
            set.rows[j] = row; // add row to set
        }
        cache.sets[i] = set; // add set to cache
    }
    return cache;
}

int visit(Set *set, int tag, int offset)
{
    time_t low = INT32_MAX;
    int low_i = set->numRows - 1;
    int i;
    for (i = 0; i < set->numRows; i++) {
        
        // get row
        Row *row = &set->rows[i];
        
        // If the tag is found
        if (row->tag == tag) {
            time(&row->time);
            row->data[offset] = 1;
            return HIT;
        }
        
        // If the tag is not found but there is an empty slot
        if (!row->valid) {
            row->valid = 1;
            time(&row->time);
            row->tag = tag;
            row->data[offset] = 1;
            return MISS;
        }
        
        // If the tag is not found and set is full, evict lowest version
        if (i == set->numRows - 1) {
            Row *rowl = &set->rows[low_i];
            rowl->tag = tag;
            time(&rowl->time);
            rowl->data[offset] = 1;
            return MISS;
        } else {
            if (row->time < low) {
                low_i = i;
                low = row->time;
            }
        }
    }
    
    return MISS;
}

void visitAll(Cache *cache, int len, int adrs[])
{
    printf("Cache: (%i, %i, %i) [\n",cache->s,cache->n,cache->l);
    
    // compute number of rows per set
    int rows = (cache->s / cache->l) / cache->n;
    
    // calculate bits for offset and index
    int OFFSET_BITS = log2f(cache->l / 4);
    int INDEX_BITS = log2f(rows);
    
    // calculate max size for offset and idnext
    int OFFSET_SIZE = powf(2, OFFSET_BITS) - 1;
    int INDEX_SIZE = powf(2, INDEX_BITS) - 1;
    
    // loop through addresses and visit each
	int i,j;
    for (i = 0; i < len; i++) {
        
        // get address
        int adr = adrs[i];
        
        // compute tag, index, offset
        int offset, index, tag;
        offset = index = tag = 0;
        
        for (j = 0; j < adr; j++) {
            offset++;
            if (offset > OFFSET_SIZE) {
                offset = 0;
                index++;
            }
            if (index > INDEX_SIZE) {
                index = 0;
                tag++;
            }
        }
        
        // get set
        Set *set = &cache->sets[index];
        
        // test for hit
        int hit = visit(set, tag, offset);
        
        // log to console
//        printf("%d %d %d\n", tag, index, offset);
        printf("(%i : %s)\n", adrs[i], hit ? "HIT" : "MISS");
    }
    
    printf("]\n\n");
}

int main(int argc, char *argv[])
{
    printf("HW5\n\n");
    
    static int len = 39;
    static int adrs[] = {
        0,  4,  8,  12, 16, 20, 24, 28,
        32, 36, 40, 44, 48, 52, 56, 60,
        64, 68, 72, 76, 80, 0,  4,  8,
        12, 16, 71, 3,  41, 81, 39, 38,
        71, 15, 39, 11, 51, 57, 41
    };
    
    Cache ex1 = buildCache(128, 16, 8);
    Cache ex2 = buildCache(64,  2, 8);
    Cache ex3 = buildCache(128, 8, 16);
    Cache ex4 = buildCache(64,  1, 8);
    
    visitAll(&ex1, len, adrs);
    visitAll(&ex2, len, adrs);
    visitAll(&ex3, len, adrs);
    visitAll(&ex4, len, adrs);
    
//    static int tadrs[] = {1, 4, 8, 5, 20, 17, 19, 56, 9, 11, 4, 43, 5, 6, 9, 17};
//    Cache t = buildCache(64, 2, 4);
//    visitAll(&t, 16, tadrs);
    
    return 0;
}