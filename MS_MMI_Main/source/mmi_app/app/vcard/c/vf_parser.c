/******************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_parser.c $
    $Revision: 1.20 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Parser for text stream in vobject format.

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_parser.c,v $
 *  Revision 1.20  2002/11/03 18:43:16  tilda
 *  IID619851 - Update and check headers and function prototypes.
 *
 *  Revision 1.19  2002/11/02 18:29:26  tilda
 *  IID485157 - UI does character conversion based on CHARSET property.
 *
 *  Revision 1.18  2002/11/02 08:56:17  tilda
 *  Start of internationalisation changes.
 *
 *  Revision 1.17  2002/10/26 16:09:23  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.16  2002/10/08 21:45:07  tilda
 *  IID620473 - reduce c-runtime dependencies.
 *
 *  Revision 1.15  2002/10/08 21:25:28  tilda
 *  Assignment of parent property.
 *
 *  Revision 1.14  2002/10/08 21:11:36  tilda
 *  Remove common.h.
 *
 *  Revision 1.13  2002/02/24 17:10:34  tilda
 *  Add API for "is modified" functionality.
 *
 *  Revision 1.12  2001/12/13 06:45:35  tilda
 *  IID488021 - Various bugs with quoted printable format.
 *
 *  Revision 1.11  2001/10/24 18:36:06  tilda
 *  BASE64 bugfixes.  Split reader/writer code. Start create/modify work.
 *
 *  Revision 1.10  2001/10/24 05:32:19  tilda
 *  BASE64 bugfixes - first part
 *
 *  Revision 1.9  2001/10/14 20:42:37  tilda
 *  Addition of group searching.
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
 *****************************************************************************/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_parser_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_parser.c,v 1.20 2002/11/03 18:43:16 tilda Exp $";
#endif

/*============================================================================*
 ANSI C & System-wide Header Files
 *===========================================================================*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "types.h"

/*============================================================================*
 Interface Header Files
 *===========================================================================*/

#include "vf_iface.h"

/*============================================================================*
 Local Header File
 *===========================================================================*/

#include "vf_config.h"
#include "vf_malloc.h"
#include "vf_internals.h"
#include "vf_strings.h"
#include "vf_string_arrays.h"

/*============================================================================*
 Public Data
 *===========================================================================*/
/* None */

/*============================================================================*
 Private Defines
 *===========================================================================*/

/*
 * Significant characters.
 */
#define PERIOD      '.'
#define COLON       ':'
#define VF_SEMICOLON   ';'/*lint !e767*/
#define LINEFEED    '\n'
#define CRETURN     '\r'
#define TAB         '\t'
#define BACKSLASH   '\\'
#define SPACE       ' '
#define EQUALS      '='
#define COMMA       ','
#define HTAB        9
//#define PERIOD      '.'

/*
 * dec hex  oct char
 * 10  0x0A 012 LF    NL, line feed, new line, \n
 * 13  0x0D 015 CR    carriage return, \r
 *
 * => <CR><LF> = 0x0D 0x0A
 */

/*
 * States associated with propname:propvalue parsing.
 */
#define _VF_STATE_PROPNAME          (1)     /* reading name (including groupings) */
#define _VF_STATE_PROPNAMEESCAPE    (2)     /* escaped semicolon in name */
#define _VF_STATE_RFC822VALUE       (3)     /* basic */
#define _VF_STATE_RFC822VALUEFOLD   (4)     /* waiting for line fold (simple fields) */
#define _VF_STATE_QPIDLE            (5)     /* Non escaped position in QP */
#define _VF_STATE_QPIDLENL          (6)     /* Non escaped position in QP */
#define _VF_STATE_QPEQUALSC1        (7)     /* After = in QP */
#define _VF_STATE_QPEQUALSC2        (8)     /* After =X in QP */
#define _VF_STATE_BASE64            (9)     /* Reading BASE64 */

#define _VF_STATE_RFC822VALUEFOLD_1   (10)
#define _VF_STATE_RFC822VALUEFOLD_2   (11)
#define _VF_STATE_VALUEESCAPE      (12)

#define ISCRORNL(c)                 ((CRETURN == (c)) || (LINEFEED == (c)))

/*============================================================================*
 Private Data Types
 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *      Maintains state variables etc. for current VOBJECT_T parsing.
 *----------------------------------------------------------------------------*/

typedef struct
{
    int             state;              /* Main state variable */
    char            qpchar;             /* Workspace for QuotedPrintable decoder */
    char            *p_b64buf;          /* Workspace for BASE64 decoder */
    VOBJECT_T       **pp_root_object;   /* Pointer to the root */
    VOBJECT_T       *p_object;          /* Current position in tree */
    VPROP_T         prop;               /* Current property, copied into tree on completion */
}VPARSE_T;

/*============================================================================*
 Private Function Prototypes
 *===========================================================================*/

//static bool_t handle_end_of_line(
//    VPARSE_T *p_parse           /* Current parse state info */
//     );

static bool_t append_value_to_object(
    VPROP_T **pp_prop,          /* Pointer to the new property */
    VPARSE_T *p_parse           /* The property we're naming */
    );

