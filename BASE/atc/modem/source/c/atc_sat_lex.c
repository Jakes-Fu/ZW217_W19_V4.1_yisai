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

#define ATC_NUM_RULES 162
#define ATC_END_OF_BUFFER 163
static const uint16 atc_accept[429] =
    {   0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      163,  161,  160,  162,    3,  161,  161,    2,   12,    4,
       15,   13,   10,  161,  161,   11,   20,   14,  161,  161,
      161,  155,  157,  158,  159,  160,    5,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    1,   22,
        0,   23,   24,   25,   26,   27,   28,   29,    0,    0,
      156,  155,  157,  158,  159,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,  118,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,   93,    0,   30,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,   35,    0,    0,    0,    0,    0,    0,
       41,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      111,    0,    0,    0,    0,  115,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,   96,    0,   99,   94,    9,

        0,    0,   90,   91,  113,   32,   95,   92,   33,  117,
      110,    0,    0,    0,    0,    0,    0,    0,   43,   44,
       46,    0,    0,    0,    0,    0,   45,   36,  109,  139,
       42,  112,   37,    8,   47,   73,   69,   66,   68,   67,
       70,   74,   71,   98,  133,   79,   85,   38,   31,   81,
       84,   83,   80,   87,   82,    6,   65,   49,   50,   88,
        7,   34,   64,   97,  129,   76,   75,   48,   63,   72,
       62,   39,   86,   89,  104,    0,    0,    0,    0,    0,
        0,    0,  149,    0,  107,  116,    0,    0,    0,    0,
        0,    0,  119,    0,    0,  134,   40,  123,    0,   77,

      120,    0,    0,    0,    0,    0,  108,    0,    0,  105,
      106,  101,  125,    0,  124,   78,    0,   18,   17,   16,
       19,    0,   56,   55,    0,    0,    0,    0,    0,    0,
        0,   52,    0,    0,   61,    0,    0,    0,    0,    0,
        0,    0,    0,    0,  102,  121,  130,  135,  136,    0,
      138,  122,  103,    0,    0,    0,    0,    0,    0,  132,
        0,  114,    0,    0,   59,   58,    0,    0,    0,   60,
        0,   54,   53,    0,    0,    0,    0,  142,  143,  140,
        0,    0,  126,    0,  141,    0,    0,    0,    0,    0,
      148,    0,   51,  128,  127,   57,  147,    0,  144,  145,

        0,  151,  150,    0,    0,  100,    0,    0,    0,    0,
      137,  152,    0,  131,    0,    0,  146,    0,    0,    0,
        0,  154,    0,    0,    0,  153,   21,    0
    } ;

static const int atc_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    4,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    5,    6,    7,    8,    9,   10,   11,   12,   13,
       14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
       24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
       34,   35,   36,   37,   43,   44,   45,   46,   47,   48,
       49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
       59,   60,   61,   62,   63,   64,   65,   66,   67,   68,
       38,    1,   39,   40,   41,   42,   43,   44,   45,   46,

       47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
       57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
       67,   68,   69,   70,   71,   72,   73,    1,    1,    1,
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

static const int atc_meta[74] =
    {   0,
        1,    1,    1,    1,    2,    1,    1,    2,    1,    1,
        1,    1,    1,    1,    2,    2,    2,    1,    1,    1,
        2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
        1,    1,    1,    1,    2,    1,    1,    1,    1,    1,
        1,    1,    2,    2,    2,    2,    1,    1,    1,    1,
        2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1
    } ;

