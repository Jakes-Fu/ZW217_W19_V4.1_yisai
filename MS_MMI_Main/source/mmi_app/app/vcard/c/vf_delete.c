/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_delete.c $
    $Revision: 1.14 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Delete a vformat object.

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_delete.c,v $
 *  Revision 1.14  2002/11/03 18:43:16  tilda
 *  IID619851 - Update and check headers and function prototypes.
 *
 *  Revision 1.13  2002/10/26 16:09:24  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.12  2002/10/08 21:45:07  tilda
 *  IID620473 - reduce c-runtime dependencies.
 *
 *  Revision 1.11  2002/10/08 21:11:36  tilda
 *  Remove common.h.
 *
 *  Revision 1.10  2001/11/05 21:07:20  tilda
 *  Various changes for initial version of vfedit.
 *
 *  Revision 1.9  2001/10/24 18:36:06  tilda
 *  BASE64 bugfixes.  Split reader/writer code. Start create/modify work.
 *
 *  Revision 1.8  2001/10/14 19:53:36  tilda
 *  Group handling.  NO group searching functions.
 *
 *  Revision 1.7  2001/10/13 16:22:08  tilda
 *  Introduce VBINDATA_T and VOBJDATA_T to tidy up internals.
 *
 *  Revision 1.6  2001/10/13 14:58:56  tilda
 *  Tidy up version headers, add vf_strings.h where needed.
 *
 *  Revision 1.5  2001/10/13 14:49:30  tilda
 *  Add string array code to unify handling of names / values.
 *  
 *  Revision 1.4  2001/10/12 16:20:03  tilda
 *  Correctly parse compound quoted printable properties.
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
static const char vf_delete_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_delete.c,v 1.14 2002/11/03 18:43:16 tilda Exp $";
#endif

/*=============================================================================*
 ANSI C & System-wide Header Files
 *=============================================================================*/
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
#include "vf_string_arrays.h"

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

static void free_prop_list(
    VPROP_T *p_props                /* List of properties to free */
    );



/*============================================================================*
 Private Data
 *============================================================================*/
/* None */

/*============================================================================*
 Public Function Implementations
 *============================================================================*/



/*----------------------------------------------------------------------------*
 * NAME
 *      vf_delete_object()
 * 
 * DESCRIPTION
 *      Cleans up the memory used by the indicated vformat object.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

void vf_delete_object(
    VF_OBJECT_T *p_object,
    bool_t all
    )
{
    VOBJECT_T *p_obj = (VOBJECT_T *)p_object;
    
    if (p_obj)
    {
        VOBJECT_T *p_next = p_obj->p_next;

        free_prop_list(p_obj->p_props);

        if (p_obj->p_type)
        {
            vf_free(p_obj->p_type);
        }

        vf_free(p_obj);

        if (all )
        {
            vf_delete_object((VF_OBJECT_T *)p_next, TRUE);
        }
    }
}




/*----------------------------------------------------------------------------*
 * NAME
 *      vf_delete_prop()
 * 
 * DESCRIPTION
 *      Deletes indicated property from the indicated object.  Deletes prop
 *      contents if dc is set.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

void vf_delete_prop(
    VF_OBJECT_T *p_object,          /* The object we're deleting from */
    VF_PROP_T *p_prop,              /* The property we're removing */
    bool_t dc                       /* Should property contents be deleted? */
    )
{
    VOBJECT_T *p_obj = (VOBJECT_T *)p_object;
    
    if (p_obj)
    {
        VPROP_T **p_vprop = &(p_obj->p_props);

        while (*p_vprop)
        {
            if ((*p_vprop) == (VPROP_T *)p_prop)
            {
                *p_vprop = ((VPROP_T *)p_prop)->p_next;

                if (dc)
                {
                    delete_prop_contents(p_prop, TRUE);
                }

                vf_free(p_prop);

                break;
            }
            else
            {
                p_vprop = &((*p_vprop)->p_next);
            }
        }
    }
}





/*----------------------------------------------------------------------------*
 * NAME
 *      delete_prop_contents()
 * 
 * DESCRIPTION
 *      Cleans up the memory used by the value associated with the property.
 *      The property is not deleted => can be a statically allocated variable.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

void delete_prop_contents(
    VF_PROP_T *p_vprop,         /* The VF_PROP_T to clean */
    bool_t delname              /* Delete the name as well? */
    )
{
    VPROP_T *p_prop = (VPROP_T *)p_vprop;

    if (delname)
    {
        free_string_array_contents(&p_prop->name);

        if (p_prop->p_group)
        {
            vf_free(p_prop->p_group);
            p_prop->p_group = NULL;
        }
    }

    if (p_prop->value.v.b.p_buffer)
    {
        vf_free(p_prop->value.v.b.p_buffer);
        p_prop->value.v.b.p_buffer = NULL;
    }

    if (p_prop->value.v.s.pp_strings)
    {
        uint32_t n;

        for (n = 0;n < p_prop->value.v.s.n_strings;n++)
        {
            if (p_prop->value.v.s.pp_strings[n])
            {
                vf_free(p_prop->value.v.s.pp_strings[n]);
                p_prop->value.v.s.pp_strings[n] = NULL;
            }
        }

        vf_free(p_prop->value.v.s.pp_strings);
        p_prop->value.v.s.pp_strings = NULL;

        p_prop->value.v.s.n_strings = 0;
    }

    if (p_prop->value.v.o.p_object)
    {
        vf_delete_object((VF_OBJECT_T *)p_prop->value.v.o.p_object, TRUE);
        p_prop->value.v.o.p_object = NULL;
    }
}




/*============================================================================*
 Private Function Implementations
 *============================================================================*/



/*----------------------------------------------------------------------------*
 * NAME
 *      free_prop_list()
 * 
 * DESCRIPTION
 *      Cleans up the memory used by the indicated property list.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

void free_prop_list(
    VPROP_T *p_props    /* List of properties to free */
    )
{
    VPROP_T *p_tmp;

    for (p_tmp = p_props;p_tmp;)
    {
        VPROP_T *p_next = p_tmp->p_next;

        delete_prop_contents((VF_PROP_T *)p_tmp, TRUE);

        vf_free(p_tmp);

        p_tmp = p_next;
    }
}

#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT






/*============================================================================*
 End Of File
 *============================================================================*/
