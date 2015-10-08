#include "GrCfg.h"

void GrTiles_PutTile (unsigned char x, unsigned char y, unsigned char* tile);
/*================================== Header ==================================*/

void GrTiles_PutTile (unsigned char x, unsigned char y, unsigned char* tile) {
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
    POP  BC
    POP  DE      ; D = y; E = x
    POP  HL      ; HL = tile address
    PUSH HL
    PUSH DE
    PUSH BC
#ifdef GrCfg_CheckTileCoords
    LD   A,#0x1E
    CP   A,E     ; IF x <= 30 THEN RETURN
    RET  C
    CP   A,D     ; IF y <= 30 THEN RETURN
    RET  C
#endif
    LD   A,D     ; y
    ADD  A
    ADD  D       ; * 6
    ADD  A
    LD   C,A     ; BC = #TAB[y * 6]
    LD   B,#GrCfg_ScreenTable
    LD   A,(BC)
    ADD  E       ; + x
    LD   E,A     ; low screen addr byte
    INC  B
    LD   A,(BC)
    LD   D,A     ; high screen addr byte
; #1
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
; #2
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #3
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
; #4
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #5
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
; #6
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #7
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
; #8
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #9
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
; #10
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #11
    LDI          ; LD (DE),(HL): INC HL: INC DE: DEC BC
    LDI
    DEC  DE      ; calculate next screen addr
    DEC  DE
    INC  D
; #12
    LDI          ; 16t
    LD   A,(HL)  ; 7t
    LD   (DE),A  ; 7t
    RET
DOWN_DE8$:
    LD   A,E
    ADD  #32
    LD   E,A
    RET  C
    LD   A,D
    SUB  #8
    LD   D,A
  __endasm;
} //GrTiles_PutTile
