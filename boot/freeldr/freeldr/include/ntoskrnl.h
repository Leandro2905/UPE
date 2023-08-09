/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            boot/freeldr/freeldr/include/ntoskrnl.h
 * PURPOSE:         NTOS glue routines for the MINIHAL library
 * PROGRAMMERS:     Herv� Poussineau  <hpoussin@reactos.org>
 */

#include <ntdef.h>
#undef _NTHAL_
//#undef DECLSPEC_IMPORT
//#define DECLSPEC_IMPORT
#undef NTSYSAPI
#define NTSYSAPI

/* Windows Device Driver Kit */
#include <ntddk.h>
#include <ndk/haltypes.h>

//typedef GUID UUID;

/* Disk stuff */
#include <arc/arc.h>
#include <ntdddisk.h>
#include <internal/hal.h>
