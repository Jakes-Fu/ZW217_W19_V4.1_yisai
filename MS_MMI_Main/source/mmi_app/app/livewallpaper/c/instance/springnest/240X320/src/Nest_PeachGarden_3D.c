

#include "nest_peachgarden_3d.h"
//#include "stdafx.h"
#include "os_api.h"
#include "stdio.h"
#include "Graphic3D_CalcEngine.h"
#include "nest_mayadatainc.h"
#include "nest_quatslerpdata.h"

#define CAMERA_VIEWFINDER_W 11289//这个参数从maya获得，配合屏幕大小用于计算缩放比例
#define CAMERA_VIEWFINDER_H 16933//这个参数从maya获得，配合屏幕大小用于计算缩放比例
#define SCALE_MAYA2PHONE ((CAMERA_VIEWFINDER_W+CAMERA_VIEWFINDER_H)/(320+480))

#define PEACHGARDEN_SPACE_BASE_X 666
#define PEACHGARDEN_SPACE_BASE_Y -2665
#define PEACHGARDEN_SPACE_BASE_Z 1200

#define PEACHGARDEN_SPACE_X_MIN	( PEACHGARDEN_SPACE_BASE_X - 20000)
#define PEACHGARDEN_SPACE_X_MAX	( PEACHGARDEN_SPACE_BASE_X + 20000)
#define PEACHGARDEN_SPACE_Y_MIN	(PEACHGARDEN_SPACE_BASE_Y-6000)
#define PEACHGARDEN_SPACE_Y_MAX 	(PEACHGARDEN_SPACE_BASE_Y+26000)
#define PEACHGARDEN_SPACE_Z_MIN	(PEACHGARDEN_SPACE_BASE_Z-10000)
#define PEACHGARDEN_SPACE_Z_MAX	(PEACHGARDEN_SPACE_BASE_Z+10000)

#define PEACHGARDEN_SPACE_CONSTRAIN_Z_MAX	2328
#define PEACHGARDEN_SPACE_CONSTRAIN_Z_MIN	-467

#define PEACHGARDEN_CAMERA_CONSTRAIN_Z_MAX	0
#define PEACHGARDEN_CAMERA_CONSTRAIN_Z_MIN	-20000
#define PEACHGARDEN_CAMERA_CONSTRAIN_X_MAX (800+5000)
#define PEACHGARDEN_CAMERA_CONSTRAIN_X_MIN	(-5000)

#define PEACHGARDEN_GROUND_Y PEACHGARDEN_SPACE_Y_MAX

#define IsFloatProc 0

#define SORT_MIN_VAL 0xffffffff

typedef struct
{
	BOOLEAN IsActivated;	
	PEACHFLOWER_RES_T* pPetalRes;
	POINT_3D_0P3F PeachFlowerSize;
	int32 fallDownVec;//下落速度
	POINT_3D_0P3F gravity_pos;
	POINT_3D_0P3F DefaultVerInGravity[PEACHFLOWER_VER_NUM];//顶点初始形态在重力点坐标系坐标
	POINT_3D_0P3F PeachFlowerVer[PEACHFLOWER_VER_NUM];//顶点全局坐标
	POINT_3D_0P3F VertexInCamera[PEACHFLOWER_VER_NUM];//手机坐标系
	IMGREF_3D_POINT_T Rot_axis;//重心坐标系下的旋转轴
	int32 y_std;//起始花朵y值，花朵消失值由PEACHGARDEN_GROUND_Y决定
	
	//IMGREF_3D_POINT_T PeachFlowerVerInGravity[PEACHFLOWER_VER_NUM];//顶点重力点坐标系坐标

	int32 Rot_angle_max;
	int32 Rot_angle_PeriodByY;
	int32 xzBase_x;
	int32 xzBase_z;
	int32 xzBase_angle;
	int32 xzBase_Amp_numerator;
	int32 xzBase_Amp_denominator;
	int32 xzBase_PeriodByY;
	
}PEACHFLOWER_3D_OBJ_T;

typedef struct
{
POINT_3D_0P3F* Peach3DVertex[4];
MAYA_UV_T* Peach3DUV[4];
RES_TYPE_3D* BranchRes;
}PEACHFACE_OBJ_T;//FACE_ATTRIBUTE_OBJ_T;


//int32 CameraFocusLen = 18000;//25500;
//真实眼睛到屏幕的距离，该距离是将相机放置
//平行于z轴xy与关注点相同时候，调试出来的和真实图像大小一致时候的值,
//这个值需要一开始就寻找到
#if PEACHGARDEN_QVGA
LOCAL int32 CameraFocusLen = 18000;//25500;
#elif PEACHGARDEN_HVGA
LOCAL int32 CameraFocusLen = 25500;//25500;
#else
LOCAL int32 CameraFocusLen = 25500;//25500;
#endif


#if PEACHGARDEN_QVGA
LOCAL IMGREF_POINT_T PeachGardenBG_Pos[NEST_KEY_FRAME_NUM] =
	{
		{0,5},
		{0,150},
		{0,250},
		{0,330},
		{0,410},
		{0,470},
		{0,520}
	};
#elif PEACHGARDEN_HVGA
LOCAL IMGREF_POINT_T PeachGardenBG_Pos[NEST_KEY_FRAME_NUM] =
	{
		{0,5},
		{0,150},
		{0,250},
		{0,330},
		{0,410},
		{0,470},
		{0,520}
	};
#else
LOCAL IMGREF_POINT_T PeachGardenBG_Pos[NEST_KEY_FRAME_NUM] =
	{
		{0,5},
		{0,150},
		{0,250},
		{0,330},
		{0,410},
		{0,470},
		{0,520}
	};
#endif



LOCAL POINT_3D_0P3F KeyFrame_CameraPos[NEST_KEY_FRAME_NUM] = 
{
	{188,-5090,18117},
	{132,6445,18181},
	{307,9938,13960},
	{451,10887,10959},
	{382,10779,7709},
	{446,10899,3722},
	{673,9269,171}
};

LOCAL IMGREF_3D_POINT_T KeyFrame_CameraAngle[NEST_KEY_FRAME_NUM] = 
{
	{109,-15,0,0},
	{-227,-15,0,0},
	{-353,-7,0,0},
	{-437,-11,0,0},
	{-509,-19,0,0},
	{-647,-27,0,0},
	{-797,-27,0,0}
};

