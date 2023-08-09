/*
 * PROJECT:     ReactOS USB UHCI Miniport Driver
 * LICENSE:     GPL-2.0+ (https://spdx.org/licenses/GPL-2.0+)
 * PURPOSE:     USBUHCI debugging declarations
 * COPYRIGHT:   Copyright 2017-2018 Vadim Galyant <vgal@rambler.ru>
 */

#ifndef DBG_UHCI_H__
#define DBG_UHCI_H__

#if DBG

    #ifndef NDEBUG_UHCI_TRACE
        #define DPRINT_UHCI(fmt, ...) do { \
            if (DbgPrint("(%s:%d) " fmt, __RELFILE__, __LINE__, ##__VA_ARGS__))  \
                DbgPrint("(%s:%d) DbgPrint() failed!\n", __RELFILE__, __LINE__); \
        } while (0)
    #else
        #if defined(_MSC_VER)
            #define DPRINT_UHCI  __noop
        #else
            #define DPRINT_UHCI(...) do {if(0) {DbgPrint(__VA_ARGS__);}} while(0)
        #endif
    #endif

    #ifndef NDEBUG_UHCI_IMPLEMENT

        #define DPRINT_IMPL(fmt, ...) do { \
            if (DbgPrint("(%s:%d) " fmt, __RELFILE__, __LINE__, ##__VA_ARGS__))  \
                DbgPrint("(%s:%d) DbgPrint() failed!\n", __RELFILE__, __LINE__); \
        } while (0)

    #else

        #if defined(_MSC_VER)
            #define DPRINT_IMPL  __noop
        #else
            #define DPRINT_IMPL(...) do { if(0) { DbgPrint(__VA_ARGS__); } } while(0)
        #endif

    #endif


#else /* not DBG */

    #if defined(_MSC_VER)
        #define DPRINT_UHCI  __noop
        #define DPRINT_IMPL  __noop
    #else
        #define DPRINT_UHCI(...) do {if(0) {DbgPrint(__VA_ARGS__);}} while(0)
        #define DPRINT_IMPL(...) do { if(0) { DbgPrint(__VA_ARGS__); } } while(0)
    #endif /* _MSC_VER */

#endif /* not DBG */

#endif /* DBG_UHCI_H__ */
