#define __ROS_LONG64__

#define STANDALONE
#include <apitest.h>

extern void func_AttachThreadInput(void);
extern void func_CloseWindow(void);
extern void func_CreateDialog(void);
extern void func_CreateIconFromResourceEx(void);
extern void func_CreateWindowEx(void);
extern void func_DeferWindowPos(void);
extern void func_DestroyCursorIcon(void);
extern void func_DM_REPOSITION(void);
extern void func_DrawIconEx(void);
extern void func_DrawText(void);
extern void func_desktop(void);
extern void func_EmptyClipboard(void);
extern void func_EnumDisplaySettings(void);
extern void func_GetDCEx(void);
extern void func_GetIconInfo(void);
extern void func_GetKeyState(void);
extern void func_GetMessageTime(void);
extern void func_GetPeekMessage(void);
extern void func_GetSystemMetrics(void);
extern void func_GetUserObjectInformation(void);
extern void func_GetWindowPlacement(void);
extern void func_InitializeLpkHooks(void);
extern void func_LoadImage(void);
extern void func_LookupIconIdFromDirectoryEx(void);
extern void func_MessageStateAnalyzer(void);
extern void func_NextDlgItem(void);
extern void func_PrivateExtractIcons(void);
extern void func_RealGetWindowClass(void);
extern void func_RedrawWindow(void);
extern void func_RegisterHotKey(void);
extern void func_RegisterClassEx(void);
extern void func_ScrollBarWndExtra(void);
extern void func_ScrollDC(void);
extern void func_ScrollWindowEx(void);
extern void func_SendMessageTimeout(void);
extern void func_SetActiveWindow(void);
extern void func_SetCursorPos(void);
extern void func_SetParent(void);
extern void func_SetProp(void);
extern void func_SetScrollInfo(void);
extern void func_SetScrollRange(void);
extern void func_ShowWindow(void);
extern void func_SwitchToThisWindow(void);
extern void func_SystemParametersInfo(void);
extern void func_TrackMouseEvent(void);
extern void func_WndProc(void);
extern void func_wsprintf(void);

const struct test winetest_testlist[] =
{
    { "AttachThreadInput", func_AttachThreadInput },
    { "CloseWindow", func_CloseWindow },
    { "CreateDialog", func_CreateDialog },
    { "CreateIconFromResourceEx", func_CreateIconFromResourceEx },
    { "CreateWindowEx", func_CreateWindowEx },
    { "DeferWindowPos", func_DeferWindowPos },
    { "DestroyCursorIcon", func_DestroyCursorIcon },
    { "DM_REPOSITION", func_DM_REPOSITION },
    { "DrawIconEx", func_DrawIconEx },
    { "DrawText", func_DrawText },
    { "desktop", func_desktop },
    { "EmptyClipboard", func_EmptyClipboard },
    { "EnumDisplaySettings", func_EnumDisplaySettings },
    { "GetDCEx", func_GetDCEx },
    { "GetIconInfo", func_GetIconInfo },
    { "GetKeyState", func_GetKeyState },
    { "GetMessageTime", func_GetMessageTime },
    { "GetPeekMessage", func_GetPeekMessage },
    { "GetSystemMetrics", func_GetSystemMetrics },
    { "GetUserObjectInformation", func_GetUserObjectInformation },
    { "GetWindowPlacement", func_GetWindowPlacement },
    { "InitializeLpkHooks", func_InitializeLpkHooks },
    { "LoadImage", func_LoadImage },
    { "LookupIconIdFromDirectoryEx", func_LookupIconIdFromDirectoryEx },
    { "MessageStateAnalyzer", func_MessageStateAnalyzer },
    { "NextDlgItem", func_NextDlgItem },
    { "PrivateExtractIcons", func_PrivateExtractIcons },
    { "RealGetWindowClass", func_RealGetWindowClass },
    { "RedrawWindow", func_RedrawWindow },
    { "RegisterHotKey", func_RegisterHotKey },
    { "RegisterClassEx", func_RegisterClassEx },
    { "ScrollBarWndExtra", func_ScrollBarWndExtra },
    { "ScrollDC", func_ScrollDC },
    { "ScrollWindowEx", func_ScrollWindowEx },
    { "SendMessageTimeout", func_SendMessageTimeout },
    { "SetActiveWindow", func_SetActiveWindow },
    { "SetCursorPos", func_SetCursorPos },
    { "SetParent", func_SetParent },
    { "SetProp", func_SetProp },
    { "SetScrollInfo", func_SetScrollInfo },
    { "SetScrollRange", func_SetScrollRange },
    { "ShowWindow", func_ShowWindow },
    { "SwitchToThisWindow", func_SwitchToThisWindow },
    { "SystemParametersInfo", func_SystemParametersInfo },
    { "TrackMouseEvent", func_TrackMouseEvent },
    { "WndProc", func_WndProc },
    { "wsprintfApi", func_wsprintf },
    { 0, 0 }
};

