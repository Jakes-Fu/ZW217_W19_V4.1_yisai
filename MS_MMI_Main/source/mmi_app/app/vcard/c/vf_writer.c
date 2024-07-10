/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile$
    $Revision: 1.12 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley.

DESCRIPTION
    Code for writing vformat files.  This is temporary as it is not based
    around a state machine for converting the object to a character stream.

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_writer.c,v $
 *  Revision 1.12  2002/11/03 18:43:16  tilda
 *  IID619851 - Update and check headers and function prototypes.
 *
 *  Revision 1.11  2002/11/02 08:56:17  tilda
 *  Start of internationalisation changes.
 *
 *  Revision 1.10  2002/10/26 16:09:23  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.9  2002/10/08 21:45:07  tilda
 *  IID620473 - reduce c-runtime dependencies.
 *
 *  Revision 1.8  2002/10/08 21:11:35  tilda
 *  Remove common.h.
 *
 *  Revision 1.7  2002/02/24 17:10:34  tilda
 *  Add API for "is modified" functionality.
 *
 *  Revision 1.6  2001/12/13 06:45:35  tilda
 *  IID488021 - Various bugs with quoted printable format.
 *
 *  Revision 1.5  2001/11/18 21:45:10  tilda
 *  Add newline after BASE64 encodings
 *
 *  Revision 1.4  2001/11/14 22:36:55  tilda
 *  Add parameter to vf_find_prop_qual_index()
 *
 *  Revision 1.3  2001/11/06 22:51:05  tilda
 *  Supporting access functions for image selection / deletion.
 *
 *  Revision 1.2  2001/10/24 18:56:29  tilda
 *  Tidy headers after import.  Fix include path in release build.
 *
 *  Revision 1.1  2001/10/24 18:34:35  tilda
 *  Initial Version.
 *
 *******************************************************************************/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_writer_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_writer.c,v 1.12 2002/11/03 18:43:16 tilda Exp $";
#endif

/*=============================================================================*
 ANSI C & System-wide Header Files
 *=============================================================================*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "types.h"

#include <stdio.h>
#include <ctype.h>

/*============================================================================*
 Interface Header Files
 *============================================================================*/

#include "vf_iface.h"

/*============================================================================*
 Local Header File
 *============================================================================*/

#include "vf_config.h"
#include "vf_internals.h"
#include "vf_malloc.h"
#include "vf_strings.h"
#include <ctype.h>
/*============================================================================*
 Public Data
 *============================================================================*/
/* None */

/*============================================================================*
 Private Defines
 *============================================================================*/

#define QPMAXPERLINE        (76)
#define BASE64PERLINE       (64)

/*============================================================================*
 Private Data Types
 *============================================================================*/
/* None */

/*============================================================================*
 Private Function Prototypes
 *============================================================================*/

static bool_t write_name_fields(
    FILE *fp,
    uint32_t *p_charsonline,
    VPROP_T *p_prop
    );

static void write_base64_chars(
    FILE *fp,               /* File we're writing */
    uint8_t *s,             /* Pointer to buffer */
    uint32_t len            /* buffer length */
    );

static bool_t write_quoted_printable(
    FILE *fp,               /* File we're writing */
    uint8_t *s,             /* Pointer to buffer */
    uint32_t *p_charsonline /* Num on line so far */
    );

static char char_to_base64(
    uint8_t b               /* Byte to convert */
    );

static bool_t qp_needs_quoting(
    uint8_t c               /* Char to test */
    );

static bool_t write_vobject_to_file(
    FILE *fp,
    VOBJECT_T *p_object,
    bool_t write_all
    );

/*============================================================================*
 Private Data
 *============================================================================*/

static const char szEndOfLine[3] = { 0x0D, 0x0A, 0x00 };

/*============================================================================*
 Public Function Implementations
 *============================================================================*/




/*----------------------------------------------------------------------------*
 * NAME
 *      vf_write_file()
 * 
 * DESCRIPTION
 *      Write indicated vobject to file.
 *
 * RETURNS
 *      TRUE <=> written OK, FALSE else.
 *----------------------------------------------------------------------------*/

bool_t vf_write_file(
    const char *p_name,         /* Outpt filename */
    VF_OBJECT_T *p_object,      /* The object to write */
    bool_t write_all            /* Should write p_next as well? */
    )
{
    FILE *fp;
    bool_t ret = FALSE;

    fp = fopen(p_name, "wb");

    if (fp)
    {
        ret = write_vobject_to_file(fp, (VOBJECT_T *)p_object, write_all);

        if (0 == fclose(fp))
        {
            /* as is */
        }
        else
        {
            ret = FALSE;
        }
    }

    return ret;
}






