#include "msg.h"
#include <stdio.h>
#include <stdlib.h>

void ModelsExplanation(void){
  fprintf(stderr,
  "                                                                         \n"
  "  -rhm <c>:<d>:<m/e>     reference horizontal model (ex:-rhm 13:100:0/0),\n"
  "  -rvm <t>:<d>:<m/e>     reference vertical model (ex:-rvm 4:1000:1/50), \n"
  "  ...                                                                    \n"
  "  -thm <c>:<d>:<m/e>     target horizontal model (ex:-thm 4:1:0/0),      \n"
  "  -tvm <t>:<d>:<m/e>     target vertical model (ex:-tvm 3:20:1/10),      \n"
  "  ...                                                                    \n"
  "                         target and reference templates use <c> for      \n"
  "                         context-order size, <t> used for vertical       \n"
  "                         templates, <d> for alpha (1/<d>), <m> to the    \n"
  "                         maximum sets the substitutions, on the context  \n"
  "                         without being discarded (for deep contexts),    \n"
  "                         under the estimator <e>,                        \n");
  } 

void PrintMenu(void){
  fprintf(stderr,
  "Usage: XC [OPTIONS]... -r [FILE]  [FILE]:[...]                         \n"
  "Compress and analyze a sequence (by default, compress).                \n"
  "                                                                       \n"
  "Non-mandatory arguments:                                               \n"
  "                                                                       \n"
  "  -h                     give this help,                               \n"
  "  -s                     show XC compression levels,                   \n"
  "  -v                     verbose mode (more information),              \n"
  "  -V                     display version number,                       \n"
  "  -f                     force overwrite of output,                    \n"
  "  -l <level>             level of compression [1;5] (lazy -r*m setup), \n"
  "  -t <threshold>         threshold frequency to discard from alphabet, \n"
  "  -g <gamma>             mixture decayment forgetting factor. It is    \n"
  "                         a real value in the interval [0;1),           \n");
  #ifdef ESTIMATE
  fprintf(stderr,
  "  -e                     it creates a file with the extension \".iae\" \n"
  "                         with the respective information content.      \n");
  #endif
  ModelsExplanation();
  fprintf(stderr,
  "                                                                       \n"
  "  -r <FILE>              reference file (\"-r*m\" are loaded here),    \n"
  "                                                                       \n"
  "Mandatory arguments:                                                   \n"
  "                                                                       \n"
  "  <FILE>                 file to compress (last argument). For more    \n"
  "                         files use splitting \":\" characters.         \n"
  "                                                                       \n"
  "Report bugs to pratas@ua.pt.                                           \n");
  }

void PrintMenuD(void){
  fprintf(stderr,
  "Usage: XD [OPTIONS]... -r [FILE]  [FILE]:[...]                      \n"
  "Decompress a text sequence compressed by XC.                        \n"
  "                                                                    \n"
  "Non-mandatory arguments:                                            \n"
  "                                                                    \n"
  "  -h                     give this help,                            \n"
  "  -v                     verbose mode (more information),           \n"
  "                                                                    \n"
  "  -r <FILE>              reference file,                            \n"
  "                                                                    \n"
  "Mandatory arguments:                                                \n"
  "                                                                    \n"
  "  <FILE>                 file to uncompress (last argument). For    \n"
  "                         more files use splitting \":\" characters. \n"
  "                                                                    \n"
  "Report bugs to pratas@ua.pt>.                                       \n");
  }

void PrintVersion(void){
  fprintf(stderr,
  "                                                                       \n"
  "                          ================                             \n"
  "                          |    XC  %u.%u   |                           \n"
  "                          ================                             \n"
  "                                                                       \n"
  "Copyright (C) 2017-2018 University of Aveiro. This is a Free software. \n"
  "You may redistribute copies of it under the terms of the GNU - General \n"
  "Public License v3 <http://www.gnu.org/licenses/gpl.html>. There is NOT \n"
  "ANY WARRANTY, to the extent permitted by law. Developed and Written by \n"
  "Diogo Pratas.\n\n", VERSION,
  RELEASE);
  }

