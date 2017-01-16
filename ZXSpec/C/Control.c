#include "GrApp.h"

void Control_ChangePalette (void) __preserves_regs(iyl,iyh);
unsigned char Control_Get (void);
unsigned char Control_GetCustom (void);
unsigned char Control_PressedAnyKey (void);
unsigned char Control_ReadKey (void);
void Control_Select (unsigned char n) __z88dk_fastcall;
/*================================== Header ==================================*/

void Control_ChangePalette (void) __preserves_regs(iyl,iyh) {
  __asm
    LD   A,#0xF7
    IN   A,(#0xFE)
    CPL
    AND  #3       ; 00000021
    RET  Z        ; '1' & '2' not pressed
    LD   B,A
    AND  #1       ; '1'
    JR   Z,PAL_KEY2$
PAL_KEY1$:
NEXT_INK$:        ; Change ink
    LD   HL,#_GrApp_ink
    LD   A,(HL)
    INC  A
    AND  #7
    LD   (HL),A
    LD   A,(_GrApp_paper)
    AND  #7
    CP   (HL)     ; Ink must be # Paper
    JR   Z,NEXT_INK$
PAL_KEY2$:
    BIT  1,B      ; '2'
    JR   Z,SET_PALETTE$
NEXT_PAPER$:      ; Change paper
    LD   HL,#_GrApp_paper
    LD   A,(HL)
    INC  A
    AND  #0xF
    LD   (HL),A
    DEC  HL
    AND  #7
    CP   (HL)     ; Ink must be # Paper
    JR   Z,NEXT_PAPER$
SET_PALETTE$:
    CALL _GrApp_SetPalette
    SCF           ; Set flag C if pallette was changed
  __endasm;
} //Control_ChangePalette

/*--------------------------------- Cut here ---------------------------------*/
unsigned char Control_PressedAnyKey (void) {
  __asm
    CALL  _Control_ChangePalette
    JR    C,RET_FALSE$ ; RETURN FALSE if palette was changed
    CALL  0x28E        ; Scan keys
    INC   DE
    LD    A,E
    OR    D
    LD    L,#1         ; TRUE
    RET   NZ
RET_FALSE$:
    LD    L,#0         ; FALSE
  __endasm;
} //Control_PressedAnyKey

/*--------------------------------- Cut here ---------------------------------*/
unsigned char Control_ReadKey (void) { // With auto-repeat
__asm
  LD   IY,#0x5C3A
  RES  5,1(IY)
  IM   1
LOOP_REPEAT$:
  BIT  5,1(IY)
  JR   Z,LOOP_REPEAT$
  LD   L,-50(IY)
  IM   2
__endasm;
} //Input_Read_Repeat

/*--------------------------------- Cut here ---------------------------------*/
unsigned char Control_Get (void) __naked {
__asm
/* Из журнала Deja Vu #09, Кемерово, 1999
 (C) Rezonance group
 Добавлен опрос Kempston-джойстика */

;Кроме стандартных Q, A, O, P, E, здесь
;опрашиваются Sinclair 1/2 и Kempston. На выходе
;в регистре 'L' в формате кемпстон джойстика
;мы имеем требуемое направление движения.

KEY1A$: LD    HL,#KEY1F$
        LD    C,#0
        LD    A,(HL)
KEY1B$: INC   HL
        IN    A,(#0xFE)
        AND   (HL)
        INC   HL
        JR    NZ,KEY1C$
        LD    A,(HL)
        OR    C
        LD    C,A
KEY1C$: INC   HL
        LD    A,(HL)
        AND   A
        JR    NZ,KEY1B$
        LD    L,C
.globl __KEMPSTON
__KEMPSTON:
        .DB   0xC9  ; RET or IN
#ifdef __detect_kempston__
        .DB   0x1F
#endif //__detect_kempston__
CustKempston$:
        AND   #0x1F
        OR    L
        LD    L,A
        RET
KEY1F$: ;Ст.байт порта,бит кнопки,бит результата
        .DB   0xDF,0x01,0x01 ;P
        .DB   0xEF,0x08,0x01 ;7
;       .DB   0xF7,0x02,0x01 ;2
        .DB   0xDF,0x02,0x02 ;O
        .DB   0xEF,0x10,0x02 ;6
;       .DB   0xF7,0x01,0x02 ;1
        .DB   0xFD,0x01,0x04 ;A
        .DB   0xEF,0x04,0x04 ;8
;       .DB   0xF7,0x04,0x04 ;3
        .DB   0xFB,0x01,0x08 ;Q
        .DB   0xEF,0x02,0x08 ;9
;       .DB   0xF7,0x08,0x08 ;4
        .DB   0x7F,0x04,0x10 ;M
        .DB   0xEF,0x01,0x10 ;0
;       .DB   0xF7,0x10,0x10 ;5
        .DB   0xFB,0x04,0x20 ;E
        .DB   0
__endasm;
} //Control_Get

unsigned char Control_GetCustom (void) __naked {
           Control_ChangePalette();
__asm
Custom$:   JR    KEYBOARD$
KEYBOARD$: LD    HL,#KeyboardKeys$
           JR    KEY1A$+3
KEMPSTON$: XOR   A
           IN    A,(#0x1F)
           JR    CustKempston$
SINCLAIR$: LD    HL,#SinclairKeys$
           JR    KEY1A$+3
CURSOR$:   LD    HL,#CursorKeys$
           JR    KEY1A$+3
KeyboardKeys$:
        .DB   0xDF,0x01,0x01 ;P
        .DB   0xDF,0x02,0x02 ;O
        .DB   0xFD,0x01,0x04 ;A
        .DB   0xFB,0x01,0x08 ;Q
        .DB   0x7F,0x04,0x10 ;M
        .DB   0xFB,0x04,0x20 ;E
        .DB   0
SinclairKeys$:
        .DB   0xEF,0x08,0x01 ;7
        .DB   0xEF,0x10,0x02 ;6
        .DB   0xEF,0x04,0x04 ;8
        .DB   0xEF,0x02,0x08 ;9
        .DB   0xEF,0x01,0x10 ;0
        .DB   0xFB,0x04,0x20 ;E
        .DB   0
CursorKeys$:
        .DB   0xEF,0x04,0x01 ;8
        .DB   0xF7,0x10,0x02 ;5
        .DB   0xEF,0x10,0x04 ;6
        .DB   0xEF,0x08,0x08 ;7
        .DB   0xEF,0x01,0x10 ;0
        .DB   0xFB,0x04,0x20 ;E
        .DB   0
__endasm;
} //Control_GetCustom

void Control_Select (unsigned char n) __z88dk_fastcall {
__asm
        LD    A,L
        ADD   A
        ADD   A
        ADD   L
        LD    (Custom$+1),A
__endasm;
} //Control_Select

#ifdef __detect_kempston__
void Control__init (void) {
__asm
; AUTOconfig
      LD     HL,#__KEMPSTON
      LD     (HL),#0xC9   ;RET
      LD     B,#250
A_config01$:
      IN     A,(#0x1F)
      AND    #0x1F
      RET    NZ
      DJNZ   A_config01$
      LD     (HL),#0xDB   ;IN
__endasm;
} //Control__init
#endif //__detect_kempston__
