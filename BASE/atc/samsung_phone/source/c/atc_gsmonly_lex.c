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

#define ATC_NUM_RULES 146
#define ATC_END_OF_BUFFER 147
static const uint16 atc_accept[480] =
    {   0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      147,  145,  144,  146,    3,  145,  145,    2,   20,    4,
       22,   21,  145,  145,  145,  145,  139,  141,  142,  143,
      144,    5,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        1,  137,    0,  140,  139,  141,  142,  143,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,    0,    0,    0,    0,    0,  128,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,  120,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,   82,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,   31,    9,    0,    0,    0,  109,   41,

      107,  131,  122,   30,   25,  121,  130,  123,  117,    8,
       24,  119,  135,  113,  102,   98,  110,  114,  115,  125,
       95,  136,   32,  112,  106,   29,   94,  127,   97,  105,
      104,   96,   92,  103,    6,  101,   91,    7,   93,  124,
      111,  138,  108,  100,   28,   99,  118,   74,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       39,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  129,   27,   44,  116,    0,    0,    0,    0,  132,
        0,   42,   37,   38,   33,  126,   45,    0,    0,    0,

        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,   83,    0,    0,    0,    0,    0,   34,   47,    0,
       43,    0,    0,    0,    0,    0,   51,    0,    0,    0,
        0,    0,   49,    0,   50,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,   46,    0,    0,    0,    0,  134,    0,    0,
       52,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       40,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,   79,   67,   80,   14,   84,   77,   86,   57,   85,
       89,    0,    0,    0,    0,   54,   10,   59,   78,   73,
       64,   72,   69,   56,   11,  133,   60,   71,   12,   15,
        0,    0,   81,   53,   48,   58,   55,   66,    0,    0,
        0,   61,    0,   13,    0,    0,    0,    0,    0,    0,
        0,    0,   90,    0,    0,   16,   63,   65,    0
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
       51,   52,   53,   54,   55,   56,   57,   58,   59,   44,
       30,    1,   31,   32,   33,   34,   35,   36,   37,   38,

       39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
       49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
       59,   44,   60,   61,   62,   63,   64,    1,    1,    1,
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

static const int atc_meta[65] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1
    } ;

static const uint16 atc_base[482] =
    {   0,
        0,   52,  104,    0,  153,  187,  225,    0,  289,    0,
      853,  854,    4,  854,  854,  844,  319,  854,  797,  854,
      854,  854,  829,  373,    0,    4,    6,  433,  467,  527,
        5,  854,  804,  813,  557,  808,  799,    6,  806,  805,
      796,    5,    3,   26,  789,    9,  579,  802,    9,  782,
      854,  854,    0,  854,    8,    0,    0,    0,  786,  784,
        0,   27,  782,   17,    8,   48,   25,   25,   80,   35,
       93,  783,  793,  780,  794,  795,   57,  777,  795,  790,
      769,  773,   61,  777,  767,  771,  777,  773,  786,  766,
      781,  783,   35,  770,   29,   46,   66,  767,  762,  110,

      757,   82,  763,   65,  765,  759,  757,  854,  770,  754,
      764,  769,  767,  767,  765,  757,  760,  765,  751,  755,
      759,   87,   83,  114,  743,  757,  741,  739,  749,  744,
      740,  736,  735,  129,  739,  733,  739,  746,  742,  736,
      112,  739,  100,  854,  742,  724,  743,  730,  722,  721,
      724,  723,  119,  734,  728,  717,  717,  723,  728,  716,
      725,  714,  726,  709,  709,  722,  703,  711,  709,  708,
      715,  699,  710,  124,  705,  708,  145,  712,  708,  711,
      709,  145,  693,  696,  705,  705,  147,  707,  688,  687,
      686,  695,  687,  854,  854,  697,  681,  696,  854,  854,

      854,  854,  854,  854,  854,  854,  854,  854,  854,  854,
      854,  854,  854,  854,  854,  854,  854,  854,  854,  854,
      854,  854,  854,  854,  854,  854,  854,  854,  854,  854,
      854,  854,  854,  854,  854,  854,  854,  854,  854,  854,
      854,  854,  854,  854,  854,  854,  854,  854,  687,  678,
      682,  676,  675,  674,  160,  688,  683,  671,  682,  680,
      670,  669,  666,  673,  669,  679,  677,  674,  662,  674,
      674,  673,  670,  657,  654,  672,  161,  667,  652,  656,
      664,  854,  854,  854,  854,  656,  659,  650,  645,  854,
      658,  854,  854,  854,  854,  854,  854,  651,  648,  641,

      656,  654,  657,  642,  651,  637,  649,  648,  647,  650,
      638,  630,  634,  642,  631,  631,  639,  642,  637,  626,
      617,  854,  636,  625,  618,  618,  616,  854,  854,  629,
      854,  621,  631,  619,  611,  616,  854,  608,  605,  625,
      624,  619,  854,  618,  854,  617,  607,  601,  599,  615,
      598,  597,  596,  595,  594,  592,  599,  590,  603,  589,
      589,  599,  604,  584,  583,  598,  594,  575,  579,  574,
      581,  573,  854,  573,  566,  565,  554,  854,  563,  545,
      854,  547,  545,  557,  542,  544,  420,  428,  421,  411,
      410,  409,  408,  407,  424,  163,  410,  408,  402,  399,

      416,  413,  410,  393,  401,  393,  392,  405,  399,  400,
      854,  390,  341,  334,  322,  331,  328,  328,  167,  170,
      165,  854,  854,  854,  854,  854,  854,  854,  854,  854,
      854,  180,  170,  173,  155,  854,  854,  854,  854,  854,
      854,  854,  854,  854,  854,  854,  854,  854,  854,  854,
      172,  151,  854,  854,  854,  854,  854,  854,  171,  148,
      147,  854,  152,  854,  131,  122,  118,  109,  103,   88,
       86,   57,  854,   30,   21,  854,  854,  854,  854,   10,
        0
    } ;

