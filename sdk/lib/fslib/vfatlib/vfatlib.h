/*
 * COPYRIGHT:   See COPYING in the top level directory
 * PROJECT:     ReactOS VFAT filesystem library
 * FILE:        vfatlib.h
 */

#ifndef _VFATLIB_H_
#define _VFATLIB_H_

#include <stdio.h>
#include <stdlib.h>

#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H
#include <windef.h>
#include <winbase.h>
#define NTOS_MODE_USER
#include <ndk/iofuncs.h>
#include <ndk/kefuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/rtlfuncs.h>
#include <fmifs/fmifs.h>

#include "check/dosfsck.h"

#include <pshpack1.h>
typedef struct _FAT16_BOOT_SECTOR
{
    unsigned char  Jump[3];                     // 0
    unsigned char  OEMName[8];                  // 3
    unsigned short BytesPerSector;              // 11
    unsigned char  SectorsPerCluster;           // 13
    unsigned short ReservedSectors;             // 14
    unsigned char  FATCount;                    // 16
    unsigned short RootEntries;                 // 17
    unsigned short Sectors;                     // 19
    unsigned char  Media;                       // 21
    unsigned short FATSectors;                  // 22
    unsigned short SectorsPerTrack;             // 24
    unsigned short Heads;                       // 26
    unsigned long  HiddenSectors;               // 28
    unsigned long  SectorsHuge;                 // 32
    unsigned char  Drive;                       // 36
    unsigned char  Res1;                        // 37
    unsigned char  ExtBootSignature;            // 38
    unsigned long  VolumeID;                    // 39
    unsigned char  VolumeLabel[11];             // 43
    unsigned char  SysType[8];                  // 54
    unsigned char  Res2[446];                   // 62
    unsigned long  Signature1;                  // 508
} FAT16_BOOT_SECTOR, *PFAT16_BOOT_SECTOR;

typedef struct _FAT32_BOOT_SECTOR
{
    unsigned char  Jump[3];                     // 0
    unsigned char  OEMName[8];                  // 3
    unsigned short BytesPerSector;              // 11
    unsigned char  SectorsPerCluster;           // 13
    unsigned short ReservedSectors;             // 14
    unsigned char  FATCount;                    // 16
    unsigned short RootEntries;                 // 17
    unsigned short Sectors;                     // 19
    unsigned char  Media;                       // 21
    unsigned short FATSectors;                  // 22
    unsigned short SectorsPerTrack;             // 24
    unsigned short Heads;                       // 26
    unsigned long  HiddenSectors;               // 28
    unsigned long  SectorsHuge;                 // 32
    unsigned long  FATSectors32;                // 36
    unsigned short ExtFlag;                     // 40
    unsigned short FSVersion;                   // 42
    unsigned long  RootCluster;                 // 44
    unsigned short FSInfoSector;                // 48
    unsigned short BootBackup;                  // 50
    unsigned char  Res3[12];                    // 52
    unsigned char  Drive;                       // 64
    unsigned char  Res4;                        // 65
    unsigned char  ExtBootSignature;            // 66
    unsigned long  VolumeID;                    // 67
    unsigned char  VolumeLabel[11];             // 71
    unsigned char  SysType[8];                  // 82
    unsigned char  Res2[418];                   // 90
    unsigned long  Signature1;                  // 508
} FAT32_BOOT_SECTOR, *PFAT32_BOOT_SECTOR;

typedef struct _FAT32_FSINFO
{
    unsigned long  LeadSig;          // 0
    unsigned char  Res1[480];        // 4
    unsigned long  StrucSig;         // 484
    unsigned long  FreeCount;        // 488
    unsigned long  NextFree;         // 492
    unsigned long  Res2[3];          // 496
    unsigned long  TrailSig;         // 508
} FAT32_FSINFO, *PFAT32_FSINFO;
#include <poppack.h>

#define FSINFO_SECTOR_BEGIN_SIGNATURE   0x41615252  // 'RRaA'
#define FSINFO_SECTOR_END_SIGNATURE     0xAA550000
#define FSINFO_SIGNATURE                0x61417272  // 'rrAa'

typedef struct _FORMAT_CONTEXT
{
    PFMIFSCALLBACK Callback;
    ULONG TotalSectorCount;
    ULONG CurrentSectorCount;
    BOOLEAN Success;
    ULONG Percent;
} FORMAT_CONTEXT, *PFORMAT_CONTEXT;

#include "common.h"


NTSTATUS
Fat12Format(HANDLE FileHandle,
            PPARTITION_INFORMATION PartitionInfo,
            PDISK_GEOMETRY DiskGeometry,
            PUNICODE_STRING Label,
            BOOLEAN QuickFormat,
            ULONG ClusterSize,
            PFORMAT_CONTEXT Context);

NTSTATUS
Fat16Format(HANDLE FileHandle,
            PPARTITION_INFORMATION PartitionInfo,
            PDISK_GEOMETRY DiskGeometry,
            PUNICODE_STRING Label,
            BOOLEAN QuickFormat,
            ULONG ClusterSize,
            PFORMAT_CONTEXT Context);

NTSTATUS
Fat32Format(HANDLE FileHandle,
            PPARTITION_INFORMATION PartitionInfo,
            PDISK_GEOMETRY DiskGeometry,
            PUNICODE_STRING Label,
            BOOLEAN QuickFormat,
            ULONG ClusterSize,
            PFORMAT_CONTEXT Context);

VOID
UpdateProgress(PFORMAT_CONTEXT Context,
               ULONG Increment);

VOID
VfatPrintV(PCHAR Format, va_list args);

VOID
VfatPrint(PCHAR Format, ...);

#endif /* _VFATLIB_H_ */

/* EOF */
