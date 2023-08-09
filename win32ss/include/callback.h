#ifndef __INCLUDE_USER32_CALLBACK_H
#define __INCLUDE_USER32_CALLBACK_H

#define USER32_CALLBACK_WINDOWPROC            (0)
#define USER32_CALLBACK_SENDASYNCPROC         (1)
#define USER32_CALLBACK_LOADSYSMENUTEMPLATE   (2)
#define USER32_CALLBACK_LOADDEFAULTCURSORS    (3)
#define USER32_CALLBACK_HOOKPROC              (4)
#define USER32_CALLBACK_EVENTPROC             (5)
#define USER32_CALLBACK_LOADMENU              (6)
#define USER32_CALLBACK_CLIENTTHREADSTARTUP   (7)
#define USER32_CALLBACK_CLIENTLOADLIBRARY     (8)
#define USER32_CALLBACK_GETCHARSETINFO        (9)
#define USER32_CALLBACK_COPYIMAGE             (10)
#define USER32_CALLBACK_SETWNDICONS           (11)
#define USER32_CALLBACK_DELIVERUSERAPC        (12)
#define USER32_CALLBACK_DDEPOST               (13)
#define USER32_CALLBACK_DDEGET                (14)
#define USER32_CALLBACK_SETOBM                (15)
#define USER32_CALLBACK_LPK                   (16)
#define USER32_CALLBACK_UMPD                  (17)
#define USER32_CALLBACK_MAXIMUM               (17)

typedef struct _WINDOWPROC_CALLBACK_ARGUMENTS
{
  WNDPROC Proc;
  BOOL IsAnsiProc;
  HWND Wnd;
  UINT Msg;
  WPARAM wParam;
  LPARAM lParam;
  INT lParamBufferSize;
  LRESULT Result;
  /* char Buffer[]; */
} WINDOWPROC_CALLBACK_ARGUMENTS, *PWINDOWPROC_CALLBACK_ARGUMENTS;

typedef struct _SENDASYNCPROC_CALLBACK_ARGUMENTS
{
  SENDASYNCPROC Callback;
  HWND Wnd;
  UINT Msg;
  ULONG_PTR Context;
  LRESULT Result;
} SENDASYNCPROC_CALLBACK_ARGUMENTS, *PSENDASYNCPROC_CALLBACK_ARGUMENTS;

typedef struct _CALL_BACK_INFO
{
   SENDASYNCPROC CallBack;
   ULONG_PTR Context;
} CALL_BACK_INFO, * PCALL_BACK_INFO;


typedef struct _HOOKPROC_CALLBACK_ARGUMENTS
{
  INT HookId;
  INT Code;
  WPARAM wParam;
  LPARAM lParam;
  HOOKPROC Proc;
  INT Mod;
  ULONG_PTR offPfn;
  BOOLEAN Ansi;
  LRESULT Result;
  UINT lParamSize;
  WCHAR ModuleName[512];
} HOOKPROC_CALLBACK_ARGUMENTS, *PHOOKPROC_CALLBACK_ARGUMENTS;

typedef struct _HOOKPROC_CBT_CREATEWND_EXTRA_ARGUMENTS
{
  CREATESTRUCTW Cs; /* lpszName and lpszClass replaced by offsets */
  HWND WndInsertAfter;
  /* WCHAR szName[] */
  /* WCHAR szClass[] */
} HOOKPROC_CBT_CREATEWND_EXTRA_ARGUMENTS, *PHOOKPROC_CBT_CREATEWND_EXTRA_ARGUMENTS;

typedef struct tagCWP_Struct
{
   HOOKPROC_CALLBACK_ARGUMENTS hpca;
   CWPSTRUCT cwps;
   PBYTE Extra[4];
} CWP_Struct, *PCWP_Struct;

typedef struct tagCWPR_Struct
{
   HOOKPROC_CALLBACK_ARGUMENTS hpca;
   CWPRETSTRUCT cwprs;
   PBYTE Extra[4];
} CWPR_Struct, *PCWPR_Struct;

