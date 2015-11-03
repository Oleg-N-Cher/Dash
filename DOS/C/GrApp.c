/*       Ver 1.0.0 for DOS/CGA       */
/* Use DosDev to compile this module */
#include <dos.h>
#include "Timer.h"

void GrApp_Cls (void);
void GrApp_ScrollDown (unsigned char lines);
void GrApp_ScrollUp (unsigned char lines);
void GrApp__init (void);
void GrApp_Close (void);
/*================================== Header ==================================*/

static void lmove (unsigned to, unsigned from) {
/*Пересылка (копирование) линии from в to*/

 struct line {char __[80];};
 struct line _es * Lto;
 struct line _es * Lfrom;

 if (to == from || !(to < 192 && from < 192))
  return;
 _ES = 0xB800;
 to += 4;
 from += 4;
 Lto = (struct line _es *)((to >> 1) * 80 + ((to & 1) << 13));
 Lfrom = (struct line _es *)((from >> 1) * 80 + ((from & 1) << 13));
 *Lto = *Lfrom;
} /*lmove*/

static void lclr (unsigned line) {
/*Очистка линии экрана с данным номером*/

 unsigned e = line;
 long _es* L;

 if (e < 192) {
  e += 4;
  _ES = 0xB800;
  L = (long _es*)((e >> 1) * 80 + ((e & 1) << 13));
  for (e = 20; e; e--)
   *L ++ = 0;
   }
} /*lclr*/

void GrApp_Cls (void) {

 unsigned char _es* i;
 unsigned j;

 _ES = 0xB800;
 i = 0;
 for (j = 17; j; j--) {
  Timer_Start (1);
  while ((unsigned)i < 16384)
   *i = 0, i += 17;
  i = (unsigned char _es*)(0x3FFF & (unsigned)i);
  Timer_Until ();
   }
 /*ttyx = ttyy = 0;*/
} /*GrApp_Cls*/

void scroll (signed lines)
/* +UP, -DOWN */
{{
 unsigned media, line;

 if (lines == 0)
  return;
 if (abs (lines) >= 192) {
  GrApp_Cls ();
  return;
   }
 if (lines > 0) {
  media = 192 - lines;
  for (line = 0; line < media; line ++)
   lmove (line, line + lines);
  while (line < 192)
   lclr (line ++);
   }
 else {
  media = -lines;
  for (line = 191; line >= media; line --)
   lmove (line, line + lines);
  while (line < 192)
   lclr (line --);
   }
}}

void GrApp_ScrollUp (unsigned char lines) {
  scroll((signed)lines);
} /*GrApp_ScrollUp*/

void GrApp_ScrollDown (unsigned char lines) {
  scroll(-(signed)lines);
} /*GrApp_ScrollDown*/

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