static bool_t handle_base64_chars(
    VPARSE_T *p_parse,          /* The property value we're adding to */
    const char *p_chars,        /* Pointer to characters to add */
    int numchars                /* Number of characters */
    );

static vf_encoding_t deduce_encoding(
    VSTRARRAY_T *p_propname     /* Property name */
    );

static uint8_t base64_to_char(
    char c                      /* Character to convert */
    );

static bool_t is_hex_digit(
    char *p_nibble,             /* Output nibble value */
    char c                      /* Character read from file */
    );

static bool_t handle_value_complete(
    VPARSE_T *p_parse           /* Current parse state info */
    );

static bool_t alloc_next_object(
    VPARSE_T *p_parse,          /* Current parse state info */
    char *p_type                /* Type of object */
    );

static bool_t alloc_sub_object(
    VPARSE_T *p_parse,          /* Current parse state info */
    char *p_type                /* Type of object */
    );

static bool_t alloc_object(
    VOBJECT_T **pp_new,         /* Pointer to output pointer */
    VOBJECT_T *p_parent,        /* Parent of new node */
    char *p_type,               /* Type of new node */
    VPARSE_T *p_parse           /* Current parse info */
    );

static bool_t append_group_name(
    VPROP_T *p_prop            /* Property we're updating */
    );

/****************************************************************************/
//  Description: deduce encoding method for version3.0
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/

static vf_encoding_t deduce_encoding_v30(
    VSTRARRAY_T *p_propname     /* Property name */
    );
/*============================================================================*
 Private Data
 *===========================================================================*/
/* None */

/*============================================================================*
 Public Function Implementations
 *===========================================================================*/



/*----------------------------------------------------------------------------*
 * NAME
 *      vf_parse_init()
 * 
 * DESCRIPTION
 *      Initialise a parsing instance.
 *
 * RETURNS
 *      (none)
 *---------------------------------------------------------------------------*/

bool_t vf_parse_init(
    VF_PARSER_T **pp_parser,    /* The parser */
    VF_OBJECT_T **pp_object     /* The object we're parsing into */
    )
{
    bool_t ret = FALSE;

    if (pp_parser && pp_object)
    {
        VPARSE_T *p_parse = (VPARSE_T *)vf_malloc(sizeof(VPARSE_T));

        if (p_parse)
        {
            p_memset(p_parse, '\0', sizeof(VPARSE_T));

            p_parse->state = _VF_STATE_PROPNAME;
            p_parse->p_object = NULL;
            p_parse->pp_root_object = (VOBJECT_T **)pp_object;

            *pp_parser = (VF_PARSER_T *)p_parse;

            *pp_object = NULL;

            ret = TRUE;
        }
    }

    return ret;
}






