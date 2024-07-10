/****************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Revision: 1.23 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Core access functions for VF_OBJECT_Ts.  The library includes other access
    functions (see vf_access_wrappers.c) which are often just alternative slightly
    higher level versions of what's found below.
    
    The intention is to provide a basic interface which covers all the required
    functionality and also some quick'n'easy functions which can be used but don't
    bloat the code (in a static library build anyway).

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_access.c,v $
 *  Revision 1.23  2002/11/03 18:43:16  tilda
 *  IID619851 - Update and check headers and function prototypes.
 *
 *  Revision 1.22  2002/11/02 18:29:26  tilda
 *  IID485157 - UI does character conversion based on CHARSET property.
 *
 *  Revision 1.21  2002/11/02 08:56:17  tilda
 *  Start of internationalisation changes.
 *
 *  Revision 1.20  2002/10/26 16:09:24  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.19  2002/10/08 21:45:06  tilda
 *  IID620473 - reduce c-runtime dependencies.
 *
 *  Revision 1.18  2002/10/08 21:11:36  tilda
 *  Remove common.h.
 *
 *  Revision 1.17  2002/02/24 17:10:34  tilda
 *  Add API for "is modified" functionality.
 *
 *  Revision 1.16  2002/01/06 16:18:48  tilda
 *  Add dialog box for events / todos.
 *
 *  Revision 1.15  2001/11/18 21:45:58  tilda
 *  Ensure parameters changed to QP are updated correctly.
 *
 *  Revision 1.14  2001/11/17 17:40:29  tilda
 *  Image / sound dialog box now works.
 *
 *  Revision 1.13  2001/11/05 21:07:20  tilda
 *  Various changes for initial version of vfedit.
 *
 *  Revision 1.12  2001/10/24 18:36:06  tilda
 *  BASE64 bugfixes.  Split reader/writer code. Start create/modify work.
 *
 *  Revision 1.11  2001/10/24 05:30:39  tilda
 *  Start work on object create/modify API.
 *
 *  Revision 1.10  2001/10/16 05:50:53  tilda
 *  Debug support for lists of vobjects from single file (ie. a phonebook).
 *
 *  Revision 1.9  2001/10/14 20:42:37  tilda
 *  Addition of group searching.
 *
 *  Revision 1.8  2001/10/14 16:40:05  tilda
 *  Initial testing of access functions.
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
 *****************************************************************************/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_access_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_access.c,v 1.23 2002/11/03 18:43:16 tilda Exp $";
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
#include "vf_modified.h"
#include <string.h>
/*===========================================================================*
 Public Data
 *===========================================================================*/
/* None */

/*===========================================================================*
 Private Defines
 *===========================================================================*/

#define MAXINCREMENT        (5)
#define MAXNUMTAGS          (10)
#define VF_QP_ENCODE_CRLF                 0x01
#define VF_QP_ENCODE_AUTO_LINEWRAP        0x02
#define VF_QP_ENCODE_SPACE_TO_UNDERLINE   0x04
#define VF_QP_DECODE_UNDERLINE_TO_SPACE   0x08
#define VF_QP_LINE_WRAP_COUNT     75
/* encode status */
#define VF_QP_ENCODE_STATE_INIT      0
#define VF_QP_ENCODE_STATE_LWSP      1
#define VF_QP_ENCODE_STATE_CR        2
#define VF_QP_ENCODE_STATE_LWSPCR    3

/* character macro */
#define VF_ISTAB(c)     ((c) == 9)
#define VF_ISSPACE(c)   ((c) == 32)
#define VF_ISCR(c)      ((c) == 13)
#define VF_ISLF(c)      ((c) == 10)




#define QP_NEED_ENCD_CRLF    \
            ((cntx->opt & VF_QP_ENCODE_CRLF) > 0)

#define QP_NEED_ENCD_SPACE   \
            ((cntx->opt & VF_QP_ENCODE_SPACE_TO_UNDERLINE) > 0)

#define QP_NEED_AUTO_WRAP    \
            ((cntx->opt & VF_QP_ENCODE_AUTO_LINEWRAP) > 0)


static const uint8_t VF_HEX_TABLE[] = "0123456789ABCDEF";

/*===========================================================================*
 Private Data Types
 *===========================================================================*/
/* None */

/*===========================================================================*
 Private Function Prototypes
 *===========================================================================*/
/*****************************************************************************
 * FUNCTION
 *  encode_state_init
 * DESCRIPTION
 * PARAMETERS
 *  cntx        [IN]     
 *  buf         [IN/OUT]
 *  c           [IN]
 * RETURNS
 *  length of encoded bytes
 *****************************************************************************/
static int encode_state_init(
							  QP_CONTENT_T* cntx,
                               char buf[],
					           char c
							   );

/*****************************************************************************
 * FUNCTION
 *  encode_state_cr
 * DESCRIPTION
 * PARAMETERS
 *  cntx        [IN]        
 *  buf         [IN/OUT]
 *  c           [IN]
 * RETURNS
 *  length of encoded bytes
 *****************************************************************************/
