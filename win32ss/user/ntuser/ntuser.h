#pragma once

#define DECLARE_RETURN(type) type _ret_
#define RETURN(value) { _ret_ = value; goto _cleanup_; }
#define CLEANUP /*unreachable*/ ASSERT(FALSE); _cleanup_
#define END_CLEANUP return _ret_;


#define UserEnterCo UserEnterExclusive
#define UserLeaveCo UserLeave

extern PSERVERINFO gpsi;
extern PTHREADINFO gptiCurrent;
extern PPROCESSINFO gppiList;
extern PPROCESSINFO ppiScrnSaver;
extern PPROCESSINFO gppiInputProvider;
extern BOOL g_AlwaysDisplayVersion;
extern ATOM gaGuiConsoleWndClass;
extern ATOM AtomDDETrack;
extern ATOM AtomQOS;

INIT_FUNCTION NTSTATUS NTAPI InitUserImpl(VOID);
VOID FASTCALL CleanupUserImpl(VOID);
VOID FASTCALL UserEnterShared(VOID);
VOID FASTCALL UserEnterExclusive(VOID);
VOID FASTCALL UserLeave(VOID);
BOOL FASTCALL UserIsEntered(VOID);
BOOL FASTCALL UserIsEnteredExclusive(VOID);
DWORD FASTCALL UserGetLanguageToggle(VOID);

_Success_(return != FALSE)
BOOL
NTAPI
RegReadUserSetting(
    _In_z_ PCWSTR pwszKeyName,
    _In_z_ PCWSTR pwszValueName,
    _In_ ULONG ulType,
    _Out_writes_bytes_(cjDataSize) _When_(ulType == REG_SZ, _Post_z_) PVOID pvData,
    _In_ ULONG cjDataSize);

_Success_(return != FALSE)
BOOL
NTAPI
RegWriteUserSetting(
    _In_z_ PCWSTR pwszKeyName,
    _In_z_ PCWSTR pwszValueName,
    _In_ ULONG ulType,
    _In_reads_bytes_(cjDataSize) const VOID *pvData,
    _In_ ULONG cjDataSize);

/* EOF */
