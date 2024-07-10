/*****************************************************************************
** File Name:      mmiim_tp_cstar_keyboard_cfg.c                             *
** Author:                                                                   *
** Date:           2009/09/16                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    用于配置所有语言键盘布局、按键处理及对应关系等            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/07/07     haiwu.chen        Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#if (defined (TOUCH_PANEL_SUPPORT) && (!defined MMI_IM_PDA_SUPPORT))

#include "Os_api.h"
#include "mmiim_base.h"
#include "csk2api.h"
#include "mmiim_tp_cstar_internal.h"
#include "mmiim_tp_cstar_position.h"
#include "mmiim_image.h"
#include "guistring.h"

//////////////////////////////////////////////////////////////////////////
//////                  语言的字符集                            //////////
//////////////////////////////////////////////////////////////////////////
/*常用语言的字符表
如果发现有某些语言在字符上是一样的，则请不必再配置了，能省就省点吧
下面这些代表common常操作的一些语言字符集，意味不可少的，
如果多国语需要配一些独特的，请务必添加语言宏隔开*/

//数字键
const uint16 s_dig_0[] = {1, 0x0030};//'0'
const uint16 s_dig_1[] = {1, 0x0031};//'1'
const uint16 s_dig_2[] = {1, 0x0032};//'2'
const uint16 s_dig_3[] = {1, 0x0033};//'3'
const uint16 s_dig_4[] = {1, 0x0034};//'4'
const uint16 s_dig_5[] = {1, 0x0035};//'5'
const uint16 s_dig_6[] = {1, 0x0036};//'6'
const uint16 s_dig_7[] = {1, 0x0037};//'7'
const uint16 s_dig_8[] = {1, 0x0038};//'8'
const uint16 s_dig_9[] = {1, 0x0039};//'9'

//标点符号键
const uint16 s_sym_space[] ={1, 0x0020};//space
const uint16 s_sym_del[] =  {1, 0x0008};//del
const uint16 s_sym_com[] =  {1, 0x002C};//,
const uint16 s_sym_sep[] =  {1, 0x0027};//,
const uint16 s_sym_full[] = {1, 0x002E};//.
const uint16 s_sym_ent[] =  {1, 0x000A};//enter, '\n'
const uint16 s_sym_at[] =   {1, 0x0040};//@
const uint16 s_sym_jh[] =   {1, 0x0023};//#
const uint16 s_sym_dall[] = {1, 0x0024};//$
const uint16 s_sym_xg[] =   {1, 0x002F};//'/'
const uint16 s_sym_sg[] =   {1, 0x007C};//*
const uint16 s_sym_fxg[] =  {1, 0x005C};//'\'
const uint16 s_sym_lfkh[] = {1, 0x005B};//[
const uint16 s_sym_rfkh[] = {1, 0x005D};//]


const uint16 s_sym_add[] =  {1, 0x002B};//+
const uint16 s_sym_dec[] =  {1, 0x002D};//-
const uint16 s_sym_xhx[] =  {1, 0x005F};//_
const uint16 s_sym_or[] =   {1, 0x005E};//^
const uint16 s_sym_yh[] =   {1, 0x0022};//'
const uint16 s_sym_dyh[] =  {1, 0x0027};//"
const uint16 s_sym_ljkh[] = {1, 0x003C};//<
const uint16 s_sym_rjkh[] = {1, 0x003E};//>

const uint16 s_sym_mul[] =  {1, 0x002A};//*
const uint16 s_sym_euq[] =  {1, 0x003D};//=
const uint16 s_sym_and[] =  {1, 0x0026};//&
const uint16 s_sym_mod[] =  {1, 0x0025};//%
const uint16 s_sym_bl[] =   {1, 0x007E};//
const uint16 s_sym_dh[] =   {1, 0x3001};//,
const uint16 s_sym_ldkh[] = {1, 0x007B};//{
const uint16 s_sym_rdkh[] = {1, 0x007D};//}

const uint16 s_sym_wh[] =   {1, 0x003F};//?
const uint16 s_sym_gth[] =  {1, 0x0021};//!
const uint16 s_sym_mh[] =   {1, 0x003A};//:
const uint16 s_sym_fh[] =   {1, 0x003B};//;
const uint16 s_sym_lykh[] = {1, 0x0028};//(
const uint16 s_sym_rykh[] = {1, 0x0029};//)

const uint16 s_sym_cjh[] =  {1, 0x3002};//。
const uint16 s_sym_cdh[] =  {1, 0xFF0C};//，
const uint16 s_sym_yuan[] = {1, 0x00A5};//yuan
const uint16 s_sym_pond[] = {1, 0x00A3};//pond
const uint16 s_sym_xh[] =   {1, 0x00D7};//X
const uint16 s_sym_ch[] =   {1, 0x00F7};//div
const uint16 s_sym_p[] =    {1, 0x0050};//P
const uint16 s_sym_w[] =    {1, 0x0057};//W

//9键小写abc，可用于abc, Abc, en, 拼音输入等
const uint16 s_abc_1[] = {3, 0x0061, 0x0062, 0x0063};//abc
const uint16 s_abc_2[] = {3, 0x0064, 0x0065, 0x0066};//def
const uint16 s_abc_3[] = {3, 0x0067, 0x0068, 0x0069};//ghi
const uint16 s_abc_4[] = {3, 0x006A, 0x006B, 0x006C};//jkl
const uint16 s_abc_5[] = {3, 0x006D, 0x006E, 0x006F};//mno
const uint16 s_abc_6[] = {4, 0x0070, 0x0071, 0x0072, 0x0073};//pqrs
const uint16 s_abc_7[] = {3, 0x0074, 0x0075, 0x0076};//tuv
const uint16 s_abc_8[] = {4, 0x0077, 0x0078, 0x0079, 0x007A};//wxyz

//9键大写abc，可用于ABC, EN等这里多增加一套大写，只是不想去处理小写与大写的对应关系，
//这里还有对应关系，就怕一些语言大小写没有简单的算术关系，故都列出来
const uint16 s_ABC_1[] = {3, 0x0041, 0x0042, 0x0043};//abc
const uint16 s_ABC_2[] = {3, 0x0044, 0x0045, 0x0046};//def
const uint16 s_ABC_3[] = {3, 0x0047, 0x0048, 0x0049};//ghi
const uint16 s_ABC_4[] = {3, 0x004A, 0x004B, 0x004C};//jkl
const uint16 s_ABC_5[] = {3, 0x004D, 0x006E, 0x004F};//mno
const uint16 s_ABC_6[] = {4, 0x0050, 0x0051, 0x0052, 0x0053};//pqrs
const uint16 s_ABC_7[] = {3, 0x0054, 0x0055, 0x0056};//tuv
const uint16 s_ABC_8[] = {4, 0x0057, 0x0058, 0x0059, 0x007A};//wxyz

//26键小写abc，可用于abc, en, 拼音等
const uint16 s_abc_a[] = {1, 0x0061};
const uint16 s_abc_b[] = {1, 0x0062};
const uint16 s_abc_c[] = {1, 0x0063};
const uint16 s_abc_d[] = {1, 0x0064};
const uint16 s_abc_e[] = {1, 0x0065};
const uint16 s_abc_f[] = {1, 0x0066};
const uint16 s_abc_g[] = {1, 0x0067};
const uint16 s_abc_h[] = {1, 0x0068};
const uint16 s_abc_i[] = {1, 0x0069};
const uint16 s_abc_j[] = {1, 0x006A};
const uint16 s_abc_k[] = {1, 0x006B};
const uint16 s_abc_l[] = {1, 0x006C};
const uint16 s_abc_m[] = {1, 0x006D};
const uint16 s_abc_n[] = {1, 0x006E};
const uint16 s_abc_o[] = {1, 0x006F};
const uint16 s_abc_p[] = {1, 0x0070};
const uint16 s_abc_q[] = {1, 0x0071};
const uint16 s_abc_r[] = {1, 0x0072};
const uint16 s_abc_s[] = {1, 0x0073};
const uint16 s_abc_t[] = {1, 0x0074};
const uint16 s_abc_u[] = {1, 0x0075};
const uint16 s_abc_v[] = {1, 0x0076};
const uint16 s_abc_w[] = {1, 0x0077};
const uint16 s_abc_x[] = {1, 0x0078};
const uint16 s_abc_y[] = {1, 0x0079};
const uint16 s_abc_z[] = {1, 0x007A};

/*
////26键大写ABC
const uint16 s_abc_A[] = {1, 0x0041};
const uint16 s_abc_B[] = {1, 0x0042};
const uint16 s_abc_C[] = {1, 0x0043};
const uint16 s_abc_D[] = {1, 0x0044};
const uint16 s_abc_E[] = {1, 0x0045};
const uint16 s_abc_F[] = {1, 0x0046};
const uint16 s_abc_G[] = {1, 0x0047};
const uint16 s_abc_H[] = {1, 0x0048};
const uint16 s_abc_I[] = {1, 0x0049};
const uint16 s_abc_J[] = {1, 0x004A};
const uint16 s_abc_K[] = {1, 0x004B};
const uint16 s_abc_L[] = {1, 0x004C};
const uint16 s_abc_M[] = {1, 0x004D};
const uint16 s_abc_N[] = {1, 0x004E};
const uint16 s_abc_O[] = {1, 0x004F};
const uint16 s_abc_P[] = {1, 0x0050};
const uint16 s_abc_Q[] = {1, 0x0051};
const uint16 s_abc_R[] = {1, 0x0052};
const uint16 s_abc_S[] = {1, 0x0053};
const uint16 s_abc_T[] = {1, 0x0054};
const uint16 s_abc_U[] = {1, 0x0055};
const uint16 s_abc_V[] = {1, 0x0056};
const uint16 s_abc_W[] = {1, 0x0057};
const uint16 s_abc_X[] = {1, 0x0058};
const uint16 s_abc_Y[] = {1, 0x0059};
const uint16 s_abc_Z[] = {1, 0x005A};
*/

//笔画输入法各键配置表
const uint16 s_stroke_1[] = {1, CS_VK_1};   //横
const uint16 s_stroke_2[] = {1, CS_VK_2};   //坚
const uint16 s_stroke_3[] = {1, CS_VK_3};   //撇
const uint16 s_stroke_4[] = {1, CS_VK_4};   //抐
const uint16 s_stroke_5[] = {1, CS_VK_5};   //折
const uint16 s_stroke_6[] = {1, CS_VK_6};   //?


//多国语的配置见下面，注意只配每种语言对应不同的字符
#ifdef IM_FRENCH_SUPPORT//下列配置其实只有26key会用到
const uint16 s_french_1[] = {1, 0x0076};    //只配不同的哦，这里做测试
const uint16 s_french_2[] = {1, 0x0077};
const uint16 s_french_3[] = {1, 0x0078};
#endif






//////////////////////////////////////////////////////////////////////////
//////                  每个键盘的字符表                        //////////
//////////////////////////////////////////////////////////////////////////
/*目前先只配240*320的字符表，如果其它分辨率键盘字符表顺序不变，则为公共通用*/
//数字界面
const uint16* const s_dig_strtab[] =
{
    s_dig_1, s_dig_2, s_dig_3, s_dig_4, s_dig_5,
    s_dig_6, s_dig_7, s_dig_8, s_dig_9, s_dig_0,
    s_sym_del
};

//数字符号界面
const uint16* const s_digsym_strtab[] =
{
    s_sym_lykh, s_sym_rykh, s_sym_jh, s_sym_com, s_sym_full,
    s_sym_mul, s_sym_dec, s_sym_add, s_sym_p, s_sym_w, s_sym_del
};

//英文26Key小写
const uint16* const s_abc_26strtab[] =
{
    s_abc_q, s_abc_w, s_abc_e,s_abc_r, s_abc_t, s_abc_y, s_abc_u, s_abc_i, s_abc_o, s_abc_p,
    s_abc_a, s_abc_s, s_abc_d, s_abc_f, s_abc_g, s_abc_h, s_abc_j, s_abc_k, s_abc_l, s_sym_ent,
    s_abc_z, s_abc_x, s_abc_c, s_abc_v, s_abc_b, s_abc_n, s_abc_m, s_sym_del, s_sym_space  
};

/*
//英文26Key大写
const uint16* s_ABC_26strtab[]=
{
    s_abc_Q, s_abc_W, s_abc_E, s_abc_R, s_abc_T, s_abc_Y, s_abc_U, s_abc_I, s_abc_O, s_abc_P,
    s_abc_A, s_abc_S, s_abc_D, s_abc_F, s_abc_G, s_abc_H, s_abc_J, s_abc_K, s_abc_L, s_sym_ent,
    s_abc_Z, s_abc_X, s_abc_C, s_abc_V, s_abc_B, s_abc_N, s_abc_M, s_sym_del, s_sym_space  
};*/

//对应的物理9Key，传入都是1,2,3等
const uint16* const s_abc_9strtab[] =
{
    s_dig_1, s_dig_2, s_dig_3, s_sym_del,
    s_dig_4, s_dig_5, s_dig_6,s_sym_ent,
    s_dig_7, s_dig_8, s_dig_9,s_sym_space
};

//拼音26Key
const uint16* const s_py_26strtab[] =
{
    s_abc_q, s_abc_w, s_abc_e,s_abc_r, s_abc_t, s_abc_y, s_abc_u, s_abc_i, s_abc_o, s_abc_p,
    s_abc_a, s_abc_s, s_abc_d, s_abc_f, s_abc_g, s_abc_h, s_abc_j, s_abc_k, s_abc_l, s_sym_ent,
    s_sym_com, s_abc_z, s_abc_x, s_abc_c, s_abc_v, s_abc_b, s_abc_n, s_abc_m, s_sym_del, s_sym_space  
};

// 外文26key: 阿拉伯文
const uint16* const s_foreign_26strtab[] =
{
    s_abc_q, s_abc_w, s_abc_e,s_abc_r, s_abc_t, s_abc_y, s_abc_u, s_abc_i, s_abc_o, s_abc_p,
    s_abc_a, s_abc_s, s_abc_d, s_abc_f, s_abc_g, s_abc_h, s_abc_j, s_abc_k, s_abc_l, s_sym_ent,
    s_sym_com, s_abc_z, s_abc_x, s_abc_c, s_abc_v, s_abc_b, s_abc_n, s_abc_m, s_sym_del, s_sym_space  
};

