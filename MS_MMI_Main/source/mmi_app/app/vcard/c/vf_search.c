/****************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_access.c $
    $Revision: 1.11 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Basic searching functions for the vformat library.

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_search.c,v $
 *  Revision 1.11  2002/11/03 18:43:16  tilda
 *  IID619851 - Update and check headers and function prototypes.
 *
 *  Revision 1.10  2002/11/02 18:29:26  tilda
 *  IID485157 - UI does character conversion based on CHARSET property.
 *
 *  Revision 1.9  2002/10/26 16:09:23  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.8  2002/10/08 21:45:07  tilda
 *  IID620473 - reduce c-runtime dependencies.
 *
 *  Revision 1.7  2002/10/08 21:11:36  tilda
 *  Remove common.h.
 *
 *  Revision 1.6  2002/02/24 17:10:34  tilda
 *  Add API for "is modified" functionality.
 *
 *  Revision 1.5  2001/11/18 21:46:59  tilda
 *  Remove redundant code.
 *
 *  Revision 1.4  2001/11/16 22:34:50  tilda
 *  New vf_get_property() allows append as well as find,
 *
 *  Revision 1.3  2001/11/05 21:07:19  tilda
 *  Various changes for initial version of vfedit.
 *
 *  Revision 1.2  2001/10/24 18:56:29  tilda
 *  Tidy headers after import.  Fix include path in release build.
 *
 *  Revision 1.1  2001/10/24 18:34:35  tilda
 *  Initial Version.
 * 
 *****************************************************************************/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_search_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_search.c,v 1.11 2002/11/03 18:43:16 tilda Exp $";
#endif

/*============================================================================*
 ANSI C & System-wide Header Files
 *============================================================================*/

#ifdef WIN32
#include "std_header.h"
#endif

#include "types.h"

/*===========================================================================*
 Interface Header Files
 *===========================================================================*/

#include "vf_iface.h"

/*===========================================================================*
 Local Header File
 *===========================================================================*/

#include "vf_config.h"
#include "vf_malloc.h"
#include "vf_internals.h"
#include "vf_strings.h"
#include "vf_string_arrays.h"

/*===========================================================================*
 Public Data
 *===========================================================================*/
/* None */

/*===========================================================================*
 Private Defines
 *===========================================================================*/

/*
 * Maximum supported search tags.
 */
#define MAXNUMTAGS          (10)

/*===========================================================================*
 Private Data Types
 *===========================================================================*/
/* None */

/*===========================================================================*
 Private Function Prototypes
 *===========================================================================*/


/*===========================================================================*
 Private Data
 *===========================================================================*/
/* None */

/*===========================================================================*
 Public Function Implementations
 *===========================================================================*/



/*---------------------------------------------------------------------------*
 * NAME
 *      vf_get_property()
 * 
 * DESCRIPTION
 *      Basic searching function locating elements of the VOBJECT by qualified
 *      name.  The function is a varargs function (like sprintf) and the list
 *      of arguments must be NULL terminated (hence the appearance of the
 *      p_qualifier argument in the arglist).  Valid calls might be:
 *
 *        vf_get_property(&p_out, p_object, FALSE, NULL, "N", NULL);
 *        vf_get_property(&p_out, p_object, FALSE, NULL, "TEL", "WORK", NULL);
 *        vf_get_property(&p_out, p_object, FALSE, "ME", "TEL", "WORK", NULL);
 *        vf_get_property(&p_out, p_object, FALSE, "ME", "*", NULL);
 *
 *      A pointer to the first property matching the search criteria is returned
 *      the pp_prop argument.  The search actually locates all such matches and
 *      pointer to subsequent entries (if there are >1) may be found by calling
 *      the vf_get_next_property() function.
 *
 *      Qualifying tags (ie. not the name) may occur in any order
 *
 *      Note that the VF_PROP_T returned vi pp_prop is an opaque type and the
 *      various accessor functions must be used to get to the data.
 *
 * RETURNS
 *      TRUE iff found/added successfully.  ptr to prop returned via pp_prop.
 *---------------------------------------------------------------------------*/
 
