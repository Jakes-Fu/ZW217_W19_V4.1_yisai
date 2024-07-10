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

#define ATC_NUM_RULES 141
#define ATC_END_OF_BUFFER 142
static const uint16 atc_accept[338] =
    {   0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      142,  140,  139,  141,    3,  140,  140,  140,  140,    2,
       17,    4,    6,   18,   15,  140,   16,  123,   19,  140,
      140,  140,  134,  136,  137,  138,  139,    5,    0,   21,
       22,   23,    0,    0,    0,    0,    0,    0,    1,   44,
       24,    0,  135,  134,  136,  137,  138,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,   83,    0,    0,  130,    0,    0,    0,    0,    0,

        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,   34,    0,  126,  125,  129,  127,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,   85,   10,    0,
       81,  128,   99,   26,   84,   82,   80,   27,   97,    0,
        0,    0,    0,    0,    0,    0,   36,   37,   39,    0,
        0,    0,    0,   38,   30,   35,   98,   31,    9,   40,

       66,   62,   59,   61,   60,   63,   67,   64,   86,  117,
       72,   32,  131,   74,   77,   76,   73,   79,   75,    7,
       58,   42,   43,    8,   28,   57,   69,   68,   71,   41,
       56,   65,   55,  132,    0,   94,  102,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,  118,   33,   70,
        0,    0,    0,    0,   96,    0,   92,   93,   87,   78,
      101,  107,    0,  106,    0,  133,   50,   49,    0,    0,
        0,    0,    0,    0,   46,    0,    0,   54,    0,    0,
        0,   88,  111,  119,    0,  105,   89,    0,  116,    0,
        0,    0,    0,    0,  114,  100,  115,   11,   52,    0,

        0,    0,   53,   48,   47,    0,    0,    0,  108,  112,
        0,    0,    0,  121,    0,   45,  110,  109,    0,   95,
        0,    0,    0,    0,    0,  120,  103,    0,  122,    0,
      113,  104,    0,    0,    0,  124,    0
    } ;

static const int atc_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    4,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    5,    6,    7,    8,    9,   10,   11,   12,   13,
       14,   15,   16,   17,   18,   19,   20,   21,   22,   22,
       22,   22,   22,   22,   23,   22,   22,   24,   25,   26,
       27,   28,   29,   30,   36,   37,   38,   39,   40,   41,
       42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
       52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
       31,    1,   32,   33,   34,   35,   36,   37,   38,   39,

       40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
       50,   51,   52,   53,   54,   55,   56,   57,   58,   59,
       60,   61,   62,   63,   64,   65,   66,    1,    1,    1,
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

static const int atc_meta[67] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1
    } ;

