#ifndef CM_H_INCLUDED
#define CM_H_INCLUDED

#include "defs.h"
#include "hash.h"
#include "array.h"
#include "template.h"
#include "buffer.h"
#include "alphabet.h"
#include "cmcache.h"
#include "pmodels.h"
#include "tolerant.h"

#define ARRAY_MODE            0
#define HASH_TABLE_MODE       1

typedef struct{
  uint32_t    ctx;           // Current depth of context template
  uint32_t    nSym;          // Number of symbols
  uint64_t    nPModels;      // Maximum number of probability models
  uint32_t    alphaDen;      // Denominator of alpha
  uint64_t    multiplier;
  uint64_t    idx;           // IDX
  uint32_t    mode;
  HASH        *HT;
  ARRAY       *AT;
  TOLERANT    *TM;           // EDITS
  uint32_t    edits;         // EDITS
  uint8_t     vert;
  TEMPLATE2D  VM;
  uint8_t     ref;
  }
CModel;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CModel          *CreateCModel        (uint32_t, uint32_t, uint8_t, uint32_t, 
                                      uint32_t, uint32_t, uint8_t);
void            GetCModelIdx         (uint8_t *, CModel *);
uint64_t        GetCModelIdx2D       (CModel *, CACHE *, int, ALPHABET *);
uint64_t        GetCModelIdxCorr     (uint8_t *, CModel *, uint64_t);
void            ResetCModelIdx       (CModel *);
void            UpdateCModelCounter  (CModel *, uint32_t, uint64_t);
void            ComputePModel        (CModel *, PModel *, uint64_t, uint32_t);
void            RemoveCModel         (CModel *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
