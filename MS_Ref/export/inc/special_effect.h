/******************************************************************************
 ** File Name:    special_effect.h                                             *
 ** Author:       Allen.Mao                                                   *
 ** DATE:         06/12/2010                                                  *
 ** Copyright:    2010 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/12/2010    Allen.Mao       Create.                                     *
 ******************************************************************************/

#ifndef _SPECIAL_EFFECT_H_
#define _SPECIAL_EFFECT_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "img_ref_types.h"

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

#define SE_HANDLE uint32 

typedef enum
{
    SE_ID_INVALID = 0,
    SE_ID_RIPPLE,
    SE_ID_FIRE,
    SE_ID_VISUAL,
    SE_ID_PAGE_TURN,
    SE_ID_ROLL,
    SE_ID_SOLID,
    SE_ID_GRID,    
    SE_ID_LEAF,
    SE_ID_COLLISION,
    SE_ID_CRT,
	SE_ID_CURTAIN,
    SE_ID_NUM_MAX 
}SE_ID_E;

typedef enum
{ 
    SE_RESULT_SUCCESS = 0,
    SE_RESULT_INVALID_PARAM,          //  invalid parameter
    SE_RESULT_INVALID_HANDLE,         //  invalid handle
    SE_RESULT_NO_ENOUGH_MEMORY,       //  have not enough memory
    SE_RESULT_UNKNOW_ERR,             //  not known reason for the effect 
}SE_RESULT_E;

//ripple
typedef enum
{
    SE_RIPPLE_MODE_DOWN = 0,          //  when press down the TP
    SE_RIPPLE_MODE_UP,                //
    SE_RIPPLE_MODE_MOVE,              //  when draging
}SE_RIPPLE_MODE_E;

#define SE_RIPPLE_NORMAL_STONES     0 //  normal stones:  golobal property
#define SE_RIPPLE_CUSTOMIZED_STONES 1 //  customized stones: evey one stone has their property

#define SE_RIPPLE_IS_NOT_CALM       0 //  ripple is not calm yet now : 
#define SE_RIPPLE_IS_CALM           1 //  ripple is calm completely

//fire
#define SE_FIRE_IS_NOT_FADING_OUT   0 //  fire will not be fading out, default
#define SE_FIRE_IS_FADING_OUT       1 //  fire will fading out

#define SE_FIRE_IS_NOT_GO_OUT       0 //  fire is not go out now
#define SE_FIRE_IS_GO_OUT           1 //  fire is go out completely


// visual

// CRT
#define SE_CRT_IS_NOT_OVER          0 //  crt is not over
#define SE_CRT_HORIZONTAL_OVER      1 //  crt is over
#define SE_CRT_VERTICAL_OVER        2 //  crt is over

/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/
//  IMGREF_DATA_FORMAT_E defined in  MS_Code\MS_Ref\export\inc
//  as follows:
//      typedef enum
//      {
//           IMGREF_FORMAT_RGB565 = 0,
//           IMGREF_FORMAT_RGB888,
//           IMGREF_FORMAT_ARGB888,
//           IMGREF_FORMAT_ARGB565,
//           IMGREF_FORMAT_ARGB555,
//           IMGREF_FORMAT_COMPRESSED_ARGB    ,
//
//           IMGREF_FORMAT_YUV422,
//           IMGREF_FORMAT_YUV420,
//           IMGREF_FORMAT_YUV400
//      }IMGREF_DATA_FORMAT_E;

// for all
typedef struct SE_effect_point_tag
{
    uint16 x;
    uint16 y;
}SE_EFFECT_POINT_T;

// ripple
typedef struct SE_ripple_init_param_tag
{
    uint16 width;                  //  width,
    uint16 height;                 //  height

    IMGREF_DATA_FORMAT_E  target_data_format;  
    void    *target_pixels;        //  target buffer pointer 
    IMGREF_DATA_FORMAT_E  src_data_format;      
    void    *src_pixels ;   
    
    uint16 stone_size;             //  default 6,   range: [2,20] 
    uint16 stone_weight;           //  default 128  range: (0,255)
    
    uint16 fade_out_speed;         //  default 4  (means 1 / 2^4),   range: (2,5]
    uint16 ripple_offset;          //  default 2  [1,4]
}SE_RIPPLE_INIT_PARAM_T;


