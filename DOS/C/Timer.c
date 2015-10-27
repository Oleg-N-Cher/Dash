#include <dos.h>

void Timer_Start (unsigned ticks);
void Timer_Until (void);
void Timer_Close (void);
/*================================== Header ==================================*/

static volatile unsigned elapsed;
void interrupt (*std_timer)();

/*Перевести в Ассемблер и оптимизировать прерывания вручную*/
void interrupt timer_handler () {
  /*Ставим на обслуживание вектора 0x1C*/
  if (elapsed) elapsed --;	/*Время истекает*/
}

/*--------------------------------- Cut here ---------------------------------*/
void Timer_Start (unsigned ticks) {
  elapsed = ticks;
} /*Timer_Start*/

/*--------------------------------- Cut here ---------------------------------*/
void Timer_Until (void) {
  while (elapsed) ;
} /*Timer_Until*/

/*--------------------------------- Cut here ---------------------------------*/
static int timer_installed = 0;

void Timer__init (void) {
  if (!timer_installed) {
    /*     std_timer = (void interrupt(*)())getvect (0x1C); */
    std_timer = getvect (0x1C);
    setvect (0x1C, timer_handler);
    elapsed = 0;
    timer_installed = 1;
    atexit (Timer_Close);
  }
} /*Timer__init*/

void cdecl Timer_Close (void) {
  if (timer_installed) {
    setvect (0x1C, std_timer);
    timer_installed = 0;
  }
} /*Timer_Close*/
