/******************************************************************************
 ** File Name:      graphics_3d_rotate.h									  *
 ** Author:         Tim.Xia                                                   *
 ** DATE:           2010-11-25                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    functions to draw some graphics                            *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *    
 ** 2010-11-25          Tim.Xia       Create                                  *
 *****************************************************************************/

#ifndef __GRAPHICS_3D_ROTATE_H__
#define __GRAPHICS_3D_ROTATE_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "img_ref_types.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

#define CIRCLE_ANGLE	(3600)
#define SHINING_MAX_RADIUS (256)
#define IN
#define OUT
//#define FIX8_INT(a)	(a>>8)
//ANSON ADD
#define ABS(x) (((x) >= 0)? (x) : (-(x)))
#if 1
typedef signed long int			FIX8;

#define FIX8_INT(a)	((a)>>8)
#define FIX8_SET(a) ((a)<<8)
#define FIX8_CLEAN(a) ((a)>>8)
#define FLOAT_2_FIX8(a) ((int)((a)*256))
#define FIX16_2_FLOAT(a) ((float)(((float)(a))/((float)(65536))))
#else
typedef float			FIX8;
#define FIX8_INT(a)	INT(a)
#define FIX8_SET(a) (a)
#define FIX8_CLEAN(a) (a)

#endif
#define RES_TYPE_3D   OBJECT565_T
#define GRAPHIC3D_COLORKEY 0x07ff

typedef enum
{
	ANTIALIAS_TYPE_NONE = 0,
	ANTIALIAS_TYPE_EDGE_ONLY,
	ANTIALIAS_TYPE_TOTAL,
	ANTIALIAS_TYPE_MAX

}ANTIALIAS_TYPE_E;
typedef struct
{
	ANTIALIAS_TYPE_E anti_type;
}S3D_ALLAXIS_EXP_T;
typedef struct
{
	int16 		x;
	int16		y;	
	int16		z;	
	int16		w;
}IMGREF_3D_FLOAT_P_T;
typedef uint32 COLOR32;
typedef uint16 COLOR16;
typedef int32 FIX16;//haipeng add
typedef signed long int			FIX8;

typedef enum
{
    S3D_DATA_ARGB888,
    S3D_DATA_RGB888,
	S3D_DATA_RGB565,
	S3D_DATA_RGB5515,
	S3D_DATA_BLOCK_COLOR_RGB888,
    S3D_DATA_PAF_RGB888,
    S3D_DATA_PAF_RGB888_NF_R,//pixel replace, so the pixel near to eyes should be draw first
	
} S3D_DATA_TYPE_E;



typedef enum
{
	S3D_ALPHA_8_0,			
	S3D_ALPHA_6_2,			
	S3D_ALPHA_4_4,			
} GRAPH_ALPHA_SET_E;


typedef enum
{
	S3D_RECT_0 = 0,			
	S3D_RECT_90,			
	S3D_RECT_180,			
	S3D_RECT_270,			
	S3D_RECT_TOP,
	S3D_OUTSIDE,			
} S3D_CUBE_POINT_POSITION_E;

typedef struct
{
	int16 		x;
	int16		y;	
	int16		z;	
	int16		w;
}IMGREF_3D_POINT_T;


typedef struct {
	int16    x;
	int16	y;
	int16    z;
	int16   axis;		//0:x   1:Y
}S3D_AXIS_T;




typedef struct{
	void *src_ptr;							
	S3D_DATA_TYPE_E src_type;
	uint32 color;
} S3D_ROTATION_IMG_T;


typedef struct{
	IMGREF_SIZE_T size;
	void *color_ptr;
      S3D_DATA_TYPE_E color_type;
	IMGREF_RECT_T roi;
} S3D_ROTATION_INOUT_IMG_INFO_T;


typedef struct{
	GRAPH_ALPHA_SET_E alpha_set;	
	S3D_ROTATION_IMG_T 	(*s3d_img_ptr)[4];
	uint32  top_color[2];
} S3D_ROTATION_CUBE_SRC_INFO_T;