static int encode_state_cr(QP_CONTENT_T* cntx,
                                 char buf[],
								 char c);

/*****************************************************************************
 * FUNCTION
 *  encode_state_lwspcr
 * DESCRIPTION
 * PARAMETERS
 *  cntx        [IN]        
 *  buf         [IN/OUT]
 *  c           [IN]
 * RETURNS
 *  length of encoded bytes
 *****************************************************************************/
static int encode_state_lwspcr(QP_CONTENT_T* cntx,
                                 char buf[],
								 char c);

 /*****************************************************************************
 * FUNCTION
 *  encode_state_lwsp
 * DESCRIPTION
 * PARAMETERS
 *  cntx        [IN]        
 *  buf         [IN/OUT]
 *  c           [IN]
 * RETURNS
 *  length of encoded bytes
 *****************************************************************************/
static int encode_state_lwsp(QP_CONTENT_T* cntx,
                               char buf[],
							   char c);


/*****************************************************************************
 * FUNCTION
 *  applib_qprint_part_encode_append
 * DESCRIPTION
 *  This function accepts next part of source data; 
 *  encode them to Quote-Printable format
 * PARAMETERS
 *  cntx        [IN]        point to the applib_qprint_context
 *  src         [IN]        pointer to the input buffer
 *  srcl        [IN]        length of the input buffer
 *  dest        [IN/OUT]    pointer to the output buffer
 *  destl       [IN]        length of the output buffer
 * RETURNS
 *  negative:   the destination buffer is not enough.
 *  ow: the number of bytes written to output buffer.
 *****************************************************************************/
int vf_qprint_encode_append(
            QP_CONTENT_T* cntx,
            const char *src,
            int *srcl,
            char *dst,
             int dstl);


/*****************************************************************************
 * FUNCTION
 *  applib_qprint_part_encode_finish
 * DESCRIPTION
 *  This function finishes the part-by-part encoding process; 
 *  encode the last bytes into Quote-Printable format if there are.
 * PARAMETERS
 *  cntx        [IN]        point to the applib_qprint_context
 *  dest        [IN/OUT]    pointer to the destination buffer
 *  destl       [IN]        length of destination buffer
 * RETURNS
 *  negative:   the destination buffer is not enough.
 *  ow: the number of bytes written to output buffer.
 *****************************************************************************/
int vf_qprint_encode_finish(
            QP_CONTENT_T* cntx,
            char *dst,
             int dstl);

static bool_t strings_valid_index(
    VPROP_T *p_vprop,           /* The property */
    uint32_t n_string           /* Index required */
    );

static bool_t ensure_value_encoding_tag(
    VPROP_T *p_vprop,           /* The property */
    vf_encoding_t encoding      /* Encoding required */
    );

static bool_t set_prop_value_string(
    VPROP_T *p_vprop,           /* The property */
    uint32_t n_string,          /* Index required */
    const char *p_string        /* String required */
    );

static bool_t set_prop_value_base64(
    VPROP_T *p_vprop,           /* The property */
    const uint8_t *p_data,      /* Pointer to the binary data */
    uint32_t length,            /* Length of the binary data */
    bool_t copy                 /* Copy or keep pointer */
    );




/*===========================================================================*
 Private Data
 *===========================================================================*/
/* None */

/*===========================================================================*
 Public Function Implementations
 *===========================================================================*/


/*---------------------------------------------------------------------------*
 * NAME
 *      vf_get_prop_name()
 * 
 * DESCRIPTION
 *      Build the property name string in the indicated buffer.
 *
 * RETURNS
 *      (none)
 *---------------------------------------------------------------------------*/

void vf_get_prop_name(
    VF_PROP_T *p_prop,          /* The property */
    char *p_buffer,             /* The buffer */
    uint32_t bufsize            /* Size of the buffer */
    )
{
    VPROP_T *p_vprop = (VPROP_T *)p_prop;
    uint32_t i;

    if (p_vprop->p_group)
    {
        p_strcpy(p_buffer, p_vprop->p_group);
        p_strcat(p_buffer, ".");
    }
    else
    {
        p_buffer[0] = '\0';
    }

    for (i = 0;i < p_vprop->name.n_strings;i++)
    {
        if (0 < i)
            p_strcat(p_buffer, ";");

        if (p_vprop->name.pp_strings[i])
        {
            p_strcat(p_buffer, p_vprop->name.pp_strings[i]);
        }
    }
}



/*---------------------------------------------------------------------------*
 * NAME
 *      vf_get_object_type()
 * 
 * DESCRIPTION
 *      Return the type string identifying the indicated vformat object.
 *
 * RETURNS
 *      Ptr to string.
 *---------------------------------------------------------------------------*/

const char *vf_get_object_type(
    VF_OBJECT_T *p_object
    )
{
    const char *p_ret = NULL;

    if (p_object)
    {
        p_ret = ((VOBJECT_T *)p_object)->p_type;
    }

    return p_ret;
}






