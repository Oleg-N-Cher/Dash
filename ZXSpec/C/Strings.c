unsigned int Strings_LengthEx (unsigned char *str) __z88dk_fastcall;
/*============================================================================*/

unsigned int Strings_LengthEx (unsigned char *str) __z88dk_fastcall {
  __asm
    XOR  A
    LD   B,A
    LD   C,A
    CPIR
    LD   HL,#0xFFFF
    SBC  HL,BC
__endasm;
} //Strings_LengthEx