/*----------------------------------------------------------------------------*
 * NAME
 *      vf_parse_text()
 * 
 * DESCRIPTION
 *      Parse indicated text into the object associated with the VPARSE_T.
 *
 * RETURNS
 *      TRUE <=> allocation & syntax OK, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t vf_parse_text(
    VF_PARSER_T *p_parser,      /* The parser */
    char *p_chars,              /* New characters to parse into object */
    int numchars                /* Number of new characters */
    )
{
    int i;
    bool_t ok;
    VPARSE_T *p_parse = (VPARSE_T *)p_parser;

    /*
     * Check pointer.
     */
    if (!p_parse)
    {
        return FALSE;
    }

    /*
     * Push each character through the state machine.
     */
    for (i = 0, ok = TRUE;ok && (i < numchars);i++)
    {
        char c = p_chars[i];
		
        switch (p_parse->state)
        {
        case _VF_STATE_RFC822VALUEFOLD:
            {
                if (ISCRORNL(c))
                {
                    /* Ignore */
                }
                else
                if (SPACE == c)
                {
                    ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1);

                    p_parse->state = _VF_STATE_RFC822VALUE;
                }
                else
                {
                    if(vf_is_end(p_parse))/*lint !e64*/
					{
						return TRUE;
					}
                    ok = handle_value_complete(p_parse);

                    p_parse->state = _VF_STATE_PROPNAME;
                }
            }
            if (_VF_STATE_PROPNAME == p_parse->state)
            {
                /* Fall through */
            }
            else
            {
                break;
            }

            /* Fall through */
            //lint -fallthrough

        case _VF_STATE_PROPNAME:
            {
                if (COLON == c)
                {
                    p_parse->prop.value.encoding = deduce_encoding(&p_parse->prop.name);

                    switch (p_parse->prop.value.encoding)
                    {
                    case VF_ENC_7BIT:
                        p_parse->state = _VF_STATE_RFC822VALUE;
                        break;

                    case VF_ENC_BASE64:
                        p_parse->state = _VF_STATE_BASE64;
                        break;

                    case VF_ENC_QUOTEDPRINTABLE:
                        p_parse->state = _VF_STATE_QPIDLE;
                        break;

                    default:
                        ok = FALSE;
                        break;
                    }
                }
                else
                if (BACKSLASH == c)
                {
                    p_parse->state = _VF_STATE_PROPNAMEESCAPE;
                }
                else
                if (ISCRORNL(c))
                {
                    /* ignore */

                    free_string_array_contents(&p_parse->prop.name);
                }
                else
                if (VF_SEMICOLON == c)
                {
                    ok = add_string_to_array(&p_parse->prop.name, "");
                }
                else
                if (PERIOD == c)
                {
                    ok = append_group_name(&p_parse->prop);
                }
                else
                {
                    ok = append_to_curr_string(&(p_parse->prop.name), NULL, &c, 1);
                }
            }
            break;

        case _VF_STATE_PROPNAMEESCAPE:
            {
                if (VF_SEMICOLON == c)
                {
                    ok = append_to_curr_string(&(p_parse->prop.name), NULL, &c, 1);
                }
                else
                {
                    ok = FALSE;
                }
            }
            break;

        case _VF_STATE_RFC822VALUE:
            {
                if (p_parse->prop.value.v.s.pp_strings)
                {
                    /* Already allocated */
                }
                else
                {
                    ok = add_string_to_array(&(p_parse->prop.value.v.s), NULL);
                }

                if (ISCRORNL(c))
                {
                    p_parse->state = _VF_STATE_RFC822VALUEFOLD;
                }
                else if(BACKSLASH == c)
                {
                    if(i + 1 < numchars && (VF_SEMICOLON == p_chars[i + 1] || BACKSLASH == p_chars[i + 1]))
					{
						//parse "\;" -->";"
						i++;
						c = p_chars[i];
					}
					ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1); 
                    //p_parse->state = _VF_STATE_PROPNAMEESCAPE;
                }
				else
                if (VF_SEMICOLON == c)
                {
                    ok = add_string_to_array(&(p_parse->prop.value.v.s), NULL);
                }
                else
                {
                    ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1);
                }
            }
            break;

        case _VF_STATE_QPIDLENL:
            {
                if (ISCRORNL(c))
                {
                    break;
                }
                else
                {
                    if (EQUALS == c)
                    {
                        p_parse->qpchar = 0x00;
                        p_parse->state = _VF_STATE_QPEQUALSC1;
                        break;
                    } 
                    else
                    {
                        p_parse->state = _VF_STATE_RFC822VALUE;//_VF_STATE_QPIDLE;
                    }
                }
            }            
            //break;
        case _VF_STATE_QPIDLE:
            {
                if (p_parse->prop.value.v.s.pp_strings)
                {
                    /* Already allocated */
                }
                else
                {
                    ok = add_string_to_array(&(p_parse->prop.value.v.s), NULL);
                }

                if (EQUALS == c)
                {
                    p_parse->qpchar = 0x00;
                    p_parse->state = _VF_STATE_QPEQUALSC1;
                }
                else if(BACKSLASH == c)
                {
					//is_slash_first = TRUE;
                    if(i + 1 < numchars && (VF_SEMICOLON == p_chars[i + 1] || BACKSLASH == p_chars[i + 1]))
					{
						//parse "\;" -->";"
						i++;
						c = p_chars[i];
					}
					ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1); 

                    //p_parse->state = _VF_STATE_PROPNAMEESCAPE;
                }
				else
                if (VF_SEMICOLON == c)
                {
                    ok = add_string_to_array(&(p_parse->prop.value.v.s), NULL);
                }
                else
                if (ISCRORNL(c))
                {
                    if(vf_is_end(p_parse))/*lint !e64*/
					{
						return TRUE;
					}
                    ok = handle_value_complete(p_parse);
                }
                else
                {
                    ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1);
                }
            }
            break;

        case _VF_STATE_QPEQUALSC1:
            {
                uint8_t nibble;

                if (ISCRORNL(c))
                {
                    p_parse->state = _VF_STATE_QPIDLENL;
                }
                else
                if (is_hex_digit((char *)&nibble, c))
                {
                    (p_parse->qpchar) <<= 4;
                    (p_parse->qpchar) |= nibble;

                    p_parse->state = _VF_STATE_QPEQUALSC2;
                }
                else
                {
                    ok = FALSE;
                }
            }
            break;

        case _VF_STATE_QPEQUALSC2:
            {
                uint8_t nibble;

                if (is_hex_digit((char *)&nibble, c))
                {
                    (p_parse->qpchar) <<= 4;
                    (p_parse->qpchar) |= nibble;

					if(BACKSLASH == p_parse->qpchar)
					{
						if(i + 1 < numchars && (VF_SEMICOLON == p_chars[i + 1] || BACKSLASH == p_chars[i + 1]))
						{
							//parse "\;" -->";"
							i++;
							c = p_chars[i];
						}
						ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1); 
						//p_parse->state = _VF_STATE_PROPNAMEESCAPE;
					}
					else
					{
						ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &(p_parse->qpchar), 1);
					}
                    p_parse->state = _VF_STATE_QPIDLE;
                }
                else
                {
                    ok = FALSE;
                }
            }
            break;

        case _VF_STATE_BASE64:
            {
                /*
                 * The cr/nl stuff associated with line ends & termination of BASE64 encoding
                 * seems to be particularly problematic.  Searching and reading vCards from the
                 * internet shows that all sorts of wierd things are out there in use!  In the 
                 * interests of interoperability we look for the next value as an indication of
                 * the end of the object.  We build up each line and if it's still in BASE64
                 * format append the decoded data.  Otherwise we use the parser recursively to
                 * decode the buffered text, which is probably something like "NEXT-VALUE:"
                 */

                if (ISCRORNL(c))
                {
                    if (p_parse->p_b64buf)
                    {
                        ok = handle_base64_chars(p_parse, p_parse->p_b64buf, p_strlen(p_parse->p_b64buf));

                        vf_free(p_parse->p_b64buf);
                        p_parse->p_b64buf = NULL;
                    }
                }
                else
                {
                    ok = append_to_pointer(&(p_parse->p_b64buf), NULL, &c, 1);

                    if ((COLON == c) || (VF_SEMICOLON == c))
                    {
						if(vf_is_end(p_parse))/*lint !e64*/
						{
							ok = TRUE;
							vf_free(p_parse->p_b64buf);
							p_parse->p_b64buf = NULL;
							return ok;
						}
                        ok = handle_value_complete(p_parse);

                        ok = vf_parse_text(p_parser, p_parse->p_b64buf, p_strlen(p_parse->p_b64buf));

                        vf_free(p_parse->p_b64buf);
                        p_parse->p_b64buf = NULL;
                    }
                }
            }
            break;
        default:
            break;
        }

    }

    if (ok)
    {
        /* no need to panic */
    }
    else
    {
        vf_delete_object((VF_OBJECT_T *)*(p_parse->pp_root_object), TRUE);
        *(p_parse->pp_root_object) = NULL;
        p_parse->p_object = NULL;

        if (p_parse->p_b64buf)
        {
            vf_free(p_parse->p_b64buf);
            p_parse->p_b64buf = NULL;
        }

        delete_prop_contents((VF_PROP_T *)&p_parse->prop, TRUE);
    }

    return ok;
}