typedef struct{
	int16 angle;
	BOOLEAN is_axis_use_default;
	BOOLEAN is_eye_use_default;
	S3D_AXIS_T axis;
	IMGREF_3D_POINT_T eye;
	IMGREF_RECT_T  img_size;
} S3D_ROTATION_GEOMETRY_T;

typedef struct{
	uint8* shiningCtrl_ptr;
} S3D_EXPANDPTR_T;

typedef struct Rota_data_t{
	int angle;
	int	axis_x;
	int	axis_y;														
	int	axis_z;
	int	eye_y;
}ROTA_DATA_T;

//haipeng add
typedef struct
{
	FIX16 x;
	FIX16 y;	
	FIX16 z;	
	FIX16 w;
}FIX16_3D_POINT_T;
typedef struct Light_Shining_t {
    FIX16_POINT_T* st_point;
    FIX16_POINT_T* ed_point;
    int32 rds;
    COLOR32 color;
}LIGHT_SHINING_T;

typedef struct Light_Shining_scan_t {
    FIX16_POINT_T* cen_point;
    int32 rds;
    COLOR32 color;
    uint8* Ctrl_ptr;
}LIGHT_SHINING_SCAN_T;

typedef struct Picture_Circular_Joint{
        int32 xDelta_vertex;
        int32 yDelta_vertex;
    }PIC_CIRCULAR_JOINT_T;

typedef struct Color_Buffer_t{
	IMGREF_RECT_T* size;
	void *color_ptr;
      S3D_DATA_TYPE_E color_type;
}COLOR_BUF_T;
typedef struct
{
	int32 x;			 
	int32 y;			 
	int32 w;			 
	int32 h;			 
} T_RECT;

typedef struct POINT_2D_FIX8_t
{
	FIX8 x;
	FIX8 y ;
}POINT_2D_FIX8;

typedef struct
{
	COLOR32* srcARGB_Map;
	IMGREF_SIZE_T up_window;
}OBJECT_T;

typedef struct
{
	COLOR16* srcARGB_Map;
	IMGREF_SIZE_T up_window;
}OBJECT565_T;
typedef struct
    {
    	int32 w;
    	int32 x;
    	int32 y;
    	int32 z;
    }QUATERNIONS_T;//四元素，16bits精度
    typedef struct
    {
    	float w;
    	float x;
    	float y;
    	float z;
    }QUATERNIONS_FLOAT_T;//四元素

    typedef struct POINT_3D_FIX8_t
    {
    	FIX8 x;
    	FIX8 y ;
      FIX8 z ;
    }POINT_3D_FIX8;

    typedef struct POINT_3D_0p3f_t
    {
    	signed long int x;
    	signed long int y ;
      signed long int z ;
    }POINT_3D_0P3F;
    
    typedef struct POINT_3D_Float_t
    {
    	float x;
    	float y ;
      float z ;
    }POINT_3D_FLOAT;
    
typedef struct Edge_t {
    void *nxt;																//链表下一个结构指针，可以为空
    int32 iUpY;																//直线上顶点y坐标
	int32 iDownY;															//直线下顶点y坐标
	int32 isUsed;															//该条信息是否有效,1为有效
	int32 iRealY;															//用于画直线，直线与扫描线交点x坐标
	FIX8  f8_nowX;
	FIX8  f8_dX;
	POINT_2D_FIX8 f8_Down;													//下顶点原图中位置
	POINT_2D_FIX8 f8_Up;													//上顶点原图中位置
	POINT_2D_FIX8 f8_Now;													//当前原图位置
	POINT_2D_FIX8 f8_Dpoint;												//每走1格y，原图中的变化量
    BOOLEAN isSteepLine;														//是否直线斜率大于1
    BOOLEAN isUseUp;	
	//这里提别强调一下，所谓上顶点是指y值比较大的那个，也就是说一定有 iDownY <= iUpY
}Edge_T;