typedef struct _EVENTPROC_CALLBACK_ARGUMENTS
{
  HWINEVENTHOOK hook;
  DWORD event;
  HWND hwnd; 
  LONG idObject;
  LONG idChild;
  DWORD dwEventThread;
  DWORD dwmsEventTime;
  WINEVENTPROC Proc;
  INT_PTR Mod;
  ULONG_PTR offPfn;
} EVENTPROC_CALLBACK_ARGUMENTS, *PEVENTPROC_CALLBACK_ARGUMENTS;

typedef struct _LOADMENU_CALLBACK_ARGUMENTS
{
  HINSTANCE hModule;
  LPCWSTR InterSource;
  WCHAR MenuName[1];
} LOADMENU_CALLBACK_ARGUMENTS, *PLOADMENU_CALLBACK_ARGUMENTS;

typedef struct _COPYIMAGE_CALLBACK_ARGUMENTS
{
  HANDLE hImage;
  UINT uType;
  int cxDesired;
  int cyDesired;
  UINT fuFlags;
} COPYIMAGE_CALLBACK_ARGUMENTS, *PCOPYIMAGE_CALLBACK_ARGUMENTS;

typedef struct _CLIENT_LOAD_LIBRARY_ARGUMENTS
{
    UNICODE_STRING strLibraryName;
    UNICODE_STRING strInitFuncName;
    BOOL Unload;
    BOOL ApiHook;
} CLIENT_LOAD_LIBRARY_ARGUMENTS, *PCLIENT_LOAD_LIBRARY_ARGUMENTS;

typedef struct _GET_CHARSET_INFO
{
    LCID Locale;
    CHARSETINFO Cs;
} GET_CHARSET_INFO, *PGET_CHARSET_INFO;

typedef struct _SETWNDICONS_CALLBACK_ARGUMENTS
{
    HICON hIconSample;
    HICON hIconHand;
    HICON hIconQuestion;
    HICON hIconBang;
    HICON hIconNote;
    HICON hIconWindows;
    HICON hIconSmWindows;
} SETWNDICONS_CALLBACK_ARGUMENTS, *PSETWNDICONS_CALLBACK_ARGUMENTS;

typedef struct _DDEPOSTGET_CALLBACK_ARGUMENTS
{
    INT Type;
    MSG;
    int size;
    PVOID pvData;
    BYTE buffer[1];
} DDEPOSTGET_CALLBACK_ARGUMENTS, *PDDEPOSTGET_CALLBACK_ARGUMENTS;

typedef struct _SETOBM_CALLBACK_ARGUMENTS
{
    struct tagOEMBITMAPINFO oembmi[93];   
} SETOBM_CALLBACK_ARGUMENTS, *PSETOBM_CALLBACK_ARGUMENTS;

typedef struct _LPK_CALLBACK_ARGUMENTS
{
    LPWSTR lpString;
    HDC hdc;
    INT x;
    INT y;
    UINT flags;
    RECT rect;
    UINT count;
    BOOL bRect;
} LPK_CALLBACK_ARGUMENTS, *PLPK_CALLBACK_ARGUMENTS;

NTSTATUS WINAPI
User32CallCopyImageFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallSetWndIconsFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallWindowProcFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallSendAsyncProcForKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32LoadSysMenuTemplateForKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32SetupDefaultCursors(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallHookProcFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallEventProcFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallLoadMenuFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallClientThreadSetupFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallClientLoadLibraryFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallGetCharsetInfo(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32DeliverUserAPC(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallDDEPostFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallDDEGetFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallOBMFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallLPKFromKernel(PVOID Arguments, ULONG ArgumentLength);
NTSTATUS WINAPI
User32CallUMPDFromKernel(PVOID Arguments, ULONG ArgumentLength);
#endif /* __INCLUDE_USER32_CALLBACK_H */
