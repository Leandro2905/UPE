/*
 * COPYRIGHT:         BSD - See COPYING.ARM in the top level directory
 * PROJECT:           ReactOS CRT library
 * PURPOSE:           MSVC wrappers for C++ functions
 * PROGRAMMER:        Timo Kreuzer (timo.kreuzer@reactos.org)
 */

/* INCLUDES ******************************************************************/

#include <kxarm.h>

/* CODE **********************************************************************/

    TEXTAREA

    MACRO
    DEFINE_ALIAS $FuncName, $Target
        LCLS _FuncName
        LCLS _Target
_FuncName SETS "|$FuncName|"
_Target SETS "|$Target|"
        IMPORT $_FuncName, WEAK $_Target
    MEND

    DEFINE_ALIAS ??0__non_rtti_object@@QAA@ABV0@@Z, MSVCRT___non_rtti_object_copy_ctor
    DEFINE_ALIAS ??0__non_rtti_object@@QAA@PBD@Z, MSVCRT___non_rtti_object_ctor
    DEFINE_ALIAS ??0bad_cast@@AAA@PBQBD@Z, MSVCRT_bad_cast_ctor // private: __cdecl bad_cast::bad_cast(char const * const *)
    DEFINE_ALIAS ??0bad_cast@@QAA@ABV0@@Z, MSVCRT_bad_cast_copy_ctor // public: __cdecl bad_cast::bad_cast(class bad_cast const &)
    DEFINE_ALIAS ??0bad_cast@@QAA@PBD@Z, MSVCRT_bad_cast_ctor // public: __cdecl bad_cast::bad_cast(char const *)
    DEFINE_ALIAS ??0bad_typeid@@QAA@ABV0@@Z, MSVCRT_bad_typeid_copy_ctor // public: __cdecl bad_typeid::bad_typeid(class bad_typeid const &)
    DEFINE_ALIAS ??0bad_typeid@@QAA@PBD@Z, MSVCRT_bad_typeid_ctor // public: __cdecl bad_typeid::bad_typeid(char const *)
    DEFINE_ALIAS ??0exception@@QAA@ABQBD@Z, MSVCRT_exception_ctor // public: __cdecl exception::exception(char const * const &)
    DEFINE_ALIAS ??0exception@@QAA@ABQBDH@Z, MSVCRT_exception_ctor_noalloc // public: __cdecl exception::exception(char const * const &,int)
    DEFINE_ALIAS ??0exception@@QAA@ABV0@@Z, MSVCRT_exception_copy_ctor // public: __cdecl exception::exception(class exception const &)
    DEFINE_ALIAS ??0exception@@QAA@XZ, MSVCRT_exception_default_ctor // public: __cdecl exception::exception(void)
    DEFINE_ALIAS ??1__non_rtti_object@@UAA@XZ, MSVCRT___non_rtti_object_dtor // public: virtual __cdecl __non_rtti_object::~__non_rtti_object(void)
    DEFINE_ALIAS ??1bad_cast@@UAA@XZ, MSVCRT_bad_cast_dtor // public: virtual __cdecl bad_cast::~bad_cast(void)
    DEFINE_ALIAS ??1bad_typeid@@UAA@XZ, MSVCRT_bad_typeid_dtor // public: virtual __cdecl bad_typeid::~bad_typeid(void)
    DEFINE_ALIAS ??1exception@@UAA@XZ, MSVCRT_exception_dtor // public: virtual __cdecl exception::~exception(void)
    DEFINE_ALIAS ??1type_info@@UAA@XZ, MSVCRT_type_info_dtor // public: virtual __cdecl type_info::~type_info(void)
    DEFINE_ALIAS ??2@YAPAXI@Z, MSVCRT_operator_new // void * __cdecl operator new(unsigned int)
    DEFINE_ALIAS ??2@YAPAXIHPBDH@Z, MSVCRT_operator_new_dbg // void * __cdecl operator new(unsigned int,int,char const *,int)
    DEFINE_ALIAS ??3@YAXPAX@Z, MSVCRT_operator_delete // void __cdecl operator delete(void *)
    DEFINE_ALIAS ??4__non_rtti_object@@QAAAAV0@ABV0@@Z, MSVCRT___non_rtti_object_opequals // public: class __non_rtti_object & __cdecl __non_rtti_object::operator=(class __non_rtti_object const &)
    DEFINE_ALIAS ??4bad_cast@@QAAAAV0@ABV0@@Z, MSVCRT_bad_cast_opequals // public: class bad_cast & __cdecl bad_cast::operator=(class bad_cast const &)
    DEFINE_ALIAS ??4bad_typeid@@QAAAAV0@ABV0@@Z, MSVCRT_bad_typeid_opequals // public: class bad_typeid & __cdecl bad_typeid::operator=(class bad_typeid const &)
    DEFINE_ALIAS ??4exception@@QAAAAV0@ABV0@@Z, MSVCRT_exception_opequals // public: class exception & __cdecl exception::operator=(class exception const &)
    DEFINE_ALIAS ??8type_info@@QBAHABV0@@Z, MSVCRT_type_info_opequals_equals // public: int __cdecl type_info::operator==(class type_info const &)const
    DEFINE_ALIAS ??9type_info@@QBAHABV0@@Z, MSVCRT_type_info_opnot_equals // public: int __cdecl type_info::operator!=(class type_info const &)const
    DEFINE_ALIAS ??_Fbad_cast@@QAAXXZ, MSVCRT_bad_cast_default_ctor // public: void __cdecl bad_cast::`default constructor closure'(void)
    DEFINE_ALIAS ??_Fbad_typeid@@QAAXXZ, MSVCRT_bad_typeid_default_ctor // public: void __cdecl bad_typeid::`default constructor closure'(void)
    DEFINE_ALIAS ??_U@YAPAXI@Z, MSVCRT_operator_new // void * __cdecl operator new[](unsigned int)
    DEFINE_ALIAS ??_U@YAPAXIHPBDH@Z, MSVCRT_operator_new_dbg // void * __cdecl operator new[](unsigned int,int,char const *,int)
    DEFINE_ALIAS ??_V@YAXPAX@Z, MSVCRT_operator_delete // void __cdecl operator delete[](void *)
    DEFINE_ALIAS ?_query_new_handler@@YAP6AHI@ZXZ, MSVCRT__query_new_handler // int (__cdecl*__cdecl _query_new_handler(void))(unsigned int)
    DEFINE_ALIAS ?_set_new_handler@@YAP6AHI@ZP6AHI@Z@Z, MSVCRT__set_new_handler // int (__cdecl*__cdecl _set_new_handler(int (__cdecl*)(unsigned int)))(unsigned int)
    DEFINE_ALIAS ?_set_new_mode@@YAHH@Z, MSVCRT__set_new_mode // int __cdecl _set_new_mode(int)
    DEFINE_ALIAS ?_set_se_translator@@YAP6AXIPAU_EXCEPTION_POINTERS@@@ZP6AXI0@Z@Z, MSVCRT__set_se_translator // void (__cdecl*__cdecl _set_se_translator(void (__cdecl*)(unsigned int,struct _EXCEPTION_POINTERS *)))(unsigned int,struct _EXCEPTION_POINTERS *)
    DEFINE_ALIAS ?before@type_info@@QBAHABV1@@Z, MSVCRT_type_info_before // public: int __cdecl type_info::before(class type_info const &)const
    DEFINE_ALIAS ?name@type_info@@QBAPBDXZ, MSVCRT_type_info_name // public: char const * __cdecl type_info::name(void)const
    DEFINE_ALIAS ?raw_name@type_info@@QBAPBDXZ, MSVCRT_type_info_raw_name // public: char const * __cdecl type_info::raw_name(void)const
    DEFINE_ALIAS ?set_terminate@@YAP6AXXZP6AXXZ@Z, MSVCRT_set_terminate // void (__cdecl*__cdecl set_terminate(void (__cdecl*)(void)))(void)
    DEFINE_ALIAS ?set_unexpected@@YAP6AXXZP6AXXZ@Z, MSVCRT_set_unexpected // void (__cdecl*__cdecl set_unexpected(void (__cdecl*)(void)))(void)
    DEFINE_ALIAS ?terminate@@YAXXZ, MSVCRT_terminate // void __cdecl terminate(void)
    DEFINE_ALIAS ?unexpected@@YAXXZ, MSVCRT_unexpected // void __cdecl unexpected(void)
    DEFINE_ALIAS ?what@exception@@UBAPBDXZ, MSVCRT_what_exception // public: virtual char const * __cdecl exception::what(void)const

    #undef _MSVCRT_
    MACRO
    START_VTABLE $ShortName, $CxxName
        LCLS RttiName
        LCLS VtblName
        LCLS DtorName
        LCLS CxxLabel
