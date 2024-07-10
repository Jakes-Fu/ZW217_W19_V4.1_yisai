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

#define ATC_NUM_RULES 185
#define ATC_END_OF_BUFFER 186
static const uint16 atc_accept[585] =
    {   0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      186,  184,  183,  185,    3,  184,  184,  184,    2,   23,
        4,   26,   24,   21,  184,   22,   25,  184,  184,  184,
      178,  180,  181,  182,  183,    5,   29,   28,   27,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    1,   52,    0,
      179,  178,  180,  181,  182,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,   95,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,   42,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,    0,    0,  168,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
       97,    9,    0,    0,    0,   93,  112,   34,   96,   94,
       91,   35,  110,    0,    0,    0,    0,    0,    0,    0,
       44,   45,   47,    0,    0,    0,    0,    0,    0,   46,
       38,  139,   43,  111,   39,    8,   32,   33,   48,  137,
       76,   72,   69,   71,   70,   73,   77,  136,  138,   74,
       99,  130,   82,   88,   40,   30,   84,   87,   86,   83,
       90,   85,    6,   68,   50,   51,    7,   36,   67,   98,
       79,   78,   81,   49,   66,   75,   65,   92,  160,    0,

        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,  107,  115,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,  131,   41,   80,
        0,    0,    0,    0,    0,  109,    0,  105,  106,  100,
       89,  114,  120,    0,  119,    0,    0,    0,    0,    0,
        0,   58,   57,    0,    0,    0,    0,    0,    0,    0,
       54,    0,    0,   63,   64,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,  169,    0,    0,    0,    0,    0,    0,

      101,  124,  132,    0,  118,  102,    0,    0,    0,    0,
        0,  129,    0,    0,    0,    0,    0,    0,    0,  127,
      113,    0,  128,    0,    0,    0,    0,   15,   61,    0,
        0,    0,   62,    0,   56,   55,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
      121,    0,    0,    0,    0,  125,    0,    0,    0,    0,
      134,    0,    0,    0,    0,    0,    0,    0,   53,  123,
      122,   60,    0,    0,    0,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,    0,    0,

        0,    0,    0,    0,  108,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,    0,    0,  165,  153,
      166,   14,  170,  163,  172,  143,  171,  175,    0,    0,
        0,    0,  140,   10,  145,  164,  159,  150,  158,  155,
      142,   11,  133,  146,  157,  116,   12,   16,    0,    0,
        0,  167,  135,    0,  126,  144,  141,  152,    0,    0,
        0,  147,    0,   13,  117,    0,    0,    0,    0,    0,
        0,    0,    0,    0,    0,    0,  176,    0,    0,   17,
      177,  149,  151,    0
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
       51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
       30,    1,   31,   32,   33,   34,   35,   36,   37,   38,

       39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
       49,   50,   51,   52,   53,   54,   55,   56,   57,   58,
       59,   60,   61,   62,   63,   64,   65,    1,    1,    1,
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

static const int atc_meta[66] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1
    } ;