typedef struct SE_ripple_stone_prop_tag
{
    uint16 x;
    uint16 y;
    uint16 stone_size;             //  default 6,   range: [2,20] 
    uint16 stone_weight;           //  default 128  range: (0,255)
} SE_RIPPLE_STONE_PROP_T;

typedef struct SE_ripple_point_prop_tag
{
    uint32                 mode;   //  input mode: TP_MOVE, TP_DOWN, 
    SE_RIPPLE_STONE_PROP_T stone;  //  only when if input_param.type == 
                                   //       SE_RIPPLE_CUSTOMIZED_STONES
}SE_RIPPLE_POINT_PROP_T;


typedef struct SE_ripple_render_input_param_tag
{                             
    uint16  stone_type;            //  SE_RIPPLE_NORMAL_STONES or SE_RIPPLE_CUSTOMIZED_STONES
    uint16  point_num;             //  points number
    SE_RIPPLE_POINT_PROP_T *points;
 
    uint16  skip_frame_num;        //  must skip some frames , if 0, will not skip          
 }SE_RIPPLE_RENDER_INPUT_PARAM_T;

typedef struct SE_ripple_output_param_tag
{
    uint32        is_calm;         //  is calm: SE_RIPPLE_IS_NOT_CALM or SE_RIPPLE_IS_CALM
    IMGREF_RECT_T dirty_rect;      //  dirty_rect    
} SE_RIPPLE_RENDER_OUTPUT_PARAM_T; 

// fire 
typedef struct SE_fire_init_param_tag
{
    uint16 width;                  //  width
    uint16 height;                 //  height

    IMGREF_DATA_FORMAT_E  target_data_format;    //   
    void   *target_pixels;         //  target buffer pointer   
    
    uint16 fade_out_speed;         //  0, reserved
    uint16 hot_spot_num;           //  36
    uint32 fire_move_distance;     //  default 20, the max distance a hot spot can move
}SE_FIRE_INIT_PARAM_T;

typedef struct SE_fire_render_input_param_tag
{
    uint16 fire_type;              //  fire type, [0,8] 
    uint16 is_fading_out;          //  the fire is faing out: 

    uint16 skip_frame_num;         //  must skip some frames , if 0, will not skip      
                                   //  SE_FIRE_IS_NOT_FADING_OUT or SE_FIRE_IS_FADING_OUT
}SE_FIRE_RENDER_INPUT_PARAM_T;

typedef struct SE_fire_output_param_tag
{
    uint32        is_go_out;       //  is over  : SE_FIRE_IS_NOT_GO_OUT or SE_FIRE_IS_GO_OUT
    IMGREF_RECT_T dirty_rect;      //  dirty_rect    
 }SE_FIRE_RENDER_OUTPUT_PARAM_T;


// visual
typedef struct SE_visual_init_param_tag
{
    uint16 width;                  //  buffer' width
    uint16 height;                 //  buffer'height 
    
    IMGREF_DATA_FORMAT_E  target_data_format;    //  IMGREF_FORMAT_ARGB888 
    void   *target_pixels;         //  target buffer pointer 
}SE_VISUAL_INIT_PARAM_T;

#if 0
typedef struct SE_visual_render_param_tag
{

    
 }SE_VISUAL_RENDER_INPUT_PARAM_T;
#endif

typedef struct SE_visual_output_param_tag
{
    IMGREF_RECT_T dirty_rect;      //  dirty_rect
    
 }SE_VISUAL_RENDER_OUTPUT_PARAM_T;


// snow
typedef struct SE_snow_init_param_tag
{
    uint16 width;                  //  buffer' width
    uint16 height;                 //  buffer'height 
    
    IMGREF_DATA_FORMAT_E  target_data_format;    //  IMGREF_FORMAT_ARGB888 
    void   *target_pixels;         //  target buffer pointer          
}SE_SNOW_INIT_PARAM_T;

#if 0
typedef struct SE_snow_render_input_param_tag
{

    
}SE_SNOW_RENDER_INPUT_PARAM_T;
#endif

typedef struct SE_snow_output_param_tag
{
    IMGREF_RECT_T dirty_rect;      //  dirty_rect    
 }SE_SNOW_RENDER_OUTPUT_PARAM_T;


// grid
typedef struct SE_grid_init_param_tag
{
    uint16 width;                  //  buffer' width
    uint16 height;                 //  buffer'height 
    
    IMGREF_DATA_FORMAT_E  target_data_format;    //  only IMGREF_FORMAT_ARGB888 
    void   *target_pixels;         //  target buffer pointer          
}SE_GRID_INIT_PARAM_T;