PUBLIC BOOLEAN  S3D_CubeAxisRotation (S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
									S3D_ROTATION_CUBE_SRC_INFO_T *s3d_cube_ptr,
									S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr);


PUBLIC BOOLEAN  S3D_ImageAxisRotation (S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr);

PUBLIC BOOLEAN  S3D_CubeCheckPointPosition (S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
									FIX16_POINT_T *point_ptr,
									S3D_CUBE_POINT_POSITION_E *position_ptr);

PUBLIC BOOLEAN  S3D_CubeAxisRotation_Crystal (S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
									S3D_ROTATION_CUBE_SRC_INFO_T *s3d_cube_ptr,
									S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr);

PUBLIC BOOLEAN S3D_QuadrilateralCheckPoint_Crystal(FIX16_POINT_T *sVertex_ptr,
											FIX16_POINT_T *point_ptr);

PUBLIC BOOLEAN  S3D_ImageModifyBrightness (IMGREF_SIZE_T size,
											uint8 	bright,
											COLOR32	*InData,
											COLOR32	*OutData);

PUBLIC BOOLEAN  S3D_ImageMidAxisRotation (S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr);

PUBLIC	BOOLEAN	 S2D_DrawLine(	FIX16_POINT_T startPoint,
								FIX16_POINT_T endPoint,
								int8	line_width,
								int8	head_type,
								IMGREF_SIZE_T window,
								COLOR32	color,
								COLOR32	*OutData);
PUBLIC int s2d_sin_new(int angle);

PUBLIC  int s2d_cos_new(int angle);
PUBLIC void S2d_rotation_slim(	const IMGREF_SIZE_T window,
						const IMGREF_SIZE_T img,
						IMGREF_POINT_T window_mid,
						IMGREF_POINT_T img_mid,
						int32 angle,
						COLOR32 *in_src_ptr,
						COLOR16 *out_src_ptr,
						int32 z_para,
						FIX16_POINT_T *Point_Add_ptr);				//扫描线起始点信息指针
						
PUBLIC void S2d_rotation_alpha(	const IMGREF_SIZE_T window,
						const IMGREF_SIZE_T img,
						IMGREF_POINT_T window_mid,
						IMGREF_POINT_T img_mid,
						int32 angle,
						COLOR32 *in_src_ptr,
						COLOR32 *out_src_ptr,
						FIX16_POINT_T *Point_Add_ptr);				//扫描线起始点信息指针
						
PUBLIC void S2d_rotation_alpha_ex(	const IMGREF_SIZE_T window,
						const IMGREF_SIZE_T img,
						IMGREF_POINT_T window_mid,
						IMGREF_POINT_T img_mid,
						int32 angle,
						COLOR32 *in_src_ptr,
						COLOR32 *out_src_ptr,
						int32 z_para,
						FIX16_POINT_T *Point_Add_ptr);				//扫描线起始点信息指针					
PUBLIC BOOLEAN S2d_AxisZX_rotation(	const IMGREF_SIZE_T window,
						const IMGREF_SIZE_T img,
						IMGREF_POINT_T window_mid,
						IMGREF_POINT_T img_mid,
						int32 angle,
						FIX16_POINT_T *Point_Add_ptr,//扫描线起始点信息指针
						int32 alpha_para,
						S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
						S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
						S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr);


PUBLIC void Slim_Landscape_Create_Shadow(IMGREF_SIZE_T page_size,
                                        IMGREF_SIZE_T shadow_size,
                                        COLOR32 * pColor_in, 
                                        COLOR32 * tempSrcColor_reverse
                                        );

PUBLIC void s3d_Free_Point_change( IMGREF_3D_POINT_T in_Point ,	//转轴坐标
								 IMGREF_3D_POINT_T sEye,				//视角位置
								 FIX16_POINT_T *Point_ptr		//顶点数据地址
								);

