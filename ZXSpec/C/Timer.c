extern int Timer_ticks;
void Timer_Until (void);
/*================================== Header ==================================*/

int Timer_ticks;

void Timer_Until (void) __naked {
  __asm
loop$:
  LD   A,(_Timer_ticks+1) ; High byte
  AND  #0x80
	RET  NZ
  HALT
  JR   loop$
  __endasm;
} //Timer_Until
