
/* INCLUDES *******************************************************************/

// #include "ntvdm.h"

// #define NDEBUG
// #include <debug.h>

// #include "emulator.h"
#include "resource.h"

/* VARIABLES ******************************************************************/

static HANDLE CurrentConsoleOutput = INVALID_HANDLE_VALUE;

static HANDLE ConsoleInput  = INVALID_HANDLE_VALUE;
static HANDLE ConsoleOutput = INVALID_HANDLE_VALUE;
static DWORD  OrgConsoleInputMode, OrgConsoleOutputMode;

HWND hConsoleWnd = NULL;
static HMENU hConsoleMenu = NULL;
static INT   VdmMenuPos   = -1;
static BOOL  CaptureMouse = FALSE;

/*
 * Those menu helpers were taken from the GUI frontend in winsrv.dll
 */
typedef struct _VDM_MENUITEM
{
    UINT uID;
    const struct _VDM_MENUITEM *SubMenu;
    UINT_PTR uCmdID;
} VDM_MENUITEM, *PVDM_MENUITEM;

static const VDM_MENUITEM VdmMenuItems[] =
{
    { IDS_VDM_DUMPMEM_TXT, NULL, ID_VDM_DUMPMEM_TXT },
    { IDS_VDM_DUMPMEM_BIN, NULL, ID_VDM_DUMPMEM_BIN },
    { -1, NULL, 0 },    /* Separator */
    // { IDS_VDM_MOUNT_FLOPPY, NULL, ID_VDM_DRIVES },
    // { IDS_VDM_EJECT_FLOPPY, NULL, ID_VDM_DRIVES },
    { -1, NULL, 0 },    /* Separator */
    { IDS_VDM_QUIT       , NULL, ID_VDM_QUIT        },

    { 0, NULL, 0 }      /* End of list */
};

static const VDM_MENUITEM VdmMainMenuItems[] =
{
    { -1, NULL, 0 },    /* Separator */
    { IDS_CAPTURE_MOUSE, NULL, ID_CAPTURE_MOUSE },  /* "Capture mouse"; can be renamed to "Release mouse" */
    { IDS_VDM_MENU , VdmMenuItems, 0 },             /* ReactOS VDM Menu */

    { 0, NULL, 0 }      /* End of list */
};

static VOID
AppendMenuItems(HMENU hMenu,
                const VDM_MENUITEM *Items)
{
    UINT i = 0;
    WCHAR szMenuString[256];
    HMENU hSubMenu;

    do
    {
        if (Items[i].uID != (UINT)-1)
        {
            if (LoadStringW(GetModuleHandle(NULL),
                            Items[i].uID,
                            szMenuString,
                            ARRAYSIZE(szMenuString)) > 0)
            {
                if (Items[i].SubMenu != NULL)
                {
                    hSubMenu = CreatePopupMenu();
                    if (hSubMenu != NULL)
                    {
                        AppendMenuItems(hSubMenu, Items[i].SubMenu);

                        if (!AppendMenuW(hMenu,
                                         MF_STRING | MF_POPUP,
                                         (UINT_PTR)hSubMenu,
                                         szMenuString))
                        {
                            DestroyMenu(hSubMenu);
                        }
                    }
                }
                else
                {
                    AppendMenuW(hMenu,
                                MF_STRING,
                                Items[i].uCmdID,
                                szMenuString);
                }
            }
        }
        else
        {
            AppendMenuW(hMenu,
                        MF_SEPARATOR,
                        0,
                        NULL);
        }
        i++;
    } while (!(Items[i].uID == 0 && Items[i].SubMenu == NULL && Items[i].uCmdID == 0));
}

static BOOL
VdmMenuExists(HMENU hConsoleMenu)
{
    INT MenuPos, i;
    MenuPos = GetMenuItemCount(hConsoleMenu);

    /* Check for the presence of one of the VDM menu items */
    for (i = 0; i <= MenuPos; i++)
    {
        if (GetMenuItemID(hConsoleMenu, i) == ID_CAPTURE_MOUSE)
        {
            /* Set VdmMenuPos to the position of the existing menu */
            VdmMenuPos = i - 1;
            return TRUE;
        }
    }
    return FALSE;
}