static const uint16 atc_base[587] =
    {   0,
        0,   59,    5,   12,  115,  149,  187,    0,  252,    0,
      953,  954,    4,  954,  954,  944,    3,  283,  954,  897,
      954,  954,  954,  954,  929,  954,  954,  337,   10,   15,
       23,  398,  432,  493,    5,  954,  954,  954,  954,  904,
      913,  524,  908,  899,   11,  906,  905,  896,   13,    1,
       12,  889,   15,  546,  902,   17,  882,  954,  954,    0,
      954,   51,    0,    0,    0,  886,  884,   28,   45,  882,
       96,   23,   32,   39,  120,   34,  886,   60,   51,  370,
      882,  892,  879,  893,  894,   69,  876,  894,  889,  868,
      872,   77,  876,  866,  870,  876,  872,  885,  865,   73,

      883,  113,   39,   70,   85,  867,   93,  863,  377,  555,
      866,  864,   91,  866,  860,  858,  954,  871,  855,  865,
      870,  868,  868,  866,  858,  861,  866,  852,  118,  852,
      142,  129,  131,  297,   40,  859,  362,  859,  846,  852,
      856,   76,  288,  372,  566,  840,  854,  838,   71,  847,
      842,  835,  834,  576,  838,  832,  122,  846,  373,  837,
      829,  556,  839,  377,  954,  842,  824,  843,  830,  822,
      821,  824,  823,  364,  834,  828,  817,  817,  823,  828,
      816,  825,  814,  826,  809,  809,  822,  803,  811,  803,
      802,  807,  806,  798,  812,  796,  803,  806,  557,  801,

      804,  794,  393,  807,  807,  127,  512,  790,  793,  802,
      528,  805,  789,  784,  784,  788,  782,  781,  790,  782,
      954,  954,  792,  776,  791,  954,  954,  954,  954,  954,
      954,  954,  954,  774,  773,  777,  771,  775,  563,  784,
      954,  954,  954,  784,  772,  777,  780,  777,  764,  954,
      954,  954,  954,  954,  954,  954,  954,  954,  954,  954,
      954,  954,  954,  954,  954,  954,  954,  954,  954,  954,
      954,  954,  954,  954,  954,  954,  954,  954,  954,  954,
      954,  954,  954,  954,  954,  954,  954,  954,  954,  954,
      954,  954,  954,  954,  954,  954,  954,  954,  954,  770,

      761,  765,  759,  758,  757,  575,  771,  766,  754,  765,
      763,  753,  752,  749,  756,  752,  762,  760,  757,  745,
      757,  757,  954,  745,  755,  752,  733,  738,  735,  748,
      752,  579,  747,  732,  736,  730,  743,  954,  954,  954,
      735,  742,  737,  728,  723,  954,  736,  954,  954,  954,
      954,  954,  954,  736,  954,  728,  725,  718,  733,  731,
      734,  954,  954,  730,  732,  718,  722,  725,  713,  724,
      954,  713,  709,  954,  954,  710,  719,  705,  717,  716,
      715,  718,  706,  698,  702,  710,  699,  699,  707,  710,
      705,  694,  685,  954,  704,  693,  686,  686,  684,  701,

      954,  954,  954,  696,  954,  954,  688,  698,  686,  678,
      683,  954,  691,  674,  671,  670,  690,  689,  684,  954,
      954,  683,  954,  682,  672,  666,  664,  954,  954,  663,
      668,  664,  954,  662,  954,  954,  676,  659,  658,  657,
      656,  655,  653,  660,  651,  664,  650,  650,  660,  665,
      646,  645,  660,  657,  640,  645,  640,  649,  641,  636,
      954,  641,  634,  641,  632,  954,  650,  645,  627,  633,
      954,  629,  627,  640,  625,  628,  622,  636,  954,  954,
      954,  954,  629,  619,  618,  617,  616,  615,  633,  585,
      619,  617,  611,  610,  627,  624,  622,  606,  614,  606,

      605,  618,  612,  614,  954,  599,  603,  616,  610,  601,
      594,  610,  607,  599,  609,  589,  592,  587,  954,  954,
      954,  954,  954,  954,  954,  954,  954,  954,  602,  595,
      598,  580,  954,  954,  954,  954,  954,  954,  954,  954,
      954,  954,  954,  954,  954,  954,  954,  954,  597,  576,
      596,  954,  954,  568,  954,  954,  954,  954,  586,  567,
      554,  954,  563,  954,  954,  536,  534,  515,  514,  508,
      389,  393,  283,  279,  123,   67,  954,   43,    3,  954,
      954,  954,  954,  954,   12,    0
    } ;

static const uint16 atc_def[587] =
    {   0,
      585,  585,    2,    2,  586,  586,  584,    7,  584,    9,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,   18,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,   28,
      584,  584,   32,   33,   34,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,

      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,

      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,

      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,

      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,

      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,    0,  584,  584
    } ;