typedef enum SE_grid_mode_tag
{
    SE_GRID_MODE_DOWN = 0,          //  when press down the TP
    SE_GRID_MODE_UP,                //
    SE_GRID_MODE_MOVE,              //  when draging
}SE_GRID_MODE_E;

typedef struct grid_point_prop_tag
{
    uint32            mode;   //  input mode: TP_MOVE, TP_DOWN, 
    SE_EFFECT_POINT_T pos;  //
}SE_GRID_POINT_PROP_T;

typedef struct SE_grid_render_input_param_tag
{                             
    uint16  point_num;             //  points number
    uint16  reserved ;             //  alignment, for future
    SE_GRID_POINT_PROP_T *points;
    void   *target_pixels;         //  target buffer pointer          
 }SE_GRID_RENDER_INPUT_PARAM_T;

 // leaf
typedef struct SE_leaf_texture_tag
{
    IMGREF_DATA_FORMAT_E  format;  // ARGB
    int16   leaf_width;
    int16   leaf_height;
    void    *leaf_addr;  
}SE_LEAF_TEXTURE_T;

typedef struct SE_leaf_init_param_tag
{
    uint16 width;                  //  width,
    uint16 height;                 //  height

    IMGREF_DATA_FORMAT_E  target_data_format;  
    void    *target_pixels;        //  target buffer pointer 
    IMGREF_DATA_FORMAT_E  src_data_format;      
    void    *src_pixels ;   

    int32   leaf_kind_num;  
    SE_LEAF_TEXTURE_T *texture;

    int32   leaves_num;      
}SE_LEAF_INIT_PARAM_T;

typedef enum SE_leaf_mode_tag
{
    SE_LEAF_MODE_DOWN = 0,          //  when press down the TP
    SE_LEAF_MODE_UP,                //
    SE_LEAF_MODE_MOVE,              //  when draging
}SE_LEAF_MODE_E;

typedef struct leaf_point_prop_tag
{
    uint32            mode;   //  input mode: TP_MOVE, TP_DOWN, 
    SE_EFFECT_POINT_T pos;  //  only when if input_param.type == 
                                   //       SE_RIPPLE_CUSTOMIZED_STONES
}SE_LEAF_POINT_PROP_T;

typedef struct SE_leaf_render_input_param_tag
{                             
    uint16  point_num;             //  points number
    uint16  reserved ;             //  alignment, for future
    SE_LEAF_POINT_PROP_T *points;
    void   *target_pixels;         //  target buffer pointer 
    void   *bg_pixels ;          
 }SE_LEAF_RENDER_INPUT_PARAM_T;

//////////////////////////////////////////////////////////////////////////////
// coloision

//////////////////////////////////////////////////////////////////////////////
// coloision
 
#define COLL_VERTEX_POINTS 20
#define COLL_ICON_NUM 50
#define COLL_ICON_CHANGE_NUM 2

typedef enum 
{
    SE_COLLISION_IOCTRL_UPDATE = 0,
    SE_COLLISION_IOCTRL_CLICK,
    SE_COLLISION_IOCTRL_CHG_GRAVITY    
     
}SE_COLLISION_IOCTRL_E;

typedef enum
{
    SE_COLLISION_MODE_EGG=0,
    SE_COLLISION_MODE_MENU,
    SE_COLLISION_MODE_BALLOON,
    SE_COLLISION_MODE_NEWBALLOON
}SE_COLLISION_MODE_E;

typedef enum 
{
    SE_CLICK_LEFT = 0,
    SE_CLICK_RIGHT
     
}SE_CLICK_E;

typedef enum 
{
    SE_CLICK_DOWN = 0,
    SE_CLICK_UP
     
}SE_CLICK_STATE_E;

typedef enum
{
    SE_ACTION_ADD=0,
    SE_ACTION_DRAG
    
}SE_ACTION_E;

typedef enum
{
    SE_ICON_TYPE_POLYON=0,
    SE_ICON_TYPE_CIRCLE
    
}SE_ICON_TYPE_E;

typedef enum
{
    SE_GRAVITY_DOWN,
    SE_GRAVITY_UP,
    SE_GRAVITY_LEFT,
    SE_GRAVITY_RIGHT    

}SE_GRAVITY_DIRECTION_E;