static VOID
UpdateVdmMenuMouse(VOID)
{
    WCHAR szMenuString[256];

    /* Update "Capture/Release mouse" menu item */
    if (LoadStringW(GetModuleHandle(NULL),
                    (CaptureMouse ? IDS_RELEASE_MOUSE : IDS_CAPTURE_MOUSE),
                    szMenuString,
                    ARRAYSIZE(szMenuString)) > 0)
    {
        ModifyMenuW(hConsoleMenu, ID_CAPTURE_MOUSE,
                    MF_BYCOMMAND, ID_CAPTURE_MOUSE, szMenuString);
    }
}

/*static*/ VOID
UpdateVdmMenuDisks(VOID)
{
    UINT_PTR ItemID;
    USHORT i;

    WCHAR szNoMedia[100];
    WCHAR szMenuString1[256], szMenuString2[256];

    /* Update the disks menu items */

    LoadStringW(GetModuleHandle(NULL),
                IDS_NO_MEDIA,
                szNoMedia,
                ARRAYSIZE(szNoMedia));

    LoadStringW(GetModuleHandle(NULL),
                IDS_VDM_MOUNT_FLOPPY,
                szMenuString1,
                ARRAYSIZE(szMenuString1));

    for (i = 0; i < ARRAYSIZE(GlobalSettings.FloppyDisks); ++i)
    {
        ItemID = ID_VDM_DRIVES + (2 * i);

        if (GlobalSettings.FloppyDisks[i].Length != 0 &&
            GlobalSettings.FloppyDisks[i].Buffer      &&
           *GlobalSettings.FloppyDisks[i].Buffer != L'\0')
        {
            /* Update item text */
            _snwprintf(szMenuString2, ARRAYSIZE(szMenuString2), szMenuString1, i, GlobalSettings.FloppyDisks[i].Buffer);
            szMenuString2[ARRAYSIZE(szMenuString2) - 1] = UNICODE_NULL;
            ModifyMenuW(hConsoleMenu, ItemID, MF_BYCOMMAND | MF_STRING, ItemID, szMenuString2);

            /* Enable the eject item */
            EnableMenuItem(hConsoleMenu, ItemID + 1, MF_BYCOMMAND | MF_ENABLED);
        }
        else
        {
            /* Update item text */
            _snwprintf(szMenuString2, ARRAYSIZE(szMenuString2), szMenuString1, i, szNoMedia);
            szMenuString2[ARRAYSIZE(szMenuString2) - 1] = UNICODE_NULL;
            ModifyMenuW(hConsoleMenu, ItemID, MF_BYCOMMAND | MF_STRING, ItemID, szMenuString2);

            /* Disable the eject item */
            EnableMenuItem(hConsoleMenu, ItemID + 1, MF_BYCOMMAND | MF_GRAYED);
        }
    }
}

static VOID
UpdateVdmMenu(VOID)
{
    UpdateVdmMenuMouse();
    UpdateVdmMenuDisks();
}

