/*lint -e18*/ 
/*lint -e716*/ 
#include "sci_types.h"
#include "atc_malloc.h"

#define ATC_SC_TO_UI(c) ((uint) (unsigned char) c)

/* Enter a start condition.  This macro really ought to take a parameter,
 * but we do it the disgusting crufty way forced on us by the ()-less
 * definition of BEGIN.
 */
#define BEGIN atc_start = 1 + 2 *

/* Action number for EOF rule of a given start state. */
#define ATC_STATE_EOF(state) (ATC_END_OF_BUFFER + state + 1)

int atcleng;

/* Points to current character in buffer. */
char *atc_c_buf_p = NULL;
static int atc_start = 1;

#define ATC_DO_BEFORE_ACTION \
    atctext = atc_bp; \
    atcleng = (int) (atc_cp - atc_bp); \
    atc_c_buf_p = atc_cp;

#define ATC_NUM_RULES 72
#define ATC_END_OF_BUFFER 73
static const uint16 atc_accept[184] =
    {   0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       73,   71,   70,   72,    3,   71,   71,    2,   71,   13,
        4,   15,   14,   71,   71,   71,   71,   65,   67,   68,
       69,   70,    5,    0,    0,    0,    0,    1,   59,    0,
       66,   65,   67,   68,   69,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,   22,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,    0,    0,    0,   26,    9,    0,   35,   25,
       18,    8,   17,   57,   53,   50,   48,   51,   52,   56,
       27,   55,   24,   20,    6,   49,    7,   58,   54,   23,
       61,   34,    0,    0,    0,    0,    0,    0,   21,    0,
        0,    0,    0,    0,   36,   32,   33,   28,   60,   38,
        0,   64,    0,   29,   40,    0,    0,   37,   44,    0,
        0,    0,   42,    0,   43,    0,   39,    0,    0,   47,
        0,    0,   45,   63,    0,    0,    0,   46,   41,    0,
        0,   62,    0
    } ;

static const int atc_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    4,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    5,    6,    7,    8,    9,   10,   11,   12,   13,
       14,   15,   16,   17,   18,   19,   20,   21,   22,   22,
       22,   22,   22,   22,   22,   22,   22,   23,   24,   25,
       26,   27,   28,   29,   35,   36,   37,   38,   39,   40,
       41,   42,   43,   44,   45,   46,   47,   48,   49,   50,
       51,   52,   53,   54,   55,   56,   57,   44,   58,   44,
       30,    1,   31,   32,   33,   34,   35,   36,   37,   38,

       39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
       49,   50,   51,   52,   53,   54,   55,   56,   57,   44,
       58,   44,   59,   60,   61,   62,   63,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,

        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1
    } ;

static const int atc_meta[64] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1
    } ;

static const uint16 atc_base[186] =
    {   0,
        0,   52,    4,   12,  101,  135,  173,    0,  236,    0,
      545,  546,    4,  546,  546,  536,    6,  546,  490,  488,
      546,  546,  546,  520,  295,   24,   15,   19,  354,  388,
      447,    5,  546,   49,  476,    9,  503,  546,  546,    0,
      546,   26,    0,    0,    0,   10,   15,   28,  492,  492,
       31,  498,   38,  483,  497,  498,   50,  485,  476,  479,
       75,  468,   73,  474,  469,  470,  468,  480,  480,  476,
      480,  350,  338,   95,  334,  332,  342,  334,  330,  334,
      327,  341,  337,  342,  324,  546,  335,  325,  322,  320,
      327,  312,  310,  321,  325,  323,   22,  321,  116,  130,

       32,  132,  113,  112,  121,  546,  546,  122,  546,  546,
      546,  546,  546,  546,  546,  546,  546,  546,  546,  546,
      546,  546,  546,  546,  546,  546,  546,  546,  546,  546,
      546,  123,  122,  119,  106,  116,  100,  105,  546,  105,
      107,   96,  105,  101,  546,  546,  546,  546,  546,  546,
       94,  546,   81,  546,  546,   90,   73,  546,  546,   69,
       89,   86,  546,   73,  546,   69,  546,   63,   59,  546,
       61,   57,  546,  546,   64,   57,   33,  546,  546,   18,
       10,  546,  546,   11,    0
    } ;

static const uint16 atc_def[186] =
    {   0,
      184,  184,    2,    2,  185,  185,  183,    7,  183,    9,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,   25,
      183,  183,   29,   30,   31,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,

      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,    0,  183,  183
    } ;

