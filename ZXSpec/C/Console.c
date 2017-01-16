#include "Rsrc.h"
#include "Config.h"

void Console_Clear (unsigned char atr) __z88dk_fastcall;
void Console_WriteCh (unsigned char ch) __z88dk_fastcall; // Uses font 8x12 pixels
void Console_WriteLn (void);
void Console_WriteStrEx (unsigned char *str) __z88dk_fastcall;
//void Console__init (void);

extern unsigned char Console_x, Console_y, Console_atr;
/*================================== Header ==================================*/

void Console_Clear (unsigned char atr) __z88dk_fastcall {
__asm
  LD   IY,#0x5C3A
  LD   A,(_Console_attrib)
  PUSH AF
#ifdef __SDCC
  LD   HL,#4
  ADD  HL,SP
  LD   A,(HL)
#else
  LD   A,4(IX)
#endif
  CALL 0x229B
  LD   (_Console_attrib),A
  CALL 0xD6B // IX-safe
  POP  AF
  LD   (_Console_attrib),A
  RET
__endasm;
} //Console_Clear_ROM

/*--------------------------------- Cut here ---------------------------------*/
unsigned char Console_x, Console_y, Console_atr;

void Console_WriteCh (unsigned char ch) __z88dk_fastcall { // Uses font 8x12 pixels
  __asm
    LD   H,#0
    LD   E,L     ; L = ch
    LD   D,H
    ADD  HL,HL
    ADD  HL,DE
    ADD  HL,HL
    ADD  HL,HL   ; ch*12
    LD   DE,#Rsrc_Font - (32*12)
    ADD  HL,DE   ; + Rsrc_Font
    EX   DE,HL
    LD   HL,(_Console_y)
    LD   H,#Config_ScreenTable
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
    BIT  5,(HL)
    RET  Z
.globl WriteLn
WriteLn:
    LD   (HL),#0 ; Console_x = 0
    INC  HL
    LD   A,(HL)
    CP   #15*12 - 1
    JR   NC,Scroll$
    ADD  #12
    LD   (HL),A  ; Console_y += 12
    RET
Scroll$:
    LD   L,#12
    CALL _GrApp_ScrollUp
    LD   HL,#0x5800
    LD   DE,#0x5801
    LD   BC,#767
    LD   A,(_Console_atr)
    LD   (HL),A
    LDIR
  __endasm;
} //Console_WriteCh

/*--------------------------------- Cut here ---------------------------------*/
void Console_WriteLn (void) __naked {
  __asm
    LD   HL,#_Console_x
    JR   WriteLn
  __endasm;
} //Console_WriteLn

/*--------------------------------- Cut here ---------------------------------*/
void Console_WriteStrEx (unsigned char *str) __naked __z88dk_fastcall {
/*
  unsigned int i = 0;
  while (str[i] != 0x00) { Console_WriteCh(str[i]); i += 1; }
*/
  __asm
NextCh$:
    LD   A,(HL)
    OR   A
    RET  Z
    PUSH HL
    LD   L,A
    CALL _Console_WriteCh
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