//char PeachGardenOrder[NEST_LAMBERT_RESNUM][128] = {0};

LOCAL PEACHFLOWER_3D_OBJ_T PeachFlower_3D[PEACHFLOWER_NUM] = {0};
LOCAL PEACHFACE_OBJ_T PeachFace[PEACHFACENUM+PEACHFLOWER_NUM*3] = {0};
LOCAL POINT_3D_0P3F Peach3DVertex2Camera[PEACHVERTEXNUM] = {0};
LOCAL POINT_3D_0P3F FreeVertex = {0,0,SORT_MIN_VAL};/*lint !e569*/

LOCAL RES_TYPE_3D* PeachGardenFaceOrder[PEACHFACENUM] = {0};

PUBLIC uint32 Nest_GetMeterialNum()
{
	return NEST_LAMBERT_RESNUM;
}
PUBLIC uint32 Nest_GetTOTALGapFRAME()
{
	return NEST_CAMERA_POSITION_TOTALFRAME;
}

PUBLIC uint32  Nest_GetTOTALKeyFRAME()
{
	return NEST_KEY_FRAME_NUM;
}

PUBLIC uint32 GetPeachGardenFaceNum()
{
	return PEACHFACENUM;
}

/*PUBLIC void SetResOrder()
{
	int32 i=0,j=0,k=0;
	BOOLEAN IsMatched = FALSE;

	for(j = 0; j < NEST_LAMBERT_RESNUM;j++)
	{
		memset(PeachGardenOrder[j],0,128);
	}

	i = 0;

	for(j = 0; j < PEACHFACENUM;j++)
	{
		IsMatched = FALSE;
		for(k = 0 ; k < i; k++)
		{
			if(0 == strcmp(PeachGardenOrder[k],Nest_MayaFace[j].MaterialName))
			{
				IsMatched = TRUE;
				break;
			}
		}
		if(!IsMatched)
		{
			strcpy(PeachGardenOrder[i],Nest_MayaFace[j].MaterialName);
			i++;
		}		
	}
	if(NEST_LAMBERT_RESNUM != i)
	{
		//MessageBox(NULL, "Error NEST_LAMBERT_RESNUM !", "NEST_LAMBERT_RESNUM Error", MB_OK);
		return;
	}
}*/

PUBLIC const char* Nest_GetResFromOrder(int32 order)
{
	if(order>=NEST_LAMBERT_RESNUM)
	{
		//MessageBox(NULL, "Error order !", "order Error", MB_OK);
		return NULL;
	}
	return PeachGardenOrder[order];
}
PUBLIC int32 Nest_GetOrderFromResName(const char resName[128])
{
	int32 j = 0;
	
	for(j = 0; j < NEST_LAMBERT_RESNUM;j++)
	{
		if(0 == strcmp(PeachGardenOrder[j],resName))
		{
			return j;
		}
	}
	//MessageBox(NULL, "Error ResName !", "ResName Error", MB_OK);
	return NEST_LAMBERT_RESNUM;

}

LOCAL RES_TYPE_3D* GetResFromLambertName(const char* pName,RES_TYPE_3D* PeachGardenlambertRes)
{
	int32 i = 0;
	i = Nest_GetOrderFromResName(pName);
	if(i>=NEST_LAMBERT_RESNUM||i<0)
	{
		//MessageBox(NULL, "Error Lambert Name!", "Res Error", MB_OK);
		return NULL;
	}
	return &(PeachGardenlambertRes[i]);
}

LOCAL int32 GETVertexIndexFromMayaUV(const MAYA_UV_T* uv)
{
	if(5 > uv->Vertex_U&&5 > uv->Vertex_V)
	{
		return 3;
	}
	else if(995 < uv->Vertex_U&&5 > uv->Vertex_V)
	{
		return 2;
	}
	else if(995 < uv->Vertex_U&&995 < uv->Vertex_V)
	{
		return 1;
	}
	else if(5 > uv->Vertex_U&&995 < uv->Vertex_V)
	{
		return 0;
	}
	else
	{
		//MessageBox(NULL, "Error Nest_MayaUV !", "UV Error", MB_OK);
		return 0;
	}
	
}

LOCAL void GetAxisInMobilePhone(POINT_3D_0P3F* pVertexInCamera,
										const POINT_3D_0P3F* pPeachFlowerVer,
										int32 scale,
										POINT_3D_0P3F* CameraPos,
										QUATERNIONS_T* CalcQuat,
										IMGREF_3D_POINT_T* eyePos
										)
{
	POINT_3D_FLOAT BasePoint = {0};
	POINT_3D_FLOAT cameraVer = {0};
	QUATERNIONS_FLOAT_T Rot_Quat = {0};

	cameraVer.x = pPeachFlowerVer->x;
	cameraVer.x = (cameraVer.x-CameraPos->x);
	cameraVer.y = pPeachFlowerVer->y;
	cameraVer.y = -1*(cameraVer.y-CameraPos->y);
	cameraVer.z = pPeachFlowerVer->z;
	cameraVer.z = -1*(cameraVer.z-CameraPos->z);

	Rot_Quat.w = (float)(CalcQuat->w/65536.0);
	Rot_Quat.x = (float)(CalcQuat->x/65536.0);
	Rot_Quat.y = (float)(CalcQuat->y/65536.0);
	Rot_Quat.z = (float)(CalcQuat->z/65536.0);

	Get_QuaterninosTranslated_float(&cameraVer,&BasePoint,&Rot_Quat);
    if(0 == scale)
        return;
	pVertexInCamera->x = (long int)((cameraVer.x*65536)/scale + (eyePos->x<<16));
	pVertexInCamera->y = (long int)((cameraVer.y*65536)/scale + (eyePos->y<<16));
	pVertexInCamera->z = (long int)((cameraVer.z*65536)/scale + (eyePos->z<<16));

	
	/*pVertexInCamera->x = (pPeachFlowerVer->x-CameraPos->x)*65536;
	pVertexInCamera->y = -1*(-1*pPeachFlowerVer->y-CameraPos->y)*65536;
	pVertexInCamera->z = -1*(-1*pPeachFlowerVer->z-CameraPos->z)*65536;
	Get_QuaterninosTranslated(pVertexInCamera,&BasePoint2,CalcQuat);
	pVertexInCamera->x = (pVertexInCamera->x)/scale + (eyePos->x<<16);
	pVertexInCamera->y = (pVertexInCamera->y)/scale + (eyePos->y<<16);
	pVertexInCamera->z = (pVertexInCamera->z)/scale + (eyePos->z<<16);*/
}