static const uint16 atc_base[432] =
    {   0,
        0,   67,  129,  153,  179,  226,  277,    0,  350,    0,
      912,  913,    4,  913,  913,  903,   29,  913,    1,  913,
      913,  913,  913,    0,  860,  913,  913,  913,  419,  472,
       12,   79,    0,  534,  603,    5,  913,  634,  864,  857,
      846,  848,  858,   25,  655,    4,  838,  857,  913,  913,
      881,  913,  913,  913,  913,  913,  913,  913,  841,    0,
      913,  482,    0,    0,    0,    0,   68,  853,  836,  676,
      844,   93,  852,   76,   18,  838,  689,   51,  174,  834,
      844,  833,  837,  669,  829,  829,  832,  127,    1,  110,
       79,   33,  831,   36,  823,  841,  471,   99,  828,   60,

      102,    6,  913,  843,  913,  836,  827,  826,  834,  834,
      832,  824,  833,  815,  818,  148,    6,   84,  137,   11,
       24,  826,  816,  825,  826,  805,  817,  821,  145,  819,
      167,  804,  174,  813,  808,  801,  800,  695,  804,  798,
      137,  813,  811,  913,  196,  204,  795,  183,  805,   69,
      913,  798,  798,  806,  794,  806,  788,  794,  800,  788,
      913,  798,  801,  792,  782,  913,  786,  778,  783,  781,
      781,  787,  787,  787,  775,  769,  776,  783,  783,  779,
      766,  781,  770,  771,   48,  203,  766,  775,  760,  205,
      777,  758,  762,  756,  755,  913,  760,  913,  913,  913,

      659,  767,  913,  913,  913,  913,  913,  913,  913,  913,
      913,  751,  750,  749,  748,  752,  213,  761,  913,  913,
      913,  761,  749,  754,  757,  754,  913,  913,  913,  913,
      913,  913,  913,  913,  913,  913,  913,  913,  913,  913,
      913,  913,  913,  913,  913,  913,  913,  913,  913,  913,
      913,  913,  913,  913,  913,  913,  913,  913,  913,  913,
      913,  913,  913,  913,  913,  913,  913,  913,  913,  913,
      913,  913,  913,  913,  913,  745,  739,  743,  756,  751,
      741,  738,  913,  746,  748,  913,  747,  730,  743,  461,
      730,  740,  913,  726,  739,  913,  913,  913,  721,  913,

      913,  738,  730,  737,  729,  723,  913,  732,  728,  913,
      913,  913,  913,  731,  913,  913,  716,  913,  913,  913,
      913,  735,  913,  913,  718,  731,  717,  721,  724,  712,
      723,  913,  712,  708,  913,  709,  722,  698,  713,  701,
      704,  693,  693,  686,  913,  913,  913,  913,  913,  698,
      913,  913,  913,  699,  679,  675,  674,  681,  684,  913,
      670,  913,  656,  662,  913,  913,  646,  647,  643,  913,
      636,  913,  913,  631,  625,  639,  618,  913,  913,  913,
      621,  483,  913,  479,  913,  471,  475,  469,  467,  462,
      913,  469,  913,  913,  913,  913,  913,  452,  913,  913,

      467,  913,  913,  229,  221,  913,  231,  220,  206,  204,
      913,  913,  202,  913,  178,  192,  913,  151,  154,  152,
      128,  913,  111,   73,   30,  913,  913,  913,  760,  762,
        9
    } ;

static const uint16 atc_def[432] =
    {   0,
      429,  429,    2,    2,  430,  430,  428,    7,  428,    9,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  431,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,   29,
      428,  428,  431,   34,   35,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,

      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,

      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,

      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,

      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,    0,  428,  428,
      428
    } ;

