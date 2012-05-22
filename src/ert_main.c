
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "ControlloTXRX.h"             /* Model's header file */
#include "rtwtypes.h"                  /* MathWorks types */

int_T main(int_T argc, const char_T *argv[])
{
  ControlloTXRX_initialize();
  sei();
  while (1) {
      TX();
      if(semaforo == 1) {
        ControlloTXRX_step();
        semaforo = 0;
      }
  }
  return 0;
}
