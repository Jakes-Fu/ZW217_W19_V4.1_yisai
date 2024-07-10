#ifndef __MAYAPEACHGARDENDATAINC_H__
#define __MAYAPEACHGARDENDATAINC_H__

#include "MayaDataType.h" 

#define PEACHVERTEXNUM 343
#define PEACHFACENUM 223
#define PEACHVERTEXUVNUM 892
#define NEST_LAMBERT_RESNUM 89

extern const POINT_3D_0P3F Peach3DVertex[PEACHVERTEXNUM];

extern const MAYAFACE_OBJ_T Nest_MayaFace[PEACHFACENUM];

extern const MAYA_UV_T Nest_MayaUV[PEACHVERTEXUVNUM];

extern const char PeachGardenOrder[NEST_LAMBERT_RESNUM][128];

#endif