typedef struct se_icon_polycon_tag
{

    IMGREF_POINT_T vertex_point_ptr[COLL_VERTEX_POINTS];
    uint32 vertex_point_num; 
    

}SE_ICON_POLYCON_T;


typedef struct se_icon_circle_tag
{
      uint32 r;

}SE_ICON_CIRCLE_T;



typedef struct se_icon_img_tag
{
    uint32 icon_img_addr;                    //the pointer to every icon's image                   
    uint32 icon_img_buf_size;
    uint32 app_param_add;
    uint16 icon_num;                         //the number of icon in the given icon image
    uint16 icon_width;                       //every icon's width
    uint16 icon_height;    
    union
    {
        SE_ICON_POLYCON_T   polycon;
        SE_ICON_CIRCLE_T circle;
    }img;
    

    IMGREF_DATA_FORMAT_E data_format;    //rgb565 /argb888 support
    SE_ICON_TYPE_E icon_type;
    
}SE_ICON_IMG_T;

/*typedef struct se_icon_img_tag
{
    uint32 icon_img_addr[COLL_ICON_CHANGE_NUM];//the pointer to every icon's image
    uint8  icon_can_chg_num;                 //the number of icon can change in the collision
    uint32 icon_img_buf_size;
    uint16 icon_width;                       //every icon's width
    uint16 icon_height;
    
    union
    {
        SE_ICON_POLYCON_T   polycon;
        SE_ICON_CIRCLE_T circle;
    }img;
    

    IMGREF_DATA_FORMAT_E data_format;    //rgb565 /argb888 support
    SE_ICON_TYPE_E icon_type;
    
}SE_ICON_IMG_T;
*/

//Note: due to the struct is big, locating it at stack is not a good idea
typedef struct se_collision_init_tag
{
    uint16 bg_width;
    uint16 bg_height;
    uint32 bg_img_addr;
    uint32 bg_img_size;

    SE_ICON_IMG_T icon_img[COLL_ICON_NUM];
    IMGREF_POINT_T centre_pos[COLL_ICON_NUM];
     uint32 icon_img_num;
     
    uint32 icon_max_num;//the maximum number of collision balls.
    
    IMGREF_DATA_FORMAT_E bg_data_format;
    
    SE_COLLISION_MODE_E collision_mode;

}SE_COLLISION_INIT_T;

//Note: due to the struct is big, locating it at stack is not a good idea
typedef struct se_collision_update_tag
{
    uint32 bg_img_addr;
    uint32 bg_img_size;
    
    SE_ICON_IMG_T icon_img_ptr[COLL_ICON_NUM];
    IMGREF_POINT_T centre_pos[COLL_ICON_NUM];
    uint32 icon_img_num;    

    IMGREF_DATA_FORMAT_E bg_data_format;

}SE_COLLISION_UPDATE_IN_T;

typedef struct se_collision_update_out_tag
{
    uint16 bg_width;
    uint16 bg_height;
    uint32 bg_img_addr;

}SE_COLLISION_UPDATE_OUT_T;

typedef struct se_collision_click_in_tag
{
    IMGREF_POINT_T centre_pos;
    SE_CLICK_E click;
    SE_CLICK_STATE_E click_state;
    SE_ACTION_E click_action;

    SE_ICON_IMG_T icon_img;

}SE_COLLISION_CLICK_IN_T;

typedef struct se_collision_click_out_tag
{
    uint32 param;
    
}SE_COLLISION_CLICK_OUT_T;

typedef struct se_collision_chg_gravity_in_tag
{
      SE_GRAVITY_DIRECTION_E direction;

}SE_COLLISION_CHG_GRAVITY_IN_T;

typedef struct se_collision_gravity_out_tag
{
    uint32 param;
    
}SE_COLLISION_CHG_GRAVITY_OUT_T;

typedef struct se_collision_render_in_tag
{
    SE_COLLISION_IOCTRL_E type;
    union
    {
         SE_COLLISION_UPDATE_IN_T update_param;
         SE_COLLISION_CLICK_IN_T click_param;
         SE_COLLISION_CHG_GRAVITY_IN_T chg_gravity;

    }param;

}SE_COLLISION_RENDER_IN_T;



typedef struct se_collision_render_out_tag
{
    union
    {
         SE_COLLISION_UPDATE_OUT_T update_out_param;
         SE_COLLISION_CLICK_OUT_T click_out_param;
         SE_COLLISION_CHG_GRAVITY_OUT_T chg_gravity_out_param;

    }param;

}SE_COLLISION_RENDER_OUT_T;



