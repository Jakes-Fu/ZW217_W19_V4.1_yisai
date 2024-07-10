/*******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_internals.h $
    $Revision: 1.2 $
    $Author: tilda $

ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Utility functions handling string arrays - the VSTRARRAY_T type.

REFERENCES
    (none)

MODIFICATION HISTORY
 *  $Log: vf_string_arrays.h,v $
 *  Revision 1.2  2002/11/02 18:29:26  tilda
 *  IID485157 - UI does character conversion based on CHARSET property.
 *
 *  Revision 1.1  2002/10/26 15:57:11  tilda
 *  Initial Version
 * 
 *******************************************************************************/

#ifndef _VF_STRING_ARRAYS_H_
#define _VF_STRING_ARRAYS_H_

#ifndef NORCSID
static const char vf_string_arrays_h_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_string_arrays.h,v 1.2 2002/11/02 18:29:26 tilda Exp $";
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

extern bool_t string_array_contains_string(
    VSTRARRAY_T *p_strarray,        /* String array */
    char **pp_string_found,         /* String found */
    uint32_t index,                 /* Which entry, (-1) => any */
    const char *p_string,           /* The string we're looking for */
    bool_t exact                    /* Exact or partial match */
    );


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

extern bool_t add_string_to_array(
    VSTRARRAY_T *p_strarray,        /* String array */
    const char *p_string            /* String to add */
    );


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

extern void free_string_array_contents(
    VSTRARRAY_T *p_strarray         /* String array */
    );


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

extern bool_t append_to_curr_string(
    VSTRARRAY_T *p_strarray,        /* String array */
    uint32_t *p_length,             /* Pointer to length, NULL if zero terminated */
    const char *p_chars,            /* Characters to append */
    uint32_t numchars               /* Number of characters */
    );


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

extern bool_t append_to_pointer(
    char **pp_string,               /* String we're appending to */
    uint32_t *p_length,             /* Pointer to length, NULL if ZT */
    const char *p_chars,            /* Chars we're appending */
    int numchars                    /* Number of chars we're appending */
    );


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

extern bool_t set_string_array_entry(
    VSTRARRAY_T *p_strarray,        /* String array */
    const char *p_string,           /* String to insert */
    uint32_t n_string               /* Insertion point */
    );


/*=============================================================================*
 End of file
 *============================================================================*/

#endif /*_VF_STRING_ARRAYS_H_*/
