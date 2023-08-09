/*
 * COPYRIGHT:       See COPYING.ARM in the top level directory
 * PROJECT:         ReactOS UEFI OS Loader
 * FILE:            boot/environ/app/rosload/rosload.h
 * PURPOSE:         Main OS Loader Header
 * PROGRAMMER:      Alex Ionescu (alex.ionescu@reactos.org)
*/

#ifndef _ROSLOAD_H
#define _ROSLOAD_H

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

/* ARC Headers */
#include <arc/arc.h>

/* STRUCTURES ****************************************************************/

typedef struct _OSL_BSD_ITEM_TABLE_ENTRY
{
    ULONG Offset;
    ULONG Size;
} OSL_BSD_ITEM_TABLE_ENTRY;

/* FUNCTIONS *****************************************************************/

VOID
NTAPI
OslDrawLogo (
    VOID
);

#endif
