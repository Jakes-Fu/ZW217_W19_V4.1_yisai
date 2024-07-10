/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_malloc.c $
    $Revision: 1.1 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    C standard library based memory allocation functions referenced by default in
    the accompanying vf_malloc.c file.  These are excluded from the build entirely
    if VFORMAT_EXCLUDE_MALLOC is set in the build options.

MODIFICATION HISTORY
 *  $Log: vf_malloc_stdlib.c,v $
 *  Revision 1.1  2002/10/26 15:57:11  tilda
 *  Initial Version
 *
 *******************************************************************************/

//#if !defined(VFORMAT_EXCLUDE_MALLOC)
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_malloc_stdlib_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_malloc_stdlib.c,v 1.1 2002/10/26 15:57:11 tilda Exp $";
#endif

/*=============================================================================*
 ANSI C & System-wide Header Files
 *============================================================================*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "types.h"
//#include "sci_types.h"
#include "os_api.h"

#include <stdlib.h>
#include <stdio.h>

/*============================================================================*
 Interface Header Files
 *============================================================================*/

#include "vf_iface.h"

/*============================================================================*
 Local Header File
 *============================================================================*/

#include "vf_config.h"
#include "vf_malloc.h"
#include "vf_strings.h"

/*============================================================================*
 Public Data
 *============================================================================*/
/* None */

/*============================================================================*
 Private Defines
 *============================================================================*/

#define NUMMEMBLOCKS   (1024)

#if defined(WIN) || defined(WIN32)
#define DEBUGBREAK _asm { int 3 }
#endif

#ifndef DEBUGBREAK
#define DEBUGBREAK
#endif

#define ALLOCBREAK (-1)

/*============================================================================*
 Private Data Types
 *============================================================================*/

typedef struct MEMBLOCK_T
{
    void            *p;     /* The block */
    uint32_t        s;      /* The size */
    char            *file;  /* filename it was allocated in */
    int             line;   /* line number */
}MEMBLOCK_T;

/*============================================================================*
 Private Function Prototypes
 *============================================================================*/
#if defined(VFORMAT_MEM_DEBUG)
static void init_mem_debug(void);

static void atexit_dump_mem(void);
#endif
/*============================================================================*
 Private Data
 *============================================================================*/

static MEMBLOCK_T *blocks = PNULL;

/*============================================================================*
 Public Function Implementations
 *============================================================================*/


/*----------------------------------------------------------------------------*
 * NAME
 *      _vf_stdlib_malloc()
 * 
 * DESCRIPTION
 *      vformat compatible memory allocator using C standard library functions.
 *
 * RETURNS
 *      Ptr to new block, or NULL if failed.
 *----------------------------------------------------------------------------*/

void *_vf_stdlib_malloc(
    uint32_t s                  /* Size required */
#if defined(VFORMAT_MEM_DEBUG)
    , const char *file,         /* filename */
    int line                    /* line number */
#endif
    )
{
    void *p = NULL;

#if defined(VFORMAT_MEM_DEBUG)

    int32 i=0;
    
    init_mem_debug();

    for (i = 0;i < NUMMEMBLOCKS;i++)
    {
        if (blocks[i].p)
        {
        }
        else
        {
            p = malloc(s);

            if (p)
            {
                blocks[i].p = p;
                blocks[i].s = s;
                blocks[i].file = (char *)file;
                blocks[i].line = line;
            }

            if (i == ALLOCBREAK)
            {
                DEBUGBREAK;
            }

            break;
        }
    }

#else /*defined(VFORMAT_MEM_DEBUG)*/

    p = malloc(s);

#endif

    return p;
}


/*----------------------------------------------------------------------------*
 * NAME
 *      vf_stdlib_init_mem()
 * 
 * DESCRIPTION
 *      alloc blocks.
 *
 * RETURNS
 *      void.
 *----------------------------------------------------------------------------*/
void vf_stdlib_init_mem(void)
{
    if(PNULL == blocks)
    {
		blocks = (MEMBLOCK_T *)SCI_ALLOC_APP(NUMMEMBLOCKS * sizeof(MEMBLOCK_T));
		SCI_ASSERT(blocks != PNULL);/*assert verified*/
    }
}


