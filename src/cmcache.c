#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  C->idx        = LF_GD;
  C->splitter   = splitter;
  for(x = 0 ; x < C->nLines ; ++x){
    C->lines[x] = (uint8_t  *) Calloc(MAX_LINE_CACHE + 2 * LF_GD + 1, 
                  sizeof(uint8_t));
    }
  return C;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// UPDATE CACHE
//
void UpdateCache(CACHE *C, uint8_t sym, uint8_t realSym){

  C->lines[0][C->idx++] = sym;
  if(realSym == C->splitter){
    int32_t x;
    for(x = C->nLines-2 ; x >= 0 ; x--){
      memcpy(C->lines[x+1], C->lines[x], MAX_LINE_CACHE + LF_GD);
    
/*      int32_t y;
      for(y = LF_GD ; y < MAX_LINE_CACHE ; ++y){
        C->lines[x+1][y] = C->lines[x][y]; 
        //if(C->lines[x][y] == C->splitter)
        //  break;
        }
*/
      
      }
    C->idx = LF_GD;
    }
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