/*============================================================================*
 Private Function Implementations
 *============================================================================*/


/*----------------------------------------------------------------------------*
 * NAME
 *      write_vobject_to_file()
 * 
 * DESCRIPTION
 *      Write each field of the indicated vformat object to the indicated FILE*
 *      invoking write_vobject_to_file() recursively if a field is found which
 *      contains a vformat object.
 *
 * RETURNS
 *      TRUE <=> file acces OK, FALSE else.
 *----------------------------------------------------------------------------*/

bool_t write_vobject_to_file(
    FILE *fp,
    VOBJECT_T *p_object,
    bool_t write_all
    )
{
    bool_t ret = TRUE;

    VOBJECT_T *p_obj = (VOBJECT_T *)p_object;
    VPROP_T *p_props = p_obj->p_props;

    fprintf(fp, "%s:%s", VFP_BEGIN, p_obj->p_type);
    
    fprintf(fp, szEndOfLine);

    for (;NULL != p_props;p_props = p_props->p_next)
    {
        uint32_t charsonline;

        switch (p_props->value.encoding)
        {
        default:
        case VF_ENC_7BIT:
            {
                ret = write_name_fields(fp, &charsonline, p_props);

                if (ret)
                {
                    uint32_t n;

                    for (n = 0;ret && (n < p_props->value.v.s.n_strings);n++)
                    {
                        if (n)
                            fprintf(fp, ";");

                        if (p_props->value.v.s.pp_strings[n])
                        {
                            fprintf(fp, "%s", p_props->value.v.s.pp_strings[n]);
                        }
                    }
                }
            }
            break;

        case VF_ENC_BASE64:
            {
                ret = write_name_fields(fp, &charsonline, p_props);

                if (ret)
                {
                    write_base64_chars(fp, (uint8_t *)p_props->value.v.b.p_buffer, p_props->value.v.b.n_bufsize);
                }
            }
            break;

        case VF_ENC_QUOTEDPRINTABLE:
            {
                ret = write_name_fields(fp, &charsonline, p_props);

                if (ret)
                {
                    uint32_t n;

                    for (n = 0;ret && (n < p_props->value.v.s.n_strings);n++)
                    {
                        if (n)
                            fprintf(fp, ";");

                        if (p_props->value.v.s.pp_strings[n])
                        {
                            ret = write_quoted_printable(fp, (uint8_t *)p_props->value.v.s.pp_strings[n], &charsonline);
                        }                    
                    }
                }
            }
            break;

        case VF_ENC_VOBJECT:
            {
                write_vobject_to_file(fp, p_props->value.v.o.p_object, TRUE);
            }
            break;
        }

        if (VF_ENC_VOBJECT != p_props->value.encoding)
            fprintf(fp, szEndOfLine);
    }

    fprintf(fp, "%s:%s", VFP_END, p_obj->p_type);

    fprintf(fp, szEndOfLine);

    if (ret && p_obj->p_next && write_all)
    {
        ret = write_vobject_to_file(fp, p_obj->p_next, TRUE);
    }

    p_obj->modified = !ret;

    return ret;
}






/*----------------------------------------------------------------------------*
 * NAME
 *      write_base64_chars()
 * 
 * DESCRIPTION
 *      Write the indicated binary data stream out to FILE* using BASE64
 *      encoding.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

static void write_base64_chars(
    FILE *fp,               /* File we're writing */
    uint8_t *p_buffer,      /* Pointer to buffer */
    uint32_t n_chars        /* buffer length */
    )
{
    uint32_t posn;
    char quad[5];
    uint8_t b;

    quad[4] = 0;

    for (posn = 0;posn < n_chars;)
    {
        int i;
	    uint32_t triplet = 0;
        uint8_t bits = 0;

        /* Form triplet from data if available */

	    for (i = 0;i < 3;i++, posn++)
        {
            triplet = triplet << 8;

            if (posn < n_chars)
            {
                triplet |= *(p_buffer + posn);

                bits += 8;
            }
        }

        /* Convert the triplet to text */

        for (i = 0;i < 4;i++)
        {
            b = (uint8_t)((0xFC0000 & triplet) >> 18);

            triplet = triplet << 6;

            quad[i] = (i <= (bits/6)) ? char_to_base64(b) : '=';
        }

        /* Output the text prefixed by newlines if necessary */

        if (((posn - 3) % (BASE64PERLINE / 4)) == 0)
        {
            fprintf(fp, szEndOfLine);
            fprintf(fp, "    ");
        }

        fprintf(fp, "%s", quad);
	}

    fprintf(fp, szEndOfLine);
}