static const uint16 atc_nxt[987] =
    {   0,
      428,   13,   14,   15,   13,   36,   36,   16,   36,   36,
       63,  428,  428,  428,  428,   17,  428,  428,  428,  428,
       50,   51,   52,   53,   54,   55,   56,   57,   58,  428,
      428,   18,   62,   62,   62,   62,   62,   62,   62,   62,
       62,   62,   19,  169,  107,   20,   21,  428,  215,   22,
      216,  199,  101,  222,  108,  223,   48,  170,   23,  428,
       24,   25,   49,   26,   27,  102,  200,   28,   13,   14,
       15,   13,  134,   38,   16,   39,   40,   41,  224,   42,
      135,   86,   17,  225,   43,   87,   44,  178,  179,   45,
      181,  427,   46,  303,   47,  144,  182,  145,   18,   62,

       62,   62,   62,   62,   62,   62,   62,   62,   62,   19,
      304,  146,   20,   21,  109,  110,   22,  194,  111,  426,
      195,  112,  130,  175,  131,   23,  270,   24,   25,  271,
       26,   27,  113,  176,   28,   29,  132,  125,  133,  126,
      177,  190,  217,  218,   30,  191,   31,  127,  128,   32,
       32,   32,   32,   32,   32,   32,   32,   32,   32,   29,
      171,  196,  197,  198,  172,  192,  425,  173,   30,  174,
       31,  166,  167,   32,   32,   32,   32,   32,   32,   32,
       32,   32,   32,   33,  258,  424,   33,  219,  168,  233,
      259,  220,  212,   33,   33,   33,  221,  234,  423,   33,

       33,   33,   33,   33,   33,   33,   33,   33,   33,  213,
      214,  236,  237,   33,  238,  422,  147,  421,  148,  149,
      239,   33,   33,   33,   33,  267,  240,  241,  150,   33,
       33,  242,  151,   33,  152,  244,  420,  153,  419,  245,
       33,   33,   33,  268,  262,  305,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,  263,  264,  265,  310,
       33,  306,  311,  418,  417,  416,  415,  328,   33,   33,
       33,   33,  329,  414,  413,  412,   33,   14,   14,   14,
       14,   34,   34,   14,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,

       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       14,   14,   14,   14,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,

       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   60,   60,   61,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   62,   62,   62,   62,   62,   62,   62,   62,

       62,   62,   62,   62,   62,   62,   62,   62,   62,   62,
       62,   62,  411,  185,  410,  409,   38,  186,   39,   40,
       41,  348,   42,  408,  187,  349,  407,   43,  406,   44,
      188,  405,   45,  189,  404,   46,  403,   47,   64,   64,
      402,   64,   64,   64,   64,   64,   64,   64,   64,   64,
       64,   64,   64,   64,   64,   64,   64,   64,   64,   64,
       64,   64,   64,   64,   64,   64,   64,   64,   64,   64,
       64,   64,   64,   64,   64,   64,   64,   64,   64,   64,
       64,   64,   64,   64,   64,   64,   64,   64,   64,   64,
       64,   64,   64,   64,   64,   64,   64,   64,   64,   64,

       64,   64,   64,   64,   64,   64,   64,   65,   65,   65,
       65,   65,   65,   65,   65,   65,   65,   65,   65,   65,
       65,   65,   65,   65,   65,   65,   65,   65,   65,   65,
       65,   65,   65,   65,   65,   65,   65,   65,   65,   65,
       65,   65,   65,   65,   65,   65,   65,   65,   65,   65,
       65,   65,   65,   65,   65,   65,   65,   65,   65,   65,
       65,   65,   65,   65,   65,   65,   65,   65,   65,   65,
       65,   65,   65,   65,   65,   65,   66,  401,   67,  400,
       68,   69,   70,   71,   72,  399,  398,   73,   74,   75,
       76,   77,  397,   78,   79,  396,   80,   88,   89,   90,

       91,  395,  394,  318,  319,   92,  320,  393,   93,   94,
       95,   96,   97,  158,  159,   98,   99,  100,  116,  392,
      391,  117,  118,  321,  390,  160,  389,  388,  161,  162,
      119,  137,  138,  120,  121,  122,  123,  387,  386,  139,
      385,  384,  250,  140,  383,  141,  382,  381,  380,  379,
      251,  142,  252,  143,  253,  254,  378,  377,  376,  255,
       12,   12,   14,   14,  375,  374,  373,  372,  371,  370,
      369,  368,  367,  366,  365,  364,  363,  362,  361,  360,
      359,  358,  357,  356,  355,  354,  353,  352,  351,  350,
      347,  346,  345,  344,  343,  342,  341,  340,  339,  338,

      337,  336,  335,  334,  333,  332,  331,  330,  327,  326,
      325,  324,  323,  322,  317,  316,  315,  314,  313,  312,
      309,  308,  307,  302,  301,  300,  299,  298,  297,  296,
      295,  294,  293,  292,  291,  290,  289,  288,  287,  286,
      285,  284,  283,  282,  281,  280,  279,  278,  277,  276,
      275,  274,  273,  272,  269,  266,  261,  260,  257,  256,
      249,  248,  247,  246,  243,  235,  232,  231,  230,  229,
      228,  227,  226,  211,  210,  209,  208,  207,  206,  205,
      204,  203,  202,  201,  193,  184,  183,  180,  165,  164,
      163,  157,  156,  155,  154,  136,  129,  124,  115,  114,

      106,  105,  104,  103,   85,   84,   83,   82,   81,   59,
       37,  428,   11,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428
    } ;

