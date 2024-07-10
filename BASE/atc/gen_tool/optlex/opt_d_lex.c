#include "sci_types.h"
#include "sci_api.h"

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

#define ATC_NUM_RULES 25
#define ATC_END_OF_BUFFER 26
static const uint16 atc_accept[235] =
    {   0,
        0,    0,   26,   24,   25,   24,   24,   24,   24,   24,
       24,   24,   24,   24,   24,   24,   24,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,   11,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,    0,    0,    0,   23,    0,    0,    0,    0,
        0,    0,    9,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,   18,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    6,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,   19,    0,    0,    0,    0,
        0,    0,    0,    0,    0,   16,    0,    0,    4,    0,
        0,    0,    0,    0,    0,    0,    0,   17,    0,    0,
        0,    0,    0,   22,    0,    0,    0,   20,    0,    0,
        0,    0,    0,   21,    0,    0,    0,    0,    2,    0,

        0,    0,    0,    0,    0,   15,   13,    0,    0,    0,
        0,    5,    0,    0,    0,   12,    3,    1,    0,    0,
        0,   10,    8,    0,    0,    0,   14,    0,    0,    0,
        0,    0,    7,    0
    } ;

static const int atc_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    3,    1,    1,    1,    1,    1,    1,    4,    5,
        6,    7,    1,    1,    1,    1,    8,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    9,    1,    1,
       10,    1,    1,    1,   11,   12,   13,   14,   15,   16,
        1,   17,    1,    1,   18,   19,   20,   21,   22,   23,
        1,   24,   25,   26,   27,    1,    1,    1,   28,    1,
        1,    1,    1,    1,   29,    1,   30,   31,   32,   33,

       34,   35,   36,   37,   38,    1,    1,   39,   40,   41,
       42,   43,    1,   44,   45,   46,   47,    1,    1,   48,
       49,    1,    1,    1,    1,    1,    1,    1,    1,    1,
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

static const int atc_meta[50] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1
    } ;

static const uint16 atc_base[236] =
    {   0,
        0,   48,  256,  257,  257,  206,  247,  240,  222,    0,
      207,  222,    0,  207,    0,  214,  198,  197,  242,  227,
      211,  208,  198,  210,  210,  193,  207,  207,  232,  188,
      185,    0,  203,    2,  209,  186,  197,  185,  186,    0,
      192,  181,  194,  218,    0,  184,    5,  187,  188,  180,
      169,  257,  175,  178,  173,  169,  189,  201,  184,  207,
      206,  176,  204,  157,  164,  172,  161,  172,    0,  158,
      170,  170,  153,  167,  152,  156,  179,  167,  173,  163,
      183,  143,  139,  138,  140,  155,  145,  137,  142,  135,
      177,  143,  142,  174,  137,  129,  144,  162,  161,  156,

      142,  120,  130,  118,  137,  257,  126,  131,  131,  124,
      132,  150,  257,  125,  128,  154,  153,  109,  136,  134,
      123,  122,  101,  107,  119,   14,  257,  118,  109,  104,
      101,  140,   96,  106,  110,   97,  104,  257,  108,  111,
      120,  105,   92,   94,   90,   84,   97,  119,   97,   80,
      121,   95,   77,   90,   86,  257,  105,  104,   97,   85,
      107,   74,   76,   83,   75,  257,   65,   66,  257,   68,
       74,   61,  103,   81,   78,   89,   73,  257,   63,   54,
       55,   68,   62,  257,   61,   50,   55,  257,   68,   64,
       61,   58,   42,  257,   39,   40,   78,   34,  257,   37,

       56,   54,   53,   25,   68,  257,  257,   64,   23,   26,
       42,  257,   49,   29,   56,  257,  257,  257,   56,   31,
       29,  257,  257,   45,   11,   26,  257,   24,   20,   16,
        3,    0,  257,  257,    0
    } ;

static const uint16 atc_def[236] =
    {   0,
      235,  235,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,

      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,

      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,    0,  234
    } ;

static const uint16 atc_nxt[307] =
    {   0,
        4,    5,   64,  234,  234,  234,    6,    7,  233,  234,
      234,   57,  234,  234,    8,   58,  234,   50,  234,    9,
      234,  234,  234,   59,   10,   11,  232,   12,   47,   26,
      231,   13,   14,   65,   29,  230,   27,   15,   16,  229,
       30,   67,   22,   89,   68,   48,   23,   90,   17,    5,
       51,  145,  228,  227,    6,    7,  226,  225,  146,  224,
      223,  222,    8,  221,  220,  219,  218,    9,  217,  216,
      215,  214,   10,   11,  213,   12,  212,  211,  210,   13,
       14,  209,  208,  207,  206,   15,   16,  205,  204,  203,
      202,  201,  200,  199,  198,  197,   17,  196,  195,  194,

      193,  192,  191,  190,  189,  188,  187,  186,  185,  184,
      183,  182,  181,  180,  179,  178,  177,  176,  175,  174,
      173,  172,  171,  170,  169,  168,  167,  166,  165,  164,
      163,  162,  161,  160,  159,  158,  157,  156,  155,  154,
      153,  152,  151,  150,  149,  148,  147,  144,  143,  142,
      141,  140,  139,  138,  137,  136,  135,  134,  133,  132,
      131,  130,  129,  128,  127,  126,  125,  124,  123,  122,
      121,  120,  119,  118,  117,  116,  115,  114,  113,  112,
      111,  110,  109,  108,  107,  106,  105,  104,  103,  102,
      101,  100,   99,   98,   97,   96,   95,   94,   93,   92,

       91,   88,   87,   86,   85,   84,   83,   82,   81,   80,
       79,   78,   77,   76,   75,   74,   73,   72,   71,   70,
       69,   66,   63,   62,   61,   60,   56,   55,   54,   53,
       52,   49,   46,   45,   44,   43,   42,   41,   40,   39,
       38,   37,   36,   35,   34,   33,   32,   31,   28,   25,
       24,   21,   20,   19,   18,  234,    3,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,

      234,  234,  234,  234,  234,  234
    } ;