/*---------------------------------------------------------------------------*
 * NAME
 *      vf_get_prop_value()
 * 
 * DESCRIPTION
 *      Get hold of raw fields associated with the property.  These are of
 *      various types:
 *
 *        VF_ENC_VOBJECT
 *          - *pp_value = pointer to contained VF_OBJECT_T which can be
 *          passed back to any of the object manipulation functions.
 *
 *        VF_ENC_7BIT, VF_ENC_QUOTEDPRINTABLE
 *          - *pp_value = ptr to array of char*, *p_size = number of elts.
 *
 *        VF_ENC_8BIT, VF_BASE64
 *          - *pp_value = ptr to bytes, *p_size = number of bytes
 *
 * RETURNS
 *      TRUE <=> encoding is valid, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t vf_get_prop_value(
    VF_PROP_T *p_prop,          /* The property */
    void **pp_value,            /* Pointer value */
    uint32_t *p_size,           /* Integer value */
    vf_encoding_t *p_encoding   /* Type of return values */
    )
{
    VPROP_T *p_vprop = (VPROP_T *)p_prop;
    bool_t ret = TRUE;

    switch (p_vprop->value.encoding)
    {
    case VF_ENC_VOBJECT:
        if (pp_value)
        {
            *pp_value = p_vprop->value.v.o.p_object;
        }
        break;

    case VF_ENC_7BIT:
    case VF_ENC_QUOTEDPRINTABLE:
    case VF_ENC_8BIT:
        if (pp_value)
        {
            *pp_value = p_vprop->value.v.s.pp_strings;
        }
        if (p_size)
        {
            *p_size = p_vprop->value.v.s.n_strings;
        }
        break;

    //case VF_ENC_8BIT:
    case VF_ENC_BASE64:
        if (pp_value)
        {
            *pp_value = p_vprop->value.v.b.p_buffer;
        }
        if (p_size)
        {
            *p_size = p_vprop->value.v.b.n_bufsize;
        }
        break;

    default:
        ret = FALSE;
        break;
    }

    if (p_encoding)
    {
        *p_encoding = p_vprop->value.encoding;
    }

    return ret;
}







/*---------------------------------------------------------------------------*
 * NAME
 *      vf_set_prop_value()
 * 
 * DESCRIPTION
 *      Set values associated with a property.
 *
 *      Passing a value of encoding not the same as the current property
 *      encoding will cause the property contents to be freed prior to
 *      setting the indicated value.
 *
 * RETURNS
 *      TRUE <=> re-allocation success & encoding correct, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t vf_set_prop_value(
    VF_PROP_T *p_prop,          /* The property */
    void *p_value,              /* Pointer to the data */
    uint32_t n_param,           /* Data size or index */
    vf_encoding_t encoding,     /* Encoding in use */
    bool_t copy                 /* Copy the data? */
    )
{
    VPROP_T *p_vprop = (VPROP_T *)p_prop;
    bool_t ret = TRUE;

    if (!copy)
        return FALSE;

    if (encoding == p_vprop->value.encoding)
    {
        /* Leave it as is */
    }
    else
    {
        delete_prop_contents(p_prop, FALSE);

        ensure_value_encoding_tag(p_vprop, encoding);
    }

    switch (p_vprop->value.encoding)
    {
    case VF_ENC_VOBJECT:
        {
            if (p_vprop->value.v.o.p_object != p_value)
            {
                mark_property_modified(p_vprop, TRUE);
            }

            p_vprop->value.v.o.p_object = p_value;
        }
        break;

    case VF_ENC_7BIT:
    case VF_ENC_QUOTEDPRINTABLE:
        {
            ret = set_prop_value_string(p_vprop, n_param, p_value);
        }
        break;

    case VF_ENC_8BIT:
    case VF_ENC_BASE64:
        {
            ret = set_prop_value_base64(p_vprop, p_value, n_param, copy);
        }
        break;

    default:
        ret = FALSE;
        break;
    }

    return ret;
}







/*===========================================================================*
 Private Function Implementations
 *===========================================================================*/




/*---------------------------------------------------------------------------*
 * NAME
 *      set_prop_value_string()
 * 
 * DESCRIPTION
 *      Set the value of a property.
 *
 * RETURNS
 *      TRUE <=> set successfully.
 *---------------------------------------------------------------------------*/

bool_t set_prop_value_string(
    VPROP_T *p_vprop,
    uint32_t n_string,
    const char *p_string
    )
{
    bool_t ret = FALSE;

    if (strings_valid_index(p_vprop, n_string))
    {
        if ((p_vprop->value.v.s.pp_strings[n_string] && !p_string) ||
            (!p_vprop->value.v.s.pp_strings[n_string] && p_string) ||
            (p_vprop->value.v.s.pp_strings[n_string] && p_string && 
                p_stricmp(p_vprop->value.v.s.pp_strings[n_string], p_string)))
        {
            mark_property_modified(p_vprop, TRUE);
        }

        ret = set_string_array_entry(&(p_vprop->value.v.s), p_string, n_string);
    }

    return ret;
}