static const uint16 atc_chk[987] =
    {   0,
        0,    1,    1,    1,    1,   13,   36,    1,   13,   36,
      431,    0,    0,    0,    0,    1,    0,    0,    0,    0,
       24,   24,   24,   24,   24,   24,   24,   24,   24,    0,
        0,    1,   31,   31,   31,   31,   31,   31,   31,   31,
       31,   31,    1,   89,   66,    1,    1,    0,  117,    1,
      117,  102,   46,  120,   66,  120,   19,   89,    1,    0,
        1,    1,   19,    1,    1,   46,  102,    1,    2,    2,
        2,    2,   75,   17,    2,   17,   17,   17,  121,   17,
       75,   44,    2,  121,   17,   44,   17,   92,   92,   17,
       94,  425,   17,  185,   17,   78,   94,   78,    2,   32,

       32,   32,   32,   32,   32,   32,   32,   32,   32,    2,
      185,   78,    2,    2,   67,   67,    2,  100,   67,  424,
      100,   67,   74,   91,   74,    2,  150,    2,    2,  150,
        2,    2,   67,   91,    2,    3,   74,   72,   74,   72,
       91,   98,  118,  118,    3,   98,    3,   72,   72,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    4,
       90,  101,  101,  101,   90,   98,  423,   90,    4,   90,
        4,   88,   88,    4,    4,    4,    4,    4,    4,    4,
        4,    4,    4,    5,  141,  421,    5,  119,   88,  129,
      141,  119,  116,    5,    5,    5,  119,  129,  420,    5,

        5,    5,    5,    5,    5,    5,    5,    5,    5,  116,
      116,  131,  131,    5,  131,  419,   79,  418,   79,   79,
      131,    5,    5,    5,    5,  148,  131,  131,   79,    5,
        6,  131,   79,    6,   79,  133,  416,   79,  415,  133,
        6,    6,    6,  148,  145,  186,    6,    6,    6,    6,
        6,    6,    6,    6,    6,    6,  145,  146,  146,  190,
        6,  186,  190,  413,  410,  409,  408,  217,    6,    6,
        6,    6,  217,  407,  405,  404,    6,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,

        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,

        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   30,   30,   30,   30,   30,   30,   30,   30,

       30,   30,   62,   62,   62,   62,   62,   62,   62,   62,
       62,   62,  401,   97,  398,  392,   30,   97,   30,   30,
       30,  290,   30,  390,   97,  290,  389,   30,  388,   30,
       97,  387,   30,   97,  386,   30,  384,   30,   34,   34,
      382,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,

       34,   34,   34,   34,   34,   34,   34,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   35,   35,   35,   35,
       35,   35,   35,   35,   35,   35,   38,  381,   38,  377,
       38,   38,   38,   38,   38,  376,  375,   38,   38,   38,
       38,   38,  374,   38,   38,  371,   38,   45,   45,   45,

       45,  369,  368,  201,  201,   45,  201,  367,   45,   45,
       45,   45,   45,   84,   84,   45,   45,   45,   70,  364,
      363,   70,   70,  201,  361,   84,  359,  358,   84,   84,
       70,   77,   77,   70,   70,   70,   70,  357,  356,   77,
      355,  354,  138,   77,  350,   77,  344,  343,  342,  341,
      138,   77,  138,   77,  138,  138,  340,  339,  338,  138,
      429,  429,  430,  430,  337,  336,  334,  333,  331,  330,
      329,  328,  327,  326,  325,  322,  317,  314,  309,  308,
      306,  305,  304,  303,  302,  299,  295,  294,  292,  291,
      289,  288,  287,  285,  284,  282,  281,  280,  279,  278,

      277,  276,  226,  225,  224,  223,  222,  218,  216,  215,
      214,  213,  212,  202,  197,  195,  194,  193,  192,  191,
      189,  188,  187,  184,  183,  182,  181,  180,  179,  178,
      177,  176,  175,  174,  173,  172,  171,  170,  169,  168,
      167,  165,  164,  163,  162,  160,  159,  158,  157,  156,
      155,  154,  153,  152,  149,  147,  143,  142,  140,  139,
      137,  136,  135,  134,  132,  130,  128,  127,  126,  125,
      124,  123,  122,  115,  114,  113,  112,  111,  110,  109,
      108,  107,  106,  104,   99,   96,   95,   93,   87,   86,
       85,   83,   82,   81,   80,   76,   73,   71,   69,   68,

       59,   51,   48,   47,   43,   42,   41,   40,   39,   25,
       16,   11,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
      428,  428,  428,  428,  428,  428
    } ;

