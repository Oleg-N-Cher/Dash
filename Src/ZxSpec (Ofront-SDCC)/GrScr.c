/* Ver 1.0.0 for ZX Spectrum 48/128 */
/* Use SDCC to compile this module. */

#include "SYSTEM.h"
#include "GrCfg.h"

export void GrScr_Close (void);
export void GrScr_ClrScr (void);
export void GrScr__init (void);


export void GrScr_ClrScr (void)
{{
}}/*GrScr_ClrScr*/

export void GrScr__init (void)
{{
  __asm
    //XOR  A
    //CALL 0x229B
    //CALL 0xD6B      ; Clear screen
    DI

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
    LD   A,#24       ; ��� ������� JR
    LD   (#65535),A
    LD   A,#195      ; ��� ������� JP
    LD   (#65524),A
    LD   (#65525),HL ; � HL - ����� ����������� ����������
    LD   HL,#0xFE00  ; ���������� ������� ��� �������� ����������
    LD   DE,#0xFE01
    LD   BC,#256     ; ������ ������� ����� 1
    LD   (HL),#0xFF  ; ����� �������� #FFFF (65535)
    LD   A,H         ; ���������� ������� ���� ������ �������
    LDIR             ; ��������� �������
;   DI               ; ��������� ���������� �� �����
                     ; ��������� ������� ������
    LD   I,A         ; ������ � �������� I ������� ���� ������
                     ; ������� ��� �������� ����������
    IM   2           ; ��������� ������ ����� ����������
    EI               ; ��������� ����������
    RET

IM2PROC$:
    PUSH AF
    PUSH HL
    LD   HL,(#0x5C78) ;
    INC  HL           ; INC(FRAMES_CNTR)
    LD   (#0x5C78),HL ;
    POP  HL
    POP  AF
    EI
  __endasm;
}}/*GrScr__init*/

export void GrScr_Close (void)
{{
  //REPEAT UNTIL (Keyboard_PressedAnyKey());
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
}}/*GrScr__Close*/
