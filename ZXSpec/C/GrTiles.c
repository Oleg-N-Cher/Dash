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
#ifdef __test_atrib__
    LD HL,#0x5800
    LD DE,#0x5801
    LD BC,#0x100
    LD (HL),#4*8
    LDIR
    LD BC,#0x100
    LD (HL),#5*8
    LDIR
    LD BC,#0xFF
    LD (HL),#6*8
    LDIR
#endif //__test_atrib__
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
#endif //GrCfg_CheckTileCoords
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
#ifndef GrCfg_FastTiles
    LD   D,A     ; high screen addr byte
; #1
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
; #2
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #3
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
; #4
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #5
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
; #6
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #7
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
; #8
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #9
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
; #10
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LD   A,D
    AND  #7
    CALL Z,DOWN_DE8$
; #11
    LDI
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
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
#else //GrCfg_FastTiles
    BIT  1,D     ; y&2
    JP   NZ,Y_MOD_2_3$
Y_MOD_0_1$:
    BIT  0,D     ; y&1
    LD   D,A     ; high screen addr byte
    JR   NZ,Y_MOD_1$
Y_MOD_0$: // y = {0, 4, 8, 12, 16, 20, 24, 28}
    LDI          ; #1
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #2
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #3
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #4
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #5
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #6
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #7
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #8
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    INC  D
    LD   A,E
    ADD  #32-1
    LD   E,A
    JR   C,Y20$
    LD   A,D
    SUB  #8
    LD   D,A
Y20$:
    LDI          ; #9
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #10
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #11
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #12 ; 16t
    LD   A,(HL)  ; 7t
    LD   (DE),A  ; 7t
    RET
Y_MOD_1$: // y = {1, 5, 9, 13, 17, 21, 25, 29}
    LDI          ; #1
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #2
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    INC  D
    LD   A,E
    ADD  #32-1
    LD   E,A
    JR   C,Y21$
    LD   A,D
    SUB  #8
    LD   D,A
Y21$:
    LDI          ; #3
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #4
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #5
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #6
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #7
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #8
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #9
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #10
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    INC  D
    LD   A,E
    ADD  #32-1
    LD   E,A
    JR   C,Y9$
    LD   A,D
    SUB  #8
    LD   D,A
Y9$:
    LDI          ; #11
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #12 ; 16t
    LD   A,(HL)  ; 7t
    LD   (DE),A  ; 7t
    RET
Y_MOD_2_3$:
    BIT  0,D     ; y&1
    LD   D,A     ; high screen addr byte
    JR   NZ,Y_MOD_3$
Y_MOD_2$: // y = {2, 6, 10, 14, 18, 22, 26, 30}
    LDI          ; #1
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #2
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #3
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #4
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    INC  D
    LD   A,E
    ADD  #32-1
    LD   E,A
    JR   C,Y10$
    LD   A,D
    SUB  #8
    LD   D,A
Y10$:
    LDI          ; #5
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #6
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #7
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #8
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #9
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #10
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #11
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #12 ; 16t
    LD   A,(HL)  ; 7t
    LD   (DE),A  ; 7t
    RET
Y_MOD_3$: // y = {3, 7, 11, 15, 19, 23, 27}
    LDI          ; #1
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #2
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #3
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #4
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #5
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #6
    LDI
    LD   A,E
    ADD  #32-2
    LD   E,A
    LD   A,D
    SUB  #8-1
    LD   D,A
    LDI          ; #7
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #8
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #9
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #10
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #11
    LD   A,(HL)
    LD   (DE),A
    INC  HL
    DEC  E
    INC  D
    LDI          ; #12 ; 16t
    LD   A,(HL)  ; 7t
    LD   (DE),A  ; 7t
#endif //GrCfg_FastTiles
  __endasm;
} //GrTiles_PutTile