CxxLabel    SETS "|$CxxName|"
RttiName    SETS "|$ShortName._rtti|"
VtblName    SETS "|MSVCRT_":CC:"$ShortName._vtable|"
DtorName    SETS "|MSVCRT_":CC:"$ShortName._vector_dtor|"
        EXTERN $RttiName
        DCD $RttiName
        EXPORT $VtblName
$VtblName
        EXPORT $CxxLabel
$CxxLabel
        EXTERN $DtorName
        DCD $DtorName
    MEND

    MACRO
    DEFINE_EXCEPTION_VTABLE $ShortName, $CxxName
        START_VTABLE $ShortName, $CxxName
        EXTERN MSVCRT_what_exception
        DCD MSVCRT_what_exception
    MEND

    START_VTABLE type_info, __dummyname_type_info
    DEFINE_EXCEPTION_VTABLE exception, ??_7exception@@6B@
    DEFINE_EXCEPTION_VTABLE bad_typeid, ??_7bad_typeid@@6B@
    DEFINE_EXCEPTION_VTABLE bad_cast, ??_7bad_cast@@6B@
    DEFINE_EXCEPTION_VTABLE __non_rtti_object, ??_7__non_rtti_object@@6B@

    GBLS FuncName

    //EXTERN MSVCRT_operator_delete
    //__ExportName ??3@YAXPAX@Z
    //b MSVCRT_operator_delete

    //EXTERN MSVCRT_operator_new
    //__ExportName ??_U@YAPAXI@Z
    //b MSVCRT_operator_new

    END
/* EOF */
