


#ifndef PIATYPES_H__
#define PIATYPES_H__

// $Header: /cvsroot/rarvcode-video/codec/rvg2dec/env/piatypes.h,v 1.5 2003/08/07 16:39:40 streeper Exp $

// This file defines the fundamental types used by the HIVE/PIA interfaces.
// These types are intended to be portable across a wide variety of
// compilation environments.
//
// The following identifiers define scalar data types having a known size
// and known range of values, regardless of the host compiler.
//
// Name     Size     Comments      Range of Values
// -------+--------+-------------+--------------------------
// U8        8 bits  unsigned                0 ..        255
// I8        8 bits  signed               -128 ..        127
// U16      16 bits  unsigned                0 ..      65535
// I16      16 bits  signed             -32768 ..      32767
// U32      32 bits  unsigned                0 .. 4294967295
// I32      32 bits  signed        -2147483648 .. 2147483647
//
// Bool8     8 bits  boolean                 0 ..   non-zero
// Bool16   16 bits  boolean                 0 ..   non-zero
// Bool32   32 bits  boolean                 0 ..   non-zero
//
// Enum32   32 bits  enumeration   -2147483648 .. 2147483647
//
// F32      32 bits  floating point
// F64      64 bits  floating point
//
//          NOTE:  floating point representations are compiler specific
//
// The following identifiers define scalar data types whose size is
// compiler specific.  They should only be used in contexts where size
// is not relevant.
//
// PIA_Boolean       boolean                 0 ..   non-zero
//
//
// The following pointers to the above types are also defined.
//
// PU8      PI8     PBool8      PEnum32
// PU16     PI16    PBool16
// PU32     PI32    PBool32
// PF32     PF64    PBoolean
//
//
// The following macros are defined to support compilers that provide
// a variety of calling conventions.  They expand to nothing (i.e., empty)
// for compilation environments where they are not needed.
//
//     PIA_CDECL
//     PIA_FASTCALL
//     PIA_STDCALL
//         These are the _WIN32 __cdecl, __fastcall and __stdcall conventions.
//
//     PIA_CALL
//
//         This is the calling convention for HIVE/PIA functions.
//         We use an explicit calling convention so that the HIVE/PIA
//         functionality could be packaged in library, and linked to by
//         a codec built with an arbitrary calling convention.
//
//     PIA_FREE_STORE_CALL
//
//         This represents the host compiler's calling convention for
//         the C++ new and delete operators.

#if defined(WINCE_EMULATOR)
#define PIA_CDECL               __cdecl
#define PIA_FASTCALL            __fastcall
#define PIA_STDCALL             __cdecl

#elif (defined(ARM) || defined(_ARM_)) && defined(_WIN32_WCE)

#define PIA_CDECL               __cdecl
#define PIA_FASTCALL            __fastcall
#define PIA_FREE_STORE_CALL
#define PIA_STDCALL

#elif defined(_WIN32) && defined(_M_IX86)

#define PIA_CDECL               __cdecl
#define PIA_FASTCALL            __fastcall
#define PIA_STDCALL             __stdcall

    // The Microsoft compiler uses the __cdecl convention for new
    // and delete.

#define PIA_FREE_STORE_CALL     __cdecl

#else // defined(_WIN32) && defined(_M_IX86)

#define PIA_CDECL
#define PIA_FASTCALL
#define PIA_STDCALL
#define PIA_FREE_STORE_CALL

#endif // defined(_WIN32) && defined(_M_IX86)


// GNU variants of STDCALL, CDECL and FASTCALL
#if defined __GNUC__ && !defined(_ARM_)

#define GNUSTDCALL __attribute__ ((stdcall))
#define GNUCDECL   __attribute__ ((cdecl))
#define GNUFASTCALL __attribute__ ((regparm (2)))

#else // defined __GNUC__

#define GNUSTDCALL
#define GNUCDECL
#define GNUFASTCALL

