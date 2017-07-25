#include <stdio.h>
#include "cmcache.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CREATE CACHE
//
CACHE *CreateCache(uint32_t size, uint8_t splitter){
  uint32_t x;
  CACHE *C      = (CACHE    *) Calloc(1,    sizeof(CACHE));
  C->lines      = (uint8_t **) Calloc(size, sizeof(uint8_t *));
  C->nLines     = size;
  C->splitter   = splitter;
  for(x = 0 ; x < C->nLines ; ++x){
    C->lines[x] = (uint8_t  *) Calloc(65535, sizeof(uint8_t));
    }
  return C;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// REMOVE CACHE
//
void RemoveCache(CACHE *C){
  uint32_t x;
  for(x = 0 ; x < C->nLines ; ++x)
    Free(C->lines[x]);
  Free(C->lines);
  Free(C);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
