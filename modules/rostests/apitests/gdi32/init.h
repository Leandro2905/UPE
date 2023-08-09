#pragma once

extern HBITMAP ghbmp1, ghbmp4, ghbmp8, ghbmp16, ghbmp24, ghbmp32;
extern HBITMAP ghbmpDIB1, ghbmpDIB4, ghbmpDIB8, ghbmpDIB16, ghbmpDIB24, ghbmpDIB32;
extern HDC ghdcDIB1, ghdcDIB4, ghdcDIB8, ghdcDIB16, ghdcDIB24, ghdcDIB32;
extern PVOID gpvDIB1, gpvDIB4, gpvDIB8, gpvDIB16, gpvDIB24, gpvDIB32;

extern HBITMAP ghbmpDIB32;
//extern PULONG pulDIB32Bits;
extern PULONG pulDIB4Bits;
extern HPALETTE ghpal;
typedef struct
{
    WORD palVersion;
    WORD palNumEntries;
    PALETTEENTRY logpalettedata[8];
} MYPAL;

extern ULONG (*gpDIB32)[8][8];

extern MYPAL gpal;

BOOL InitStuff(void);