static const uint16 atc_base[340] =
    {   0,
        0,   60,    5,   26,  117,  152,  191,    0,  257,    0,
      729,  730,    5,  730,  730,  720,  678,   12,   32,  730,
      671,  730,  730,  730,  730,    1,  730,  730,  730,  319,
       56,    8,   59,  381,  416,  478,   12,  730,  667,  730,
      730,  730,  509,   18,  670,  528,    3,  664,  730,  730,
      730,    0,  730,   83,    0,    0,    0,  669,  671,   54,
       86,  664,  110,  672,   39,  680,   55,  669,  665,  357,
      675,  539,  678,   76,  664,  662,   96,  675,  128,   12,
       47,  659,   88,  655,  130,  352,  658,  656,  651,  652,
      650,  730,  667,  664,  730,  646,  661,  661,  659,  651,

      654,  659,  645,    0,  645,   37,  129,  537,  654,   70,
      651,  641,  650,  644,  648,  140,  646,  554,  631,   88,
      640,  629,  628,  545,  632,  626,   43,  640,   74,  624,
      354,  634,  114,  730,  625,  730,  730,  730,  730,  622,
      626,  618,  617,  622,  621,  613,  627,  611,  618,  617,
      620,  610,  622,  622,  622,  358,  135,  609,  618,  362,
      621,  605,  600,  600,  604,  598,  607,  730,  730,  595,
      730,  730,  730,  730,  730,  730,  730,  730,  730,  594,
      593,  597,  591,  595,  363,  604,  730,  730,  730,  593,
      598,  601,  598,  730,  730,  730,  730,  730,  730,  730,

      730,  730,  730,  730,  730,  730,  730,  730,  730,  730,
      730,  730,  730,  730,  730,  730,  730,  730,  730,  730,
      730,  730,  730,  730,  730,  730,  730,  730,  730,  730,
      730,  730,  730,  730,  597,  599,  730,  587,  597,  594,
      575,  580,  577,  590,  576,  580,  574,  730,  730,  730,
      580,  587,  582,  573,  730,  582,  730,  730,  730,  730,
      730,  730,  582,  730,  572,  730,  730,  730,  581,  583,
      569,  573,  576,  564,  730,  565,  561,  730,  563,  556,
      573,  730,  730,  730,  566,  730,  730,  557,  730,  565,
      545,  543,  561,  553,  730,  730,  730,  730,  730,  525,

      525,  521,  730,  730,  730,  521,  517,  506,  730,  730,
      519,  497,  499,  730,  495,  730,  730,  730,  384,  730,
      344,  340,  347,  138,  135,  730,  730,  123,  730,   86,
      730,  730,   71,   42,    1,  730,  730,    5,    0
    } ;

static const uint16 atc_def[340] =
    {   0,
      338,  338,    2,    2,  339,  339,  337,    7,  337,    9,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,   30,  337,  337,   34,   35,   36,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,

      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,

      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,

      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,    0,  337,  337
    } ;

static const uint16 atc_nxt[797] =
    {   0,
       14,   13,   14,   15,   13,   12,   37,   16,   17,   37,
       18,   30,  337,   37,  337,   19,   37,  337,  337,  337,
       31,   50,   32,   51,   20,   33,   33,   33,   54,   54,
       54,  337,   30,  337,  337,   21,  337,  180,   22,   23,
      336,   31,   24,   32,   90,  337,   33,   33,   33,   40,
       41,   25,   42,   26,  181,   73,   27,   91,   28,  148,
       29,   13,   14,   15,   13,   74,  149,   16,   17,   43,
       18,   75,  183,   44,  184,   19,   54,   54,   54,   54,
       54,   54,   45,  222,   20,   46,  114,  115,   47,  223,
       48,   95,  335,   43,  117,   21,  118,   44,   22,   23,

      150,  151,   24,   54,   54,   54,   45,   96,  119,   46,
      120,   25,   47,   26,   48,  225,   27,  191,   28,  136,
       29,   34,  192,  137,   34,   97,   98,  226,  138,   99,
      334,   34,  100,   34,  141,  153,  154,   34,   34,   34,
      333,  101,  209,  102,   34,  104,  210,  105,  106,  107,
      142,  143,   34,   34,   34,   34,   34,  108,  332,   34,
      109,  110,  111,  112,  232,  156,   34,  233,   34,  157,
      331,  145,   34,   34,   34,  146,  158,  198,  147,   34,
      185,  186,  159,  253,  330,  199,  254,   34,   34,   34,
       34,   14,   14,   14,   14,   35,   35,   14,   35,   35,

       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   14,   14,   14,
       14,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,

       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   52,   52,   53,   52,   52,   52,   52,
       52,   52,   52,   52,   52,   52,   52,   52,   52,   52,
       52,   52,   52,   52,   52,   52,   52,   52,   52,   52,
       52,   52,   52,   52,   52,   52,   52,   52,   52,   52,
       52,   52,   52,   52,   52,   52,   52,   52,   52,   52,
       52,   52,   52,   52,   52,   52,   52,   52,   52,   52,
       52,   52,   52,   52,   52,   55,  329,  160,   55,  228,
      229,  161,  123,  124,  328,   55,  251,   55,  327,  162,

      125,   55,   55,   55,  126,  163,  127,  230,   55,  257,
      272,  164,  258,  252,  128,  273,   55,   55,   55,   55,
       56,   56,  326,   56,   56,   56,   56,   56,   56,   56,
       56,   56,   56,   56,   56,   56,   56,   56,   56,   56,
       56,   56,   56,   56,   56,   56,   56,   56,   56,   56,
       56,   56,   56,   56,   56,   56,   56,   56,   56,   56,
       56,   56,   56,   56,   56,   56,   56,   56,   56,   56,
       56,   56,   56,   56,   56,   56,   56,   56,   56,   56,
       56,   56,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,

       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   59,   60,   61,  325,  324,   62,
       63,   64,   65,  323,  322,   66,   67,   68,   69,   70,
      321,   71,   72,   77,   78,   79,   80,  320,   81,   82,
      319,   83,  318,  317,  130,   84,  131,  132,   85,  316,
      187,   86,   87,   88,  188,  214,  133,   89,  315,  189,
      134,  201,  202,  215,  203,  216,  314,  217,  218,  313,

      204,  312,  219,  311,  310,  309,  205,  206,  308,  307,
      306,  207,  305,  304,  303,  302,  301,  300,  299,  298,
      297,  296,  295,  294,  293,  292,  291,  290,  289,  288,
      287,  286,  285,  284,  283,  282,  281,  280,  279,  278,
      277,  276,  275,  274,  271,  270,  269,  268,  267,  266,
      265,  264,  263,  262,  261,  260,  259,  256,  255,  250,
      249,  248,  247,  246,  245,  244,  243,  242,  241,  240,
      239,  238,  237,  236,  235,  234,  231,  227,  224,  221,
      220,  213,  212,  211,  208,  200,  197,  196,  195,  194,
      193,  190,  182,  179,  178,  177,  176,  175,  174,  173,

      172,  171,  170,  169,  168,  167,  166,  165,  155,  152,
      144,  140,  139,  135,  129,  122,  121,  116,  113,  103,
       94,   93,   92,   76,   58,   49,   39,   38,  337,   11,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337

    } ;