bool_t vf_get_property(
    VF_PROP_T **pp_prop,        /* Output pointer */
    VF_OBJECT_T *p_object,      /* Object to add to */
    vf_get_t ops,               /* Search flags */
    const char *p_group,        /* Group name if any */
    const char *p_name,         /* Name of tag */
    const char *p_qualifier,    /* First qualifier if any */
    ...                         /* Subequent qualifiers */
    )
{
    bool_t ret = FALSE;
    va_list args;

    va_start(args, p_qualifier);  /*lint !e718 !e64 !e746 */

    ret = vf_get_property_ex(pp_prop, p_object, ops, p_group, p_name, p_qualifier, args);

    va_end(args); /*lint !e718 */

    return ret;
}








/*---------------------------------------------------------------------------*
 * NAME
 *      vf_get_property_ex()
 * 
 * DESCRIPTION
 *      The grunt behind vf_get_property(). Manages the search as described
 *      vf_get_property() but takes the list of arguments as a va_list.
 *
 * RETURNS
 *      TRUE iff found / appended OK.
 *---------------------------------------------------------------------------*/

bool_t vf_get_property_ex(
    VF_PROP_T **pp_prop,        /* Output pointer */
    VF_OBJECT_T *p_object,      /* Object to search */
    vf_get_t ops,               /* Search flags */
    const char *p_group,        /* Group name if any */
    const char *p_name,         /* Name of tag */
    const char *p_qualifier,    /* First qualifier if any */
    va_list args                /* Argument list */
    )
{
    bool_t ret = FALSE;
    char **pp_tags = NULL;

    if (!p_name || !p_object)
        return ret;

    if (pp_prop)
    {
        *pp_prop = NULL;
    }

    pp_tags = (char **)vf_malloc(MAXNUMTAGS * sizeof(char *));

    if (pp_tags)
    {
        VOBJECT_T *p_obj = (VOBJECT_T *)p_object;
        VPROP_T *p_props = p_obj->p_props;
        int i;

        VPROP_T **pp_lastprop = &(p_obj->p_props);
        VPROP_T **pp_next_srch = NULL;

        p_memset(pp_tags, '\0', MAXNUMTAGS * sizeof(char *));

        pp_tags[0] = (char *)p_name;

        /*
         * The name is the first tag.  We insist on there being at least one qualifier in the
         * argument list to make sure callers terminate the list.  Subsequent arguments are
         * optional, but will be ignored if the (first) qualifier is NULL.
         */
        if (p_qualifier)
        {
            pp_tags[1] = (char *)p_qualifier;

            for (i = 2;i < MAXNUMTAGS;i++)
            {
                //lint -esym(628, __va_*)
                char *p_tag = va_arg(args, char *);/*lint !e718 !e64 !e746*/

                if (p_tag)
                {
                    pp_tags[i] = p_tag;
                }
                else
                {
                    break;
                }
            }
        }

        if (ops & VFGP_FIND)
        {
            /*
             * Skip down the list or properties.  I'm assuming that we only want to search the
             * current object for properties not the current object AND any contained objects.
             * Such a search sounds a bit application specific, ie. the caller would be looking
             * for a particular structure of things.  Each element that is found is stitched
             * into the emerging list.
             */
            for (;NULL != p_props;p_props = p_props->p_next)
            {
                bool_t found = TRUE;

                /*
                 * Check the name & qualifiers.
                 */
                for (i = 0;found && (i < MAXNUMTAGS) && pp_tags[i];i++)
                {
                    if (0 != p_strcmp(VFP_ANY, pp_tags[i]))
                        found &= string_array_contains_string(&p_props->name, NULL, (uint32_t)-1, pp_tags[i], TRUE);
                }
                if (p_group && found)
                {
                    found = FALSE;

                    if (p_props->p_group)
                    {
                        if (0 == p_stricmp(p_props->p_group, p_group))
                        {
                            found = TRUE;
                        }
                    }
                    else
                    {
                        /* Search group specified & property doesn't have a group => not required */
                    }
                }

                if (found)
                {
                    if (pp_prop)
                    {
                        if (!*pp_prop)
                        {
                            pp_next_srch = &(p_props->p_next_srch);
                            p_props->p_next_srch = NULL;

                            *pp_prop = (VF_PROP_T *)p_props;
                        }
                        else
                        {
							if (pp_next_srch) 
                            {
								*pp_next_srch = p_props;
							}

                            pp_next_srch = &(p_props->p_next_srch);
                            p_props->p_next_srch = NULL;
                        }
                    }

                    ret = TRUE;
                }

                if (p_props->p_next)
                {
                    pp_lastprop = &(p_props->p_next);
                }
            }
        }

        if (!ret && (ops & VFGP_APPEND))
        {
            VPROP_T *p_new = (VPROP_T *)vf_malloc(sizeof(VPROP_T));

            if (p_new)
            {
                ret = TRUE;

                p_memset(p_new, '\0', sizeof(VPROP_T));

                p_new->p_parent = p_obj;

                for (i = 0;ret && (i < MAXNUMTAGS) && pp_tags[i];i++)
                {
                    ret = add_string_to_array(&p_new->name, pp_tags[i]);
                }
                
                if (ret)
                {
                    /* All OK */

                    p_new->value.encoding = VF_ENC_7BIT;
                }
                else
                {
                    free_string_array_contents(&p_new->name);

                    vf_free(p_new);
                    p_new = NULL;
                }
            }
            
            if (p_new)
            {
                if (pp_prop)
                {
                    *pp_prop = (VF_PROP_T *)p_new;
                }

                p_new->p_next = *pp_lastprop;              
                *pp_lastprop = p_new;

                ret = TRUE;
            }
        }

        vf_free(pp_tags);
    }

    return ret;
}