LOCAL void PeachVerCalc(IMGREF_3D_POINT_T* eyePos,
							int32 scale,
							POINT_3D_0P3F* CalcPoint,
							QUATERNIONS_T* CalcQuat,
							RES_TYPE_3D* PeachGardenlambertRes)
{
	int32 i = 0, j = 0,i_temp = 0;
	POINT_3D_0P3F BasePoint = {0};
	int32 time;
	/*
		BranchImgPos_Set存储的是全局坐标，先对于眼睛位置，在眼睛竖直状态下，
		3D 空间里的平移和缩放是我们能接受的线性形变。以获得手机坐标里面的全局坐标
		
		eyePos为移位操作
		Real2PixelScale 为缩放操作
		
		将base点移到我们希望的点上，
		再控制Real2PixelScale将模型缩小到我们希望的大小
		
		注意这里所有坐标的平移和缩放走只在相机坐标系三个坐标轴平行于maya全局坐标系时有效
		即没有经过rotate旋转
	
    */		
		/*
		for(i = 0; i < PEACHVERTEXNUM; ++i)
		{
			Peach3DVertex2Camera[i].x = (Peach3DVertex[i].x-CalcPoint->x)/scale + eyePos->x;
			Peach3DVertex2Camera[i].y = -1*(Peach3DVertex[i].y-CalcPoint->y)/scale + eyePos->y;
			Peach3DVertex2Camera[i].z = -1*(Peach3DVertex[i].z-CalcPoint->z)/scale + eyePos->z;
			Get_QuaterninosTranslated_V3(&(Peach3DVertex2Camera[i]),eyePos,CalcQuat);
		}
		*/
time = SCI_GetTickCount();
    BasePoint.x = 0;
    BasePoint.y = 0;
    BasePoint.z = 0;
		for(i = 0; i < PEACHVERTEXNUM; ++i)
		{
			#if IsFloatProc
				Peach3DVertex2Camera[i].x = (Peach3DVertex[i].x-CalcPoint->x)*65536;
				Peach3DVertex2Camera[i].y = -1*(Peach3DVertex[i].y-CalcPoint->y)*65536;
				Peach3DVertex2Camera[i].z = -1*(Peach3DVertex[i].z-CalcPoint->z)*65536;
				Get_QuaterninosTranslated(&(Peach3DVertex2Camera[i]),&BasePoint,CalcQuat);
                        if(0 != scale)
                          {
				Peach3DVertex2Camera[i].x = (Peach3DVertex2Camera[i].x)/scale + (eyePos->x<<16);
				Peach3DVertex2Camera[i].y = (Peach3DVertex2Camera[i].y)/scale + (eyePos->y<<16);
				Peach3DVertex2Camera[i].z = (Peach3DVertex2Camera[i].z)/scale + (eyePos->z<<16);
                          }
			#else

                    GetAxisInMobilePhone(&(Peach3DVertex2Camera[i]),
										&(Peach3DVertex[i]),
										scale,
										CalcPoint,
										CalcQuat,
										eyePos
										);

			#endif
		}
//SCI_TRACE_LOW("[NEST] calc PeachVerCalc-GetAxisInMobilePhone %d",SCI_GetTickCount()-time);

		//玛雅里提供相机旋转参数，这里将物体坐标相对于摄像机坐标反旋转即可
		
		//欧拉角到四元素的转换
		//*
		//Rot_pos = eyePos;//BranchObj.Point_Base;
		/*calc_angle.x = -1*Camera_angle3D.x;
		calc_angle.y = Camera_angle3D.y;
		calc_angle.z = Camera_angle3D.z;
		Quat_do = GetQuaterninosFromEuler(&calc_angle);*/		
		
		/*
		//将树枝顶点分配给各图片
		
		*/
time = SCI_GetTickCount();
		for(i = 0; i < PEACHFACENUM; ++i)
		{
			PeachFace[i].BranchRes = PeachGardenFaceOrder[i];//GetResFromLambertName(Nest_MayaFace[i].MaterialName,PeachGardenlambertRes);
			for(j = 0; j < 4; j++)
			{
				i_temp = GETVertexIndexFromMayaUV(&Nest_MayaUV[Nest_MayaFace[i].VertexUV[j]-1]);
				PeachFace[i].Peach3DVertex[i_temp] \
					= &Peach3DVertex2Camera[Nest_MayaFace[i].VertexIndex[j]-1];
			}
		}
//SCI_TRACE_LOW("[NEST] calc PeachVerCalc-VertexIndex %d",SCI_GetTickCount()-time);	

}