#endif // defined __GNUC__


#define PIA_CALL                PIA_STDCALL

#if defined(ARM) || defined(_ARM_)
    typedef unsigned char       U8;
    typedef signed char         I8;
    typedef unsigned short      U16;
    typedef signed short        I16;
    typedef unsigned long       U32;
    typedef signed long         I32;
    typedef float               F32;
    typedef double              F64;
#elif defined(_MSC_VER) && defined(_M_IX86) && defined(_WIN32)

    // Win32 definitions for Windows 95 and Windows NT on Intel
    // processors compatible with the x86 instruction set.

    typedef unsigned char       U8;
    typedef signed char         I8;
    typedef unsigned short      U16;
    typedef signed short        I16;
    typedef unsigned long       U32;
    typedef signed long         I32;
    typedef float               F32;
    typedef double              F64;

#elif defined(_M_IX86) && !defined(_WIN32)

    #error "Non-_MSC_VER or 16-bit environments are not supported at this time."

#elif defined (_OSF1) || defined(_DECALPHA)

    typedef unsigned char       U8;
    typedef signed char         I8;
    typedef unsigned short      U16;
    typedef signed short        I16;
    typedef unsigned int        U32;
    typedef int                 I32;
    typedef float               F32;
    typedef double              F64;

#else
    // These definitions should work for most other "32-bit" environments.
    // If not, an additional "#elif" section can be added above.

    typedef unsigned char       U8;
    typedef signed char         I8;
    typedef unsigned short      U16;
    typedef signed short        I16;
    typedef unsigned long       U32;
    typedef signed long         I32;
    typedef float               F32;
    typedef double              F64;

#endif


// Enumerations
//
// The size of an object declared with an enumeration type is
// compiler-specific.  The Enum32 type can be used to represent
// enumeration values when the representation is significant.

typedef I32         Enum32;


// PIA_Boolean values
//
// The "PIA_Boolean" type should be used only when an object's size is not
// significant.  Bool8, Bool16 or Bool32 should be used elsewhere.
//
// "TRUE" is defined here for assignment purposes only, for example
// "is_valid = TRUE;".  As per the definition of C and C++, any
// non-zero value is considered to be TRUE.  So "TRUE" should not be used
// in tests such as "if (is_valid == TRUE)".  Use "if (is_valid)" instead.

#undef  FALSE
#undef  TRUE

#if defined(_AIX)
    // Work around some porting problems with booleans under _AIX

typedef int         PIA_Boolean;
enum                { FALSE, TRUE };

#if !defined(_bool_is_defined)
#define _bool_is_defined

typedef int         bool;
enum                { false, true };

#endif // !defined(_bool_is_defined)

#else  // defined(_AIX)

typedef int PIA_Boolean;
enum        { FALSE, TRUE };

#endif // defined(_AIX)

typedef I8          Bool8;
typedef I16         Bool16;
typedef I32         Bool32;


//
// Define the "P*" pointer types
//

typedef U8             *PU8;
typedef I8             *PI8;
typedef U16            *PU16;
typedef I16            *PI16;
typedef U32            *PU32;
typedef I32            *PI32;
typedef F32            *PF32;
typedef F64            *PF64;
typedef Bool8          *PBool8;
typedef Bool16         *PBool16;
typedef Bool32         *PBool32;
typedef PIA_Boolean    *PBoolean;
typedef Enum32         *PEnum32;


// NULL is defined here so that you don't always have to
// include <stdio.h> or some other standard include file.

#undef  NULL
#define NULL 0


//
// Define some useful macros
//

#undef  ABS
#define ABS(a)              (((a) < 0) ? (-(a)) : (a))

#undef  MAX
#define MAX(a, b)           (((a) > (b)) ? (a) : (b))

#undef  MIN
#define MIN(a, b)           (((a) < (b)) ? (a) : (b))

#undef  SIGN
#define SIGN(a)        ((a) < 0 ? (-1) : (1))

