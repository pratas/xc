#ifndef CMCACHE_H_INCLUDED
#define CMCACHE_H_INCLUDED

#include "defs.h"

#define MAX_LINE_CACHE   65535
#define LF_GD            20

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct {
  uint8_t    **lines;          
  uint32_t   nLines;
  uint8_t    splitter;
  }
CACHE;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CACHE      *CreateCache            (uint32_t, uint8_t);
uint32_t   UpdateCache             (CACHE *, uint8_t, uint32_t);
void       RemoveCache             (CACHE *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
