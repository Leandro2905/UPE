/*
 * PROJECT:     ReactOS USB EHCI Miniport Driver
 * LICENSE:     GPL-2.0+ (https://spdx.org/licenses/GPL-2.0+)
 * PURPOSE:     USBEHCI declarations
 * COPYRIGHT:   Copyright 2017-2018 Vadim Galyant <vgal@rambler.ru>
 */

#ifndef USBEHCI_H__
#define USBEHCI_H__

#include <ntddk.h>
#include <windef.h>
#include <stdio.h>
#include <hubbusif.h>
#include <usbbusif.h>
#include <usbdlib.h>
#include <drivers/usbport/usbmport.h>
#include "hardware.h"

extern USBPORT_REGISTRATION_PACKET RegPacket;

#define EHCI_MAX_CONTROL_TRANSFER_SIZE    0x10000
#define EHCI_MAX_INTERRUPT_TRANSFER_SIZE  0x1000
#define EHCI_MAX_BULK_TRANSFER_SIZE       0x400000
#define EHCI_MAX_FS_ISO_TRANSFER_SIZE     0x40000
#define EHCI_MAX_HS_ISO_TRANSFER_SIZE     0x180000

#define EHCI_MAX_FS_ISO_HEADER_BUFFER_SIZE  0x1000
#define EHCI_MAX_HS_ISO_HEADER_BUFFER_SIZE  0x40000

#define EHCI_MAX_CONTROL_TD_COUNT    6
#define EHCI_MAX_INTERRUPT_TD_COUNT  4
#define EHCI_MAX_BULK_TD_COUNT       209

#define EHCI_FRAMES       32
#define EHCI_MICROFRAMES  8

#define EHCI_MAX_HC_SYSTEM_ERRORS  256

typedef struct _EHCI_PERIOD {
  UCHAR Period;
  UCHAR PeriodIdx;
  UCHAR ScheduleMask;
} EHCI_PERIOD, *PEHCI_PERIOD;

/* Transfer Descriptor */
#define EHCI_HCD_TD_FLAG_ALLOCATED 0x01
#define EHCI_HCD_TD_FLAG_PROCESSED 0x02
#define EHCI_HCD_TD_FLAG_DONE      0x08
#define EHCI_HCD_TD_FLAG_ACTIVE    0x10
#define EHCI_HCD_TD_FLAG_DUMMY     0x20

struct _EHCI_HCD_QH;
struct _EHCI_ENDPOINT;
struct _EHCI_TRANSFER;

typedef struct _EHCI_HCD_TD {
  /* Hardware*/
  EHCI_QUEUE_TD HwTD;
  /* Software */
  ULONG PhysicalAddress;
  ULONG TdFlags;
  struct _EHCI_ENDPOINT * EhciEndpoint;
  struct _EHCI_TRANSFER * EhciTransfer;
  struct _EHCI_HCD_TD * NextHcdTD;
  struct _EHCI_HCD_TD * AltNextHcdTD;
  USB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;
  ULONG LengthThisTD;
  LIST_ENTRY DoneLink;
#ifdef _WIN64
  ULONG Pad[31];
#else
  ULONG Pad[40];
#endif
} EHCI_HCD_TD, *PEHCI_HCD_TD;

C_ASSERT(sizeof(EHCI_HCD_TD) == 0x100);

/* Queue Head */
#define EHCI_QH_FLAG_IN_SCHEDULE  0x01
#define EHCI_QH_FLAG_CLOSED       0x02
#define EHCI_QH_FLAG_STATIC       0x04
#define EHCI_QH_FLAG_STATIC_FAST  0x08
#define EHCI_QH_FLAG_UPDATING     0x10
#define EHCI_QH_FLAG_NUKED        0x20

typedef struct _EHCI_STATIC_QH {
  /* Hardware part */
  EHCI_QUEUE_HEAD HwQH;
  /* Software part */
  ULONG QhFlags;
  ULONG PhysicalAddress;
  struct _EHCI_HCD_QH * PrevHead;
#ifndef _WIN64
  ULONG Pad2;
#endif
  struct _EHCI_HCD_QH * NextHead;
#ifndef _WIN64
  ULONG Pad3;
#endif
  struct _EHCI_STATIC_QH * StaticQH;
#ifndef _WIN64
  ULONG Pad4;
#endif
  ULONG Period;
  ULONG Ordinal;
#ifdef _WIN64
  ULONG Pad[11];
#else
  ULONG Pad[13];
#endif
} EHCI_STATIC_QH, *PEHCI_STATIC_QH;

C_ASSERT(sizeof(EHCI_STATIC_QH) == 0xA0);

#define EHCI_DUMMYQH_MAX_PACKET_LENGTH  64

typedef struct _EHCI_HCD_QH {
  EHCI_STATIC_QH sqh;
#ifdef _WIN64
  ULONG Pad[23];
#else
  ULONG Pad[24];
#endif
} EHCI_HCD_QH, *PEHCI_HCD_QH;

C_ASSERT(sizeof(EHCI_HCD_QH) == 0x100);

