/*
 * COPYRIGHT:       GPL - See COPYING in the top level directory
 * PROJECT:         ReactOS Virtual DOS Machine
 * FILE:            subsystems/mvdm/ntvdm/bios/bios32/moubios32.h
 * PURPOSE:         VDM 32-bit PS/2 Mouse BIOS
 * PROGRAMMERS:     Aleksandar Andrejevic <theflash AT sdf DOT lonestar DOT org>
 */

#ifndef _MOUBIOS32_H_
#define _MOUBIOS32_H_

/* DEFINES ********************************************************************/

/* FUNCTIONS ******************************************************************/

VOID BiosMousePs2Interface(LPWORD Stack);

VOID MouseBios32Post(VOID);
BOOLEAN MouseBiosInitialize(VOID);
VOID MouseBios32Cleanup(VOID);

#endif /* _MOUBIOS32_H_ */