/*----------------------------------------------------------------------------*
 * NAME
 *      _vf_stdlib_realloc()
 * 
 * DESCRIPTION
 *      Re-allocate chunk of memory allocated by _vf_malloc().
 *
 * RETURNS
 *      Ptr to new block, or NULL if failed.
 *----------------------------------------------------------------------------*/

void *_vf_stdlib_realloc(
    void *p,                    /* Original pointer */
    uint32_t s                  /* Size required */
#if defined(VFORMAT_MEM_DEBUG)
    , const char *file,         /* Filename */
    int line                    /* Line number */
#endif
    )
{
    void *np = NULL;

#if defined(VFORMAT_MEM_DEBUG)

    uint32_t i;

    init_mem_debug();

    if (!p)
        return _vf_stdlib_malloc(s, file, line);

    for (i = 0;i < NUMMEMBLOCKS;i++)
    {
        if (blocks[i].p == p)
        {
            np = realloc(p, s);

            if (np)
            {
                blocks[i].p = np;
                blocks[i].s = s;
            }

            break;
        }
    }

#else /*defined(VFORMAT_MEM_DEBUG)*/

    np = realloc(p, s);

#endif /*defined(VFORMAT_MEM_DEBUG)*/

    return np;
}





/*----------------------------------------------------------------------------*
 * NAME
 *      _vf_stdlib_free()
 * 
 * DESCRIPTION
 *      De-allocate chunk of memory allocated by _vf_malloc().
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

void _vf_stdlib_free(
    void *p                         /* Pointer */
#if defined(VFORMAT_MEM_DEBUG)
    , const char *file,             /* Filename */
    int line                        /* Line number */
#endif
    )
{
#if defined(VFORMAT_MEM_DEBUG)

    uint32_t i;

    init_mem_debug();

    for (i = 0;i < NUMMEMBLOCKS;i++)
    {
        if (blocks[i].p == p)
        {
            blocks[i].p = NULL;
            blocks[i].s = 0;
            blocks[i].line = 0;
            blocks[i].file = NULL;

            free(p);

            break;
        }
    }

#else

    free(p);

#endif
}





/*----------------------------------------------------------------------------*
 * NAME
 *      vf_stdlib_dump_alloc_info()
 * 
 * DESCRIPTION
 *      Display current state of memory allocator.
 *
 * RETURNS
 *      TRUE if blocks are currently allocated, FALSE else.
 *----------------------------------------------------------------------------*/

bool_t vf_stdlib_dump_alloc_info()
{
    uint32_t i;
    uint32_t c, s;

    for (i = 0, c = 0, s = 0;i < NUMMEMBLOCKS;i++)
    {
        if (blocks[i].p)
        {
            c += 1;
            s += blocks[i].s;

            printf("block[%ld] size %lu, file %s, line %d\n", i, blocks[i].s, blocks[i].file, blocks[i].line);
        }
    }

    if (c)
    {
        printf("... a total of %lu blocks, %lu bytes\n", c, s);
    }

    return (0 < c) ? TRUE : FALSE;
}



/*============================================================================*
 Private Function Implementations
 *============================================================================*/


/*----------------------------------------------------------------------------*
 * NAME
 *      init_mem_debug()
 * 
 * DESCRIPTION
 *      Initialisation function.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/
#if defined(VFORMAT_MEM_DEBUG)
static void init_mem_debug()
{
    static bool_t init_yet = FALSE;

    if (init_yet)
    {
        /* OK */
    }
    else
    {
        p_memset(blocks, '\0', sizeof(blocks));

        atexit(atexit_dump_mem);

        init_yet = TRUE;
    }
}





/*----------------------------------------------------------------------------*
 * NAME
 *      atexit_dump_mem()
 * 
 * DESCRIPTION
 *      atexit() callback function which dumps memory allocated info.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

static void atexit_dump_mem(void)
{
    vf_stdlib_dump_alloc_info();
}

#endif
//#endif /*defined(VFORMAT_EXCLUDE_MALLOC)*/
#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT

/*============================================================================*
 End Of File
 *============================================================================*/
