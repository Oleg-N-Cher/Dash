unsigned int Strings_LengthEx (unsigned char *str);
/*============================================================================*/

unsigned int Strings_LengthEx (unsigned char *str) {
  __asm
    POP  BC
    POP  HL
    PUSH HL
    PUSH BC
    XOR  A
    LD   B,A
    LD   C,A
    CPIR
    LD   HL,#0xFFFF
    SBC  HL,BC
__endasm;
} //Strings_LengthEx
