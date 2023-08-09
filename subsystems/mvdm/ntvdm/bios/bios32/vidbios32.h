/*
 * COPYRIGHT:       GPL - See COPYING in the top level directory
 * PROJECT:         ReactOS Virtual DOS Machine
 * FILE:            subsystems/mvdm/ntvdm/bios/bios32/vidbios32.h
 * PURPOSE:         VDM 32-bit Video BIOS
 * PROGRAMMERS:     Hermes Belusca-Maito (hermes.belusca@sfr.fr)
 *
 * NOTE:            All of the real code is in bios/vidbios.c
 */

#ifndef _VIDBIOS32_H_
#define _VIDBIOS32_H_

/* DEFINES ********************************************************************/

// #define BIOS_VIDEO_INTERRUPT    0x10

/* FUNCTIONS ******************************************************************/

BOOLEAN VidBios32Initialize(VOID);
VOID VidBios32Cleanup(VOID);

#endif /* _VIDBIOS32_H_ */
