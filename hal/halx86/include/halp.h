/*
 *
 */

#pragma once

#ifdef CONFIG_SMP
#define HAL_BUILD_TYPE (DBG ? PRCB_BUILD_DEBUG : 0)
#else
#define HAL_BUILD_TYPE ((DBG ? PRCB_BUILD_DEBUG : 0) | PRCB_BUILD_UNIPROCESSOR)
#endif

typedef struct _HAL_BIOS_FRAME
{
    ULONG SegSs;
    ULONG Esp;
    ULONG EFlags;
    ULONG SegCs;
    ULONG Eip;
    PKTRAP_FRAME TrapFrame;
    ULONG CsLimit;
    ULONG CsBase;
    ULONG CsFlags;
    ULONG SsLimit;
    ULONG SsBase;
    ULONG SsFlags;
    ULONG Prefix;
} HAL_BIOS_FRAME, *PHAL_BIOS_FRAME;

typedef
VOID
(__cdecl *PHAL_SW_INTERRUPT_HANDLER)(
    VOID
);

typedef
VOID
(FASTCALL *PHAL_SW_INTERRUPT_HANDLER_2ND_ENTRY)(
    IN PKTRAP_FRAME TrapFrame
);

#define HAL_APC_REQUEST         0
#define HAL_DPC_REQUEST         1

/* HAL profiling offsets in KeGetPcr()->HalReserved[] */
#define HAL_PROFILING_INTERVAL      0
#define HAL_PROFILING_MULTIPLIER    1

/* Usage flags */
#define IDT_REGISTERED          0x01
#define IDT_LATCHED             0x02
#define IDT_READ_ONLY           0x04
#define IDT_INTERNAL            0x11
#define IDT_DEVICE              0x21