static const uint16 atc_def[482] =
    {   0,
      480,  480,  480,    3,  481,  481,  479,    7,  479,    9,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,   17,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,   24,  479,  479,   28,   29,   30,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,

      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,

      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,

      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,

      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,    0,  479,
      479
    } ;

static const uint16 atc_nxt[919] =
    {   0,
       14,   13,   14,   15,   13,   31,   31,   16,   31,   31,
       12,  479,  479,  479,  479,   17,  479,  479,  479,  479,
       55,   55,  479,   18,   55,   55,   55,   55,   55,   55,
      479,  479,  479,  479,   19,  479,  479,   20,   21,  479,
       76,   22,   77,   81,  122,  111,  479,   89,  112,  106,
      123,   83,   23,   13,   14,   15,   13,   82,   84,   16,
       85,   90,  107,  120,  121,  113,  114,   17,   86,  115,
      131,  129,  116,  139,  132,   18,  478,  170,   87,  130,
      173,  117,  174,  118,  171,  477,   19,  140,  124,   20,
       21,  476,  150,   22,  125,  151,  126,  158,  175,  176,

      127,  159,  128,  152,   23,   13,   14,   15,   13,  160,
       24,   16,  177,  178,  133,  134,  187,  191,  192,   25,
      188,   26,  135,  209,   27,   27,  136,   18,  137,  141,
      142,  210,  211,  475,  212,  474,  138,  473,   19,  143,
      189,   20,   21,  144,  181,   22,  241,  242,  182,  245,
      213,  214,  246,  215,  183,  184,   23,   28,  255,  216,
       28,  185,  472,  277,  243,  217,  218,   28,  229,   28,
      219,  471,  256,   28,   28,  470,  230,  278,  231,   28,
      232,  233,  281,  282,  469,  234,  468,   28,   28,   28,
       28,   28,  287,  293,   28,  288,  294,  310,  333,  467,

      466,   28,  432,   28,  433,  465,  464,   28,   28,  463,
      462,  461,  460,   28,  311,  334,  434,  459,  458,  457,
      456,   28,   28,   28,   28,   14,   14,   14,   14,   29,
       29,   14,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   14,
       14,   14,   14,   30,   30,   30,   30,   30,   30,   30,

       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   33,   34,   35,   36,   37,   38,   39,
       40,   41,  455,   42,   43,   44,  454,  453,   45,  452,
       46,   47,   48,  451,   49,  450,   50,   53,   53,   54,
       53,   53,   53,   53,   53,   53,   53,   53,   53,   53,
       53,   53,   53,   53,   53,   53,   53,   53,   53,   53,

       53,   53,   53,   53,   53,   53,   53,   53,   53,   53,
       53,   53,   53,   53,   53,   53,   53,   53,   53,   53,
       53,   53,   53,   53,   53,   53,   53,   53,   53,   53,
       53,   53,   53,   53,   53,   53,   53,   56,  449,  448,
       56,  447,  446,  445,  444,  443,  442,   56,  441,   56,
      440,  439,  438,   56,   56,  437,  436,  435,  431,   56,
      430,  429,  428,  427,  426,  425,  424,   56,   56,   56,
       56,   57,   57,  423,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,

       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
       57,   58,   58,   58,   58,   58,   58,   58,   58,   58,
       58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
       58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
       58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
       58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
       58,   58,   58,   58,   58,   58,   58,   58,   58,   58,
       58,   61,  422,   62,  421,  420,   63,  419,  418,   64,

      417,  416,   65,   66,   67,   68,   69,  415,   70,   71,
      414,   72,   73,   91,   92,   93,   94,   95,   96,  413,
      412,   97,  411,  410,   98,   99,  409,  408,  100,  407,
      101,  102,  406,  103,  405,  404,  403,  104,  402,  401,
      400,  399,  398,  397,  396,  395,  394,  393,  392,  391,
      390,  389,  388,  387,  386,  385,  384,  383,  382,  381,
      380,  379,  378,  377,  376,  375,  374,  373,  372,  371,
      370,  369,  368,  367,  366,  365,  364,  363,  362,  361,
      360,  359,  358,  357,  356,  355,  354,  353,  352,  351,
      350,  349,  348,  347,  346,  345,  344,  343,  342,  341,

      340,  339,  338,  337,  336,  335,  332,  331,  330,  329,
      328,  327,  326,  325,  324,  323,  322,  321,  320,  319,
      318,  317,  316,  315,  314,  313,  312,  309,  308,  307,
      306,  305,  304,  303,  302,  301,  300,  299,  298,  297,
      296,  295,  292,  291,  290,  289,  286,  285,  284,  283,
      280,  279,  276,  275,  274,  273,  272,  271,  270,  269,
      268,  267,  266,  265,  264,  263,  262,  261,  260,  259,
      258,  257,  254,  253,  252,  251,  250,  249,  248,  247,
      244,  240,  239,  238,  237,  236,  235,  228,  227,  226,
      225,  224,  223,  222,  221,  220,  208,  207,  206,  205,

      204,  203,  202,  201,  200,  199,  198,  197,  196,  195,
      194,  193,  190,  186,  180,  179,  172,  169,  168,  167,
      166,  165,  164,  163,  162,  161,  157,  156,  155,  154,
      153,  149,  148,  147,  146,  145,  119,  110,  109,  108,
      105,   88,   80,   79,   78,   75,   74,   60,   59,   52,
       51,   32,  479,   11,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,

      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479
    } ;

