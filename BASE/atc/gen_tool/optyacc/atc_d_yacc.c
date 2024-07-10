#include "sci_types.h"

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

#define ATC_NUM_RULES 17
#define ATC_END_OF_BUFFER 18
static const uint16 atc_accept[149] =
    {   0,
        0,    0,   18,   16,   17,   16,   16,   16,   16,   16,
       16,   16,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    3,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    1,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    4,    3,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    3,
        0,    2,    0,    0,    0,    0,   13,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,   10,   15,    0,    0,    0,    0,    0,    0,    0,
        0,    0,   14,    0,    0,    0,    0,    0,    0,   12,
        0,    8,    0,    0,    5,    0,    7,    0,    0,   11,
        0,    0,    9,    0,    0,    0,    6,    0
    } ;

static const int atc_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    3,    4,    1,    5,    1,    1,    1,    1,    1,
        1,    6,    1,    1,    7,    8,    9,    1,   10,   11,
        1,    1,    1,    1,    1,   12,    1,   13,   14,    1,
       15,   16,    1,    1,   17,   18,   19,   20,   21,    1,
       22,   23,    1,    1,    1,   24,   25,   26,    1,   27,
        1,   28,   29,   30,   31,    1,    1,   32,   33,    1,
        1,    1,    1,    1,   34,    1,   35,   36,   37,   38,

       39,   40,    1,    1,   41,    1,    1,   42,    1,   43,
       44,   45,    1,   46,   47,   48,   49,    1,    1,    1,
       50,    1,    1,    1,    1,    1,    1,    1,    1,    1,
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

static const int atc_meta[51] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1
    } ;

static const uint16 atc_base[150] =
    {   0,
        0,   49,  174,  175,  175,  170,    1,  166,  138,  129,
      126,  118,  117,  127,  122,  124,  122,  159,  141,  113,
      111,    9,  108,  117,  118,   17,  112,  151,  132,  108,
      148,  113,  103,  109,  100,  107,  104,  103,  104,  104,
      102,  107,  121,   95,   87,   99,   89,   88,   88,   86,
       88,   90,   89,   89,  175,   94,   95,  118,   74,   84,
       75,   75,  117,   73,   79,  175,  114,   76,   90,   92,
      103,   70,   66,   62,   62,   92,   60,  103,   72,  175,
       87,  175,   95,   63,   53,   65,  175,   83,   49,   63,
       62,   62,   82,   49,   49,   41,   85,   52,   53,    0,

       64,   71,   67,   45,   30,   28,   29,   59,   46,   43,
       52,  175,  175,   36,   24,   20,   20,   49,   40,   38,
       38,   51,  175,   16,   15,   42,   25,   36,   19,  175,
        0,  175,   16,    9,  175,   11,  175,    5,   10,  175,
        0,    2,  175,    2,    0,    0,  175,  175,    0
    } ;

static const uint16 atc_def[150] =
    {   0,
      149,  149,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,

      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,    0,  148
    } ;

static const uint16 atc_nxt[226] =
    {   0,
        4,    5,  148,    6,    7,  148,  148,  148,    8,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  147,
      146,  145,  144,  109,  148,  148,  110,  148,  148,  143,
      142,  141,    9,  140,  139,   10,  138,  148,   14,   15,
       11,   16,   17,   32,  148,  148,  137,   33,  136,   12,
        5,   34,    6,    7,   39,   35,  135,    8,  134,   40,
      133,  132,  131,  130,  129,  128,  127,  126,  125,  124,
      123,  122,  121,  120,  119,  118,  117,  116,  115,  114,
      113,    9,  112,  111,   10,  108,  107,  106,  105,   11,
      104,  103,  102,  101,  100,   99,   98,   97,   12,   96,

       95,   94,   93,   92,   91,   90,   89,   88,   87,   86,
       85,   84,   83,   82,   81,   80,   79,   78,   77,   76,
       75,   74,   73,   72,   71,   70,   69,   68,   67,   66,
       65,   64,   63,   62,   61,   60,   59,   58,   57,   56,
       55,   54,   53,   52,   51,   50,   49,   48,   47,   46,
       45,   44,   43,   42,   41,   38,   37,   36,   31,   30,
       29,   28,   27,   26,   25,   24,   23,   22,   21,   20,
       19,   18,   13,  148,    3,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,

      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148
    } ;

static const uint16 atc_chk[226] =
    {   0,
      149,    1,    0,    1,    1,    0,    0,    0,    1,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,  146,
      145,  144,  142,  100,    0,    0,  100,    0,    0,  141,
      139,  138,    1,  136,  134,    1,  133,    0,    7,    7,
        1,    7,    7,   22,    0,    0,  131,   22,  129,    1,
        2,   22,    2,    2,   26,   22,  128,    2,  127,   26,
      126,  125,  124,  122,  121,  120,  119,  118,  117,  116,
      115,  114,  111,  110,  109,  108,  107,  106,  105,  104,
      103,    2,  102,  101,    2,   99,   98,   97,   96,    2,
       95,   94,   93,   92,   91,   90,   89,   88,    2,   86,

       85,   84,   83,   81,   79,   78,   77,   76,   75,   74,
       73,   72,   71,   70,   69,   68,   67,   65,   64,   63,
       62,   61,   60,   59,   58,   57,   56,   54,   53,   52,
       51,   50,   49,   48,   47,   46,   45,   44,   43,   42,
       41,   40,   39,   38,   37,   36,   35,   34,   33,   32,
       31,   30,   29,   28,   27,   25,   24,   23,   21,   20,
       19,   18,   17,   16,   15,   14,   13,   12,   11,   10,
        9,    8,    6,    3,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,

      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148,  148,  148,  148,  148,  148,
      148,  148,  148,  148,  148
    } ;

static uint atc_last_accepting_state;
static char *atc_last_accepting_cpos;

char *atctext;
#define INITIAL 0
#include <stdio.h>
#include <stdlib.h>

#include "OptYacc.h"


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
                if ( atc_current_state >= 149 )
                    atc_c = atc_meta[(uint) atc_c];
                }
            atc_current_state = atc_nxt[atc_base[atc_current_state] + (uint) atc_c];
            ++atc_cp;
            }
        while ( atc_base[atc_current_state] != 175 );

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
{ return KEY_PREPROCESSOR_LINE;      }
    break;
case 2:
{ return KEY_YYDEBUG;                }
    break;
case 3:
{ return KEY_MICRO_BEGIN;            }
    break;
case 4:
{ return KEY_ENDIF;                  }
    break;
case 5:
{ return KEY_DEF_YYPURE;             }
    break;
case 6:
{ return KEY_YYLSP_NEEDED;           }
    break;
case 7:
{ return KEY_YYSSP_YYSS;             }
    break;
case 8:
{ return KEY_YYERRSTATUS;            }
    break;
case 9:
{ return KEY_DEFINE_YYACCEPT;      }
    break;
case 10:
{ return KEY_BISON;                  }
    break;
case 11:
{ return KEY_YYMAXDEPTH;             }
    break;
case 12:
{ return KEY_YYACCEPTLAB;            }
    break;
case 13:
{ return KEY_YYNERRS;                }
    break;
case 14:
{ return KEY_YYERRSTATUS1;           }
    break;
case 15:
{ return KEY_INT_YYLEN;              }
    break;
case 16:
{ }
    break;
case 17:
    break;
case ATC_STATE_EOF(INITIAL):
    return 0;

    case ATC_END_OF_BUFFER:
        return 0;
    default:
        return -1;    } /* end of action switch */
        } /* end of scanning one token */
    } /* end of atclex */