/****************************************************************************/
//  Description: Parses text for version3.0
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/

bool_t vf_parse_text_v30(
    VF_PARSER_T *p_parser,      /* The parser */
    char *p_chars,              /* New characters to parse into object */
    int numchars                /* Number of new characters */
    )
{
    int i;
    bool_t ok;
    VPARSE_T *p_parse = (VPARSE_T *)p_parser;

    /*
     * Check pointer.
     */
    if (!p_parse)
    {
        return FALSE;
    }

    /*
     * Push each character through the state machine.
     */
    for (i = 0, ok = TRUE;ok && (i < numchars);i++)
    {
        char c = p_chars[i];
		
        switch (p_parse->state)
        {
        case _VF_STATE_RFC822VALUEFOLD_1:
            {
                if (LINEFEED==c)
                {
                    p_parse->state = _VF_STATE_RFC822VALUEFOLD_2;
                    if(vf_is_end(p_parse))/*lint !e64*/
    				{
    					return TRUE;
    				}
                }
                else 
                {
                    if(vf_is_end(p_parse))/*lint !e64*/
    				{
    					return TRUE;
    				}
                    else
                    {
                       ok = FALSE; 
                    }
                }
  
            }
            break;

        case _VF_STATE_RFC822VALUEFOLD_2:
            {
                if (SPACE == c || HTAB==c)
                {
                    p_parse->state = _VF_STATE_RFC822VALUE;
                }
                else
                {
                    if(vf_is_end(p_parse))/*lint !e64*/
					{
						return TRUE;
					}
                    ok = handle_value_complete(p_parse);

                    p_parse->state = _VF_STATE_PROPNAME;
                }
            }
            if (_VF_STATE_PROPNAME == p_parse->state)
            {
                /* Fall through */
            }
            else
            {
                break;
            }

            /* Fall through */
            //lint -fallthrough
        case _VF_STATE_PROPNAME:
            {
                if (COLON == c)
                {
                    p_parse->prop.value.encoding = deduce_encoding_v30(&p_parse->prop.name);

                    switch (p_parse->prop.value.encoding)
                    {
                    case VF_ENC_8BIT:
                        p_parse->state = _VF_STATE_RFC822VALUE;
                        break;

                    case VF_ENC_BASE64:
                        p_parse->state = _VF_STATE_BASE64;
                        break;

                    default:
                        ok = FALSE;
                        break;
                    }
                }
                else
                if (BACKSLASH == c)
                {
                    p_parse->state = _VF_STATE_PROPNAMEESCAPE;
                }
                else
                if (ISCRORNL(c))
                {
                    /* ignore */

                    free_string_array_contents(&p_parse->prop.name);
                }
                else
                if (VF_SEMICOLON == c)
                {
                    ok = add_string_to_array(&p_parse->prop.name, "");
                }
                else
                if (PERIOD == c)
                {
                    ok = append_group_name(&p_parse->prop);
                }
                else
                {
                    ok = append_to_curr_string(&(p_parse->prop.name), NULL, &c, 1);
                }
            }
            break;

        case _VF_STATE_PROPNAMEESCAPE:
            {
                if (VF_SEMICOLON == c || COMMA==c)
                {
                    ok = append_to_curr_string(&(p_parse->prop.name), NULL, &c, 1);
                }
                else
                {
                    ok = FALSE;
                }
            }
            break;
       case _VF_STATE_VALUEESCAPE:
            {
                char tmp=0;
                if(VF_SEMICOLON == c|| BACKSLASH == c || COMMA == c)
                {
                    ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1);
                    p_parse->state = _VF_STATE_RFC822VALUE;
                }
                else if('n' == c || 'N' == c)
                {
                    tmp=CRETURN;
                    ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &tmp, 1);
                    tmp=LINEFEED;
                    ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &tmp, 1);
                    p_parse->state = _VF_STATE_RFC822VALUE;
                }
                else
                {
                    tmp=BACKSLASH;
                    ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &tmp, 1);
                    
                    if(CRETURN==c)
                    {
                        p_parse->state = _VF_STATE_RFC822VALUEFOLD_1;
                    }
                    else
                    {
                        p_parse->state = _VF_STATE_RFC822VALUE;
                        ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1);
                    }
                }
                  
            }
            break;

        case _VF_STATE_RFC822VALUE:
            {
                if (p_parse->prop.value.v.s.pp_strings)
                {
                    /* Already allocated */
                }
                else
                {
                    ok = add_string_to_array(&(p_parse->prop.value.v.s), NULL);
                }

                if (CRETURN==c)
                {
                    p_parse->state = _VF_STATE_RFC822VALUEFOLD_1;
                }
                else if(BACKSLASH == c)
                {
                    p_parse->state = _VF_STATE_VALUEESCAPE;
                }
				else
                if (VF_SEMICOLON == c || COMMA == c)
                {
                    ok = add_string_to_array(&(p_parse->prop.value.v.s), NULL);
                }
                else
                {
                    ok = append_to_curr_string(&(p_parse->prop.value.v.s), NULL, &c, 1);
                }
            }
            break;

        case _VF_STATE_BASE64:
            {
                /*
                 * The cr/nl stuff associated with line ends & termination of BASE64 encoding
                 * seems to be particularly problematic.  Searching and reading vCards from the
                 * internet shows that all sorts of wierd things are out there in use!  In the 
                 * interests of interoperability we look for the next value as an indication of
                 * the end of the object.  We build up each line and if it's still in BASE64
                 * format append the decoded data.  Otherwise we use the parser recursively to
                 * decode the buffered text, which is probably something like "NEXT-VALUE:"
                 */

                if (ISCRORNL(c))
                {
                    if (p_parse->p_b64buf)
                    {
                        ok = handle_base64_chars(p_parse, p_parse->p_b64buf, p_strlen(p_parse->p_b64buf));

                        vf_free(p_parse->p_b64buf);
                        p_parse->p_b64buf = NULL;
                    }
                }
                else
                {
                    ok = append_to_pointer(&(p_parse->p_b64buf), NULL, &c, 1);

                    if ((COLON == c) || (VF_SEMICOLON == c))
                    {
						if(vf_is_end(p_parse))/*lint !e64*/
						{
							ok = TRUE;
							vf_free(p_parse->p_b64buf);
							p_parse->p_b64buf = NULL;
							return ok;
						}
                        ok = handle_value_complete(p_parse);

                        ok = vf_parse_text_v30(p_parser, p_parse->p_b64buf, p_strlen(p_parse->p_b64buf));

                        vf_free(p_parse->p_b64buf);
                        p_parse->p_b64buf = NULL;
                    }
                }
            }
            break;
        default:
            break;
        }

    }

    if (ok)
    {
        /* no need to panic */
    }
    else
    {
        vf_delete_object((VF_OBJECT_T *)*(p_parse->pp_root_object), TRUE);
        *(p_parse->pp_root_object) = NULL;
        p_parse->p_object = NULL;

        if (p_parse->p_b64buf)
        {
            vf_free(p_parse->p_b64buf);
            p_parse->p_b64buf = NULL;
        }

        delete_prop_contents((VF_PROP_T *)&p_parse->prop, TRUE);
    }

    return ok;
}





