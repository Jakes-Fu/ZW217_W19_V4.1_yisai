/*******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile$
    $Revision: 1.4 $
    $Author: tilda $

ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    In the abscence of suitable typedefs provided by whatever environment you're
    building under, this defines the minimal set of types required by vformat library.

MODIFICATION HISTORY
 *  $Log: types.h,v $
 *  Revision 1.4  2002/10/08 20:23:16  tilda
 *  Improve comments.  Tidy header.
 *
 *  Revision 1.3  2002/10/08 20:20:10  tilda
 *  Comments (!).  Removed unused types.
 *******************************************************************************/

#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef NORCSID
static const char _types_h_vss_id[] = "$Header: /cvsroot/vformat/src/common/types.h,v 1.4 2002/10/08 20:23:16 tilda Exp $";
#endif

/*=============================================================================*
 Public Includes
 *============================================================================*/
/* None */

/*=============================================================================*
 Public Defines
 *============================================================================*/

#undef  NULL
#define NULL    0

#undef  FALSE
#define FALSE   ((bool_t)(0))

#undef  TRUE
#define TRUE    ((bool_t)(1))

/*=============================================================================*
 Public Types
 *============================================================================*/

/*
 * Basic types.
 */
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int   uint32_t;

/*
 * Please avoide 'BOOL' (from Windows) and 'bool' (from C++) - vformat is used
 * extensively in pure C embedded applications where neither are relevant.
 */
typedef unsigned char bool_t;

/*=============================================================================*
 Public Functions
 *============================================================================*/
/* None */

/*=============================================================================*
 End of file
 *============================================================================*/

#endif// _TYPES_H_
