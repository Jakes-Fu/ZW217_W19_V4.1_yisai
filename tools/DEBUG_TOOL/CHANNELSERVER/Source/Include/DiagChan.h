#ifndef DIAGCHAN_H
#define DIAGCHAN_H
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DIAGCHAN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DIAGCHAN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DIAGCHAN_EXPORTS
#define DIAGCHAN_API extern "C" __declspec(dllexport)
#else
#define DIAGCHAN_API extern "C" __declspec(dllimport)
#endif

#include "ProtoChan.h"

#define SPECIAL_SN 0xFFFFFFFF

//Export type definitions
#define DIAG_HDR_LEN     8
struct DIAG_HEADER
{
	unsigned int sn;			// Sequence number
    unsigned short len;			// Package length
	unsigned char type;
	unsigned char subtype;
};

struct DIAG_PACKAGE
{
	DIAG_HEADER header;
	void* data;					// Package data
};

// Bootmode package
struct BM_HEADER
{
	unsigned short type;
	unsigned short len;
};
struct BM_PACKAGE
{
	BM_HEADER header;
	void* data;
};

// SMP package
#define SMP_START_FLAG  0x7E7E7E7E
#define SMP_START_LEN   4
#define SMP_HDR_LEN     8
#define SMP_RESERVED    0x5A5A
struct SMP_HEADER
{
	unsigned short len;
    unsigned char  lcn;
	unsigned char  packet_type;
	unsigned short reserved;    //0x5A5A
	unsigned short check_sum;
};

struct SMP_PACKAGE
{
	SMP_HEADER header;
	void*      data;
};

enum
{
	DP_OK,
	DP_INVALID_PARAMETER,
	DP_NO_ENUOUPH_MEMORY,
	DP_INVALID_ENDIAN,
};

enum
{
	PPI_DIAG_Type = PPI_USER_BASE,
	PPI_BM_CRC_Type,
	PPI_DIAG_MAX_SN
};

typedef enum
{
	PT_DIAG,
	PT_BM,
	PT_SMP	
}E_PROTO_TYPE;

#define DIAG_TYPE_NORMAL	0
#define DIAG_TYPE_NO_HEADER 1
#define DIAG_TYPE_NO_ESCAPE 2

// Export functions
class IProtocolChannel;
DIAGCHAN_API bool CreateDiagChannel( IProtocolChannel** );
DIAGCHAN_API void ReleaseDiagChannel( IProtocolChannel* );

class IProtocolPackage;
DIAGCHAN_API bool CreateDiagPackage( IProtocolPackage** );
DIAGCHAN_API void ReleaseDiagPackage( IProtocolPackage* );

DIAGCHAN_API bool CreateBmChannel( IProtocolChannel** );
DIAGCHAN_API void ReleaseBmChannel( IProtocolChannel* );

DIAGCHAN_API bool CreateBmPackage( IProtocolPackage** );
DIAGCHAN_API void ReleaseBmPackage( IProtocolPackage* );

DIAGCHAN_API bool CreateProtoChannel( IProtocolChannel**, E_PROTO_TYPE);
DIAGCHAN_API void ReleaseProtoChannel( IProtocolChannel* );
DIAGCHAN_API bool CreateProtoPackage( IProtocolPackage** , E_PROTO_TYPE);
DIAGCHAN_API void ReleaseProtoPackage( IProtocolPackage* );

#endif