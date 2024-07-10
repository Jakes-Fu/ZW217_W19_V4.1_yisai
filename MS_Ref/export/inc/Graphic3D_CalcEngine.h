#ifndef __GRAPHICS3D_CALCENGINE_H__
#define __GRAPHICS3D_CALCENGINE_H__

#include "graphics_3d_rotate.h" 

PUBLIC void Vector_Normalize(IMGREF_3D_POINT_T *ori_vector, IMGREF_3D_POINT_T *normalized_vector);
PUBLIC QUATERNIONS_T GetQuaterninosFromEuler(IMGREF_3D_POINT_T* angle3D);
PUBLIC void Quaterninos_Multiplicate_float(QUATERNIONS_FLOAT_T* quat1,QUATERNIONS_FLOAT_T* quat2,QUATERNIONS_FLOAT_T* quat_Out);
PUBLIC void QuaterninosSlerp(int32 totalFrame, int32 cur_Frame,QUATERNIONS_T* Quat_std, QUATERNIONS_T* Quat_end, QUATERNIONS_T* Quat_out);
PUBLIC void Quaterninos_Multiplicate(QUATERNIONS_T* quat1,QUATERNIONS_T* quat2,QUATERNIONS_T* quat_Out);
PUBLIC void Get_QuaterninosTranslated(POINT_3D_0P3F* OriPoint,POINT_3D_0P3F* Rot_pos,QUATERNIONS_T* Rot_Quat);
PUBLIC void Get_QuaterninosTranslated_float(POINT_3D_FLOAT* OriPoint,POINT_3D_FLOAT* Rot_pos,QUATERNIONS_FLOAT_T* Rot_Quat);
PUBLIC void Vector_MultiplicateCross_Normalize(IMGREF_3D_POINT_T *vector1, IMGREF_3D_POINT_T *vector2,IMGREF_3D_POINT_T *normalized_vector,BOOLEAN isVectorNormalized);
PUBLIC void Zaxis_sort(int32* z_value,int32 Len,int32* sort_index,BOOLEAN IsBigEnding);
PUBLIC void ShowContourLine565(
	FIX16_POINT_T startPoint,
	FIX16_POINT_T endPoint,
	COLOR16 colorVal,
	IMGREF_SIZE_T window,
	COLOR16* outbuf);
#endif