static const uint16 atc_nxt[1020] =
    {   0,
       14,   13,   14,   15,   13,   35,   35,   16,   35,   35,
       17,   28,   12,  584,  584,   18,  584,  584,   28,  584,
       29,  584,   30,   19,  584,   31,   31,   29,  584,   30,
       62,   62,   31,   31,   20,   62,   62,   21,   22,   37,
       38,   23,   39,   62,   62,   85,   94,   86,  584,   92,
       24,   90,   25,   98,   95,   26,   93,  115,  583,   27,
       13,   14,   15,   13,   96,   91,   16,   99,  138,   17,
      116,   62,   62,  120,   18,  142,  121,  139,  140,  141,
      150,  143,   19,  122,  123,  196,  246,  124,  151,  159,
      125,  247,  197,   20,  153,  154,   21,   22,  582,  126,

       23,  127,  155,  160,  171,  581,  156,  172,  157,   24,
      189,   25,  255,  179,   26,  173,  158,  180,   27,   32,
      256,  198,   32,  199,  271,  181,  190,  191,  272,   32,
      129,   32,  130,  131,  132,   32,   32,  200,  201,  203,
      204,   32,  133,  218,  219,  134,  135,  136,  137,   32,
       32,   32,   32,   32,  234,  193,   32,  580,  144,  194,
      145,  285,  195,   32,  341,   32,  146,  286,  147,   32,
       32,  235,  148,  241,  149,   32,  237,  242,  238,  239,
      240,  342,  243,   32,   32,   32,   32,   14,   14,   14,
       14,   33,   33,   14,   33,   33,   33,   33,   33,   33,

       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   14,   14,   14,   14,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,

       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   40,   41,   42,
       43,   44,   45,   46,   47,   48,  579,   49,   50,   51,
      578,  244,   52,  245,   53,   54,   55,  257,   56,  258,
       57,   60,   60,   61,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
       60,   60,   60,   60,   60,   60,   60,   60,   60,   60,

       60,   60,   63,  306,  161,   63,  162,  163,  249,  250,
      259,  206,   63,  288,   63,  207,  164,  307,   63,   63,
      165,  208,  209,  260,   63,  289,  296,  577,  210,  297,
      337,  338,   63,   63,   63,   63,   64,   64,  576,   64,
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
       65,   65,   65,   65,   65,   65,   65,   65,   68,  343,
       69,  575,  344,   70,   71,   72,   73,  574,  573,   74,
       75,   76,   77,   78,  348,   79,   80,  349,   81,   82,
      100,  101,  102,  103,  104,  105,  106,  572,  107,  211,
      292,  293,  108,  212,  571,  109,  332,  570,  110,  111,

      112,  213,  261,  262,  113,  263,  569,  214,  294,  367,
      333,  264,  382,  215,  368,  277,  408,  265,  266,  568,
      567,  566,  267,  278,  529,  279,  530,  280,  281,  383,
      565,  564,  282,  409,  563,  562,  561,  560,  531,  559,
      558,  557,  556,  555,  554,  553,  552,  551,  550,  549,
      548,  547,  546,  545,  544,  543,  542,  541,  540,  539,
      538,  537,  536,  535,  534,  533,  532,  528,  527,  526,
      525,  524,  523,  522,  521,  520,  519,  518,  517,  516,
      515,  514,  513,  512,  511,  510,  509,  508,  507,  506,
      505,  504,  503,  502,  501,  500,  499,  498,  497,  496,

      495,  494,  493,  492,  491,  490,  489,  488,  487,  486,
      485,  484,  483,  482,  481,  480,  479,  478,  477,  476,
      475,  474,  473,  472,  471,  470,  469,  468,  467,  466,
      465,  464,  463,  462,  461,  460,  459,  458,  457,  456,
      455,  454,  453,  452,  451,  450,  449,  448,  447,  446,
      445,  444,  443,  442,  441,  440,  439,  438,  437,  436,
      435,  434,  433,  432,  431,  430,  429,  428,  427,  426,
      425,  424,  423,  422,  421,  420,  419,  418,  417,  416,
      415,  414,  413,  412,  411,  410,  407,  406,  405,  404,
      403,  402,  401,  400,  399,  398,  397,  396,  395,  394,

      393,  392,  391,  390,  389,  388,  387,  386,  385,  384,
      381,  380,  379,  378,  377,  376,  375,  374,  373,  372,
      371,  370,  369,  366,  365,  364,  363,  362,  361,  360,
      359,  358,  357,  356,  355,  354,  353,  352,  351,  350,
      347,  346,  345,  340,  339,  336,  335,  334,  331,  330,
      329,  328,  327,  326,  325,  324,  323,  322,  321,  320,
      319,  318,  317,  316,  315,  314,  313,  312,  311,  310,
      309,  308,  305,  304,  303,  302,  301,  300,  299,  298,
      295,  291,  290,  287,  284,  283,  276,  275,  274,  273,
      270,  269,  268,  254,  253,  252,  251,  248,  236,  233,

      232,  231,  230,  229,  228,  227,  226,  225,  224,  223,
      222,  221,  220,  217,  216,  205,  202,  192,  188,  187,
      186,  185,  184,  183,  182,  178,  177,  176,  175,  174,
      170,  169,  168,  167,  166,  152,  128,  119,  118,  117,
      114,   97,   89,   88,   87,   84,   83,   67,   66,   59,
       58,   36,  584,   11,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,

      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584
    } ;

