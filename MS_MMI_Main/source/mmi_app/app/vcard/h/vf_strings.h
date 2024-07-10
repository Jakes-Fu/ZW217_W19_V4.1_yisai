/*******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile$
    $Revision: 1.6 $
    $Author: tilda $

ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    String handling functions.  The library uses the p_ functions (where hopefully
    the 'p' stands for 'portable') and they are provided here in terms of C runtime
    functions or implemented explicitly.

REFERENCES
    (none)

MODIFICATION HISTORY
 *  $Log: vf_strings.h,v $
 *  Revision 1.6  2002/10/26 16:09:23  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.5  2002/10/08 21:27:20  tilda
 *  Correct #endif directive.
 *
 *  Revision 1.4  2001/11/05 21:07:19  tilda
 *  Various changes for initial version of vfedit.
 *
 *  Revision 1.3  2001/10/14 20:42:37  tilda
 *  Addition of group searching.
 *
 *  Revision 1.2  2001/10/13 14:58:56  tilda
 *  Tidy up version headers, add vf_strings.h where needed.
 *
 *  Revision 1.1  2001/10/13 14:50:33  tilda
 *  Add string array code to unify handling of names / values.
 * 
 *******************************************************************************/

#ifndef _VF_STRINGS_H_
#define _VF_STRINGS_H_

#ifndef NORCSID
static const char vf_strings_h_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_strings.h,v 1.6 2002/10/26 16:09:23 tilda Exp $";
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
 *      p_strlen()
 * 
 * DESCRIPTION
 *      Find length of string.
 *
 * RETURNS
 *      Length of strings.
 *----------------------------------------------------------------------------*/

extern int p_strlen(
    const char *p_string                /* String to measure */
    );


/*----------------------------------------------------------------------------*
 * NAME
 *      p_strcpy()
 * 
 * DESCRIPTION
 *      Copy string to buffer.
 *
 * RETURNS
 *      Pointer to buffer.
 *----------------------------------------------------------------------------*/

extern char *p_strcpy(
    char *p_string1,                /* Buffer to copy to */
    const char *p_string2           /* String to copy */
    );


/*----------------------------------------------------------------------------*
 * NAME
 *      p_strcmp()
 * 
 * DESCRIPTION
 *      Case sensitive string comparison.
 *
 * RETURNS
 *      0<=>strings match, !=0 else.
 *----------------------------------------------------------------------------*/

extern int p_strcmp(
    const char *p_string1,
    const char *p_string2
    );


/*----------------------------------------------------------------------------*
 * NAME
 *      p_strcat()
 * 
 * DESCRIPTION
 *      Append one string to another.
 *
 * RETURNS
 *      Pointer to resulting string.
 *----------------------------------------------------------------------------*/

extern char *p_strcat(
    char *p_string1,                /* String to append to */
    const char *p_string2           /* String to append */
    );


/*----------------------------------------------------------------------------*
 * NAME
 *      p_strstr()
 * 
 * DESCRIPTION
 *      Case sensitive string searching function.
 *
 * RETURNS
 *      Pointer to position "looked for" has been located or NULL if !found.
 *----------------------------------------------------------------------------*/

extern char *p_strstr(
    const char *p_searched,             /* Buffer searched */
    const char *p_lookedfor             /* String we're looking for */
    );


/*----------------------------------------------------------------------------*
 * NAME
 *      p_stricmp()
 * 
 * DESCRIPTION
 *      Case insensitive string comparison function.
 *
 * RETURNS
 *      0<=>strings match, !=0 else.
 *----------------------------------------------------------------------------*/

extern int p_stricmp(
    const char *p_string1,              /* First string */
    const char *p_string2               /* Second string */
    );


/*----------------------------------------------------------------------------*
 * NAME
 *      p_stristr()
 * 
 * DESCRIPTION
 *      Case insensitive string searching function.
 *
 * RETURNS
 *      Pointer to position "looked for" has been located or NULL if !found.
 *----------------------------------------------------------------------------*/

extern char *p_stristr(
    const char *p_searched,             /* Buffer searched */
    const char *p_lookedfor             /* String we're looking for */
    );

/*----------------------------------------------------------------------------*
 * NAME
 *      p_memcpy()
 * 
 * DESCRIPTION
 *      Copy characetrs between buffers.  No checks on overlap.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

extern void p_memcpy(
    void *p_destination,            /* Pointer to buffer */
    const void *p_source,           /* Source of copy */
    uint32_t length                 /* Number of characters to copy */
    );


/*----------------------------------------------------------------------------*
 * NAME
 *      p_memset()
 * 
 * DESCRIPTION
 *      Fill buffer with indicated character.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

extern void p_memset(
    void *p_destination,            /* Pointer to buffer */
    uint8_t v,                      /* Character to fill with */
    uint32_t length                 /* Length of buffer to set */
    );


/*=============================================================================*
 End of file
 *============================================================================*/

#endif /*_VF_STRINGS_H_*/
