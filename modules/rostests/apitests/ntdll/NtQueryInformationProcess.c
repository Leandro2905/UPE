/*
 * PROJECT:         ReactOS API tests
 * LICENSE:         LGPLv2.1+ - See COPYING.LIB in the top level directory
 * PURPOSE:         Tests for the NtQueryInformationProcess API
 * PROGRAMMER:      Thomas Faber <thomas.faber@reactos.org>
 */

#include "precomp.h"

static LARGE_INTEGER TestStartTime;

static
void
Test_ProcessTimes(void)
{
#define SPIN_TIME 1000000
    NTSTATUS Status;
    KERNEL_USER_TIMES Times1;
    KERNEL_USER_TIMES Times2;
    ULONG Length;
    LARGE_INTEGER Time1, Time2;

    /* Everything is NULL */
    Status = NtQueryInformationProcess(NULL,
                                       ProcessTimes,
                                       NULL,
                                       0,
                                       NULL);
    ok_hex(Status, STATUS_INFO_LENGTH_MISMATCH);

    /* Right size, invalid process */
    Status = NtQueryInformationProcess(NULL,
                                       ProcessTimes,
                                       NULL,
                                       sizeof(KERNEL_USER_TIMES),
                                       NULL);
    ok_hex(Status, STATUS_INVALID_HANDLE);

    /* Valid process, no buffer */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       NULL,
                                       0,
                                       NULL);
    ok_hex(Status, STATUS_INFO_LENGTH_MISMATCH);

    /* Unaligned buffer, wrong size */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       (PVOID)2,
                                       0,
                                       NULL);
    ok_hex(Status, STATUS_INFO_LENGTH_MISMATCH);

    /* Unaligned buffer, correct size */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       (PVOID)2,
                                       sizeof(KERNEL_USER_TIMES),
                                       NULL);
    ok_hex(Status, STATUS_DATATYPE_MISALIGNMENT);

    /* Buffer too small */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       NULL,
                                       sizeof(KERNEL_USER_TIMES) - 1,
                                       NULL);
    ok_hex(Status, STATUS_INFO_LENGTH_MISMATCH);

    /* Right buffer size but NULL pointer */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       NULL,
                                       sizeof(KERNEL_USER_TIMES),
                                       NULL);
    ok_hex(Status, STATUS_ACCESS_VIOLATION);

    /* Buffer too large */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       NULL,
                                       sizeof(KERNEL_USER_TIMES) + 1,
                                       NULL);
    ok_hex(Status, STATUS_INFO_LENGTH_MISMATCH);

    /* Buffer too small, ask for length */
    Length = 0x55555555;
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       NULL,
                                       sizeof(KERNEL_USER_TIMES) - 1,
                                       &Length);
    ok_hex(Status, STATUS_INFO_LENGTH_MISMATCH);
    ok_dec(Length, 0x55555555);

    Status = NtQuerySystemTime(&Time1);
    ok_hex(Status, STATUS_SUCCESS);

    /* Do some busy waiting to increase UserTime */
    do
    {
        Status = NtQuerySystemTime(&Time2);
        if (!NT_SUCCESS(Status))
        {
            ok(0, "NtQuerySystemTime failed with %lx\n", Status);
            break;
        }
    } while (Time2.QuadPart - Time1.QuadPart < SPIN_TIME);

    /* Valid parameters, no return length */
    Status = NtQuerySystemTime(&Time1);
    ok_hex(Status, STATUS_SUCCESS);

    RtlFillMemory(&Times1, sizeof(Times1), 0x55);
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       &Times1,
                                       sizeof(KERNEL_USER_TIMES),
                                       NULL);
    ok_hex(Status, STATUS_SUCCESS);
    ros_skip_flaky
    ok(Times1.CreateTime.QuadPart < TestStartTime.QuadPart,
       "CreateTime is %I64u, expected < %I64u\n", Times1.CreateTime.QuadPart, TestStartTime.QuadPart);
    ok(Times1.CreateTime.QuadPart > TestStartTime.QuadPart - 100000000LL,
       "CreateTime is %I64u, expected > %I64u\n", Times1.CreateTime.QuadPart, TestStartTime.QuadPart - 100000000LL);
    ok(Times1.ExitTime.QuadPart == 0,
       "ExitTime is %I64u, expected 0\n", Times1.ExitTime.QuadPart);
    ok(Times1.KernelTime.QuadPart != 0, "KernelTime is 0\n");
    ros_skip_flaky
    ok(Times1.UserTime.QuadPart != 0, "UserTime is 0\n");

    /* Do some busy waiting to increase UserTime */
    do
    {
        Status = NtQuerySystemTime(&Time2);
        if (!NT_SUCCESS(Status))
        {
            ok(0, "NtQuerySystemTime failed with %lx\n", Status);
            break;
        }
    } while (Time2.QuadPart - Time1.QuadPart < SPIN_TIME);

    /* Again, this time with a return length */
    Length = 0x55555555;
    RtlFillMemory(&Times2, sizeof(Times2), 0x55);
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessTimes,
                                       &Times2,
                                       sizeof(KERNEL_USER_TIMES),
                                       &Length);
    ok_hex(Status, STATUS_SUCCESS);
    ok_dec(Length, sizeof(KERNEL_USER_TIMES));
    ok(Times1.CreateTime.QuadPart == Times2.CreateTime.QuadPart,
       "CreateTimes not equal: %I64u != %I64u\n", Times1.CreateTime.QuadPart, Times2.CreateTime.QuadPart);
    ok(Times2.ExitTime.QuadPart == 0,
       "ExitTime is %I64u, expected 0\n", Times2.ExitTime.QuadPart);
    ok(Times2.KernelTime.QuadPart != 0, "KernelTime is 0\n");
    ok(Times2.UserTime.QuadPart != 0, "UserTime is 0\n");

    /* Compare the two sets of KernelTime/UserTime values */
    Status = NtQuerySystemTime(&Time2);
    ok_hex(Status, STATUS_SUCCESS);
    /* Time values must have increased */
    ok(Times2.KernelTime.QuadPart > Times1.KernelTime.QuadPart,
       "KernelTime values inconsistent. Expected %I64u > %I64u\n", Times2.KernelTime.QuadPart, Times1.KernelTime.QuadPart);
    ros_skip_flaky
    ok(Times2.UserTime.QuadPart > Times1.UserTime.QuadPart,
       "UserTime values inconsistent. Expected %I64u > %I64u\n", Times2.UserTime.QuadPart, Times1.UserTime.QuadPart);
    /* They can't have increased by more than wall clock time difference (we only have one thread) */
    ros_skip_flaky
    ok(Times2.KernelTime.QuadPart - Times1.KernelTime.QuadPart < Time2.QuadPart - Time1.QuadPart,
       "KernelTime values inconsistent. Expected %I64u - %I64u < %I64u\n",
       Times2.KernelTime.QuadPart, Times1.KernelTime.QuadPart, Time2.QuadPart - Time1.QuadPart);
    ok(Times2.UserTime.QuadPart - Times1.UserTime.QuadPart < Time2.QuadPart - Time1.QuadPart,
       "UserTime values inconsistent. Expected %I64u - %I64u < %I64u\n",
       Times2.UserTime.QuadPart, Times1.UserTime.QuadPart, Time2.QuadPart - Time1.QuadPart);

    trace("KernelTime1 = %I64u\n", Times1.KernelTime.QuadPart);
    trace("KernelTime2 = %I64u\n", Times2.KernelTime.QuadPart);
    trace("UserTime1 = %I64u\n", Times1.UserTime.QuadPart);
    trace("UserTime2 = %I64u\n", Times2.UserTime.QuadPart);

    /* TODO: Test ExitTime on a terminated process */
