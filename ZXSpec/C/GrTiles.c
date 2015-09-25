#include "SYSTEM.h"
#include "GrCfg.h"

export void GrTiles_PutTile (SHORTINT x, SHORTINT y, BYTE* tile)
{{
/* Попробуй ускорить с помощью LDI */
/*
  unsigned char i; int* spr_addr;

  IF ((unsigned char)x <= 30) && ((unsigned char)y <= 30) THEN
    y *= 6;
    spr_addr = (int*)((unsigned int)spr * Resource_SpriteSize) + Resource_Sprites;
    FOR i = 12; i--; DO
      __PUT(PIXEL_ADD_192(y++) + x, *spr_addr++, int);
    END
  END
*/
  __asm
#ifdef __SDCC
  PUSH IX
  LD   IX,#0
  ADD  IX,SP
#endif
    LD   A,#0x1E
    CP   A,4(IX) ; x
    RET  C       ; IF x <= 30 THEN RETURN
    LD   C,5(IX) ; y
    CP   A,C
    RET  C       ; IF y <= 30 THEN RETURN
    LD   E,6(IX) ; tile
    LD   D,7(IX)
    LD   A,C
    ADD  A
    ADD  C
    ADD  A
    LD   L,A     ; HL = #TAB[y * 6]
    LD   H,#GrCfg_ScreenTable
; #1
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC   C
    INC   DE
    LD    A,(DE)
    LD    (BC),A
    INC   DE
    INC   L
; #2
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #3
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #4
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #5
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #6
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #7
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #8
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #9
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #10
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #11
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #12
    LD   A,4(IX)
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    LD   A,(DE)  ; Sprite addr
    LD   (BC),A  ; Screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
#ifdef __SDCC
  POP  IX
#endif
  __endasm;
}} /*GrTiles_PutTile*/

