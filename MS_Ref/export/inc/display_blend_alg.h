#include "sci_types.h"


PUBLIC void DISPLAY_Blend_ARGB888ToARGB888(BOOLEAN is_colorkey_en, 
                                          uint32 colorkey,
                                          uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          );
                                          
                                          

PUBLIC void DISPLAY_Blend_RGB565ToARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 colorkey,
                                          uint32 *dst_buf,
                                          uint16 *src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          );
                                          
                                          

/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//	Global resource dependence:
//  Author: Jianping.wang
//  param: top layer: PMARGB888
//              bottom layer: ARGB888: image, not OSD. alpha is ignored.
//	Note:  do not support color key and layer_alpha
/*****************************************************************************/

PUBLIC void DISPLAY_Blend_PMARGB888ToARGB888(BOOLEAN is_colorkey_en, 
                                          uint32 colorkey,
                                          uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          );



/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//	Global resource dependence:
//  Author: Jianping.wang
//  param: top layer: ARGB888
//              bottom layer: PMARGB888
//	Note:
/*****************************************************************************/


PUBLIC void DISPLAY_Blend_ARGB888ToPMARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 colorkey,
										  uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          );
                                          
                                          

/*****************************************************************************/
//  Description:  low layer blends with RGB565
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC void DISPLAY_Blend_RGB565ToPMARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 colorkey,
                                          uint32 *dst_buf,
                                          uint16 *src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
										  									  uint32 layer_alpha
                                          );
                                          
                                          
PUBLIC  void DISPLAY_Blend_PMARGB888ToPMARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 colorkey,
										  										uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          );
                                          



PUBLIC void  DISPLAY_ARGB888ToPMARGB888(uint32 *dst_ptr, 
							  uint32 *src_ptr, 
							  uint32 dst_width,
							  uint32 src_width, 
							  uint32 roi_width,
							  uint32 roi_height, 
							  uint32 colorkey,
							  uint32 colorkey_en);
							  
							  

PUBLIC void  DISPLAY_RGB565ToPMARGB888(uint32 *dst_ptr,
							 uint16 *src_ptr,
							 uint32 dst_width,
							 uint32 src_width,
							 uint32 roi_width,
							 uint32 roi_height,
							 uint32 colorkey,
							 uint32 colorkey_en); 


PUBLIC void DISPLAY_YUV2RGB565(uint16 *rgb565_ptr, uint8 * y_ptr, uint8 * uv_ptr, uint32 w,uint32 h);


PUBLIC void DISPLAY_YUV2ARGB888(uint32 *argb888_ptr, uint8 * y_ptr, uint8 * uv_ptr, uint32 w,uint32 h);


//width must be 2X pixel.
PUBLIC void DISPLAY_RGB5652YUV422( uint8 * y_ptr,uint8 * uv_ptr, uint16 *rgb565_ptr,uint32 w,uint32 h);


//width must be 2X pixel.
//argb888_ptr is image, not osd. Alpha will be skipped.
PUBLIC void DISPLAY_ARGB8882YUV422( uint8 * y_ptr,uint8 * uv_ptr, uint16 *argb888_ptr,uint32 w,uint32 h);