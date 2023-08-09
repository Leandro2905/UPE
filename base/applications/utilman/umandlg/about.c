/*
 * PROJECT:         ReactOS Utility Manager Resources DLL (UManDlg.dll)
 * LICENSE:         GPL-2.0+ (https://spdx.org/licenses/GPL-2.0+)
 * PURPOSE:         About dialog file
 * COPYRIGHT:       Copyright 2019-2020 Bișoc George (fraizeraust99 at gmail dot com)
 */

/* INCLUDES *******************************************************************/

#include "umandlg.h"

/* GLOBALS ********************************************************************/

UTILMAN_GLOBALS Globals;

/* FUNCTIONS ******************************************************************/

/**
 * @AboutDlgProc
 *
 * "About" dialog procedure.
 *
 * @param hDlg
 * The handle object of the dialog.
 *
 * @param Msg
 * Message events (in unsigned int).
 *
 * @param wParam
 * Message parameter (in UINT_PTR).
 *
 * @param lParam
 * Message paramater (in LONG_PTR).
 *
 * @return
 * Return TRUE if the dialog processed messages,
 * FALSE otherwise.
 *
 */
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
        case WM_INITDIALOG:
        {
            WCHAR wszAppPath[MAX_BUFFER];

            /* Extract the icon resource from the executable process */
            GetModuleFileNameW(NULL, wszAppPath, _countof(wszAppPath));
            Globals.hIcon = ExtractIconW(Globals.hInstance, wszAppPath, 0);

            /* Set the icon within the dialog's title bar */
            if (Globals.hIcon)
            {
                SendMessageW(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)Globals.hIcon);
            }

            return TRUE;
        }

        case WM_COMMAND:
        {
            case IDOK:
            case IDCANCEL:
                DestroyIcon(Globals.hIcon);
                EndDialog(hDlg, FALSE);
                break;
        }
    }

    return FALSE;
}

/**
 * @ShowAboutDlg
 *
 * Display the "About" dialog.
 *
 * @param hDlgParent
 * The handle object of the parent dialog.
 *
 * @return
 * Nothing.
 */
VOID ShowAboutDlg(HWND hDlgParent)
{
    /* Display the "About" dialog when the user clicks on the "About" menu item */
    DialogBoxW(Globals.hInstance,
               MAKEINTRESOURCEW(IDD_ABOUT_DIALOG),
               hDlgParent,
               AboutDlgProc);
}
