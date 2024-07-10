/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_access.c $
    $Revision: 1.4 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Utility functions handling string arrays - the VSTRARRAY_T type.

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_string_arrays.c,v $
 *  Revision 1.4  2002/11/03 18:43:16  tilda
 *  IID619851 - Update and check headers and function prototypes.
 *
 *  Revision 1.3  2002/11/02 18:29:26  tilda
 *  IID485157 - UI does character conversion based on CHARSET property.
 *
 *  Revision 1.2  2002/10/29 07:19:20  tilda
 *  Tidy headers.
 *
 *  Revision 1.1  2002/10/26 15:57:11  tilda
 *  Initial Version
 * 
 *******************************************************************************/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_string_arrays_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_string_arrays.c,v 1.4 2002/11/03 18:43:16 tilda Exp $";
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
#include "vf_strings.h"
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
/* None */

/*============================================================================*
 Private Data
 *============================================================================*/
/* None */

/*============================================================================*
 Public Function Implementations
 *============================================================================*/


/*----------------------------------------------------------------------------*
 * NAME
 *      string_array_contains_string()
 * 
 * DESCRIPTION
 *      Checks the strings in the indicated array to see if it contains a
 *      particulr value.  Can check a particular index or the whole array.
 *      Can perform an exact match or check wether one of the strings simply
 *      contains the value.
 *
 * RETURNS
 *      TRUE <=> includes indicated value, FALSE else.
 *----------------------------------------------------------------------------*/

bool_t string_array_contains_string(
    VSTRARRAY_T *p_strarray,        /* String array */
    char **pp_string_found,         /* String found */
    uint32_t index,                 /* Which entry, (-1) => any */
    const char *p_string,           /* The string we're looking for */
    bool_t exact                    /* Exact or partial match */
    )
{
    bool_t ret = FALSE;
    uint32_t i;
    uint32_t s, e;

	if(NULL == p_strarray || NULL == p_strarray->pp_strings)
	{
		return ret;
	}
    if (index == (uint32_t)(-1))
    {
        s = 0;
        e = p_strarray->n_strings;
    }
    else
    {
        s = index;
        e = index + 1;
    }

    for (i = s;!ret && (i < e);i++)
    {
        if ((i < p_strarray->n_strings) && p_strarray->pp_strings[i])
        {
            if (exact)
            {
                if (0 == p_stricmp(p_strarray->pp_strings[i], p_string))
                    ret = TRUE;
            }
            else
            {
                if (p_strstr(p_strarray->pp_strings[i], p_string))
                    ret = TRUE;
            }
        }

        if (ret && pp_string_found)
        {
            *pp_string_found = p_strarray->pp_strings[i];
        }
    }

    return ret;
}






/*----------------------------------------------------------------------------*
 * NAME
 *      add_string_to_array()
 * 
 * DESCRIPTION
 *      Append string to indicated array.
 *
 * RETURNS
 *      TRUE <=> allocation OK, FALSE else.
 *----------------------------------------------------------------------------*/

bool_t add_string_to_array(
    VSTRARRAY_T *p_strarray,        /* String array */
    const char *p_string            /* String to add */
    )
{
    char **pp_new;
    bool_t ret = FALSE;

    pp_new = vf_realloc(p_strarray->pp_strings, sizeof(char *) * (1 + p_strarray->n_strings));

    if (pp_new)
    {
        if (p_string)
        {
            uint32_t l;
            char *p_strcopy;

            l = p_strlen(p_string);

            p_strcopy = vf_malloc(1 + l);

            if (p_strcopy)
            {
                p_strcpy(p_strcopy, p_string);

                pp_new[p_strarray->n_strings] = p_strcopy;

                ret = TRUE;
            }
            else
            {
                vf_free(pp_new);
            }
        }
        else
        {
            pp_new[p_strarray->n_strings] = NULL;

            ret = TRUE;
        }

        if (ret)
        {
            p_strarray->n_strings += 1;
            p_strarray->pp_strings = pp_new;
        }
    }

    return ret;
}





/*----------------------------------------------------------------------------*
 * NAME
 *      free_string_array_contents()
 * 
 * DESCRIPTION
 *      Delete contents of a string array.  The structure itself is not free()d.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

void free_string_array_contents(
    VSTRARRAY_T *p_strarray         /* String array */
    )
{
    if (p_strarray && p_strarray->pp_strings)
    {
        uint32_t i;

        for (i = 0;i < p_strarray->n_strings;i++)
        {
            if (p_strarray->pp_strings[i])
            {
                vf_free(p_strarray->pp_strings[i]);
                p_strarray->pp_strings[i] = NULL;
            }
        }

        vf_free(p_strarray->pp_strings);
        p_strarray->pp_strings = NULL;

        p_strarray->n_strings = 0;
    }
}







