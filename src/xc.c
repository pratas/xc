#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include "mem.h"
#include "defs.h"
#include "msg.h"
#include "buffer.h"
#include "alphabet.h"
#include "cmcache.h"
#include "levels.h"
#include "common.h"
#include "pmodels.h"
#include "cm.h"
#include "mclass.h"
#include "tolerant.h"
#include "bitio.h"
#include "arith.h"
#include "arith_aux.h"

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - C O M P R E S S O R - - - - - - - - - - - - - -

int Compress(Parameters *P, uint8_t id, INF *I, MCLASS *MC){
  FILE        *Reader  = Fopen(P->tar[id], "r");
  char        *name    = concatenate(P->tar[id], ".co");
  FILE        *Writter = Fopen(name, "w");
  uint32_t    n, k, x, model, idxPos;
  uint64_t    i = 0, size = 0;
  uint8_t     *readerBuffer, sym, *pos; 
  PModel      **pModel, *MX;
  FloatPModel *PT;
  CBUF        *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);
  CACHE       *CS = CreateCache(50, '\n');
  CMWeight    *WM;

  if(P->verbose)
    fprintf(stderr, "Analyzing data and creating models ...\n");

  _bytes_output = 0;
  size = NBytesInFile(Reader);

  // BUILD ALPHABET
  ALPHABET *AL = CreateAlphabet(P->low);
  LoadAlphabet(AL, Reader);
  //PrintAlphabet(AL);

  // ADAPT ALPHABET FOR NON FREQUENT SYMBOLS
  AdaptAlphabetNonFrequent(AL, Reader);
  
  // EXTRA MODELS DERIVED FROM EDITS
  MC->totalNPModels = MC->nModels;
  for(n = 0 ; n < MC->nModels ; ++n) 
    if(P->model[n].edits != 0){
      #ifdef RUN_SUBS
      MC->totalNPModels++;
      #endif
      }

  PrintMClassInfo(MC);

  pModel        = (PModel **) Calloc(MC->totalNPModels, sizeof(PModel *));
  for(n = 0 ; n < MC->totalNPModels ; ++n)
    pModel[n]   = CreatePModel(AL->cardinality);
  MX            = CreatePModel(AL->cardinality);
  PT            = CreateFloatPModel(AL->cardinality);
  WM            = CreateWeightModel(MC->totalNPModels);
  readerBuffer  = (uint8_t *) Calloc(BUFFER_SIZE, sizeof(uint8_t));

  for(n = MC->refNModels ; n < MC->nModels ; ++n)
    MC->CM[n] = CreateCModel(P->model[n].ctx, P->model[n].den, TARGET, 
                P->model[n].edits, P->model[n].eDen, AL->cardinality, 
                P->model[n].vert);

  if(P->verbose){
    fprintf(stderr, "Done!\n");
    fprintf(stderr, "Compressing target sequence %d [%"PRIu64" symbols] ...\n", 
    id + 1, size);
    }

  startoutputtingbits();
  start_encode();

  WriteNBits(WATERMARK,                32, Writter);
  WriteNBits(P->checksum,              46, Writter);
  WriteNBits(size,                     46, Writter);

  // PRE HEADER : NON FREQUENT SYMBOLS
  WriteNBits(P->low,                   32, Writter);
  WriteNBits(AL->nLow,                 32, Writter);
  for(x = 0 ; x < AL->nLow ; ++x){
    WriteNBits(AL->lowAlpha[x],         8, Writter);
    }

  // REMAP ALPHABET
  // ResetAlphabet(AL);
  // PrintAlphabet(AL);

  WriteNBits(AL->cardinality,          16, Writter);
  for(x = 0 ; x < AL->cardinality ; ++x)
    WriteNBits(AL->toChars[x],          8, Writter);
  WriteNBits((int) (P->gamma * 65536), 32, Writter);
  WriteNBits(CS->nLines,               16, Writter);
  WriteNBits(MC->nModels,              16, Writter);
  for(n = 0 ; n < MC->nModels ; ++n){
    WriteNBits(MC->CM[n]->ctx,         16, Writter);
    WriteNBits(MC->CM[n]->alphaDen,    16, Writter);
    WriteNBits(MC->CM[n]->edits,        8, Writter);
    //WriteNBits(MC->CM[n]->TM->den,    32, Writter);
    WriteNBits(MC->CM[n]->ref,          1, Writter);
    WriteNBits(MC->CM[n]->vert,         1, Writter);
    }

  I[id].header = _bytes_output;

  uint32_t col = 0;
  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    for(idxPos = 0 ; idxPos < k ; ++idxPos){

      CalcProgress(size, ++i);

      symBuf->buf[symBuf->idx] = sym = AL->revMap[ readerBuffer[idxPos] ];
      memset((void *)PT->freqs, 0, AL->cardinality * sizeof(double));

      n = 0;
      pos = &symBuf->buf[symBuf->idx-1];
      for(model = 0 ; model < MC->nModels ; ++model){
        CModel *CM = MC->CM[model];

        if(CM->vert == 0){
          GetCModelIdx(pos, CM);
          }
        else{
          GetCModelIdx2D(CM, CS, col, AL);
          }
          
        ComputePModel(CM, pModel[n], CM->idx, CM->alphaDen);
        ComputeWeightedFreqs(WM->weight[n], pModel[n], PT, CM->nSym);
 
        if(CM->edits != 0){
          ++n;
          CM->TM->seq->buf[CM->TM->seq->idx] = sym;
          CM->TM->idx = GetCModelIdxCorr(CM->TM->seq->buf+
          CM->TM->seq->idx-1, CM, CM->TM->idx);

          ComputePModel(CM, pModel[n], CM->TM->idx, CM->TM->den);
          ComputeWeightedFreqs(WM->weight[n], pModel[n], PT, CM->nSym);
          }
        ++n;
        }

      ComputeMXProbs(PT, MX, AL->cardinality);
      AESym(sym, (int *)(MX->freqs), (int) MX->sum, Writter);
      CalcDecayment(WM, pModel, sym, P->gamma);

      for(n = MC->refNModels ; n < MC->nModels ; ++n)
        UpdateCModelCounter(MC->CM[n], sym, MC->CM[n]->idx);

      RenormalizeWeights(WM);

      n = 0;
      for(model = 0 ; model < MC->nModels ; ++model){
        if(MC->CM[model]->edits != 0)
          UpdateTolerantModel(MC->CM[model]->TM, pModel[++n], sym);
        ++n;
        }

      col = UpdateCache(CS, readerBuffer[idxPos], col);
      UpdateCBuffer(symBuf);
      }

  finish_encode(Writter);
  doneoutputtingbits(Writter);
  fclose(Writter);

  Free(name);
  Free(readerBuffer);

  for(n = 0 ; n < MC->refNModels ; ++n)
    ResetCModelIdx(MC->CM[n]);

  for(n = MC->refNModels ; n < MC->nModels ; ++n)
    RemoveCModel(MC->CM[n]);

  for(n = 0 ; n < MC->totalNPModels ; ++n)
    RemovePModel(pModel[n]);
  Free(pModel);

  RemovePModel(MX);
  RemoveWeightModel(WM);
  RemoveFPModel(PT);
  RemoveCBuffer(symBuf);
  RemoveCache(CS);

  fclose(Reader);
  int cardinality = AL->cardinality;
  RemoveAlphabet(AL);

  if(P->verbose == 1)
    fprintf(stderr, "Done!                          \n");  // SPACES ARE VALID 

  I[id].bytes = _bytes_output;
  I[id].size  = i;

  return cardinality;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - R E F E R E N C E - - - - - - - - - - - - -

void LoadReference(Parameters *P, MCLASS *MC){
  FILE      *Reader = Fopen(P->ref, "r");
  uint32_t  n, k, idxPos;
  uint64_t  nSymbols = 0, i = 0;
  uint8_t   *readerBuffer, sym;
  CBUF      *symBuf = CreateCBuffer(BUFFER_SIZE, BGUARD);

  if(P->verbose == 1)
    fprintf(stdout, "Building reference model ...\n");

  // BUILD ALPHABET
  ALPHABET *AL = CreateAlphabet(P->low);
  LoadAlphabet(AL, Reader);
  PrintAlphabet(AL);

  readerBuffer  = (uint8_t *) Calloc(BUFFER_SIZE + 1, sizeof(uint8_t));
  for(n = 0 ; n < MC->refNModels ; ++n)
    MC->CM[n] = CreateCModel(P->model[n].ctx, P->model[n].den, REFERENCE, 
                P->model[n].edits, P->model[n].eDen, AL->cardinality, 
                P->model[n].vert);

  nSymbols = NBytesInFile(Reader);

  P->checksum = 0;
  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    for(idxPos = 0 ; idxPos < k ; ++idxPos){

      symBuf->buf[symBuf->idx] = sym = AL->revMap[ readerBuffer[idxPos] ];
      P->checksum = (P->checksum + (uint8_t) sym);

      for(n = 0 ; n < MC->refNModels ; ++n){
        GetCModelIdx(symBuf->buf+symBuf->idx-1, MC->CM[n]);
        UpdateCModelCounter(MC->CM[n], sym, MC->CM[n]->idx);
        }

      UpdateCBuffer(symBuf);
      CalcProgress(nSymbols, ++i);
      }
 
  P->checksum %= CHECKSUMGF; 
  for(n = 0 ; n < MC->refNModels ; ++n)
    ResetCModelIdx(MC->CM[n]);
  Free(readerBuffer);
  RemoveCBuffer(symBuf);
  RemoveAlphabet(AL);
  fclose(Reader);

  if(P->verbose == 1)
    fprintf(stdout, "Done!                          \n");  // SPACES ARE VALID  
  else
    fprintf(stdout, "                               \n");  // SPACES ARE VALID
  }
  
//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[]){
  char        **p = *&argv, **xargv, *xpl = NULL;
  int32_t     xargc = 0, cardinality = 1;
  uint32_t    n, k;
  uint64_t    totalBytes, headerBytes, totalSize;
  clock_t     stop = 0, start = clock();
  double      gamma;
  MCLASS      *MC;
  Parameters  *P;
  INF         *I;

  P = (Parameters *) Malloc(1 * sizeof(Parameters));
  if((P->help = ArgsState(DEFAULT_HELP, p, argc, "-h")) == 1 || argc < 2){
    PrintMenu();
    return EXIT_SUCCESS;
    }

  if(ArgsState(DEF_VERSION, p, argc, "-V")){
    PrintVersion();
    return EXIT_SUCCESS;
    }

  if(ArgsState(0, p, argc, "-s")){
    PrintLevels(); 
    return EXIT_SUCCESS;
    }

  P->verbose = ArgsState  (DEFAULT_VERBOSE, p, argc, "-v" );
  P->force   = ArgsState  (DEFAULT_FORCE,   p, argc, "-f" );
  P->estim   = ArgsState  (0,               p, argc, "-e" );
  P->level   = ArgsNum    (0,   p, argc, "-l", MIN_LEVEL, MAX_LEVEL);
  P->low     = ArgsNum    (200, p, argc, "-t", MIN_THRESHOLD, MAX_THRESHOLD);

  P->nModels = 0;
  for(n = 1 ; n < argc ; ++n)
    if(strcmp(argv[n], "-rhm") == 0 || strcmp(argv[n], "-rvm") == 0 || 
       strcmp(argv[n], "-thm") == 0 || strcmp(argv[n], "-tvm") == 0)
      P->nModels += 1;

  if(P->nModels == 0 && P->level == 0)
    P->level = DEFAULT_LEVEL;
  
  if(P->level != 0){
    xpl = GetLevels(P->level);
    xargc = StrToArgv(xpl, &xargv);
    for(n = 1 ; n < xargc ; ++n)
      if(strcmp(xargv[n], "-rhm") == 0 || strcmp(xargv[n], "-thm") == 0 ||
         strcmp(xargv[n], "-rvm") == 0 || strcmp(xargv[n], "-tvm") == 0)
        P->nModels += 1;
    }

  if(P->nModels == 0){
    fprintf(stderr, "Error: at least you need to use a context model!\n");
    return 1;
    }

  P->model = (ModelPar *) Calloc(P->nModels, sizeof(ModelPar));

  MC = CreateMClass(P->nModels);

  k = 0;
  // LOAD REFERENCE MODELS FROM ARGS
  for(n = 1 ; n < argc ; ++n){
    if(strcmp(argv[n], "-rhm") == 0){
      P->model[k++] = ArgsUniqModel(argv[n+1], 1, 0);
      MC->refNModels++;
      }
    if(strcmp(argv[n], "-rvm") == 0){
      P->model[k++] = ArgsUniqModel(argv[n+1], 1, 1);
      MC->refNModels++;
      }
    }

  // LOAD REFERENCE MODELS FROM XARGS
  if(P->level != 0){
    for(n = 1 ; n < xargc ; ++n){
      if(strcmp(xargv[n], "-rhm") == 0){
        P->model[k++] = ArgsUniqModel(xargv[n+1], 1, 0);
        MC->refNModels++;
        }
      if(strcmp(xargv[n], "-rvm") == 0){
        P->model[k++] = ArgsUniqModel(xargv[n+1], 1, 1);
        MC->refNModels++;
        }
      }
    }

  // LOAD TARGET MODELS FROM ARGS
  for(n = 1 ; n < argc ; ++n){
    if(strcmp(argv[n], "-thm") == 0){
      P->model[k++] = ArgsUniqModel(argv[n+1], 0, 0);
      MC->tarNModels++;
      }
    if(strcmp(argv[n], "-tvm") == 0){
      P->model[k++] = ArgsUniqModel(argv[n+1], 0, 1);
      MC->tarNModels++;
      }
    }

  // LOAD TARGET MODELS FROM XARGS
  if(P->level != 0){
    for(n = 1 ; n < xargc ; ++n){
      if(strcmp(xargv[n], "-thm") == 0){
        P->model[k++] = ArgsUniqModel(xargv[n+1], 0, 0);
        MC->tarNModels++;
        }
      if(strcmp(xargv[n], "-tvm") == 0){
        P->model[k++] = ArgsUniqModel(xargv[n+1], 0, 1);
        MC->tarNModels++;
        }
      }
    } 

  gamma = DEFAULT_GAMMA;
  for(n = 1 ; n < xargc ; ++n) 
    if(strcmp(xargv[n], "-g") == 0) 
      gamma = atof(xargv[n+1]);

  P->gamma    = ArgsDouble (gamma, p, argc, "-g");
  P->gamma    = ((int)(P->gamma * 65536)) / 65536.0;
  P->ref      = ArgsString (NULL, p, argc, "-r");
  P->nTar     = ReadFNames (P, argv[argc-1]);
  P->checksum = 0;
  if(P->verbose) 
    PrintArgs(P);

  if(MC->refNModels != 0){
    if(P->ref == NULL){
      fprintf(stderr, "Error: using reference model(s) in nonexistent "
      "reference sequence!\n");
      exit(1);
      }
    LoadReference(P, MC);
    if(P->verbose)
      fprintf(stderr, "Checksum: %"PRIu64"\n", P->checksum);
    }

  I = (INF *) Calloc(P->nTar, sizeof(INF));

  totalSize   = 0;
  totalBytes  = 0;
  headerBytes = 0;
  for(n = 0 ; n < P->nTar ; ++n){
    cardinality = Compress(P, n, I, MC);
    totalSize   += I[n].size;
    totalBytes  += I[n].bytes;
    headerBytes += I[n].header;
    }

  if(P->nTar > 1)
    for(n = 0 ; n < P->nTar ; ++n){
      fprintf(stdout, "File %d compressed bytes: %"PRIu64" (", n+1, (uint64_t) 
      I[n].bytes);
      PrintHRBytes(I[n].bytes);
      fprintf(stdout, ") , Normalized Dissimilarity Rate: %.6g\n", 
      (8.0*I[n].bytes)/(log2(cardinality)*I[n].size));
      }


  fprintf(stdout, "Total bytes: %"PRIu64" (", totalBytes);
  PrintHRBytes(totalBytes);
  fprintf(stdout, "), %.4g bpb, %.4g bps w/ no header, Normalized Dissimilarity" 
  " Rate: %.6g\n", ((8.0*totalBytes)/totalSize), ((8.0*(totalBytes-headerBytes))
  /totalSize), (8.0*totalBytes)/(log2(cardinality)*totalSize));  
  stop = clock();
  fprintf(stdout, "Spent %g sec.\n", ((double)(stop-start))/CLOCKS_PER_SEC);

  RemoveMClass(MC);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