static const uint16 atc_nxt[610] =
    {   0,
       14,   13,   14,   15,   13,   32,   32,   16,   32,   32,
       25,   12,  183,  183,  183,   17,  183,  183,   25,   26,
      183,   27,  183,   18,   28,   28,  183,   26,  183,   27,
      183,   19,   28,   28,   20,   42,   42,   21,   22,   42,
       42,   23,   34,  183,   42,   42,   42,   42,   69,   66,
      183,   72,   24,   13,   14,   15,   13,   73,   35,   16,
       34,   36,   67,  182,   70,   79,   71,   17,   74,  141,
      181,  180,  142,   80,   84,   18,   35,   81,  146,   36,
       75,  147,   76,   19,   85,   46,   20,   82,   86,   21,
       22,  179,   90,   23,   47,   48,   49,   50,   51,   91,

       52,   53,  178,   54,   24,   29,   92,  101,   29,   96,
      177,  102,  176,   97,  175,   29,  174,   29,  173,  172,
      171,   29,   29,  170,  169,  168,   98,   29,  167,   99,
      103,  114,  115,  166,  116,   29,   29,   29,   29,   29,
      117,  165,   29,  164,  163,  162,  118,  119,  161,   29,
      160,   29,  159,  158,  157,   29,   29,  156,  155,  154,
      153,   29,  152,  151,  150,  149,  148,  145,  144,   29,
       29,   29,   29,   14,   14,   14,   14,   30,   30,   14,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,

       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   14,   14,   14,   14,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   40,

       40,   41,   40,   40,   40,   40,   40,   40,   40,   40,
       40,   40,   40,   40,   40,   40,   40,   40,   40,   40,
       40,   40,   40,   40,   40,   40,   40,   40,   40,   40,
       40,   40,   40,   40,   40,   40,   40,   40,   40,   40,
       40,   40,   40,   40,   40,   40,   40,   40,   40,   40,
       40,   40,   40,   40,   40,   40,   40,   40,   43,  143,
      140,   43,  139,  138,  137,  136,  135,  134,   43,  133,
       43,  132,  131,  130,   43,   43,  129,  128,  127,  126,
       43,  125,  124,  123,  122,  121,  120,  113,   43,   43,
       43,   43,   44,   44,  112,   44,   44,   44,   44,   44,

       44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
       44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
       44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
       44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
       44,   44,   44,   44,   44,   44,   44,   44,   44,   44,
       44,   45,   45,   45,   45,   45,   45,   45,   45,   45,
       45,   45,   45,   45,   45,   45,   45,   45,   45,   45,
       45,   45,   45,   45,   45,   45,   45,   45,   45,   45,
       45,   45,   45,   45,   45,   45,   45,   45,   45,   45,
       45,   45,   45,   45,   45,   45,   45,   45,   45,   45,

       45,   45,   45,   45,   45,   45,   45,   45,   45,   45,
       55,   56,   57,   58,  111,   59,  110,  109,   60,  108,
      107,  106,  105,  104,  100,   61,   95,   62,   63,   94,
       64,   93,   89,   65,   88,   87,   83,   78,   77,   68,
       39,   38,   37,   33,  183,   11,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,

      183,  183,  183,  183,  183,  183,  183,  183,  183
    } ;

static const uint16 atc_chk[610] =
    {   0,
      185,    1,    1,    1,    1,   13,   32,    1,   13,   32,
        3,  184,    0,    0,    0,    1,    0,    0,    4,    3,
        0,    3,    0,    1,    3,    3,    0,    4,    0,    4,
        0,    1,    4,    4,    1,   27,   27,    1,    1,   28,
       28,    1,   17,    0,   26,   26,   42,   42,   46,   36,
        0,   47,    1,    2,    2,    2,    2,   47,   17,    2,
       26,   17,   36,  181,   46,   51,   46,    2,   48,   97,
      180,  177,   97,   51,   53,    2,   26,   51,  101,   26,
       48,  101,   48,    2,   53,   34,    2,   51,   53,    2,
        2,  176,   57,    2,   34,   34,   34,   34,   34,   57,

       34,   34,  175,   34,    2,    5,   57,   63,    5,   61,
      172,   63,  171,   61,  169,    5,  168,    5,  166,  164,
      162,    5,    5,  161,  160,  157,   61,    5,  156,   61,
       63,   74,   74,  153,   74,    5,    5,    5,    5,    6,
       74,  151,    6,  144,  143,  142,   74,   74,  141,    6,
      140,    6,  138,  137,  136,    6,    6,  135,  134,  133,
      132,    6,  108,  105,  104,  103,  102,  100,   99,    6,
        6,    6,    6,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,

        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,   25,

       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   29,   98,
       96,   29,   95,   94,   93,   92,   91,   90,   29,   89,
       29,   88,   87,   85,   29,   29,   84,   83,   82,   81,
       29,   80,   79,   78,   77,   76,   75,   73,   29,   29,
       29,   29,   30,   30,   72,   30,   30,   30,   30,   30,

       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,

       31,   31,   31,   31,   31,   31,   31,   31,   31,   31,
       35,   35,   35,   35,   71,   35,   70,   69,   35,   68,
       67,   66,   65,   64,   62,   35,   60,   35,   35,   59,
       35,   58,   56,   35,   55,   54,   52,   50,   49,   37,
       24,   20,   19,   16,   11,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
      183,  183,  183,  183,  183,  183,  183,  183,  183,  183,

      183,  183,  183,  183,  183,  183,  183,  183,  183
    } ;

