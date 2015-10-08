#ifndef Timer__h
#define Timer__h


extern int Timer_ticks;

/* Max. delay time = 32767/50/60 ~= 10.922 min */
#define Timer_Start(ticks) Timer_ticks = ticks-1
extern void Timer_Until (void);

#define Timer__init()


#endif
