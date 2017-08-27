#ifndef CM2D_H_INCLUDED
#define CM2D_H_INCLUDED

#include "defs.h"
#include "buffer.h"
#include "alphabet.h"
#include "cmcache.h"
#include "pmodels.h"

#define WINDOW_SIZE       6  // IT WILL ACCEPT X SUBSTITUTIONS IN 6
#define ARRAY_MODE        0
#define HASH_TABLE_MODE   1
#define HASH_SIZE         16777259

typedef U16  ACC;            // Size of context counters for arrays
typedef U8   HCC;            // Size of context counters for hash tables
typedef U8   ENTMAX;         // Entry size (nKeys for each hIndex)

typedef struct{
  uint64_t   key;            //The key stored in this entry 
  HCC        *counters;      //The context counters 
  }
Entry;

typedef struct{
  uint32_t   size;           //Size of the hash table
  uint8_t    *entrySize;     //Number of keys in this entry
  Entry      **entries;      //The heads of the hash table lists
  HCC        **zeroCounters;  
  }
HashTable;

typedef struct{
  ACC        *counters;
  }
Array;

typedef struct{
  uint32_t   in;
  CBUF       *seq;           // BUFFER FOR EDITED SEQUENCE
  uint8_t    *mask;          // BUFFER FOR FAILS & HITS
  uint64_t   idx;            // INDEX FOR UPDATE
  uint64_t   idx2;           // AUXILIAR INDEX FOR UPDATE
  uint32_t   threshold;      // DISCARD ABOVE THIS VALUE
  uint32_t   eDen;           // ALPHA DENOMINATOR FOR THIS MODEL
  }
Correct;

typedef struct{
  int        row;
  int        col;
  }
Coords;

typedef struct{
  int        id;
  int        size;
  Coords     *position;
  }
Template2D;

typedef struct{
  U32        ctx;           // Current depth of context template
  U64        nPModels;      // Maximum number of probability models
  U32        alphaDen;      // Denominator of alpha
  U32        maxCount;      // Counters /= 2 if one counter >= maxCount
  U64        multiplier;
  U8         ref;
  U32        mode;
  HashTable  hTable;
  Array      array;
  U64        pModelIdx;     // IDX
  U32        edits;         // EDITS
  U32        nSym;          // EDITS
  Correct    SUBS;          // EDITS
  }
CHModel;

typedef struct{
  U32        ctx;           // Current depth of context template
  U64        nPModels;      // Maximum number of probability models
  U32        alphaDen;      // Denominator of alpha
  U32        maxCount;      // Counters /= 2 if one counter >= maxCount
  U64        multiplier;
  U8         ref;
  U32        mode;
  HashTable  hTable;
  Array      array;
  U64        pModelIdx;     // IDX
  U32        edits;         // EDITS
  U32        nSym;          // EDITS
  Correct    SUBS;          // EDITS
  }
CVModel;

typedef struct{
  uint32_t   ref_regular_nModels;
  uint32_t   ref_tolerant_nModels;
  uint32_t   ref_template_nModels;
  uint32_t   ref_total_nModels;
  uint32_t   tar_regular_nModels;
  uint32_t   tar_tolerant_nModels;
  uint32_t   tar_template_nModels;
  uint32_t   tar_total_nModels;
  uint32_t   total_regular_nModels;
  uint32_t   total_tolerant_nModels;
  uint32_t   total_template_nModels;
  uint32_t   total_template_and_regular_nModels;
  uint32_t   total_nModels;
  }
MCLASS;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t         BestId               (uint32_t *, uint32_t, uint32_t);
void            HitSUBS              (CHModel *);
void            FailSUBS             (CHModel *);
void            FreeCHModel          (CHModel *);
Template2D      CreateCTemplate2D    (int);
void            GetPModelIdx         (U8 *, CHModel *);
uint64_t        GetPModelIdx2D       (CVModel *, CACHE *, int, ALPHABET *, 
                                     Template2D *);
uint64_t        GetPModelIdxCorr     (U8 *, CHModel *, uint64_t);
void            CorrectCHModelSUBS   (CHModel *, PModel *, uint8_t);
void            ResetCHModelIdx      (CHModel *);
void            UpdateCHModelCounter (CHModel *, U32, U64);
CHModel         *CreateCHModel       (U32, U32, U8, U32, U32, U32, U8);
void            ComputePModel        (CHModel *, PModel *, uint64_t, uint32_t);
double          PModelSymbolNats     (PModel *, U32);
MCLASS          *CreateMClass        (void);
void            RemoveMClass         (MCLASS *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
