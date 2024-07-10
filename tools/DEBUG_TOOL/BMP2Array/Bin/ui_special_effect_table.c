/******************************************************************************
 ** File Name:      	ui_special_effect_table.c                                                                              *
 ** Author:           Shan.He                                                                                            *
 ** DATE:             2008-7-3                                                                                          *
 ** Copyright:       2008 Spreatrum, Incoporated. All Rights Reserved.                                  *
 ** Description:     tables for ui_special_effect.c                                                                         *
 ** Note:            Warning: Do not edit what you see in this file!! It will be generated                                    *
 **									 by the tool.																																										*
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                                                                       *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                                                                 * 
 ** 2008-07-30      Shan.He             Create.                                                                     *
 *****************************************************************************/

#include "sci_types.h"

/****************************MICRO definitions*************************************/
//inter micro definitions here
//#MICRO_DEFINITION_BEGIN
//#MICRO_DEFINITION_END

#ifdef UI_P3D_SUPPORT
/****************************Desktop rotation tables*************************************/
PUBLIC  const uint32 g_desktop_tbl_size = DESKTOP_WIDTH;
//inter desktop rotation tables here
//#DESKTOP_ROTATION_TABLE_BEGIN
//#DESKTOP_ROTATION_TABLE_END

/****************************Distort move tables*************************************/
//inter distort tables here
//#DISTORT_MOVE_TABLE_BEGIN
//#DISTORT_MOVE_TABLE_END

PUBLIC const uint32 g_distort_vert_tbl_size = ICON_HEIGHT;
PUBLIC const uint32* g_distort_vert_tbl[] = 
{
    s_distort_vert_tbl_0, s_distort_vert_tbl_1, s_distort_vert_tbl_2, s_distort_vert_tbl_3,
    s_distort_vert_tbl_4, s_distort_vert_tbl_5, s_distort_vert_tbl_6, 
    s_distort_vert_tbl_5, s_distort_vert_tbl_4, 
    s_distort_vert_tbl_3, s_distort_vert_tbl_2, s_distort_vert_tbl_1, s_distort_vert_tbl_0,
};


PUBLIC const uint32 g_distort_horz_tbl_size = ICON_WIDTH;
PUBLIC const uint32* g_distort_horz_tbl[] = 
{
    s_distort_horz_tbl_0, s_distort_horz_tbl_1, s_distort_horz_tbl_2, s_distort_horz_tbl_3,
    s_distort_horz_tbl_4, s_distort_horz_tbl_5, s_distort_horz_tbl_6, 
    s_distort_horz_tbl_5, s_distort_horz_tbl_4, 
    s_distort_horz_tbl_3, s_distort_horz_tbl_2, s_distort_horz_tbl_1, s_distort_horz_tbl_0,
};

/****************************Slide move tables*************************************/
PUBLIC  const uint32 g_slide_move_horz_tbl_size = ICON_WIDTH;
//inter desktop rotation tables here
//#SLIDE_MOVE__TABLE_BEGIN
//#SLIDE_MOVE_TABLE_END
#else

PUBLIC const uint32 g_distort_vert_tbl_size = ICON_HEIGHT;
PUBLIC const uint32* g_distort_vert_tbl[] = 
{
    NULL
};


PUBLIC const uint32 g_distort_horz_tbl_size = ICON_WIDTH;
PUBLIC const uint32* g_distort_horz_tbl[] = 
{
    NULL
};

PUBLIC  const uint32 g_desktop_tbl_size = DESKTOP_WIDTH;

//used for desktop rotate, height increasment is (1/8) * height, for 320 * 240 desktop
PUBLIC const uint32 g_desktop_rotate_small_inc_tbl[] =
{
    0
};

//used for desktop rotate, height increasment is (1/4) * height, for 320 * 240 desktop
PUBLIC const uint32 g_desktop_rotate_large_inc_tbl[]=
{
    0
};

PUBLIC  const uint32 g_slide_move_horz_tbl_size = ICON_WIDTH;

//horizental slide move table, original height = 80, left height = 80, right height = 20
PUBLIC const uint32 g_slide_move_horz_major_tbl_0[] =
{
    0
};

//horizental slide move table, original height = 80, left height = 60, right height = 10
PUBLIC const uint32 g_slide_move_horz_minor_tbl_0[] =
{		
    0
};

//horizental slide move table, original height = 80, left height = 40, right height = 10
PUBLIC const uint32 g_slide_move_horz_minor_tbl_1[] =
{
    0
};

PUBLIC  const uint32 g_slide_move_vert_tbl_size = ICON_HEIGHT;

//vertical slide move table, original height = 80, top width = 80, bottom width = 20
PUBLIC const uint32 g_slide_move_vert_major_tbl_0[]=
{
    0
};

//vertical slide move table, original height = 80, top width = 60, bottom width = 10
PUBLIC const uint32 g_slide_move_vert_minor_tbl_0[]=
{
    0
};

//vertical slide move table, original width = 80, top width = 40, bottom width = 10
PUBLIC const uint32 g_slide_move_vert_minor_tbl_1[] =
{
    0
};
#endif