#undef SPIN_TIME
}

static
void
Test_ProcessPriorityClassAlignment(void)
{
    NTSTATUS Status;
    PPROCESS_PRIORITY_CLASS ProcPriority;

    /* Allocate some memory for the priority class structure */
    ProcPriority = malloc(sizeof(PROCESS_PRIORITY_CLASS));
    if (ProcPriority == NULL)
    {
        skip("Failed to allocate memory for PROCESS_PRIORITY_CLASS!\n");
        return;
    }

    /*
     * Initialize the PriorityClass member to ensure the test won't randomly succeed (if such data is uninitialized).
     * Filling 85 to the data member makes sure that if the test fails continously then NtQueryInformationProcess()
     * didn't initialize the structure with data.
     */
    RtlFillMemory(&ProcPriority->PriorityClass, sizeof(ProcPriority->PriorityClass), 0x55);

    /* Unaligned buffer -- wrong size */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessPriorityClass,
                                       (PVOID)1,
                                       0,
                                       NULL);
    ok_hex(Status, STATUS_INFO_LENGTH_MISMATCH);

    /* Unaligned buffer -- correct size */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessPriorityClass,
                                       (PVOID)1,
                                       sizeof(PROCESS_PRIORITY_CLASS),
                                       NULL);
    ok_hex(Status, STATUS_DATATYPE_MISALIGNMENT);

    /* Unaligned buffer -- wrong size (but this time do with an alignment of 2) */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessPriorityClass,
                                       (PVOID)2,
                                       0,
                                       NULL);
    ok_hex(Status, STATUS_INFO_LENGTH_MISMATCH);

    /* Unaligned buffer -- correct size (but this time do with an alignment of 2) */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessPriorityClass,
                                       (PVOID)2,
                                       sizeof(PROCESS_PRIORITY_CLASS),
                                       NULL);
    ok_hex(Status, STATUS_DATATYPE_MISALIGNMENT);

    /* Do not care for the length but expect to return the priority class */
    Status = NtQueryInformationProcess(NtCurrentProcess(),
                                       ProcessPriorityClass,
                                       ProcPriority,
                                       sizeof(PROCESS_PRIORITY_CLASS),
                                       NULL);
    ok_hex(Status, STATUS_SUCCESS);

    /* Make sure the returned priority class is a valid number (non negative) but also it should be within the PROCESS_PRIORITY_CLASS range */
    ok(ProcPriority->PriorityClass > PROCESS_PRIORITY_CLASS_INVALID && ProcPriority->PriorityClass <= PROCESS_PRIORITY_CLASS_ABOVE_NORMAL,
       "Expected a valid number from priority class range but got %d\n", ProcPriority->PriorityClass);
    free(ProcPriority);
}

START_TEST(NtQueryInformationProcess)
{
    NTSTATUS Status;

    Status = NtQuerySystemTime(&TestStartTime);
    ok_hex(Status, STATUS_SUCCESS);

    Test_ProcessTimes();
    Test_ProcessPriorityClassAlignment();
}
