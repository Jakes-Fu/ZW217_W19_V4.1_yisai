/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_malloc.h $
    $Revision: 1.1 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Externs for the malloc() based memory allocator.

MODIFICATION HISTORY
 *  $Log: vf_malloc_stdlib.h,v $
 *  Revision 1.1  2002/10/26 15:57:11  tilda
 *  Initial Version
 *
 * 
 *******************************************************************************/

#ifndef _VF_MALLOC_STDLIB_H_
#define _VF_MALLOC_STDLIB_H_

#ifndef NORCSID
static const char vf_malloc_stdlib_h_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_malloc_stdlib.h,v 1.1 2002/10/26 15:57:11 tilda Exp $";
#endif

/*=============================================================================*
 Public Includes
 *============================================================================*/
/* None */

/*=============================================================================*
 Public Defines
 *============================================================================*/
/* None */

/*=============================================================================*
 Public Types
 *============================================================================*/
/* None */

/*=============================================================================*
 Public Functions
 *============================================================================*/
/* None */

/*=============================================================================*
 End of file
 *============================================================================*/


/*----------------------------------------------------------------------------*
 * NAME
 *      _vf_stdlib_malloc(), _vf_stdlib_realloc(), _vf_stdlib_free()
 * 
 * DESCRIPTION
 *      Memory allocation functions provided in terms of C runtime library
 *      malloc() etc.  If VFORMAT_MEM_DEBUG is defined the line & file are
 *      passed through for ebugging purposes.
 *
 * RETURNS
 *      (various)
 *----------------------------------------------------------------------------*/
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
void vf_stdlib_init_mem(void);

#if defined(VFORMAT_MEM_DEBUG)

extern void *_vf_stdlib_malloc(uint32_t s, const char *file, int line);
extern void *_vf_stdlib_realloc(void *p, uint32_t ns, const char *file, int line);
extern void _vf_stdlib_free(void *p, const char *file, int line);

#else

extern void *_vf_stdlib_malloc(uint32_t s);
extern void *_vf_stdlib_realloc(void *p, uint32_t ns);
extern void _vf_stdlib_free(void *p);

#endif

/*=============================================================================*
 FIN
 *============================================================================*/

#endif /*_VF_MALLOC_STDLIB_H_*/
