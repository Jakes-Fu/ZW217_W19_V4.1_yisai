/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _CM_INTERFACE_H_
#define _CM_INTERFACE_H_
#include "img_ref_types.h"


#define MAX_ICON_NUM 50
#define MAX_VERTEX_POINTS 20


/**/

typedef enum
{
    CM_ICON_TYPE_POLYON=0,
    CM_ICON_TYPE_CIRCLE
    
}CM_ICON_TYPE_E;

typedef enum
{
    CM_COLLISION_MODE_EGG=0,
    CM_COLLISION_MODE_MENU,
    CM_COLLISION_MODE_BALLOON

}CM_COLLISION_MODE_E;

typedef struct cm_icon_polycon_tag
{
    uint32 vertex_point_num; 
	IMGREF_POINT_T vertex_point_ptr[MAX_VERTEX_POINTS];    

}CM_ICON_POLYCON_T;


typedef struct cm_icon_circle_tag
{
      uint32 r;

}CM_ICON_CIRCLE_T;


typedef struct cm_icon_img_tag
{	
	uint16 icon_width;
	uint16 icon_height;

	uint32 icon_img_addr;
	uint32 icon_img_addr2;
	uint32 icon_img_buf_size;
	uint32 app_param_add;
	IMGREF_POINT_T centre_pos;
	
    union
    {
        CM_ICON_POLYCON_T   polycon;
        CM_ICON_CIRCLE_T circle;
    }img;

    IMGREF_DATA_FORMAT_E data_format;	//rgb565 /argb888 support
    CM_ICON_TYPE_E icon_type;    
	
}CM_ICON_IMG_T;

/*

typedef struct cm_icon_img_tag
{	
	uint16 icon_width;
	uint16 icon_height;

	uint32 icon_img_addr;
	uint32 icon_img_addr2;
	uint32 icon_img_buf_size;
    uint32 vertex_point_num;
    
    IMGREF_POINT_T centre_pos;
    IMGREF_POINT_T vertex_point_ptr[MAX_VERTEX_POINTS];
    IMGREF_DATA_FORMAT_E data_format;	//rgb565 /argb888 support                                                                                                                                              
	
}CM_ICON_IMG_T;*/


typedef struct cm_img_tag
{

    uint16 bg_width;
    uint16 bg_height; 
    uint32 bg_img_addr;
    uint32 bg_img_size;

    CM_ICON_IMG_T icon_img[MAX_ICON_NUM];  
    uint32 icon_img_num;

    IMGREF_DATA_FORMAT_E bg_data_format;
    CM_COLLISION_MODE_E collision_mode;

}CM_IMG_T;

typedef struct cm_update_tag
{
	uint32 bg_img_addr;
	uint32 bg_img_size;	  
  	
  	IMGREF_DATA_FORMAT_E bg_data_format;
	BOOLEAN is_add_icon;
	
	CM_ICON_IMG_T icon_img; 

}CM_UPDATE_T;



uint32 CM_Init(CM_IMG_T *init_param_ptr);
uint32 CM_ClickAdd(CM_ICON_IMG_T *icon_img_ptr,int state, int x, int y,int8 is_user_add);
uint32 CM_ClickChoose(uint32 handle,int state, int x, int y, uint32* param_ptr);
uint32 CM_ClickedMoving(uint32 handle,int state, int x, int y );
uint32 CM_Update(CM_UPDATE_T *update_param_ptr);
uint32 CM_Destroy(void);


#endif
