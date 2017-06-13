#ifndef Strings__h
#define Strings__h


extern unsigned int Strings_LengthEx (unsigned char *str) __z88dk_fastcall __preserves_regs(d,e,iyl,iyh);
#define Strings_Length(str,len) Strings_LengthEx(str)

#define Strings__init()


#endif