static VOID
CreateVdmMenu(HANDLE ConOutHandle)
{
    HMENU hVdmSubMenu;
    UINT_PTR ItemID;
    UINT Pos;
    USHORT i;
    WCHAR szNoMedia[100];
    WCHAR szMenuString1[256], szMenuString2[256];

    hConsoleMenu = ConsoleMenuControl(ConOutHandle,
                                      ID_CAPTURE_MOUSE,
                                      ID_VDM_DRIVES + (2 * ARRAYSIZE(GlobalSettings.FloppyDisks)));
    if (hConsoleMenu == NULL) return;

    /* Get the position where we are going to insert our menu items */
    VdmMenuPos = GetMenuItemCount(hConsoleMenu);

    /* Really add the menu if it doesn't already exist (in case eg. NTVDM crashed) */
    if (!VdmMenuExists(hConsoleMenu))
    {
        /* Add all the menu entries */
        AppendMenuItems(hConsoleMenu, VdmMainMenuItems);

        /* Add the removable drives menu entries */
        hVdmSubMenu = GetSubMenu(hConsoleMenu, VdmMenuPos + 2); // VdmMenuItems
        Pos = 3; // After the 2 items and the separator in VdmMenuItems

        LoadStringW(GetModuleHandle(NULL),
                    IDS_NO_MEDIA,
                    szNoMedia,
                    ARRAYSIZE(szNoMedia));

        LoadStringW(GetModuleHandle(NULL),
                    IDS_VDM_MOUNT_FLOPPY,
                    szMenuString1,
                    ARRAYSIZE(szMenuString1));

        /* Drive 'x' -- Mount */
        for (i = 0; i < ARRAYSIZE(GlobalSettings.FloppyDisks); ++i)
        {
            ItemID = ID_VDM_DRIVES + (2 * i);

            /* Add the item */
            _snwprintf(szMenuString2, ARRAYSIZE(szMenuString2), szMenuString1, i, szNoMedia);
            szMenuString2[ARRAYSIZE(szMenuString2) - 1] = UNICODE_NULL;
            InsertMenuW(hVdmSubMenu, Pos++, MF_STRING | MF_BYPOSITION, ItemID, szMenuString2);
        }

        LoadStringW(GetModuleHandle(NULL),
                    IDS_VDM_EJECT_FLOPPY,
                    szMenuString1,
                    ARRAYSIZE(szMenuString1));

        /* Drive 'x' -- Eject */
        for (i = 0; i < ARRAYSIZE(GlobalSettings.FloppyDisks); ++i)
        {
            ItemID = ID_VDM_DRIVES + (2 * i);

            /* Add the item */
            _snwprintf(szMenuString2, ARRAYSIZE(szMenuString2), szMenuString1, i);
            szMenuString2[ARRAYSIZE(szMenuString2) - 1] = UNICODE_NULL;
            InsertMenuW(hVdmSubMenu, Pos++, MF_STRING | MF_BYPOSITION, ItemID + 1, szMenuString2);
        }

        /* Refresh the menu state */
        UpdateVdmMenu();
        DrawMenuBar(hConsoleWnd);
    }
}

static VOID
DestroyVdmMenu(VOID)
{
    UINT i = 0;
    const VDM_MENUITEM *Items = VdmMainMenuItems;

    do
    {
        DeleteMenu(hConsoleMenu, VdmMenuPos, MF_BYPOSITION);
        i++;
    } while (!(Items[i].uID == 0 && Items[i].SubMenu == NULL && Items[i].uCmdID == 0));

    DrawMenuBar(hConsoleWnd);
}

static VOID CaptureMousePointer(HANDLE ConOutHandle, BOOLEAN Capture)
{
    static BOOL IsClipped = FALSE; // For debugging purposes
    UNREFERENCED_PARAMETER(IsClipped);

    if (Capture)
    {
        RECT rcClip;

        // if (IsClipped) return;

        /* Be sure the cursor will be hidden */
        while (ShowConsoleCursor(ConOutHandle, FALSE) >= 0) ;

        GetClientRect(hConsoleWnd, &rcClip);
        MapWindowPoints(hConsoleWnd, HWND_DESKTOP /*NULL*/, (LPPOINT)&rcClip, 2 /* Magic value when the LPPOINT parameter is a RECT */);
        IsClipped = ClipCursor(&rcClip);
    }
    else
    {
        // if (!IsClipped) return;

        ClipCursor(NULL);
        IsClipped = FALSE;

        /* Be sure the cursor will be shown */
        while (ShowConsoleCursor(ConOutHandle, TRUE) < 0) ;
    }
}

