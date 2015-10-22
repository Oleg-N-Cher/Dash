/* Ver 1.0.0 for ZX Spectrum 48/128 */
/* Use ZXDev to compile this module */

#include "Timer.h"
#include "Config.h"

void GrApp_Close (void);
void GrApp_Cls (void);
void GrApp_ScrollDown (unsigned char lines);
void GrApp_ScrollUp (unsigned char lines);
void GrApp_SetPalette (void);
void GrApp__init (void);

extern unsigned char GrApp_ink, GrApp_paper;
/*================================== Header ==================================*/

static void MoveLineCtoL (void /* A=from; L=to */) {
  __asm
    LD   H,#Config_ScreenTable
    LD   E,(HL)
    INC  H
    LD   D,(HL) ; DE = to
    LD   L,C
    LD   A,(HL)
    DEC  H
    LD   L,(HL)
    LD   H,A    ; HL = from
    LDI         ; #1
    LDI         ; #2
    LDI         ; #3
    LDI         ; #4
    LDI         ; #5
    LDI         ; #6
    LDI         ; #7
    LDI         ; #8
    LDI         ; #9
    LDI         ; #10
    LDI         ; #11
    LDI         ; #12
    LDI         ; #13
    LDI         ; #14
    LDI         ; #15
    LDI         ; #16
    LDI         ; #17
    LDI         ; #18
    LDI         ; #19
    LDI         ; #20
    LDI         ; #21
    LDI         ; #22
    LDI         ; #23
    LDI         ; #24
    LDI         ; #25
    LDI         ; #26
    LDI         ; #27
    LDI         ; #28
    LDI         ; #29
    LDI         ; #30
    LDI         ; #31
    LD   A,(HL)
    LD   (DE),A ; #31
  __endasm;
} //MoveLineCtoL

/*--------------------------------- Cut here ---------------------------------*/
static void ClearLineL (void /*register L*/) {
  __asm
    LD   H,#Config_ScreenTable
    LD   E,(HL)
    INC  H
    LD   D,(HL)
    XOR  A
    LD   (DE),A ; #1
    INC  DE
    LD   (DE),A ; #2
    INC  DE
    LD   (DE),A ; #3
    INC  DE
    LD   (DE),A ; #4
    INC  DE
    LD   (DE),A ; #5
    INC  DE
    LD   (DE),A ; #6
    INC  DE
    LD   (DE),A ; #7
    INC  DE
    LD   (DE),A ; #8
    INC  DE
    LD   (DE),A ; #9
    INC  DE
    LD   (DE),A ; #10
    INC  DE
    LD   (DE),A ; #11
    INC  DE
    LD   (DE),A ; #12
    INC  DE
    LD   (DE),A ; #13
    INC  DE
    LD   (DE),A ; #14
    INC  DE
    LD   (DE),A ; #15
    INC  DE
    LD   (DE),A ; #16
    INC  DE
    LD   (DE),A ; #17
    INC  DE
    LD   (DE),A ; #18
    INC  DE
    LD   (DE),A ; #19
    INC  DE
    LD   (DE),A ; #20
    INC  DE
    LD   (DE),A ; #21
    INC  DE
    LD   (DE),A ; #22
    INC  DE
    LD   (DE),A ; #23
    INC  DE
    LD   (DE),A ; #24
    INC  DE
    LD   (DE),A ; #25
    INC  DE
    LD   (DE),A ; #26
    INC  DE
    LD   (DE),A ; #27
    INC  DE
    LD   (DE),A ; #28
    INC  DE
    LD   (DE),A ; #29
    INC  DE
    LD   (DE),A ; #30
    INC  DE
    LD   (DE),A ; #31
    INC  DE
    LD   (DE),A ; #32
  __endasm;
} //ClearLineN

/*--------------------------------- Cut here ---------------------------------*/
void GrApp_ScrollUp (unsigned char lines) {
/*
  unsigned media, line;
  if (lines > 0) {
    media = 192 - lines;
    for (line = 0; line < media; line ++)
      lmove (line, line + lines);
    while (line < 192)
      lclr (line ++);
  }
*/
  __asm
    POP  HL
    POP  BC     ; C = lines
    PUSH BC
    PUSH HL
    LD   A,#192
    SUB  C
    LD   B,A    ; B = media
    LD   L,#0   ; L = line
LMOVE_UP$:
    PUSH BC
    PUSH HL
    CALL _MoveLineCtoL
    POP  HL
    POP  BC
    INC  L      ; line++
    INC  C      ; (line + lines)++
    DJNZ LMOVE_UP$
    LD   A,#192
    SUB  L
    LD   B,A
LCLR_UP$:
    CALL _ClearLineL
    INC  L      ; line++
    DJNZ LCLR_UP$
  __endasm;
} //GrApp_ScrollUp

