/* Ver 1.0.0 for ZX Spectrum 48/128 */
/* Use ZXDev to compile this module */

#include "Timer.h"
#include "GrCfg.h"

void GrApp_Close (void);
void GrApp_Cls (void);
void GrApp_SetPalette (void);
void GrApp__init (void);

extern unsigned char GrApp_ink, GrApp_paper;
/*================================== Header ==================================*/

void GrApp_Cls (void) {
} //GrApp_Cls

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
    LD   D,#GrCfg_ScreenTable
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
void GrApp_Close (void) {
  __asm
; ************************************************
; *              Set IM1 mode back               *
; ************************************************
IMOFF$:
    DI
    LD   A,#63
    LD   I,A
    IM   1
    EI
  __endasm;
} //GrApp__Close
