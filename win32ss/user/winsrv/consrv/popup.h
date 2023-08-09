/*
 * LICENSE:         GPL - See COPYING in the top level directory
 * PROJECT:         ReactOS Console Server DLL
 * FILE:            win32ss/user/winsrv/consrv/popup.h
 * PURPOSE:         Console popup windows
 * PROGRAMMERS:     Hermes Belusca-Maito (hermes.belusca@sfr.fr)
 */

#pragma once

typedef
VOID
(NTAPI *PPOPUP_INPUT_ROUTINE)(VOID);

typedef struct _POPUP_WINDOW
{
    LIST_ENTRY  ListEntry;          /* Entry in console's list of popups */
    PTEXTMODE_SCREEN_BUFFER ScreenBuffer;   /* Associated screen-buffer */

    // SMALL_RECT Region;           /* The region the popup occupies */
    COORD       Origin;             /* Origin of the popup window */
    COORD       Size;               /* Size of the popup window */

    PCHAR_INFO  OldContents;        /* The data under the popup window */
    PPOPUP_INPUT_ROUTINE PopupInputRoutine; /* Routine called when input is received */
} POPUP_WINDOW, *PPOPUP_WINDOW;


PPOPUP_WINDOW
CreatePopupWindow(
    IN PCONSRV_CONSOLE Console,
    IN PCONSOLE_SCREEN_BUFFER ScreenBuffer,
    IN SHORT xLeft,
    IN SHORT yTop,
    IN SHORT Width,
    IN SHORT Height);

VOID
DestroyPopupWindow(
    IN PPOPUP_WINDOW Popup);