static const uint16 atc_chk[797] =
    {   0,
      339,    1,    1,    1,    1,  338,   13,    1,    1,   13,
        1,    3,    0,   37,    0,    1,   37,    0,    0,    0,
        3,   26,    3,   26,    1,    3,    3,    3,   32,   32,
       32,    0,    4,    0,    0,    1,    0,  104,    1,    1,
      335,    4,    1,    4,   47,    0,    4,    4,    4,   18,
       18,    1,   18,    1,  104,   44,    1,   47,    1,   80,
        1,    2,    2,    2,    2,   44,   80,    2,    2,   19,
        2,   44,  106,   19,  106,    2,   31,   31,   31,   33,
       33,   33,   19,  127,    2,   19,   65,   65,   19,  127,
       19,   60,  334,   31,   67,    2,   67,   31,    2,    2,

       81,   81,    2,   54,   54,   54,   31,   60,   67,   31,
       67,    2,   31,    2,   31,  129,    2,  110,    2,   74,
        2,    5,  110,   74,    5,   61,   61,  129,   74,   61,
      333,    5,   61,    5,   77,   83,   83,    5,    5,    5,
      330,   61,  120,   61,    5,   63,  120,   63,   63,   63,
       77,   77,    5,    5,    5,    5,    6,   63,  328,    6,
       63,   63,   63,   63,  133,   85,    6,  133,    6,   85,
      325,   79,    6,    6,    6,   79,   85,  116,   79,    6,
      107,  107,   85,  157,  324,  116,  157,    6,    6,    6,
        6,    7,    7,    7,    7,    7,    7,    7,    7,    7,

        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,

        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   34,  323,   86,   34,  131,
      131,   86,   70,   70,  322,   34,  156,   34,  321,   86,

       70,   34,   34,   34,   70,   86,   70,  131,   34,  160,
      185,   86,  160,  156,   70,  185,   34,   34,   34,   34,
       35,   35,  319,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,

       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   36,   36,   36,   36,   36,   36,
       36,   36,   36,   36,   43,   43,   43,  315,  313,   43,
       43,   43,   43,  312,  311,   43,   43,   43,   43,   43,
      308,   43,   43,   46,   46,   46,   46,  307,   46,   46,
      306,   46,  302,  301,   72,   46,   72,   72,   46,  300,
      108,   46,   46,   46,  108,  124,   72,   46,  294,  108,
       72,  118,  118,  124,  118,  124,  293,  124,  124,  292,

      118,  291,  124,  290,  288,  285,  118,  118,  281,  280,
      279,  118,  277,  276,  274,  273,  272,  271,  270,  269,
      265,  263,  256,  254,  253,  252,  251,  247,  246,  245,
      244,  243,  242,  241,  240,  239,  238,  236,  235,  193,
      192,  191,  190,  186,  184,  183,  182,  181,  180,  170,
      167,  166,  165,  164,  163,  162,  161,  159,  158,  155,
      154,  153,  152,  151,  150,  149,  148,  147,  146,  145,
      144,  143,  142,  141,  140,  135,  132,  130,  128,  126,
      125,  123,  122,  121,  119,  117,  115,  114,  113,  112,
      111,  109,  105,  103,  102,  101,  100,   99,   98,   97,

       96,   94,   93,   91,   90,   89,   88,   87,   84,   82,
       78,   76,   75,   73,   71,   69,   68,   66,   64,   62,
       59,   58,   48,   45,   39,   21,   17,   16,   11,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337,  337,  337,  337,  337,
      337,  337,  337,  337,  337,  337

    } ;

