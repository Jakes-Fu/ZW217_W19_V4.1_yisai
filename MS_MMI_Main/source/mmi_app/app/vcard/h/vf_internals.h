/*******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_internals.h $
    $Revision: 1.15 $
    $Author: tilda $

ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Definition of the internal representation used for VOBJECTs.

REFERENCES
    (none)

MODIFICATION HISTORY
 *  $Log: vf_internals.h,v $
 *  Revision 1.15  2002/11/02 18:29:26  tilda
 *  IID485157 - UI does character conversion based on CHARSET property.
 *
 *  Revision 1.14  2002/11/02 08:56:17  tilda
 *  Start of internationalisation changes.
 *
 *  Revision 1.13  2002/10/26 16:09:24  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.12  2002/10/08 21:45:07  tilda
 *  IID620473 - reduce c-runtime dependencies.
 *
 *  Revision 1.11  2002/02/24 17:10:34  tilda
 *  Add API for "is modified" functionality.
 *
 *  Revision 1.10  2001/10/24 18:36:06  tilda
 *  BASE64 bugfixes.  Split reader/writer code. Start create/modify work.
 *
 *  Revision 1.9  2001/10/14 19:53:36  tilda
 *  Group handling.  NO group searching functions.
 *
 *  Revision 1.8  2001/10/14 16:40:04  tilda
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
 *******************************************************************************/

#ifndef _VF_INTERNALS_H_
#define _VF_INTERNALS_H_

#ifndef NORCSID
static const char vf_internals_h_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_internals.h,v 1.15 2002/11/02 18:29:26 tilda Exp $";
#endif

/*=============================================================================*
 Public Includes
 *============================================================================*/
/* None */

/*=============================================================================*
 Public Defines
 *============================================================================*/

#define VFP_BEGIN       "BEGIN"
#define VFP_END         "END"

/*=============================================================================*
 Public Types
 *============================================================================*/


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      VSTRARRAY_T encapsulates an array of strings.
 *----------------------------------------------------------------------------*/

typedef struct VSTRARRAY_T
{
    uint32_t            n_strings;          /* Then number of strings */
    char                **pp_strings;       /* The strings */
}
VSTRARRAY_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      VBINDATA_T encapsulates a chunk of binary data.  Yes, you could encode
 *      any and all vformat attributes in BASE64 but custom is to use BASE64
 *      for images etc.
 *----------------------------------------------------------------------------*/

typedef struct VBINDATA_T
{
    char                *p_buffer;          /* Binary data */
    uint32_t            n_bufsize;
}
VBINDATA_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      VOBJDATA_T 
 *----------------------------------------------------------------------------*/

typedef struct VOBJDATA_T
{
    struct VOBJECT_T    *p_object;          /* Another vformat object */
}
VOBJDATA_T;



/*----------------------------------------------------------------------------*
 * PURPOSE
 *      VPROPVALUE_T encapsulates the "value" half of a property.
 *----------------------------------------------------------------------------*/

typedef struct VPROPVALUE_T
{
    vf_encoding_t   encoding;

    struct
    {
        VSTRARRAY_T s;
        VBINDATA_T  b;
        VOBJDATA_T  o;
    }
    v;
}
VPROPVALUE_T;



/*----------------------------------------------------------------------------*
 * PURPOSE
 *      VPROP_T defines a single property.  It's an association of a name
 *      and value pair.  A vformat object is simply a list of properties.
 *      Associated with a property is (possibly) a group name.
 *----------------------------------------------------------------------------*/

typedef struct VPROP_T
{
    char                *p_group;       /* Group - we keep the A.B.C format */
    VSTRARRAY_T         name;           /* Name fields */
    VPROPVALUE_T        value;          /* Value fields */

    struct VPROP_T      *p_next;        /* Next property */
    struct VPROP_T      *p_next_srch;   /* Next in current search */

    bool_t              modified;       /* Property modified? */

    struct VOBJECT_T    *p_parent;      /* Owning object (if any) */
}
VPROP_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      VOBJECT_T defines a single vformat object.
 *----------------------------------------------------------------------------*/

typedef struct VOBJECT_T
{
    char                *p_type;        /* "VCARD" or "VCALENDAR" etc. */
    uint32_t            version;        /*v2.1 or v3.0*/
    VPROP_T             *p_props;       /* List of properties */

    bool_t              modified;       /* Object modified? */

    struct VOBJECT_T    *p_parent;      /* Owning object (if any) */
    struct VOBJECT_T    *p_next;        /* Next object (if any) */
}
VOBJECT_T;

typedef struct _QP_CONTENT_T
{
    uint8_t   opt;
    uint8_t    line_cnt;
	uint8_t   state;
	uint8_t   state2;
	uint8_t   remain;
	uint8_t   illege;
}
QP_CONTENT_T;

/*=============================================================================*
 Public Functions
 *============================================================================*/


/*---------------------------------------------------------------------------*
 * NAME
 *      delete_prop_contents()
 * 
 * DESCRIPTION
 *      Cleans up the memory used by the value associated with the property.
 *      The property is not deleted => can be a statically allocated variable.
 *
 * RETURNS
 *      (none)
 *---------------------------------------------------------------------------*/

extern void delete_prop_contents(
    VF_PROP_T *p_vprop,         /* The VF_PROP_T to clean */
    bool_t delname              /* Delete the name as well? */
    );


/*---------------------------------------------------------------------------*
 * NAME      vf_qprint_encode
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
            bool_t encode_crlf);

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
            bool_t encode_crlf);

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
            uint8_t opt);
/*=============================================================================*
 End of file
 *============================================================================*/

#endif /*_VF_INTERNALS_H_*/

