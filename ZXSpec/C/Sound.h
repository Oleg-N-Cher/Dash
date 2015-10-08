#ifndef Sound__h
#define Sound__h

#include "GrCfg.h"


#ifdef GrCfg_UseSound
  extern void Sound_TitleNext (void);
  extern void Sound_Death (unsigned char y);
  extern void Sound_DropStoneOrAlmas (unsigned char y);
  extern void Sound_GetAlmas (unsigned int xn_plus_yn);
  extern void Sound_KeyPressed (void);
  extern void Sound__init (void);
#else
#  define Sound_TitleNext()
#  define Sound_Death(y)
#  define Sound_DropStoneOrAlmas(y)
#  define Sound_GetAlmas(xn_plus_yn)
#  define Sound_KeyPressed()
#  define Sound__init()
#endif


#endif
