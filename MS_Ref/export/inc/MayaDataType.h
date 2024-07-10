#ifndef __MAYADATATYPE_H__
#define __MAYADATATYPE_H__

#include "graphics_3d_rotate.h" 


typedef struct
{
char GroupName[128];
char MaterialName[128];
int32 VertexIndex[4];
int32 VertexUV[4];
}MAYAFACE_OBJ_T;

typedef struct
{
int32 Vertex_U;
int32 Vertex_V;
}MAYA_UV_T;

typedef struct
{
POINT_3D_0P3F TranslatePos;
QUATERNIONS_T CameraDirection;
}SLERPDATA_T;

#endif