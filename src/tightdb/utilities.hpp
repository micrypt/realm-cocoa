/*************************************************************************
 *
 * TIGHTDB CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2012] TightDB Inc
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of TightDB Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to TightDB Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from TightDB Incorporated.
 *
 **************************************************************************/
#ifndef TIGHTDB_UTILITIES_HPP
#define TIGHTDB_UTILITIES_HPP

#include <stdint.h>
#include <cstdlib>
#include <cstdlib> // size_t

#ifdef _MSC_VER
#  include <win32/types.h>
#  include <intrin.h>
#endif

#include <tightdb/assert.hpp>

#if defined(__GNUC__)
    #define TIGHTDB_FORCEINLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
    #define TIGHTDB_FORCEINLINE __forceinline
#elif defined(__HP_aCC)
    #define TIGHTDB_FORCEINLINE inline __attribute__((always_inline))
#elif defined(__xlC__ )
    #define TIGHTDB_FORCEINLINE inline
#else
    #error TEXT("Compiler version not detectable")
#endif

/* GCC defines __i386__ and __x86_64__ */
#if (defined(__X86__) || defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(__386__) || defined(__x86_64__) || defined(_M_X64))
    #define TIGHTDB_X86_OR_X64
#endif

/* GCC defines __arm__ */
#ifdef __arm__
#  define TIGHTDB_ARCH_ARM
#endif

#if defined _LP64 || defined __LP64__ || defined __64BIT__ || _ADDR64 || defined _WIN64 || defined __arch64__ || __WORDSIZE == 64 || (defined __sparc && defined __sparcv9) || defined __x86_64 || defined __amd64 || defined __x86_64__ || defined _M_X64 || defined _M_IA64 || defined __ia64 || defined __IA64__
    #define TIGHTDB_PTR_64
#endif


// On platforms with cache coherence this macro should not do anything
// on all other platforms it must implement a sync or memory barrier
// FIXME: It is common to define at least two diffrent levels of cache coherency. Which one do we require? Which ones are guaranteed by Intel and ARM? See http://en.wikipedia.org/wiki/Cache_coherence.
#if defined(TIGHTDB_X86_OR_X64)
#  define TIGHTDB_SYNC_IF_NO_CACHE_COHERENCE
#elif defined (TIGHTDB_ARCH_ARM)
#  define TIGHTDB_SYNC_IF_NO_CACHE_COHERENCE
#endif

#if defined(TIGHTDB_PTR_64) && defined(TIGHTDB_X86_OR_X64)
    #define TIGHTDB_COMPILER_SSE  // Compiler supports SSE 4.2 through __builtin_ accessors or back-end assembler
#endif

namespace tightdb {

extern signed char sse_support;

template<int version> TIGHTDB_FORCEINLINE bool cpuid_sse()
{
/*
    Return wether or not SSE 3.0 (if version = 30) or 4.2 (for version = 42) is supported. Return value
    is based on the CPUID instruction.

    sse_support = -1: No SSE support
    sse_support = 0: SSE3
    sse_support = 1: SSE42

    This lets us test very rapidly at runtime because we just need 1 compare instruction (with 0) to test both for
    3 and 4.2 by caller (compiler optimizes if calls are concecutive), and can decide branch with ja/jl/je because
    sse_support is signed type. Also, 0 requires no immediate operand

    We runtime-initialize sse_support in a constructor of a static variable which is not guaranteed to be called
    prior to cpu_sse(). So we compile-time initialize sse_support to -2 as fallback.
*/
    TIGHTDB_STATIC_ASSERT(version == 30 || version == 42, "Only SSE 3 and 42 supported for detection");
#ifdef TIGHTDB_COMPILER_SSE
    if (version == 30)
        return (sse_support >= 0);
    else if (version == 42)
        return (sse_support > 0);   // faster than == 1 (0 requres no immediate operand)
#else
    return false;
#endif
}

typedef struct {
    unsigned long long remainder;
    unsigned long long remainder_len;
    unsigned long long b_val;
    unsigned long long a_val;
    unsigned long long result;
} checksum_t;

std::size_t to_ref(int64_t) TIGHTDB_NOEXCEPT;
std::size_t to_size_t(int64_t) TIGHTDB_NOEXCEPT;
void cpuid_init();
unsigned long long checksum(unsigned char* data, size_t len);
void checksum_rolling(unsigned char* data, size_t len, checksum_t* t);
void* round_up(void* p, size_t align);
void* round_down(void* p, size_t align);
size_t round_up(size_t p, size_t align);
size_t round_down(size_t p, size_t align);
void checksum_init(checksum_t* t);

// popcount
int fast_popcount32(int32_t x);
int fast_popcount64(int64_t x);



// Implementation:

inline std::size_t to_ref(int64_t v) TIGHTDB_NOEXCEPT
{
#ifdef TIGHTDB_DEBUG
    uint64_t m = std::size_t(-1);
    TIGHTDB_ASSERT(uint64_t(v) <= m);
    // FIXME: Must also check that v is divisible by 8 (64-bit aligned).
#endif
    return std::size_t(v);
}

// Safe cast from 64 to 32 bits on 32 bit architecture. Differs from to_ref() by not testing alignment and REF-bitflag.
inline std::size_t to_size_t(int64_t v) TIGHTDB_NOEXCEPT
{
#ifdef TIGHTDB_DEBUG
    uint64_t m = std::size_t(-1);
    TIGHTDB_ASSERT(uint64_t(v) <= m);
    // FIXME: Should probably be TIGHTDB_ASSERT(0 <= v && uint64_t(v) <= numeric_limits<size_t>::max());
#endif
    return std::size_t(v);
}


template<typename ReturnType, typename OriginalType>
ReturnType TypePunning( OriginalType variable )
{
    union {
        OriginalType    in;
        ReturnType      out;
    };
    in = variable;
    return out;
}

} // namespace tightdb

#endif // TIGHTDB_UTILITIES_HPP