static const uint16 atc_chk[307] =
    {   0,
      235,    1,   45,    0,    0,    0,    1,    1,  232,    0,
        0,   40,    0,    0,    1,   40,    0,   34,    0,    1,
        0,    0,    0,   40,    1,    1,  231,    1,   32,   13,
      230,    1,    1,   45,   15,  229,   13,    1,    1,  228,
       15,   47,   10,   69,   47,   32,   10,   69,    1,    2,
       34,  126,  226,  225,    2,    2,  224,  221,  126,  220,
      219,  215,    2,  214,  213,  211,  210,    2,  209,  208,
      205,  204,    2,    2,  203,    2,  202,  201,  200,    2,
        2,  198,  197,  196,  195,    2,    2,  193,  192,  191,
      190,  189,  187,  186,  185,  183,    2,  182,  181,  180,

      179,  177,  176,  175,  174,  173,  172,  171,  170,  168,
      167,  165,  164,  163,  162,  161,  160,  159,  158,  157,
      155,  154,  153,  152,  151,  150,  149,  148,  147,  146,
      145,  144,  143,  142,  141,  140,  139,  137,  136,  135,
      134,  133,  132,  131,  130,  129,  128,  125,  124,  123,
      122,  121,  120,  119,  118,  117,  116,  115,  114,  112,
      111,  110,  109,  108,  107,  105,  104,  103,  102,  101,
      100,   99,   98,   97,   96,   95,   94,   93,   92,   91,
       90,   89,   88,   87,   86,   85,   84,   83,   82,   81,
       80,   79,   78,   77,   76,   75,   74,   73,   72,   71,

       70,   68,   67,   66,   65,   64,   63,   62,   61,   60,
       59,   58,   57,   56,   55,   54,   53,   51,   50,   49,
       48,   46,   44,   43,   42,   41,   39,   38,   37,   36,
       35,   33,   31,   30,   29,   28,   27,   26,   25,   24,
       23,   22,   21,   20,   19,   18,   17,   16,   14,   12,
       11,    9,    8,    7,    6,    3,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,
      234,  234,  234,  234,  234,  234,  234,  234,  234,  234,

      234,  234,  234,  234,  234,  234
    } ;

static uint atc_last_accepting_state;
static char *atc_last_accepting_cpos;

char *atctext;
#define INITIAL 0
#include <stdio.h>
#include <stdlib.h>

#include "OptLex.h"

int atclex(void)
    {
    register uint atc_current_state;
    register char *atc_cp, *atc_bp;
    register int atc_act;
    unsigned char is_atc_cmd_echo = 0;


    // reserve at command of echo from deletion for echo key
    if (strstr(atc_c_buf_p, "AT_CMD_") && strstr(atc_c_buf_p, "ECHO"))
    {
        is_atc_cmd_echo = 1;
    }

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
                if ( atc_current_state >= 235 )
                    atc_c = atc_meta[(uint) atc_c];
                }
            atc_current_state = atc_nxt[atc_base[atc_current_state] + (uint) atc_c];
            ++atc_cp;
            }
        while ( atc_base[atc_current_state] != 257 );

atc_find_action:
        atc_act = atc_accept[atc_current_state];
        if ( atc_act == 0 )
            { /* have to back up */
            atc_cp = atc_last_accepting_cpos;
            atc_current_state = atc_last_accepting_state;
            atc_act = atc_accept[atc_current_state];
            }

        // reserve at command of echo from deletion for echo key
        if (is_atc_cmd_echo && 11 == atc_act)
        {
            atc_act = 24;
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
{ return KEY_SPECIAL;            }
    break;
case 2:
{ return KEY_MACROS;             }
    break;
case 3:
{ return KEY_YY_GET_NEXT;        }
    break;
case 4:
{ return KEY_YY_CP;              }
    break;
case 5:
{ return KEY_YY_RULE_SETUP;      }
    break;
case 6:
{ return KEY_YY_BREAK;           }
    break;
case 7:
{ return KEY_YY_END_OF_BUFFER;   }
    break;
case 8:
{ return KEY_YY_FATAL_ERROR;     }
    break;
case 9:
{ return KEY_FLAG;               }
    break;
case 10:
{ return KEY_YY_INIT;            }
    break;
case 11:
{ return KEY_ECHO;               }
    break;
case 12:
{ return KEY_YYTERMINATE;        }
    break;
case 13:
{ return KEY_YY_HOLD_CHAR;       }
    break;
case 14:
{ return KEY_YY_C_BUF_P;         }
    break;
case 15:
{ return KEY_YY_START;           }
    break;
case 16:
{ return KEY_YY_MATCH;           }
    break;
case 17:
{ return KEY_DO_ACTION;          }
    break;
case 18:
{ return KEY_LEXICAL;            }
    break;
case 19:
{ return KEY_TRANSLATE;          }
    break;
case 20:
{ return KEY_SUPPORT_OF;         }
    break;
case 21:
{ return KEY_INT_YY_INIT;        }
    break;
case 22:
{ return KEY_YYTEXT_PTR;         }
    break;
case 23:
{ return KEY_INTENT;             }
    break;
case 24:
{ }
    break;
case 25:
    break;
case ATC_STATE_EOF(INITIAL):
    return 0;

    case ATC_END_OF_BUFFER:
        return 0;
    default:
        printf("atclex internal error");
    } /* end of action switch */
        } /* end of scanning one token */
    } /* end of atclex */


