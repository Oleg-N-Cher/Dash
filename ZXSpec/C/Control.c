#include "GrApp.h"

void Control_ChangePalette (void);
unsigned char Control_ReadKey (void);
unsigned char Control_Pressed (unsigned char key);
unsigned char Control_PressedAnyKey (void);
/*================================== Header ==================================*/

void Control_ChangePalette (void) {
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
#define KEY_UP    0x39
#define KEY_DOWN  0x38
#define KEY_LEFT  0x36
#define KEY_RIGHT 0x37

unsigned char Control_Pressed (unsigned char key) {
  //ChangePalette();
  __asm
    POP  HL
    POP  DE
    PUSH DE
    PUSH HL
    LD   A,E      ; key
    LD   L,#0     ; FALSE
    CP   #KEY_UP
    JR   NZ,KEY1$
    LD   A,#0xFB
    IN   A,(#254)
    RRCA          ; "Q"
    RET  C
    INC  L
    RET
KEY1$:
    CP    #KEY_DOWN
    JR    NZ,KEY2$
    LD    A,#253
    IN    A,(#254)
    RRCA              ; "A"
    RET   C
    INC   L
    RET
KEY2$:
    CP    #KEY_RIGHT
    JR    NZ,KEY3$
    LD    A,#223
    IN    A,(#254)
    RRCA              ; "P"
    RET   C
    INC   L
    RET
KEY3$:
    CP    #KEY_LEFT
    RET   NZ
    LD    A,#223
    IN    A,(#254)
    RRCA
    RRCA              ; "O"
    RET   C
    INC   L
  __endasm;
} //Control_Pressed

/*--------------------------------- Cut here ---------------------------------*/
unsigned char Control_PressedAnyKey (void) {
  Control_ChangePalette();
  __asm
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
