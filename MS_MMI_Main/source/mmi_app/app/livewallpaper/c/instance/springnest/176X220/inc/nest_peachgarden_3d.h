#ifndef __3D_PEACHGARDEN_H__
#define __3D_PEACHGARDEN_H__

    #include "graphics_3d_rotate.h"
    

    #define PEACHFLOWER_NUM 12
    #define PEACHFLOWER_VER_NUM 7
    #define PEACHFLOWER_PETAL_NUM  3   

    #define PEACHGARDEN_QVGA 1
    #define PEACHGARDEN_HVGA 0
    typedef struct
    {
    	RES_TYPE_3D Petal[PEACHFLOWER_PETAL_NUM];
    	//IMGREF_3D_POINT_T Res_box;// 3张图片的外框	
        
    	//IMGREF_3D_POINT_T GravityOff2PicCenter;//重心坐标和图片合成时的坐标偏移
    	
    }PEACHFLOWER_RES_T;

    //extern char PeachGardenOrder[NEST_LAMBERT_RESNUM][128];
    PUBLIC void SetPeachGardenResOrder(RES_TYPE_3D* PeachGardenlambertRes);
    PUBLIC uint32 Nest_GetTOTALGapFRAME();
    PUBLIC uint32 Nest_GetTOTALKeyFRAME();
    PUBLIC uint32 Nest_GetMeterialNum();
	PUBLIC uint32 GetPeachGardenFaceNum();
    //PUBLIC void SetResOrder();
    PUBLIC const char* Nest_GetResFromOrder(int32 order);
    PUBLIC int32 Nest_GetOrderFromResName(const char resName[128]);
    PUBLIC void PeachGardenCalc(int32 Gapframe, 
						int32 GaptotalFrame, 
						int32 KeyFrame, 
						COLOR16* BG_ResBuf, 
						RES_TYPE_3D* PeachGardenlambertRes,
						PEACHFLOWER_RES_T* PeachFlowerRes,
						IMGREF_SIZE_T* pWindowSize, 
						COLOR16* TargetBuf,
						int32* calcBuf);
#endif
