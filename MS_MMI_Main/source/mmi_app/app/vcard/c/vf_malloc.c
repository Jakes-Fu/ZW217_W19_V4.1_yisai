/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_malloc.c $
    $Revision: 1.7 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Implements the memory allocation functions used by the rest of the vformat
    library, defaulting to the functions provided by vf_malloc_stdlib.c which
    uses malloc() etc.

    Code below knows nothing about the specific allocation scheme in use or it's
    debugging facilities and is simply provides the common functions and the
    ability to point vformat at an alternative allocation module.

    The only conditional compile option recognised is VFORMAT_MEM_DEBUG which
    is described in the accompanying header file.
    
    Note
    ====
    The original code merged the malloc() implementation with the debug facility
    and also included a conditional compile VFORMAT_EXT_MALLOC to control the use
    of external allocation functions.  Given that this tripped up the _author_ on
    at least one public test event (UPF-9, where the sync engine was unable to read
    VCARDS & extract the X-IRMC stuff) it was considered overcomplicated and has
    been considerably simplified ;) - T.

MODIFICATION HISTORY
 *  $Log: vf_malloc.c,v $
 *  Revision 1.7  2002/10/26 16:09:23  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.6  2002/10/08 21:45:07  tilda
 *  IID620473 - reduce c-runtime dependencies.
 *
 *  Revision 1.5  2002/10/08 21:08:43  tilda
 *  Improve memory debugging functions.
 *
 *  Revision 1.4  2001/10/13 14:58:56  tilda
 *  Tidy up version headers, add vf_strings.h where needed.
 *
 *  Revision 1.3  2001/10/10 20:53:56  tilda
 *  Various minor tidy ups.
 *  
 *  Revision 1.2  2001/10/09 22:01:59  tilda
 *  Remove older version control comments.
 * 
 *******************************************************************************/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_malloc_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_malloc.c,v 1.7 2002/10/26 16:09:23 tilda Exp $";
#endif

/*=============================================================================*
 ANSI C & System-wide Header Files
 *============================================================================*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "types.h"

/*============================================================================*
 Interface Header Files
 *============================================================================*/

#include "vf_iface.h"

/*============================================================================*
 Local Header File
 *============================================================================*/

#include "vf_config.h"
#include "vf_malloc.h"
#include "vf_internals.h"
#if !defined(VFORMAT_EXCLUDE_MALLOC)
#include "vf_malloc_stdlib.h"
#endif

/*============================================================================*
 Public Data
 *============================================================================*/
/* None */

/*============================================================================*
 Private Defines
 *============================================================================*/
/* None */

/*============================================================================*
 Private Data Types
 *============================================================================*/
/* None */

/*============================================================================*
 Private Function Prototypes
 *============================================================================*/
/* None */

/*============================================================================*
 Private Data
 *============================================================================*/

/*
 * Pointers to the functions in use - initialised to the default CRT based allocation
 * functions unless VFORMAT_EXCLUDE_MALLOC is set in which case they are initialised
 * to NULL in the expectation that the user calls vf_set_mem_functions() at some point.
 */

#if defined(VFORMAT_EXCLUDE_MALLOC)
static vf_malloc_fn_t vf_malloc_fn;
static vf_realloc_fn_t vf_realloc_fn;
static vf_free_fn_t vf_free_fn;
#else
static vf_malloc_fn_t vf_malloc_fn = _vf_stdlib_malloc;/*lint !e64*/
static vf_realloc_fn_t vf_realloc_fn = _vf_stdlib_realloc;/*lint !e64*/
static vf_free_fn_t vf_free_fn = _vf_stdlib_free;/*lint !e64*/
#endif

/*============================================================================*
 Public Function Implementations
 *============================================================================*/



/*----------------------------------------------------------------------------*
 * NAME
 *      vf_set_mem_functions()
 * 
 * DESCRIPTION
 *      Set the memory allocation functions we're using.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

void vf_set_mem_functions(
    vf_malloc_fn_t malloc_fn,           /* Allocation function */
    vf_realloc_fn_t realloc_fn,         /* Reallocation function */
    vf_free_fn_t free_fn                /* Free function */
    )
{
    vf_realloc_fn = realloc_fn;
    vf_malloc_fn = malloc_fn;
    vf_free_fn = free_fn;
}





/*----------------------------------------------------------------------------*
 * NAME
 *      _vf_malloc()
 * 
 * DESCRIPTION
 *      Allocate chunk of memory.
 *
 * RETURNS
 *      Ptr to new block, or NULL if failed.
 *----------------------------------------------------------------------------*/

void *_vf_malloc(
    uint32_t s                      /* Size required */
#if defined(VFORMAT_MEM_DEBUG)
    , const char *file,             /* Filename */
    int line                        /* Line number */
#endif
    )
{
    void *p = NULL;

    if (vf_malloc_fn)
    {
#if defined(VFORMAT_MEM_DEBUG)
        p = vf_malloc_fn(s, file, line);
#else
        p = vf_malloc_fn(s);
#endif
    }

    return p;
}





/*----------------------------------------------------------------------------*
 * NAME
 *      _vf_realloc()
 * 
 * DESCRIPTION
 *      Re-allocate chunk of memory allocated by _vf_malloc().
 *
 * RETURNS
 *      Ptr to new block, or NULL if failed.
 *----------------------------------------------------------------------------*/

void *_vf_realloc(
    void *p,                        /* Original pointer */
    uint32_t s                      /* Size required */
#if defined(VFORMAT_MEM_DEBUG)
    , const char *file,             /* filename */
    int line                        /* line number */
#endif
    )
{
    void *np = NULL;

    if (vf_realloc_fn)
    {
#if defined(VFORMAT_MEM_DEBUG)
        np = vf_realloc_fn(p, s, file, line);
#else
        np = vf_realloc_fn(p, s);
#endif
    }

    return np;
}





/*----------------------------------------------------------------------------*
 * NAME
 *      _vf_free()
 * 
 * DESCRIPTION
 *      De-allocate chunk of memory allocated by _vf_malloc().
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

void _vf_free(
    void *p                             /* Pointer */
#if defined(VFORMAT_MEM_DEBUG)
    , const char *file,                 /* Filename */
    int line                            /* Line number */
#endif
    )
{
    if (vf_free_fn)
    {
#if defined(VFORMAT_MEM_DEBUG)
        vf_free_fn(p, file, line);
#else
        vf_free_fn(p);
#endif
    }
}
#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT

/*============================================================================*
 Private Function Implementations
 *============================================================================*/
/* None */

/*============================================================================*
 End Of File
 *============================================================================*/