/*---------------------------------------------------------------------------*
 * NAME
 *      set_prop_value_base64()
 * 
 * DESCRIPTION
 *      Set the value of a property.
 *
 * RETURNS
 *      TRUE <=> set successfully.
 *---------------------------------------------------------------------------*/

bool_t set_prop_value_base64(
    VPROP_T *p_vprop,
    const uint8_t *p_data,      /* Pointer to the binary data */
    uint32_t length,            /* Length of the binary data */
    bool_t copy                 /* Copy or keep pointer */
    )
{
    bool_t ret = FALSE;

    if (copy)
    {
        p_vprop->value.v.b.p_buffer = vf_malloc(length);

        if (p_vprop->value.v.b.p_buffer)
        {
            p_memcpy(p_vprop->value.v.b.p_buffer, p_data, length);
            p_vprop->value.v.b.n_bufsize = length;

            ret = TRUE;
        }
    }
    else
    {
        /* TBD */
    }

    return ret;
}







/*---------------------------------------------------------------------------*
 * NAME
 *      strings_valid_index()
 * 
 * DESCRIPTION
 *      Check request for indicated string.  If it's either in the current
 *      object or represents a reasonable exapansion the request is allowed.
 *
 * RETURNS
 *      TRUE <=> object can include indicated string.
 *---------------------------------------------------------------------------*/

bool_t strings_valid_index(
    VPROP_T *p_vprop,
    uint32_t n_string
    )
{
    bool_t ret = FALSE;

    if (n_string < p_vprop->value.v.s.n_strings)
    {
        /* String in current extent of object */

        ret = TRUE;
    }
    else
    if ((p_vprop->value.v.s.n_strings <= n_string) && (n_string < p_vprop->value.v.s.n_strings + MAXINCREMENT))
    {
        /* Set string within reasonable expansion of object */

        void *p_tmp = vf_realloc(p_vprop->value.v.s.pp_strings, (1 + n_string) * sizeof(char *));

        if (p_tmp)
        {
            uint32_t i;

            p_vprop->value.v.s.pp_strings = p_tmp;

            for (i = p_vprop->value.v.s.n_strings;i < (uint32_t)(1 + n_string);i++)
            {
                p_vprop->value.v.s.pp_strings[i] = NULL;
            }

            p_vprop->value.v.s.n_strings = (1 + n_string);

            ret = TRUE;
        }
    }
    else
    {
        /* Invalid request */
    }

    return ret;
}





/*---------------------------------------------------------------------------*
 * NAME
 *      ensure_value_encoding_tag()
 * 
 * DESCRIPTION
 *      Check/set encoding.
 *
 * RETURNS
 *      TRUE <=> encoding was set successfully.
 *---------------------------------------------------------------------------*/

bool_t ensure_value_encoding_tag(
    VPROP_T *p_vprop,
    vf_encoding_t encoding
    )
{
    int  n;
    uint32_t i;
    bool_t ret = TRUE;

    /*
     * Locate the encoding value.
     */
    for (i = 0, n = (-1);n == (-1) && (i < p_vprop->name.n_strings);i++)
    {
        const char *p_string = p_vprop->name.pp_strings[i];

        if (p_string)
        {
            if (p_stristr(p_string, VFP_ENCODING)
                || p_stristr(p_string, VFP_QUOTEDPRINTABLE)
                || p_stristr(p_string, VFP_BASE64)
                || p_stristr(p_string, VFP_8BIT)
                || p_stristr(p_string, VFP_7BIT))
            {
                n = i;
            }
        }
    }

    /*
     * Remove previous encoding
     */
    if (0 <= n)
    {
        ret = set_string_array_entry(&(p_vprop->name), NULL, n);
    }

    if (ret)
    {
        char *p_enc_string;

        switch (encoding)
        {
        case VF_ENC_QUOTEDPRINTABLE:
            p_enc_string = VFP_QUOTEDPRINTABLE;
            break;

        case VF_ENC_BASE64:
            p_enc_string = VFP_BASE64;
            break;

        default:
            p_enc_string = NULL;
        }

        if (p_enc_string)
        {
            if ((-1) == n)
            {
                ret = add_string_to_array(&(p_vprop->name), p_enc_string);
            }
            else
            {
                ret = set_string_array_entry(&(p_vprop->name), p_enc_string, n);
            }
        }
    }

    if (ret)
    {
        p_vprop->value.encoding = encoding;
    }

    return ret;
}

/*---------------------------------------------------------------------------*
 * NAME      vf_vcard_qprint_encode
 * 
 * DESCRIPTION
 *      This function is used to encode string into QP encoded string.
 *
 *---------------------------------------------------------------------------*/
