#pragma output CLIB_MALLOC_HEAP_SIZE = 0 // do not create a heap so that malloc() will not work
#pragma output CLIB_STDIO_HEAP_SIZE = 0 // cannot open files
#pragma output REGISTER_SP = -1
#pragma output CRT_ON_EXIT = 0x10002
