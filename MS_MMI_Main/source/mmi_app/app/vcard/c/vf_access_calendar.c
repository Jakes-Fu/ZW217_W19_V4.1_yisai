/****************************************************************************

    (C) Nick Marley, 2001 -

    This software is distributed under the GNU Lesser General Public Licence.
    Please read and understand the comments at the top of vf_iface.h before use!

FILE
    $Workfile: vf_access.c $
    $Revision: 1.5 $
    $Author: tilda $
         
ORIGINAL AUTHOR
    Nick Marley

DESCRIPTION
    Wrappers on the core access functions & other utility functions.

REFERENCES
    (none)    

MODIFICATION HISTORY
 *  $Log: vf_access_calendar.c,v $
 *  Revision 1.5  2002/11/03 18:43:16  tilda
 *  IID619851 - Update and check headers and function prototypes.
 *
 *  Revision 1.4  2002/10/26 16:09:24  tilda
 *  IID629125 - Ensure string functions used are portable.
 *
 *  Revision 1.3  2002/10/08 21:45:07  tilda
 *  IID620473 - reduce c-runtime dependencies.
 *
 *  Revision 1.2  2002/10/08 21:11:36  tilda
 *  Remove common.h.
 *
 *  Revision 1.1  2002/01/06 16:18:48  tilda
 *  Add dialog box for events / todos.
 *
 *****************************************************************************/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
#ifndef NORCSID
static const char vf_access_calendar_c_vss_id[] = "$Header: /cvsroot/vformat/src/vformat/src/vf_access_calendar.c,v 1.5 2002/11/03 18:43:16 tilda Exp $";
#endif

/*============================================================================*
 ANSI C & System-wide Header Files
 *============================================================================*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "types.h"

#include <time.h>
#include <stdio.h>
#include <ctype.h>

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

#define ISO8061_TOKEN_YEARS         'Y'
#define ISO8061_TOKEN_MONTHS        'M'
#define ISO8061_TOKEN_WEEKS         'W'
#define ISO8061_TOKEN_DAYS          'D'
#define ISO8061_TOKEN_HOURS         'H'
#define ISO8061_TOKEN_MINUTES       'M'
#define ISO8061_TOKEN_SECONDS       'S'

#define ISO8061_TOKEN_TIME          'T'
#define ISO8061_TOKEN_PERIOD        'P'

/*===========================================================================*
 Private Data Types
 *===========================================================================*/
/* None */

/*===========================================================================*
 Private Function Prototypes
 *===========================================================================*/

static void append_iso8061_value(char *p_string, uint32_t value, char cSymbol);
static void append_iso8061_tag(char *p_string, char cSymbol);

/*===========================================================================*
 Private Data
 *===========================================================================*/
/* None */

/*===========================================================================*
 Public Function Implementations
 *===========================================================================*/


/*---------------------------------------------------------------------------*
 * NAME
 *      vf_date_string_to_time()
 * 
 * DESCRIPTION
 *      Convert calendar string to absolute time.  The basic formats are
 *      19960401, 19960401T073000Z
 *
 * RETURNS
 *      TRUE <=> conversion OK, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t vf_date_string_to_time(
    uint32_t *p_time,               /* Output time value */
    const char *p_string            /* Input string */
    )
{
    bool_t ret = FALSE;

    if (p_string && p_time)
    {
        time_t t;
        struct tm tm;

        p_memset(&tm, '\0', sizeof(tm));

        /*
         * p_string[8] is either 'T' or '\0' or an error.
         */

        if (ISO8061_TOKEN_TIME == p_string[8])
        {
            if (6 == sscanf(p_string, "%4d%2d%2dT%2d%2d%2d",
                &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec))
            {
                if ((1970 <= tm.tm_year) &&
                    ((1 <= tm.tm_mon) && (tm.tm_mon <= 12)) &&
                    ((1 <= tm.tm_mday) && (tm.tm_mday <= 28)) &&
                    ((0 <= tm.tm_hour) && (tm.tm_hour <= 23)) &&
                    ((0 <= tm.tm_min) && (tm.tm_hour <= 59)) &&
                    ((0 <= tm.tm_sec) && (tm.tm_sec <= 59)))
                {
                    ret = TRUE;
                }
            }
        }
        else
        if ('\0' == p_string[8])
        {
            if (3 == sscanf(p_string, "%4d%2d%2d",
                &tm.tm_year, &tm.tm_mon, &tm.tm_mday))
            {
                if ((1970 <= tm.tm_year) &&
                    ((1 <= tm.tm_mon) && (tm.tm_mon <= 12)) &&
                    ((1 <= tm.tm_mday) && (tm.tm_mday <= 28)))
                {
                    ret = TRUE;
                }
            }
        }
        else
        {
            /* Nor flesh nor fish nor fowl */
        }

        /*
         * Reduce the year value to correct range & convert to time_t.
         */

        if (ret)
        {
            tm.tm_year -= 1900;
            tm.tm_isdst = -1;

            t = mktime(&tm);

            if ((-1) == (int)t)
            {
                ret = FALSE;
            }
            else
            {
                *p_time = (uint32_t)t;
            }
        }
    }

    return ret;
}