/*****************************************************************************/
// Description : 把565的数据转化成888存储到另一块BUF
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
PUBLIC void S2d_Blend_RGB565ToARGB888(uint16* dst_buf_ptr,
										uint32 *src_buf_ptr,
										uint32 width,
										uint32 height,
										uint8 alpha);
/*****************************************************************************/
// Description : S2d_4_in_one_fast_Out32
// Global resource dependence : 
// Author:anson
// Note:
/*****************************************************************************/
PUBLIC void S2d_4_in_one_fast_Out32(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR32 *out_src_ptr,						//输出buf指针
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr);	//扫描线起始点信息指针
/*****************************************************************************/
// Description : S2d_4_in_one_fast_Out32_Zero_size
// Global resource dependence : 
// Author:anson
// Note:
/*****************************************************************************/

PUBLIC void S2d_4_in_one_fast_Out32_Zero_size(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR32 *out_src_ptr,
						int32 zero_size,//输出buf指针
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr);
PUBLIC void s3d_Free_Point_change_2(FIX16_3D_POINT_T in_Point ,	//转轴坐标
								 FIX16_3D_POINT_T sEye,				//视角位置
								 FIX16_3D_POINT_T *Point_ptr		//顶点数据地址
								);
PUBLIC BOOLEAN  S3D_ImageMidAxis565Rotation_Windmill (S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr,
								uint8 	bright,
								int32 grass_offset );/*for windmill's grass rotation*/
PUBLIC void S2d_Shadow_slim(	const IMGREF_SIZE_T window,
						const IMGREF_SIZE_T img,
						IMGREF_POINT_T window_mid,
						IMGREF_POINT_T img_mid,
						int32 angle,
						COLOR32 *in_src_ptr,
						COLOR16 *out_src_ptr,
						int32 z_para,
						FIX16_POINT_T *Point_Add_ptr);
PUBLIC BOOLEAN  S3D_ImageMidAxisRotation_Ex (S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr,
								int grass_offset
								);
PUBLIC void S2d_4_in_one_fast_565(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR16 *in_src_ptr,						//输入buf指针
						COLOR16 *out_src_ptr,						//输出buf指针
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr,
						int32 zero_size);				//扫描线起始点信息指针
/*****************************************************************************/
// Description : S3D_ImageMidAxis565Rotation
// Global resource dependence : 
// Author:timxia
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  S3D_ImageMidAxis565Rotation (S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr,
								uint8 	bright);
/*****************************************************************************/
// Description : S2d_rotation_Symmetry
// Global resource dependence : 
// Author:anson
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  S2d_rotation_Symmetry(IMGREF_SIZE_T window, 
									COLOR32 * bg_buf, 
									COLOR32 * up_buf,
									IMGREF_RECT_T icon_rect,
									int angle, 
									int x_zero,
									FIX16_POINT_T *Point_Add_ptr);
/*****************************************************************************/
// Description : S2d_4_in_one_fast
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
PUBLIC void S2d_4_in_one_fast(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR16 *out_src_ptr,						//输出buf指针
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr);				//扫描线起始点信息指针

/*****************************************************************************/
// Description : S3D_ImageMidAxis565Rotation+光照效果
// Global resource dependence : 
// Author: Benny
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  S3D_ImageMidAxis565Rotation_shining(S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr,
								uint8 	bright,
								void* shiningPtr);
/*****************************************************************************/
// Description : S2d_4_in_one_fast+光照效果
// Global resource dependence : 
// Author: Benny
// Note:
/*****************************************************************************/
PUBLIC void S2d_4_in_one_fast_shining(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR16 *out_src_ptr,						//输出buf指针
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						void* shiningPtr,
						FIX16_POINT_T *Point_Add_ptr,
						void* expand_ptr);	


PUBLIC void S2d_4_in_one_fast_PAF888_shining(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR32 *out_src_ptr,						//输出buf指针
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						void* shiningPtr,
						FIX16_POINT_T *Point_Add_ptr,
						void* expand_ptr);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description : clean a block of PAF888 memory