//////////////////////////////////////////////////////////////////////////////
//parameter for page turn
typedef enum
{
    SE_PAGE_TURN_LEFT_TOP = 0,                    //turn from the left top
    SE_PAGE_TURN_RIGHT_TOP,                    //turn from the left right
    SE_PAGE_TURN_LEFT_BOTTOM,                    //turn from the left bottom
    SE_PAGE_TURN_RIGHT_BOTTOM                    //turn from the right bottom
}SE_PAGE_TURN_MODE_E;

typedef enum
{
	SE_PAGE_TURN_PATTERN_TEXT = 0,						//display text on backface 
	SE_PAGE_TURN_PATTERN_COLOR							//display color on backface
}SE_PAGE_TURN_BACKFACE_PATTERN_E;

typedef struct SE_page_turn_init_param_tag
{    
    uint16                    page_width;                //width of page
    uint16                    page_height;            //height of page
    void                    *cur_page_ptr;            //data of current page
    void                    *next_page_ptr;            //data of next page
    IMGREF_DATA_FORMAT_E    format;                    //format of page data, only support RGB565 format
    uint32                    backface_color;            //backface color of the page	
	SE_PAGE_TURN_BACKFACE_PATTERN_E		backface_pattern;	//pattern of backface
	
    SE_PAGE_TURN_MODE_E        mode;                    //mode of turn off
}SE_PAGE_TURN_INIT_PARAM_T;

typedef struct SE_page_turn_page_render_in_tag
{
    int16                    frame_index;            //set to -1 if use hot_spot     
    int16                    hot_spot;                // [1, page_width - 1]
    void                    *target_buf_ptr;
    uint32                    target_buf_size;
}SE_PAGE_TURN_RENDER_IN_PARAM_T;

typedef struct SE_page_turn_page_render_out_tag
{
    IMGREF_RECT_T             dirty_rect;      //  dirty_rect
    BOOLEAN                    is_finish;         // whether the animation is finished, only used if the frame_index is valid
}SE_PAGE_TURN_RENDER_OUT_PARAM_T;

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//parameter for roll effect
typedef struct
{
    int16   top;            //the top-left coordinate
    int16   left;
    uint16  row_inx;
    uint16  clmn_inx;
    uint32*  img_buf; 
    
}ROLL_ICON_IMG;

typedef struct
{
    uint16 row_num;
    uint16 clmn_num;
    uint16 icon_h;
    uint16 icon_w;
    ROLL_ICON_IMG* icon_ptr;
    
}ROLL_ICONS_INFO;

typedef struct SE_roll_init_param_tag
{    
    IMGREF_DATA_FORMAT_E    format;            //only support ARGB888 foramt
    
}SE_ROLL_INIT_PARAM_T;

typedef struct SE_roll_page_render_in_tag
{    
    IMGREF_SIZE_T            src_size;            //source image size    
    IMGREF_RECT_T            src_rect;            //reserved

    uint32                    *src_buf_ptr;        //source image buffer, if it is NULL, use strip image    
    
    uint32                    **src_slice_pptr;    //valid if src_buf_ptr is NULL. Divide the source image to serveral slice. 
    uint32                    slice_num;           //strip numbers
    uint32                    slice_height;        //size of each slice
	ROLL_ICONS_INFO           icons_info;     //valid if src_buf_ptr and src_slice_pptr are NULL. Divide the source image to serveral icon images
	                                               //icon image is saved according to column
	                                               
    BOOLEAN                    use_default_param;     //whether use default param? if TURE, the eye and angle parameters are invalid
    IMGREF_POINT_3D_T        eye;                //position of eye
    int32                    angle[2];            //90 - 180    
    
    int32                    src_hot_line[2];    //(src_hot_line[1] - src_hot_line[0]) must equal (target_hot_line[1] - target_hot_line[0])
    int32                    target_hot_line[2];
    
    uint32                    *target_buf_ptr;    //target buffer pointer
    IMGREF_SIZE_T            target_size;        //target image size
    IMGREF_RECT_T            target_rect;        //rectangle to receive output image
}SE_ROLL_RENDER_IN_PARAM_T;

typedef struct SE_roll_page_render_out_tag
{
    IMGREF_RECT_T             dirty_rect;      //  dirty_rect
    BOOLEAN                    is_finish;         // whether the animation is finished, only used if the frame_index is valid
}SE_ROLL_RENDER_OUT_PARAM_T;