int vf_vcard_qprint_encode(
            uint8_t *src_ptr,
            uint32_t src_len,
            char *dest_ptr,
            uint32_t dest_len,
            bool_t encode_crlf)
{
	    uint32_t i = 0;
		uint32_t j =0;
		uint32_t ret = 0;
        uint32_t num_of_semicolon = 0;
		char   *new_str_buf = NULL;
		
        if(src_ptr == NULL || src_len == 0)
		{
			return -1;
		}
        while (i < src_len)
        {
            if (src_ptr[i] == ';' || src_ptr[i] == '\\')
            {
                num_of_semicolon++;
            }
            i++;
        }

        if (num_of_semicolon > 0)
        {
            new_str_buf = vf_malloc((src_len + num_of_semicolon + 1));
			if(NULL == new_str_buf)
			{
				return -1;
			}
            memset(new_str_buf, 0, ((src_len + num_of_semicolon + 1)));
           
            i = 0;
            j = 0;
            while (i < src_len)
            {
                if(src_ptr[i] == ';' || src_ptr[i] == '\\')
                {
                    new_str_buf[j] = '\\';
                    j++;
                }
                new_str_buf[j] = src_ptr[i];
                i++;
                j++;
            } 
			ret = vf_qprint_encode((uint8_t*)new_str_buf, strlen(new_str_buf), dest_ptr, dest_len, encode_crlf);
			vf_free(new_str_buf);			
		}
		else
		{
			ret = vf_qprint_encode(src_ptr, src_len, dest_ptr, dest_len, encode_crlf);
		}
        return ret;
}
/*---------------------------------------------------------------------------*
 * NAME      vf_qprint_encode
 * 
 * DESCRIPTION
 *      This function is used to encode string into QP encoded string.
 *
 * PARAMETERS
 *  src             [IN]            Source
 *  srcl            [IN]            Source length
 *  dest            [IN/OUT]        Dest buffer
 *  destl           [IN]            Destination buffer length
 *  encode_crlf     [IN]            TRUE:  "\r\n" ==> "=0D=0A"
 *  FALSE: "\r\n" ==> "\r\n"
 * RETURNS
 *  negative: error
 *  ow: length of QP encoded buffer
 *---------------------------------------------------------------------------*/
