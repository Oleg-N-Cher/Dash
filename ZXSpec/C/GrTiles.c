#include "SYSTEM.h"
#include "GrCfg.h"

export void GrTiles_PutTile (SHORTINT x, SHORTINT y, BYTE* tile) {
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
    POP  HL
    POP  BC      ; B = y; C = x
    POP  DE      ; DE = tile address
    PUSH DE
    PUSH BC
    PUSH HL
#ifdef GrCfg_CheckTileCoords
    LD   A,#0x1E
    CP   A,C     ; IF x <= 30 THEN RETURN
    RET  C
    CP   A,B     ; IF y <= 30 THEN RETURN
    RET  C
#endif
    LD   A,B     ; y
    ADD  A
    ADD  B
    ADD  A
    LD   L,A     ; HL = #TAB[y * 6]
    LD   H,#GrCfg_ScreenTable
    LD   A,C     ; x
    PUSH AF
; #1
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #2
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #3
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #4
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #5
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #6
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #7
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #8
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #9
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #10
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #11
    POP  AF      ; x
    PUSH AF
    ADD  (HL)
    LD   C,A
    INC  H
    LD   B,(HL)
    DEC  H
    LD   A,(DE)  ; tile addr
    LD   (BC),A  ; screen addr
    INC  C
    INC  DE
    LD   A,(DE)
    LD   (BC),A
    INC  DE
    INC  L
; #12
    POP  AF      ; x
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
  __endasm;
} //GrTiles_PutTile