static const uint16 atc_chk[919] =
    {   0,
      481,    1,    1,    1,    1,   13,   31,    1,   13,   31,
      480,    0,    0,    0,    0,    1,    0,    0,    0,    0,
       25,   25,    0,    1,   26,   26,   27,   27,   55,   55,
        0,    0,    0,    0,    1,    0,    0,    1,    1,    0,
       38,    1,   38,   42,   65,   61,    0,   46,   61,   49,
       65,   43,    1,    2,    2,    2,    2,   42,   43,    2,
       44,   46,   49,   64,   64,   62,   62,    2,   44,   62,
       68,   67,   62,   70,   68,    2,  475,   93,   44,   67,
       95,   62,   95,   62,   93,  474,    2,   70,   66,    2,
        2,  472,   77,    2,   66,   77,   66,   83,   96,   96,

       66,   83,   66,   77,    2,    3,    3,    3,    3,   83,
        3,    3,   97,   97,   69,   69,  102,  104,  104,    3,
      102,    3,   69,  122,    3,    3,   69,    3,   69,   71,
       71,  122,  123,  471,  123,  470,   69,  469,    3,   71,
      102,    3,    3,   71,  100,    3,  141,  141,  100,  143,
      124,  124,  143,  124,  100,  100,    3,    5,  153,  124,
        5,  100,  468,  174,  141,  124,  124,    5,  134,    5,
      124,  467,  153,    5,    5,  466,  134,  174,  134,    5,
      134,  134,  177,  177,  465,  134,  463,    5,    5,    5,
        5,    6,  182,  187,    6,  182,  187,  255,  277,  461,

      460,    6,  396,    6,  396,  459,  452,    6,    6,  451,
      435,  434,  433,    6,  255,  277,  396,  432,  421,  420,
      419,    6,    6,    6,    6,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,

        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,   17,   17,   17,   17,   17,   17,   17,
       17,   17,  418,   17,   17,   17,  417,  416,   17,  415,
       17,   17,   17,  414,   17,  413,   17,   24,   24,   24,
       24,   24,   24,   24,   24,   24,   24,   24,   24,   24,
       24,   24,   24,   24,   24,   24,   24,   24,   24,   24,

       24,   24,   24,   24,   24,   24,   24,   24,   24,   24,
       24,   24,   24,   24,   24,   24,   24,   24,   24,   24,
       24,   24,   24,   24,   24,   24,   24,   24,   24,   24,
       24,   24,   24,   24,   24,   24,   24,   28,  412,  410,
       28,  409,  408,  407,  406,  405,  404,   28,  403,   28,
      402,  401,  400,   28,   28,  399,  398,  397,  395,   28,
      394,  393,  392,  391,  390,  389,  388,   28,   28,   28,
       28,   29,   29,  387,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,

       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   29,   29,   29,   29,   29,   29,   29,   29,   29,
       29,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   30,   30,   30,   30,   30,   30,   30,   30,   30,
       30,   35,  386,   35,  385,  384,   35,  383,  382,   35,

      380,  379,   35,   35,   35,   35,   35,  377,   35,   35,
      376,   35,   35,   47,   47,   47,   47,   47,   47,  375,
      374,   47,  372,  371,   47,   47,  370,  369,   47,  368,
       47,   47,  367,   47,  366,  365,  364,   47,  363,  362,
      361,  360,  359,  358,  357,  356,  355,  354,  353,  352,
      351,  350,  349,  348,  347,  346,  344,  342,  341,  340,
      339,  338,  336,  335,  334,  333,  332,  330,  327,  326,
      325,  324,  323,  321,  320,  319,  318,  317,  316,  315,
      314,  313,  312,  311,  310,  309,  308,  307,  306,  305,
      304,  303,  302,  301,  300,  299,  298,  291,  289,  288,

      287,  286,  281,  280,  279,  278,  276,  275,  274,  273,
      272,  271,  270,  269,  268,  267,  266,  265,  264,  263,
      262,  261,  260,  259,  258,  257,  256,  254,  253,  252,
      251,  250,  249,  198,  197,  196,  193,  192,  191,  190,
      189,  188,  186,  185,  184,  183,  181,  180,  179,  178,
      176,  175,  173,  172,  171,  170,  169,  168,  167,  166,
      165,  164,  163,  162,  161,  160,  159,  158,  157,  156,
      155,  154,  152,  151,  150,  149,  148,  147,  146,  145,
      142,  140,  139,  138,  137,  136,  135,  133,  132,  131,
      130,  129,  128,  127,  126,  125,  121,  120,  119,  118,

      117,  116,  115,  114,  113,  112,  111,  110,  109,  107,
      106,  105,  103,  101,   99,   98,   94,   92,   91,   90,
       89,   88,   87,   86,   85,   84,   82,   81,   80,   79,
       78,   76,   75,   74,   73,   72,   63,   60,   59,   50,
       48,   45,   41,   40,   39,   37,   36,   34,   33,   23,
       19,   16,   11,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,

      479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
      479,  479,  479,  479,  479,  479,  479,  479
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
                if ( atc_current_state >= 480 )
                    atc_c = atc_meta[(uint) atc_c];
                }
            atc_current_state = atc_nxt[atc_base[atc_current_state] + (uint) atc_c];
            ++atc_cp;
            }
        while ( atc_base[atc_current_state] != 854 );

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
{ atclval.cmd_index = AT_CMD_IMEITEST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 11:
{ atclval.cmd_index = AT_CMD_MSLSECUR; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 12:
{ atclval.cmd_index = AT_CMD_SERIALNO; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 13:
{ atclval.cmd_index = AT_CMD_SETTESTNV; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 14:
{ atclval.cmd_index = AT_CMD_DETALOCK; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 15:
{ atclval.cmd_index = AT_CMD_SETFDATA; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 16:
{ atclval.cmd_index = AT_CMD_SETFULLFDATA; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 17:
{ atclval.cmd_index = AT_CMD_AT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 18:
{ atclval.cmd_index = AT_CMD_SHARP_SHARP; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 19:
{ atclval.cmd_index = AT_CMD_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 20:
{ atclval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 21:
{ atclval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 22:
{ atclval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 23:
{ atclval.cmd_index = AT_CMD_CPIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 24:
{ atclval.cmd_index = AT_CMD_CLIP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 25:
{ atclval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 26:
{ atclval.cmd_index = AT_CMD_VTS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 27:
{ atclval.cmd_index = AT_CMD_SIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 28:
{ atclval.cmd_index = AT_CMD_CSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 29:
{ atclval.cmd_index = AT_CMD_COLP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 30:
{ atclval.cmd_index = AT_CMD_CCUG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 31:
{ atclval.cmd_index = AT_CMD_VGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 32:
{ atclval.cmd_index = AT_CMD_CMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 33:
{ atclval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 34:
{ atclval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 35:
{ atclval.cmd_index = AT_CMD_CLCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 36:
{ atclval.cmd_index = AT_CMD_CPWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 37:
{ atclval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 38:
{ atclval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 39:
{ atclval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 40:
{ atclval.cmd_index = AT_CMD_SADMDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 41:
{ atclval.cmd_index = AT_CMD_CCED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 42:
{ atclval.cmd_index = AT_CMD_SRWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 43:
{ atclval.cmd_index = AT_CMD_SDMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 44:
{ atclval.cmd_index = AT_CMD_SLOG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 45:
{ atclval.cmd_index = AT_CMD_SUPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 46:
{ atclval.cmd_index = AT_CMD_SCPFRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 47:
{ atclval.cmd_index = AT_CMD_SCINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 48:
{ atclval.cmd_index = AT_CMD_SPEQPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 49:
{ atclval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 50:
{ atclval.cmd_index = AT_CMD_SYTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 51:
{ atclval.cmd_index = AT_CMD_SFTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 52:
{ atclval.cmd_index = AT_CMD_SPENHA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 53:
{ atclval.cmd_index = AT_CMD_SPADLVVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 54:
{ atclval.cmd_index = AT_CMD_HEADINFO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 55:
{ atclval.cmd_index = AT_CMD_SYSSLEEP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 56:
{ atclval.cmd_index = AT_CMD_MAXPOWER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 57:
{ atclval.cmd_index = AT_CMD_FCEPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 58:
{ atclval.cmd_index = AT_CMD_SPKSTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 59:
{ atclval.cmd_index = AT_CMD_KEYSHORT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 60:
{ atclval.cmd_index = AT_CMD_READRSSI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 61:
{ atclval.cmd_index = AT_CMD_GETTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 62:
{ atclval.cmd_index = AT_CMD_SETTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 63:
{ atclval.cmd_index = AT_CMD_GETFULLHISTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 64:
{ atclval.cmd_index = AT_CMD_LOGERASE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 65:
{ atclval.cmd_index = AT_CMD_GETFULLTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 66:
{ atclval.cmd_index = AT_CMD_TEMPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 67:
{ atclval.cmd_index = AT_CMD_BATTTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 68:
{ atclval.cmd_index = AT_CMD_IMEITEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 69:
{ atclval.cmd_index = AT_CMD_LVOFLOCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 70:
{ atclval.cmd_index = AT_CMD_DETALOCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 71:
{ atclval.cmd_index = AT_CMD_RSTVERIF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 72:
{ atclval.cmd_index = AT_CMD_LOOPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 73:
{ atclval.cmd_index = AT_CMD_LOCKREAD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 74:
{ atclval.cmd_index = AT_CMD_CVHU; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 75:
{ atclval.cmd_index = AT_CMD_MSLSECUR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 76:
{ atclval.cmd_index = AT_CMD_SERIALNO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 77:
{ atclval.cmd_index = AT_CMD_FACTORST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 78:
{ atclval.cmd_index = AT_CMD_KSTRINGB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 79:
{ atclval.cmd_index = AT_CMD_AKSEEDNO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 80:
{ atclval.cmd_index = AT_CMD_CALIDATE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 81:
{ atclval.cmd_index = AT_CMD_SIMDETEC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 82:
{ atclval.cmd_index = AT_CMD_SMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 83:
{ atclval.cmd_index = AT_CMD_MISCD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 84:
{ atclval.cmd_index = AT_CMD_EMEMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 85:
{ atclval.cmd_index = AT_CMD_FCMPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 86:
{ atclval.cmd_index = AT_CMD_FCBTTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 87:
{ atclval.cmd_index = AT_CMD_SETFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 88:
{ atclval.cmd_index = AT_CMD_SETFULLFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 89:
{ atclval.cmd_index = AT_CMD_GETFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 90:
{ atclval.cmd_index = AT_CMD_GETFULLFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 91:
{ atclval.cmd_index = AT_CMD_CPOL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 92:
{ atclval.cmd_index = AT_CMD_CPBS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 93:
{ atclval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 94:
{ atclval.cmd_index = AT_CMD_COPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 95:
{ atclval.cmd_index = AT_CMD_CMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 96:
{ atclval.cmd_index = AT_CMD_CPBR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 97:
{ atclval.cmd_index = AT_CMD_CPBF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 98:
{ atclval.cmd_index = AT_CMD_CMGL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 99:
{ atclval.cmd_index = AT_CMD_CSMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 100:
{ atclval.cmd_index = AT_CMD_CSDH; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 101:
{ atclval.cmd_index = AT_CMD_CPMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 102:
{ atclval.cmd_index = AT_CMD_CMGF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 103:
{ atclval.cmd_index = AT_CMD_CPBW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 104:
{ atclval.cmd_index = AT_CMD_CPBP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 105:
{ atclval.cmd_index = AT_CMD_CPBN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 106:
{ atclval.cmd_index = AT_CMD_CNUM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 107:
{ atclval.cmd_index = AT_CMD_CCFC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 108:
{ atclval.cmd_index = AT_CMD_CSCS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 109:
{ atclval.cmd_index = AT_CMD_CAOC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 110:
{ atclval.cmd_index = AT_CMD_CMGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 111:
{ atclval.cmd_index = AT_CMD_CSCA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 112:
{ atclval.cmd_index = AT_CMD_CNMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 113:
{ atclval.cmd_index = AT_CMD_CMGD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 114:
{ atclval.cmd_index = AT_CMD_CMGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 115:
{ atclval.cmd_index = AT_CMD_CMGW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 116:
{ atclval.cmd_index = AT_CMD_SMSC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 117:
{ atclval.cmd_index = AT_CMD_CLCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 118:
{ atclval.cmd_index = AT_CMD_CUSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 119:
{ atclval.cmd_index = AT_CMD_CLIR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 120:
{ atclval.cmd_index = AT_CMD_CSQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 121:
{ atclval.cmd_index = AT_CMD_CFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 122:
{ atclval.cmd_index = AT_CMD_CCLK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 123:
{ atclval.cmd_index = AT_CMD_CIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 124:
{ atclval.cmd_index = AT_CMD_CRSL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 125:
{ atclval.cmd_index = AT_CMD_CMMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 126:
{ atclval.cmd_index = AT_CMD_SSYS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 127:
{ atclval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 128:
{ atclval.cmd_index = AT_CMD_XX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 129:
{ atclval.cmd_index = AT_CMD_SIME; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 130:
{ atclval.cmd_index = AT_CMD_CIMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 131:
{ atclval.cmd_index = AT_CMD_CCID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 132:
{ atclval.cmd_index = AT_CMD_SPLO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 133:
{ atclval.cmd_index = AT_CMD_PSORGMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 134:
{ atclval.cmd_index = AT_CMD_SFSMSM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 135:
{ atclval.cmd_index = AT_CMD_CMGC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 136:
{ atclval.cmd_index = AT_CMD_CMSS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 137:
{ atclval.cmd_index = AT_CMD_S0; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 138:
{ atclval.cmd_index = AT_CMD_CSCB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 139:
{ return NUMBER;  }
case 140:
{ return STRING; }
case 141:
{ BEGIN(INITIAL); return DIALSTRING; }
case 142:
{ BEGIN(INITIAL); return SHARPSTRING; }
case 143:
{ BEGIN(INITIAL); return DTMFSTRING; }
case ATC_STATE_EOF(INITIAL):
case ATC_STATE_EOF(AT_CMD):
case ATC_STATE_EOF(AT_DIAL_CMD):
case ATC_STATE_EOF(AT_SHARP_CMD):
case ATC_STATE_EOF(AT_DTMF_CMD):
{return 0;}
case 144:
{ }
    break;
case 145:
{ return atctext[0]; }
case 146:
    break;

    case ATC_END_OF_BUFFER:
        return 0;
    default:
        return -1;    } /* end of action switch */
        } /* end of scanning one token */
    } /* end of atclex */




/*lint +e716*/ 
/*lint +e18*/ 