/*----------------------------------------------------------------------------*
 * NAME
 *      vf_parse_end()
 * 
 * DESCRIPTION
 *      Ensure parse completion.
 *
 * RETURNS
 *      TRUE <=> allocation & syntax OK, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t vf_parse_end(
    VF_PARSER_T *p_parser       /* The parser */
    )
{
    bool_t ret = FALSE;
    VPARSE_T *p_parse = (VPARSE_T *)p_parser;

    /*
     * Check pointer.
     */
    if (p_parse)
    {
        ret = handle_value_complete(p_parse);

        vf_free(p_parse);
    }    

    return ret;
}

/*----------------------------------------------------------------------------*
 * NAME
 *      handle_value_complete()
 * 
 * DESCRIPTION
 *      Called when a complete value is parsed.
 *
 * RETURNS
 *      (none).
 *---------------------------------------------------------------------------*/

bool_t vf_is_end(
    VF_PARSER_T *p_parser          /* Current parse state info */
    )
{
    bool_t ret = FALSE;
    VOBJECT_T *p_object;
	VPARSE_T *p_parse = (VPARSE_T *)p_parser;

    /*
     * If we've got no object open we ignore values until we read a BEGIN at which point
     * we create a vformat object and start to parse properties into it.  If we subsequently
     * read a BEGIN we add a property & parse the next vobject into that.  Each time we read
     * an END we pop the pp_object pointer back up to the owner of the object we're parsing
     * into.  There should be an easier way of doing this <G> ...
     */
    if(p_parse == NULL )
    {
        return FALSE;
    }

    p_object = p_parse->p_object;

    if (p_object)
    {

        if ((string_array_contains_string(&p_parse->prop.name, NULL, 0, VFP_END, TRUE))
            &&(p_parse->prop.value.v.s.pp_strings== NULL || 0!=p_stricmp(p_parse->prop.value.v.s.pp_strings[0], VF_NAME_VEVENT))
            &&(p_parse->prop.value.v.s.pp_strings== NULL || 0!=p_stricmp(p_parse->prop.value.v.s.pp_strings[0], VF_NAME_VTODO)))
        {

			ret = TRUE;
        }

    }

    return ret;
}


