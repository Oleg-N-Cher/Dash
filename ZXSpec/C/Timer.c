void Timer_Start (void);
void Timer_Until (unsigned char ticks);
/*================================== Header ==================================*/

void Timer_Start (void) {
  __asm
  LD   HL,(#0x5C78)  ; WARNING: 65535/50/60 ~= 21.845 minutes (max delay time)
  LD   (time$+1),HL
  __endasm;
}

/*--------------------------------- Cut here ---------------------------------*/
void Timer_Until (unsigned char ticks) __naked {
  __asm
  POP  HL
  POP  DE
  PUSH DE
  PUSH HL
  LD   D,#0
  LD   HL,(#0x5C78)  ; WARNING: 65535/50/60 ~= 21.845 minutes (max delay time)
time$:
  LD   BC,#0
  XOR  A
  SBC  HL,BC         ; Elapsed ticks
  SBC  HL,DE         ; Remained ticks
  RET  NC
loop$:
  RET  Z
  HALT
  INC  L
  JR   loop$
  __endasm;
} //Timer_Until

