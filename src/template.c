#include <stdio.h>
#include <stdlib.h>
#include "template.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TEMPLATES COORDINATES
//

//
//            1
//            X  
//
static COORDS template1[] = {{1, 0}};

/*
//
//            
//          1 X  
//
static COORDS template1[] = {{0, -1}};
*/

//
//          1  
//            X  
//
static COORDS template2[] = {{1, -1}};

//
//            2
//            1
//            X
//
static COORDS template3[] = {{1, 0}, {2, 0}};

//
//            3
//            2
//            1
//            X
//
static COORDS template4[] = {{1, 0}, {2, 0}, {3, 0}};

//
//            4
//            3
//            2
//            1
//            X
//
static COORDS template5[] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}};

//
//            5
//            4
//            3
//            2
//            1
//            X
//
static COORDS template6[] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}};

//
//            6
//            5
//            4
//            3
//            2
//            1
//            X
//
static COORDS template7[] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6,0}};

//
//            7
//            6
//            5
//            4
//            3
//            2
//            1
//            X
//
static COORDS template8[] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6,0}, {7, 0}};

//
//            8
//            7
//            6
//            5
//            4
//            3
//            2
//            1
//            X
//
static COORDS template9[] = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6,0}, {7, 0}, {8, 0}};

//
//         2
//         1  
//            X
//
static COORDS template10[] = {{1, -1}, {2, -1}};

//
//         
//      2  1  
//            X
//
static COORDS template11[] = {{1, -1}, {1, -2}};

//
//         
//               1  2
//            X
//
static COORDS template12[] = {{1, 1}, {1, 2}};

//
//            2
//         1  X
//
static COORDS template13[] = {{0, -1}, {1, 0}};

//
//         3  2
//         1  X
//
static COORDS template14[] = {{0, -1}, {1, 0}, {1, -1}};

//
//         3  2  4
//         1  X
//
static COORDS template15[] = {{0, -1}, {1, 0}, {1, -1}, {1, 1}};

//
//         3  2  4
//      5  1  X
//
static COORDS template16[] = {{0, -1}, {1, 0}, {1, -1}, {1, 1}, {0, -2}};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

TEMPLATE2D CreateCTemplate2D(int id){
  TEMPLATE2D T;

  switch(id){
    case 1:
      T.position = template1;
      T.size = sizeof(template1) / sizeof(template1[0]);
    break;
    case 2:
      T.position = template2;
      T.size = sizeof(template2) / sizeof(template2[0]);
    break;
    case 3:
      T.position = template3;
      T.size = sizeof(template3) / sizeof(template3[0]);
    break;
    case 4:
      T.position = template4;
      T.size = sizeof(template4) / sizeof(template4[0]);
    break;
    case 5:
      T.position = template5;
      T.size = sizeof(template5) / sizeof(template5[0]);
    break;
    case 6:
      T.position = template6;
      T.size = sizeof(template6) / sizeof(template6[0]);
    break;
    case 7:
      T.position = template7;
      T.size = sizeof(template7) / sizeof(template7[0]);
    break;
    case 8:
      T.position = template8;
      T.size = sizeof(template8) / sizeof(template8[0]);
    break;
    case 9:
      T.position = template9;
      T.size = sizeof(template9) / sizeof(template9[0]);
    break;
    case 10:
      T.position = template10;
      T.size = sizeof(template10) / sizeof(template10[0]);
    break;
    case 11:
      T.position = template11;
      T.size = sizeof(template11) / sizeof(template11[0]);
    break;
    case 12:
      T.position = template12;
      T.size = sizeof(template12) / sizeof(template12[0]);
    break;
    case 13:
      T.position = template13;
      T.size = sizeof(template13) / sizeof(template13[0]);
    break;
    case 14:
      T.position = template14;
      T.size = sizeof(template14) / sizeof(template14[0]);
    break;
    case 15:
      T.position = template15;
      T.size = sizeof(template15) / sizeof(template15[0]);
    break;
    case 16:
      T.position = template16;
      T.size = sizeof(template16) / sizeof(template16[0]);
    break;
    default:
      fprintf(stderr, "Error: invalid template id\n");
      exit(1);
    }

  T.id = id;
  return T;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void PrintCTemplate2D(TEMPLATE2D T){
  uint32_t x;
  for(x = 0 ; x < T.size ; ++x)
    fprintf(stderr, "(%d,%d), ", T.position[x].row, T.position[x].col);
  fprintf(stderr, "\n");
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
void RemoveCTemplate2D(TEMPLATE2D *T){
  Free(T);
  }
*/
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