static const uint16 atc_chk[1020] =
    {   0,
      586,    1,    1,    1,    1,   13,   35,    1,   13,   35,
        1,    3,  585,    0,    0,    1,    0,    0,    4,    0,
        3,    0,    3,    1,    0,    3,    3,    4,    0,    4,
       29,   29,    4,    4,    1,   30,   30,    1,    1,   17,
       17,    1,   17,   31,   31,   45,   51,   45,    0,   50,
        1,   49,    1,   53,   51,    1,   50,   56,  579,    1,
        2,    2,    2,    2,   51,   49,    2,   53,   72,    2,
       56,   62,   62,   68,    2,   74,   68,   72,   73,   73,
       76,   74,    2,   69,   69,  103,  135,   69,   76,   79,
       69,  135,  103,    2,   78,   78,    2,    2,  578,   69,

        2,   69,   78,   79,   86,  576,   78,   86,   78,    2,
      100,    2,  142,   92,    2,   86,   78,   92,    2,    5,
      142,  104,    5,  104,  149,   92,  100,  100,  149,    5,
       71,    5,   71,   71,   71,    5,    5,  105,  105,  107,
      107,    5,   71,  113,  113,   71,   71,   71,   71,    5,
        5,    5,    5,    6,  129,  102,    6,  575,   75,  102,
       75,  157,  102,    6,  206,    6,   75,  157,   75,    6,
        6,  129,   75,  133,   75,    6,  131,  133,  131,  132,
      132,  206,  133,    6,    6,    6,    6,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,

        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
        7,    7,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,

        9,    9,    9,    9,    9,    9,    9,    9,    9,    9,
        9,    9,    9,    9,    9,    9,    9,   18,   18,   18,
       18,   18,   18,   18,   18,   18,  574,   18,   18,   18,
      573,  134,   18,  134,   18,   18,   18,  143,   18,  143,
       18,   28,   28,   28,   28,   28,   28,   28,   28,   28,
       28,   28,   28,   28,   28,   28,   28,   28,   28,   28,
       28,   28,   28,   28,   28,   28,   28,   28,   28,   28,
       28,   28,   28,   28,   28,   28,   28,   28,   28,   28,
       28,   28,   28,   28,   28,   28,   28,   28,   28,   28,
       28,   28,   28,   28,   28,   28,   28,   28,   28,   28,

       28,   28,   32,  174,   80,   32,   80,   80,  137,  137,
      144,  109,   32,  159,   32,  109,   80,  174,   32,   32,
       80,  109,  109,  144,   32,  159,  164,  572,  109,  164,
      203,  203,   32,   32,   32,   32,   33,   33,  571,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   33,   33,   33,
       33,   33,   33,   33,   33,   33,   33,   34,   34,   34,

       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   34,   34,
       34,   34,   34,   34,   34,   34,   34,   34,   42,  207,
       42,  570,  207,   42,   42,   42,   42,  569,  568,   42,
       42,   42,   42,   42,  211,   42,   42,  211,   42,   42,
       54,   54,   54,   54,   54,   54,   54,  567,   54,  110,
      162,  162,   54,  110,  566,   54,  199,  563,   54,   54,

       54,  110,  145,  145,   54,  145,  561,  110,  162,  239,
      199,  145,  306,  110,  239,  154,  332,  145,  145,  560,
      559,  554,  145,  154,  490,  154,  490,  154,  154,  306,
      551,  550,  154,  332,  549,  532,  531,  530,  490,  529,
      518,  517,  516,  515,  514,  513,  512,  511,  510,  509,
      508,  507,  506,  504,  503,  502,  501,  500,  499,  498,
      497,  496,  495,  494,  493,  492,  491,  489,  488,  487,
      486,  485,  484,  483,  478,  477,  476,  475,  474,  473,
      472,  470,  469,  468,  467,  465,  464,  463,  462,  460,
      459,  458,  457,  456,  455,  454,  453,  452,  451,  450,

      449,  448,  447,  446,  445,  444,  443,  442,  441,  440,
      439,  438,  437,  434,  432,  431,  430,  427,  426,  425,
      424,  422,  419,  418,  417,  416,  415,  414,  413,  411,
      410,  409,  408,  407,  404,  400,  399,  398,  397,  396,
      395,  393,  392,  391,  390,  389,  388,  387,  386,  385,
      384,  383,  382,  381,  380,  379,  378,  377,  376,  373,
      372,  370,  369,  368,  367,  366,  365,  364,  361,  360,
      359,  358,  357,  356,  354,  347,  345,  344,  343,  342,
      341,  337,  336,  335,  334,  333,  331,  330,  329,  328,
      327,  326,  325,  324,  322,  321,  320,  319,  318,  317,

      316,  315,  314,  313,  312,  311,  310,  309,  308,  307,
      305,  304,  303,  302,  301,  300,  249,  248,  247,  246,
      245,  244,  240,  238,  237,  236,  235,  234,  225,  224,
      223,  220,  219,  218,  217,  216,  215,  214,  213,  212,
      210,  209,  208,  205,  204,  202,  201,  200,  198,  197,
      196,  195,  194,  193,  192,  191,  190,  189,  188,  187,
      186,  185,  184,  183,  182,  181,  180,  179,  178,  177,
      176,  175,  173,  172,  171,  170,  169,  168,  167,  166,
      163,  161,  160,  158,  156,  155,  153,  152,  151,  150,
      148,  147,  146,  141,  140,  139,  138,  136,  130,  128,

      127,  126,  125,  124,  123,  122,  121,  120,  119,  118,
      116,  115,  114,  112,  111,  108,  106,  101,   99,   98,
       97,   96,   95,   94,   93,   91,   90,   89,   88,   87,
       85,   84,   83,   82,   81,   77,   70,   67,   66,   57,
       55,   52,   48,   47,   46,   44,   43,   41,   40,   25,
       20,   16,   11,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,

      584,  584,  584,  584,  584,  584,  584,  584,  584,  584,
      584,  584,  584,  584,  584,  584,  584,  584,  584
    } ;

