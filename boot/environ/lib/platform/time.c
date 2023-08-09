/*
 * COPYRIGHT:       See COPYING.ARM in the top level directory
 * PROJECT:         ReactOS UEFI Boot Library
 * FILE:            boot/environ/lib/platform/time.c
 * PURPOSE:         Boot Library Time Management Routines
 * PROGRAMMER:      Alex Ionescu (alex.ionescu@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include "bl.h"

/* DATA VARIABLES ************************************************************/

ULONGLONG BlpTimePerformanceFrequency;

/* FUNCTIONS *****************************************************************/

NTSTATUS
BlpTimeMeasureTscFrequency (
    VOID
    )
{
#if defined(_M_IX86) || defined(_M_X64)
    ULONG Count;
    INT CpuInfo[4];
    ULONGLONG TimeStamp1, TimeStamp2, Delta;

    /* Check if the ISVM bit it set, meaning we're in a hypervisor */
    __cpuid(CpuInfo, 1);
    Count = CpuInfo[2] & 0x80000000 ? 10 : 1;

    /* Loop trying to get an accurate TSC */
    do
    {
        /* Stall for 1us and get count 1 */
        EfiStall(1);
        TimeStamp1 = __rdtsc();

        /* Stall for 1000us and get count 2*/
        EfiStall(1000);
        TimeStamp2 = __rdtsc();

        /* Stall for 9000us and get the difference */
        EfiStall(9000);
        Delta = __rdtsc() - TimeStamp2;

        /* Keep going as long as the TSC is fluctuating */
        --Count;
    } while (((TimeStamp2 - TimeStamp1) > Delta) && (Count));

    /* Set the frequency based on the two measurements we took */
    BlpTimePerformanceFrequency = 125 * (Delta - (TimeStamp2 - TimeStamp1)) & 0x1FFFFFFFFFFFFFF;
    return STATUS_SUCCESS;
#else
    EfiPrintf(L"BlpTimeMeasureTscFrequency not implemented for this platform.\r\n");
    return STATUS_NOT_IMPLEMENTED;
#endif
}

NTSTATUS
BlpTimeCalibratePerformanceCounter (
    VOID
    )
{
#if defined(_M_IX86) || defined(_M_X64)
    INT CpuInfo[4];

    /* Check if the ISVM bit it set, meaning we're in a hypervisor */
    __cpuid(CpuInfo, 1);
    if (CpuInfo[2] & 0x80000000)
    {
        /* Get the Hypervisor Identification Leaf */
        __cpuid(CpuInfo, 0x40000001);

        /* Is this Hyper-V? */
        if (CpuInfo[0] == '1#vH')
        {
            /* Get the Hypervisor Feature Identification Leaf */
            __cpuid(CpuInfo, 0x40000003);

            /* Check if HV_X64_MSR_REFERENCE_TSC is present */
            if (CpuInfo[3] & 0x100)
            {
                /* Read the TSC frequency from the MSR */
                BlpTimePerformanceFrequency = __readmsr(0x40000022);
                return STATUS_SUCCESS;
            }
        }
    }

    /* On other systems, compute it */
    return BlpTimeMeasureTscFrequency();
#else
    EfiPrintf(L"BlpTimeCalibratePerformanceCounter not implemented for this platform.\r\n");
    return STATUS_NOT_IMPLEMENTED;
#endif
}

ULONGLONG
BlTimeQueryPerformanceCounter (
    _Out_opt_ PLARGE_INTEGER Frequency
    )
{
#if defined(_M_IX86) || defined(_M_X64)
    /* Check if caller wants frequency */
    if (Frequency)
    {
        /* Return it */
        Frequency->QuadPart = BlpTimePerformanceFrequency;
    }

    /* Return the TSC value */
    return __rdtsc();
#else
    EfiPrintf(L"BlTimeQueryPerformanceCounter not implemented for this platform.\r\n");
    return 0;
#endif
};