//////////////////////////////////////////////////////////////////////////////
//parameter for solid figure, such as sphere, cylinder, etc.
#define SE_SOLID_PAGE_MIN_COLUMN_NUM			(2)
#define SE_SOLID_PAGE_MIN_ROW_NUM				(2)
#define SE_SOLID_PAGE_MAX_COLUMN_NUM            (5)
#define SE_SOLID_PAGE_MAX_ROW_NUM                (5)
//the number of icon per page
#define SE_SOLID_PAGE_MAX_ICON_NUM                (SE_SOLID_PAGE_MAX_COLUMN_NUM * SE_SOLID_PAGE_MAX_ROW_NUM)

typedef enum SE_solid_op_tag
{
    SE_SOLID_OP_RENDER = 0,
    SE_SOLID_OP_GET_DEFAULT_PARAM
}SE_SOLID_OP_T;


typedef enum SE_solid_type_tag
{
    SE_SOLID_SPHERE = 0,
    SE_SOLID_CYLINDER,
}SE_SOLID_TYPE_E;

typedef enum SE_solid_quality_tag
{
    SE_SOLID_QUALITY_LOW = 0,                //lower image quality and higher speed
    SE_SOLID_QUALITY_MIDDLE,                //moderate image quality and speed
    SE_SOLID_QUALITY_HIGH                    //higher image quality and lower speed
}SE_SOLID_QUALITY_E;

//parameter of sphere
typedef struct SE_solid_sphere_param_tag
{
    uint32                radius;                    //radius of the sphere
    IMGREF_POINT_T        center;                    //center of cylinder on target image coordinate
}SE_SOLID_SPHERE_PARAM_T;

//parameter of cylinder
typedef struct SE_solid_cylinder_param_tag
{
    uint32                radius;                    //radius of the cylinder
    uint32                height;                    //height of the cylinder
    IMGREF_POINT_T        center;                    //center of sphere on target image coordinate
}SE_SOLID_CYLINDER_PARAM_T;

typedef struct SE_solid_rot_angle_tag
{
    int32        x;                            //rotate angle about x axis
    int32        y;                            //rotate angle about y axis
    int32        z;                            //rotate angle about z axis    
}SE_SOLID_ROT_ANGLE_T;

typedef struct SE_solid_init_param_tag
{    
    SE_SOLID_TYPE_E            type;                        //type of solid figure
    
    IMGREF_SIZE_T            target_size;                //target image size 
    IMGREF_DATA_FORMAT_E    target_format;                //ARGB888 format only
    IMGREF_RECT_T            target_rect;                //target rect to display output image

    IMGREF_SIZE_T            icon_size;                    //icon size
    IMGREF_DATA_FORMAT_E    icon_format;                //ARGB888 format only    

    uint16                    icon_columns;                //number of columns of one page, range from [1-5]    
    uint16                    icon_rows;                //number of rows of one page,,range from [1-5]
}SE_SOLID_INIT_PARAM_T;

typedef struct SE_solid_page_param_tag
{
    IMGREF_BUF_T            target_buf;                        //output buffer of the page
    uint32                    *icon[SE_SOLID_PAGE_MAX_ICON_NUM];    //array of icon address
    uint32                    intensity;                        //intensity of output image, 1-7
    SE_SOLID_QUALITY_E        quality;                        //quality of output image
}SE_SOLID_PAGE_T;

typedef struct SE_solid_adv_param_tag
{
    IMGREF_POINT_3D_T        eye_pos;                        //position of eye in global coordinate
    IMGREF_POINT_3D_T        solid_pos;                        //position of solid in global coordinate
}SE_SOLID_ADVANCED_PARAM_T;

typedef struct SE_solid_render_in_tag
{        
    SE_SOLID_PAGE_T                    backside;
    SE_SOLID_PAGE_T                    frontage;

    union
    {
        SE_SOLID_SPHERE_PARAM_T        sphere;
        SE_SOLID_CYLINDER_PARAM_T    cylinder;
    }solid_param;

    SE_SOLID_ADVANCED_PARAM_T        *adv_param_ptr;            // set to NULL to use default advanced parameters

    SE_SOLID_ROT_ANGLE_T            rot_angle;                //rotate angle

    SE_SOLID_OP_T                    operation;                //operation
}SE_SOLID_RENDER_IN_PARAM_T;