/*---------------------------------------------------------------------------*
 * NAME
 *      vf_period_string_to_time()
 * 
 * DESCRIPTION
 *      Convert period definition string to time value.  The format is
 *      P[aaaY][bbbM][cccW][dddD]T[eeeH][fffM][gggS] where 'aaa' is a
 *      number of years, bbb months etc.
 *
 * RETURNS
 *      TRUE <=> conversion OK, FALSE else.
 *---------------------------------------------------------------------------*/

bool_t vf_period_string_to_time(
    VF_ISO8601_PERIOD_T *p_period,  /* Output time value */
    const char *p_string            /* Input string */
    )
{
    bool_t ret = TRUE;
    bool_t in_time_part = FALSE;

    p_memset(p_period, '\0', sizeof(*p_period));

    if (p_string && ('P' == p_string[0]) && *(++p_string))
    {
        while (ret && *p_string)
        {
            uint32_t value = 0;

            //lint -esym(628, __rt_ctype_table)
            while (isdigit(*p_string)) /*lint !e718 !e48 !e746*/
            {
                value = (uint32_t)((value * 10) + *(p_string++) - '0');/*lint !e737 */
            }

            switch (*p_string++)
            {
            case '\0':
                {
                    ret = FALSE;
                }
                break;

            case ISO8061_TOKEN_YEARS:
                {
                    p_period->years = value;
                }
                break;

            case ISO8061_TOKEN_MONTHS:
            /*case ISO8061_TOKEN_MINUTES:*/
                {
                    if (in_time_part)
                    {
                        p_period->minutes = value;
                    }
                    else
                    {
                        p_period->months = value;
                    }
                }
                break;

            case ISO8061_TOKEN_WEEKS:
                {
                    p_period->weeks = value;
                }
                break;

            case ISO8061_TOKEN_DAYS:
                {
                    p_period->days = value;
                }
                break;

            case ISO8061_TOKEN_TIME:
                {
                    if (in_time_part)
                    {
                        ret = FALSE;
                    }
                    else
                    {
                        in_time_part = TRUE;
                    }
                }
                break;

            case ISO8061_TOKEN_HOURS:
                {
                    p_period->hours = value;
                }
                break;

            case ISO8061_TOKEN_SECONDS:
                {
                    p_period->seconds = value;
                }
                break;

            default:
                {
                    ret = FALSE;
                }
                break;
            }
        }
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}




/*---------------------------------------------------------------------------*
 * NAME
 *      vf_period_time_to_string()
 * 
 * DESCRIPTION
 *      Convert a VF_ISO8601_PERIOD_T to a string.
 *
 * RETURNS
 *      Number of characters written.
 *---------------------------------------------------------------------------*/

uint32_t vf_period_time_to_string(
    char *p_string,                     /* Input string */
    const VF_ISO8601_PERIOD_T *p_period /* Output time value */
    )
{
    uint32_t ret = 0;

    if (p_string && p_period)
    {
        p_string[0] = '\0';

        append_iso8061_tag(p_string, ISO8061_TOKEN_PERIOD);

        append_iso8061_value(p_string, p_period->years, ISO8061_TOKEN_YEARS);
        append_iso8061_value(p_string, p_period->months, ISO8061_TOKEN_MONTHS);
        append_iso8061_value(p_string, p_period->weeks, ISO8061_TOKEN_WEEKS);
        append_iso8061_value(p_string, p_period->days, ISO8061_TOKEN_DAYS);

        if (p_period->hours || p_period->minutes || p_period->seconds)
        {
            append_iso8061_tag(p_string, ISO8061_TOKEN_TIME);
        }

        append_iso8061_value(p_string, p_period->hours, ISO8061_TOKEN_HOURS);
        append_iso8061_value(p_string, p_period->minutes, ISO8061_TOKEN_MINUTES);
        append_iso8061_value(p_string, p_period->seconds, ISO8061_TOKEN_SECONDS);

        ret = p_strlen(p_string);
    }

    return ret;
}



/*===========================================================================*
 Private Function Implementations
 *===========================================================================*/



/*---------------------------------------------------------------------------*
 * NAME
 *      append_iso8061_value()
 * 
 * DESCRIPTION
 *      Append xxxY value to the current encoded string.
 *
 * RETURNS
 *      (none)
 *---------------------------------------------------------------------------*/

void append_iso8061_value(char *p_string, uint32_t value, char cSymbol)
{
    if (value)
    {
        sprintf(p_string + p_strlen(p_string), "%lu%c", value, cSymbol);
    }
}




/*---------------------------------------------------------------------------*
 * NAME
 *      append_iso8061_tag()
 * 
 * DESCRIPTION
 *      Append indicated character to string.
 *
 * RETURNS
 *      (none).
 *---------------------------------------------------------------------------*/

void append_iso8061_tag(char *p_string, char cSymbol)
{
    p_string += p_strlen(p_string);

    *p_string++ = cSymbol;
    *p_string++ = '\0';
}


/*===========================================================================*
 End Of File
 *===========================================================================*/
#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT


