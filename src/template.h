#ifndef TEMPLATE_H_INCLUDED
#define TEMPLATE_H_INCLUDED

#include "defs.h"
#include "template.h"

typedef struct{
  int        row;
  int        col;
  }
COORDS;

typedef struct{
  int        id;
  int        size;
  COORDS     *position;
  }
TEMPLATE2D;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

TEMPLATE2D      CreateCTemplate2D    (int);
void            PrintCTemplate2D     (TEMPLATE2D);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
