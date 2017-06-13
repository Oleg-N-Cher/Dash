/* Runtime system routines */

//#define SYSTEM_IncRef

/* Module registry */
//#define SYSTEM_RegisterModules
//#define SYSTEM_RegisterMain
//#define SYSTEM_RegisterCommands

/* Record type descriptors */
//#define SYSTEM_RecTypeDesc
//#define SYSTEM_InitTypes

/* Oberon-2 type bound procedures support */
//#define SYSTEM_TypeBoundProcDynCalls

/* Std procs and operator mappings */
#define SYSTEM_DIV_as_in_C
#define SYSTEM_MOD_as_in_C
#define SYSTEM_NoASSERT

/* Runtime checks */
#define SYSTEM_NoCheck_X
#define SYSTEM_NoCheck_CASE

/* Garbage collector */
#define SYSTEM_NoGC
//#define SYSTEM_Use_HEAP
