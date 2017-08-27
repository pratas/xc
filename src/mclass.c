#include <stdio.h>
#include <stdlib.h>
#include "mclass.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

MCLASS *CreateMClass(uint32_t n){

  MCLASS *MC        = (MCLASS  *) Calloc(1, sizeof(MCLASS));
  MC->CM            = (CModel **) Calloc(n, sizeof(CModel *));
  MC->nModels       = n;
  MC->refNModels    = 0;
  MC->tarNModels    = 0;
  MC->totalNPModels = 0;

  return MC;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void PrintMClassInfo(MCLASS *MC){
  printf("Number of reference models     : %d\n", MC->refNModels);
  printf("Number of target models        : %d\n", MC->tarNModels);
  printf("Number of total models         : %d\n", MC->nModels);
  printf("Number of probabilistic models : %d\n", MC->totalNPModels);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RemoveMClass(MCLASS *MC){
  uint32_t x;

  for(x = 0 ; x < MC->refNModels ; ++x){
    RemoveCModel(MC->CM[x]);
    }

  Free(MC->CM);  
  Free(MC);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


