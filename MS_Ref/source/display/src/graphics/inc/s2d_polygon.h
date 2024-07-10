#ifndef __S2D_POLYGON_H__
#define __S2D_POLYGON_H__

#include "s2d.h"
#include "s2d_rotate.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif
        
typedef struct Alpha_Point_t {
	uint32 alpha;												//alpha值 0-255
    int16	x;
	int16	y;
}Alpha_Point_T;
typedef struct GDI_POLYGON_CB_t {
	void (*cb_p) ( int16 xStart, int16 y , int16 len, void *data_ptr, Alpha_Point_T *add_ptr, int32 len_alpha);
	void *data_ptr;
}GDI_POLYGON_CB;

typedef struct GDI_POLYGON_ALPHA_CB_t {
	void (*cb_p) ( Alpha_Point_T point1,Alpha_Point_T point2, void *data_ptr);
	void *data_ptr;
}GDI_POLYGON_ALPHA_CB;

PUBLIC BOOLEAN Gdi_Draw_Polygon_Interface(
					IN BOOLEAN IsUseAlphaEdge,					//边缘柔化
					IN int32 vertex_total_num,					//顶点个数 
					IN FIX16_POINT_T	*sVertex_ptr,				//顶点数据表指针
					OUT Alpha_Point_T *Point_Alpha_ptr,			//边缘柔化数据信息指针
					OUT int32 *iAlpha_Num_ptr,					//边缘柔化数据个数
					OUT FIX16_POINT_T *Point_Add_ptr,				//扫描线起始点信息指针
					OUT int32 *iData_Num_ptr					//扫描线起始点信息个数
					);
/*****************************************************************/
// 	Description : draw a polygon
//	Global resource dependence : 
//   Author:Tim Xia
//	Note:
//	
/*****************************************************************/
PUBLIC BOOLEAN Gdi_Draw_Polygon_Interface_se(
					IN BOOLEAN IsUseAlphaEdge,					//边缘柔化
					IN int32 vertex_total_num,					//顶点个数 
					IN FIX16_POINT_T	*sVertex_ptr,				//顶点数据表指针
					OUT Alpha_Point_T *Point_Alpha_ptr,			//边缘柔化数据信息指针
					OUT int32 *iAlpha_Num_ptr,					//边缘柔化数据个数
					OUT FIX16_POINT_T *Point_Add_ptr,				//扫描线起始点信息指针
					OUT int32 *iData_Num_ptr					//扫描线起始点信息个数
					);
#ifdef __cplusplus
    }
#endif
#endif