int vf_qprint_encode(
            uint8_t *src_ptr,
            uint32_t src_len,
            char *dest_ptr,
            uint32_t dest_len,
            bool_t encode_crlf)
{
    int ret = 0;
	int finish_len = 0;
    uint8_t opt = 0;
    QP_CONTENT_T cntx ={0};

	
    if (dest_ptr == NULL)
    {
		//no dest 
        return -1;
    }

    opt |= VF_QP_ENCODE_AUTO_LINEWRAP;
    if (encode_crlf)
    {
		//decode "\r\n" ==> "=0D=0A"
        opt |= VF_QP_ENCODE_CRLF;
    }
    //qp encode
	{
		if (src_len == 0)
		{
			//no source
			ret = 0;
		}
		else if (src_len > 0 && src_ptr == NULL)
		{
			//source error
			ret = -1;
		}
		else
		{
			//no error
			finish_len = src_len;
			//encode init
			vf_qprint_encode_init(&cntx,0,opt);
			//encode...
			ret = vf_qprint_encode_append(&cntx,(char*)src_ptr,&finish_len,dest_ptr,dest_len);
			//encode finished
			finish_len = vf_qprint_encode_finish(&cntx,dest_ptr + ret, (int)dest_len - ret);
			ret += finish_len;
			if (finish_len < 0)
			{
				ret =  -1;
			}
		}
	}
    if (ret <= 0 || ret >= (int)dest_len)
    {
		//encode error
        return -1;
    }

    dest_ptr[ret] = 0;
    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  applib_qprint_part_encode_init
 * DESCRIPTION
 *  This function initializes the STREAM-STYLE part by part quote-printable 
 *  encode process and calculate the needed output buffer size per-part.
 * PARAMETERS
 *  cntx            [IN]        point to the applib_qprint_context
 *  max_part_size   [IN]        the number of MAX input buffer size per-part.
 *  opt             [IN]        encode option
 * RETURNS
 *  negative:   invalid parameter.
 *  ow: the MIN output buffer size needed for the per-parts process.
 *****************************************************************************/
int vf_qprint_encode_init(
            QP_CONTENT_T* cntx,
            int max_size,
            uint8_t opt)
{
    int size;
    if (cntx == NULL)
    {
        return -1;
    }

    cntx->opt = opt;
    cntx->line_cnt = 0;
    cntx->state = VF_QP_ENCODE_STATE_INIT;

    if (max_size <= 0)
    {
        /* no need to cal the per-part output buffer size */
        return 0;
    }

    size = (max_size + 2) * 3;
    if (QP_NEED_AUTO_WRAP)
    {
        size += (size / VF_QP_LINE_WRAP_COUNT + 1) * 3;
    }
    return size;
}


/*****************************************************************************
 * FUNCTION
 *  applib_qprint_part_encode_append
 * DESCRIPTION
 *  This function accepts next part of source data; 
 *  encode them to Quote-Printable format
 * PARAMETERS
 *  cntx        [IN]        point to the applib_qprint_context
 *  src         [IN]        pointer to the input buffer
 *  srcl        [IN]        length of the input buffer
 *  dest        [IN/OUT]    pointer to the output buffer
 *  destl       [IN]        length of the output buffer
 * RETURNS
 *  negative:   the destination buffer is not enough.
 *  ow: the number of bytes written to output buffer.
 *****************************************************************************/
int vf_qprint_encode_append(
            QP_CONTENT_T* cntx,
            const char *src,
            int *srcl,
            char *dst,
            int dstl)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    char buf[8] = {0};
    int cnt;

    char  c;
    int i_input,i_output;

    char  pre_state;
    char  pre_lwsp;
    int pre_line_cnt;

    bool_t is_wrap_reached;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (cntx == NULL || 
        srcl == NULL || 
        (*srcl > 0 && src == NULL))
    {
        return -1;
    }

    if (cntx->line_cnt > VF_QP_LINE_WRAP_COUNT)
    {
        return -1;
    }

    i_input = i_output = 0;
    while(i_input < *srcl)
    {
        if(cntx->line_cnt > VF_QP_LINE_WRAP_COUNT)
		{
			break;
		}
        /* remember the pre-state to enable roll back */
        pre_lwsp = cntx->remain;
        pre_state = cntx->state;
        pre_line_cnt = cntx->line_cnt;

        /* encode to temp buf*/
        c = src[i_input++];
        switch (cntx->state)
        {
        case VF_QP_ENCODE_STATE_INIT:
            cnt = encode_state_init(cntx,buf,c);
            break;

        case VF_QP_ENCODE_STATE_LWSP:
            cnt = encode_state_lwsp(cntx,buf,c);
            break;

        case VF_QP_ENCODE_STATE_CR:
            cnt = encode_state_cr(cntx,buf,c);
            break;
            
        case VF_QP_ENCODE_STATE_LWSPCR:
            cnt = encode_state_lwspcr(cntx,buf,c);
            break;

        default:
            cnt = 0;
            break;
        }

        /* copy to dst and add hard line break if needed */
        if (cnt == 0)
        {
            /* No output data yet */
            continue;
        }
        
        if (dst == NULL)
        {
            return -1;
        }

        /* need to add soft line break ? */
        if (QP_NEED_AUTO_WRAP && 
            ((cntx->line_cnt + cnt) > VF_QP_LINE_WRAP_COUNT))
        {
            is_wrap_reached = TRUE;
        }
        else
        {
            is_wrap_reached = FALSE;
        }

        /* check if dst buffer is enough */
        if ((i_output + cnt + (is_wrap_reached ? 3 : 0)) > dstl)
        {
            /* roll back to pre-state */
            i_input--;
            cntx->state = pre_state;
            cntx->remain = pre_lwsp;
            cntx->line_cnt = pre_line_cnt;
            break;
        }
        
        if (is_wrap_reached)
        {
            /* add soft line break */
            dst[i_output++] = '=';
            dst[i_output++] = '\r';
            dst[i_output++] = '\n';
            cntx->line_cnt = 0;
        }
        
        /* copy temp buf data to dst */
		strncpy(dst+i_output,buf,cnt);
        i_output += cnt;
        if (QP_NEED_AUTO_WRAP)
        {
            cntx->line_cnt += cnt;
        }
    }

    *srcl = i_input;
    return i_output;
}

/*****************************************************************************
 * FUNCTION
 *  encode_state_init
 * DESCRIPTION
 * PARAMETERS
 *  cntx        [IN]        point to the applib_qprint_context
 *  buf         [IN/OUT]
 *  c           [IN]
 * RETURNS
 *  length of encoded bytes
 *****************************************************************************/
static int encode_state_init(QP_CONTENT_T* cntx,
                               char buf[],
					           char c
							   )
{
    int cnt = 0;

    if (VF_ISTAB(c))
    {
        if (QP_NEED_ENCD_CRLF)
        {
            buf[cnt++] = c;
            cntx->state = VF_QP_ENCODE_STATE_INIT;
            return cnt;
        }
        else
        {
            cntx->remain = c;
            cntx->state = VF_QP_ENCODE_STATE_LWSP;
            return 0;
        }        
    }

    if (VF_ISSPACE(c))
    {
        if (QP_NEED_ENCD_SPACE)
        {
            buf[cnt++] = '_';
            cntx->state = VF_QP_ENCODE_STATE_INIT;
            return cnt;
        }

        if (QP_NEED_ENCD_CRLF)
        {
            buf[cnt++] = c;
            cntx->state = VF_QP_ENCODE_STATE_INIT;
            return cnt;
        }
        else
        {
            cntx->remain = c;
            cntx->state = VF_QP_ENCODE_STATE_LWSP;
            return 0;
        }
    }

    if (VF_ISCR(c))
    {
        if (QP_NEED_ENCD_CRLF)
        {
            buf[cnt++] = '=';
            buf[cnt++] = VF_HEX_TABLE[(unsigned char)c >> 4];
            buf[cnt++] = VF_HEX_TABLE[(unsigned char)c & 0xF];
            cntx->state = VF_QP_ENCODE_STATE_INIT;
            return cnt;
        }
        else
        {
            cntx->state = VF_QP_ENCODE_STATE_LWSP;
            return cnt;
        }
    }

    if (VF_ISLF(c))
    {
        /* single LF without CR, take it as CONTROL character */
        buf[cnt++] = '=';
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)c >> 4];
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)c & 0xF];
        cntx->state = VF_QP_ENCODE_STATE_INIT;
        return cnt;
    }

    /* Others */
    if (c < 32 || c == 61 || c > 126 || (c == 95 && QP_NEED_ENCD_SPACE))
    {
        buf[cnt++] = '=';
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)c >> 4];
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)c & 0xF];
        cntx->state = VF_QP_ENCODE_STATE_INIT;
        return cnt;
    }
    else
    {
        buf[cnt++] = c;
        cntx->state = VF_QP_ENCODE_STATE_INIT;
        return cnt;
    }
}


