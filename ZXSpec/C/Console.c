#include "Rsrc.h"
#include "GrCfg.h"

void Console_WriteCh (unsigned char ch); // Uses font 8x12 pixels
void Console_WriteStrEx (unsigned char *str);
void Console__init (void);

extern unsigned char Console_x, Console_y, Console_atr;
/*================================== Header ==================================*/

unsigned char Console_x, Console_y, Console_atr;

void Console_WriteCh (unsigned char ch) { // Uses font 8x12 pixels
  __asm
    POP  DE
    POP  HL      ; L = ch
    PUSH HL
    PUSH DE
    LD   H,#0
    LD   E,L     ; ch
    LD   D,H
    ADD  HL,HL
    ADD  HL,DE
    ADD  HL,HL
    ADD  HL,HL   ; ch*12
    LD   DE,#Rsrc_Font - (32*12)
    ADD  HL,DE   ; + Rsrc_Font
    EX   DE,HL
    LD   HL,(_Console_y)
    LD   H,#GrCfg_ScreenTable
    LD   A,#12
DrawLine$:
    EX   AF,AF
    LD   A,(_Console_x)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Character addr
    LD   (BC),A  ; Screen addr
    INC  DE
    INC  L
    EX   AF,AF
    DEC  A
    JR   NZ,DrawLine$
    LD   A,B     ; Calculate attribute address
    RRCA
    RRCA
    RRCA
    AND  #3
    OR   #0x58
    LD   B,A
    LD   A,(_Console_atr)
    LD   (BC),A
    LD   HL,#-32
    ADD  HL,BC
    LD   (HL),A  ; Set color to attributes
    LD   HL,#_Console_x
    INC  (HL)    ; Console_x++
  __endasm;
} //Console_WriteCh

/*--------------------------------- Cut here ---------------------------------*/
void Console_WriteStrEx (unsigned char *str) __naked {
/*
  unsigned int i = 0;
  while (str[i] != 0x00) { Console_WriteCh(str[i]); i += 1; }
*/
  __asm
    POP  DE
    POP  HL
    PUSH HL
    PUSH DE
NextCh$:
    LD   A,(HL)
    OR   A
    RET  Z
    PUSH HL
    PUSH AF
    INC  SP
    CALL _Console_WriteCh
    INC  SP
    POP  HL
    INC  HL
    JR   NextCh$
  __endasm;
} //Console_WriteStr

/*--------------------------------- Cut here ---------------------------------*/
/*
void Console__init (void) {
  Console_x = 0; Console_y = 0; Console_atr = 0x7;
}
*/