/* Conversion functions */
#define BCD_INT(bcd)            \
    (((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F))
#define INT_BCD(int)            \
    (UCHAR)(((int / 10) << 4) + (int % 10))

typedef
BOOLEAN
(NTAPI *PHAL_DISMISS_INTERRUPT)(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

BOOLEAN
NTAPI
HalpDismissIrqGeneric(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

BOOLEAN
NTAPI
HalpDismissIrq15(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

BOOLEAN
NTAPI
HalpDismissIrq13(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

BOOLEAN
NTAPI
HalpDismissIrq07(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

BOOLEAN
NTAPI
HalpDismissIrqLevel(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

BOOLEAN
NTAPI
HalpDismissIrq15Level(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

BOOLEAN
NTAPI
HalpDismissIrq13Level(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

BOOLEAN
NTAPI
HalpDismissIrq07Level(
    IN KIRQL Irql,
    IN ULONG Irq,
    OUT PKIRQL OldIrql
);

VOID
__cdecl
HalpHardwareInterruptLevel(
    VOID
);

//
// Hack Flags
//
#define HALP_REVISION_FROM_HACK_FLAGS(x)    ((x) >> 24)
#define HALP_REVISION_HACK_FLAGS(x)         ((x) >> 12)
#define HALP_HACK_FLAGS(x)                  ((x) & 0xFFF)

//
// Feature flags
//
#define HALP_CARD_FEATURE_FULL_DECODE   0x0001

//
// Match Flags
//
#define HALP_CHECK_CARD_REVISION_ID     0x10000
#define HALP_CHECK_CARD_SUBVENDOR_ID    0x20000
#define HALP_CHECK_CARD_SUBSYSTEM_ID    0x40000

//
// Mm PTE/PDE to Hal PTE/PDE
//
#define HalAddressToPde(x) (PHARDWARE_PTE)MiAddressToPde(x)
#define HalAddressToPte(x) (PHARDWARE_PTE)MiAddressToPte(x)

typedef struct _IDTUsageFlags
{
    UCHAR Flags;
} IDTUsageFlags;

typedef struct
{
    KIRQL Irql;
    UCHAR BusReleativeVector;
} IDTUsage;

typedef struct _HalAddressUsage
{
    struct _HalAddressUsage *Next;
    CM_RESOURCE_TYPE Type;
    UCHAR Flags;
    struct
    {
        ULONG Start;
        ULONG Length;
    } Element[];
} ADDRESS_USAGE, *PADDRESS_USAGE;

/* adapter.c */
PADAPTER_OBJECT NTAPI HalpAllocateAdapterEx(ULONG NumberOfMapRegisters,BOOLEAN IsMaster, BOOLEAN Dma32BitAddresses);

/* sysinfo.c */
INIT_FUNCTION
VOID
NTAPI
HalpRegisterVector(IN UCHAR Flags,
                   IN ULONG BusVector,
                   IN ULONG SystemVector,
                   IN KIRQL Irql);

INIT_FUNCTION
VOID
NTAPI
HalpEnableInterruptHandler(IN UCHAR Flags,
                           IN ULONG BusVector,
                           IN ULONG SystemVector,
                           IN KIRQL Irql,
                           IN PVOID Handler,
                           IN KINTERRUPT_MODE Mode);

/* pic.c */
VOID NTAPI HalpInitializePICs(IN BOOLEAN EnableInterrupts);
VOID __cdecl HalpApcInterrupt(VOID);
VOID __cdecl HalpDispatchInterrupt(VOID);
PHAL_SW_INTERRUPT_HANDLER __cdecl HalpDispatchInterrupt2(VOID);
DECLSPEC_NORETURN VOID FASTCALL HalpApcInterrupt2ndEntry(IN PKTRAP_FRAME TrapFrame);
DECLSPEC_NORETURN VOID FASTCALL HalpDispatchInterrupt2ndEntry(IN PKTRAP_FRAME TrapFrame);

/* profil.c */
extern BOOLEAN HalpProfilingStopped;

/* timer.c */
INIT_FUNCTION VOID NTAPI HalpInitializeClock(VOID);
VOID __cdecl HalpClockInterrupt(VOID);
VOID __cdecl HalpProfileInterrupt(VOID);

VOID
NTAPI
HalpCalibrateStallExecution(VOID);

/* pci.c */
VOID HalpInitPciBus (VOID);

/* dma.c */
INIT_FUNCTION VOID HalpInitDma (VOID);

/* Non-generic initialization */
VOID HalpInitPhase0 (PLOADER_PARAMETER_BLOCK LoaderBlock);
VOID HalpInitPhase1(VOID);

VOID
NTAPI
HalpFlushTLB(VOID);

//
// KD Support
//
VOID
NTAPI
HalpCheckPowerButton(
    VOID
);

INIT_FUNCTION
VOID
NTAPI
HalpRegisterKdSupportFunctions(
    VOID
);

INIT_FUNCTION
NTSTATUS
NTAPI
HalpSetupPciDeviceForDebugging(
    IN PVOID LoaderBlock,
    IN OUT PDEBUG_DEVICE_DESCRIPTOR PciDevice
);

INIT_FUNCTION
NTSTATUS
NTAPI
HalpReleasePciDeviceForDebugging(
    IN OUT PDEBUG_DEVICE_DESCRIPTOR PciDevice
);

//
// Memory routines
//
ULONG64
NTAPI
HalpAllocPhysicalMemory(
    IN PLOADER_PARAMETER_BLOCK LoaderBlock,
    IN ULONG64 MaxAddress,
    IN PFN_NUMBER PageCount,
    IN BOOLEAN Aligned
);

PVOID
NTAPI
HalpMapPhysicalMemory64Vista(
    IN PHYSICAL_ADDRESS PhysicalAddress,
    IN PFN_COUNT PageCount,
    IN BOOLEAN FlushCurrentTLB
);

VOID
NTAPI
HalpUnmapVirtualAddressVista(
    IN PVOID VirtualAddress,
    IN PFN_COUNT NumberPages,
    IN BOOLEAN FlushCurrentTLB
);

PVOID
NTAPI
HalpMapPhysicalMemory64(
    IN PHYSICAL_ADDRESS PhysicalAddress,
    IN PFN_COUNT PageCount
);

VOID
NTAPI
HalpUnmapVirtualAddress(
    IN PVOID VirtualAddress,
    IN PFN_COUNT NumberPages
);

/* sysinfo.c */
NTSTATUS
NTAPI
HaliQuerySystemInformation(
    IN HAL_QUERY_INFORMATION_CLASS InformationClass,
    IN ULONG BufferSize,
    IN OUT PVOID Buffer,
    OUT PULONG ReturnedLength
);

NTSTATUS
NTAPI
HaliSetSystemInformation(
    IN HAL_SET_INFORMATION_CLASS InformationClass,
    IN ULONG BufferSize,
    IN OUT PVOID Buffer
);

//
// BIOS Routines
//
BOOLEAN
NTAPI
HalpBiosDisplayReset(
    VOID
);

VOID
FASTCALL
HalpExitToV86(
    PKTRAP_FRAME TrapFrame
);

VOID
__cdecl
HalpRealModeStart(
    VOID
);

//
// Processor Halt Routine
//
VOID
NTAPI
HaliHaltSystem(
    VOID
);

//
// CMOS Routines
//
INIT_FUNCTION
VOID
NTAPI
HalpInitializeCmos(
    VOID
);

UCHAR
NTAPI
HalpReadCmos(
    IN UCHAR Reg
);

VOID
NTAPI
HalpWriteCmos(
    IN UCHAR Reg,
    IN UCHAR Value
);

//
// Spinlock for protecting CMOS access
//
VOID
NTAPI
HalpAcquireCmosSpinLock(
    VOID
);

VOID
NTAPI
HalpReleaseCmosSpinLock(
    VOID
);

VOID
NTAPI
HalpInitializeLegacyPICs(
    VOID
);

NTSTATUS
NTAPI
HalpOpenRegistryKey(
    IN PHANDLE KeyHandle,
    IN HANDLE RootKey,
    IN PUNICODE_STRING KeyName,
    IN ACCESS_MASK DesiredAccess,
    IN BOOLEAN Create
);

INIT_FUNCTION
VOID
NTAPI
HalpGetNMICrashFlag(
    VOID
);

INIT_FUNCTION
BOOLEAN
NTAPI
HalpGetDebugPortTable(
    VOID
);

INIT_FUNCTION
VOID
NTAPI
HalpReportSerialNumber(
    VOID
);

INIT_FUNCTION
NTSTATUS
NTAPI
HalpMarkAcpiHal(
    VOID
);

INIT_FUNCTION
VOID
NTAPI
HalpBuildAddressMap(
    VOID
);

INIT_FUNCTION
VOID
NTAPI
HalpReportResourceUsage(
    IN PUNICODE_STRING HalName,
    IN INTERFACE_TYPE InterfaceType
);

INIT_FUNCTION
ULONG
NTAPI
HalpIs16BitPortDecodeSupported(
    VOID
);

NTSTATUS
NTAPI
HalpQueryAcpiResourceRequirements(
    OUT PIO_RESOURCE_REQUIREMENTS_LIST *Requirements
);

VOID
FASTCALL
KeUpdateSystemTime(
    IN PKTRAP_FRAME TrapFrame,
    IN ULONG Increment,
    IN KIRQL OldIrql
);

INIT_FUNCTION
VOID
NTAPI
HalpInitBusHandlers(
    VOID
);

NTSTATUS
NTAPI
HaliInitPnpDriver(
    VOID
);

INIT_FUNCTION
VOID
NTAPI
HalpDebugPciDumpBus(
    IN ULONG i,
    IN ULONG j,
    IN ULONG k,
    IN PPCI_COMMON_CONFIG PciData
);

VOID
NTAPI
HalpInitProcessor(
    IN ULONG ProcessorNumber,
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
);

#ifdef _M_AMD64

VOID
NTAPI
HalInitializeBios(
    _In_ ULONG Unknown,
    _In_ PLOADER_PARAMETER_BLOCK LoaderBlock
);

#define KfLowerIrql KeLowerIrql
#define KiEnterInterruptTrap(TrapFrame) /* We do all neccessary in asm code */
#define KiEoiHelper(TrapFrame) return /* Just return to the caller */
#define HalBeginSystemInterrupt(Irql, Vector, OldIrql) ((*(OldIrql) = PASSIVE_LEVEL), TRUE)
#ifndef CONFIG_SMP
/* On UP builds, spinlocks don't exist at IRQL >= DISPATCH */
#define KiAcquireSpinLock(SpinLock)
#define KiReleaseSpinLock(SpinLock)
#define KfAcquireSpinLock(SpinLock) KfRaiseIrql(DISPATCH_LEVEL);
#define KfReleaseSpinLock(SpinLock, OldIrql) KeLowerIrql(OldIrql);
#endif // !CONFIG_SMP
#endif // _M_AMD64

extern BOOLEAN HalpNMIInProgress;

extern ADDRESS_USAGE HalpDefaultIoSpace;

extern KSPIN_LOCK HalpSystemHardwareLock;

extern PADDRESS_USAGE HalpAddressUsageList;

extern LARGE_INTEGER HalpPerfCounter;

extern KAFFINITY HalpActiveProcessors;

extern BOOLEAN HalDisableFirmwareMapper;
extern PWCHAR HalHardwareIdString;
extern PWCHAR HalName;

extern KAFFINITY HalpDefaultInterruptAffinity;

extern IDTUsageFlags HalpIDTUsageFlags[MAXIMUM_IDTVECTOR+1];

extern const USHORT HalpBuildType;