/*============================================================================*
 Private Functions
 *===========================================================================*/


/*----------------------------------------------------------------------------*
 * NAME
 *      append_group_name()
 * 
 * DESCRIPTION
 *      After reading a ".", we transfer the segment of name we've read (which
 *      must be the first string <G>) into the group name.  Keep the group name
 *      in one string of "." tokens for simplicity, so we may end up with a
 *      group called "X-PHONE-BOOK.ENTRY.1" etc.
 *
 * RETURNS
 *      TRUE <=> allocation OK.
 *---------------------------------------------------------------------------*/

bool_t append_group_name(
    VPROP_T *p_prop
    )
{
    bool_t ok = TRUE;

	if(p_prop == NULL)
	{
		return FALSE;
	}
    if (p_prop->p_group)
    {
        ok &= append_to_pointer(&p_prop->p_group, NULL, ".", 1);
    }

    if (ok && ( NULL != p_prop->name.pp_strings))
    {
        const char *p_string = p_prop->name.pp_strings[0];
        if(p_string == NULL)
		{
			return FALSE;
		}
        ok &= append_to_pointer(&p_prop->p_group, NULL, p_string, p_strlen(p_string));

        if (ok)
        {
            vf_free(p_prop->name.pp_strings[0]);
            p_prop->name.pp_strings[0] = NULL;
        }
    }

    return ok;
}





/*----------------------------------------------------------------------------*
 * NAME
 *      handle_value_complete()
 * 
 * DESCRIPTION
 *      Called when a complete value is parsed.
 *
 * RETURNS
 *      (none).
 *---------------------------------------------------------------------------*/

bool_t handle_value_complete(
    VPARSE_T *p_parse          /* Current parse state info */
    )
{
    bool_t ret = TRUE;
    VOBJECT_T *p_object;

    /*
     * If we've got no object open we ignore values until we read a BEGIN at which point
     * we create a vformat object and start to parse properties into it.  If we subsequently
     * read a BEGIN we add a property & parse the next vobject into that.  Each time we read
     * an END we pop the pp_object pointer back up to the owner of the object we're parsing
     * into.  There should be an easier way of doing this <G> ...
     */
    if(p_parse == NULL )
    {
        return FALSE;
    }

    p_object = p_parse->p_object;

    if (p_object)
    {
        if ((string_array_contains_string(&p_parse->prop.name, NULL, 0, VFP_BEGIN, TRUE))
            &&(p_parse->prop.value.v.s.pp_strings== NULL || 0!=p_stricmp(p_parse->prop.value.v.s.pp_strings[0], VF_NAME_VEVENT))
            &&(p_parse->prop.value.v.s.pp_strings== NULL || 0!=p_stricmp(p_parse->prop.value.v.s.pp_strings[0], VF_NAME_VTODO)))
        {
            char *p_type;

            p_type = p_parse->prop.value.v.s.pp_strings[0];
            p_parse->prop.value.v.s.pp_strings[0] = NULL;

            delete_prop_contents((VF_PROP_T *)(&(p_parse->prop)), TRUE);

            p_parse->prop.value.encoding = VF_ENC_VOBJECT;

            ret = alloc_sub_object(p_parse, p_type);
        }
        else
        if ((string_array_contains_string(&p_parse->prop.name, NULL, 0, VFP_END, TRUE))
            &&(p_parse->prop.value.v.s.pp_strings== NULL || 0!=p_stricmp(p_parse->prop.value.v.s.pp_strings[0], VF_NAME_VEVENT))
            &&(p_parse->prop.value.v.s.pp_strings== NULL || 0!=p_stricmp(p_parse->prop.value.v.s.pp_strings[0], VF_NAME_VTODO)))
        {
            delete_prop_contents((VF_PROP_T *)(&(p_parse->prop)), TRUE);

            p_parse->p_object = p_parse->p_object->p_parent;
        }
        else
        {
            ret = append_value_to_object(NULL, p_parse);
        }
    }
    else
    {
        if (string_array_contains_string(&p_parse->prop.name, NULL, 0, VFP_BEGIN, TRUE))
        {
            char *p_type = NULL;
            if(p_parse->prop.value.v.s.pp_strings!= NULL)
			{
				p_type = p_parse->prop.value.v.s.pp_strings[0];
				p_parse->prop.value.v.s.pp_strings[0] = NULL;
			}

            ret = alloc_next_object(p_parse, p_type);
        }

        delete_prop_contents((VF_PROP_T *)(&(p_parse->prop)), TRUE);
    }

    p_parse->state = _VF_STATE_PROPNAME;

    return ret;
}