static uint atc_last_accepting_state;
static char *atc_last_accepting_cpos;

char *atctext;
#define INITIAL 0
#include "sci_types.h"
#include "atc_org_def.h"
#include "atc_common.h"

#include "atc_yacc.h"

/******************************************************************************/
// Description : This function is used to reset lex state while do some unexpect
//               parase.
// Global resource dependence : None
// Author : Shijun.cui
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
                if ( atc_current_state >= 338 )
                    atc_c = atc_meta[(uint) atc_c];
                }
            atc_current_state = atc_nxt[atc_base[atc_current_state] + (uint) atc_c];
            ++atc_cp;
            }
        while ( atc_base[atc_current_state] != 730 );

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
{ atclval.cmd_index = AT_CMD_E; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 7:
{ atclval.cmd_index = AT_CMD_CPIN; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 8:
{ atclval.cmd_index = AT_CMD_CPWD; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 9:
{ atclval.cmd_index = AT_CMD_CLCK; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 10:
{ atclval.cmd_index = AT_CMD_VTS; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 11:
{ atclval.cmd_index = AT_CMD_CGCMOD; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 12:
{ atclval.cmd_index = AT_CMD_AT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 13:
{ atclval.cmd_index = AT_CMD_SHARP_SHARP; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 14:
{ atclval.cmd_index = AT_CMD_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 15:
{ atclval.cmd_index = AT_CMD_Q; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 16:
{ atclval.cmd_index = AT_CMD_V; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 17:
{ atclval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 18:
{ atclval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 19:
{ atclval.cmd_index = AT_CMD_Z; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 20:
{ atclval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 21:
{ atclval.cmd_index = AT_CMD_and_C; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 22:
{ atclval.cmd_index = AT_CMD_and_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 23:
{ atclval.cmd_index = AT_CMD_and_F; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 24:
{ atclval.cmd_index = AT_CMD_S7; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 25:
{ atclval.cmd_index = AT_CMD_CPIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 26:
{ atclval.cmd_index = AT_CMD_CCFC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 27:
{ atclval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 28:
{ atclval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 29:
{ atclval.cmd_index = AT_CMD_VTS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 30:
{ atclval.cmd_index = AT_CMD_CHLD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 31:
{ atclval.cmd_index = AT_CMD_CLCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 32:
{ atclval.cmd_index = AT_CMD_COPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 33:
{ atclval.cmd_index = AT_CMD_SIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 34:
{ atclval.cmd_index = AT_CMD_CSQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 35:
{ atclval.cmd_index = AT_CMD_CIMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 36:
{ atclval.cmd_index = AT_CMD_CGMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 37:
{ atclval.cmd_index = AT_CMD_CGMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 38:
{ atclval.cmd_index = AT_CMD_CGSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 39:
{ atclval.cmd_index = AT_CMD_CGMR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 40:
{ atclval.cmd_index = AT_CMD_CMEE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 41:
{ atclval.cmd_index = AT_CMD_CSCS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 42:
{ atclval.cmd_index = AT_CMD_CPOF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 43:
{ atclval.cmd_index = AT_CMD_CPOL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 44:
{ atclval.cmd_index = AT_CMD_S0; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 45:
{ atclval.cmd_index = AT_CMD_CGDCONT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 46:
{ atclval.cmd_index = AT_CMD_CGPCO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 47:
{ atclval.cmd_index = AT_CMD_CGQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 48:
{ atclval.cmd_index = AT_CMD_CGQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 49:
{ atclval.cmd_index = AT_CMD_CGATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 50:
{ atclval.cmd_index = AT_CMD_CGACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 51:
{ atclval.cmd_index = AT_CMD_CGCMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 52:
{ atclval.cmd_index = AT_CMD_CGDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 53:
{ atclval.cmd_index = AT_CMD_CGEREP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 54:
{ atclval.cmd_index = AT_CMD_CGREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 55:
{ atclval.cmd_index = AT_CMD_CSMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 56:
{ atclval.cmd_index = AT_CMD_CSDH; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 57:
{ atclval.cmd_index = AT_CMD_CRES; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 58:
{ atclval.cmd_index = AT_CMD_CPMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 59:
{ atclval.cmd_index = AT_CMD_CMGF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 60:
{ atclval.cmd_index = AT_CMD_CMGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 61:
{ atclval.cmd_index = AT_CMD_CMGL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 62:
{ atclval.cmd_index = AT_CMD_CMGD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 63:
{ atclval.cmd_index = AT_CMD_CMGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 64:
{ atclval.cmd_index = AT_CMD_CMSS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 65:
{ atclval.cmd_index = AT_CMD_CSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 66:
{ atclval.cmd_index = AT_CMD_CMGC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 67:
{ atclval.cmd_index = AT_CMD_CMGW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 68:
{ atclval.cmd_index = AT_CMD_CSCA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 69:
{ atclval.cmd_index = AT_CMD_CSAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 70:
{ atclval.cmd_index = AT_CMD_SMSC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 71:
{ atclval.cmd_index = AT_CMD_CSCB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 72:
{ atclval.cmd_index = AT_CMD_CNMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 73:
{ atclval.cmd_index = AT_CMD_CPBR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 74:
{ atclval.cmd_index = AT_CMD_CPBF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 75:
{ atclval.cmd_index = AT_CMD_CPBW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 76:
{ atclval.cmd_index = AT_CMD_CPBP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 77:
{ atclval.cmd_index = AT_CMD_CPBN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 78:
{ atclval.cmd_index = AT_CMD_SSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 79:
{ atclval.cmd_index = AT_CMD_CPBS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 80:
{ atclval.cmd_index = AT_CMD_CCUG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 81:
{ atclval.cmd_index = AT_CMD_CAOC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 82:
{ atclval.cmd_index = AT_CMD_CCLK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 83:
{ atclval.cmd_index = AT_CMD_XX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 84:
{ atclval.cmd_index = AT_CMD_CCID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 85:
{ atclval.cmd_index = AT_CMD_VGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 86:
{ atclval.cmd_index = AT_CMD_CMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 87:
{ atclval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 88:
{ atclval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 89:
{ atclval.cmd_index = AT_CMD_SDTMF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 90:
{ atclval.cmd_index = AT_CMD_CLCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 91:
{ atclval.cmd_index = AT_CMD_CPWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 92:
{ atclval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 93:
{ atclval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 94:
{ atclval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 95:
{ atclval.cmd_index = AT_CMD_SADMDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 96:
{ atclval.cmd_index = AT_CMD_SPLO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 97:
{ atclval.cmd_index = AT_CMD_CFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 98:
{ atclval.cmd_index = AT_CMD_CIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 99:
{ atclval.cmd_index = AT_CMD_CCED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 100:
{ atclval.cmd_index = AT_CMD_STONE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 101:
{ atclval.cmd_index = AT_CMD_SSST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 102:
{ atclval.cmd_index = AT_CMD_SATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 103:
{ atclval.cmd_index = AT_CMD_SAUTOATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 104:
{ atclval.cmd_index = AT_CMD_SGPRSDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 105:
{ atclval.cmd_index = AT_CMD_SDMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 106:
{ atclval.cmd_index = AT_CMD_SUPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 107:
{ atclval.cmd_index = AT_CMD_SSYS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 108:
{ atclval.cmd_index = AT_CMD_SCPFRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 109:
{ atclval.cmd_index = AT_CMD_CGEQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 110:
{ atclval.cmd_index = AT_CMD_CGEQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 111:
{ atclval.cmd_index = AT_CMD_SCINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 112:
{ atclval.cmd_index = AT_CMD_SFSMSM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 113:
{ atclval.cmd_index = AT_CMD_SPEQPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 114:
{ atclval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 115:
{ atclval.cmd_index = AT_CMD_SYTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 116:
{ atclval.cmd_index = AT_CMD_SFTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 117:
{ atclval.cmd_index = AT_CMD_CMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 118:
{ atclval.cmd_index = AT_CMD_SIME; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 119:
{ atclval.cmd_index = AT_CMD_SCMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 120:
{ atclval.cmd_index = AT_CMD_PSORGMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 121:
{ atclval.cmd_index = AT_CMD_SPENHA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 122:
{ atclval.cmd_index = AT_CMD_SPADLVVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 123:
{ atclval.cmd_index = AT_CMD_X; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 124:
{ atclval.cmd_index = AT_CMD_SPAUDVOLTYPE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 125:
{ atclval.cmd_index = AT_CMD_GMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 126:
{ atclval.cmd_index = AT_CMD_GMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 127:
{ atclval.cmd_index = AT_CMD_GSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 128:
{ atclval.cmd_index = AT_CMD_CBST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 129:
{ atclval.cmd_index = AT_CMD_GMR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 130:
{ atclval.cmd_index = AT_CMD_CBC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 131:
{ atclval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 132:
{ atclval.cmd_index = AT_CMD_GCAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 133:
{ atclval.cmd_index = AT_CMD_DOLLAR_NWRAT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 134:
{ return NUMBER;  }
case 135:
{ return STRING; }
case 136:
{ BEGIN(INITIAL); return DIALSTRING; }
case 137:
{ BEGIN(INITIAL); return SHARPSTRING; }
case 138:
{ BEGIN(INITIAL); return DTMFSTRING; }
case ATC_STATE_EOF(INITIAL):
case ATC_STATE_EOF(AT_CMD):
case ATC_STATE_EOF(AT_DIAL_CMD):
case ATC_STATE_EOF(AT_SHARP_CMD):
case ATC_STATE_EOF(AT_DTMF_CMD):
{return 0;}
case 139:
{ }
    break;
case 140:
{ return atctext[0]; }
case 141:
    break;

    case ATC_END_OF_BUFFER:
        return 0;
    default:
        return -1;    } /* end of action switch */
        } /* end of scanning one token */
    } /* end of atclex */




/*lint +e716*/ 
/*lint +e18*/ 
