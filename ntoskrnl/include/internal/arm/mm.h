/*
 * kernel internal memory management definitions for arm
 */
#pragma once

#define _MI_PAGING_LEVELS 2
#define _MI_HAS_NO_EXECUTE 1

/* Memory layout base addresses */
#define MI_USER_PROBE_ADDRESS                   (PVOID)0x7FFF0000
#define MI_DEFAULT_SYSTEM_RANGE_START           (PVOID)0x80000000
#define HYPER_SPACE                                    0xC0500000
#define HYPER_SPACE_END                                0xC08FFFFF
#define MI_SYSTEM_CACHE_WS_START                (PVOID)0xC0C00000
#define MI_PAGED_POOL_START                     (PVOID)0xE1000000
#define MI_NONPAGED_POOL_END                    (PVOID)0xFFBE0000
#define MI_DEBUG_MAPPING                        (PVOID)0xFFBFF000
#define MI_HIGHEST_SYSTEM_ADDRESS               (PVOID)0xFFFFFFFF

#define PTE_PER_PAGE 256
#define PDE_PER_PAGE 4096
#define PPE_PER_PAGE 1

/* Misc address definitions */
#define MI_SYSTEM_PTE_BASE                      (PVOID)MiAddressToPte(NULL)
#define MM_HIGHEST_VAD_ADDRESS \
    (PVOID)((ULONG_PTR)MM_HIGHEST_USER_ADDRESS - (16 * PAGE_SIZE))
#define MI_MAPPING_RANGE_START              ((ULONG)HYPER_SPACE)
#define MI_MAPPING_RANGE_END                (MI_MAPPING_RANGE_START + \
                                                  MI_HYPERSPACE_PTES * PAGE_SIZE)
#define MI_DUMMY_PTE                        (PMMPTE)(MI_MAPPING_RANGE_END + \
                                                  PAGE_SIZE)
#define MI_VAD_BITMAP                       (PMMPTE)(MI_DUMMY_PTE + \
                                                  PAGE_SIZE)
#define MI_WORKING_SET_LIST                 (PMMPTE)(MI_VAD_BITMAP + \
                                                  PAGE_SIZE)

/* Memory sizes */
#define MI_MIN_PAGES_FOR_NONPAGED_POOL_TUNING   ((255 * _1MB) >> PAGE_SHIFT)
#define MI_MIN_PAGES_FOR_SYSPTE_TUNING          ((19 * _1MB) >> PAGE_SHIFT)
#define MI_MIN_PAGES_FOR_SYSPTE_BOOST           ((32 * _1MB) >> PAGE_SHIFT)
#define MI_MIN_PAGES_FOR_SYSPTE_BOOST_BOOST     ((256 * _1MB) >> PAGE_SHIFT)
#define MI_MIN_INIT_PAGED_POOLSIZE              (32 * _1MB)
#define MI_MAX_INIT_NONPAGED_POOL_SIZE          (128 * _1MB)
#define MI_MAX_NONPAGED_POOL_SIZE               (128 * _1MB)
#define MI_SYSTEM_VIEW_SIZE                     (32 * _1MB)
#define MI_SESSION_VIEW_SIZE                    (48 * _1MB)
#define MI_SESSION_POOL_SIZE                    (16 * _1MB)
#define MI_SESSION_IMAGE_SIZE                   (8 * _1MB)
#define MI_SESSION_WORKING_SET_SIZE             (4 * _1MB)
#define MI_SESSION_SIZE                         (MI_SESSION_VIEW_SIZE + \
                                                 MI_SESSION_POOL_SIZE + \
                                                 MI_SESSION_IMAGE_SIZE + \
                                                 MI_SESSION_WORKING_SET_SIZE)
#define MI_MIN_ALLOCATION_FRAGMENT              (4 * _1KB)
#define MI_ALLOCATION_FRAGMENT                  (64 * _1KB)
#define MI_MAX_ALLOCATION_FRAGMENT              (2  * _1MB)

/* Misc constants */
#define MM_PTE_SOFTWARE_PROTECTION_BITS         6
#define MI_MIN_SECONDARY_COLORS                 8
#define MI_SECONDARY_COLORS                     64
#define MI_MAX_SECONDARY_COLORS                 1024
#define MI_MAX_FREE_PAGE_LISTS                  4
#define MI_HYPERSPACE_PTES                     (256 - 1) /* Dee PDR definition */
#define MI_ZERO_PTES                           (32) /* Dee PDR definition */
#define MI_MAX_ZERO_BITS                        21
#define SESSION_POOL_LOOKASIDES                 26 // CHECKME

