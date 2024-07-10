/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_modified.c $
    $Revision: 1.4 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Memory allocation system for vformat parser / editor.

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_modified.c,v $
 *  Revision 1.4  2002/11/02 18:29:26  tilda
 *  IID485157 - UI does character conversion based on CHARSET property.
 *
 *  Revision 1.3  2002/10/29 07:19:20  tilda
 *  Tidy headers.
 *
 *  Revision 1.2  2002/10/26 16:09:23  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.1  2002/10/11 20:29:25  tilda
 *  Include vf_modified.c
 *
 *******************************************************************************/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_modified_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_modified.c,v 1.4 2002/11/02 18:29:26 tilda Exp $";
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
#include "vf_modified.h"

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
/* None */

/*============================================================================*
 Public Function Implementations
 *============================================================================*/

/*---------------------------------------------------------------------------*
 * NAME
 *      vf_is_modified()
 * 
 * DESCRIPTION
 *      Return the status of the modified flag for the indicated object.
 *
 * RETURNS
 *      TRUE/FALSE
 *---------------------------------------------------------------------------*/

bool_t vf_is_modified(
    VF_OBJECT_T *p_object
    )
{
    bool_t ret = FALSE;

    if (p_object)
    {
        ret = ((VOBJECT_T *)p_object)->modified;
    }

    return ret;
}




/*---------------------------------------------------------------------------*
 * NAME
 *      mark_property_modified()
 * 
 * DESCRIPTION
 *      Mark indicated property and it's owning object as modified.  If the
 *      recurse flag is st, then the owning object's parent object is also
 *      marked as modified recursively up to the top of the tree.
 *
 * RETURNS
 *      (none)
 *---------------------------------------------------------------------------*/

void mark_property_modified(
    VPROP_T *p_prop,        /* The property */
    bool_t recurse          /* Recurse? */
    )
{
    p_prop->modified = TRUE;
    p_prop->p_parent->modified = TRUE;

    if (recurse)
    {
        VOBJECT_T *p_parent = p_prop->p_parent->p_parent;

        for (;p_parent;p_parent = p_parent->p_parent)
        {
            p_parent->modified = TRUE;
        }
    }
}
#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT

/*============================================================================*
 Private Function Implementations
 *============================================================================*/

 /*============================================================================*
 End Of File
 *============================================================================*/