/*--------------------------------- Cut here ---------------------------------*/
void GrApp_ScrollDown (unsigned char lines) {
/*
  for (line = 191; line >= lines; line --)
    lmove (line, line - lines);
  while (line < 192)
    lclr (line --);
*/
  __asm
    POP  HL
    POP  BC     ; C = lines
    PUSH BC
    PUSH HL
    LD   L,#192 ; L = line
    LD   A,L
    SUB  C
    LD   C,A    ; C = line - lines
    LD   B,A
LMOVE_DOWN$:
    DEC  L      ; line--
    DEC  C      ; (line + lines)--
    PUSH BC
    PUSH HL
    CALL _MoveLineCtoL
    POP  HL
    POP  BC
    DJNZ LMOVE_DOWN$
    LD   B,L
LCLR_DOWN$:
    DEC  L      ; line--
    CALL _ClearLineL
    DJNZ LCLR_DOWN$
  __endasm;
} //GrApp_ScrollDown

/*--------------------------------- Cut here ---------------------------------*/
unsigned char GrApp_ink, GrApp_paper;

void GrApp_SetPalette (void) __naked {
  __asm
    LD   HL,(_GrApp_ink)
    LD   A,H ; 0000PPPP
    ADD  A
    ADD  A
    ADD  A   ; 0PPPP000
    ADD  L   ; 00000III
    LD   HL,#0x5800
    LD   DE,#0x5801
    LD   BC,#0x2FF
    LD   (HL),A
    LDIR
    LD   A,(_GrApp_paper)
    JP   0x229B
  __endasm;
} //GrApp_SetPalette

/*--------------------------------- Cut here ---------------------------------*/
void GrApp_Cls (void) {
  __asm
    LD   L,#0
    LD   DE,#31  ; {7, 13, 35, 37, 41, 43, 47}
    LD   C,#0x0F ; Mask
    CALL NextScreen$
    CALL NextScreen$
    JR   _GrApp_SetPalette
NextScreen$:
    LD   H,#0x40
NextPiece$:
    LD   A,C
    CPL
    LD   C,A
    AND  (HL)
    LD   (HL),A
    ADD  HL,DE
    LD   A,H
    CP   #0x58
    JR   NZ,NextPiece$
    LD   A,C
    CPL
    LD   C,A
    HALT
    LD   A,L
    OR   A
    RET  Z
    JR   NextScreen$
  __endasm;
} //GrApp_Cls

/*--------------------------------- Cut here ---------------------------------*/
void GrApp__init (void) {
  __asm
    XOR  A                ; Black
    LD   (_GrApp_paper),A
    CALL 0x229B           ;  => border color
    LD   A,#5             ; Cyan
    LD   (_GrApp_ink),A
    LD   (0x5C48),A       ;  => low lines atr
    LD   (0x5C8D),A       ;  => screen atr
    CALL 0xD6B            ; Clear screen
    DI              ; запрещаем прерывания на время установки IM 2

; ************************************************
; *              Fill screen table               *
; ************************************************
    LD   BC,#0xC000 ; y := 192; x := 0
00000$:
    DEC  B
    LD   D,#Config_ScreenTable
    LD   E,B
    LD   A,B
    CALL #0x22B1 ;   PIXEL_ADD
    LD   A,L
    LD   (DE),A
    INC  D
    LD   A,H
    LD   (DE),A
    INC  B
    DJNZ 00000$

; ************************************************
; * Set IM2 mode (need for correct work with IY) *
; ************************************************
    LD   HL,#IM2PROC$
IMON$:
    LD   A,#24       ; код команды JR
    LD   (#65535),A
    LD   A,#195      ; код команды JP
    LD   (#65524),A
    LD   (#65525),HL ; в HL - адрес обработчика прерываний
    LD   HL,#0xFE00  ; построение таблицы для векторов прерываний
    LD   DE,#0xFE01
    LD   BC,#256     ; размер таблицы минус 1
    LD   (HL),#0xFF  ; адрес перехода #FFFF (65535)
    LD   A,H         ; запоминаем старший байт адреса таблицы
    LDIR             ; заполняем таблицу
    LD   I,A         ; задаем в регистре I старший байт адреса
                     ; таблицы для векторов прерываний
    IM   2           ; назначаем второй режим прерываний
    EI               ; разрешаем прерывания
    RET

IM2PROC$:
    PUSH AF
    PUSH HL
    LD   HL,(_Timer_ticks)
    DEC  HL
    LD   (_Timer_ticks),HL
/*  PUSH BC
    PUSH DE
    PUSH IY
    LD   IY,#0x5C3A
    CALL 0x2BF        ; Keyboard
    POP  IY
    POP  DE
    POP  BC */
    POP  HL
    POP  AF
    EI
  __endasm;
} //GrApp__init

/*--------------------------------- Cut here ---------------------------------*/
void GrApp_Close (void) __naked {
  __asm
; ************************************************
; *              Set IM1 mode back               *
; ************************************************
IMOFF$:
    DI
    LD   HL,#0x2758
    EXX
    LD   IY,#0x5C3A
    LD   A,#0x3F
    LD   I,A
    IM   1
    EI
    JP   0xD6B
  __endasm;
} //GrApp__Close
