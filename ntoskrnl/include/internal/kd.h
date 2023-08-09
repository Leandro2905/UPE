#pragma once

#include <cportlib/cportlib.h>

//
// Kernel Debugger Port Definition
//
struct _KD_DISPATCH_TABLE;
extern CPPORT GdbPortInfo;

BOOLEAN
NTAPI
KdPortInitializeEx(
    PCPPORT PortInformation,
    ULONG ComPortNumber
);

BOOLEAN
NTAPI
KdPortGetByteEx(
    PCPPORT PortInformation,
    PUCHAR ByteReceived);

VOID
NTAPI
KdPortPutByteEx(
    PCPPORT PortInformation,
    UCHAR ByteToSend
);

/* SYMBOL ROUTINES **********************************************************/
#ifdef __NTOSKRNL__

#if defined(KDBG) || DBG

#if defined(KDBG)
typedef
BOOLEAN
(NTAPI *PKDBG_CLI_ROUTINE)(
    IN PCHAR Command,
    IN ULONG Argc,
    IN PCH Argv[]);

BOOLEAN
NTAPI
KdbRegisterCliCallback(
    PVOID Callback,
    BOOLEAN Deregister);
#endif

VOID
KdbSymProcessSymbols(
    IN PLDR_DATA_TABLE_ENTRY LdrEntry);


BOOLEAN
KdbSymPrintAddress(
    IN PVOID Address,
    IN PKTRAP_FRAME Context
);

NTSTATUS
KdbSymGetAddressInformation(
    IN PROSSYM_INFO  RosSymInfo,
    IN ULONG_PTR  RelativeAddress,
#ifdef __ROS_DWARF__
    IN PROSSYM_LINEINFO RosSymLineInfo
#else
    OUT PULONG LineNumber  OPTIONAL,
    OUT PCH FileName  OPTIONAL,
    OUT PCH FunctionName  OPTIONAL
#endif
);
#endif

#ifdef KDBG
# define KdbInit()                                  KdbpCliInit()
# define KdbModuleLoaded(FILENAME)                  KdbpCliModuleLoaded(FILENAME)
#else
# define KdbEnterDebuggerException(ER, PM, C, TF, F)    kdHandleException
# define KdbInit()                                      do { } while (0)
# define KdbEnter()                                     do { } while (0)
# define KdbModuleLoaded(X)                             do { } while (0)
#endif

/* KD ROUTINES ***************************************************************/

typedef enum _KD_CONTINUE_TYPE
{
    kdContinue = 0,
    kdDoNotHandleException,
    kdHandleException
} KD_CONTINUE_TYPE;

typedef
VOID
(NTAPI*PKDP_INIT_ROUTINE)(
    struct _KD_DISPATCH_TABLE *DispatchTable,
    ULONG BootPhase
);

typedef
VOID
(NTAPI*PKDP_PRINT_ROUTINE)(
    PCHAR String,
    ULONG Length
);

typedef
VOID
(NTAPI*PKDP_PROMPT_ROUTINE)(PCH String);

typedef
KD_CONTINUE_TYPE
(NTAPI*PKDP_EXCEPTION_ROUTINE)(
    PEXCEPTION_RECORD ExceptionRecord,
    PCONTEXT Context,
    PKTRAP_FRAME TrapFrame
);

/* INIT ROUTINES *************************************************************/

BOOLEAN
NTAPI
KdInitSystem(
    ULONG Reserved,
    PLOADER_PARAMETER_BLOCK LoaderBlock
);

VOID
KdpScreenAcquire(VOID);

VOID
KdpScreenRelease(VOID);

VOID
NTAPI
KdpScreenInit(
    struct _KD_DISPATCH_TABLE *DispatchTable,
    ULONG BootPhase
);

VOID
NTAPI
KdpSerialInit(
    struct _KD_DISPATCH_TABLE *DispatchTable,
    ULONG BootPhase
);

VOID
NTAPI
KdpDebugLogInit(
    struct _KD_DISPATCH_TABLE *DispatchTable,
    ULONG BootPhase
);

VOID
NTAPI
KdpBochsInit(
    struct _KD_DISPATCH_TABLE *DispatchTable,
    ULONG BootPhase
);

VOID
NTAPI
KdpGdbStubInit(
    struct _KD_DISPATCH_TABLE *DispatchTable,
    ULONG BootPhase);

VOID
NTAPI
KdpKdbgInit(
    struct _KD_DISPATCH_TABLE *DispatchTable,
    ULONG BootPhase);


