/******************************************************************************
 ** File Name:    effect_grid.h                                               *
 ** Author:       Allen.Mao                                                   *
 ** DATE:         02/08/2011                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 02/08/2011    Allen.Mao       Create.                                     *
 ******************************************************************************/

#ifndef _EFFECT_GRID_H_
#define _EFFECT_GRID_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "special_effect_util.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/ 
#define GRID_COLOR_WIDTH  40
#define GRID_GAP          5
#define GRID_WIDTH  (GRID_COLOR_WIDTH + GRID_GAP)

#define GRID_MOVE_STEPS   9
#define GRID_COLOR_LEVEL  15
#define GRID_SCALING_LEVEL 13

/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/

enum GRID_MODE_ACTION
{
    GRID_MODE_NOP = 0,
    GRID_MODE_NORMAL = 1,
    GRID_MODE_MOVING,
    GRID_MODE_SCALING,
};

enum GRID_MOVE_DIRECTION
{
	GRID_MOVE_LEFT = 1,
	GRID_MOVE_RIGHT ,
    GRID_MOVE_DOWN,
    GRID_MOVE_UP,
};

typedef struct tag_grid_status{
    uint16 mode;     //  + or -
    uint16 dir;
    int16  value;    //  color
    int16  delta;
    int32  skip;
}Grid_Status;


typedef struct tag_effect_grid {
    int start_x;
    int start_y;
    int count_w;
    int count_h;
    Grid_Status *status;
}Effect_Grid_T;


typedef struct SE_grid_handle_tag
{
    int32   handle_tag;           //  const number : " SE "
    SE_EFFECT_FUNC_T *func;

    SE_EFFECT_SURFACE_T  surface;
    Effect_Grid_T  grid;
} SE_GRID_HANDLE_T;


/**---------------------------------------------------------------------------*
 **                         GLobal  Var.                                     **
 **---------------------------------------------------------------------------*/
extern SE_EFFECT_FUNC_T se_effect_grid_func;


#ifdef __cplusplus
    }
#endif

#endif