// Perform byte swapping on a 16 or 32-byte value.

#define PIA_SWAP16(x) (U16( (((x) & 0xff) << 8) | (((x) & 0xff00) >> 8) ))

#define PIA_SWAP32(x) (U32( (((x) & 0xff)     << 24) \
                          | (((x) & 0xff00)   <<  8) \
                          | (((x) & 0xff0000) >>  8) \
                          | (((x) >> 24) & 0xff) ))

// STRINGIZE(x) will "return" its argument in double quotes.  Example:
//
//   #define  months30  April June September November
//   char *shorties = STRINGIZE(months30);
//
// results in
//
//   char *shorties = "April June September November";

#undef  STRINGIZE
#undef  STRINGIZE2

// Return "x" before macro-expanding x
#define STRINGIZE2(x) # x

// Return "x" after macro-expanding x
#define STRINGIZE(x) STRINGIZE2(x)


// XCAT(x,y) will "return" its arguments pasted together with no
// intervening white space.  Example:
//
//   I32 XCAT(tmp, __LINE__);
//
// yields a local variable with a likely unique name
//
//   I32 tmp218;

#undef  XCAT
#undef  XCAT2

// Return xy before macro-expanding x and y
#define XCAT2(x,y) x ## y

// Return xy after macro-expanding x and y
#define XCAT(x,y) XCAT2(x,y)

// DEBUG_PARAM can be used to pass parameters to functions which only
// exist when DEBUG is enabled.  For example,
//
//    void foo
//        (
//        U32 p1
//    #if defined(DEBUG)
//        , char *name
//        , I32 p3
//    #endif
//        )
//    {
//        ...
//    }
//
//    void bar(U32 u) { foo(u DEBUG_PARAM("from bar") DEBUG_PARAM(3)); }

#if defined(DEBUG)
#define DEBUG_PARAM(p) ,(p)
#else
#define DEBUG_PARAM(p)
#endif

// Duplicate symbols fix-ups
#define Hive                    RV20_Hive
#define RealVideo_Hive          RV20_RealVideo_Hive
#define RealVideo_Encoder_Hive  RV20_RealVideo_Encoder_Hive
#define RealVideo_Decoder_Hive  RV20_RealVideo_Decoder_Hive
#define PIA_Encoder             RV20_PIA_Encoder
#define PIA_Decoder             RV20_PIA_Decoder
#define PIA_Image_Format        RV20_PIA_Image_Format
#define PIA_FID_Information     RV20_PIA_FID_Information
#define PIA_Image               RV20_PIA_Image
#define PIA_TL_HDET             RV20_PIA_TL_HDET
#define PIA_TL_BLK              RV20_PIA_TL_BLK
#define PIA_TL_MBLK             RV20_PIA_TL_MBLK
#define PIA_TL_SUBPICT          RV20_PIA_TL_SUBPICT
#define PIA_TL_INIT             RV20_PIA_TL_INIT
#define PIA_TL_ALWAYS           RV20_PIA_TL_ALWAYS
#define PIA_TL_PICT             RV20_PIA_TL_PICT
#define PIA_Get_Status_Name     RV20_PIA_Get_Status_Name
#define PIA_Get_FID_Name        RV20_PIA_Get_FID_Name
#define hDriverModule           RV20_hDriverModule
//#define PIA_Status_to_HX_RESULT RV20_PIA_Status_to_HX_RESULT
#define PIA_Debug_Message       RV20_PIA_Debug_Message
#define PIA_Assert_Failed       RV20_PIA_Assert_Failed
#define CBaseBitstream          RV20_CBaseBitstream
//#define ClampTbl                RV20_ClampTbl

#define PIA_Allocate            RV20_PIA_Allocate
#define PIA_Free                RV20_PIA_Free

#define pia_fid_info            RV20_pia_fid_info

#endif // PIATYPES_H__