static VOID EnableExtraHardware(HANDLE ConsoleInput)
{
    DWORD ConInMode;

    if (GetConsoleMode(ConsoleInput, &ConInMode))
    {
#if 0
        // GetNumberOfConsoleMouseButtons();
        // GetSystemMetrics(SM_CMOUSEBUTTONS);
        // GetSystemMetrics(SM_MOUSEPRESENT);
        if (MousePresent)
        {
#endif
            /* Support mouse input events if there is a mouse on the system */
            ConInMode |= ENABLE_MOUSE_INPUT;
#if 0
        }
        else
        {
            /* Do not support mouse input events if there is no mouse on the system */
            ConInMode &= ~ENABLE_MOUSE_INPUT;
        }
#endif

        SetConsoleMode(ConsoleInput, ConInMode);
    }
}







/* PUBLIC FUNCTIONS ***********************************************************/

/*static*/ VOID
VdmShutdown(BOOLEAN Immediate);

static BOOL
WINAPI
ConsoleCtrlHandler(DWORD ControlType)
{
    switch (ControlType)
    {
        case CTRL_LAST_CLOSE_EVENT:
        {
            /* Delayed shutdown */
            DPRINT1("NTVDM delayed killing in the CTRL_LAST_CLOSE_EVENT CtrlHandler!\n");
            VdmShutdown(FALSE);
            break;
        }

        default:
        {
            /* Stop the VDM if the user logs out or closes the console */
            DPRINT1("Killing NTVDM in the 'default' CtrlHandler!\n");
            VdmShutdown(TRUE);
        }
    }
    return TRUE;
}

static VOID
ConsoleInitUI(VOID)
{
    hConsoleWnd = GetConsoleWindow();
    CreateVdmMenu(ConsoleOutput);
}

static VOID
ConsoleCleanupUI(VOID)
{
    /* Display again properly the mouse pointer */
    if (CaptureMouse) CaptureMousePointer(ConsoleOutput, !CaptureMouse);

    DestroyVdmMenu();
}

BOOL
ConsoleAttach(VOID)
{
    /* Save the original input and output console modes */
    if (!GetConsoleMode(ConsoleInput , &OrgConsoleInputMode ) ||
        !GetConsoleMode(ConsoleOutput, &OrgConsoleOutputMode))
    {
        CloseHandle(ConsoleOutput);
        CloseHandle(ConsoleInput);
        wprintf(L"FATAL: Cannot save console in/out modes\n");
        return FALSE;
    }

    /* Set the console input mode */
    SetConsoleMode(ConsoleInput, ENABLE_WINDOW_INPUT);
    EnableExtraHardware(ConsoleInput);

    /* Set the console output mode */
    // SetConsoleMode(ConsoleOutput, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

    /* Initialize the UI */
    ConsoleInitUI();

    return TRUE;
}

VOID
ConsoleDetach(VOID)
{
    /* Cleanup the UI */
    ConsoleCleanupUI();

    /* Restore the original input and output console modes */
    SetConsoleMode(ConsoleOutput, OrgConsoleOutputMode);
    SetConsoleMode(ConsoleInput , OrgConsoleInputMode );
}

VOID
ConsoleReattach(HANDLE ConOutHandle)
{
    DestroyVdmMenu();
    CurrentConsoleOutput = ConOutHandle;
    CreateVdmMenu(ConOutHandle);

    /* Synchronize mouse cursor display with console screenbuffer switches */
    CaptureMousePointer(CurrentConsoleOutput, CaptureMouse);
}

static BOOL
ConsoleInit(VOID)
{
    /* Set the handler routine */
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    /* Enable the CTRL_LAST_CLOSE_EVENT */
    SetLastConsoleEventActive();

    /*
     * NOTE: The CONIN$ and CONOUT$ "virtual" files
     * always point to non-redirected console handles.
     */

    /* Get the input handle to the real console, and check for success */
    ConsoleInput = CreateFileW(L"CONIN$",
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL,
                               OPEN_EXISTING,
                               0,
                               NULL);
    if (ConsoleInput == INVALID_HANDLE_VALUE)
    {
        wprintf(L"FATAL: Cannot retrieve a handle to the console input\n");
        return FALSE;
    }

    /* Get the output handle to the real console, and check for success */
    ConsoleOutput = CreateFileW(L"CONOUT$",
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_EXISTING,
                                0,
                                NULL);
    if (ConsoleOutput == INVALID_HANDLE_VALUE)
    {
        CloseHandle(ConsoleInput);
        wprintf(L"FATAL: Cannot retrieve a handle to the console output\n");
        return FALSE;
    }

    /* Effectively attach to the console */
    return ConsoleAttach();
}