/*----------------------------------------------------------------------------*
 * NAME
 *      alloc_next_object()
 * 
 * DESCRIPTION
 *      Allocate the "next" object, ie. annother vformat tagged to the p_next
 *      pointer to the current one.
 *
 * RETURNS
 *      TRUE iff memory allocated OK, FALSE else OK.
 *---------------------------------------------------------------------------*/

bool_t alloc_next_object(
    VPARSE_T *p_parse,          /* Current parse state info */
    char *p_type                /* Type of object */
    )
{
    bool_t ok = TRUE;
    VOBJECT_T *p_parent = p_parse->p_object;
    VOBJECT_T *p_new = NULL;

// 	if(p_parse == NULL)
// 	{
// 		return FALSE;
// 	}
    ok = alloc_object(&p_new, p_parent ? p_parent->p_parent : NULL, p_type, p_parse);

    if (ok)
    {
        if (*p_parse->pp_root_object)
        {
            /* root object already set */

            if (p_new->p_parent)
            {
                /* Owned by property */
            }
            else
            {
                /* Need to tag to end of top list */

                VOBJECT_T **pp_tmp = p_parse->pp_root_object;

                while (*pp_tmp)
                {
                    pp_tmp = &((*pp_tmp)->p_next);
                }

                *pp_tmp = p_new;
            }
        }
        else
        {
            *p_parse->pp_root_object = p_new;
        }
    }

    return ok;
}







/*----------------------------------------------------------------------------*
 * NAME
 *      alloc_sub_object()
 * 
 * DESCRIPTION
 *      Allocate "sub" object, ie. annother vformat tagged to the p_object
 *      pointer of a VPROP_T allocated to contain it.
 *
 * RETURNS
 *      TRUE iff memory allocated OK, FALSE else OK.
 *---------------------------------------------------------------------------*/

bool_t alloc_sub_object(
    VPARSE_T *p_parse,          /* Current parse state info */
    char *p_type                /* Type of object */
    )
{
    bool_t ok = FALSE;
    VOBJECT_T *p_parent = NULL;

	p_parent = p_parse->p_object;
    if (p_parent)
    {
        VPROP_T *p_tmp;

        ok = append_value_to_object(&p_tmp, p_parse);

        if (ok && p_tmp)
        {
            ok = alloc_object(&p_tmp->value.v.o.p_object, p_parent, p_type, p_parse);
        }
    }

    return ok;
}






/*----------------------------------------------------------------------------*
 * NAME
 *      alloc_object()
 * 
 * DESCRIPTION
 *      Allocate & initialise a new VOBJECT_T.  The new element od made the
 *      current parse target (ie. new properties will be added to it).
 *
 * RETURNS
 *      TRUE <=> allocation went OK, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t alloc_object(
    VOBJECT_T **pp_new,     /* Pointer to output pointer */
    VOBJECT_T *p_parent,    /* Parent of new node */
    char *p_type,           /* Type of new node */
    VPARSE_T *p_parse       /* Current parse info */
    )
{
    bool_t ok = TRUE;
    VOBJECT_T *p_new;
    
	if(pp_new == NULL || p_parse == NULL)
	{
		return FALSE;
	}
    p_new = (VOBJECT_T *)vf_malloc(sizeof(VOBJECT_T));

    if (p_new)
    {
        p_memset(p_new, '\0', sizeof(VOBJECT_T));

        p_new->p_parent = p_parent;
        p_new->p_type = p_type;

        p_parse->p_object = p_new;

        *pp_new = p_new;
    }
    else
    {
        ok = FALSE;
    }

    return ok;
}







/*----------------------------------------------------------------------------*
 * NAME
 *      handle_base64_chars()
 * 
 * DESCRIPTION
 *      Handle BASE64 decoding.
 *
 * RETURNS
 *      TRUE <=> allocation OK, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t handle_base64_chars(
    VPARSE_T *p_parse,          /* The property value we're adding to */
    const char *p_chars,        /* Pointer to characters to add */
    int numchars                /* Number of characters */
    )
{
    bool_t ok = TRUE;

    /*
     * Skip spaces.
     */
	if(p_parse == NULL || p_chars == NULL)
	{
		return FALSE;
	}
    while ((0 < numchars) && (SPACE == p_chars[0]))
    {
        numchars--;
        p_chars++;
    }

    /*
     * Convert groups of 4 characters to byte triplets & append the bytes
     * to the binary data.
     */
    if (0 < numchars)
    {
        int i, j, len, num, left;
        const char *p_quad;

        len = numchars;
        num = len / 4;
        left = len % 4;

        for (i = 0, p_quad = p_chars;(i < num);i++, p_quad += 4)
        {
            uint32_t b;
            char bytes[3];
            uint8_t bits;

            for (j = 0, b = 0, bits = 0;(j < 4);j++)
            {
                b = (b << 6) | base64_to_char(p_quad[j]);

                if (EQUALS != p_quad[j])
                {
                    bits += 6;
                }
            }

            for (j = 0;(j < 3);j++)
            {
                bytes[2 - j] = (unsigned char)(b & 0xFF);

                b >>= 8;
            }

            ok = append_to_pointer(&(p_parse->prop.value.v.b.p_buffer), &(p_parse->prop.value.v.b.n_bufsize), bytes, bits / 8);
        }
    }

    return ok;
}