static uint atc_last_accepting_state;
static char *atc_last_accepting_cpos;

char *atctext;
#define INITIAL 0
#include "sci_types.h"
//#include "sdl_api.h"

#include "atc_common.h"
#include "atc_gsmonly_def.h"

#include "atc_yacc.h"

/******************************************************************************/
// Description : This function is used to reset lex state while do some unexpect
//               parase.
// Global resource dependence : None
// Author : Winnie.zhu
// Note : add for reset lex state during at command parase.
/******************************************************************************/
void zerolex(void)
{
    atc_start = 1;
    atc_last_accepting_state = 0;
    atc_last_accepting_cpos = NULL;
}

const uint8 * atcparse_at(const uint8 * atcbuf, uint32 len)
{
    if(atcbuf && len)
    {
        while(len)
        {
            if((atcbuf[0] == 'a' || atcbuf[0] == 'A') && (atcbuf[1] == 't' || atcbuf[1] == 'T'))
            {
                return atcbuf;
            }

            atcbuf++;
            len--;
        }
    }

    return atcbuf;
}

#define AT_CMD 1

#define AT_DIAL_CMD 2

#define AT_SHARP_CMD 3

#define AT_DTMF_CMD 4


int atclex(void)
    {
    register uint atc_current_state;
    register char *atc_cp, *atc_bp;
    register int atc_act;




    while ( 1 )        /* loops until end-of-file is reached */
        {
        atc_cp = atc_c_buf_p;

        /* atc_bp points to the position in atc_ch_buf of the start of
         * the current run.
         */
        atc_bp = atc_cp;

        atc_current_state = atc_start;
        do
            {
            register uint8 atc_c = atc_ec[ATC_SC_TO_UI(*atc_cp)];
            if ( atc_accept[atc_current_state] )
                {
                atc_last_accepting_state = atc_current_state;
                atc_last_accepting_cpos = atc_cp;
                }
            while ( atc_chk[atc_base[atc_current_state] + atc_c] != atc_current_state )
                {
                atc_current_state = (int) atc_def[atc_current_state];
                if ( atc_current_state >= 184 )
                    atc_c = atc_meta[(uint) atc_c];
                }
            atc_current_state = atc_nxt[atc_base[atc_current_state] + (uint) atc_c];
            ++atc_cp;
            }
        while ( atc_base[atc_current_state] != 546 );

atc_find_action:
        atc_act = atc_accept[atc_current_state];
        if ( atc_act == 0 )
            { /* have to back up */
            atc_cp = atc_last_accepting_cpos;
            atc_current_state = atc_last_accepting_state;
            atc_act = atc_accept[atc_current_state];
            }

        ATC_DO_BEFORE_ACTION;




        switch ( atc_act )
    { /* beginning of action switch */
            case 0: /* must back up */
            /* undo the effects of ATC_DO_BEFORE_ACTION */
            atc_cp = atc_last_accepting_cpos;
            atc_current_state = atc_last_accepting_state;
            goto atc_find_action;

case 1:
{ return AT_CMD_PREFIX; }
case 2:
{ BEGIN(INITIAL); return AT_CMD_DIVIDE;}
case 3:
{ BEGIN(INITIAL); return AT_CMD_END; }
case 4:
{ atclval.cmd_index = AT_CMD_D; BEGIN(AT_DIAL_CMD); return AT_BASIC_CMD; }
case 5:
{ atclval.cmd_index = AT_CMD_SHARP_SHARP; BEGIN(AT_SHARP_CMD); return AT_BASIC_CMD; }
case 6:
{ atclval.cmd_index = AT_CMD_CPIN; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 7:
{ atclval.cmd_index = AT_CMD_CPWD; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 8:
{ atclval.cmd_index = AT_CMD_CLCK; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 9:
{ atclval.cmd_index = AT_CMD_VTS; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 10:
{ atclval.cmd_index = AT_CMD_AT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 11:
{ atclval.cmd_index = AT_CMD_SHARP_SHARP; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 12:
{ atclval.cmd_index = AT_CMD_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 13:
{ atclval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 14:
{ atclval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 15:
{ atclval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 16:
{ atclval.cmd_index = AT_CMD_CPIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 17:
{ atclval.cmd_index = AT_CMD_CLIP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 18:
{ atclval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 19:
{ atclval.cmd_index = AT_CMD_VTS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 20:
{ atclval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 21:
{ atclval.cmd_index = AT_CMD_SIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 22:
{ atclval.cmd_index = AT_CMD_CSQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 23:
{ atclval.cmd_index = AT_CMD_CSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 24:
{ atclval.cmd_index = AT_CMD_COLP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 25:
{ atclval.cmd_index = AT_CMD_CCUG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 26:
{ atclval.cmd_index = AT_CMD_VGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 27:
{ atclval.cmd_index = AT_CMD_CMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 28:
{ atclval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 29:
{ atclval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 30:
{ atclval.cmd_index = AT_CMD_CLCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 31:
{ atclval.cmd_index = AT_CMD_CPWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 32:
{ atclval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 33:
{ atclval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 34:
{ atclval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 35:
{ atclval.cmd_index = AT_CMD_CCED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 36:
{ atclval.cmd_index = AT_CMD_SRWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 37:
{ atclval.cmd_index = AT_CMD_SDMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 38:
{ atclval.cmd_index = AT_CMD_SUPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 39:
{ atclval.cmd_index = AT_CMD_SCPFRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 40:
{ atclval.cmd_index = AT_CMD_SCINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 41:
{ atclval.cmd_index = AT_CMD_SPEQPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 42:
{ atclval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 43:
{ atclval.cmd_index = AT_CMD_SYTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 44:
{ atclval.cmd_index = AT_CMD_SFTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 45:
{ atclval.cmd_index = AT_CMD_SADMDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 46:
{ atclval.cmd_index = AT_CMD_SPADLVVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 47:
{ atclval.cmd_index = AT_CMD_SPENHA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 48:
{ atclval.cmd_index = AT_CMD_CMGL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 49:
{ atclval.cmd_index = AT_CMD_CPMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 50:
{ atclval.cmd_index = AT_CMD_CMGF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 51:
{ atclval.cmd_index = AT_CMD_CMGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 52:
{ atclval.cmd_index = AT_CMD_CMGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 53:
{ atclval.cmd_index = AT_CMD_CMGD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 54:
{ atclval.cmd_index = AT_CMD_CSCA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 55:
{ atclval.cmd_index = AT_CMD_CNMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 56:
{ atclval.cmd_index = AT_CMD_CMSS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 57:
{ atclval.cmd_index = AT_CMD_CMGC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 58:
{ atclval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 59:
{ atclval.cmd_index = AT_CMD_S0; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 60:
{ atclval.cmd_index = AT_CMD_SSYS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 61:
{ atclval.cmd_index = AT_CMD_CUSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 62:
{ atclval.cmd_index = AT_CMD_SPUSIMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 63:
{ atclval.cmd_index = AT_CMD_SCWTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 64:
{ atclval.cmd_index = AT_CMD_xor_SCFG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 65:
{ return NUMBER;  }
case 66:
{ return STRING; }
case 67:
{ BEGIN(INITIAL); return DIALSTRING; }
case 68:
{ BEGIN(INITIAL); return SHARPSTRING; }
case 69:
{ BEGIN(INITIAL); return DTMFSTRING; }
case ATC_STATE_EOF(INITIAL):
case ATC_STATE_EOF(AT_CMD):
case ATC_STATE_EOF(AT_DIAL_CMD):
case ATC_STATE_EOF(AT_SHARP_CMD):
case ATC_STATE_EOF(AT_DTMF_CMD):
{return 0;}
case 70:
{ }
    break;
case 71:
{ return atctext[0]; }
case 72:
    break;

    case ATC_END_OF_BUFFER:
        return 0;
    default:
        return -1;    } /* end of action switch */
        } /* end of scanning one token */
    } /* end of atclex */




/*lint +e716*/ 
/*lint +e18*/ 