//					0 = PA888
//  Global resource dependence : 
//  Author: Tim.Xia
//  Note: 6530 use only,PAF888 is different from PA888. Do not
//			use as the PA888.
/*****************************************************************************/
void Clean_PAF888_Buf(IMGREF_SIZE_T size,
					 uint32 *OutBuf);



/*****************************************************************************/
//  Description : transform ARGB888 memory to PAF888 memory
//					ARGB888 = PA888
//  Global resource dependence : 
//  Author: Tim.Xia
//  Note: 6530 use only,PAF888 is different from PA888. Do not
//			use as the PA888.
/*****************************************************************************/
void ARGB888_2_PAF888(IMGREF_SIZE_T size,
					 uint32 *InBuf,
					 uint32 *OutBuf);


/*****************************************************************************/
//  Description : Blend PAF888 memory to PAF888 memory
//					PA888 + PA888 = PA888
//  Global resource dependence : 
//  Author: Tim.Xia
//  Note: 1.6530 use only,PAF888 is different from PA888. Do not
//			use as the PA888.
//		  2.InBuf is the up layer
/*****************************************************************************/
void Blending_PAF888_2_PAF888(IMGREF_SIZE_T size,
					 uint32 *InBuf,
					 uint32 *OutBuf);


PUBLIC	BOOLEAN	S2D_Blending_PAF888(
							 IMGREF_SIZE_T	bg_window,
							 COLOR32	*bg_buf,
							 IMGREF_SIZE_T	up_window,
							 FIX16_POINT_T	offset,
							 COLOR32	*up_buf,
							 COLOR32	*out_buf);




/*****************************************************************************/
//  Description : Blend PAF888 memory to RGB888 memory
//					PA888 + RGB888 = RGB888
//  Global resource dependence : 
//  Author: Tim.Xia
//  Note: 1.6530 use only,PAF888 is different from PA888. Do not
//			use as the PA888.
//		  2.InBuf is the up layer
//		  3.OutBuf should be RGB888
/*****************************************************************************/
void Blending_PAF888_2_RGB888(IMGREF_SIZE_T size,
					 uint32 *InBuf,
					 uint32 *OutBuf);


void Blending_PAF888_2_RGB565(IMGREF_SIZE_T size,
					 uint32 *InBuf,
					 uint16 *OutBuf);



/*****************************************************************************/
//  Description : Blend ARGB888 memory to PAF888 memory
//					ARGB888 + PA888 = PA888
//  Global resource dependence : 
//  Author: Tim.Xia
//  Note: 1.6530 use only,PAF888 is different from PA888. Do not
//			use as the PA888.
//		  2.InBuf is the up layer,and it is the ARGB888 format
/*****************************************************************************/
void Blending_ARBG888_2_PAF888(IMGREF_SIZE_T size,
					 uint32 *InBuf,
					 uint32 *OutBuf);


void RBG565_2_ARGB888(IMGREF_SIZE_T size,
							 uint8 alpha,
							 uint16 *InBuf,
							 uint32 *OutBuf);
/*****************************************************************************/
// Description : S2d_rotation_slim_with_z_size
// Global resource dependence : 
// Author:anson
// Note:
/*****************************************************************************/
PUBLIC void S2d_rotation_slim_with_z_size(	const IMGREF_SIZE_T window,
						const IMGREF_SIZE_T img,
						IMGREF_POINT_T window_mid,
						IMGREF_POINT_T img_mid,
						int32 angle,
						COLOR32 *in_src_ptr,
						COLOR16 *out_src_ptr,
						int32 z_para,
						uint8 zero_size,
						FIX16_POINT_T *Point_Add_ptr);				//扫描线起始点信息指针						
/*****************************************************************************/
// Description : S2d_4_in_one_fast_PAF888
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
PUBLIC void S2d_4_in_one_fast_PAF888(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR32 *out_src_ptr,						//输出buf指针
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr);				//扫描线起始点信息指针
/*****************************************************************************/
// Description : S3D_AxisMidRotation_Img
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  S3D_AxisMidRotation_Img(S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr,
								uint8 	bright,
								void* expand_ptr);



