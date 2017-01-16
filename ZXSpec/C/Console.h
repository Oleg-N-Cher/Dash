#ifndef Console__h
#define Console__h


extern unsigned char Console_x, Console_y, Console_atr;

#define Console_At(x,y) Console_x = x; Console_y = ((y)*12)
extern void Console_Ink (unsigned char color) __z88dk_fastcall __preserves_regs(b,c,d,e,h,iyl,iyh);
extern void Console_WriteCh (unsigned char ch) __z88dk_fastcall __preserves_regs(iyl,iyh);
extern void Console_WriteLn (void);
extern void Console_WriteStrEx (unsigned char *str) __z88dk_fastcall __preserves_regs(iyl,iyh);
#define Console_WriteStr(str,len) Console_WriteStrEx(str)

#define Console__init()


#endif