/* KD ROUTINES ***************************************************************/

BOOLEAN
NTAPI
KdpDetectConflicts(PCM_RESOURCE_LIST DriverList);

BOOLEAN
NTAPI
KdpSafeReadMemory(
    IN ULONG_PTR Addr,
    IN LONG Len,
    OUT PVOID Value
);

BOOLEAN
NTAPI
KdpSafeWriteMemory(
    IN ULONG_PTR Addr,
    IN LONG Len,
    IN ULONGLONG Value
);


/* KD GLOBALS  ***************************************************************/

/* Serial debug connection */
#define DEFAULT_DEBUG_PORT      2 /* COM2 */
#define DEFAULT_DEBUG_COM1_IRQ  4 /* COM1 IRQ */
#define DEFAULT_DEBUG_COM2_IRQ  3 /* COM2 IRQ */
#define DEFAULT_DEBUG_BAUD_RATE 115200 /* 115200 Baud */

/* KD Native Modes */
#define KdScreen    0
#define KdSerial    1
#define KdFile      2
#define KdBochs     3
#define KdKdbg      4
#define KdMax       5

/* KD Private Debug Modes */
typedef struct _KDP_DEBUG_MODE
{
    union
    {
        struct
        {
            /* Native Modes */
            UCHAR Screen :1;
            UCHAR Serial :1;
            UCHAR File   :1;
            UCHAR Bochs  :1;

            /* Currently Supported Wrappers */
            UCHAR Pice   :1;
            UCHAR Gdb    :1;
        };

        /* Generic Value */
        ULONG Value;
    };
} KDP_DEBUG_MODE;

/* KD Internal Debug Services */
typedef enum _KDP_DEBUG_SERVICE
{
    DumpNonPagedPool = 0x1e, /* a */
    ManualBugCheck = 0x30, /* b */
    DumpNonPagedPoolStats = 0x2e, /* c */
    DumpNewNonPagedPool = 0x20, /* d */
    DumpNewNonPagedPoolStats = 0x12, /* e */
    DumpAllThreads = 0x21, /* f */
    DumpUserThreads = 0x22, /* g */
    KdSpare1 = 0x23, /* h */
    KdSpare2 = 0x17, /* i */
    KdSpare3 = 0x24, /* j */
    EnterDebugger = 0x25,  /* k */
    ThatsWhatSheSaid = 69 /* FIGURE IT OUT */
} KDP_DEBUG_SERVICE;

/* Dispatch Table for Wrapper Functions */
typedef struct _KD_DISPATCH_TABLE
{
    LIST_ENTRY KdProvidersList;
    PKDP_INIT_ROUTINE KdpInitRoutine;
    PKDP_PRINT_ROUTINE KdpPrintRoutine;
    PKDP_PROMPT_ROUTINE KdpPromptRoutine;
    PKDP_EXCEPTION_ROUTINE KdpExceptionRoutine;
} KD_DISPATCH_TABLE, *PKD_DISPATCH_TABLE;

/* The current Debugging Mode */
extern KDP_DEBUG_MODE KdpDebugMode;

/* The current Port IRQ */
extern ULONG KdpPortIrq;

/* The current Port */
extern ULONG KdpPort;

/* Port Information for the Serial Native Mode */
extern ULONG  SerialPortNumber;
extern CPPORT SerialPortInfo;

/* Init Functions for Native Providers */
extern PKDP_INIT_ROUTINE InitRoutines[KdMax];

/* Wrapper Init Function */
extern PKDP_INIT_ROUTINE WrapperInitRoutine;

/* Dispatch Tables for Native Providers */
extern KD_DISPATCH_TABLE DispatchTable[KdMax];

/* Dispatch Table for the Wrapper */
extern KD_DISPATCH_TABLE WrapperTable;

/* The KD Native Provider List */
extern LIST_ENTRY KdProviders;

#endif

#if DBG && defined(_M_IX86) && !defined(_WINKD_) // See ke/i386/traphdlr.c
#define ID_Win32PreServiceHook 'WSH0'
#define ID_Win32PostServiceHook 'WSH1'
typedef void (NTAPI *PKDBG_PRESERVICEHOOK)(ULONG, PULONG_PTR);
typedef ULONG_PTR (NTAPI *PKDBG_POSTSERVICEHOOK)(ULONG, ULONG_PTR);
extern PKDBG_PRESERVICEHOOK KeWin32PreServiceHook;
extern PKDBG_POSTSERVICEHOOK KeWin32PostServiceHook;
#endif