PUBLIC BOOLEAN  S3D_AxisMidRotation_Img_shining(S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr,
								uint8 	bright,
								void* shiningPtr,
								void* expand_ptr);
PUBLIC	BOOLEAN	S2D_TypeBlending(
							 IMGREF_SIZE_T	bg_window,
							 void	*bg_buf,
							 S3D_DATA_TYPE_E bg_type,
							 IMGREF_SIZE_T	up_window,
							 FIX16_POINT_T	offset,
							 void	*up_buf,
							 S3D_DATA_TYPE_E up_type,
							 void	*out_buf,
							 void* expand_ptr);

PUBLIC BOOLEAN S2d_Type_4_in_one_fast(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						const IMGREF_POINT_T window_mid,					//图像整体位移
						const IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						void *in_src_ptr,						//输入buf指针
						S3D_DATA_TYPE_E in_type,
						void *out_src_ptr,						//输出buf指针
						S3D_DATA_TYPE_E out_type,
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr,
						void* expand_ptr);

PUBLIC BOOLEAN S2d_Type_4_in_one_fast_shining(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						const IMGREF_POINT_T window_mid,					//图像整体位移
						const IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						void *in_src_ptr,						//输入buf指针
						S3D_DATA_TYPE_E in_type,
						void *out_src_ptr,						//输出buf指针
						S3D_DATA_TYPE_E out_type,
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						void* shiningPtr,
						FIX16_POINT_T *Point_Add_ptr,
						void* expand_ptr);

PUBLIC void S2d_4_in_one_fast_zerosize(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR16 *out_src_ptr,						//输出buf指针
						uint8 zero_size,
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr);	
/*****************************************************************************/
// Description : S2d_Alpha_Blending_Out32_EX
// Global resource dependence : 
// Author:anson
// Note:
/*****************************************************************************/
PUBLIC void S2d_Alpha_Blending_Out32_EX(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR32 *out_src_ptr,						//输出buf指针
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr);				//扫描线起始点信息指针


/*****************************************************************************/
// Description : Slim_Mode_borderShiningCalc
// Global resource dependence : 
// Author:Benny
// Note:
/*****************************************************************************/
PUBLIC void Slim_Mode_borderShiningCalc(IMGREF_SIZE_T* size,COLOR32* output,IMGREF_RECT_T* ShiningZone,PIC_CIRCULAR_JOINT_T* Joint,FIX16_POINT_T* Calculate_output_ptr);


PUBLIC	BOOLEAN	S2D_BlendingBehind_ARGB888(
							 IMGREF_SIZE_T	bg_window,
							 COLOR32	*bg_buf,
							 IMGREF_SIZE_T	up_window,
							 FIX16_POINT_T	offset,
							 COLOR32	*up_buf,
							 COLOR32	*out_buf);

PUBLIC BOOLEAN PopBlinking(
	int32 cur_frame,
	int32 total_frame,
	const FIX16_POINT_T* CentrePoint, 
	int32 radius,
	COLOR32 FilledcolorVal,
	IMGREF_SIZE_T buf_size, 
	S3D_DATA_TYPE_E buf_type,
	void* in_out_buf);
PUBLIC	BOOLEAN	S2D_TypeBlending_BlockTpcy(
							 IMGREF_SIZE_T	bg_window,
							 void	*bg_buf,
							 S3D_DATA_TYPE_E bg_type,
							 IMGREF_SIZE_T	up_window,
							 FIX16_POINT_T	offset,
							 void	*up_buf,
							 uint32 tpcy, //8 bits prec
							 S3D_DATA_TYPE_E up_type,
							 void	*out_buf,
							 void* expand_ptr);

  PUBLIC	BOOLEAN	S2D_Blending_Dec(
							 IMGREF_SIZE_T	bg_window,
							 COLOR16	*bg_buf,
							 IMGREF_SIZE_T	up_window,
							 FIX16_POINT_T	offset,
							 COLOR32	*up_buf,
							 COLOR16	*out_buf);

