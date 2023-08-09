#define STANDALONE
#include <apitest.h>

extern void func_atltypes(void);
extern void func_CAtlFileMapping(void);
extern void func_CAtlArray(void);
extern void func_CAtlList(void);
extern void func_CComBSTR(void);
extern void func_CComHeapPtr(void);
extern void func_CComObject(void);
extern void func_CComQIPtr(void);
extern void func_CComVariant(void);
extern void func_CHeapPtrList(void);
extern void func_CImage(void);
extern void func_CRegKey(void);
extern void func_CSimpleArray(void);
extern void func_CSimpleMap(void);
extern void func_CString(void);

const struct test winetest_testlist[] =
{
    { "atltypes", func_atltypes },
    { "CAtlFileMapping", func_CAtlFileMapping },
    { "CAtlArray", func_CAtlArray },
    { "CAtlList", func_CAtlList },
    { "CComBSTR", func_CComBSTR },
    { "CComHeapPtr", func_CComHeapPtr },
    { "CComObject", func_CComObject },
    { "CComQIPtr", func_CComQIPtr },
    { "CComVariant", func_CComVariant },
    { "CHeapPtrList", func_CHeapPtrList },
    { "CImage", func_CImage },
    { "CRegKey", func_CRegKey },
    { "CSimpleArray", func_CSimpleArray },
    { "CSimpleMap", func_CSimpleMap },
    { "CString", func_CString },
    { 0, 0 }
};
