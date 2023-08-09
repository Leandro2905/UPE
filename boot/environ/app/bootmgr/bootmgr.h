/*
 * COPYRIGHT:       See COPYING.ARM in the top level directory
 * PROJECT:         ReactOS UEFI Boot Manager
 * FILE:            boot/environ/app/bootmgr/bootmgr.h
 * PURPOSE:         Main Boot Manager Header
 * PROGRAMMER:      Alex Ionescu (alex.ionescu@reactos.org)
*/

#ifndef _BOOTMGR_H
#define _BOOTMGR_H

/* INCLUDES ******************************************************************/

/* C Headers */
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

/* NT Base Headers */
#include <initguid.h>
#include <ntifs.h>

/* UEFI Headers */
#include <Uefi.h>

/* Boot Library Headers */
#include <bl.h>

/* BCD Headers */
#include <bcd.h>

/* Message Header */
#include <bootmsg.h>

/* STRUCTURES ****************************************************************/

typedef struct _BL_BOOT_ERROR
{
    ULONG ErrorCode;
    NTSTATUS ErrorStatus;
    ULONG Unknown1;
    PWCHAR ErrorString;
    PWCHAR FileName;
    ULONG HelpMsgId;
    ULONG Unknown2;
} BL_BOOT_ERROR, *PBL_BOOT_ERROR;

typedef struct _BL_PACKED_BOOT_ERROR
{
    PBL_BOOT_ERROR BootError;
    ULONG Unknown;
    ULONG Size;
} BL_PACKED_BOOT_ERROR, *PBL_PACKED_BOOT_ERROR;

#define BL_FATAL_ERROR_BCD_READ     0x01
#define BL_FATAL_ERROR_APP_LOAD     0x02
#define BL_FATAL_ERROR_BCD_ENTRIES  0x03
#define BL_FATAL_ERROR_GENERIC      0x04
#define BL_FATAL_ERROR_BCD_PARSE    0x07
#define BL_FATAL_ERROR_NO_PAE       0x0B

/* FUNCTIONS *****************************************************************/

NTSTATUS
NTAPI
BmMain (
    _In_ PBOOT_APPLICATION_PARAMETER_BLOCK BootParameters
    );

NTSTATUS
BmpLaunchBootEntry (
    _In_ PBL_LOADED_APPLICATION_ENTRY BootEntry,
    _Out_ PULONG EntryIndex,
    _In_ ULONG LaunchCode,
    _In_ BOOLEAN LaunchWinRe
    );

#endif