/*----------------------------------------------------------------------------*
 * NAME
 *      append_to_curr_string()
 * 
 * DESCRIPTION
 *      Append characters to the current string in a string array.
 *
 * RETURNS
 *      TRUE <=> allocation OK, FALSE else.
 *----------------------------------------------------------------------------*/

bool_t append_to_curr_string(
    VSTRARRAY_T *p_strarray,        /* String array */
    uint32_t *p_length,             /* Pointer to length, NULL if zero terminated */
    const char *p_chars,            /* Characters to append */
    uint32_t numchars               /* Number of characters */
    )
{
    bool_t ret = TRUE;

    if (p_strarray && !p_strarray->pp_strings)
    {
        ret = add_string_to_array(p_strarray, "");
    }

    if (ret)
    {
		if (p_strarray)
        {
			if(NULL == p_strarray->pp_strings)
			{
				ret = FALSE;
			}
			else
			{
				ret = append_to_pointer(&(p_strarray->pp_strings[p_strarray->n_strings - 1]), p_length, p_chars, numchars);
			}
		}
    }

    return ret;
}







/*----------------------------------------------------------------------------*
 * NAME
 *      append_to_pointer()
 * 
 * DESCRIPTION
 *      Append characters to indicated pointer.  Handles both NULL terminated
 *      strings (for simple 7-bit values) and buffer/length pairs.  Passing
 *      the p_length field indicates that we're building up binary data.
 *
 * RETURNS
 *      TRUE <=> allocation OK, FALSE else.
 *----------------------------------------------------------------------------*/

bool_t append_to_pointer(
    char **pp_string,           /* String we're appending to */
    uint32_t *p_length,         /* Pointer to length, NULL if ZT */
    const char *p_chars,        /* Chars we're appending */
    int numchars                /* Number of chars we're appending */
    )
{
    bool_t ok = FALSE;

	if(p_chars == NULL)
	{
		return ok;
	}
    if (pp_string)
    {
        int newlen, currlen;
        char *p_new;

        newlen = numchars;

        if (*pp_string)
        {
            currlen = p_length ? *p_length : p_strlen(*pp_string);/*lint !e737 */

            newlen += currlen;
        }
        else
        {
            currlen = 0;
        }
        
        p_new = vf_realloc(*pp_string, newlen + (p_length ? 0 : 1));

        if (p_new)
        {
            p_memcpy(p_new + currlen, p_chars, numchars);

            if (p_length)
            {
                *p_length = newlen;
            }
            else
            {
                p_new[newlen] = '\0';
            }

            *pp_string = p_new;
            ok = TRUE;
        }
    }

    return ok;
}







/*----------------------------------------------------------------------------*
 * NAME
 *      set_string_array_entry()
 * 
 * DESCRIPTION
 *      Set indicated entry in a string array.
 *
 * RETURNS
 *      TRUE <=> allocation OK, FALSE else.
 *----------------------------------------------------------------------------*/

bool_t set_string_array_entry(
    VSTRARRAY_T *p_strarray,        /* String array */
    const char *p_string,           /* String to insert */
    uint32_t n_string               /* Insertion point */
    )
{
    bool_t ret = FALSE;

	if(NULL == p_strarray)  //modify for coverity
	{
		return ret;
	}
    if(NULL == p_strarray->pp_strings)
    {
        return ret;
    }
    if (n_string < p_strarray->n_strings)
    {
        if (p_strarray->pp_strings[n_string])
        {
            vf_free(p_strarray->pp_strings[n_string]);
            p_strarray->pp_strings[n_string] = NULL;
        }

        if (p_string)
        {
            uint32_t len = p_strlen(p_string);

            p_strarray->pp_strings[n_string] = vf_malloc(1 + len);

            if (p_strarray->pp_strings[n_string])
            {
                p_strcpy(p_strarray->pp_strings[n_string], p_string);

                ret = TRUE;
            }                
        }
        else
        {
            /* NULL = "delete" */

            ret = TRUE;
        }
    }

    return ret;
}
#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
    
/*============================================================================*
 Private Function Implementations
 *============================================================================*/
/* None */

/*============================================================================*
 End Of File
 *============================================================================*/