static uint atc_last_accepting_state;
static char *atc_last_accepting_cpos;

char *atctext;
#define INITIAL 0
#include "sci_types.h"
//#include "sdl_api.h"

#include "atc_common.h"
#include "atc_def.h"

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
                if ( atc_current_state >= 585 )
                    atc_c = atc_meta[(uint) atc_c];
                }
            atc_current_state = atc_nxt[atc_base[atc_current_state] + (uint) atc_c];
            ++atc_cp;
            }
        while ( atc_base[atc_current_state] != 954 );

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
{ atclval.cmd_index = AT_CMD_CGCMOD; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 16:
{ atclval.cmd_index = AT_CMD_SETFDATA; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 17:
{ atclval.cmd_index = AT_CMD_SETFULLFDATA; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
case 18:
{ atclval.cmd_index = AT_CMD_AT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 19:
{ atclval.cmd_index = AT_CMD_SHARP_SHARP; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 20:
{ atclval.cmd_index = AT_CMD_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 21:
{ atclval.cmd_index = AT_CMD_Q; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 22:
{ atclval.cmd_index = AT_CMD_V; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 23:
{ atclval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 24:
{ atclval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 25:
{ atclval.cmd_index = AT_CMD_Z; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 26:
{ atclval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 27:
{ atclval.cmd_index = AT_CMD_and_F; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 28:
{ atclval.cmd_index = AT_CMD_and_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 29:
{ atclval.cmd_index = AT_CMD_and_C; BEGIN(AT_CMD); return AT_BASIC_CMD; }
case 30:
{ atclval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 31:
{ atclval.cmd_index = AT_CMD_CPIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 32:
{ atclval.cmd_index = AT_CMD_CLIP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 33:
{ atclval.cmd_index = AT_CMD_CLIR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 34:
{ atclval.cmd_index = AT_CMD_CCFC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 35:
{ atclval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 36:
{ atclval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 37:
{ atclval.cmd_index = AT_CMD_VTS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 38:
{ atclval.cmd_index = AT_CMD_CHLD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 39:
{ atclval.cmd_index = AT_CMD_CLCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 40:
{ atclval.cmd_index = AT_CMD_COPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 41:
{ atclval.cmd_index = AT_CMD_SIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 42:
{ atclval.cmd_index = AT_CMD_CSQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 43:
{ atclval.cmd_index = AT_CMD_CIMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 44:
{ atclval.cmd_index = AT_CMD_CGMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 45:
{ atclval.cmd_index = AT_CMD_CGMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 46:
{ atclval.cmd_index = AT_CMD_CGSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 47:
{ atclval.cmd_index = AT_CMD_CGMR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 48:
{ atclval.cmd_index = AT_CMD_CMEE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 49:
{ atclval.cmd_index = AT_CMD_CSCS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 50:
{ atclval.cmd_index = AT_CMD_CPOF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 51:
{ atclval.cmd_index = AT_CMD_CPOL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 52:
{ atclval.cmd_index = AT_CMD_S0; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 53:
{ atclval.cmd_index = AT_CMD_CGDCONT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 54:
{ atclval.cmd_index = AT_CMD_CGPCO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 55:
{ atclval.cmd_index = AT_CMD_CGQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 56:
{ atclval.cmd_index = AT_CMD_CGQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 57:
{ atclval.cmd_index = AT_CMD_CGATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 58:
{ atclval.cmd_index = AT_CMD_CGACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 59:
{ atclval.cmd_index = AT_CMD_CGCMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 60:
{ atclval.cmd_index = AT_CMD_CGPADDR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 61:
{ atclval.cmd_index = AT_CMD_CGDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 62:
{ atclval.cmd_index = AT_CMD_CGEREP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 63:
{ atclval.cmd_index = AT_CMD_CGREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 64:
{ atclval.cmd_index = AT_CMD_CGSMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 65:
{ atclval.cmd_index = AT_CMD_CSMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 66:
{ atclval.cmd_index = AT_CMD_CSDH; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 67:
{ atclval.cmd_index = AT_CMD_CRES; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 68:
{ atclval.cmd_index = AT_CMD_CPMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 69:
{ atclval.cmd_index = AT_CMD_CMGF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 70:
{ atclval.cmd_index = AT_CMD_CMGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 71:
{ atclval.cmd_index = AT_CMD_CMGL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 72:
{ atclval.cmd_index = AT_CMD_CMGD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 73:
{ atclval.cmd_index = AT_CMD_CMGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 74:
{ atclval.cmd_index = AT_CMD_CMSS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 75:
{ atclval.cmd_index = AT_CMD_CSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 76:
{ atclval.cmd_index = AT_CMD_CMGC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 77:
{ atclval.cmd_index = AT_CMD_CMGW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 78:
{ atclval.cmd_index = AT_CMD_CSCA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 79:
{ atclval.cmd_index = AT_CMD_CSAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 80:
{ atclval.cmd_index = AT_CMD_SMSC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 81:
{ atclval.cmd_index = AT_CMD_CSCB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 82:
{ atclval.cmd_index = AT_CMD_CNMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 83:
{ atclval.cmd_index = AT_CMD_CPBR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 84:
{ atclval.cmd_index = AT_CMD_CPBF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 85:
{ atclval.cmd_index = AT_CMD_CPBW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 86:
{ atclval.cmd_index = AT_CMD_CPBP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 87:
{ atclval.cmd_index = AT_CMD_CPBN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 88:
{ atclval.cmd_index = AT_CMD_CNUM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 89:
{ atclval.cmd_index = AT_CMD_SSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 90:
{ atclval.cmd_index = AT_CMD_CPBS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 91:
{ atclval.cmd_index = AT_CMD_CCUG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 92:
{ atclval.cmd_index = AT_CMD_CUSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 93:
{ atclval.cmd_index = AT_CMD_CAOC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 94:
{ atclval.cmd_index = AT_CMD_CCLK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 95:
{ atclval.cmd_index = AT_CMD_XX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 96:
{ atclval.cmd_index = AT_CMD_CCID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 97:
{ atclval.cmd_index = AT_CMD_VGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 98:
{ atclval.cmd_index = AT_CMD_CRSL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 99:
{ atclval.cmd_index = AT_CMD_CMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 100:
{ atclval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 101:
{ atclval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 102:
{ atclval.cmd_index = AT_CMD_SDTMF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 103:
{ atclval.cmd_index = AT_CMD_CLCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 104:
{ atclval.cmd_index = AT_CMD_CPWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 105:
{ atclval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 106:
{ atclval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 107:
{ atclval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 108:
{ atclval.cmd_index = AT_CMD_SADMDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 109:
{ atclval.cmd_index = AT_CMD_SPLO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 110:
{ atclval.cmd_index = AT_CMD_CFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 111:
{ atclval.cmd_index = AT_CMD_CIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 112:
{ atclval.cmd_index = AT_CMD_CCED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 113:
{ atclval.cmd_index = AT_CMD_STONE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 114:
{ atclval.cmd_index = AT_CMD_SSST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 115:
{ atclval.cmd_index = AT_CMD_SATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 116:
{ atclval.cmd_index = AT_CMD_SAUTOATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 117:
{ atclval.cmd_index = AT_CMD_SGPRSDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 118:
{ atclval.cmd_index = AT_CMD_SDMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 119:
{ atclval.cmd_index = AT_CMD_SUPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 120:
{ atclval.cmd_index = AT_CMD_SSYS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 121:
{ atclval.cmd_index = AT_CMD_SCPFRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 122:
{ atclval.cmd_index = AT_CMD_CGEQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 123:
{ atclval.cmd_index = AT_CMD_CGEQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 124:
{ atclval.cmd_index = AT_CMD_SCINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 125:
{ atclval.cmd_index = AT_CMD_SFSMSM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 126:
{ atclval.cmd_index = AT_CMD_SPEQPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 127:
{ atclval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 128:
{ atclval.cmd_index = AT_CMD_SYTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 129:
{ atclval.cmd_index = AT_CMD_SFTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 130:
{ atclval.cmd_index = AT_CMD_CMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 131:
{ atclval.cmd_index = AT_CMD_SIME; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 132:
{ atclval.cmd_index = AT_CMD_SCMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 133:
{ atclval.cmd_index = AT_CMD_PSORGMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 134:
{ atclval.cmd_index = AT_CMD_SPENHA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 135:
{ atclval.cmd_index = AT_CMD_SPADLVVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 136:
{ atclval.cmd_index = AT_CMD_CMMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 137:
{ atclval.cmd_index = AT_CMD_CMER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 138:
{ atclval.cmd_index = AT_CMD_CMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 139:
{ atclval.cmd_index = AT_CMD_CHUP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 140:
{ atclval.cmd_index = AT_CMD_HEADINFO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 141:
{ atclval.cmd_index = AT_CMD_SYSSLEEP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 142:
{ atclval.cmd_index = AT_CMD_MAXPOWER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 143:
{ atclval.cmd_index = AT_CMD_FCEPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 144:
{ atclval.cmd_index = AT_CMD_SPKSTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 145:
{ atclval.cmd_index = AT_CMD_KEYSHORT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 146:
{ atclval.cmd_index = AT_CMD_READRSSI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 147:
{ atclval.cmd_index = AT_CMD_GETTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 148:
{ atclval.cmd_index = AT_CMD_SETTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 149:
{ atclval.cmd_index = AT_CMD_GETFULLHISTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 150:
{ atclval.cmd_index = AT_CMD_LOGERASE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 151:
{ atclval.cmd_index = AT_CMD_GETFULLTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 152:
{ atclval.cmd_index = AT_CMD_TEMPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 153:
{ atclval.cmd_index = AT_CMD_BATTTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 154:
{ atclval.cmd_index = AT_CMD_IMEITEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 155:
{ atclval.cmd_index = AT_CMD_LVOFLOCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 156:
{ atclval.cmd_index = AT_CMD_DETALOCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 157:
{ atclval.cmd_index = AT_CMD_RSTVERIF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 158:
{ atclval.cmd_index = AT_CMD_LOOPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 159:
{ atclval.cmd_index = AT_CMD_LOCKREAD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 160:
{ atclval.cmd_index = AT_CMD_CVHU; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 161:
{ atclval.cmd_index = AT_CMD_MSLSECUR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 162:
{ atclval.cmd_index = AT_CMD_SERIALNO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 163:
{ atclval.cmd_index = AT_CMD_FACTORST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 164:
{ atclval.cmd_index = AT_CMD_KSTRINGB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 165:
{ atclval.cmd_index = AT_CMD_AKSEEDNO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 166:
{ atclval.cmd_index = AT_CMD_CALIDATE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 167:
{ atclval.cmd_index = AT_CMD_SIMDETEC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 168:
{ atclval.cmd_index = AT_CMD_SMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 169:
{ atclval.cmd_index = AT_CMD_MISCD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 170:
{ atclval.cmd_index = AT_CMD_EMEMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 171:
{ atclval.cmd_index = AT_CMD_FCMPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 172:
{ atclval.cmd_index = AT_CMD_FCBTTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 173:
{ atclval.cmd_index = AT_CMD_SETFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 174:
{ atclval.cmd_index = AT_CMD_SETFULLFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 175:
{ atclval.cmd_index = AT_CMD_GETFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 176:
{ atclval.cmd_index = AT_CMD_GETFULLFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 177:
{ atclval.cmd_index = AT_CMD_SPAUDVOLTYPE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
case 178:
{ return NUMBER;  }
case 179:
{ return STRING; }
case 180:
{ BEGIN(INITIAL); return DIALSTRING; }
case 181:
{ BEGIN(INITIAL); return SHARPSTRING; }
case 182:
{ BEGIN(INITIAL); return DTMFSTRING; }
case ATC_STATE_EOF(INITIAL):
case ATC_STATE_EOF(AT_CMD):
case ATC_STATE_EOF(AT_DIAL_CMD):
case ATC_STATE_EOF(AT_SHARP_CMD):
case ATC_STATE_EOF(AT_DTMF_CMD):
{return 0;}
case 183:
{ }
    break;
case 184:
{ return atctext[0]; }
case 185:
    break;

    case ATC_END_OF_BUFFER:
        return 0;
    default:
        return -1;    } /* end of action switch */
        } /* end of scanning one token */
    } /* end of atclex */




/*lint +e716*/ 
/*lint +e18*/ 