//笔画Key
const uint16* const s_stroke_strtab[] =
{
    s_stroke_1, s_stroke_2, s_stroke_3, s_sym_del,
    s_stroke_4, s_stroke_5, s_stroke_6, s_sym_ent,
    s_sym_space
};




//////////////////////////////////////////////////////////////////////////
//////                  每个键盘的的按键布局情况                //////////
//////////////////////////////////////////////////////////////////////////
/*
上面字符集，及键盘的普通字符键是不随分辨率而改变的，
下面对于键盘的布局，则要考虑分辨率的因素，
如果一些键盘在各个分辨率下，行列中各个最小按键的个数没有改变，
这里每行、列中的个数，因为我们不允许用户在不同的分辨率下改变有普通按键的左右位置关系
则可以考虑把此类键盘提取出来，独立于分辨率之外。
*/

//数字键盘的操作对应表
LOCAL MMIIM_TP_CSTAR_FUNC_T const s_dig_kb_functab[MMIIM_TP_CSTAR_DIG_VNUM*MMIIM_TP_CSTAR_DIG_HNUM] =
{
    {0, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {1, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {2, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {3, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {4, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {5, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {6, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {7, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {8, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {9, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_STATE_DIGSYM, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {MMIIM_TP_CSTAR_STATE_DIGSYM, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {10, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {10, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key}
};

//数字符号 键盘的操作对应表
LOCAL MMIIM_TP_CSTAR_FUNC_T const s_digsym_kb_functab[MMIIM_TP_CSTAR_DIGSYM_VNUM*MMIIM_TP_CSTAR_DIG_HNUM] =
{
    {0, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {1, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {2, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {3, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {4, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {5, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {6, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {7, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {8, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {9, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_LOCK, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleLock},
    {MMIIM_TP_CSTAR_TAG_LOCK, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleLock},
    {10, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {10, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key}
};

//abc 26Key
LOCAL MMIIM_TP_CSTAR_FUNC_T const s_abc_26kb_func[MMIIM_TP_CSTAR_ABC_VNUM*MMIIM_TP_CSTAR_ABC_HNUM] =
{
    {0, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {1, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {2, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {3, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {4, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {5, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {6, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {7, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {8, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {9, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {10, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {11, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {12, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {13, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {14, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {15, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {16, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {17, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {18, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {19, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {MMIIM_TP_CSTAR_TAG_SHIFT_CAP, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleCaps},
    {20, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {21, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {22, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {23, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {24, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {25, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {26, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {27, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {27, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26KeyTo9Key},
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26KeyTo9Key},
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_STATE_DIG, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {MMIIM_TP_CSTAR_STATE_DIG, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {28, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {28, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar}
};

//abc 9Key
LOCAL MMIIM_TP_CSTAR_FUNC_T const s_abc_9kb_func[MMIIM_TP_CSTAR_9KEY_ABC_VNUM*MMIIM_TP_CSTAR_9KEY_ABC_HNUM] =
{
    {0, 1, MMIIM_TP_CSTAR_FUNC_NONE, PNULL},
    {0, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {1, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {2, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {3, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {0, 1, MMIIM_TP_CSTAR_FUNC_NONE, PNULL},
    {4, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {5, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {6, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {7, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {0, 1, MMIIM_TP_CSTAR_FUNC_NONE, PNULL},
    {8, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {9, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {10, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {11, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {0, 1, MMIIM_TP_CSTAR_FUNC_NONE, PNULL},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9KeyTo26Key},
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_STATE_DIG, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
};

//pinyin 26key
LOCAL MMIIM_TP_CSTAR_FUNC_T const s_py_26kb_func[MMIIM_TP_CSTAR_PY_HNUM*MMIIM_TP_CSTAR_PY_VNUM] =
{
    {0, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {1, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {2, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {3, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {4, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {5, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {6, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {7, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {8, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {9, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {10, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {11, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {12, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {13, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {14, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {15, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {16, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {17, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {18, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {19, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {20, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},
    {21, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {22, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {23, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {24, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {25, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {26, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {27, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {28, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {28, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26KeyTo9Key},//切换到9键盘
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26KeyTo9Key},//切换到9键盘
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_STATE_DIG, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {MMIIM_TP_CSTAR_STATE_DIG, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {29, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {29, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar}
};

//笔画键盘操作对应表
LOCAL MMIIM_TP_CSTAR_FUNC_T const s_stroke_kb_func[MMIIM_TP_CSTAR_STROKE_VNUM*MMIIM_TP_CSTAR_STROKE_HNUM] =
{
    {0, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {1, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {2, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {3, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {4, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {5, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {6, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key},
    {7, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_STATE_DIG, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {8, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle9Key}
};

//en 26key
LOCAL MMIIM_TP_CSTAR_FUNC_T const s_en_26kb_func[MMIIM_TP_CSTAR_PY_HNUM*MMIIM_TP_CSTAR_PY_VNUM] =
{
    {0, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {1, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {2, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {3, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {4, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {5, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {6, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {7, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {8, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {9, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {10, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {11, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {12, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {13, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {14, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {15, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {16, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {17, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {18, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {19, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {MMIIM_TP_CSTAR_TAG_SHIFT_CAP, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleCaps},
    {20, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {21, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {22, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {23, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {24, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {25, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {26, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {27, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {27, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26KeyTo9Key},//切换到9键盘
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26KeyTo9Key},//切换到9键盘
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_STATE_DIG, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {MMIIM_TP_CSTAR_STATE_DIG, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {28, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {28, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar}
};

//缅甸语，替换键没有大小写
#if defined IM_MYANMAR_SUPPORT

//26Key小写
const uint16* const s_myanmar_26strtab[] =
{
    s_abc_q, s_abc_w, s_abc_e,s_abc_r, s_abc_t, s_abc_y, s_abc_u, s_abc_i, s_abc_o, s_abc_p,
    s_abc_a, s_abc_s, s_abc_d, s_abc_f, s_abc_g, s_abc_h, s_abc_j, s_abc_k, s_abc_l, s_sym_ent,
    s_sym_com, s_abc_z, s_abc_x, s_abc_c, s_abc_v, s_abc_b, s_abc_n, s_abc_m, s_sym_del, s_sym_space  
};

LOCAL MMIIM_TP_CSTAR_FUNC_T const s_myanmar_26kb_func[MMIIM_TP_CSTAR_ABC_VNUM*MMIIM_TP_CSTAR_ABC_HNUM] =
{
    {0, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {1, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {2, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {3, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {4, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {5, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {6, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {7, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {8, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {9, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {10, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {11, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {12, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {13, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {14, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {15, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {16, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {17, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {18, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {19, 1, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {20, 1, MMIIM_TP_CSTAR_FUNC_TIPS, HandleChar},      //点号替换大小写切换
    {21, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {22, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {23, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {24, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {25, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {26, 1, MMIIM_TP_CSTAR_FUNC_TIPS, Handle26Key},
    {27, 1, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {28, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {28, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26Key},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_IM, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleMethodSelect},
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 2, MMIIM_TP_CSTAR_FUNC_NONE, Handle26KeyTo9Key},
    {MMIIM_TP_CSTAR_TAG_SHIFT_926, 0, MMIIM_TP_CSTAR_FUNC_NONE, Handle26KeyTo9Key},
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_TAG_SYMBOL, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicthSymbol},
    {MMIIM_TP_CSTAR_STATE_DIG, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {MMIIM_TP_CSTAR_STATE_DIG, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleSwicth},
    {29, 2, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar},
    {29, 0, MMIIM_TP_CSTAR_FUNC_NONE, HandleChar}
};
#endif

/*配置每个键盘需要替换的按键*/
//abc 26 key
/*lint -save -e785 */
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_abc_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A','b','c',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_abc_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a','b','c',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_abc_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A','B','C',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

//abc 9key
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_abc_9kb_lead_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A','b','c',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_abc_9kb_lower_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a','b','c',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_abc_9kb_upper_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A','B','C',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

//en 26 key 原键盘上那个显示输入法语言的键已经是正常的了，所以只配大小写即可
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_en_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'E','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_en_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'e','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_en_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'E','N',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

//en 9key
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_en_9kb_lead_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'E','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_en_9kb_lower_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'e','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_en_9kb_upper_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'E','N',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

#if defined (IM_SIMP_CHINESE_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_pyin_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {0x62FC, 0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_pyin_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {0x62FC, 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_zyin_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {0x6CE8, 0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_zyin_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {0x6CE8, 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_FRENCH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfrench_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','F','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfrench_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','f','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfrench_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','F','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_french_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_french_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'f','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_french_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfrench_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','F','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfrench_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','f','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfrench_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','F','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_french_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_french_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'f','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_french_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_ARABIC_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sarb_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','A','r','b',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_arb_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A','r','b',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sarb_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','A','r','b', 0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_arb_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A','r','b', 0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_TURKISH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_stur_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','T','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_stur_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','t','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_stur_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','T','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tur_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tur_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'t','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tur_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_stur_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','T','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_stur_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','t','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_stur_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','T','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tur_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tur_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'y','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tur_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_PERSIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_per_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_per_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_PORTUGUESE_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sport_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sport_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','p','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sport_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_port_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_port_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'p','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_port_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sport_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sport_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','p','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sport_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','0',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_port_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_port_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'p','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_port_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','0',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_SPANISH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sspan_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','p',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sspan_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','s','p',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sspan_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','P',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_span_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','p',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_span_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','p',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_span_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','P',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sspan_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','p',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sspan_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','s','p',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sspan_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','P',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_span_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','p',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_span_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','p',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_span_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','P',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//他加诺multitap虚拟键盘部分	
#ifdef IM_TAGALOG_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tagalog_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T', 'a', 'g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tagalog_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T', 'a', 'g', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif


#if defined (IM_RUSSIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_srus_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','R','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_srus_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','r','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_srus_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','R','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_rus_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'R','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_rus_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'r','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_rus_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'R','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_srus_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','R','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_srus_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','r','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_srus_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','R','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_rus_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'R','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_rus_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'r','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_rus_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'R','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_INDONESIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sind_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','I','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sind_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','i','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sind_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','I','N',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ind_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ind_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ind_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I','N',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sind_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','I','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sind_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','i','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sind_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','I','N',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ind_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ind_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i','n',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ind_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I','N',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_MALAY_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_smalay_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','M','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_smalay_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','m','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_smalay_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','M','A',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_malay_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_malay_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'m','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_malay_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M','A',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_smalay_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','M','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_smalay_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','m','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_smalay_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','M','A',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_malay_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_malay_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'m','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_malay_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M','A',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_HUNGARIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shung_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shung_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','h','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shung_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hung_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hung_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'h','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hung_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shung_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shung_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','h','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shung_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hung_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hung_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'h','u',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hung_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','U',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_GERMAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgerman_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','G','e',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgerman_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','g','e',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgerman_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','G','E',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_german_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','e',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_german_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'g','e',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_german_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','E',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgerman_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','G','e',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgerman_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','g','e',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgerman_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','G','E',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_german_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','e',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_german_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'g','e',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_german_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','E',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_GREEK_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgreek_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','G','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgreek_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','g','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgreek_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','G','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_greek_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_greek_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'g','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_greek_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgreek_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','G','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgreek_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','g','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sgreek_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','G','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_greek_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_greek_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'g','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_greek_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_HINDI_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shindi_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','i',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hindi_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','i', 0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shindi_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','i',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hindi_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','i',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif   

#if defined (IM_THAI_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sthai_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','T','h',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_thai_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T','h',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sthai_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','T','h',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_thai_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T','h',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_ITALIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sita_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','I','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sita_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','i','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sita_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','I','T',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ita_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ita_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ita_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I','T',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sita_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','I','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sita_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','i','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sita_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','I','T',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ita_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ita_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ita_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I','T',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_URDU_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_surdu_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','U','r',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_urdu_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'U','r',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_surdu_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','U','r',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_urdu_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'U','r',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_VIETNAMESE_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sviet_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','V','i',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sviet_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','v','i',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sviet_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','V','I',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_viet_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'V','i',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_viet_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'v','i',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_viet_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'V','I',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sviet_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','V','i',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sviet_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','v','i',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sviet_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','V','I',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_viet_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'V','i',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_viet_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'v','i',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_viet_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'V','I',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_HEBREW_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shebrew_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hebrew_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_shebrew_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hebrew_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif 

#if defined (IM_BENGALI_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sben_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','B','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ben_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B','e', 0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sben_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','B','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ben_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B','e',0}, 
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_CZECH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scze_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','C','z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scze_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','c','z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scze_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','C','Z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_cze_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'C','z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_cze_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'c','z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_cze_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'C','Z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scze_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','C','z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scze_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','c','z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scze_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','C','Z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_cze_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'C','z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_cze_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'c','z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_cze_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'C','Z',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_SLOVENIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslo_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslo_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','s','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslo_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slo_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slo_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slo_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslo_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslo_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','s','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslo_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slo_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slo_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slo_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_ROMANIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sroma_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','R','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sroma_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','r','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sroma_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','R','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_roma_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'R','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_roma_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'r','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_roma_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'R','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sroma_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','R','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sroma_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','r','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sroma_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','R','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_roma_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'R','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_roma_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'r','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_roma_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'R','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_TELUGU_SUPPORT)

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_telugu_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_telugu_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

#endif


#if defined (IM_MARATHI_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_marathi_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'a', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_marathi_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'a', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_TAMIL_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tamil_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T', 'a', 'm', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_tamil_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'T', 'a', 'm', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_GUJARATI_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_gujarati_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G', 'u', 'j', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_gujarati_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G', 'u', 'j', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_KANNADA_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_kannada_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'K', 'a', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_kannada_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'K', 'a', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_MALAYALAM_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_malayalam_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'a', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_malayalam_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'a', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_MYANMAR_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_myanmar_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'y', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_26_IM_COMMA_OFF, IMG_IMSP_26_IM_COMMA_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_myanmar_lead_9kb_replace_key[] = 
{
    //数字1
    {1, 0, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, 
        {0x1031,0x102C, 0x102D, 0x102E, 0x102B, 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_EN9_KEY_BACKGROUND, IMG_IMSP_EN9_KEY_HIGHLIGHT}},

    //空格
    {4, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, 
        {0x0020, 0x1031, 0x103C, 0x1039, 0x1040, 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_EN9_KEY_BACKGROUND, IMG_IMSP_EN9_KEY_HIGHLIGHT}},
        
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'y', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_ORIYA_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_oriya_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'O', 'r', 'i', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_oriya_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'O', 'r', 'i', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//旁遮普multitap关于虚拟键盘部分	
#ifdef IM_PUNJABI_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_punjabi_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P', 'u', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_punjabi_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P', 'u', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//南非荷兰语multitap关于虚拟键盘部分	
#ifdef IM_AFRIKAANS_SUPPORT

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_safrikaans_lead_26kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'A', 'f', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_safrikaans_lower_26kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'a', 'f', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_safrikaans_upper_26kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'A', 'F', 'R', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};


LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_safrikaans_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'A', 'f', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_safrikaans_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'a', 'f', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_safrikaans_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'A', 'F', 'R', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_afrikaans_lead_26kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'f', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_afrikaans_lower_26kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a', 'f', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_afrikaans_upper_26kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'F', 'R', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};


LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_afrikaans_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'f', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_afrikaans_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a', 'f', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_afrikaans_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'F', 'R', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

#endif


//阿尔巴尼亚语multitap关于虚拟键盘部分	
#ifdef IM_ALBANIAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_albanian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'l', 'b', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_albanian_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a', 'l', 'b', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_albanian_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'L', 'B', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_albanian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'l', 'b', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_albanian_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a', 'l', 'b', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_albanian_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'L', 'B', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//亚美尼亚multitap关于虚拟键盘部分	
#ifdef IM_ARMENIAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_armenian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'r', 'm', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_armenian_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a', 'r', 'm', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_armenian_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'R', 'M', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_armenian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'r', 'm', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_armenian_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a', 'r', 'm', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_armenian_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'R', 'M', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//阿塞拜疆语(属阿尔泰语系土耳其语族)multitap关于虚拟键盘部分	
#ifdef IM_AZERBAIJANI_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_azerbaijani_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'z', 'e', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_azerbaijani_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a', 'z', 'e', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_azerbaijani_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'Z', 'E', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};


LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_azerbaijani_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'z', 'e', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_azerbaijani_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'a', 'z', 'e', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_azerbaijani_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'A', 'Z', 'E', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//巴斯克语multitap关于虚拟键盘部分	
#ifdef IM_BASQUE_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_basque_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B', 'a', 's', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_basque_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'b', 'a', 's', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_basque_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B', 'A', 'S', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_basque_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B', 'a', 's', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_basque_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'b', 'a', 's', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_basque_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B', 'A', 'S', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif


#if defined (IM_CROATIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scroatian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scroatian_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','h','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scroatian_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_croatian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_croatian_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'h','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_croatian_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scroatian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scroatian_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','h','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_scroatian_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','H','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_croatian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_croatian_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'h','r',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_croatian_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','R',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_DUTCH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdutch_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','N','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdutch_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','n','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdutch_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','N','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_dutch_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'N','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_dutch_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'n','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_dutch_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'N','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdutch_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','N','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdutch_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','n','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdutch_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','N','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_dutch_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'N','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_dutch_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'n','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_dutch_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'N','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//爱沙尼亚multitap关于虚拟键盘部分	
#ifdef IM_ESTONIAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_estonian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'E', 's', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_estonian_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'e', 's', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_estonian_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'E', 'S', 'T', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_estonian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'E', 's', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_estonian_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'e', 's', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_estonian_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'E', 'S', 'T', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//菲律宾multitap关于虚拟键盘部分	
#ifdef IM_FILIPINO_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_filipino_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'i', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_filipino_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'f', 'i', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_filipino_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'I', 'L', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_filipino_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'i', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_filipino_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'f', 'i', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_filipino_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'I', 'L', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//芬兰multitap关于虚拟键盘部分	
#ifdef IM_FINNISH_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_finnish_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F','i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_finnish_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'f','i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_finnish_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F','I', 'N', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_finnish_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F','i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_finnish_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'f', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_finnish_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'I', 'N', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//加利西亚语multitap关于虚拟键盘部分	
#ifdef IM_GALICIAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_galician_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','a', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_galician_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'g','a', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_galician_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','A', 'L', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_galician_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','a', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_galician_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'g', 'a', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_galician_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G', 'A', 'L', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//格鲁吉亚multitap关于虚拟键盘部分	
#ifdef IM_GEORGIAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_georgian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','e', 'o', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_georgian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'G','e', 'o', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif


//豪萨语(西非语系中的斯瓦西里语multitap关于虚拟键盘部分	
#ifdef IM_HAUSA_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hausa_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','a', 'u', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hausa_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'h','a', 'u', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hausa_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','A', 'U', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hausa_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','a', 'u', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hausa_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'h','a', 'u', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_hausa_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'H','A', 'U', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//冰岛语multitap关于虚拟键盘部分	
#ifdef IM_ICELANDIC_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_icelandic_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'c', 'e', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_icelandic_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i', 'c', 'e', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_icelandic_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'C', 'E', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_icelandic_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'c', 'e', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_icelandic_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i', 'c', 'e', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_icelandic_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'C', 'E', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//伊博语multitap关于虚拟键盘部分	
#ifdef IM_IGBO_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_igbo_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'g', 'b', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_igbo_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i', 'g', 'b', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_igbo_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'G', 'B', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_igbo_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'g', 'b', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_igbo_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i', 'g', 'b', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_igbo_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'G', 'B', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//爱尔兰语multitap关于虚拟键盘部分	
#ifdef IM_IRISH_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_irish_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'r', 'i', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_irish_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i', 'r', 'i', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_irish_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'R', 'I', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_irish_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'r', 'i', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_irish_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'i', 'r', 'i', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_irish_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'I', 'R', 'I', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//哈萨克斯坦multitap关于虚拟键盘部分	
#ifdef IM_KAZAKH_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_kazakh_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'K', 'a', 'z', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_kazakh_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'k', 'a', 'z', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_kazakh_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'K', 'A', 'Z', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_kazakh_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'K', 'a', 'z', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_kazakh_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'k', 'a', 'z', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_kazakh_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'K', 'A', 'Z', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//拉脱维亚multitap关于虚拟键盘部分	
#ifdef IM_LATVIAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_latvian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'L', 'a', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_latvian_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'l', 'a', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_latvian_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'L', 'A', 'T', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_latvian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'L', 'a', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_latvian_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'l', 'a', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_latvian_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'L', 'A', 'T', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_BULGARIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sbulg_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','B','g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sbulg_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','b','g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sbulg_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','B','G',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_bulg_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B','g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_bulg_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'b','g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_bulg_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B','G',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sbulg_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','B','g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sbulg_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','b','g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sbulg_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','B','G',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_bulg_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B','g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_bulg_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'b','g',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_bulg_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'B','G',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//加泰罗尼亚语multitap关于虚拟键盘部分	
#ifdef IM_CATALAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_catalan_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'C','a', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_catalan_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'c', 'a', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_catalan_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'C', 'A', 'T', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_catalan_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'C','a', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_catalan_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'c', 'a', 't', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_catalan_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'C', 'A', 'T', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_POLISH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_spolish_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_spolish_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','p','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_spolish_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_polish_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_polish_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'p','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_polish_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_spolish_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_spolish_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','p','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_spolish_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','P','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_polish_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_polish_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'p','l',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_polish_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'P','L',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif


//塞尔维亚语multitap关于虚拟键盘部分	
#ifdef IM_SERBIAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_serbian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'e', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_serbian_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'e', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_serbian_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'E', 'R', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_serbian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'e', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_serbian_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'e', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_serbian_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'E', 'R', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//塞索托语(属班图语系) multitap关于虚拟键盘部分	
#ifdef IM_SESOTHO_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sesotho_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'e', 's', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sesotho_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'e', 's', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sesotho_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'E', 'S', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sesotho_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'e', 's', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sesotho_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'e', 's', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sesotho_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'E', 'S', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_DANISH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdanish_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','D','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdanish_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','d','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdanish_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','D','A',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_danish_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'D','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_danish_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'d','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_danish_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'D','A',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdanish_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','D','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdanish_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','d','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sdanish_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','D','A',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_danish_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'D','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_danish_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'d','a',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_danish_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'D','A',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_SLOVAK_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslovak_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslovak_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','s','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslovak_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','K',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slovak_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slovak_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slovak_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','K',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslovak_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslovak_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','s','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sslovak_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','K',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slovak_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slovak_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slovak_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','K',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_LITHUANIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slithu_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','L','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slithu_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','l','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slithu_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','L','T',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_lithu_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'L','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_lithu_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'l','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_lithu_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'L','T',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slithu_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','L','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slithu_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','l','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_slithu_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','L','T',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_lithu_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'L','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_lithu_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'l','t',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_lithu_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'L','T',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//马其顿语multitap关于虚拟键盘部分	
#ifdef IM_MACEDONIAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_macedonian_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'a', 'c', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_macedonian_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'m', 'a', 'c', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_macedonian_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'A', 'C', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_macedonian_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'a', 'c', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_macedonian_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'m', 'a', 'c', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_macedonian_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'A', 'C', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//摩尔多瓦multitap关于虚拟键盘部分	
#ifdef IM_MOLDOVAN_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_moldonav_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'o', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_moldonav_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'m', 'o', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_moldonav_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'O', 'L', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_moldonav_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'o', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_moldonav_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'m', 'o', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_moldonav_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'M', 'O', 'L', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_NORWEGIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_snorw_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','N','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_snorw_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','n','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_snorw_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','N','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_norw_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'N','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_norw_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'n','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_norw_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'N','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_snorw_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','N','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_snorw_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','n','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_snorw_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','N','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_norw_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'N','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_norw_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'n','o',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_norw_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'N','O',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_SWEDISH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswed_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','v',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswed_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','s','v',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswed_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','V',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swed_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','v',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swed_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','v',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swed_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','V',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswed_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','v',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswed_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','s','v',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswed_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','S','V',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swed_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','v',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swed_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','v',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swed_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S','V',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_UKRAINIAN_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sukra_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','U','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sukra_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','u','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sukra_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','U','K',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ukra_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'U','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ukra_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'u','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ukra_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'U','K',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sukra_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','U','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sukra_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','u','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sukra_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s','U','K',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ukra_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'U','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ukra_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'u','k',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_ukra_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'U','K',0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

// 约鲁巴语 multitap关于虚拟键盘部分	
#ifdef IM_YORUBA_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_yoruba_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'Y', 'o', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_yoruba_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'y', 'o', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_yoruba_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'Y', 'O', 'R', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};


LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_yoruba_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'Y', 'o', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_yoruba_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'y', 'o', 'r', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_yoruba_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'Y', 'O', 'R', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//科萨语	 multitap关于虚拟键盘部分	
#ifdef IM_XHOSA_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_xhosa_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'X', 'h', 'o', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_xhosa_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'x', 'h', 'o', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_xhosa_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'X', 'H', 'O', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};


LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_xhosa_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'X', 'h', 'o', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_xhosa_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'x', 'h', 'o', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_xhosa_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'X', 'H', 'O', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

//祖鲁语multitap关于虚拟键盘部分	
#ifdef IM_ZULU_SUPPORT
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_zulu_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'Z', 'u', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_zulu_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'z', 'u', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_zulu_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'Z', 'U', 'L', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
};


LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_zulu_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'Z', 'u', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_zulu_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'z', 'u', 'l', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_zulu_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'Z', 'U', 'L', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif
#if defined (IM_SWAHILI_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswahl_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'S', 'w', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswahl_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 's', 'w', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswahl_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'S', 'W', 'A', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swahl_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'w', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swahl_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'w', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swahl_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'W', 'A', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswahl_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'S', 'w', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswahl_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 's', 'w', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sswahl_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'S', 'W', 'A', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swahl_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'w', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swahl_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'w', 'a', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_swahl_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'S', 'W', 'A', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif

#if defined (IM_FINNISH_SUPPORT)
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfi_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'F', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfi_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'f', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfi_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'F', 'I', 'N', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_fi_lead_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LEAD, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_fi_lower_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'f', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_LOWER, IMG_IMSP_26_CAP_ON}}
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_fi_upper_26kb_replace_key[] = 
{
    {0, 3, 2, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'I', 'N', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM26_BACK_OFF, IMG_IMSP_IM_SWITCH_IM26_BACK_ON}},
    {0, 2, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE, {0}, 
        2, {0, 0, IMG_IMSP_26_CAP_UPPER, IMG_IMSP_26_CAP_ON}}
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfi_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'F', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfi_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'f', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_sfi_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'s', 'F', 'I', 'N', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};

LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_fi_lead_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_fi_lower_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'f', 'i', 'n', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
LOCAL MMIIM_TP_CSTAR_REPLACE_KEY_T const s_fi_upper_9kb_replace_key[] = 
{
    {1, 3, 1, MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT, {'F', 'I', 'N', 0},
        MMIIM_TP_CSTAR_KEY_STATE_ACTIVE, {0, 0, IMG_IMSP_IM_SWITCH_IM9_BACK_OFF, IMG_IMSP_IM_SWITCH_IM9_BACK_ON}},
};
#endif



//-----------------------以下部分为multitap虚拟键盘按钮显示字符or字符串表及相关信息---------
//==9键multitap虚拟键盘默认显示字符所需资源
//9键虚拟键盘之特殊按键
LOCAL const   MMIIM_TP_KEY_DISPLAY_TEXT s_9_def_enter = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL const   MMIIM_TP_KEY_DISPLAY_TEXT s_9_def_comma = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL const   MMIIM_TP_KEY_DISPLAY_TEXT s_9_def_backspace = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL const   MMIIM_TP_KEY_DISPLAY_TEXT s_9_def_cap = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL const   MMIIM_TP_KEY_DISPLAY_TEXT s_9_def_symbol = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL const   MMIIM_TP_KEY_DISPLAY_TEXT s_9_def_digit = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL const   MMIIM_TP_KEY_DISPLAY_TEXT s_9_def_space = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL const   MMIIM_TP_KEY_DISPLAY_TEXT s_9_def_to_26 = {MMIIM_KB_KEY_DISP_TEXT_NONE};

//9键默认键盘布局，主要是与multitap数据对照表对应，获取multitap数据，否则可以不用的
const uint16 s_mul_text_9kb_def_data_map[MMIIM_TP_CSTAR_9KEY_ABC_VNUM * MMIIM_TP_CSTAR_9KEY_ABC_HNUM] = 
{
    MMIIM_SP_26KEY_INDEX_MAX,   //choice 不做修改
    MMIIM_SP_26KEY_INDEX_COMMA, 
    MMIIM_SP_26KEY_INDEX_2,
    MMIIM_SP_26KEY_INDEX_3,
    MMIIM_SP_26KEY_INDEX_DEL,

    MMIIM_SP_26KEY_INDEX_MAX,   //choice 不做修改
    MMIIM_SP_26KEY_INDEX_4,
    MMIIM_SP_26KEY_INDEX_5,
    MMIIM_SP_26KEY_INDEX_6,
    MMIIM_SP_26KEY_INDEX_ENTER,

    MMIIM_SP_26KEY_INDEX_MAX,   //choice 不做修改
    MMIIM_SP_26KEY_INDEX_7,
    MMIIM_SP_26KEY_INDEX_8,
    MMIIM_SP_26KEY_INDEX_9,
    MMIIM_SP_26KEY_INDEX_SPACE,
    
    MMIIM_SP_26KEY_INDEX_MAX,   //choice 不做修改
    MMIIM_SP_26KEY_INDEX_MAX,   //s_9_def_cap
    MMIIM_SP_26KEY_INDEX_MAX,   //s_9_def_to_26
    MMIIM_SP_26KEY_INDEX_MAX,       //s_9_def_symbol
    MMIIM_SP_26KEY_INDEX_MAX,   //s_9_def_digit
};

//将默认数据保存到此数组中，通过s_mul_text_9kb_def_data调用,之所以使用数组，而不采用单个数据表示，
//是因为数组操作起来比较方便，虽然名称全部一样
LOCAL MMIIM_TP_KEY_DISPLAY_TEXT s_save_9kb_data[MMIIM_TP_CSTAR_9KEY_ABC_VNUM * MMIIM_TP_CSTAR_9KEY_ABC_HNUM] =
{
    {MMIIM_KB_KEY_DISP_TEXT_NONE},  //choice 不做修改
    {MMIIM_KB_KEY_DISP_TEXT_NONE},  //MMIIM_SP_26KEY_INDEX_COMMA
    {MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT},  //MMIIM_SP_26KEY_INDEX_2
    {MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT},  //MMIIM_SP_26KEY_INDEX_3
    {MMIIM_KB_KEY_DISP_TEXT_NONE},

    {MMIIM_KB_KEY_DISP_TEXT_NONE},  //choice 不做修改
    {MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT},  
    {MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT}, 
    {MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT}, 
    {MMIIM_KB_KEY_DISP_TEXT_NONE},

    {MMIIM_KB_KEY_DISP_TEXT_NONE},  //choice 不做修改
    {MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT},  
    {MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT}, 
    {MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT}, 
    {MMIIM_KB_KEY_DISP_TEXT_NONE},

    {MMIIM_KB_KEY_DISP_TEXT_NONE},  //choice 不做修改
    {MMIIM_KB_KEY_DISP_TEXT_NONE},  
    {MMIIM_KB_KEY_DISP_TEXT_NONE}, 
    {MMIIM_KB_KEY_DISP_TEXT_NONE}, 
    {MMIIM_KB_KEY_DISP_TEXT_NONE},
};      

/*lint -save -e605 */   //此处部分指针变量的内容可以修改，而部分不许修改，不能兼顾
/*lint -save -e651 */   //初始化赋值，提示
//此布局与s_mul_text_9kb_def_data_map布局相同，修改时，统一修改,特殊按键用特殊按键处理
MMIIM_TP_KEY_DISPLAY_TEXT*  const s_mul_text_9kb_def_data[MMIIM_TP_CSTAR_9KEY_ABC_VNUM * MMIIM_TP_CSTAR_9KEY_ABC_HNUM] = 
{
    &s_save_9kb_data[0],        //choice 不做修改
    &s_save_9kb_data[1],        //MMIIM_SP_26KEY_INDEX_COMMA
    &s_save_9kb_data[2],        //MMIIM_SP_26KEY_INDEX_2
    &s_save_9kb_data[3],        //MMIIM_SP_26KEY_INDEX_3
    &s_9_def_backspace,         //MMIIM_SP_26KEY_INDEX_DEL
    
    &s_save_9kb_data[5],        //choice 不做修改
    &s_save_9kb_data[6],        //MMIIM_SP_26KEY_INDEX_4
    &s_save_9kb_data[7],        //MMIIM_SP_26KEY_INDEX_5
    &s_save_9kb_data[8],        //MMIIM_SP_26KEY_INDEX_6
    &s_9_def_enter,             //MMIIM_SP_26KEY_INDEX_ENTER
        
    &s_save_9kb_data[10],   //choice 不做修改
    &s_save_9kb_data[11],   //MMIIM_SP_26KEY_INDEX_7
    &s_save_9kb_data[12],   //MMIIM_SP_26KEY_INDEX_8
    &s_save_9kb_data[13],   //MMIIM_SP_26KEY_INDEX_9
    &s_9_def_space,             //MMIIM_SP_26KEY_INDEX_SPACE
    
    &s_save_9kb_data[15],   //choice 不做修改
    &s_9_def_cap,   //s_9_def_cap
    &s_9_def_to_26,     //s_9_def_to_26
    &s_9_def_symbol,    //s_9_def_symbol
    &s_9_def_digit,     //s_9_def_digit
};

//==26键multitap虚拟键盘默认显示字符所需资源
//26键虚拟键盘之特殊按键
LOCAL   MMIIM_TP_KEY_DISPLAY_TEXT s_26_def_enter = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL   MMIIM_TP_KEY_DISPLAY_TEXT s_26_def_comma = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL   MMIIM_TP_KEY_DISPLAY_TEXT s_26_def_backspace = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL   MMIIM_TP_KEY_DISPLAY_TEXT s_26_def_cap = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL   MMIIM_TP_KEY_DISPLAY_TEXT s_26_def_to_9 = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL   MMIIM_TP_KEY_DISPLAY_TEXT s_26_def_symbol = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL   MMIIM_TP_KEY_DISPLAY_TEXT s_26_def_digit = {MMIIM_KB_KEY_DISP_TEXT_NONE};
LOCAL   MMIIM_TP_KEY_DISPLAY_TEXT s_26_def_space = {MMIIM_KB_KEY_DISP_TEXT_NONE};

//26键默认键盘布局，主要是与multitap数据对照表对应，获取multitap数据，否则可以不用的
const uint16 s_mul_text_26kb_def_data_map[MMIIM_TP_CSTAR_ABC_VNUM * MMIIM_TP_CSTAR_ABC_HNUM] = 
{
    MMIIM_SP_26KEY_INDEX_Q,
    MMIIM_SP_26KEY_INDEX_W,
    MMIIM_SP_26KEY_INDEX_E,
    MMIIM_SP_26KEY_INDEX_R,
    MMIIM_SP_26KEY_INDEX_T,
    MMIIM_SP_26KEY_INDEX_Y,
    MMIIM_SP_26KEY_INDEX_U,
    MMIIM_SP_26KEY_INDEX_I,
    MMIIM_SP_26KEY_INDEX_O,
    MMIIM_SP_26KEY_INDEX_P,

    MMIIM_SP_26KEY_INDEX_A,
    MMIIM_SP_26KEY_INDEX_S,
    MMIIM_SP_26KEY_INDEX_D,
    MMIIM_SP_26KEY_INDEX_F,
    MMIIM_SP_26KEY_INDEX_G,
    MMIIM_SP_26KEY_INDEX_H,
    MMIIM_SP_26KEY_INDEX_J,
    MMIIM_SP_26KEY_INDEX_K,
    MMIIM_SP_26KEY_INDEX_L,
    MMIIM_SP_26KEY_INDEX_ENTER,

    MMIIM_SP_26KEY_INDEX_COMMA,
    MMIIM_SP_26KEY_INDEX_Z,
    MMIIM_SP_26KEY_INDEX_X,
    MMIIM_SP_26KEY_INDEX_C,
    MMIIM_SP_26KEY_INDEX_V,
    MMIIM_SP_26KEY_INDEX_B,
    MMIIM_SP_26KEY_INDEX_N,
    MMIIM_SP_26KEY_INDEX_M,
    MMIIM_SP_26KEY_INDEX_DEL,
    MMIIM_SP_26KEY_INDEX_MAX,   //前一个字符占用两个空格时，如此填写

    MMIIM_SP_26KEY_INDEX_MAX,   //s_26_def_cap
    MMIIM_SP_26KEY_INDEX_MAX,   //前一个字符占用两个空格时，如此填写
    MMIIM_SP_26KEY_INDEX_MAX,   //s_26_def_to_9不做修改
    MMIIM_SP_26KEY_INDEX_MAX,   //前一个字符占用两个空格时，如此填写
    MMIIM_SP_26KEY_INDEX_MAX,   //s_26_def_symbol不做修改
    MMIIM_SP_26KEY_INDEX_MAX,   //前一个字符占用两个空格时，如此填写
    MMIIM_SP_26KEY_INDEX_MAX,   //s_26_def_digit不做修改
    MMIIM_SP_26KEY_INDEX_MAX,   //前一个字符占用两个空格时，如此填写
    MMIIM_SP_26KEY_INDEX_MAX,   //s_26_def_space不做修改
    MMIIM_SP_26KEY_INDEX_MAX,   //前一个字符占用两个空格时，如此填写
};

LOCAL MMIIM_TP_KEY_DISPLAY_TEXT s_save_26kb_data[MMIIM_TP_CSTAR_ABC_VNUM * MMIIM_TP_CSTAR_ABC_HNUM];        //将默认数据保存到此数组中，通过s_mul_text_26kb_def_data调用
MMIIM_TP_KEY_DISPLAY_TEXT*  const s_mul_text_26kb_def_data[MMIIM_TP_CSTAR_ABC_VNUM * MMIIM_TP_CSTAR_ABC_HNUM] = 
{
    &s_save_26kb_data[0], 
    &s_save_26kb_data[1], 
    &s_save_26kb_data[2], 
    &s_save_26kb_data[3], 
    &s_save_26kb_data[4], 
    
    &s_save_26kb_data[5], 
    &s_save_26kb_data[6], 
    &s_save_26kb_data[7], 
    &s_save_26kb_data[8], 
    &s_save_26kb_data[9], 
    
    &s_save_26kb_data[10], 
    &s_save_26kb_data[11], 
    &s_save_26kb_data[12], 
    &s_save_26kb_data[13], 
    &s_save_26kb_data[14], 
    
    &s_save_26kb_data[15], 
    &s_save_26kb_data[16], 
    &s_save_26kb_data[17], 
    &s_save_26kb_data[18], 
    &s_26_def_enter,            //MMIIM_SP_26KEY_INDEX_ENTER

    &s_26_def_comma,        //MMIIM_SP_26KEY_INDEX_COMMA
    &s_save_26kb_data[21], 
    &s_save_26kb_data[22], 
    &s_save_26kb_data[23], 
    &s_save_26kb_data[24], 
    
    &s_save_26kb_data[25], 
    &s_save_26kb_data[26], 
    &s_save_26kb_data[27], 
    &s_26_def_backspace,        //MMIIM_SP_26KEY_INDEX_DEL
    &s_save_26kb_data[29], 
    
    &s_26_def_cap, 
    &s_save_26kb_data[31], 
    &s_26_def_to_9, 
    &s_save_26kb_data[33], 

    &s_26_def_symbol, 
    &s_save_26kb_data[35], 
    &s_26_def_digit, 
    &s_save_26kb_data[37], 
    &s_26_def_space, 
    &s_save_26kb_data[39], 
};

//数字键盘
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_1 = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_2 = {MMIIM_KB_KEY_DISP_TEXT_NONE};               
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_3 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_4 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_5 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_6 = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_7 = {MMIIM_KB_KEY_DISP_TEXT_NONE};               
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_8 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_9 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_0 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       

LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_cap = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_symbol = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_digit_txt_disp_backspace = {MMIIM_KB_KEY_DISP_TEXT_NONE};      


//此处之所以将部分没有显示的方格区域保留，主要是与前期程序兼容，便于控制
//否则需要计算按键索引值，具体可参见类似变量s_py_26kb_func
LOCAL const MMIIM_TP_KEY_DISPLAY_TEXT*  const s_digit_txt_disp_strtab[] =
{
    &s_digit_txt_disp_1, &s_digit_txt_disp_2, &s_digit_txt_disp_3, &s_digit_txt_disp_4, &s_digit_txt_disp_5,
    &s_digit_txt_disp_6, &s_digit_txt_disp_7, &s_digit_txt_disp_8, &s_digit_txt_disp_9, &s_digit_txt_disp_0,
    &s_digit_txt_disp_cap, &s_digit_txt_disp_symbol, NULL, &s_digit_txt_disp_backspace, NULL
};

//常用符号键盘
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_1 = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_2 = {MMIIM_KB_KEY_DISP_TEXT_NONE};               
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_3 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_4 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_5 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_6 = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_7 = {MMIIM_KB_KEY_DISP_TEXT_NONE};               
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_8 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_9 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_0 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       

LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_cap = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_symbol = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_symbol_txt_disp_backspace = {MMIIM_KB_KEY_DISP_TEXT_NONE};      


//此处之所以将部分没有显示的方格区域保留，主要是与前期程序兼容，便于控制
//否则需要计算按键索引值，具体可参见类似变量s_py_26kb_func
LOCAL const MMIIM_TP_KEY_DISPLAY_TEXT*  const s_symbol_txt_disp_strtab[] =
{
    &s_symbol_txt_disp_1, &s_symbol_txt_disp_2, &s_symbol_txt_disp_3, &s_symbol_txt_disp_4, &s_symbol_txt_disp_5,
    &s_symbol_txt_disp_6, &s_symbol_txt_disp_7, &s_symbol_txt_disp_8, &s_symbol_txt_disp_9, &s_symbol_txt_disp_0,
    &s_symbol_txt_disp_cap, &s_symbol_txt_disp_symbol, NULL, &s_symbol_txt_disp_backspace, NULL
};

#if defined (IM_SIMP_CHINESE_SUPPORT)
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_chinese_txt_disp_stroke_1 = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_chinese_txt_disp_stroke_2 = {MMIIM_KB_KEY_DISP_TEXT_NONE};               
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_chinese_txt_disp_stroke_3 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_chinese_txt_disp_stroke_4 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_chinese_txt_disp_stroke_5 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_chinese_txt_disp_stroke_6 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       


//此处之所以将部分没有显示的方格区域保留，主要是与前期程序兼容，便于控制
//否则需要计算按键索引值，具体可参见类似变量s_py_26kb_func
LOCAL const MMIIM_TP_KEY_DISPLAY_TEXT*  const s_chinese_txt_disp_stroke_strtab[] =
{
    &s_chinese_txt_disp_stroke_1, &s_chinese_txt_disp_stroke_2, &s_chinese_txt_disp_stroke_3, &s_9_def_backspace,
    &s_chinese_txt_disp_stroke_4, &s_chinese_txt_disp_stroke_5, &s_chinese_txt_disp_stroke_6, &s_9_def_enter,
    &s_9_def_cap, &s_9_def_symbol, &s_9_def_digit, &s_9_def_space,
};
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_trad_chinese_txt_disp_stroke_1 = {MMIIM_KB_KEY_DISP_TEXT_NONE};      
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_trad_chinese_txt_disp_stroke_2 = {MMIIM_KB_KEY_DISP_TEXT_NONE};               
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_trad_chinese_txt_disp_stroke_3 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_trad_chinese_txt_disp_stroke_4 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_trad_chinese_txt_disp_stroke_5 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       
LOCAL const  MMIIM_TP_KEY_DISPLAY_TEXT s_trad_chinese_txt_disp_stroke_6 = {MMIIM_KB_KEY_DISP_TEXT_NONE};       


//此处之所以将部分没有显示的方格区域保留，主要是与前期程序兼容，便于控制
//否则需要计算按键索引值，具体可参见类似变量s_py_26kb_func
LOCAL const MMIIM_TP_KEY_DISPLAY_TEXT*  const s_trad_chinese_txt_disp_stroke_strtab[] =
{
    &s_trad_chinese_txt_disp_stroke_1, &s_trad_chinese_txt_disp_stroke_2, &s_trad_chinese_txt_disp_stroke_3, &s_9_def_backspace,
    &s_trad_chinese_txt_disp_stroke_4, &s_trad_chinese_txt_disp_stroke_5, &s_trad_chinese_txt_disp_stroke_6, &s_9_def_enter,
    &s_9_def_cap, &s_9_def_symbol, &s_9_def_digit, &s_9_def_space,
};
#endif


/*lint -restore */
/*lint -restore */


/*
每个输入法状态所对应的配置表，无论如何请认真检查是否与头文件中定义的枚举顺序是否一致
最关键的一个表了，无论如何得认真
*/
//将虚拟键盘配置顺序作了一个调整，与枚举变量MMISET_LANGUAGE_TYPE_E的顺序一致，当然，前提是
//MMIIM_TP_CSTAR_STATE_ID_T的顺序也与MMISET_LANGUAGE_TYPE_E一致，这样尽量减少出错的可能性
/*lint -save -e605 */
MMIIM_TP_CSTAR_STATE_T g_keyboard_state[MMIIM_TP_CSTAR_STATE_MAX - MMIIM_TP_CSTAR_KB_START_ID] =
{
     //数字
    {MMIIM_TP_CSTAR_DIG_VNUM, MMIIM_TP_CSTAR_DIG_HNUM, s_dig_kb_functab, s_dig_strtab, ARR_SIZE(s_dig_strtab),
     PNULL, 0, 0, {0}, {0}, IMG_IMSP_NUM_OFF, IMG_IMSP_NUM_ON, s_digit_txt_disp_strtab, ARR_SIZE(s_digit_txt_disp_strtab)},

     //符号
    {MMIIM_TP_CSTAR_DIGSYM_VNUM, MMIIM_TP_CSTAR_DIGSYM_HNUM, s_digsym_kb_functab, s_digsym_strtab, ARR_SIZE(s_digsym_strtab),
     PNULL, 0, MMIIM_TP_CSTAR_LOCK, {0}, {0}, IMG_IMSP_NUM_SYMB_OFF, IMG_IMSP_NUM_SYMB_ON,
     s_symbol_txt_disp_strtab, ARR_SIZE(s_symbol_txt_disp_strtab)},

     //abc 26key三种状态
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_abc_lead_26kb_replace_key, ARR_SIZE(s_abc_lead_26kb_replace_key), 0, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_abc_lower_26kb_replace_key, ARR_SIZE(s_abc_lower_26kb_replace_key), 0, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_abc_upper_26kb_replace_key, ARR_SIZE(s_abc_upper_26kb_replace_key), 0, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_abc_9kb_lead_replace_key, ARR_SIZE(s_abc_9kb_lead_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_abc_9kb_lower_replace_key, ARR_SIZE(s_abc_9kb_lower_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_abc_9kb_upper_replace_key, ARR_SIZE(s_abc_9kb_upper_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#if defined (IM_ENGLISH_SUPPORT)
    //En 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_en_lead_26kb_replace_key, ARR_SIZE(s_en_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_en_lower_26kb_replace_key, ARR_SIZE(s_en_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_en_upper_26kb_replace_key, ARR_SIZE(s_en_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //En 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_en_9kb_lead_replace_key, ARR_SIZE(s_en_9kb_lead_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_en_9kb_lower_replace_key, ARR_SIZE(s_en_9kb_lower_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_en_9kb_upper_replace_key, ARR_SIZE(s_en_9kb_upper_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif
	 
#if defined (IM_SIMP_CHINESE_SUPPORT)
     //拼音26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_pyin_26kb_replace_key, ARR_SIZE(s_pyin_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //拼音9Key，对智能来讲，有必要多说几句，图片大小写、字符串在各个语言可以不一样，但是9键布局是一样的，各个按键的处理要归类在一个函数
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_pyin_9kb_replace_key, ARR_SIZE(s_pyin_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //笔画配置表
    {MMIIM_TP_CSTAR_STROKE_VNUM, MMIIM_TP_CSTAR_STROKE_HNUM, s_stroke_kb_func, s_stroke_strtab, ARR_SIZE(s_stroke_strtab), 
     PNULL, 0, MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_STROKE_OFF, IMG_IMSP_STROKE_ON, s_chinese_txt_disp_stroke_strtab, ARR_SIZE(s_chinese_txt_disp_stroke_strtab)},
#endif    

/*下面开始支持多国语了*/
#if defined (IM_TRAD_CHINESE_SUPPORT)
     //拼音26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_zyin_26kb_replace_key, ARR_SIZE(s_zyin_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //拼音9Key，对智能来讲，有必要多说几句，图片大小写、字符串在各个语言可以不一样，但是9键布局是一样的，各个按键的处理要归类在一个函数
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_zyin_9kb_replace_key, ARR_SIZE(s_zyin_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //笔画配置表
    {MMIIM_TP_CSTAR_STROKE_VNUM, MMIIM_TP_CSTAR_STROKE_HNUM, s_stroke_kb_func, s_stroke_strtab, ARR_SIZE(s_stroke_strtab), 
     PNULL, 0, MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_STROKE_OFF, IMG_IMSP_STROKE_ON, 
     s_trad_chinese_txt_disp_stroke_strtab, ARR_SIZE(s_trad_chinese_txt_disp_stroke_strtab)
     },
#endif

#if defined (IM_ARABIC_SUPPORT)
     //ARABIC 26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_foreign_26strtab, ARR_SIZE(s_foreign_26strtab), 
     s_sarb_26kb_replace_key, ARR_SIZE(s_sarb_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //ARABIC 9Key， 阿拉伯的9键，全键盘替换掉，其它也没有什么可以replace的键了
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sarb_9kb_replace_key, ARR_SIZE(s_sarb_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //ARABIC ABC 26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_foreign_26strtab, ARR_SIZE(s_foreign_26strtab), 
     s_arb_26kb_replace_key, ARR_SIZE(s_arb_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //ARABIC ABC 9Key， 阿拉伯的9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_arb_9kb_replace_key, ARR_SIZE(s_arb_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

#ifdef IM_FRENCH_SUPPORT
    //法语智能配置表26键
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sfrench_lead_26kb_replace_key, ARR_SIZE(s_sfrench_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sfrench_lower_26kb_replace_key, ARR_SIZE(s_sfrench_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sfrench_upper_26kb_replace_key, ARR_SIZE(s_sfrench_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //法语智能配置表9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sfrench_lead_9kb_replace_key, ARR_SIZE(s_sfrench_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sfrench_lower_9kb_replace_key, ARR_SIZE(s_sfrench_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sfrench_upper_9kb_replace_key, ARR_SIZE(s_sfrench_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //法语abc配置表26键
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_french_lead_26kb_replace_key, ARR_SIZE(s_french_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_french_lower_26kb_replace_key, ARR_SIZE(s_french_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_french_upper_26kb_replace_key, ARR_SIZE(s_french_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //法语abc配置表9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_french_lead_9kb_replace_key, ARR_SIZE(s_french_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_french_lower_9kb_replace_key, ARR_SIZE(s_french_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_french_upper_9kb_replace_key, ARR_SIZE(s_french_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//印第
#if defined (IM_HINDI_SUPPORT)
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_shindi_26kb_replace_key, ARR_SIZE(s_shindi_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_shindi_9kb_replace_key, ARR_SIZE(s_shindi_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //ABC 26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_hindi_26kb_replace_key, ARR_SIZE(s_hindi_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //ABC 9Key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_hindi_9kb_replace_key, ARR_SIZE(s_hindi_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif   

#ifdef IM_HUNGARIAN_SUPPORT//匈牙利语
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_shung_lead_26kb_replace_key, ARR_SIZE(s_shung_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_shung_lower_26kb_replace_key, ARR_SIZE(s_shung_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_shung_upper_26kb_replace_key, ARR_SIZE(s_shung_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_shung_lead_9kb_replace_key, ARR_SIZE(s_shung_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_shung_lower_9kb_replace_key, ARR_SIZE(s_shung_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_shung_upper_9kb_replace_key, ARR_SIZE(s_shung_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_hung_lead_26kb_replace_key, ARR_SIZE(s_hung_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_hung_lower_26kb_replace_key, ARR_SIZE(s_hung_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_hung_upper_26kb_replace_key, ARR_SIZE(s_hung_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_hung_lead_9kb_replace_key, ARR_SIZE(s_hung_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_hung_lower_9kb_replace_key, ARR_SIZE(s_hung_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_hung_upper_9kb_replace_key, ARR_SIZE(s_hung_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

#ifdef IM_INDONESIAN_SUPPORT//印度尼西亚
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sind_lead_26kb_replace_key, ARR_SIZE(s_sind_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sind_lower_26kb_replace_key, ARR_SIZE(s_sind_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sind_upper_26kb_replace_key, ARR_SIZE(s_sind_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sind_lead_9kb_replace_key, ARR_SIZE(s_sind_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sind_lower_9kb_replace_key, ARR_SIZE(s_sind_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sind_upper_9kb_replace_key, ARR_SIZE(s_sind_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},   

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ind_lead_26kb_replace_key, ARR_SIZE(s_ind_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ind_lower_26kb_replace_key, ARR_SIZE(s_ind_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ind_upper_26kb_replace_key, ARR_SIZE(s_ind_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ind_lead_9kb_replace_key, ARR_SIZE(s_ind_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ind_lower_9kb_replace_key, ARR_SIZE(s_ind_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ind_upper_9kb_replace_key, ARR_SIZE(s_ind_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT}, 
#endif

#ifdef IM_MALAY_SUPPORT//马来语
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_smalay_lead_26kb_replace_key, ARR_SIZE(s_smalay_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_smalay_lower_26kb_replace_key, ARR_SIZE(s_smalay_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_smalay_upper_26kb_replace_key, ARR_SIZE(s_smalay_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_smalay_lead_9kb_replace_key, ARR_SIZE(s_smalay_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_smalay_lower_9kb_replace_key, ARR_SIZE(s_smalay_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_smalay_upper_9kb_replace_key, ARR_SIZE(s_smalay_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},  

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_malay_lead_26kb_replace_key, ARR_SIZE(s_malay_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_malay_lower_26kb_replace_key, ARR_SIZE(s_malay_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_malay_upper_26kb_replace_key, ARR_SIZE(s_malay_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_malay_lead_9kb_replace_key, ARR_SIZE(s_malay_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_malay_lower_9kb_replace_key, ARR_SIZE(s_malay_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_malay_upper_9kb_replace_key, ARR_SIZE(s_malay_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//葡萄牙
#ifdef IM_PORTUGUESE_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sport_lead_26kb_replace_key, ARR_SIZE(s_sport_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sport_lower_26kb_replace_key, ARR_SIZE(s_sport_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sport_upper_26kb_replace_key, ARR_SIZE(s_sport_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sport_lead_9kb_replace_key, ARR_SIZE(s_sport_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sport_lower_9kb_replace_key, ARR_SIZE(s_sport_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sport_upper_9kb_replace_key, ARR_SIZE(s_sport_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_port_lead_26kb_replace_key, ARR_SIZE(s_port_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_port_lower_26kb_replace_key, ARR_SIZE(s_port_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_port_upper_26kb_replace_key, ARR_SIZE(s_port_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_port_lead_9kb_replace_key, ARR_SIZE(s_port_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_port_lower_9kb_replace_key, ARR_SIZE(s_port_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_port_upper_9kb_replace_key, ARR_SIZE(s_port_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

#ifdef IM_RUSSIAN_SUPPORT//俄语
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_srus_lead_26kb_replace_key, ARR_SIZE(s_srus_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_srus_lower_26kb_replace_key, ARR_SIZE(s_srus_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_srus_upper_26kb_replace_key, ARR_SIZE(s_srus_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_srus_lead_9kb_replace_key, ARR_SIZE(s_srus_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_srus_lower_9kb_replace_key, ARR_SIZE(s_srus_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_srus_upper_9kb_replace_key, ARR_SIZE(s_srus_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},  

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_rus_lead_26kb_replace_key, ARR_SIZE(s_rus_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_rus_lower_26kb_replace_key, ARR_SIZE(s_rus_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_rus_upper_26kb_replace_key, ARR_SIZE(s_rus_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_rus_lead_9kb_replace_key, ARR_SIZE(s_rus_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_rus_lower_9kb_replace_key, ARR_SIZE(s_rus_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_rus_upper_9kb_replace_key, ARR_SIZE(s_rus_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},       
#endif

//西班牙
#ifdef IM_SPANISH_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sspan_lead_26kb_replace_key, ARR_SIZE(s_sspan_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sspan_lower_26kb_replace_key, ARR_SIZE(s_sspan_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sspan_upper_26kb_replace_key, ARR_SIZE(s_sspan_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sspan_lead_9kb_replace_key, ARR_SIZE(s_sspan_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sspan_lower_9kb_replace_key, ARR_SIZE(s_sspan_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sspan_upper_9kb_replace_key, ARR_SIZE(s_sspan_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_span_lead_26kb_replace_key, ARR_SIZE(s_span_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_span_lower_26kb_replace_key, ARR_SIZE(s_span_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_span_upper_26kb_replace_key, ARR_SIZE(s_span_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_span_lead_9kb_replace_key, ARR_SIZE(s_span_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_span_lower_9kb_replace_key, ARR_SIZE(s_span_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_span_upper_9kb_replace_key, ARR_SIZE(s_span_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//他加诺multitap虚拟键盘部分	
#ifdef IM_TAGALOG_SUPPORT
    //multitap 
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_tagalog_lead_26kb_replace_key, ARR_SIZE(s_tagalog_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},   //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_tagalog_lead_9kb_replace_key, ARR_SIZE(s_tagalog_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},   //ABC_9KEY_UPPER
#endif

//泰语
#if defined (IM_THAI_SUPPORT)
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_sthai_26kb_replace_key, ARR_SIZE(s_sthai_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sthai_9kb_replace_key, ARR_SIZE(s_sthai_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //ABC 26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_thai_26kb_replace_key, ARR_SIZE(s_thai_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //ABC 9Key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_thai_9kb_replace_key, ARR_SIZE(s_thai_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif 

//越南语
#ifdef IM_VIETNAMESE_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sviet_lead_26kb_replace_key, ARR_SIZE(s_sviet_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sviet_lower_26kb_replace_key, ARR_SIZE(s_sviet_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sviet_upper_26kb_replace_key, ARR_SIZE(s_sviet_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sviet_lead_9kb_replace_key, ARR_SIZE(s_sviet_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sviet_lower_9kb_replace_key, ARR_SIZE(s_sviet_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sviet_upper_9kb_replace_key, ARR_SIZE(s_sviet_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_viet_lead_26kb_replace_key, ARR_SIZE(s_viet_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_viet_lower_26kb_replace_key, ARR_SIZE(s_viet_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_viet_upper_26kb_replace_key, ARR_SIZE(s_viet_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_viet_lead_9kb_replace_key, ARR_SIZE(s_viet_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_viet_lower_9kb_replace_key, ARR_SIZE(s_viet_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_viet_upper_9kb_replace_key, ARR_SIZE(s_viet_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//乌尔都
#if defined (IM_URDU_SUPPORT)
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_surdu_26kb_replace_key, ARR_SIZE(s_surdu_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_surdu_9kb_replace_key, ARR_SIZE(s_surdu_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //ABC 26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_urdu_26kb_replace_key, ARR_SIZE(s_urdu_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //ABC 9Key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_urdu_9kb_replace_key, ARR_SIZE(s_urdu_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif 

//意大利
#ifdef IM_ITALIAN_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sita_lead_26kb_replace_key, ARR_SIZE(s_sita_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sita_lower_26kb_replace_key, ARR_SIZE(s_sita_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sita_upper_26kb_replace_key, ARR_SIZE(s_sita_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sita_lead_9kb_replace_key, ARR_SIZE(s_sita_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sita_lower_9kb_replace_key, ARR_SIZE(s_sita_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sita_upper_9kb_replace_key, ARR_SIZE(s_sita_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ita_lead_26kb_replace_key, ARR_SIZE(s_ita_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ita_lower_26kb_replace_key, ARR_SIZE(s_ita_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ita_upper_26kb_replace_key, ARR_SIZE(s_ita_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ita_lead_9kb_replace_key, ARR_SIZE(s_ita_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ita_lower_9kb_replace_key, ARR_SIZE(s_ita_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ita_upper_9kb_replace_key, ARR_SIZE(s_ita_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//波斯
#ifdef IM_PERSIAN_SUPPORT
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_sper_26kb_replace_key, ARR_SIZE(s_sper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sper_9kb_replace_key, ARR_SIZE(s_sper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //ABC 26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_per_26kb_replace_key, ARR_SIZE(s_per_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //ABC 9Key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_per_9kb_replace_key, ARR_SIZE(s_per_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//土耳其
#ifdef IM_TURKISH_SUPPORT
    //26键
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_stur_lead_26kb_replace_key, ARR_SIZE(s_stur_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_stur_lower_26kb_replace_key, ARR_SIZE(s_stur_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_stur_upper_26kb_replace_key, ARR_SIZE(s_stur_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //9键
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_stur_lead_9kb_replace_key, ARR_SIZE(s_stur_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_stur_lower_9kb_replace_key, ARR_SIZE(s_stur_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_stur_upper_9kb_replace_key, ARR_SIZE(s_stur_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_tur_lead_26kb_replace_key, ARR_SIZE(s_tur_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_tur_lower_26kb_replace_key, ARR_SIZE(s_tur_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_tur_upper_26kb_replace_key, ARR_SIZE(s_tur_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_tur_lead_9kb_replace_key, ARR_SIZE(s_tur_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_tur_lower_9kb_replace_key, ARR_SIZE(s_tur_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_tur_upper_9kb_replace_key, ARR_SIZE(s_tur_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//德语
#ifdef IM_GERMAN_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sgerman_lead_26kb_replace_key, ARR_SIZE(s_sgerman_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sgerman_lower_26kb_replace_key, ARR_SIZE(s_sgerman_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sgerman_upper_26kb_replace_key, ARR_SIZE(s_sgerman_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sgerman_lead_9kb_replace_key, ARR_SIZE(s_sgerman_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sgerman_lower_9kb_replace_key, ARR_SIZE(s_sgerman_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sgerman_upper_9kb_replace_key, ARR_SIZE(s_sgerman_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_german_lead_26kb_replace_key, ARR_SIZE(s_german_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_german_lower_26kb_replace_key, ARR_SIZE(s_german_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_german_upper_26kb_replace_key, ARR_SIZE(s_german_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_german_lead_9kb_replace_key, ARR_SIZE(s_german_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_german_lower_9kb_replace_key, ARR_SIZE(s_german_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_german_upper_9kb_replace_key, ARR_SIZE(s_german_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//希腊
#ifdef IM_GREEK_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sgreek_lead_26kb_replace_key, ARR_SIZE(s_sgreek_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sgreek_lower_26kb_replace_key, ARR_SIZE(s_sgreek_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sgreek_upper_26kb_replace_key, ARR_SIZE(s_sgreek_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sgreek_lead_9kb_replace_key, ARR_SIZE(s_sgreek_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sgreek_lower_9kb_replace_key, ARR_SIZE(s_sgreek_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sgreek_upper_9kb_replace_key, ARR_SIZE(s_sgreek_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_greek_lead_26kb_replace_key, ARR_SIZE(s_greek_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_greek_lower_26kb_replace_key, ARR_SIZE(s_greek_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_greek_upper_26kb_replace_key, ARR_SIZE(s_greek_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_greek_lead_9kb_replace_key, ARR_SIZE(s_greek_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_greek_lower_9kb_replace_key, ARR_SIZE(s_greek_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_greek_upper_9kb_replace_key, ARR_SIZE(s_greek_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//希伯莱
#if defined (IM_HEBREW_SUPPORT)
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_shebrew_26kb_replace_key, ARR_SIZE(s_shebrew_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_shebrew_9kb_replace_key, ARR_SIZE(s_shebrew_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //ABC 26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_hebrew_26kb_replace_key, ARR_SIZE(s_hebrew_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //ABC 9Key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_hebrew_9kb_replace_key, ARR_SIZE(s_hebrew_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif 

//孟加拉
#if defined (IM_BENGALI_SUPPORT)
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_sben_26kb_replace_key, ARR_SIZE(s_sben_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sben_9kb_replace_key, ARR_SIZE(s_sben_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //ABC 26Key
    {MMIIM_TP_CSTAR_PY_VNUM, MMIIM_TP_CSTAR_PY_HNUM, s_py_26kb_func, s_py_26strtab, ARR_SIZE(s_py_26strtab), 
     s_ben_26kb_replace_key, ARR_SIZE(s_ben_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //ABC 9Key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ben_9kb_replace_key, ARR_SIZE(s_ben_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif 

//捷克
#ifdef IM_CZECH_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_scze_lead_26kb_replace_key, ARR_SIZE(s_scze_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_scze_lower_26kb_replace_key, ARR_SIZE(s_scze_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_scze_upper_26kb_replace_key, ARR_SIZE(s_scze_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_scze_lead_9kb_replace_key, ARR_SIZE(s_scze_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_scze_lower_9kb_replace_key, ARR_SIZE(s_scze_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_scze_upper_9kb_replace_key, ARR_SIZE(s_scze_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_cze_lead_26kb_replace_key, ARR_SIZE(s_cze_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_cze_lower_26kb_replace_key, ARR_SIZE(s_cze_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_cze_upper_26kb_replace_key, ARR_SIZE(s_cze_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_cze_lead_9kb_replace_key, ARR_SIZE(s_cze_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_cze_lower_9kb_replace_key, ARR_SIZE(s_cze_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_cze_upper_9kb_replace_key, ARR_SIZE(s_cze_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//斯诺文尼亚
#ifdef IM_SLOVENIAN_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sslo_lead_26kb_replace_key, ARR_SIZE(s_sslo_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sslo_lower_26kb_replace_key, ARR_SIZE(s_sslo_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sslo_upper_26kb_replace_key, ARR_SIZE(s_sslo_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sslo_lead_9kb_replace_key, ARR_SIZE(s_sslo_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sslo_lower_9kb_replace_key, ARR_SIZE(s_sslo_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sslo_upper_9kb_replace_key, ARR_SIZE(s_sslo_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slo_lead_26kb_replace_key, ARR_SIZE(s_slo_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slo_lower_26kb_replace_key, ARR_SIZE(s_slo_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slo_upper_26kb_replace_key, ARR_SIZE(s_slo_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slo_lead_9kb_replace_key, ARR_SIZE(s_slo_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slo_lower_9kb_replace_key, ARR_SIZE(s_slo_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slo_upper_9kb_replace_key, ARR_SIZE(s_slo_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//罗马尼亚
#ifdef IM_ROMANIAN_SUPPORT
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sroma_lead_26kb_replace_key, ARR_SIZE(s_sroma_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sroma_lower_26kb_replace_key, ARR_SIZE(s_sroma_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sroma_upper_26kb_replace_key, ARR_SIZE(s_sroma_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},


    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sroma_lead_9kb_replace_key, ARR_SIZE(s_sroma_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sroma_lower_9kb_replace_key, ARR_SIZE(s_sroma_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sroma_upper_9kb_replace_key, ARR_SIZE(s_sroma_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

    //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_roma_lead_26kb_replace_key, ARR_SIZE(s_roma_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_roma_lower_26kb_replace_key, ARR_SIZE(s_roma_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_roma_upper_26kb_replace_key, ARR_SIZE(s_roma_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_roma_lead_9kb_replace_key, ARR_SIZE(s_roma_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_roma_lower_9kb_replace_key, ARR_SIZE(s_roma_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_roma_upper_9kb_replace_key, ARR_SIZE(s_roma_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//泰卢固语	
#if defined (IM_TELUGU_SUPPORT)
    //multitap 
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_telugu_lead_26kb_replace_key, ARR_SIZE(s_telugu_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},   //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_telugu_lead_9kb_replace_key, ARR_SIZE(s_telugu_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},   //ABC_9KEY_UPPER
#endif

//马拉地语multitap关于虚拟键盘部分	
#ifdef IM_MARATHI_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_marathi_lead_26kb_replace_key, ARR_SIZE(s_marathi_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},   //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_marathi_lead_9kb_replace_key, ARR_SIZE(s_marathi_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},    //ABC_9KEY_UPPER

#endif

//泰米尔语multitap关于虚拟键盘部分	
#ifdef IM_TAMIL_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_tamil_lead_26kb_replace_key, ARR_SIZE(s_tamil_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},		//ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_tamil_lead_9kb_replace_key, ARR_SIZE(s_tamil_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//古吉拉特语multitap关于虚拟键盘部分	
#ifdef IM_GUJARATI_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_gujarati_lead_26kb_replace_key, ARR_SIZE(s_gujarati_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},    //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_gujarati_lead_9kb_replace_key, ARR_SIZE(s_gujarati_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//卡纳达语multitap关于虚拟键盘部分	
#ifdef IM_KANNADA_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_kannada_lead_26kb_replace_key, ARR_SIZE(s_kannada_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},    //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_kannada_lead_9kb_replace_key, ARR_SIZE(s_kannada_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//马拉亚拉姆语multitap关于虚拟键盘部分	
#ifdef IM_MALAYALAM_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_malayalam_lead_26kb_replace_key, ARR_SIZE(s_malayalam_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},    //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_malayalam_lead_9kb_replace_key, ARR_SIZE(s_malayalam_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//缅甸语multitap关于虚拟键盘部分	
#ifdef IM_MYANMAR_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_myanmar_26kb_func, s_myanmar_26strtab, ARR_SIZE(s_myanmar_26strtab), 
     s_myanmar_lead_26kb_replace_key, ARR_SIZE(s_myanmar_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},    //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_myanmar_lead_9kb_replace_key, ARR_SIZE(s_myanmar_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif
//奥里亚multitap关于虚拟键盘部分	
#ifdef IM_ORIYA_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_oriya_lead_26kb_replace_key, ARR_SIZE(s_oriya_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},		    //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_oriya_lead_9kb_replace_key, ARR_SIZE(s_oriya_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//旁遮普multitap关于虚拟键盘部分	
#ifdef IM_PUNJABI_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_punjabi_lead_26kb_replace_key, ARR_SIZE(s_punjabi_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},    //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_punjabi_lead_9kb_replace_key, ARR_SIZE(s_punjabi_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//南非荷兰语multitap关于虚拟键盘部分	
#ifdef IM_AFRIKAANS_SUPPORT

	//smart
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_safrikaans_lead_26kb_replace_key, ARR_SIZE(s_safrikaans_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_safrikaans_lower_26kb_replace_key, ARR_SIZE(s_safrikaans_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_safrikaans_upper_26kb_replace_key, ARR_SIZE(s_safrikaans_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_safrikaans_lead_9kb_replace_key, ARR_SIZE(s_safrikaans_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_safrikaans_lower_9kb_replace_key, ARR_SIZE(s_safrikaans_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_safrikaans_upper_9kb_replace_key, ARR_SIZE(s_safrikaans_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_afrikaans_lead_26kb_replace_key, ARR_SIZE(s_afrikaans_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_afrikaans_lower_26kb_replace_key, ARR_SIZE(s_afrikaans_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_afrikaans_upper_26kb_replace_key, ARR_SIZE(s_afrikaans_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_afrikaans_lead_9kb_replace_key, ARR_SIZE(s_afrikaans_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_afrikaans_lower_9kb_replace_key, ARR_SIZE(s_afrikaans_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_afrikaans_lower_9kb_replace_key, ARR_SIZE(s_afrikaans_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//阿尔巴尼亚语multitap关于虚拟键盘部分	
#ifdef IM_ALBANIAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_albanian_lead_26kb_replace_key, ARR_SIZE(s_albanian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_albanian_lower_26kb_replace_key, ARR_SIZE(s_albanian_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_albanian_upper_26kb_replace_key, ARR_SIZE(s_albanian_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_albanian_lead_9kb_replace_key, ARR_SIZE(s_albanian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_albanian_lower_9kb_replace_key, ARR_SIZE(s_albanian_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_albanian_upper_9kb_replace_key, ARR_SIZE(s_albanian_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//亚美尼亚multitap关于虚拟键盘部分	
#ifdef IM_ARMENIAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_armenian_lead_26kb_replace_key, ARR_SIZE(s_armenian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_armenian_lower_26kb_replace_key, ARR_SIZE(s_armenian_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_armenian_upper_26kb_replace_key, ARR_SIZE(s_armenian_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_armenian_lead_9kb_replace_key, ARR_SIZE(s_armenian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_armenian_lower_9kb_replace_key, ARR_SIZE(s_armenian_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_armenian_upper_9kb_replace_key, ARR_SIZE(s_armenian_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//阿塞拜疆语(属阿尔泰语系土耳其语族)multitap关于虚拟键盘部分	
#ifdef IM_AZERBAIJANI_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_azerbaijani_lead_26kb_replace_key, ARR_SIZE(s_azerbaijani_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_azerbaijani_lower_26kb_replace_key, ARR_SIZE(s_azerbaijani_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_azerbaijani_upper_26kb_replace_key, ARR_SIZE(s_azerbaijani_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_azerbaijani_lead_9kb_replace_key, ARR_SIZE(s_azerbaijani_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_azerbaijani_lower_26kb_replace_key, ARR_SIZE(s_azerbaijani_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_azerbaijani_upper_9kb_replace_key, ARR_SIZE(s_azerbaijani_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//巴斯克语multitap关于虚拟键盘部分	
#ifdef IM_BASQUE_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_basque_lead_26kb_replace_key, ARR_SIZE(s_basque_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_basque_lower_26kb_replace_key, ARR_SIZE(s_basque_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_basque_upper_26kb_replace_key, ARR_SIZE(s_basque_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_basque_lead_9kb_replace_key, ARR_SIZE(s_basque_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_basque_lower_9kb_replace_key, ARR_SIZE(s_basque_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_basque_upper_9kb_replace_key, ARR_SIZE(s_basque_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//保加利亚
#ifdef IM_BULGARIAN_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sbulg_lead_26kb_replace_key, ARR_SIZE(s_sbulg_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sbulg_lower_26kb_replace_key, ARR_SIZE(s_sbulg_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sbulg_upper_26kb_replace_key, ARR_SIZE(s_sbulg_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sbulg_lead_9kb_replace_key, ARR_SIZE(s_sbulg_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sbulg_lower_9kb_replace_key, ARR_SIZE(s_sbulg_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sbulg_upper_9kb_replace_key, ARR_SIZE(s_sbulg_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_bulg_lead_26kb_replace_key, ARR_SIZE(s_bulg_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_bulg_lower_26kb_replace_key, ARR_SIZE(s_bulg_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_bulg_upper_26kb_replace_key, ARR_SIZE(s_bulg_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_bulg_lead_9kb_replace_key, ARR_SIZE(s_bulg_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_bulg_lower_9kb_replace_key, ARR_SIZE(s_bulg_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_bulg_upper_9kb_replace_key, ARR_SIZE(s_bulg_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//加泰罗尼亚语multitap关于虚拟键盘部分	
#ifdef IM_CATALAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_catalan_lead_26kb_replace_key, ARR_SIZE(s_catalan_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_catalan_lower_26kb_replace_key, ARR_SIZE(s_catalan_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_catalan_upper_26kb_replace_key, ARR_SIZE(s_catalan_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_catalan_lead_9kb_replace_key, ARR_SIZE(s_catalan_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_catalan_lower_9kb_replace_key, ARR_SIZE(s_catalan_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_catalan_upper_9kb_replace_key, ARR_SIZE(s_catalan_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//克罗地亚语
#ifdef IM_CROATIAN_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_scroatian_lead_26kb_replace_key, ARR_SIZE(s_scroatian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_scroatian_lower_26kb_replace_key, ARR_SIZE(s_scroatian_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_scroatian_upper_26kb_replace_key, ARR_SIZE(s_scroatian_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_scroatian_lead_9kb_replace_key, ARR_SIZE(s_scroatian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_scroatian_lower_9kb_replace_key, ARR_SIZE(s_scroatian_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_scroatian_upper_9kb_replace_key, ARR_SIZE(s_scroatian_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_croatian_lead_26kb_replace_key, ARR_SIZE(s_croatian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_croatian_lower_26kb_replace_key, ARR_SIZE(s_croatian_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_croatian_upper_26kb_replace_key, ARR_SIZE(s_croatian_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_croatian_lead_9kb_replace_key, ARR_SIZE(s_croatian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_croatian_lower_9kb_replace_key, ARR_SIZE(s_croatian_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_croatian_upper_9kb_replace_key, ARR_SIZE(s_croatian_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//丹麦
#ifdef IM_DANISH_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sdanish_lead_26kb_replace_key, ARR_SIZE(s_sdanish_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sdanish_lower_26kb_replace_key, ARR_SIZE(s_sdanish_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sdanish_upper_26kb_replace_key, ARR_SIZE(s_sdanish_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sdanish_lead_9kb_replace_key, ARR_SIZE(s_sdanish_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sdanish_lower_9kb_replace_key, ARR_SIZE(s_sdanish_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sdanish_upper_9kb_replace_key, ARR_SIZE(s_sdanish_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_danish_lead_26kb_replace_key, ARR_SIZE(s_danish_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_danish_lower_26kb_replace_key, ARR_SIZE(s_danish_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_danish_upper_26kb_replace_key, ARR_SIZE(s_danish_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_danish_lead_9kb_replace_key, ARR_SIZE(s_danish_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_danish_lower_9kb_replace_key, ARR_SIZE(s_danish_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_danish_upper_9kb_replace_key, ARR_SIZE(s_danish_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//荷兰
#ifdef IM_DUTCH_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sdutch_lead_26kb_replace_key, ARR_SIZE(s_sdutch_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sdutch_lower_26kb_replace_key, ARR_SIZE(s_sdutch_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sdutch_upper_26kb_replace_key, ARR_SIZE(s_sdutch_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sdutch_lead_9kb_replace_key, ARR_SIZE(s_sdutch_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sdutch_lower_9kb_replace_key, ARR_SIZE(s_sdutch_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sdutch_upper_9kb_replace_key, ARR_SIZE(s_sdutch_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_dutch_lead_26kb_replace_key, ARR_SIZE(s_dutch_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_dutch_lower_26kb_replace_key, ARR_SIZE(s_dutch_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_dutch_upper_26kb_replace_key, ARR_SIZE(s_dutch_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_dutch_lead_9kb_replace_key, ARR_SIZE(s_dutch_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_dutch_lower_9kb_replace_key, ARR_SIZE(s_dutch_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_dutch_upper_9kb_replace_key, ARR_SIZE(s_dutch_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//爱沙尼亚multitap关于虚拟键盘部分	
#ifdef IM_ESTONIAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_estonian_lead_26kb_replace_key, ARR_SIZE(s_estonian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_estonian_lower_26kb_replace_key, ARR_SIZE(s_estonian_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_estonian_upper_26kb_replace_key, ARR_SIZE(s_estonian_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_estonian_lead_9kb_replace_key, ARR_SIZE(s_estonian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_estonian_lower_9kb_replace_key, ARR_SIZE(s_estonian_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_estonian_upper_9kb_replace_key, ARR_SIZE(s_estonian_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//菲律宾multitap关于虚拟键盘部分	
#ifdef IM_FILIPINO_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_filipino_lead_26kb_replace_key, ARR_SIZE(s_filipino_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_filipino_lower_26kb_replace_key, ARR_SIZE(s_filipino_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_filipino_upper_26kb_replace_key, ARR_SIZE(s_filipino_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_filipino_lead_9kb_replace_key, ARR_SIZE(s_filipino_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_filipino_lower_9kb_replace_key, ARR_SIZE(s_filipino_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_filipino_upper_9kb_replace_key, ARR_SIZE(s_filipino_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//芬兰multitap关于虚拟键盘部分	
#ifdef IM_FINNISH_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sfi_lead_26kb_replace_key, ARR_SIZE(s_sfi_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sfi_lower_26kb_replace_key, ARR_SIZE(s_sfi_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sfi_upper_26kb_replace_key, ARR_SIZE(s_sfi_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sfi_lead_9kb_replace_key, ARR_SIZE(s_sfi_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sfi_lower_9kb_replace_key, ARR_SIZE(s_sfi_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sfi_upper_9kb_replace_key, ARR_SIZE(s_sfi_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_fi_lead_26kb_replace_key, ARR_SIZE(s_fi_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_fi_lower_26kb_replace_key, ARR_SIZE(s_fi_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_fi_upper_26kb_replace_key, ARR_SIZE(s_fi_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_fi_lead_9kb_replace_key, ARR_SIZE(s_fi_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_fi_lower_9kb_replace_key, ARR_SIZE(s_fi_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_fi_upper_9kb_replace_key, ARR_SIZE(s_fi_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//加利西亚语multitap关于虚拟键盘部分	
#ifdef IM_GALICIAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_galician_lead_26kb_replace_key, ARR_SIZE(s_galician_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_galician_lower_26kb_replace_key, ARR_SIZE(s_galician_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_galician_upper_26kb_replace_key, ARR_SIZE(s_galician_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_galician_lead_9kb_replace_key, ARR_SIZE(s_galician_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_galician_lower_9kb_replace_key, ARR_SIZE(s_galician_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_galician_upper_9kb_replace_key, ARR_SIZE(s_galician_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//格鲁吉亚multitap关于虚拟键盘部分	
#ifdef IM_GEORGIAN_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_georgian_lead_26kb_replace_key, ARR_SIZE(s_georgian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},    //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     s_georgian_lead_9kb_replace_key, ARR_SIZE(s_georgian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

//豪萨语(西非语系中的斯瓦西里语multitap关于虚拟键盘部分	
#ifdef IM_HAUSA_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_hausa_lead_26kb_replace_key, ARR_SIZE(s_hausa_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_hausa_lower_26kb_replace_key, ARR_SIZE(s_hausa_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_hausa_upper_26kb_replace_key, ARR_SIZE(s_hausa_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_hausa_lead_9kb_replace_key, ARR_SIZE(s_hausa_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_hausa_lower_9kb_replace_key, ARR_SIZE(s_hausa_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_hausa_upper_9kb_replace_key, ARR_SIZE(s_hausa_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//冰岛语multitap关于虚拟键盘部分	
#ifdef IM_ICELANDIC_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_icelandic_lead_26kb_replace_key, ARR_SIZE(s_icelandic_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_icelandic_lower_26kb_replace_key, ARR_SIZE(s_icelandic_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_icelandic_upper_26kb_replace_key, ARR_SIZE(s_icelandic_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_icelandic_lead_9kb_replace_key, ARR_SIZE(s_icelandic_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_icelandic_lower_9kb_replace_key, ARR_SIZE(s_icelandic_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_icelandic_upper_9kb_replace_key, ARR_SIZE(s_icelandic_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//伊博语multitap关于虚拟键盘部分	
#ifdef IM_IGBO_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_igbo_lead_26kb_replace_key, ARR_SIZE(s_igbo_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_igbo_lower_26kb_replace_key, ARR_SIZE(s_igbo_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_igbo_upper_26kb_replace_key, ARR_SIZE(s_igbo_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_igbo_lead_9kb_replace_key, ARR_SIZE(s_igbo_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_igbo_lower_9kb_replace_key, ARR_SIZE(s_igbo_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_igbo_upper_9kb_replace_key, ARR_SIZE(s_igbo_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//爱尔兰语multitap关于虚拟键盘部分	
#ifdef IM_IRISH_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_irish_lead_26kb_replace_key, ARR_SIZE(s_irish_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_irish_lower_26kb_replace_key, ARR_SIZE(s_irish_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_irish_upper_26kb_replace_key, ARR_SIZE(s_irish_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_irish_lead_9kb_replace_key, ARR_SIZE(s_irish_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_irish_lower_9kb_replace_key, ARR_SIZE(s_irish_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_irish_upper_9kb_replace_key, ARR_SIZE(s_irish_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//哈萨克斯坦multitap关于虚拟键盘部分	
#ifdef IM_KAZAKH_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_kazakh_lead_26kb_replace_key, ARR_SIZE(s_kazakh_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_kazakh_lower_26kb_replace_key, ARR_SIZE(s_kazakh_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_kazakh_upper_26kb_replace_key, ARR_SIZE(s_kazakh_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_kazakh_lead_9kb_replace_key, ARR_SIZE(s_kazakh_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_kazakh_lower_9kb_replace_key, ARR_SIZE(s_kazakh_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_kazakh_upper_9kb_replace_key, ARR_SIZE(s_kazakh_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//拉脱维亚multitap关于虚拟键盘部分	
#ifdef IM_LATVIAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_latvian_lead_26kb_replace_key, ARR_SIZE(s_latvian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_latvian_lower_26kb_replace_key, ARR_SIZE(s_latvian_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_latvian_upper_26kb_replace_key, ARR_SIZE(s_latvian_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_latvian_lead_9kb_replace_key, ARR_SIZE(s_latvian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_latvian_lower_9kb_replace_key, ARR_SIZE(s_latvian_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_latvian_upper_9kb_replace_key, ARR_SIZE(s_latvian_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//立陶宛语
#ifdef IM_LITHUANIAN_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slithu_lead_26kb_replace_key, ARR_SIZE(s_slithu_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slithu_lower_26kb_replace_key, ARR_SIZE(s_slithu_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slithu_upper_26kb_replace_key, ARR_SIZE(s_slithu_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slithu_lead_9kb_replace_key, ARR_SIZE(s_slithu_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slithu_lower_9kb_replace_key, ARR_SIZE(s_slithu_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slithu_upper_9kb_replace_key, ARR_SIZE(s_slithu_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_lithu_lead_26kb_replace_key, ARR_SIZE(s_lithu_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_lithu_lower_26kb_replace_key, ARR_SIZE(s_lithu_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_lithu_upper_26kb_replace_key, ARR_SIZE(s_lithu_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_lithu_lead_9kb_replace_key, ARR_SIZE(s_lithu_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_lithu_lower_9kb_replace_key, ARR_SIZE(s_lithu_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_lithu_upper_9kb_replace_key, ARR_SIZE(s_lithu_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif


//马其顿语multitap关于虚拟键盘部分	
#ifdef IM_MACEDONIAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_macedonian_lead_26kb_replace_key, ARR_SIZE(s_macedonian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_macedonian_lower_26kb_replace_key, ARR_SIZE(s_macedonian_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_macedonian_upper_26kb_replace_key, ARR_SIZE(s_macedonian_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_macedonian_lead_9kb_replace_key, ARR_SIZE(s_macedonian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_macedonian_lower_9kb_replace_key, ARR_SIZE(s_macedonian_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_macedonian_upper_9kb_replace_key, ARR_SIZE(s_macedonian_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//摩尔多瓦multitap关于虚拟键盘部分	
#ifdef IM_MOLDOVAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_moldonav_lead_26kb_replace_key, ARR_SIZE(s_moldonav_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_moldonav_lower_26kb_replace_key, ARR_SIZE(s_moldonav_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_moldonav_upper_26kb_replace_key, ARR_SIZE(s_moldonav_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_moldonav_lead_9kb_replace_key, ARR_SIZE(s_moldonav_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_moldonav_lower_9kb_replace_key, ARR_SIZE(s_moldonav_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_moldonav_upper_9kb_replace_key, ARR_SIZE(s_moldonav_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//挪威
#ifdef IM_NORWEGIAN_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_snorw_lead_26kb_replace_key, ARR_SIZE(s_snorw_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_snorw_lower_26kb_replace_key, ARR_SIZE(s_snorw_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_snorw_upper_26kb_replace_key, ARR_SIZE(s_snorw_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_snorw_lead_9kb_replace_key, ARR_SIZE(s_snorw_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_snorw_lower_9kb_replace_key, ARR_SIZE(s_snorw_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_snorw_upper_9kb_replace_key, ARR_SIZE(s_snorw_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_norw_lead_26kb_replace_key, ARR_SIZE(s_norw_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_norw_lower_26kb_replace_key, ARR_SIZE(s_norw_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_norw_upper_26kb_replace_key, ARR_SIZE(s_norw_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_norw_lead_9kb_replace_key, ARR_SIZE(s_norw_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_norw_lower_9kb_replace_key, ARR_SIZE(s_norw_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_norw_upper_9kb_replace_key, ARR_SIZE(s_norw_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//波兰
#ifdef IM_POLISH_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_spolish_lead_26kb_replace_key, ARR_SIZE(s_spolish_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_spolish_lower_26kb_replace_key, ARR_SIZE(s_spolish_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_spolish_upper_26kb_replace_key, ARR_SIZE(s_spolish_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_spolish_lead_9kb_replace_key, ARR_SIZE(s_spolish_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_spolish_lower_9kb_replace_key, ARR_SIZE(s_spolish_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_spolish_upper_9kb_replace_key, ARR_SIZE(s_spolish_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_polish_lead_26kb_replace_key, ARR_SIZE(s_polish_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_polish_lower_26kb_replace_key, ARR_SIZE(s_polish_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_polish_upper_26kb_replace_key, ARR_SIZE(s_polish_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_polish_lead_9kb_replace_key, ARR_SIZE(s_polish_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_polish_lower_9kb_replace_key, ARR_SIZE(s_polish_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_polish_upper_9kb_replace_key, ARR_SIZE(s_polish_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//塞尔维亚语multitap关于虚拟键盘部分	
#ifdef IM_SERBIAN_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_serbian_lead_26kb_replace_key, ARR_SIZE(s_serbian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_serbian_lower_26kb_replace_key, ARR_SIZE(s_serbian_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_serbian_upper_26kb_replace_key, ARR_SIZE(s_serbian_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_serbian_lead_9kb_replace_key, ARR_SIZE(s_serbian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_serbian_lower_9kb_replace_key, ARR_SIZE(s_serbian_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_serbian_upper_9kb_replace_key, ARR_SIZE(s_serbian_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//塞索托语(属班图语系) multitap关于虚拟键盘部分	
#ifdef IM_SESOTHO_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sesotho_lead_26kb_replace_key, ARR_SIZE(s_sesotho_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sesotho_lower_26kb_replace_key, ARR_SIZE(s_sesotho_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sesotho_upper_26kb_replace_key, ARR_SIZE(s_sesotho_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sesotho_lead_9kb_replace_key, ARR_SIZE(s_sesotho_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sesotho_lower_9kb_replace_key, ARR_SIZE(s_sesotho_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sesotho_upper_9kb_replace_key, ARR_SIZE(s_sesotho_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//斯诺伐克
#ifdef IM_SLOVAK_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sslovak_lead_26kb_replace_key, ARR_SIZE(s_sslovak_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sslovak_lower_26kb_replace_key, ARR_SIZE(s_sslovak_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sslovak_upper_26kb_replace_key, ARR_SIZE(s_sslovak_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sslovak_lead_9kb_replace_key, ARR_SIZE(s_sslovak_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sslovak_lower_9kb_replace_key, ARR_SIZE(s_sslovak_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sslovak_upper_9kb_replace_key, ARR_SIZE(s_sslovak_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slovak_lead_26kb_replace_key, ARR_SIZE(s_slovak_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slovak_lower_26kb_replace_key, ARR_SIZE(s_slovak_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_slovak_upper_26kb_replace_key, ARR_SIZE(s_slovak_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slovak_lead_9kb_replace_key, ARR_SIZE(s_slovak_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slovak_lower_9kb_replace_key, ARR_SIZE(s_slovak_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_slovak_upper_9kb_replace_key, ARR_SIZE(s_slovak_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

#ifdef IM_SWEDISH_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sswed_lead_26kb_replace_key, ARR_SIZE(s_sswed_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sswed_lower_26kb_replace_key, ARR_SIZE(s_sswed_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sswed_upper_26kb_replace_key, ARR_SIZE(s_sswed_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sswed_lead_9kb_replace_key, ARR_SIZE(s_sswed_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sswed_lower_9kb_replace_key, ARR_SIZE(s_sswed_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sswed_upper_9kb_replace_key, ARR_SIZE(s_sswed_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_swed_lead_26kb_replace_key, ARR_SIZE(s_swed_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_swed_lower_26kb_replace_key, ARR_SIZE(s_swed_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_swed_upper_26kb_replace_key, ARR_SIZE(s_swed_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_swed_lead_9kb_replace_key, ARR_SIZE(s_swed_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_swed_lower_9kb_replace_key, ARR_SIZE(s_swed_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_swed_upper_9kb_replace_key, ARR_SIZE(s_swed_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

#ifdef IM_UKRAINIAN_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sukra_lead_26kb_replace_key, ARR_SIZE(s_sukra_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sukra_lower_26kb_replace_key, ARR_SIZE(s_sukra_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sukra_upper_26kb_replace_key, ARR_SIZE(s_sukra_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sukra_lead_9kb_replace_key, ARR_SIZE(s_sukra_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sukra_lower_9kb_replace_key, ARR_SIZE(s_sukra_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sukra_upper_9kb_replace_key, ARR_SIZE(s_sukra_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ukra_lead_26kb_replace_key, ARR_SIZE(s_ukra_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ukra_lower_26kb_replace_key, ARR_SIZE(s_ukra_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_ukra_upper_26kb_replace_key, ARR_SIZE(s_ukra_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ukra_lead_9kb_replace_key, ARR_SIZE(s_ukra_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ukra_lower_9kb_replace_key, ARR_SIZE(s_ukra_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_ukra_upper_9kb_replace_key, ARR_SIZE(s_ukra_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},

#endif

// 约鲁巴语 multitap关于虚拟键盘部分	
#ifdef IM_YORUBA_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_yoruba_lead_26kb_replace_key, ARR_SIZE(s_yoruba_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_yoruba_lower_26kb_replace_key, ARR_SIZE(s_yoruba_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_yoruba_upper_26kb_replace_key, ARR_SIZE(s_yoruba_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_yoruba_lead_9kb_replace_key, ARR_SIZE(s_yoruba_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_yoruba_lower_9kb_replace_key, ARR_SIZE(s_yoruba_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_yoruba_upper_9kb_replace_key, ARR_SIZE(s_yoruba_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//科萨语	 multitap关于虚拟键盘部分	
#ifdef IM_XHOSA_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_xhosa_lead_26kb_replace_key, ARR_SIZE(s_xhosa_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_xhosa_lower_26kb_replace_key, ARR_SIZE(s_xhosa_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_xhosa_upper_26kb_replace_key, ARR_SIZE(s_xhosa_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_xhosa_lead_9kb_replace_key, ARR_SIZE(s_xhosa_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_xhosa_lower_9kb_replace_key, ARR_SIZE(s_xhosa_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_xhosa_upper_9kb_replace_key, ARR_SIZE(s_xhosa_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//祖鲁语multitap关于虚拟键盘部分	
#ifdef IM_ZULU_SUPPORT
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_zulu_lead_26kb_replace_key, ARR_SIZE(s_zulu_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND  | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_zulu_lower_26kb_replace_key, ARR_SIZE(s_zulu_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND  | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_zulu_upper_26kb_replace_key, ARR_SIZE(s_zulu_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND  | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_zulu_lead_9kb_replace_key, ARR_SIZE(s_zulu_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_zulu_lower_9kb_replace_key, ARR_SIZE(s_zulu_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_zulu_upper_9kb_replace_key, ARR_SIZE(s_zulu_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND  | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//印度语支的阿萨姆语	multitap关于虚拟键盘部分	
#ifdef IM_ASSAMESE_SUPPORT
    //multitap
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     NULL, 0, MMIIM_TP_CSTAR_CAND  | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},    //ABC_26KEY_UPPER

    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9kb_func), 
     NULL, 0, MMIIM_TP_CSTAR_CAND  | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

//斯瓦西里语	
#ifdef IM_SWAHILI_SUPPORT
    //智能26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sswahl_lead_26kb_replace_key, ARR_SIZE(s_sswahl_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sswahl_lower_26kb_replace_key, ARR_SIZE(s_sswahl_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_en_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_sswahl_upper_26kb_replace_key, ARR_SIZE(s_sswahl_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY, 
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
     
    //智能9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sswahl_lead_9kb_replace_key, ARR_SIZE(s_sswahl_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sswahl_lower_9kb_replace_key, ARR_SIZE(s_sswahl_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_sswahl_upper_9kb_replace_key, ARR_SIZE(s_sswahl_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
     
     //abc 26key
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_swahl_lead_26kb_replace_key, ARR_SIZE(s_swahl_lead_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_swahl_lower_26kb_replace_key, ARR_SIZE(s_swahl_lower_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_ABC_VNUM, MMIIM_TP_CSTAR_ABC_HNUM, s_abc_26kb_func, s_abc_26strtab, ARR_SIZE(s_abc_26strtab), 
     s_swahl_upper_26kb_replace_key, ARR_SIZE(s_swahl_upper_26kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN26_KB_BACKGROUND, IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT},

     //abc 9key
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_swahl_lead_9kb_replace_key, ARR_SIZE(s_swahl_lead_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_swahl_lower_9kb_replace_key, ARR_SIZE(s_swahl_lower_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
    {MMIIM_TP_CSTAR_9KEY_ABC_VNUM, MMIIM_TP_CSTAR_9KEY_ABC_HNUM, s_abc_9kb_func, s_abc_9strtab, ARR_SIZE(s_abc_9strtab), 
     s_swahl_upper_9kb_replace_key, ARR_SIZE(s_swahl_upper_9kb_replace_key), MMIIM_TP_CSTAR_CAND | MMIIM_TP_CSTAR_ED | MMIIM_TP_CSTAR_PY,
     {0}, {0}, IMG_IMSP_EN9_KB_BACKGROUND, IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT},
#endif

};
/*lint -restore */

#endif//MMIIM_CORE_USE_CSTAR


