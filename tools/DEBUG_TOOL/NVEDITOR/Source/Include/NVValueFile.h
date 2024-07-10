#ifndef _NVVALUEFILE_H
#define _NVVALUEFILE_H

#include "NVItemDef.h"

#define NVVALUE_FILE_SIGNATURE        	0x4E56    // NV
#define MAX_MACHINE_NAME_LEN	     	128
 
// An NV value file is a file that dictates automatically loading of NV values from this file
// to a specified NV project.
// The NV value file has following parts,

// NVVALUE_FILE_HEADER
// NVVALUE_SECTION_ENTRY table with NumberOfSecions entries
// NVVALUE_TABLE_ENTRY and specific searching directories for every section 

#define VERSION_1 0x0100
#define VERSION_2 0x0200
  
typedef struct _NVVALUE_FILE_HEADER {
    WORD    Magic;                          // = NVVALUE_FILE_SIGNATURE
    WORD    Version;                        // = 0x0100 or 0x0200
    DWORD   TimeDateStamp;                  // Optional, but must be intialized to 0 if not used
    char    Machine[MAX_MACHINE_NAME_LEN];  // Optional, but must be intialized to empty string if not used
    DWORD   NumberOfSections;
    DWORD   NVVersion;                      // The version of NV items in the file, not used now
} NVVALUE_FILE_HEADER, *PNVVALUE_FILE_HEADER;

#define NVVALUE_FILE_HEADER_SIZE 		( sizeof NVVALUE_FILE_HEADER )

// NVVALUE_SECTION_ENTRY table just follows NVVALUE_FILE_HEADER, which specifies the position and
// number of its value table and searching directories.


// For version 0x0100,we use OffsetOfValueTable and OffsetOfDirectoryTable,
// For version 0x0200,we use LongOffsetOfValueTable and LongOffsetOfDirectoryTable.
typedef struct _NVVALUE_SECTION_ENTRY {
    WORD    OffsetOfValueTable;         // Offset of value table from the beginning of the file
    WORD    NumberOfValueEntries;
    WORD    OffsetOfDirectoryTable;     // Offset of Dir table from the beginning of the file
    WORD    NumberOfDirectories;         
//    DWORD   Reserved[2];
    // Because the file size maybe exceed 64k,so we must use DWORD for
    // offsets of value table and directory table.We remail the 
    // original definition of these offsets for backward conpatibility
    DWORD   LongOffsetOfValueTable;     // Offset of value table from the beginning of the file
    DWORD   LongOffsetOfDirectoryTable; // Offset of Dir table from the beginning of the file
} NVVALUE_SECTION_ENTRY, *PNVVALUE_SECTION_ENTRY;

#define NVVALUE_SECTION_ENTRY_SIZE      ( sizeof NVVALUE_SECTION_ENTRY )

// Directory Table is a list of NUL terminated ASCII string, which
// specifies the starting point to search for items. The directory must start
// from root and have following format, 
// "DownloadParam\\system_param_dsp_use_T\\audio_PGA_gain_struct" in which backslash
// is used to seperate differetn parts of the directory. No leading or tailing backslash
// is required.
// Value table is a list of VALUE_TABLE_ENTRY, which specifies the value and name
// of nv items whose value are to be set. The legitimate values of Type field are
// CHAR_ITEM_TYPE, SHORT_ITEM_TYPE, LONG_ITEM_TYPE as defined in ITEMTYPE.
// Although dir table and value table can be got by offsets respectively,
// it is preferable to put value table before dir table for sake of alignment.

typedef struct _NVVALUE_TABLE_ENTRY {
    WORD    Type;                       // ITEMTYPE defined in FileParser.h
    WORD    Reserved;
    DWORD   Value;
    char    Name[MAX_NAME_LENGTH];
} NVVALUE_TABLE_ENTRY, *PNVVALUE_TABLE_ENTRY;
    
#define NVVALUE_TABLE_ENTRY_SIZE        ( sizeof NVVALUE_TABLE_ENTRY )

#define SECTION_TABLE( pBase ) \
	( (PNVVALUE_SECTION_ENTRY)( (LPBYTE)(pBase) + NVVALUE_FILE_HEADER_SIZE ) )

#define NVVALUE_NUM( pSection )		( (pSection)->NumberOfValueEntries )

#define NVVALUE_TABLE( pBase, pSection ) \
	( (PNVVALUE_TABLE_ENTRY)( (LPBYTE)(pBase) + (pSection)->OffsetOfValueTable ) )

#define NVVALUE_TABLE_EX( pBase, pSection ) \
( (PNVVALUE_TABLE_ENTRY)( (LPBYTE)(pBase) + (pSection)->LongOffsetOfValueTable ) )

#define NVDIR_NUM( pSection )		( (pSection)->NumberOfDirectories )

#define NVDIR_TABLE( pBase, pSection ) \
	( (char*)( (LPBYTE)(pBase) + (pSection)->OffsetOfDirectoryTable ) )

#define NVDIR_TABLE_EX( pBase, pSection ) \
( (char*)( (LPBYTE)(pBase) + (pSection)->LongOffsetOfDirectoryTable ) )

#endif // _NVVALUEFILE_H