/*----------------------------------------------------------------------------*
 * NAME
 *      append_value_to_object()
 * 
 * DESCRIPTION
 *      Append characters to the name half of a VOBJECT_T.  The VOBJECT is
 *      allocated if not already present.
 *
 * RETURNS
 *      TRUE <=> allocation OK, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t append_value_to_object(
    VPROP_T **pp_prop,          /* Pointer to the new property */
    VPARSE_T *p_parse           /* The property we're naming */
    )
{
    bool_t ok = FALSE;

    VPROP_T **pp_tmp;
    VPROP_T *p_prop;

	if(p_parse == NULL ||p_parse->p_object == NULL)
	{
		return FALSE;
	}
    pp_tmp = &(p_parse->p_object->p_props);

    while (*pp_tmp)
    {
        pp_tmp = &((*pp_tmp)->p_next);
    }

    *pp_tmp = p_prop = (VPROP_T *)vf_malloc(sizeof(VPROP_T));

    if (pp_prop)
    {
        *pp_prop = p_prop;
    }

    if (p_prop)
    {
        p_memset(p_prop, '\0', sizeof(VPROP_T));

        *p_prop = p_parse->prop;

        p_memset(&(p_parse->prop), '\0', sizeof(VPROP_T));

        p_prop->p_parent = p_parse->p_object;

        ok = TRUE;
    }

    return ok;
}







/*----------------------------------------------------------------------------*
 * NAME
 *      deduce_encoding()
 * 
 * DESCRIPTION
 *      Check property name for encoding tags.  If none present, return the
 *      default encoding.
 *
 * RETURNS
 *      vf_encoding_t.
 *---------------------------------------------------------------------------*/

vf_encoding_t deduce_encoding(
    VSTRARRAY_T *p_propname     /* Property name */
    )
{
    vf_encoding_t ret = VF_ENC_7BIT;

	if(p_propname == NULL)
	{
		return FALSE;
	}
    if (string_array_contains_string(p_propname, NULL, (uint32_t)-1, VFP_QUOTEDPRINTABLE, FALSE))
    {
        ret = VF_ENC_QUOTEDPRINTABLE;
    }
    else
    if (string_array_contains_string(p_propname, NULL, (uint32_t)-1, VFP_BASE64, FALSE))
    {
        ret = VF_ENC_BASE64;
    }
//    else
//    if (string_array_contains_string(p_propname, NULL, (uint32_t)-1, VFP_8BIT, FALSE))
//    {
//        ret = VF_ENC_8BIT;
//    }

    return ret;
}


/****************************************************************************/
//  Description: deduce encoding method for version3.0
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/

static vf_encoding_t deduce_encoding_v30(
    VSTRARRAY_T *p_propname     /* Property name */
    )
{
    vf_encoding_t ret = VF_ENC_8BIT;

	if(p_propname == NULL)
	{
		return FALSE;
	}
    if (string_array_contains_string(p_propname, NULL, (uint32_t)-1, VFP_BASE64, FALSE))
    {
        ret = VF_ENC_BASE64;
    }

    return ret;
}




/*----------------------------------------------------------------------------*
 * NAME
 *      is_hex_digit()
 * 
 * DESCRIPTION
 *      
 *
 * RETURNS
 *      TRUE <=> valid HEX digit read.
 *---------------------------------------------------------------------------*/

bool_t is_hex_digit(
    char *p_nibble,     /* Output nibble value */
    char c              /* Character read from file */
    )
{
    bool_t ret = TRUE;

	if(p_nibble == NULL)
	{
		return FALSE;
	}
    if (('A' <= c) && (c <= 'F'))
    {
        *p_nibble = c - 'A' + 10;
    }
    else
    if (('a' <= c) && (c <= 'f'))
    {
        *p_nibble = c - 'a' + 10;
    }
    else
    if (('0' <= c) && (c <= '9'))
    {
        *p_nibble = c - '0';
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}





/*----------------------------------------------------------------------------*
 * NAME
 *      base64_to_char()
 * 
 * DESCRIPTION
 *      Convert base 64 char to byte.
 *
 * RETURNS
 *      Number corresponding to char.
 *---------------------------------------------------------------------------*/

uint8_t base64_to_char(char c)
{
    if (('A' <= c) && (c <= 'Z'))
        return c - 'A';

    if (('a' <= c) && (c <= 'z'))
        return c - 'a' + 26;

    if (('0' <= c) && (c <= '9'))
        return c - '0' + 52;

    if ('+' == c)
        return 62;

    if ('/' == c)
        return 63;

    return 0x00;
}
#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT

/*============================================================================*
 End Of File
 *===========================================================================*/