LOCAL void SetDefVerInGravity(IMGREF_3D_POINT_T* FlowerDrection, PEACHFLOWER_3D_OBJ_T* pPeachFlower, int32 angle)
{
	IMGREF_3D_POINT_T Def_vec = {0};
	IMGREF_3D_POINT_T N_Def_vec = {0};
	IMGREF_3D_POINT_T N_Drection = {0};
	IMGREF_3D_POINT_T N_Rot = {0};
	IMGREF_3D_POINT_T PointCalc = {0};
	//IMGREF_3D_POINT_T Ori_PeachFlowerVer[PEACHFLOWER_VER_NUM]={0};
	int32 GravityAxisPercent = 4; // 25%
	int32 i = 0;

	POINT_3D_0P3F Axis_zero = {0};
	POINT_3D_0P3F Axis_Max = {0};


	Axis_zero.x = -pPeachFlower->PeachFlowerSize.x/GravityAxisPercent;
	Axis_zero.y = -pPeachFlower->PeachFlowerSize.y/GravityAxisPercent;
	Axis_zero.z = pPeachFlower->PeachFlowerSize.z/GravityAxisPercent;

	Axis_Max.x = pPeachFlower->PeachFlowerSize.x - pPeachFlower->PeachFlowerSize.x/GravityAxisPercent;
	Axis_Max.y = pPeachFlower->PeachFlowerSize.y - pPeachFlower->PeachFlowerSize.y/GravityAxisPercent;
	Axis_Max.z = pPeachFlower->PeachFlowerSize.z/GravityAxisPercent - pPeachFlower->PeachFlowerSize.z;
	
	i=0;
		pPeachFlower->DefaultVerInGravity[i].x = Axis_zero.x;
		pPeachFlower->DefaultVerInGravity[i].y = Axis_zero.y;
		pPeachFlower->DefaultVerInGravity[i].z = Axis_zero.z;
	i=1;
		pPeachFlower->DefaultVerInGravity[i].x = Axis_Max.x;
		pPeachFlower->DefaultVerInGravity[i].y = Axis_zero.y;
		pPeachFlower->DefaultVerInGravity[i].z = Axis_zero.z;
	i=2;
		pPeachFlower->DefaultVerInGravity[i].x = Axis_Max.x;
		pPeachFlower->DefaultVerInGravity[i].y = Axis_zero.y;
		pPeachFlower->DefaultVerInGravity[i].z = Axis_Max.z;
	i=3;
		pPeachFlower->DefaultVerInGravity[i].x = Axis_zero.x;
		pPeachFlower->DefaultVerInGravity[i].y = Axis_zero.y;
		pPeachFlower->DefaultVerInGravity[i].z = Axis_Max.z;
	i=4;
		pPeachFlower->DefaultVerInGravity[i].x = Axis_zero.x;
		pPeachFlower->DefaultVerInGravity[i].y = Axis_Max.y;
		pPeachFlower->DefaultVerInGravity[i].z = Axis_zero.z;
	i=5;
		pPeachFlower->DefaultVerInGravity[i].x = Axis_Max.x;
		pPeachFlower->DefaultVerInGravity[i].y = Axis_Max.y;
		pPeachFlower->DefaultVerInGravity[i].z = Axis_zero.z;
	i=6;
		pPeachFlower->DefaultVerInGravity[i].x = Axis_zero.x;
		pPeachFlower->DefaultVerInGravity[i].y = Axis_Max.y;
		pPeachFlower->DefaultVerInGravity[i].z = Axis_Max.z;		

	Def_vec.x = Def_vec.y = 1;
	Def_vec.z = -1;
	Vector_Normalize(&Def_vec,&N_Def_vec);
	Vector_Normalize(FlowerDrection,&N_Drection);

	
	Vector_MultiplicateCross_Normalize(&N_Def_vec,&N_Drection,&N_Rot,TRUE);

	for(i = 0;i<PEACHFLOWER_VER_NUM;++i)
	{
		PointCalc.x = pPeachFlower->DefaultVerInGravity[i].x;
		PointCalc.y = pPeachFlower->DefaultVerInGravity[i].y;
		PointCalc.z = pPeachFlower->DefaultVerInGravity[i].z;
		
		change_free_rotation_chage(&PointCalc,&N_Rot,angle);

		pPeachFlower->DefaultVerInGravity[i].x = PointCalc.x;
		pPeachFlower->DefaultVerInGravity[i].y = PointCalc.y;
		pPeachFlower->DefaultVerInGravity[i].z = PointCalc.z;
	}

	
}



LOCAL void ResetPeachFlower(PEACHFLOWER_3D_OBJ_T* pPeachFlower,PEACHFLOWER_RES_T* PeachFlowerRes)
{
	IMGREF_3D_POINT_T FlowerDefDrection = {0};
	int32 def_angle = 0;
	int32 i = 0;

	FlowerDefDrection.x = 0;
	FlowerDefDrection.y = 1;
	FlowerDefDrection.z = 0;
	def_angle = rand()%3600-1800;

	pPeachFlower->pPetalRes = PeachFlowerRes;//&(PeachFlowerRes[1]);
	i = 800;//800+((rand()%800)-400);
	pPeachFlower->PeachFlowerSize.x = i;
	pPeachFlower->PeachFlowerSize.y = i;
	pPeachFlower->PeachFlowerSize.z = i;

	pPeachFlower->Rot_axis.x = 10+rand()%10;
	pPeachFlower->Rot_axis.y = -10+rand()%10;
	pPeachFlower->Rot_axis.z = 10+rand()%10;

	pPeachFlower->fallDownVec = 130+40;//rand()%80-40;
	//*
	pPeachFlower->gravity_pos.y = PEACHGARDEN_SPACE_Y_MIN-(rand()%18000);
	pPeachFlower->xzBase_x = PEACHGARDEN_SPACE_X_MIN+((rand()%(PEACHGARDEN_SPACE_X_MAX-PEACHGARDEN_SPACE_X_MIN)));
	pPeachFlower->xzBase_z = PEACHGARDEN_SPACE_Z_MIN+((rand()%(PEACHGARDEN_SPACE_Z_MAX-PEACHGARDEN_SPACE_Z_MIN)));
	//*/
	/*
	i = rand()%PEACHFLOWER_FALLPOINT_NUM;
	pPeachFlower->gravity_pos.y = -1*PEACHFLOWER_FallPoint[i].y;
	pPeachFlower->xzBase_x = PEACHFLOWER_FallPoint[i].x;
	pPeachFlower->xzBase_z = -1*PEACHFLOWER_FallPoint[i].z;
    */
	
	pPeachFlower->Rot_angle_PeriodByY = 20000+((rand()%10000));//20000;//
	pPeachFlower->xzBase_PeriodByY = 22000+((rand()%2000)-1000);//16000;
	pPeachFlower->Rot_angle_max = 2000-((rand()%1000));//	

	pPeachFlower->xzBase_angle = 1300+((rand()%600)-300);
	pPeachFlower->xzBase_Amp_numerator = 30;//30+((rand()%20)-10);
	pPeachFlower->xzBase_Amp_denominator = 100;
	SetDefVerInGravity(&FlowerDefDrection,pPeachFlower,def_angle);
	
	pPeachFlower->IsActivated = TRUE;

	if(pPeachFlower->xzBase_z > PEACHGARDEN_SPACE_CONSTRAIN_Z_MIN
		&&pPeachFlower->xzBase_z < PEACHGARDEN_SPACE_CONSTRAIN_Z_MAX)
	{
		pPeachFlower->IsActivated = FALSE;
	}
	/*if(pPeachFlower->xzBase_z > PEACHGARDEN_CAMERA_CONSTRAIN_Z_MIN
		&&pPeachFlower->xzBase_z < PEACHGARDEN_CAMERA_CONSTRAIN_Z_MAX
		&&pPeachFlower->xzBase_x > PEACHGARDEN_CAMERA_CONSTRAIN_X_MIN
		&&pPeachFlower->xzBase_x < PEACHGARDEN_CAMERA_CONSTRAIN_X_MAX)
	{
		pPeachFlower->IsActivated = false;
	}*/
}





