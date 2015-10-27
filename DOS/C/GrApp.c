/*       Ver 1.0.0 for DOS/CGA       */
/* Use DosDev to compile this module */
#include <dos.h>
#include "Timer.h"

void GrApp_Close (void);
void GrApp__init (void);
/*================================== Header ==================================*/

static char grapp_init = 0;

void GrApp__init (void) {
  if (grapp_init != 0) return; /* ONCE!!! */
  grapp_init = 1;

    geninterrupt(0x11);
    if ((_AX & 0x30) == 0x30) exit(100); /* No hardware for CGA graphics. */

    _AX = 0x0004; geninterrupt(0x10);	/*Grafix 320x200 4 colors*/
  Timer__init();

} /*GrApp__init*/

/*--------------------------------- Cut here ---------------------------------*/
void GrApp_Close (void) {
  if (grapp_init) { grapp_init = 0;
  Timer_Close(); /*
       Процессор NTVDM обнаружил недопустимую инструкцию.
       CS:0000 IP:0a84 OP:ff ff ff ff a9 */
  _AX = 0x0003; geninterrupt(0x10);	/*Text 80x25 16 colors*/
 }
} /*GrApp__Close*/
