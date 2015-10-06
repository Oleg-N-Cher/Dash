#ifndef Timer__h
#define Timer__h


/* Max. delay time = 32767/50/60 ~= 10.922 min */
#define Timer_Start(ticks) (*(unsigned int*) (0x5C78) = (ticks-1))
extern void Timer_Until (void);

#define Timer__init()


#endif
