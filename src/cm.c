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

  M->vert       = vert;
  if(M->vert != 0){
    M->VM       = CreateCTemplate2D(ctx);
    M->ctx      = M->VM.size;
    //PrintCTemplate2D(M->VM);
    }
  else{
    M->ctx      = ctx;
    }

  M->nSym       = nSym;
  M->nPModels   = (uint64_t) pow(M->nSym, M->ctx);
  mult          = (uint64_t *) Calloc(M->ctx, sizeof(uint64_t));
  M->alphaDen   = aDen;
  M->edits      = edits;
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

  for(n = 0 ; n < M->ctx ; ++n){
    mult[n] = prod;
    prod *= M->nSym;
    }

  M->multiplier = mult[M->ctx-1];

  if(M->edits != 0){
    M->TM       = CreateTolerantModel(edits, eDen, M->ctx, nSym);
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

uint64_t GetCModelIdx2D(CModel *M, CACHE *C, ALPHABET *A){
  int32_t x;
  uint64_t idx = 0, prod = 1;

  //fprintf(stderr, "col: %d\n", C->idx);
  //fprintf(stderr, "M->VM.size: %d\n", M->VM.size);
  //for(x = 0 ; x < M->VM.size ; ++x){
  //  fprintf(stderr, "M->VM.position[%d].row: %d\n", x, M->VM.position[x].row);
  //  fprintf(stderr, "M->VM.position[%d].col: %d\n", x, M->VM.position[x].col);
  //    }

  for(x = M->VM.size-1 ; x >= 0 ; --x){
    idx += C->lines[M->VM.position[x].row]
                   [M->VM.position[x].col + C->idx] * prod;
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
