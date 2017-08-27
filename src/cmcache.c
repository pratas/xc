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
    C->lines[x] = (uint8_t  *) Calloc(MAX_LINE_CACHE, sizeof(uint8_t));
    }
  return C;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// UPDATE CACHE
//
uint32_t UpdateCache(CACHE *C, uint8_t sym, uint32_t col){

  C->lines[0][col++] = sym;
  if(sym == C->splitter){
    int32_t x, y; // TODO: USE MEMMOVE
    for(x = C->nLines-2 ; x >= 0 ; x--){
      for(y = 0 ; y < MAX_LINE_CACHE ; ++y){
        C->lines[x+1][y] = C->lines[x][y]; 
        if(C->lines[x][y] == C->splitter)
          break;
        }
      }
    col = 0;
    }

  return col;
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