PUBLIC void S3D_All_Axis_Interface_Type(
								  IMGREF_SIZE_T window,
								  IMGREF_SIZE_T src_all_size,
								  POINT_2D_FIX8	*sub_Vertex_ptr,
								  POINT_2D_FIX8	*sVertex_ptr,				//顶点数据表指针
								  void *window_ptr,
								  S3D_DATA_TYPE_E OutType,
								  void *src_ptr,
								  S3D_DATA_TYPE_E SrcType,
								  void* exp_ptr
					);
PUBLIC void S3D_All_Axis_Interface_shadow(
								  IMGREF_SIZE_T window,
								  IMGREF_SIZE_T src_all_size,
								  POINT_2D_FIX8	*sub_Vertex_ptr,
								  POINT_2D_FIX8	*sVertex_ptr,				//顶点数据表指针
								  uint16 *window_ptr,
								  uint16 *src_ptr,
								  uint16 origin_h);
PUBLIC void s3d_Free_Point_change_POINT_2D_FIX8
							(	IMGREF_3D_POINT_T in_Point ,	//转轴坐标
								IMGREF_3D_POINT_T sEye,				//视角位置
								POINT_2D_FIX8 *Point_ptr		//顶点数据地址
								);
PUBLIC void S2d_4_in_one_fast_PAF888_Zerosize(	const IMGREF_SIZE_T window,			//输出buf大小
						const IMGREF_SIZE_T img,					//图像大小
						IMGREF_POINT_T window_mid,					//图像整体位移
						IMGREF_POINT_T img_mid,						//旋转/放大中心点
						int32 angle,								//旋转角度
						COLOR32 *in_src_ptr,						//输入buf指针
						COLOR32 *out_src_ptr,						//输出buf指针  
						uint8 zero_size,
						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
						uint8 alpha_para,							//alpha衰减值
						FIX16_POINT_T *Point_Add_ptr)	;			//扫描线起始点信息指针   
PUBLIC void S2d_rotation_alpha_zerosize_ex(	const IMGREF_SIZE_T window,
						const IMGREF_SIZE_T img,
						IMGREF_POINT_T window_mid,
						IMGREF_POINT_T img_mid,
						int32 angle,
						COLOR32 *in_src_ptr,
						COLOR32 *out_src_ptr,
						int32 z_para,
						int32 zerosize,
						FIX16_POINT_T *Point_Add_ptr)	;

PUBLIC	BOOLEAN	S2D_Blending_PAF888ShowBox(
							 IMGREF_SIZE_T	bg_window,
							 COLOR32	*bg_buf,
							 IMGREF_SIZE_T	up_window,
							 IMGREF_RECT_T   show_box,
							 FIX16_POINT_T	offset,
							 COLOR32	*up_buf,
							 COLOR32	*out_buf);
PUBLIC void Linear_Head_draw_PAF888(uint32 color,int32 yh_st,int32 yh_end,
                                            int32 xh_st,int32 xh_end,COLOR32 *pInOutColor,IMGREF_SIZE_T size,
                                            FIX16_POINT_T cen_Poi, int32 radius_inUse);


PUBLIC void Linear_Head_draw(uint32 color,int32 yh_st,int32 yh_end,
                                            int32 xh_st,int32 xh_end,COLOR16 *pInOutColor,IMGREF_SIZE_T size,
                                            FIX16_POINT_T cen_Poi, int32 radius_inUse);

PUBLIC void Linear_Body_U(uint32 color,int32 y_st,int32 y_end,
                                            int32 x_st,int32 x_end,COLOR16 *pInOutColor,IMGREF_SIZE_T size,
                                            FIX16_POINT_T pos_inPic, IMGREF_SIZE_T wh_in2D);

PUBLIC void Linear_Body_L(uint32 color,int32 y_st,int32 y_end,
                                            int32 x_st,int32 x_end,COLOR16 *pInOutColor,IMGREF_SIZE_T size,
                                            FIX16_POINT_T pos_inPic, IMGREF_SIZE_T wh_in2D);