static VOID
ConsoleCleanup(VOID)
{
    /* Detach from the console */
    ConsoleDetach();

    /* Close the console handles */
    if (ConsoleOutput != INVALID_HANDLE_VALUE) CloseHandle(ConsoleOutput);
    if (ConsoleInput  != INVALID_HANDLE_VALUE) CloseHandle(ConsoleInput);
}

BOOL IsConsoleHandle(HANDLE hHandle)
{
    DWORD dwMode;

    /* Check whether the handle may be that of a console... */
    if ((GetFileType(hHandle) & ~FILE_TYPE_REMOTE) != FILE_TYPE_CHAR)
        return FALSE;

    /*
     * It may be. Perform another test... The idea comes from the
     * MSDN description of the WriteConsole API:
     *
     * "WriteConsole fails if it is used with a standard handle
     *  that is redirected to a file. If an application processes
     *  multilingual output that can be redirected, determine whether
     *  the output handle is a console handle (one method is to call
     *  the GetConsoleMode function and check whether it succeeds).
     *  If the handle is a console handle, call WriteConsole. If the
     *  handle is not a console handle, the output is redirected and
     *  you should call WriteFile to perform the I/O."
     */
    return GetConsoleMode(hHandle, &dwMode);
}

VOID MenuEventHandler(PMENU_EVENT_RECORD MenuEvent)
{
    switch (MenuEvent->dwCommandId)
    {
        /*
         * System-defined menu commands
         */

        case WM_INITMENU:
        case WM_MENUSELECT:
        {
            /*
             * If the mouse is captured, release it or recapture it
             * when the menu opens or closes, respectively.
             */
            if (!CaptureMouse) break;
            CaptureMousePointer(CurrentConsoleOutput, MenuEvent->dwCommandId == WM_INITMENU ? FALSE : TRUE);
            break;
        }


        /*
         * User-defined menu commands
         */

        case ID_CAPTURE_MOUSE:
            CaptureMouse = !CaptureMouse;
            CaptureMousePointer(CurrentConsoleOutput, CaptureMouse);
            UpdateVdmMenuMouse();
            break;

        case ID_VDM_DUMPMEM_TXT:
            DumpMemory(TRUE);
            break;

        case ID_VDM_DUMPMEM_BIN:
            DumpMemory(FALSE);
            break;

        /* Drive 0 -- Mount */
        /* Drive 1 -- Mount */
        case ID_VDM_DRIVES + 0:
        case ID_VDM_DRIVES + 2:
        {
            ULONG DiskNumber = (MenuEvent->dwCommandId - ID_VDM_DRIVES) / 2;
            MountFloppy(DiskNumber);
            break;
        }

        /* Drive 0 -- Eject */
        /* Drive 1 -- Eject */
        case ID_VDM_DRIVES + 1:
        case ID_VDM_DRIVES + 3:
        {
            ULONG DiskNumber = (MenuEvent->dwCommandId - ID_VDM_DRIVES - 1) / 2;
            EjectFloppy(DiskNumber);
            break;
        }

        case ID_VDM_QUIT:
            /* Stop the VDM */
            // EmulatorTerminate();

            /* Nothing runs, so exit immediately */
            DPRINT1("Killing NTVDM via console menu!\n");
            VdmShutdown(TRUE);
            break;

        default:
            break;
    }
}

VOID FocusEventHandler(PFOCUS_EVENT_RECORD FocusEvent)
{
    /*
     * If the mouse is captured, release it or recapture it
     * when we lose or regain focus, respectively.
     */
    if (!CaptureMouse) return;
    CaptureMousePointer(CurrentConsoleOutput, FocusEvent->bSetFocus);
}
