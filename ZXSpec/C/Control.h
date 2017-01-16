#ifndef Control__h
#define Control__h


extern unsigned char Control_Get (void) __preserves_regs(b,d,e,iyl,iyh);
extern unsigned char Control_GetCustom (void) __preserves_regs(b,d,e,iyl,iyh);
extern unsigned char Control_ReadKey (void) __preserves_regs(a,b,c,d,e,h); // With auto-repeat
extern unsigned char Control_PressedAnyKey (void) __preserves_regs(iyl,iyh);
extern void Control_Select (unsigned char n) __z88dk_fastcall __preserves_regs(b,c,d,e,h,iyl,iyh);
#define Control_Close()
#ifdef __detect_kempston__
  void Control__init (void) __preserves_regs(c,d,e,iyl,iyh);
#else
# define Control__init()
#endif //__detect_kempston__


#endif
