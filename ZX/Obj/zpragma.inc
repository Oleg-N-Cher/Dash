#pragma output CRT_ORG_CODE = 32832
#pragma output CRT_ORG_DATA = 56488
#pragma output CLIB_MALLOC_HEAP_SIZE = 0 // do not create a heap so that malloc() will not work
#pragma output CLIB_STDIO_HEAP_SIZE = 0 // cannot open files
#pragma output REGISTER_SP = -1
#pragma output CRT_ON_EXIT = 0x10001
#pragma output CRT_ENABLE_EIDI = 0 // Bit flags 0x01 = insert di on start, 0x02 = insert ei on start, 0x10 = insert di on exit, 0x20 = insert ei on exit