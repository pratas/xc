#ifndef MCLASS_H_INCLUDED
#define MCLASS_H_INCLUDED

#include "defs.h"
#include "cm.h"

typedef struct{
  CModel   **CM;
  uint32_t nModels;
  uint32_t refNModels;
  uint32_t tarNModels;
  uint32_t totalNPModels;
  }
MCLASS;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

MCLASS          *CreateMClass        (uint32_t);
void            PrintMClassInfo      (MCLASS *);
void            RemoveMClass         (MCLASS *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
