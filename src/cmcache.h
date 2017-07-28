#ifndef CMCACHE_H_INCLUDED
#define CMCACHE_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct {
  uint8_t    **lines;          
  uint32_t   nLines;
  uint8_t    splitter;
  }
CACHE;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CACHE      *CreateCache            (uint32_t, uint8_t);
void       UpdateCache             (CACHE *);
void       RemoveCache             (CACHE *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