LOCAL void GetPeachFlowerVer(PEACHFLOWER_3D_OBJ_T* pPeachFlower, IMGREF_3D_POINT_T* Rot_vec, int32 angle)
{
	IMGREF_3D_POINT_T N_Rot = {0};
	IMGREF_3D_POINT_T Ori_PeachFlowerVer[PEACHFLOWER_VER_NUM]={0};
	int32 i = 0;
	
	for(i = 0;i<PEACHFLOWER_VER_NUM;++i)
	{
		Ori_PeachFlowerVer[i].x = pPeachFlower->DefaultVerInGravity[i].x;
		Ori_PeachFlowerVer[i].y = pPeachFlower->DefaultVerInGravity[i].y;
		Ori_PeachFlowerVer[i].z = pPeachFlower->DefaultVerInGravity[i].z;
	}

	Vector_Normalize(Rot_vec,&N_Rot);

	for(i = 0;i<PEACHFLOWER_VER_NUM;++i)
	{
		change_free_rotation_chage(&Ori_PeachFlowerVer[i],&N_Rot,angle);
	}
	
	for(i = 0;i<PEACHFLOWER_VER_NUM;++i)
	{
		pPeachFlower->PeachFlowerVer[i].x = Ori_PeachFlowerVer[i].x+pPeachFlower->gravity_pos.x;
		pPeachFlower->PeachFlowerVer[i].y = Ori_PeachFlowerVer[i].y+pPeachFlower->gravity_pos.y;
		pPeachFlower->PeachFlowerVer[i].z = Ori_PeachFlowerVer[i].z+pPeachFlower->gravity_pos.z;
	}
	
}

LOCAL BOOLEAN IsPeachFlowerLanded(PEACHFLOWER_3D_OBJ_T* pPeachFlower)
{
	if(pPeachFlower->gravity_pos.y > PEACHGARDEN_GROUND_Y)
	{
		return TRUE;
	}
	return FALSE;
}



LOCAL void Get3DPetalFaceVer(uint32 PetalNum,PEACHFLOWER_3D_OBJ_T* pPeachFlower, PEACHFACE_OBJ_T* pFace)
{
	int32 i = 0,j=0;
	
	pFace->BranchRes = &(pPeachFlower->pPetalRes->Petal[PetalNum-1]);
	if(1 == PetalNum)
	{		
		i=0;
		j=2;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);
		

		i=1;
		j=1;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);

		i=2;
		j=0;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);

		i=3;
		j=3;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);
	}
	else if(2 == PetalNum)
	{
		i=0;
		j=3;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);

		i=1;
		j=0;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);

		i=2;
		j=4;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);

		i=3;
		j=6;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);
	}
	else if(3 == PetalNum)
	{
		i=0;
		j=0;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);

		i=1;
		j=1;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);

		i=2;
		j=5;
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);
		//In_3D_ver[i].w = PeachFlowerVer[j].w;

		i=3;
		j=4;
		/*pFace->Peach3DVertex[i].x =  pPeachFlower->VertexInCamera[j].x;
		pFace->Peach3DVertex[i].y =  pPeachFlower->VertexInCamera[j].y;
		pFace->Peach3DVertex[i].z =  pPeachFlower->VertexInCamera[j].z;*/
		pFace->Peach3DVertex[i] = &(pPeachFlower->VertexInCamera[j]);
		//In_3D_ver[i].w = PeachFlowerVer[j].w;
	}
	else
	{
		for(i=0;i<4;i++)
		{
			pFace->Peach3DVertex[i] = NULL;
		}		
	}
}



