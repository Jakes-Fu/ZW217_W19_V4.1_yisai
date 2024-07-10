/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_common.h $
    $Revision: 1.1 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Common header file for vformat library.  This file is internal to vformat
    (ie. changes here don't affect the interface) but contains the definitions
    which affect the operation of various components eg. the memory debug.

    This file is #included before all other header files within the library.

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_config.h,v $
 *  Revision 1.1  2002/10/26 15:57:11  tilda
 *  Initial Version
 *
 * 
 *******************************************************************************/

#ifndef _VF_CONFIG_H_
#define _VF_CONFIG_H_

#ifndef NORCSID
static const char vf_config_h_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_config.h,v 1.1 2002/10/26 15:57:11 tilda Exp $";
#endif

/*=============================================================================*
 Public Includes
 *============================================================================*/
/* None */

/*=============================================================================*
 Public Defines
 *============================================================================*/

/*
 * If building debug version under windows then include the memory debug interface.
 */
#if (defined(WIN) || defined(WIN32)) && defined(_DEBUG)
#if !defined(VFORMAT_MEM_DEBUG)
#define VFORMAT_MEM_DEBUG
#endif
#endif

/*
 * Conditional compiles allowing configuration of various string portability functions.
 */
#define HAVE_STRLEN
#define HAVE_STRCPY
#define HAVE_STRCMP
#define HAVE_STRCAT
#define HAVE_STRSTR
/* #define HAVE_STRICMP */
/* #define HAVE_MEMCPY */
/* #define HAVE_MEMSET */

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

#endif /*_VF_CONFIG_H_*/