/* MMPTE related defines */
#define MM_EMPTY_PTE_LIST  ((ULONG)0xFFFFF)
#define MM_EMPTY_LIST  ((ULONG_PTR)-1)


/* Easy accessing PFN in PTE */
#define PFN_FROM_PTE(v) ((v)->u.Hard.PageFrameNumber)

/* Macros for portable PTE modification */
#define MI_MAKE_DIRTY_PAGE(x)
#define MI_MAKE_CLEAN_PAGE(x)
#define MI_MAKE_ACCESSED_PAGE(x)
#define MI_PAGE_DISABLE_CACHE(x)   ((x)->u.Hard.Cached = 0)
#define MI_PAGE_WRITE_THROUGH(x)   ((x)->u.Hard.Buffered = 0)
#define MI_PAGE_WRITE_COMBINED(x)  ((x)->u.Hard.Buffered = 1)
#define MI_IS_PAGE_LARGE(x)        FALSE
#define MI_IS_PAGE_WRITEABLE(x)    ((x)->u.Hard.ReadOnly == 0)
#define MI_IS_PAGE_COPY_ON_WRITE(x)FALSE
#define MI_IS_PAGE_EXECUTABLE(x)   TRUE
#define MI_IS_PAGE_DIRTY(x)        TRUE
#define MI_MAKE_OWNER_PAGE(x)      ((x)->u.Hard.Owner = 1)
#define MI_MAKE_WRITE_PAGE(x)      ((x)->u.Hard.ReadOnly = 0)

/* Macros to identify the page fault reason from the error code */
#define MI_IS_NOT_PRESENT_FAULT(FaultCode) TRUE
#define MI_IS_WRITE_ACCESS(FaultCode) TRUE
#define MI_IS_INSTRUCTION_FETCH(FaultCode) FALSE

/* Convert an address to a corresponding PTE */
#define MiAddressToPte(x) \
    ((PMMPTE)(PTE_BASE + (((ULONG)(x) >> 12) << 2)))

/* Convert an address to a corresponding PDE */
#define MiAddressToPde(x) \
    ((PMMPDE)(PDE_BASE + (((ULONG)(x) >> 20) << 2)))

/* Convert an address to a corresponding PTE offset/index */
#define MiAddressToPteOffset(x) \
    ((((ULONG)(x)) << 12) >> 24)

/* Convert an address to a corresponding PDE offset/index */
#define MiAddressToPdeOffset(x) \
    (((ULONG)(x)) >> 20)
#define MiGetPdeOffset MiAddressToPdeOffset

/* Convert a PTE/PDE into a corresponding address */
#define MiPteToAddress(_Pte) ((PVOID)((ULONG)(_Pte) << 10))
#define MiPdeToAddress(_Pde) ((PVOID)((ULONG)(_Pde) << 18))

/* Translate between P*Es */
#define MiPdeToPte(_Pde)   ((PMMPTE)0) /* FIXME */
#define MiPteToPde(_Pte)   ((PMMPDE)0) /* FIXME */

/* Check P*E boundaries */
#define MiIsPteOnPdeBoundary(PointerPte) \
    ((((ULONG_PTR)PointerPte) & (PAGE_SIZE - 1)) == 0)

//
// Decodes a Prototype PTE into the underlying PTE
//
#define MiProtoPteToPte(x)                  \
    (PMMPTE)((ULONG_PTR)MmPagedPoolStart +  \
             (((x)->u.Proto.ProtoAddressHigh << 9) | (x)->u.Proto.ProtoAddressLow << 2))

//
// Decodes a Prototype PTE into the underlying PTE
//
#define MiSubsectionPteToSubsection(x)                              \
    ((x)->u.Subsect.WhichPool == PagedPool) ?                       \
        (PMMPTE)((ULONG_PTR)MmSubsectionBase +                      \
                 (((x)->u.Subsect.SubsectionAddressHigh << 7) |     \
                   (x)->u.Subsect.SubsectionAddressLow << 3)) :     \
        (PMMPTE)((ULONG_PTR)MmNonPagedPoolEnd -                     \
                (((x)->u.Subsect.SubsectionAddressHigh << 7) |      \
                  (x)->u.Subsect.SubsectionAddressLow << 3))

//
// Number of bits corresponding to the area that a coarse page table occupies (1KB)
//
#define CPT_SHIFT 10

/* See PDR definition */
#define MI_ZERO_PTE                         (PMMPTE)(MI_MAPPING_RANGE_END + \
                                             PAGE_SIZE)