static uint atc_last_accepting_state;
static char *atc_last_accepting_cpos;

char *atctext;
#define INITIAL 0
#include "sci_types.h"
//#include "sdl_api.h"

#include "atc_common.h"
#include "atc_sat_def.h"

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
                if ( atc_current_state >= 429 )
                    atc_c = atc_meta[(uint) atc_c];
                }
            atc_current_state = atc_nxt[atc_base[atc_current_state] + (uint) atc_c];
            ++atc_cp;
            }
        while ( atc_base[atc_current_state] != 913 );

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
{ atclval.cmd_index = AT_CMD_Q; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 11:
{ atclval.cmd_index = AT_CMD_V; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 12:
{ atclval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 13:
{ atclval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 14:
{ atclval.cmd_index = AT_CMD_Z; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 15:
{ atclval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 16:
{ atclval.cmd_index = AT_CMD_and_F; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 17:
{ atclval.cmd_index = AT_CMD_and_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 18:
{ atclval.cmd_index = AT_CMD_and_C; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 19:
{ atclval.cmd_index = AT_CMD_and_W; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 20:
{ atclval.cmd_index = AT_CMD_W; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 21:
{ atclval.cmd_index = AT_CMD_THREE_PERCENT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 22:
{ atclval.cmd_index = AT_CMD_S0; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 23:
{ atclval.cmd_index = AT_CMD_S2; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 24:
{ atclval.cmd_index = AT_CMD_S3; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 25:
{ atclval.cmd_index = AT_CMD_S4; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 26:
{ atclval.cmd_index = AT_CMD_S5; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 27:
{ atclval.cmd_index = AT_CMD_S6; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 28:
{ atclval.cmd_index = AT_CMD_S7; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 29:
{ atclval.cmd_index = AT_CMD_S8; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 30:
{ atclval.cmd_index = AT_CMD_S10; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 31:
{ atclval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 32:
{ atclval.cmd_index = AT_CMD_CCFC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 33:
{ atclval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 34:
{ atclval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 35:
{ atclval.cmd_index = AT_CMD_CRC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 36:
{ atclval.cmd_index = AT_CMD_CHLD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 37:
{ atclval.cmd_index = AT_CMD_CLCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 38:
{ atclval.cmd_index = AT_CMD_COPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 39:
{ atclval.cmd_index = AT_CMD_CSSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 40:
{ atclval.cmd_index = AT_CMD_SIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 41:
{ atclval.cmd_index = AT_CMD_CSQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 42:
{ atclval.cmd_index = AT_CMD_CIMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 43:
{ atclval.cmd_index = AT_CMD_CGMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 44:
{ atclval.cmd_index = AT_CMD_CGMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 45:
{ atclval.cmd_index = AT_CMD_CGSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 46:
{ atclval.cmd_index = AT_CMD_CGMR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 47:
{ atclval.cmd_index = AT_CMD_CMEE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 48:
{ atclval.cmd_index = AT_CMD_CSCS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 49:
{ atclval.cmd_index = AT_CMD_CPOF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 50:
{ atclval.cmd_index = AT_CMD_CPOL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 51:
{ atclval.cmd_index = AT_CMD_CGDCONT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 52:
{ atclval.cmd_index = AT_CMD_CGPCO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 53:
{ atclval.cmd_index = AT_CMD_CGQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 54:
{ atclval.cmd_index = AT_CMD_CGQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 55:
{ atclval.cmd_index = AT_CMD_CGATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 56:
{ atclval.cmd_index = AT_CMD_CGACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 57:
{ atclval.cmd_index = AT_CMD_CGPADDR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 58:
{ atclval.cmd_index = AT_CMD_CGDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 59:
{ atclval.cmd_index = AT_CMD_CGAUTO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 60:
{ atclval.cmd_index = AT_CMD_CGEREP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 61:
{ atclval.cmd_index = AT_CMD_CGREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 62:
{ atclval.cmd_index = AT_CMD_CSMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 63:
{ atclval.cmd_index = AT_CMD_CSDH; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 64:
{ atclval.cmd_index = AT_CMD_CRES; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 65:
{ atclval.cmd_index = AT_CMD_CPMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 66:
{ atclval.cmd_index = AT_CMD_CMGF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 67:
{ atclval.cmd_index = AT_CMD_CMGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 68:
{ atclval.cmd_index = AT_CMD_CMGL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 69:
{ atclval.cmd_index = AT_CMD_CMGD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 70:
{ atclval.cmd_index = AT_CMD_CMGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 71:
{ atclval.cmd_index = AT_CMD_CMSS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 72:
{ atclval.cmd_index = AT_CMD_CSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 73:
{ atclval.cmd_index = AT_CMD_CMGC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 74:
{ atclval.cmd_index = AT_CMD_CMGW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 75:
{ atclval.cmd_index = AT_CMD_CSCA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 76:
{ atclval.cmd_index = AT_CMD_CSAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 77:
{ atclval.cmd_index = AT_CMD_SMSC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 78:
{ atclval.cmd_index = AT_CMD_SUSS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 79:
{ atclval.cmd_index = AT_CMD_CNMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 80:
{ atclval.cmd_index = AT_CMD_CPBR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 81:
{ atclval.cmd_index = AT_CMD_CPBF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 82:
{ atclval.cmd_index = AT_CMD_CPBW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 83:
{ atclval.cmd_index = AT_CMD_CPBP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 84:
{ atclval.cmd_index = AT_CMD_CPBN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 85:
{ atclval.cmd_index = AT_CMD_CNUM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 86:
{ atclval.cmd_index = AT_CMD_CSVM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 87:
{ atclval.cmd_index = AT_CMD_CPBS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 88:
{ atclval.cmd_index = AT_CMD_CPUC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 89:
{ atclval.cmd_index = AT_CMD_CUSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 90:
{ atclval.cmd_index = AT_CMD_CACM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 91:
{ atclval.cmd_index = AT_CMD_CAMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 92:
{ atclval.cmd_index = AT_CMD_CCLK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 93:
{ atclval.cmd_index = AT_CMD_XX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 94:
{ atclval.cmd_index = AT_CMD_VTD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 95:
{ atclval.cmd_index = AT_CMD_CCID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 96:
{ atclval.cmd_index = AT_CMD_VGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 97:
{ atclval.cmd_index = AT_CMD_CRSL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 98:
{ atclval.cmd_index = AT_CMD_CMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 99:
{ atclval.cmd_index = AT_CMD_VGT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 100:
{ atclval.cmd_index = AT_CMD_SPEAKER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 101:
{ atclval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 102:
{ atclval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 103:
{ atclval.cmd_index = AT_CMD_SDTMF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 104:
{ atclval.cmd_index = AT_CMD_ECHO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 105:
{ atclval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 106:
{ atclval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 107:
{ atclval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 108:
{ atclval.cmd_index = AT_CMD_SPLO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 109:
{ atclval.cmd_index = AT_CMD_CICB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 110:
{ atclval.cmd_index = AT_CMD_CFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 111:
{ atclval.cmd_index = AT_CMD_IPR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 112:
{ atclval.cmd_index = AT_CMD_CIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 113:
{ atclval.cmd_index = AT_CMD_CCED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 114:
{ atclval.cmd_index = AT_CMD_STONE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 115:
{ atclval.cmd_index = AT_CMD_SAC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 116:
{ atclval.cmd_index = AT_CMD_SATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 117:
{ atclval.cmd_index = AT_CMD_CEER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 118:
{ atclval.cmd_index = AT_CMD_CR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 119:
{ atclval.cmd_index = AT_CMD_SDCP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 120:
{ atclval.cmd_index = AT_CMD_SNVM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 121:
{ atclval.cmd_index = AT_CMD_SBOOT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 122:
{ atclval.cmd_index = AT_CMD_SDMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 123:
{ atclval.cmd_index = AT_CMD_SLOG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 124:
{ atclval.cmd_index = AT_CMD_SUPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 125:
{ atclval.cmd_index = AT_CMD_SSYS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 126:
{ atclval.cmd_index = AT_CMD_SCPFRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 127:
{ atclval.cmd_index = AT_CMD_CGEQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 128:
{ atclval.cmd_index = AT_CMD_CGEQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 129:
{ atclval.cmd_index = AT_CMD_CRSM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 130:
{ atclval.cmd_index = AT_CMD_SCINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 131:
{ atclval.cmd_index = AT_CMD_SPEQPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 132:
{ atclval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 133:
{ atclval.cmd_index = AT_CMD_CMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 134:
{ atclval.cmd_index = AT_CMD_SIME; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 135:
{ atclval.cmd_index = AT_CMD_SCMGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 136:
{ atclval.cmd_index = AT_CMD_SCMGW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 137:
{ atclval.cmd_index = AT_CMD_PSORGMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 138:
{ atclval.cmd_index = AT_CMD_SCREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 139:
{ atclval.cmd_index = AT_CMD_CIEV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 140:
{ atclval.cmd_index = AT_CMD_NETAPN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 141:
{ atclval.cmd_index = AT_CMD_SOCKET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 142:
{ atclval.cmd_index = AT_CMD_IPOPEN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 143:
{ atclval.cmd_index = AT_CMD_IPSEND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 144:
{ atclval.cmd_index = AT_CMD_IPCLOSE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 145:
{ atclval.cmd_index = AT_CMD_IPDEACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 146:
{ atclval.cmd_index = AT_CMD_GSMSTATUS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 147:
{ atclval.cmd_index = AT_CMD_FIRBOOT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 148:
{ atclval.cmd_index = AT_CMD_VGSMSW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 149:
{ atclval.cmd_index = AT_CMD_POSI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 150:
{ atclval.cmd_index = AT_CMD_SMMSWAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 151:
{ atclval.cmd_index = AT_CMD_SADMDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 152:
{ atclval.cmd_index = AT_CMD_SPADLVVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 153:
{ atclval.cmd_index = AT_CMD_SPAUDVOLTYPE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 154:
{ atclval.cmd_index = AT_CMD_SPUSIMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 155:
{ return NUMBER;  }
case 156:
{ return STRING; }
case 157:
{ BEGIN(INITIAL); return DIALSTRING; }
case 158:
{ BEGIN(INITIAL); return SHARPSTRING; }
case 159:
{ BEGIN(INITIAL); return DTMFSTRING; }
case ATC_STATE_EOF(INITIAL):
case ATC_STATE_EOF(AT_CMD):
case ATC_STATE_EOF(AT_DIAL_CMD):
case ATC_STATE_EOF(AT_SHARP_CMD):
case ATC_STATE_EOF(AT_DTMF_CMD):
{return 0;}
case 160:
{ }
    break;
case 161:
{ return atctext[0]; }
case 162:
    break;

    case ATC_END_OF_BUFFER:
        return 0;
    default:
        return -1;    } /* end of action switch */
        } /* end of scanning one token */
    } /* end of atclex */




/*lint +e716*/ 
/*lint +e18*/ 
