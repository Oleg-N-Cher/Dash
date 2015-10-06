void Timer_Until (void);
/*================================== Header ==================================*/

void Timer_Until (void) __naked {
  __asm
loop$:
  LD   A,(#0x5C78+1) ; High byte
  AND  #0x80
	RET  NZ
  HALT
  JR   loop$
  __endasm;
} //Timer_Until