/*----------------------------------------------------------------------------*
 * NAME
 *      write_quoted_printable()
 * 
 * DESCRIPTION
 *      Write the indicated field back to the file in quoted printable format.
 *
 * RETURNS
 *      (none)
 *----------------------------------------------------------------------------*/

static bool_t write_quoted_printable(
    FILE *fp,               /* File we're writing */
    uint8_t *s,             /* Pointer to buffer */
    uint32_t *p_charsonline /* Num on line so far */
    )
{
    uint32_t i;
    uint32_t charsonline = *p_charsonline;

    for (i = 0;s;i++)
    {
        uint8_t c = s[i];

        if ('\0' == c)
        {
            break;
        }
        else
        {
            if (3 + charsonline > QPMAXPERLINE)
            {
                fprintf(fp, "=");
                fprintf(fp, szEndOfLine);
                charsonline = 0;
            }

            if (qp_needs_quoting(c))
            {
                fprintf(fp, "=%02X", c);
                charsonline += 3;

                if (0x0D == c)
                {
                    fprintf(fp, "=");
                    fprintf(fp, szEndOfLine);
                    charsonline = 0;
                }
            }
            else
            {
                fprintf(fp, "%c", c);
                charsonline += 1;
            }
        }
    }

    *p_charsonline = charsonline;

    return TRUE;
}




/*----------------------------------------------------------------------------*
 * NAME
 *      char_to_base64()
 * 
 * DESCRIPTION
 *      Converts byte to BASE64.
 *
 * RETURNS
 *      Character encoded.
 *----------------------------------------------------------------------------*/

static char char_to_base64(
    uint8_t b       /* Byte to convert */
    )
{
    char ret;

    if (b < 26)
    {
        ret = (char)b + 'A';
    }
    else
    if (b < 52)
    {
        ret = (char)(b - 26) + 'a';
    }
    else
    if (b < 62)
    {
        ret = (char)(b - 52) + '0';
    }
    else
    if (b == 62)
    {
        ret = '+';
    }
    else
    {
        ret = '/';
    }

    return ret;
}




/*----------------------------------------------------------------------------*
 * NAME
 *      qp_needs_quoting()
 * 
 * DESCRIPTION
 *      Works out of 'c' needs quoting in quoted printable format.
 *
 * RETURNS
 *      TRUE <=> need to quote.
 *----------------------------------------------------------------------------*/

static bool_t qp_needs_quoting(
    uint8_t c               /* Char to test */
    )
{
    /* TBD - look in the RFC?! */

    bool_t needs_quoting = FALSE;

    switch (c)
    {
    case ' ':
    case ',':
    case '-':
    case '.':
    case '!':
    case '?':
    case '\'':
        return FALSE;

    default:
        if (isalnum(c)) /*lint !e718 !e48 !e746*/
            needs_quoting = FALSE;
        else
            needs_quoting = TRUE;
        break;
    }

    return needs_quoting;
}






/*----------------------------------------------------------------------------*
 * NAME
 *      write_name_fields()
 * 
 * DESCRIPTION
 *      Write name fields to file.
 *
 * RETURNS
 *      TRUE <=> OK so far.
 *----------------------------------------------------------------------------*/

bool_t write_name_fields(
    FILE *fp,
    uint32_t *p_charsonline,
    VPROP_T *p_prop
    )
{
    VSTRARRAY_T *p_strarray = &(p_prop->name);
    bool_t ret = FALSE;

    *p_charsonline = 0;

    if (p_prop->p_group)
    {
        *p_charsonline += (uint32_t)fprintf(fp, "%s.", p_prop->p_group);
    }

    if (p_strarray->pp_strings)
    {
        uint32_t i;

        for (i = 0;i < p_strarray->n_strings;i++)
        {
            char *p_name_field = p_strarray->pp_strings[i];

            if (p_name_field && (0 < p_strlen(p_name_field)))
            {
                if (0 < i)
                {
                    fprintf(fp, ";");
                    *p_charsonline += 1;
                }

                *p_charsonline += (uint32_t)fprintf(fp, "%s", p_name_field);
            }
        }

        fprintf(fp, ":");
        *p_charsonline += 1;

        ret = TRUE;
    }

    return ret;
}

#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
/*============================================================================*
 End Of File
 *============================================================================*/