/*****************************************************************************
 * FUNCTION
 *  encode_state_cr
 * DESCRIPTION
 * PARAMETERS
 *  cntx        [IN]        point to the applib_qprint_context
 *  buf         [IN/OUT]
 *  c           [IN]
 * RETURNS
 *  length of encoded bytes
 *****************************************************************************/
static int encode_state_cr(QP_CONTENT_T* cntx,
                                 char buf[],
								 char c)
{
    int cnt = 0;


    if (VF_ISLF(c))
    {
        buf[cnt++] = '\r';
        buf[cnt++] = '\n';

        if (QP_NEED_AUTO_WRAP)
        {
            cntx->line_cnt = -(cnt); /* new line */
        }

        cntx->state = VF_QP_ENCODE_STATE_INIT;
    }
    else
    {
        /*
         * Invalide! Single CR without LF.
         * take it as CONTROL character
         */
        buf[cnt++] = '=';
        buf[cnt++] = VF_HEX_TABLE['\r' >> 4];   /*lint !e572*//*lint !e778*/
        buf[cnt++] = VF_HEX_TABLE['\r' & 0xF];
        
        cntx->state = VF_QP_ENCODE_STATE_INIT;
        cnt += encode_state_init(cntx,buf+cnt,c);
    }
    return cnt;
}


/*****************************************************************************
 * FUNCTION
 *  encode_state_lwspcr
 * DESCRIPTION
 * PARAMETERS
 *  cntx        [IN]        point to the applib_qprint_context
 *  buf         [IN/OUT]
 *  c           [IN]
 * RETURNS
 *  length of encoded bytes
 *****************************************************************************/
static int encode_state_lwspcr(QP_CONTENT_T* cntx,
                                 char buf[],
								 char c)
{
    int cnt = 0;
    
    if (VF_ISLF(c))
    {
        /* check the line-wrap count here */
        if (QP_NEED_AUTO_WRAP &&
            ((cntx->line_cnt + 3) > VF_QP_LINE_WRAP_COUNT))
        {
            buf[cnt++] = '=';
            buf[cnt++] = '\r';
            buf[cnt++] = '\n';
        }
        
        buf[cnt++] = '=';
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)(cntx->remain) >> 4];
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)(cntx->remain) & 0xF];

        buf[cnt++] = '\r';
        buf[cnt++] = '\n';

        if (QP_NEED_AUTO_WRAP)
        {
            cntx->line_cnt = -(cnt); /* new line */
        }

        cntx->state = VF_QP_ENCODE_STATE_INIT;
    }
    else
    {
        /*
         * Invalide! Single CR without LF.
         * take it as CONTROL character
         */
        buf[cnt++] = cntx->remain;
        buf[cnt++] = '=';
        buf[cnt++] = VF_HEX_TABLE['\r' >> 4];/*lint !e572*//*lint !e778*/
        buf[cnt++] = VF_HEX_TABLE['\r' & 0xF];
        
        cntx->state = VF_QP_ENCODE_STATE_INIT;
        cnt += encode_state_init(cntx,buf+cnt,c);
    }
    
    return cnt;
}

