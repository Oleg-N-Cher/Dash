#ifndef Console__h
#define Console__h


extern unsigned char Console_x, Console_y, Console_atr;

#define Console_At(x,y) Console_x = x; Console_y = ((y)*12)
#define Console_Ink(color) Console_atr = color
extern void Console_WriteCh (unsigned char ch);
extern void Console_WriteStrEx (unsigned char *str);
#define Console_WriteStr(str,len) Console_WriteStrEx(str)

#define Console__init()


#endif