PUBLIC void Linear_Body_D(uint32 color,int32 y_st,int32 y_end,
                                            int32 x_st,int32 x_end,COLOR16 *pInOutColor,IMGREF_SIZE_T size,
                                            FIX16_POINT_T pos_inPic, IMGREF_SIZE_T wh_in2D);

PUBLIC void Linear_Body_R(uint32 color,int32 y_st,int32 y_end,
                                            int32 x_st,int32 x_end,COLOR16 *pInOutColor,IMGREF_SIZE_T size,
                                            FIX16_POINT_T pos_inPic, IMGREF_SIZE_T wh_in2D);

PUBLIC void MagicPop(
	IMGREF_SIZE_T window,
	const FIX16_POINT_T* CentrePoint, 
	int32 radius,// 4 bits prec
	int32 cos_top,// 4 bits prec
	int32 cos_st,// 4 bits prec
	int32 cos_st_angle,
	COLOR32 FilledcolorVal,
	COLOR16	*outbuf);
PUBLIC void s3d_Free_Point_change_POINT_2D_FLOAT
							(	IN IMGREF_3D_FLOAT_P_T in_Point ,	//转轴坐标
								IN IMGREF_3D_FLOAT_P_T sEye,				//视角位置
								OUT POINT_2D_FIX8 *Point_ptr		//顶点数据地址
								)	;
PUBLIC void s3d_Free_Point_change_0P3FToFIX8_FLOAT
							(	POINT_3D_0P3F in_Point ,	//转轴坐标
								IMGREF_3D_POINT_T sEye,				//视角位置
								POINT_2D_FIX8 *Point_ptr		//顶点数据地址
								);

PUBLIC void ShiningBorderDraw(
    PIC_CIRCULAR_JOINT_T* cirJoint,
	const IMGREF_RECT_T* ShiningZone,
	const IMGREF_SIZE_T* OutWindow,
	void* outbuf,
	S3D_DATA_TYPE_E OutType,
	IMGREF_RECT_T* minRefreshZone,
	uint32 ShiningRadius,
	COLOR32 ShiningColor,
	int32 ShiningThickness,
	uint32 cur_frame,
	uint32 total_frame,
	FIX16_POINT_T* Calculated_ptr   //size 4096
	);

PUBLIC void ShiningBorderCalc(
	PIC_CIRCULAR_JOINT_T* cirJoint,
	const IMGREF_RECT_T* ShiningZone,
	uint32* total_frame,
	FIX16_POINT_T* Calculate_output_ptr);

/*****************************************************************************/
// Description : S2D_BlendingOverWrite_PAF888
// Global resource dependence : 
// Author:Anson
// Note:for second menu project
/*****************************************************************************/
PUBLIC	BOOLEAN	S2D_BlendingOverWrite_PAF888(
							 IMGREF_SIZE_T	bg_window,
							 COLOR32	*bg_buf,
							 IMGREF_SIZE_T	up_window,
							 FIX16_POINT_T	offset,
							 COLOR32	*up_buf,
							 COLOR32	*out_buf);

PUBLIC void change_free_rotation_chage(
								 IMGREF_3D_POINT_T *sPoint_ptr,		//视角位置
								 IMGREF_3D_POINT_T *sAxis_ptr,
								 int16 angle
								);
//anson add for 3d party
PUBLIC void S2d_rotation_slim_fix16(	const IMGREF_SIZE_T window,
						const IMGREF_SIZE_T img,
						FIX16_POINT_T window_mid,
						FIX16_POINT_T img_mid,
						int32 zero_size,
						int32 angle,
						COLOR32 *in_src_ptr,
						COLOR16 *out_src_ptr,
						int32 z_para,
						FIX16_POINT_T *Point_Add_ptr);				//扫描线起始点信息指针

#ifdef __cplusplus
}
#endif

#endif