LOCAL void PeachFlowerFaceCalc(PEACHFLOWER_3D_OBJ_T* pPeachFlower, 
										PEACHFLOWER_RES_T* PeachFlowerRes,
										IMGREF_3D_POINT_T* eyePos,
										int32 scale,
										POINT_3D_0P3F* CalcPoint,
										QUATERNIONS_T* CalcQuat,
										PEACHFACE_OBJ_T* peachFace, 
										int32* faceNum)
{
	int32 delta_y = 0,SelfRot_angle;
	int32 base_r = 0, i = 0;
	POINT_3D_0P3F BasePoint = {0};
	static int32 s_i = 0;

	POINT_3D_0P3F CalcFlowerVer = {0};

	s_i++;
	if(s_i>=3600)
		s_i = 0;
	
	if(!pPeachFlower->IsActivated)
	{
		ResetPeachFlower(pPeachFlower,PeachFlowerRes);		
	}
	
	if(pPeachFlower->IsActivated)
	{
		pPeachFlower->gravity_pos.y += pPeachFlower->fallDownVec*2;
		
		delta_y = (pPeachFlower->gravity_pos.y-PEACHGARDEN_SPACE_Y_MIN);
        if(0 == pPeachFlower->Rot_angle_PeriodByY)
            {
		SelfRot_angle = 0+((pPeachFlower->Rot_angle_max*s2d_sin_new(delta_y*3600))/65536);
            }
        else
            {
		SelfRot_angle = 0+((pPeachFlower->Rot_angle_max*s2d_sin_new(delta_y*3600/pPeachFlower->Rot_angle_PeriodByY))/65536);
            }
        if(0 == pPeachFlower->xzBase_PeriodByY)
            {
                if(0 == pPeachFlower->xzBase_Amp_denominator)
                    {
                        base_r = (pPeachFlower->xzBase_Amp_numerator*delta_y*((s2d_sin_new(delta_y*3600))>>8))>>8;//>>6 to avoid over range
                    }
                else
                    {
                        base_r = (pPeachFlower->xzBase_Amp_numerator*delta_y*((s2d_sin_new(delta_y*3600))>>8)/pPeachFlower->xzBase_Amp_denominator)>>8;//>>6 to avoid over range
                    }
            }
        else
            {
                if(0 == pPeachFlower->xzBase_Amp_denominator)
                    {
                        base_r = (pPeachFlower->xzBase_Amp_numerator*delta_y*((s2d_sin_new(delta_y*3600/pPeachFlower->xzBase_PeriodByY))>>8))>>8;//>>6 to avoid over range
                    }
                else
                    {
                        base_r = (pPeachFlower->xzBase_Amp_numerator*delta_y*((s2d_sin_new(delta_y*3600/pPeachFlower->xzBase_PeriodByY))>>8)/pPeachFlower->xzBase_Amp_denominator)>>8;//>>6 to avoid over range
                    }
            }
		//base_r = (pPeachFlower->xzBase_Amp_numerator*delta_y*((s2d_sin_new(delta_y*3600/pPeachFlower->xzBase_PeriodByY))>>8)/pPeachFlower->xzBase_Amp_denominator)>>8;//>>6 to avoid over range
		pPeachFlower->gravity_pos.x = pPeachFlower->xzBase_x+((base_r*s2d_cos_new(pPeachFlower->xzBase_angle))/65536);
		pPeachFlower->gravity_pos.z = pPeachFlower->xzBase_z;//+((base_r*s2d_sin_new(pPeachFlower->xzBase_angle))/65536);

		//  2.437	 3.937 0.172
		/*pPeachFlower->gravity_pos.x = 2437;
		pPeachFlower->gravity_pos.y = -3937;
		pPeachFlower->gravity_pos.z = -172;
		GetPeachFlowerVer(pPeachFlower,&(pPeachFlower->Rot_axis),s_i);*/
		
		GetPeachFlowerVer(pPeachFlower,&(pPeachFlower->Rot_axis),SelfRot_angle);

		//手机坐标计算
        BasePoint.x = 0;
        BasePoint.y = 0;
        BasePoint.z = 0;
		for(i = 0;i<PEACHFLOWER_VER_NUM;++i)
		{			
			#if IsFloatProc
				pPeachFlower->VertexInCamera[i].x = (pPeachFlower->PeachFlowerVer[i].x-CalcPoint->x)*65536;
				pPeachFlower->VertexInCamera[i].y = -1*(-1*pPeachFlower->PeachFlowerVer[i].y-CalcPoint->y)*65536;
				pPeachFlower->VertexInCamera[i].z = -1*(-1*pPeachFlower->PeachFlowerVer[i].z-CalcPoint->z)*65536;
				Get_QuaterninosTranslated(&(pPeachFlower->VertexInCamera[i]),&BasePoint,CalcQuat);
                if(0 != scale)
                    {
				pPeachFlower->VertexInCamera[i].x = (pPeachFlower->VertexInCamera[i].x)/scale + (eyePos->x<<16);
				pPeachFlower->VertexInCamera[i].y = (pPeachFlower->VertexInCamera[i].y)/scale + (eyePos->y<<16);
				pPeachFlower->VertexInCamera[i].z = (pPeachFlower->VertexInCamera[i].z)/scale + (eyePos->z<<16);
                    }
			#else
				CalcFlowerVer.x = pPeachFlower->PeachFlowerVer[i].x;
				CalcFlowerVer.y = -1*pPeachFlower->PeachFlowerVer[i].y;
				CalcFlowerVer.z = -1*pPeachFlower->PeachFlowerVer[i].z;

				GetAxisInMobilePhone(&(pPeachFlower->VertexInCamera[i]),
											&(CalcFlowerVer),
											scale,
											CalcPoint,
											CalcQuat,
											eyePos
											);

			#endif
			
		}
		
		if(IsPeachFlowerLanded(pPeachFlower))
		{
			pPeachFlower->IsActivated = FALSE;
		}
		else
		{		
			i=0;
				Get3DPetalFaceVer(i+1,pPeachFlower,&(peachFace[*faceNum]));
				*faceNum += 1;
			i=1;
				Get3DPetalFaceVer(i+1,pPeachFlower,&(peachFace[*faceNum]));
				*faceNum += 1;				
			i=2;
				Get3DPetalFaceVer(i+1,pPeachFlower,&(peachFace[*faceNum]));
				*faceNum += 1;
		}
	}

	if(!pPeachFlower->IsActivated)
	{
		peachFace[*faceNum].Peach3DVertex[0] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[1] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[2] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[3] = &FreeVertex;
		*faceNum += 1;
		peachFace[*faceNum].Peach3DVertex[0] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[1] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[2] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[3] = &FreeVertex;
		*faceNum += 1;
		peachFace[*faceNum].Peach3DVertex[0] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[1] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[2] = &FreeVertex;
		peachFace[*faceNum].Peach3DVertex[3] = &FreeVertex;
		*faceNum += 1;
	}

}


LOCAL void IMG_3D_Draw_float(	IMGREF_SIZE_T window,
						IMGREF_SIZE_T src_all_size,
						T_RECT src_rect,
						POINT_3D_0P3F In_3D_ver[4],	//base 4
						uint16 *src_ptr,
						uint16 *window_ptr,
						uint32 camera_z)
{
	//int a = 0;
	int i = 0;
	//int y,x;
	//FIX8 xEnd;
	//FIX8 Hs = 0;
	//FIX8 ySrc = 0;
	//int yOut;
	//register uint32 temp = 0;
	//FIX8 dx = 0;
	POINT_2D_FIX8 ver[5]={0};
	IMGREF_3D_POINT_T  eye;
	//int temp_xEnd = 0;
	//uint32 *In_Ptr = NULL;
	//uint16 *Out_Ptr = NULL;
	int32 constrain_disp = 0;
	//static int32 test = -100000;
    POINT_2D_FIX8 sub_ver[4] = {0};
	eye.x = window.w/2;
	eye.y = window.h/2;
	eye.z = (int16)(-1*(int32)(camera_z));
	constrain_disp = eye.z;//-400;//eye.z;
	//if((In_3D_ver[0].z>>16))
  if((In_3D_ver[0].z>>16)>constrain_disp
        &&(In_3D_ver[1].z>>16)>constrain_disp
        &&(In_3D_ver[2].z>>16)>constrain_disp
         &&(In_3D_ver[3].z>>16)>constrain_disp)
        {
        	for( i = 0 ; i < 4 ; i ++)
        	{
        		s3d_Free_Point_change_0P3FToFIX8_FLOAT(In_3D_ver[i],eye,&ver[i]);
        	}
           //*
           sub_ver[0].x = FIX8_SET(0);
	sub_ver[0].y = FIX8_SET(0);

	sub_ver[1].x = FIX8_SET(src_rect.w-1);
	sub_ver[1].y = FIX8_SET(0);

	sub_ver[2].x = FIX8_SET(src_rect.w-1);
	sub_ver[2].y = FIX8_SET(src_rect.h-1);

	sub_ver[3].x = FIX8_SET(0);
	sub_ver[3].y = FIX8_SET(src_rect.h-1);
            S3D_All_Axis_Interface_Type(window,
        						 src_all_size,
        						 sub_ver,
        						 &ver[0],				//顶点数据表指针
        						 window_ptr	,
        						 S3D_DATA_RGB5515,
        						 src_ptr,
        						 S3D_DATA_RGB5515,
        						 NULL);
		//*/
		
		//画框
        /*
        	{
                FIX16_POINT_T startPoint = {0};
            	    FIX16_POINT_T endPoint = {0};
                for(i = 0; i < 4;i++)
                {
                    startPoint.x = FIX8_INT(ver[i].x);
                    startPoint.y = FIX8_INT(ver[i].y);
                    endPoint.x = FIX8_INT(ver[(i+1)%4].x);
                    endPoint.y = FIX8_INT(ver[(i+1)%4].y);
			if(abs(startPoint.y-endPoint.y)>100
				&&(startPoint.x>120||endPoint.x>120)
				)
			{
				endPoint.y = FIX8_INT(ver[(i+1)%4].y);
			}
                	//DrawLineSegment(startPoint,endPoint,0xff00ff00,window,window_ptr);
			ShowContourLine565(startPoint,endPoint,0x07ff,window,window_ptr);
                }
            }
		*/
			
           /*
            for( i = 0 ; i < 4 ; i ++)
        	{
                	if(FIX8_INT(ver[i].y)>=0&&FIX8_INT(ver[i].y)<window.h&&FIX8_INT(ver[i].x)>=0&&FIX8_INT(ver[i].x)<window.w)
                	{
                	    window_ptr[FIX8_INT(ver[i].y)*window.w+FIX8_INT(ver[i].x)] = 0xffff;
                     }
            
        	}
            */
        }
}


LOCAL void PeachGardenFaceDraw(PEACHFACE_OBJ_T* peachFace, int32 faceNum, IMGREF_SIZE_T bg_window,COLOR16* pBG,IMGREF_3D_POINT_T eyePos,int32* calcBuf)
{
	IMGREF_SIZE_T src_all_size = {0};
	T_RECT src_rect = {0};
	POINT_3D_0P3F In_3D_ver[4] = {0};
	int32 i = 0,j = 0,index_pic = 0;
	int32* z_Value;
	int32* z_Sorted;
	int32 Calc_num_ctrl = faceNum;//(PEACHFACENUM+PEACHFLOWER_NUM*PEACHFLOWER_PETAL_NUM);//PEACHFACENUM
	int32 time;
	//BOOLEAN IsDoResetSort = FALSE;
	time = SCI_GetTickCount();

	//z_Value = (int32*)malloc(Calc_num_ctrl*sizeof(int32));
	//z_Sorted = (int32*)malloc(Calc_num_ctrl*sizeof(int32));

	z_Value = calcBuf;
	z_Sorted = calcBuf+Calc_num_ctrl;

if((z_Sorted[0] == z_Sorted[1]
	&&0 == z_Sorted[1]))
{
	//获取z值，将图片四个顶点z值的总和
	//(即均值，都除以4)去做z轴排序
		for(j=0;j<Calc_num_ctrl;++j)//for( index_pic = 0; index_pic < Calc_num_ctrl; index_pic++)
		{
			index_pic = j;
			//update_randStars(&g_aStar[i], target_buf_ptr, size);PeachFace[i].Peach3DVertex[j]
			z_Value[j] = peachFace[index_pic].Peach3DVertex[0]->z + peachFace[index_pic].Peach3DVertex[1]->z\
				+peachFace[index_pic].Peach3DVertex[2]->z + peachFace[index_pic].Peach3DVertex[3]->z;		
		}
		//初始化顺序指示数组
		for(j=0;j<Calc_num_ctrl;++j)
		{
			z_Sorted[j] = j;
		}
	//z轴排序

		Zaxis_sort(z_Value,Calc_num_ctrl,z_Sorted,TRUE);
}
else
{
	//获取z值，将图片四个顶点z值的总和
	//(即均值，都除以4)去做z轴排序
		for(j=0;j<Calc_num_ctrl;++j)//for( index_pic = 0; index_pic < Calc_num_ctrl; index_pic++)
		{
			index_pic = z_Sorted[j];
			//update_randStars(&g_aStar[i], target_buf_ptr, size);PeachFace[i].Peach3DVertex[j]
			z_Value[j] = peachFace[index_pic].Peach3DVertex[0]->z + peachFace[index_pic].Peach3DVertex[1]->z\
				+peachFace[index_pic].Peach3DVertex[2]->z + peachFace[index_pic].Peach3DVertex[3]->z;		
		}
	//z轴排序

		Zaxis_sort(z_Value,Calc_num_ctrl,z_Sorted,TRUE);
}
//SCI_TRACE_LOW("[NEST] calc Zaxis_sort %d",SCI_GetTickCount()-time);	

time = SCI_GetTickCount();
//根据排序后的结果先后绘图
	for(j=0;j<Calc_num_ctrl;++j)
	{
		index_pic = z_Sorted[j];
		
		for(i = 0; i < 4; ++i)
		{
			In_3D_ver[i].x = peachFace[index_pic].Peach3DVertex[i]->x;
			In_3D_ver[i].y = peachFace[index_pic].Peach3DVertex[i]->y;
			In_3D_ver[i].z = peachFace[index_pic].Peach3DVertex[i]->z;
		}		
		if( SORT_MIN_VAL == (In_3D_ver[0].z>>16)/*lint !e650*/
		        &&SORT_MIN_VAL == (In_3D_ver[1].z>>16)/*lint !e650*/
		        &&SORT_MIN_VAL == (In_3D_ver[2].z>>16)/*lint !e650*/
		         &&SORT_MIN_VAL == (In_3D_ver[3].z>>16))/*lint !e650*/
		{
			continue;
		}
			
		src_all_size.w = peachFace[index_pic].BranchRes->up_window.w;//pBranchObj->Img_Info[index_pic].BranchRes->up_window.w;
		src_all_size.h = peachFace[index_pic].BranchRes->up_window.h;//pBranchObj->Img_Info[index_pic].BranchRes->up_window.h;
		src_rect.w = src_all_size.w;
		src_rect.h = src_all_size.h;
		src_rect.x = 0;
		src_rect.y = 0;	

		IMG_3D_Draw_float(bg_window,
						src_all_size,
						src_rect,
						In_3D_ver,	//base 4
						peachFace[index_pic].BranchRes->srcARGB_Map,//pBranchObj->Img_Info[index_pic].BranchRes->srcARGB_Map,
						pBG,
						-eyePos.z);
	}
//SCI_TRACE_LOW("[NEST] calc for---IMG_3D_Draw_float %d",SCI_GetTickCount()-time);	
	//free(z_Value);
	//free(z_Sorted);
}

PUBLIC void SetPeachGardenResOrder(RES_TYPE_3D* PeachGardenlambertRes)
{
	int32 i = 0;
	for(i = 0; i < PEACHFACENUM; ++i)
	{
		PeachGardenFaceOrder[i] = GetResFromLambertName(Nest_MayaFace[i].MaterialName,PeachGardenlambertRes);
	}
}
PUBLIC void PeachGardenCalc(int32 Gapframe, 
						int32 GaptotalFrame, 
						int32 KeyFrame, 
						COLOR16* BG_ResBuf, 
						RES_TYPE_3D* PeachGardenlambertRes,
						PEACHFLOWER_RES_T* PeachFlowerRes,
						IMGREF_SIZE_T* pWindowSize, 
						COLOR16* TargetBuf,
						int32* calcBuf)
{
	//设置背景
	int32 i = 0, y = 0, Len = 0, Real2PixelScale = 0,totalfaceNum = 0;
	
	IMGREF_3D_POINT_T eyePos = {0};
	
	POINT_3D_0P3F CalcPoint = {0};		
	//POINT_3D_0P3F CalcPoint_std = {0};	
	//POINT_3D_0P3F CalcPoint_end = {0};	
	QUATERNIONS_T Quat_do = {0};
	//QUATERNINOS_T Quat_std = {0};
	//QUATERNINOS_T Quat_end = {0};
	//IMGREF_3D_POINT_T calc_angle ={0};
	//COLOR16* pSrc = NULL;
	//COLOR16* pDst = NULL;

	int32 time;

	//获得物体缩放比例
		Real2PixelScale = SCALE_MAYA2PHONE;//(CAMERA_VIEWFINDER_W+CAMERA_VIEWFINDER_H)/(240+320);//(320+480);//(bg_window.w+bg_window.h);
//调试得到CameraFocusLen参数，即眼睛位置
		eyePos.x = pWindowSize->w/2;
		eyePos.y = pWindowSize->h/2;
		eyePos.z = -1*CameraFocusLen/Real2PixelScale;//
		if(GaptotalFrame<=0)
                return;
            if(GaptotalFrame<Gapframe)
                return;
		
time = SCI_GetTickCount();
		Len = pWindowSize->h*pWindowSize->w*sizeof(COLOR16);
		y = ((GaptotalFrame-Gapframe)*PeachGardenBG_Pos[KeyFrame].y+Gapframe*PeachGardenBG_Pos[KeyFrame+1].y)/GaptotalFrame;
		SCI_MEMCPY(TargetBuf,&BG_ResBuf[y*pWindowSize->w],Len);
		//y = ((GaptotalFrame-Gapframe)*PeachGardenBG_Pos[KeyFrame].y+Gapframe*PeachGardenBG_Pos[KeyFrame+1].y)/GaptotalFrame;
		//memcpy(TargetBuf,(&BG_ResBuf[y*pWindowSize->w]), pWindowSize->h*pWindowSize->w*sizeof(COLOR16));
//SCI_TRACE_LOW("[NEST] calc SCI_MEMCMP %d",SCI_GetTickCount()-time);

time = SCI_GetTickCount();
#if 0
		//插值过程
		calc_angle.x = -1*KeyFrame_CameraAngle[KeyFrame].x;
		calc_angle.y = KeyFrame_CameraAngle[KeyFrame].y;
		calc_angle.z = KeyFrame_CameraAngle[KeyFrame].z;
		Quat_std = GetQuaterninosFromEuler(&calc_angle);
		calc_angle.x = -1*KeyFrame_CameraAngle[KeyFrame+1].x;
		calc_angle.y = KeyFrame_CameraAngle[KeyFrame+1].y;
		calc_angle.z = KeyFrame_CameraAngle[KeyFrame+1].z;
		Quat_end = GetQuaterninosFromEuler(&calc_angle);
		QuaterninosSlerp(GaptotalFrame,Gapframe,&Quat_std,&Quat_end,&Quat_do);	
		CalcPoint_std = KeyFrame_CameraPos[KeyFrame];
		CalcPoint_end = KeyFrame_CameraPos[KeyFrame+1];
		CalcPoint.x = ((GaptotalFrame-Gapframe)*CalcPoint_std.x + Gapframe*CalcPoint_end.x)/GaptotalFrame;
		CalcPoint.y = ((GaptotalFrame-Gapframe)*CalcPoint_std.y + Gapframe*CalcPoint_end.y)/GaptotalFrame;
		CalcPoint.z = ((GaptotalFrame-Gapframe)*CalcPoint_std.z + Gapframe*CalcPoint_end.z)/GaptotalFrame;
#else
		Quat_do = Nest_CameraState[KeyFrame][Gapframe].CameraDirection;
		CalcPoint = Nest_CameraState[KeyFrame][Gapframe].TranslatePos;
#endif

		//顶点计算
		totalfaceNum = 0;
		PeachVerCalc(&eyePos,Real2PixelScale,&CalcPoint,&Quat_do,PeachGardenlambertRes);
		totalfaceNum = PEACHFACENUM;		
//SCI_TRACE_LOW("[NEST] calc PeachVerCalc_total %d",SCI_GetTickCount()-time);	

time = SCI_GetTickCount();
		for(i=0;i<PEACHFLOWER_NUM;++i)
		{
			PeachFlowerFaceCalc(&(PeachFlower_3D[i]),PeachFlowerRes,&eyePos,Real2PixelScale,&CalcPoint,&Quat_do,PeachFace,&totalfaceNum);
		}
//SCI_TRACE_LOW("[NEST] calc PeachFlowerFaceCalc %d",SCI_GetTickCount()-time);	

time = SCI_GetTickCount();
		//根据z值大小先后顺序绘图		
		PeachGardenFaceDraw(PeachFace,totalfaceNum,*pWindowSize,TargetBuf,eyePos,calcBuf);
//SCI_TRACE_LOW("[NEST] calc PeachGardenFaceDraw %d",SCI_GetTickCount()-time);	


}