/*---------------------------------------------------------------------------*
 * NAME
 *      vf_get_next_property()
 * 
 * DESCRIPTION
 *      Find the next property given the current search critera.
 *
 * RETURNS
 *      TRUE iff found, FALSE else. *pp_prop points to the next property.
 *---------------------------------------------------------------------------*/

bool_t vf_get_next_property(
    VF_PROP_T **pp_prop         /* Output pointer */
    )
{
    bool_t ret = FALSE;

    if (pp_prop)
    {
        VPROP_T **pp_vprop = (VPROP_T **)pp_prop;

        if (*pp_vprop)
        {
            *pp_prop = (VF_PROP_T *)((*pp_vprop)->p_next_srch);

            if (*pp_prop)
            {
                ret = TRUE;
            }
        }
    }

    return ret;
}




/*---------------------------------------------------------------------------*
 * NAME
 *      vf_get_next_object()
 * 
 * DESCRIPTION
 *      Find "next" vobject.
 *
 * RETURNS
 *      TRUE iff found next object.
 *---------------------------------------------------------------------------*/

bool_t vf_get_next_object(
    VF_OBJECT_T **pp_object
    )
{
    bool_t ret = FALSE;

    if (pp_object)
    {
        if (*pp_object)
        {
            *pp_object = (VF_OBJECT_T *)(((VOBJECT_T *)(*pp_object))->p_next);

            if (*pp_object)
            {
                ret = TRUE;
            }
        }
    }

    return ret;
}
/****************************************************************************/
//  Description: vf_get_object_version
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
uint32_t vf_get_object_version(VF_OBJECT_T *pp_object)
{
    uint32_t ver=0;
    VOBJECT_T * object=NULL;
    if(pp_object!=NULL)
    {
        object=(VOBJECT_T *)pp_object;
        ver=object->version;
    }

    return ver;
}
#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT



/*===========================================================================*
 Private Function Implementations
 *===========================================================================*/
/* None */

/*===========================================================================*
 End Of File
 *===========================================================================*/
