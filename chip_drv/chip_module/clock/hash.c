/******************************************************************************
 ** File Name:      hash.c                                           *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                            *
 ******************************************************************************

 ******************************************************************************
 **                 Edit History                                            *
 ** ------------------------------------------------------------------------*
 ** DATE            NAME             DESCRIPTION                            *
 ** 02/21/2010      Yi.Qiu          Create.                                 *
 ******************************************************************************/
#include <string.h>
#include "os_api.h"
#include "hash.h"

#ifdef _HASH_DEBUG
#define HASH_ASSERT(_x_)        SCI_ASSERT(_x_)
#else
#define HASH_ASSERT(_x_)        ((void)0)
#endif /* _HASH_DEBUG */

#define SHIFT           4

LOCAL int Hash (HASH_TABLE *pTable, const char *Name)
{
    int temp = 0;

    while ('\0' != *Name)
    {
        temp = ( (temp << SHIFT) + *Name) % pTable->Size;
        ++Name;
    }

    return temp;
}

HASH_TABLE *HASH_TABLE_Create (int Size)
{
    BUCKET_ENTRY    **p = SCI_NULL;
    HASH_TABLE      *pTable = SCI_NULL;

    SCI_ASSERT (Size > 1);/*assert verified*/

    p = (BUCKET_ENTRY **) SCI_ALLOC_APP ( (uint32) Size * sizeof (BUCKET_ENTRY *));

    if (SCI_NULL == p)
    {
        return SCI_NULL;
    }

    SCI_MEMSET (p, 0, (uint32) Size * sizeof (BUCKET_ENTRY *));/*lint !e516 -e516*/

    if (SCI_NULL == (pTable = (HASH_TABLE *) SCI_ALLOC_APP (sizeof (HASH_TABLE))))
    {
        SCI_FREE (p);
        return SCI_NULL;
    }

    pTable->Slots = p;
    pTable->Size = Size;

    return pTable;
}

void HASH_TABLE_Destory (HASH_TABLE *pTable)
{
    int i;

    SCI_ASSERT (SCI_NULL != pTable);/*assert verified*/
    SCI_ASSERT (SCI_NULL != pTable->Slots);/*assert verified*/

    for (i=0; i<pTable->Size; i++) /*lint !e413 comfirmed by xuepeng*/
    {
        BUCKET_ENTRY *p = pTable->Slots[i];

        while (p)
        {
            BUCKET_ENTRY *temp = p->Next;
            SCI_FREE (p->Name);
            SCI_FREE (p);
            p = temp;
        }

        SCI_FREE (pTable->Slots);
        SCI_FREE (pTable);
    }
}

/* FIXME:
 * Anyway a NULL pointer should not be passed as Arg, for Lookup&Delete
 * will return NULL if the specified item is not in the table.
 */
BOOLEAN HASH_TABLE_Insert (HASH_TABLE *pTable, const char *Name, void *Arg)
{
    int i;
    BUCKET_ENTRY *p;

    SCI_ASSERT (SCI_NULL != pTable);/*assert verified*/
    SCI_ASSERT (SCI_NULL != pTable->Slots);/*assert verified*/
    SCI_ASSERT (NULL != Name);/*assert verified*/

    i = Hash (pTable, Name);
    p = SCI_ALLOC_APP (sizeof (*p));

    if (SCI_NULL == p)
    {
        return SCI_FALSE;
    }

    p->Name = SCI_ALLOC_APP (strlen (Name) + 1);

    if (SCI_NULL == p->Name)
    {
        SCI_FREE (p);
        return SCI_FALSE;
    }

    /* We do not check whether there has been a item with the same name in
    * the symbol table.
    */
    strcpy (p->Name, Name);
    p->Arg = Arg;
    p->Next = pTable->Slots[i];
    pTable->Slots[i] = p;

    return SCI_TRUE;
}

void *HASH_TABLE_Delete (HASH_TABLE *pTable, const char *Name)
{
    BUCKET_ENTRY **prev, *p;
    void *arg;

    SCI_ASSERT (SCI_NULL != pTable);/*assert verified*/
    SCI_ASSERT (SCI_NULL != pTable->Slots);/*assert verified*/
    SCI_ASSERT (SCI_NULL != Name);/*assert verified*/

    prev = &pTable->Slots[Hash (pTable, Name) ];

    while (NULL != (p = *prev))
    {
        if (0 == strcmp (p->Name, Name))
        {
            *prev = p->Next;
            arg = p->Arg;
            SCI_FREE (p->Name);
            SCI_FREE (p);

            return arg;
        }

        prev = &p->Next;
    }

    return SCI_NULL;
}

void *HASH_TABLE_Lookup (HASH_TABLE *pTable, const char *Name)
{
    BUCKET_ENTRY *p;

    SCI_ASSERT (SCI_NULL != pTable);/*assert verified*/
    SCI_ASSERT (SCI_NULL != pTable->Slots);/*assert verified*/
    SCI_ASSERT (NULL != Name);/*assert verified*/

    p = pTable->Slots[Hash (pTable, Name) ];

    while (p)
    {
        if (0 == strcmp (p->Name, Name))
        {
            return p->Arg;
        }

        p = p->Next;
    }

    return SCI_NULL;
}
//End of file
