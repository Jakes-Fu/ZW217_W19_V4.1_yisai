/******************************************************************************
 ** File Name:      hash.h                                           *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                            *
 ******************************************************************************/
#ifndef _HASH_H_
#define _HASH_H_

#include "sci_types.h"

/******************************************************************************
 * Structure definitions
 ******************************************************************************/
typedef struct _BUCKET_ENTRY
{
    char *Name;
    void *Arg;
    struct _BUCKET_ENTRY *Next;
} BUCKET_ENTRY;

typedef struct _HASH_TABLE
{
    BUCKET_ENTRY **Slots;
    int      Size;
} HASH_TABLE;

/******************************************************************************
 * Prototype functions
 ******************************************************************************/
HASH_TABLE *HASH_TABLE_Create (int Size);
void HASH_TABLE_Destory (HASH_TABLE *pTable);

/* Arg should not be NULL, for ST_Delete & ST_Lookup will return NULL if
 * the specified Name is not in the symbol table.
 */
BOOLEAN HASH_TABLE_Insert (HASH_TABLE *pTable, const char *Name, void *Arg);
void *HASH_TABLE_Delete (HASH_TABLE *pTable, const char *Name);
void *HASH_TABLE_Lookup (HASH_TABLE *pTable, const char *Name);

#endif /* HASH_H */
