/*******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_modified.h $
    $Revision: 1.1 $
    $Author: tilda $

ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Library internal access to the modification status.

REFERENCES
    (none)

MODIFICATION HISTORY
 *  $Log: vf_modified.h,v $
 *  Revision 1.1  2002/10/11 20:22:32  tilda
 *  Added / repaired vf_modified.h
 *
 * 
 * 2     9/05/02 3:49p Nm2
 * Adjust memoruy allocation system for use in an embedded build.
 * 
 * 1     7/08/02 7:24a Nm2
 * 
 *******************************************************************************/

#ifndef _VF_MODIFIED_H_
#define _VF_MODIFIED_H_

#ifndef NORCSID
static const char vf_modified_h_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_modified.h,v 1.1 2002/10/11 20:22:32 tilda Exp $";
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

extern void mark_property_modified(
    VPROP_T *p_prop,        /* The property */
    bool_t recurse          /* Recurse? */
    );


/*=============================================================================*
 End of file
 *============================================================================*/

#endif /*_VF_MODIFIED_H_*/