typedef struct SE_solid_render_out_tag
{
    IMGREF_RECT_T             dirty_rect;      //  dirty_rect
}SE_SOLID_RENDER_OUT_PARAM_T;

//////////////////////////////////////////////////////////////////////////////
// CRT
typedef struct SE_crt_init_param_tag
{
    uint16 width;                  //  width,
    uint16 height;                 //  height

    IMGREF_DATA_FORMAT_E  target_data_format;  
    void    *target_pixels;        //  target buffer pointer 
    IMGREF_DATA_FORMAT_E  src_data_format;      
    void    *src_pixels ;   

}SE_CRT_INIT_PARAM_T; 
 
typedef struct SE_crt_output_param_tag
{
    uint32        is_over;         //  is over: SE_CRT_IS_NOT_OVER or SE_CRT_IS_OVER
    IMGREF_RECT_T dirty_rect;      //  dirty_rect
} SE_CRT_RENDER_OUTPUT_PARAM_T; 
//////////////////////////////////////////////////////////////////////////////

//curtain

typedef enum
{
    SE_CURTAIN_MODE_ONE = 0x00,
    SE_CURTAIN_MODE_TOW,
    SE_CURTAIN_MODE_MAX
    
}SE_CURTAIN_MODE_E;

typedef enum
{
    SE_GRAPH_CIRCLE = 0x00,
    SE_GRAPH_MAX    
    
}SE_GRAPH_TYPE_E;

typedef struct se_circle_tag
{
    IMGREF_POINT_T cent_pt;
    uint16 radius;
    
}SE_CIRCLE_T, SE_CIRCLE_T_PTR;

typedef struct se_curtain_update_param_tag
{
    IMGREF_RECT_T dis_rect;

    union
    {
        SE_CIRCLE_T circle;
        
    }update;
    
    SE_GRAPH_TYPE_E graph_type;    
    
}SE_CURTAIN_UPDATE_PARM_T, *SE_CURTAIN_UPDATE_PARM_T_PTR;


typedef struct SE_curtain_init_param_tag
{
    uint16 img_width;
    uint16 img_height;
    int16 center_pt_x;
    int16 center_pt_y;
    uint16 frame_num; 
    IMGREF_RECT_T disp_rect;    
    SE_CURTAIN_MODE_E act_mode;  
    IMGREF_DATA_FORMAT_E data_type;
    SE_GRAPH_TYPE_E graph_type; 
    
}SE_CURTAIN_INIT_PARAM_T, *SE_CURTAIN_INIT_PARAM_T_PTR;

typedef struct SE_curtain_render_in_tag
{
    uint16 index;
    uint16 img_width;
    uint16 img_height;    
    IMGREF_CHN_T src_chn;    
    IMGREF_CHN_T dst_chn; 
    //SE_CURTAIN_UPDATE_PARM_T update_param; 
    
}SE_CURTAIN_RENDER_IN_T, *SE_CURTAIN_RENDER_IN_T_PTR;

typedef struct SE_curtain_render_out_tag
{
    void* param;
    
}SE_CURTAIN_RENDER_OUT_T, *SE_CURTAIN_RENDER_OUT_T_PTR;


/******************************************************************************
// Description: create effect (contains allocate memeory)
// Input:        effect_id  : SE_ID_E 
//              int_param  : void *
// Output:      None
// Return:        EFFECT_HANDLE
// Note:
******************************************************************************/ 
PUBLIC SE_HANDLE SE_Create(SE_ID_E effect_id, 
                           void    *int_param);

/******************************************************************************
//Description: calculate current state and then render it
// Input:        effect_handle: SE_HANDLE  
//              render_param  : void*
//              out_param     : void*
// Output:      None
// Return:        EFFECT_STATUS     
// Note:
******************************************************************************/ 
PUBLIC SE_RESULT_E SE_Render(SE_HANDLE effect_handle, 
                             void      *render_param,
                             void      *out_param);

/******************************************************************************
// Description: Destory the effect
// Input:        effect_handle: SE_HANDLE 
// Output:      
// Return:        None
// Note:
******************************************************************************/ 
PUBLIC SE_RESULT_E  SE_Destory(SE_HANDLE effect_handle);

PUBLIC SE_RESULT_E SE_GetInfo(SE_HANDLE effect_handle, void *info_ptr);


#ifdef __cplusplus
    }
#endif

#endif



