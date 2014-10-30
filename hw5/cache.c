
#include "cache.h"
#include <stdio.h>
#include <math.h>


  //Useful for direct-mapped cache:
  // Omitting offset; don't need it for this simulation
  int isHitOrMissForDirectMapped( int tag, int index );

  int main () {

    for (...) {
      ... get next address ...
      tag = ...;
      index = ...;
      // We declared isHitOrMissForDirectMapped above.  But we define it below.
      answer = isHitOrMissForDirectMapped( tag, index );
      if (answer)
        printf("address(%d) Hit\n", address);
      else
        printf("address(%d) Miss\n", address);
      ...
    }
    return 0;  // The fnc main returns 0 for success.  Anything else is error number.
  }
  int isHitOrMissForDirectMapped( int tag, int index ) {
    int rowIdx = 0;
    // For direct-mapped, index is the cache line number.
    if ( cache[index].valid && cache[index].tag == tag )
      isHit = true;
    else
      isHit = false;
    ...
    return isHit;
  }


//Useful for  k-way set associative cache :
  int isHitOrMissForSetAssoc( int k, int tag, int index ) {
    int isHit = 0;  // Initialize isHit to default value:  false
    int rowIdx = 0;
    // For set associative, index is the set number.
    for ( rowIdx = k*index; rowIdx < k*index + k; rowIdx++ )
      if ( cache[rowIdx].valid && cache[rowIdx].tag == tag ) {
        isHit = true;
        break;
      }
    }
    // Now, isHit has value true if and only if we found a hit.
    if (isHit) return 1; // return true
    // Else search for cache line with valid field == 0 (false)
    for ( rowIdx = k*index; rowIdx < k*index + k; rowIdx++ )
      if ( cache[rowIdx].valid ) == 0 )
        break;
    // If we didn't find a cache line with valid field false, then evict cache line
    if (rowIdx + k >= k*index) { // if falied to find invalid cache line
      ...
    }
    return isHit;
  }



Cache buildCache(int s, int n, int l)
{
    Cache cache = {s,n,l};
    for (int x = 0; x < n; x++) {
        int rows = s / ( n * l );
        Set set = {rows};
        for (int z = 0; z < set.numRows; z++) {
            Row row = {0, 0, -1, l};
            set.rows[z] = row; // add row to set
        }
        cache.sets[x] = set; // add set to cache
    }
    return cache;
}

int visit(Set *set, int tag, int offset)
{
    time_t low = INT32_MAX;
    int low_i = set->numRows - 1;
    
    for (int x = 0; x < set->numRows; x++) {
        
        // get row
        Row *row = &set->rows[x];
        
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
        if (x == set->numRows - 1) {
            Row *rowl = &set->rows[low_i];
            rowl->tag = tag;
            time(&rowl->time);
            rowl->data[offset] = 1;
            return MISS;
        } else {
            if (row->time < low) {
                low_i = x;
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
    for (int x = 0; x < len; x++) {
        
        // get address
        int adr = adrs[x];
        
        // compute tag, index, offset
        int offset, index, tag;
        offset = index = tag = 0;
        
        for (int x = 0; x < adr; x++) {
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
        printf("(%i : %s)\n", adrs[x], hit ? "HIT" : "MISS");
    }
    
    printf("]\n\n");
}
