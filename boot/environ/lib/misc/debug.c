/*
 * COPYRIGHT:       See COPYING.ARM in the top level directory
 * PROJECT:         ReactOS UEFI Boot Library
 * FILE:            boot/environ/lib/misc/debug.c
 * PURPOSE:         Boot Library Debug Routines
 * PROGRAMMER:      Alex Ionescu (alex.ionescu@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include "bl.h"

/* DATA VARIABLES ************************************************************/

CHAR AnsiBuffer[1024];
BOOLEAN BdDebuggerNotPresent;
BOOLEAN BdSubsystemInitialized;
BOOLEAN BdArchBlockDebuggerOperation;
BOOLEAN BlpStatusErrorInProgress;
PBL_STATUS_ERROR_HANDLER BlpStatusErrorHandler;

/* FUNCTIONS *****************************************************************/

BOOLEAN
BdDebuggerInitialized (
    VOID
    )
{
    /* Check if BD was initialized, and is currently usable */
    return BdSubsystemInitialized && !BdArchBlockDebuggerOperation;
}

NTSTATUS
BlBdPullRemoteFile (
    _In_ PWCHAR FilePath,
    _Out_ PVOID BaseAddress,
    _Out_ PULONGLONG FileSize
    )
{
    /* Is the boot debugger enabled? */
    if (!BlBdDebuggerEnabled())
    {
        /* Nothing to pull */
        return STATUS_DEBUGGER_INACTIVE;
    }

    /* TODO */
    EfiPrintf(L"Todo\r\n");
    return STATUS_NOT_IMPLEMENTED;
}

BOOLEAN
BlBdDebuggerEnabled (
    VOID
    )
{
    BOOLEAN Initialized, Enabled;

    /* Check if the debugger is initialized */
    Initialized = BdDebuggerInitialized();

    /* Check if it's currently active */
    Enabled = FALSE;
    if ((Initialized) && !(BdDebuggerNotPresent))
    {
        /* Yep! */
        Enabled = TRUE;
    }

    /* Return enabled state */
    return Enabled;
}

VOID
BlStatusPrint (
    _In_ PCWCH Format,
    ...
    )
{
    ANSI_STRING AnsiString;
    UNICODE_STRING UnicodeString;
    va_list va;
    NTSTATUS Status;

    va_start(va, Format);

    /* Check if the boot debugger is enabled */
    if (BlBdDebuggerEnabled()
#if DBG
        || TRUE
#endif
    )
    {
        /* Print the string out into a buffer */
        if (vswprintf(BlScratchBuffer, Format, va) > 0)
        {
#if DBG
            EfiPrintf(BlScratchBuffer);
            EfiPrintf(L"\r\n");
#endif
            /* Make it a UNICODE_STRING */
            RtlInitUnicodeString(&UnicodeString, BlScratchBuffer);

            /* Then convert it into an ANSI_STRING */
            AnsiString.Length = 0;
            AnsiString.MaximumLength = sizeof(AnsiBuffer);
            AnsiString.Buffer = AnsiBuffer;
            Status = RtlUnicodeStringToAnsiString(&AnsiString, &UnicodeString, FALSE);
            if (NT_SUCCESS(Status))
            {
                /* Print it out to the debugger if that worked */
                DbgPrint(AnsiString.Buffer);
            }
        }
    }

    va_end(va);
}

VOID
BlStatusError (
    _In_ ULONG ErrorCode,
    _In_ ULONG Parameter1,
    _In_ ULONG_PTR Parameter2,
    _In_ ULONG_PTR Parameter3,
    _In_ ULONG_PTR Parameter4
    )
{
    NTSTATUS Status;

    /* Is this a non-boot error? */
    if (ErrorCode != 1)
    {
        /* Is one already ongoing? */
        if (!BlpStatusErrorInProgress)
        {
            /* Do we have a custom error handler registered? */
            if (BlpStatusErrorHandler)
            {
                /* Call it, making sure to avoid recursion */
                BlpStatusErrorInProgress = TRUE;
                Status = BlpStatusErrorHandler(ErrorCode,
                                               Parameter1,
                                               Parameter2,
                                               Parameter3,
                                               Parameter4);
                BlpStatusErrorInProgress = FALSE;

                /* If the error handler consumed the error, we're done */
                if (NT_SUCCESS(Status))
                {
                    return;
                }
            }
        }
    }

    /* Check if the boot debugger is enabled */
    if (BlBdDebuggerEnabled())
    {
        /* Print out the fatal error */
        BlStatusPrint(L"\n"
                      L"*** Fatal Error 0x%08x :\n"
                      L"                (0x%p, 0x%p, 0x%p, 0x%p)\n"
                      L"\n",
                      ErrorCode,
                      Parameter1,
                      Parameter2,
                      Parameter3,
                      Parameter4);

        /* Issue a breakpoint */
        __debugbreak();
    }
}

