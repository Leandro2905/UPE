/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS Clip Command
 * FILE:            base/applications/cmdutils/clip/clip.c
 * PURPOSE:         Provides clipboard management for command-line programs.
 * PROGRAMMERS:     Ricardo Hanke
 */

#include <stdio.h>

#include <windef.h>
#include <winbase.h>
#include <winuser.h>

#include <conutils.h>

#include "resource.h"

VOID PrintError(DWORD dwError)
{
    if (dwError == ERROR_SUCCESS)
        return;

    ConMsgPuts(StdErr, FORMAT_MESSAGE_FROM_SYSTEM,
               NULL, dwError, LANG_USER_DEFAULT);
}

static BOOL IsDataUnicode(HGLOBAL hGlobal)
{
    BOOL bReturn;
    LPVOID lpBuffer;

    lpBuffer = GlobalLock(hGlobal);
    bReturn = IsTextUnicode(lpBuffer, GlobalSize(hGlobal), NULL);
    GlobalUnlock(hGlobal);

    return bReturn;
}

int wmain(int argc, wchar_t** argv)
{
    HANDLE hInput;
    DWORD dwBytesRead;
    BOOL bStatus;
    HGLOBAL hBuffer;
    HGLOBAL hTemp;
    LPBYTE lpBuffer;
    SIZE_T dwSize = 0;

    /* Initialize the Console Standard Streams */
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    ConInitStdStreams();

    /* Check for usage */
    if (argc > 1 && wcsncmp(argv[1], L"/?", 2) == 0)
    {
        ConResPuts(StdOut, IDS_HELP);
        return 0;
    }

    if (GetFileType(hInput) == FILE_TYPE_CHAR)
    {
        ConResPuts(StdOut, IDS_USAGE);
        return 0;
    }

    /* Initialize a growable buffer for holding clipboard data */
    hBuffer = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, 4096);
    if (!hBuffer)
        goto Failure;

    /*
     * Read data from the input stream by chunks of 4096 bytes
     * and resize the buffer each time when needed.
     */
    do
    {
        lpBuffer = GlobalLock(hBuffer);
        if (!lpBuffer)
            goto Failure;

        bStatus = ReadFile(hInput, lpBuffer + dwSize, 4096, &dwBytesRead, NULL);
        dwSize += dwBytesRead;
        GlobalUnlock(hBuffer);

        hTemp = GlobalReAlloc(hBuffer, GlobalSize(hBuffer) + 4096, GMEM_ZEROINIT);
        if (!hTemp)
            goto Failure;

        hBuffer = hTemp;
    }
    while (bStatus && dwBytesRead > 0);

    /*
     * Resize the buffer to the total size of data read.
     * Note that, if the call fails, we still have the old buffer valid.
     * The old buffer would be larger than the actual size of data it contains,
     * but this is not a problem for us.
     */
    hTemp = GlobalReAlloc(hBuffer, dwSize + sizeof(WCHAR), GMEM_ZEROINIT);
    if (hTemp)
        hBuffer = hTemp;

    /* Attempt to open the clipboard */
    if (!OpenClipboard(NULL))
        goto Failure;

    /* Empty it, copy our data, then close it */

    EmptyClipboard();

    if (IsDataUnicode(hBuffer))
    {
        SetClipboardData(CF_UNICODETEXT, hBuffer);
    }
    else
    {
        // TODO: Convert text from current console page to standard ANSI.
        // Alternatively one can use CF_OEMTEXT as done here.
        SetClipboardData(CF_OEMTEXT, hBuffer);
    }

    CloseClipboard();
    return 0;

Failure:
    if (hBuffer) GlobalFree(hBuffer);
    PrintError(GetLastError());
    return -1;
}
