/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_malloc.h $
    $Revision: 1.6 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Memory allocation functions / macros for vformat code.  The code always uses
    vf_malloc(), vf_realloc() and vf_free() which are replaced by macros with the
    names of functions implemented in vf_malloc.c.

    If the conditional compile VFORMAT_MEM_DEBUG is set then in addition to the
    caller's allocation parameters, filename and line number information is
    provided to the lower layer which may record this for debugging.

    Additionally, a function vf_set_mem_functions() is provided which allows an
    application to replace the allocation functions with it's own.  This should
    be done before vformat allocates any memory ie. during initialisation.

    Unless the library is built with the symbol VFORMAT_EXCLUDE_MALLOC defined
    then the library defaults to using malloc() etc.

MODIFICATION HISTORY
 *  $Log: vf_malloc.h,v $
 *  Revision 1.6  2002/10/26 16:09:23  tilda
 *  IID629125 - Ensure string functions used are portable.
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

#ifndef _VF_MALLOC_H_
#define _VF_MALLOC_H_

#ifndef NORCSID
static const char vf_malloc_h_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_malloc.h,v 1.6 2002/10/26 16:09:23 tilda Exp $";
#endif

/*=============================================================================*
 Public Includes
 *============================================================================*/
/* None */

/*=============================================================================*
 Public Defines
 *============================================================================*/

/*
 * If VFORMAT_MEM_DEBUG is defined, map the vf_xxx() allocation calls to
 * the debugging versions found in vf_malloc.c
 */

#if defined(VFORMAT_MEM_DEBUG)

#define vf_malloc(x)        _vf_malloc(x, __FILE__, __LINE__)
#define vf_realloc(x, y)    _vf_realloc(x, y, __FILE__, __LINE__)
#define vf_free(x)          _vf_free(x, __FILE__, __LINE__)

#else /*defined(VFORMAT_MEM_DEBUG)*/

#define vf_malloc(x)        _vf_malloc(x)
#define vf_realloc(x, y)    _vf_realloc(x, y)
#define vf_free(x)          _vf_free(x)

#endif /*defined(VFORMAT_MEM_DEBUG)*/

/*=============================================================================*
 Public Types
 *============================================================================*/

#if defined(VFORMAT_MEM_DEBUG)

typedef void *(*vf_malloc_fn_t)(uint32_t s, const char *file, int line);
typedef void *(*vf_realloc_fn_t)(void *p, uint32_t ns, const char *file, int line);
typedef void (*vf_free_fn_t)(void *p, const char *file, int line);

#else /*defined(VFORMAT_MEM_DEBUG)*/

typedef void *(*vf_malloc_fn_t)(uint32_t s);
typedef void *(*vf_realloc_fn_t)(void *p, uint32_t ns);
typedef void (*vf_free_fn_t)(void *p);

#endif /*defined(VFORMAT_MEM_DEBUG)*/

/*=============================================================================*
 Public Functions
 *============================================================================*/
/* None */

/*=============================================================================*
 End of file
 *============================================================================*/


/*----------------------------------------------------------------------------*
 * NAME
 *      _vf_malloc(), _vf_realloc(), _vf_free()
 * 
 * DESCRIPTION
 *      Memory allocation functions in use.  If VFORMAT_MEM_DEBUG is defined
 *      the line & file are recorded as well.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

#if defined(VFORMAT_MEM_DEBUG)

extern void *_vf_malloc(uint32_t s, const char *file, int line);
extern void *_vf_realloc(void *p, uint32_t ns, const char *file, int line);
extern void _vf_free(void *p, const char *file, int line);

#else

extern void *_vf_malloc(uint32_t s);
extern void *_vf_realloc(void *p, uint32_t ns);
extern void _vf_free(void *p);

#endif



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

extern VFORMATDECLSPEC void vf_set_mem_functions(
    vf_malloc_fn_t malloc_fn,
    vf_realloc_fn_t realloc_fn,
    vf_free_fn_t free_fn
    );




/*=============================================================================*
 FIN
 *============================================================================*/

#endif /*_VF_MALLOC_H_*/