/*****************************************************************************
 * FUNCTION
 *  applib_qprint_part_encode_finish
 * DESCRIPTION
 *  This function finishes the part-by-part encoding process; 
 *  encode the last bytes into Quote-Printable format if there are.
 * PARAMETERS
 *  cntx        [IN]        point to the applib_qprint_context
 *  dest        [IN/OUT]    pointer to the destination buffer
 *  destl       [IN]        length of destination buffer
 * RETURNS
 *  negative:   the destination buffer is not enough.
 *  ow: the number of bytes written to output buffer.
 *****************************************************************************/
 int vf_qprint_encode_finish(
            QP_CONTENT_T* cntx,
            char *dst,
            int dstl)
{
    int cnt,i_output;
    char  buf[8];
    bool_t  is_wrap_reached;

    if (cntx == NULL)
    {
        return -1;
    }

    if (QP_NEED_ENCD_CRLF || QP_NEED_ENCD_SPACE)
    {
        return 0;
    }

    /* cntx->encode_crlf == KAL_FALSE */
    cnt = 0;
    switch (cntx->state)
    {
        case VF_QP_ENCODE_STATE_INIT:
            /* all data had been processed before */
            return 0;

        case VF_QP_ENCODE_STATE_LWSP:
            /* the end character is a SPACE */
            buf[cnt++] = '=';
            buf[cnt++] = VF_HEX_TABLE[(unsigned char)(cntx->remain) >> 4];
            buf[cnt++] = VF_HEX_TABLE[(unsigned char)(cntx->remain) & 0xF];
            break;

        case VF_QP_ENCODE_STATE_CR:
            /* invalide, a single CR be at the end */
            buf[cnt++] = '=';
            buf[cnt++] = VF_HEX_TABLE['\r' >> 4];/*lint !e572*//*lint !e778*/
            buf[cnt++] = VF_HEX_TABLE['\r' & 0xF];
            break;

        case VF_QP_ENCODE_STATE_LWSPCR:
            /* invalide, a SPACE/TAB with a single CR be at the end */
            buf[cnt++] = '=';
            buf[cnt++] = VF_HEX_TABLE[(unsigned char)(cntx->remain) >> 4];
            buf[cnt++] = VF_HEX_TABLE[(unsigned char)(cntx->remain) & 0xF];
            buf[cnt++] = '=';
            buf[cnt++] = VF_HEX_TABLE['\r' >> 4];/*lint !e572*//*lint !e778*/
            buf[cnt++] = VF_HEX_TABLE['\r' & 0xF];
            break;

        default:
            /* Shouldn't happen */
            return -1;
    }

    /* need to add soft line break ? */
    if (QP_NEED_AUTO_WRAP && 
        ((cntx->line_cnt + cnt) > VF_QP_LINE_WRAP_COUNT))
    {
        is_wrap_reached = TRUE;
    }
    else
    {
        is_wrap_reached = FALSE;
    }
    
    /* check whether dst buffer is enough */
    if (dst == NULL || 
        ((cnt + (is_wrap_reached ? 3 : 0)) > dstl))
    {
        /* dst is not enough */
        return -1;
    }
    
    i_output = 0;
    if (is_wrap_reached)
    {
        /* add soft line break */
        dst[i_output++] = '=';
        dst[i_output++] = '\r';
        dst[i_output++] = '\n';
        cntx->line_cnt = 0;
    }
    
    /* copy temp buf data to dst */
    strncpy(dst+i_output,buf,cnt);
    i_output += cnt;
    if (QP_NEED_AUTO_WRAP)
    {
        cntx->line_cnt += cnt;
    }


    return i_output;
}

 /*****************************************************************************
 * FUNCTION
 *  encode_state_lwsp
 * DESCRIPTION
 * PARAMETERS
 *  cntx        [IN]        point to the applib_qprint_context
 *  buf         [IN/OUT]
 *  c           [IN]
 * RETURNS
 *  length of encoded bytes
 *****************************************************************************/
static int encode_state_lwsp(QP_CONTENT_T* cntx,
                               char buf[],
							   char c)
{
    int cnt = 0;

    if (VF_ISTAB(c))
    {
        buf[cnt++] = cntx->remain;
        cntx->remain = c;
        cntx->state = VF_QP_ENCODE_STATE_LWSP;
        return cnt;
    }

    if (VF_ISSPACE(c))
    {
        buf[cnt++] = cntx->remain;
        if (QP_NEED_ENCD_SPACE)
        {
            buf[cnt++] = '_';
            cntx->state = VF_QP_ENCODE_STATE_INIT;
            return cnt;
        }
        else
        {
            cntx->remain = c;
            cntx->state = VF_QP_ENCODE_STATE_LWSP;
            return cnt;
        }
    }

    if (VF_ISCR(c))
    {
        cntx->state = VF_QP_ENCODE_STATE_LWSPCR;
        return cnt;
    }

    if (VF_ISLF(c))
    {        
        /* single LF without CR take it as CONTROL character */
        buf[cnt++] = cntx->remain;

        buf[cnt++] = '=';
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)(c) >> 4];
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)(c) & 0xF];

        cntx->state = VF_QP_ENCODE_STATE_INIT; 
        return cnt;
    }

    /* Others */
    if (c < 32 || c == 61 || c > 126 || (c == 95 && QP_NEED_ENCD_SPACE))
    {
        buf[cnt++] = cntx->remain; /* NOT LINE_END SPACE / TAB */

        buf[cnt++] = '=';
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)c >> 4];
        buf[cnt++] = VF_HEX_TABLE[(unsigned char)c & 0xF];
        cntx->state = VF_QP_ENCODE_STATE_INIT;
        return cnt;
    }
    else
    {
        buf[cnt++] = cntx->remain; /* NOT LINE_END SPACE / TAB */

        buf[cnt++] = c;
        cntx->state = VF_QP_ENCODE_STATE_INIT;
        return cnt;
    }
}


/*===========================================================================*
 End Of File
 *===========================================================================*/
#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT


