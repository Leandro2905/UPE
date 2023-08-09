/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS Console Server DLL
 * FILE:            win32ss/user/winsrv/consrv/coninput.h
 * PURPOSE:         Console Input functions
 * PROGRAMMERS:     Jeffrey Morlan
 *                  Hermes Belusca-Maito (hermes.belusca@sfr.fr)
 */

#pragma once

typedef struct ConsoleInput_t
{
    LIST_ENTRY ListEntry;
    INPUT_RECORD InputEvent;
} ConsoleInput;


NTSTATUS NTAPI
ConDrvInitInputBuffer(IN PCONSOLE Console,
                      IN ULONG InputBufferSize);
VOID NTAPI
ConDrvDeinitInputBuffer(IN PCONSOLE Console);