/* EHCI Endpoint follows USBPORT Endpoint */
typedef struct _EHCI_ENDPOINT {
  ULONG Reserved;
  ULONG EndpointStatus;
  ULONG EndpointState;
  USBPORT_ENDPOINT_PROPERTIES EndpointProperties;
  PVOID DmaBufferVA;
  ULONG DmaBufferPA;
  PEHCI_HCD_TD FirstTD;
  ULONG MaxTDs;
  ULONG PendingTDs;
  ULONG RemainTDs;
  PEHCI_HCD_QH QH;
  PEHCI_HCD_TD HcdHeadP;
  PEHCI_HCD_TD HcdTailP;
  LIST_ENTRY ListTDs;
  const EHCI_PERIOD * PeriodTable;
  PEHCI_STATIC_QH StaticQH;
} EHCI_ENDPOINT, *PEHCI_ENDPOINT;

/* EHCI Transfer follows USBPORT Transfer */
typedef struct _EHCI_TRANSFER {
  ULONG Reserved;
  PUSBPORT_TRANSFER_PARAMETERS TransferParameters;
  ULONG USBDStatus;
  ULONG TransferLen;
  PEHCI_ENDPOINT EhciEndpoint;
  ULONG PendingTDs;
  ULONG TransferOnAsyncList;
} EHCI_TRANSFER, *PEHCI_TRANSFER;

typedef struct _EHCI_HC_RESOURCES {
  ULONG PeriodicFrameList[EHCI_FRAME_LIST_MAX_ENTRIES]; // 4K-page aligned array
  EHCI_STATIC_QH AsyncHead;
  EHCI_STATIC_QH PeriodicHead[64];
  UCHAR Padded[0x160];
  EHCI_HCD_QH IsoDummyQH[EHCI_FRAME_LIST_MAX_ENTRIES];
} EHCI_HC_RESOURCES, *PEHCI_HC_RESOURCES;

#define EHCI_FLAGS_CONTROLLER_SUSPEND 0x01
#define EHCI_FLAGS_IDLE_SUPPORT       0x20

/* EHCI Extension follows USBPORT Extension */
typedef struct _EHCI_EXTENSION {
  ULONG Reserved;
  ULONG Flags;
  PEHCI_HC_CAPABILITY_REGISTERS CapabilityRegisters;
  PEHCI_HW_REGISTERS OperationalRegs;
  UCHAR FrameLengthAdjustment;
  BOOLEAN IsStarted;
  USHORT HcSystemErrors;
  ULONG PortRoutingControl;
  USHORT NumberOfPorts;
  USHORT PortPowerControl;
  EHCI_INTERRUPT_ENABLE InterruptMask;
  EHCI_INTERRUPT_ENABLE InterruptStatus;
  /* Schedule */
  PEHCI_HC_RESOURCES HcResourcesVA;
  ULONG HcResourcesPA;
  PEHCI_STATIC_QH AsyncHead;
  PEHCI_STATIC_QH PeriodicHead[64];
  PEHCI_HCD_QH IsoDummyQHListVA;
  ULONG IsoDummyQHListPA;
  ULONG FrameIndex;
  ULONG FrameHighPart;
  /* Root Hub Bits */
  ULONG ConnectPortBits;
  ULONG SuspendPortBits;
  ULONG ResetPortBits;
  ULONG FinishResetPortBits;
  /* Transfers */
  ULONG PendingTransfers;
  /* Lock Queue */
  PEHCI_HCD_QH PrevQH;
  PEHCI_HCD_QH LockQH;
  PEHCI_HCD_QH NextQH;
  /* Registers Copy Backup */
  ULONG BackupPeriodiclistbase;
  ULONG BackupAsynclistaddr;
  ULONG BackupCtrlDSSegment;
  ULONG BackupUSBCmd;
} EHCI_EXTENSION, *PEHCI_EXTENSION;

/* debug.c */
VOID
NTAPI
EHCI_DumpHwTD(
  IN PEHCI_HCD_TD TD);

VOID
NTAPI
EHCI_DumpHwQH(
  IN PEHCI_HCD_QH QH);

/* roothub.c */
MPSTATUS
NTAPI
EHCI_RH_ChirpRootPort(
  IN PVOID ehciExtension,
  IN USHORT Port);

VOID
NTAPI
EHCI_RH_GetRootHubData(
  IN PVOID ohciExtension,
  IN PVOID rootHubData);

MPSTATUS
NTAPI
EHCI_RH_GetStatus(
  IN PVOID ohciExtension,
  IN PUSHORT Status);

MPSTATUS
NTAPI
EHCI_RH_GetPortStatus(
  IN PVOID ohciExtension,
  IN USHORT Port,
  IN PUSB_PORT_STATUS_AND_CHANGE PortStatus);

MPSTATUS
NTAPI
EHCI_RH_GetHubStatus(
  IN PVOID ohciExtension,
  IN PUSB_HUB_STATUS_AND_CHANGE HubStatus);

MPSTATUS
NTAPI
EHCI_RH_SetFeaturePortReset(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_SetFeaturePortPower(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_SetFeaturePortEnable(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_SetFeaturePortSuspend(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_ClearFeaturePortEnable(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_ClearFeaturePortPower(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_ClearFeaturePortSuspend(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_ClearFeaturePortEnableChange(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_ClearFeaturePortConnectChange(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_ClearFeaturePortResetChange(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_ClearFeaturePortSuspendChange(
  IN PVOID ohciExtension,
  IN USHORT Port);

MPSTATUS
NTAPI
EHCI_RH_ClearFeaturePortOvercurrentChange(
  IN PVOID ohciExtension,
  IN USHORT Port);

VOID
NTAPI
EHCI_RH_DisableIrq(
  IN PVOID ohciExtension);

VOID
NTAPI
EHCI_RH_EnableIrq(
  IN PVOID ohciExtension);

#endif /* USBEHCI_H__ */
