#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "defs.h"
#include "mem.h"
#include "common.h"
#include "cm.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CModel *CreateCModel(uint32_t ctx, uint32_t aDen, uint8_t ref, uint32_t edits, 
uint32_t eDen, uint32_t nSym, uint8_t vert){

  uint64_t prod = 1, *mult, n;

  CModel     *M = (CModel *) Calloc(1, sizeof(CModel));
  M->nSym       = nSym;
  M->nPModels   = (uint64_t) pow(M->nSym, ctx);
  mult          = (uint64_t *) Calloc(ctx, sizeof(uint64_t));
  M->ctx        = ctx;
  M->alphaDen   = aDen;
  M->edits      = edits;
  M->vert       = vert;
  M->idx        = 0;
  M->ref        = ref == 0 ? 0 : 1;

  if((ULL)(M->nPModels) * M->nSym * sizeof(ACC) >> 20 > MAX_ARRAY_MEMORY){
    M->mode     = HASH_TABLE_MODE;
    M->HT       = CreateHashTable(M->nSym);
    }
  else{
    M->mode     = ARRAY_MODE;
    M->AT       = CreateArrayTable(M->nSym, M->nPModels);
    }

  if(M->vert != 0){
    M->VM       = CreateCTemplate2D(M->ctx);
    //PrintCTemplate2D(M->VM);
    }

  for(n = 0 ; n < M->ctx ; ++n){
    mult[n] = prod;
    prod *= M->nSym;
    }

  M->multiplier = mult[M->ctx-1];

  if(M->edits != 0){
    M->TM = CreateTolerantModel(edits, eDen, ctx, nSym);
    }

  Free(mult);
  return M;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ResetCModelIdx(CModel *M){
  M->idx = 0;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void GetCModelIdx(U8 *p, CModel *M){
  M->idx = ((M->idx-*(p-M->ctx)*M->multiplier)*M->nSym)+*p;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint64_t GetCModelIdx2D(CModel *M, CACHE *C, int col, ALPHABET *A){
  int x;
  uint64_t idx = 0, prod = 1;

  fprintf(stderr, "col: %d\n", col);
  fprintf(stderr, "M->VM.size: %d\n", M->VM.size);
  fprintf(stderr, "M->VM.position[0].row: %d\n", M->VM.position[0].row);
  fprintf(stderr, "M->VM.position[0].col: %d\n", M->VM.position[0].col);
  fprintf(stderr, "M->VM.position[1].row: %d\n", M->VM.position[1].row);
  fprintf(stderr, "M->VM.position[1].col: %d\n", M->VM.position[1].col);
  

  for(x = 0 ; x < M->VM.size ; ++x){
    idx += A->alphabet[C->lines[ M->VM.position[x].row + 1  ]
                               [ M->VM.position[x].col + col] ] * prod;
    prod *= A->cardinality;
    }

  M->idx = idx;
  return idx;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint64_t GetCModelIdxCorr(uint8_t *p, CModel *M, uint64_t idx){
  return (((idx-*(p-M->ctx)*M->multiplier)*M->nSym)+*p);
  }
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ComputePModel(CModel *M, PModel *P, uint64_t idx, uint32_t aDen){
  ACC *ac;
  HCC *hc;
  uint32_t x;
  switch(M->mode){

    case HASH_TABLE_MODE:
     if(!(hc = GetHCCounters(M->HT, idx)))
       hc = (HCC *) M->HT->zeroCounters;
     P->sum = 0;
     for(x = 0 ; x < M->nSym ; ++x){
       P->freqs[x] = 1 + aDen * hc[x];
       P->sum += P->freqs[x];
       }
    break;

    case ARRAY_MODE:
      ac = &M->AT->counters[idx*M->nSym];
      P->sum = 0;
      for(x = 0 ; x < M->nSym ; ++x){
        P->freqs[x] = 1 + aDen * ac[x];
        P->sum += P->freqs[x];
        }
    break;

    default:
    fprintf(stderr, "Error: not implemented!\n");
    exit(1);
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void UpdateCModelCounter(CModel *M, uint32_t sym, uint64_t idx){
  if(M->mode == HASH_TABLE_MODE) UpdateHashCounter (M->HT, sym, idx);
  else                           UpdateArrayCounter(M->AT, sym, idx);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RemoveCModel(CModel *M){
  if(M->mode == HASH_TABLE_MODE) RemoveHashTable (M->HT);
  else                           RemoveArrayTable(M->AT);

  if(M->edits > 0)               RemoveTolerantModel(M->TM);
  Free(M);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
