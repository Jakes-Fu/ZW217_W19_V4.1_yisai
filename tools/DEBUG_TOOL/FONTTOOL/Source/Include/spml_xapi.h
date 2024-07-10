#ifndef _SPML_XAPI_H__
#define _SPML_XAPI_H__

#include "spml_def.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef __cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
int32 X_GrayRasterRender( 
						 const SPML_RASTER_PARAMS_T*  params
						 );


int32 X_OutlineGetBBox( 
					   SPML_OUTLINE_T*  outline,  
					   SPML_BBOX_T  *abbox,
					   SPML_BBOX_T  *acbox
					   );

void X_OutlineTranslate( 
						const SPML_OUTLINE_T*  outline,
						SPML_POS_T        xOffset,
						SPML_POS_T        yOffset
						);
void X_OutlineTransform( 
						const SPML_OUTLINE_T*  outline,
						const SPML_MATRIX_T*   matrix
						);

void X_VectorTransform( 
					   SPML_VECTOR_T*        vector,
					   const SPML_MATRIX_T*  matrix
					   );
void X_AddGlyphOutline( 
					   SPML_OUTLINE_SPEC_T* dst_ptr,
					   const SPML_OUTLINE_T*  src_ptr
					   );
void X_TranslateArray( 
					  int     n,
					  SPML_VECTOR_T*  coords,
					  int      delta_x,
					  int      delta_y
					  );

void X_OutlineAutohinter(
						 SPML_OUTLINE_T* outline_ptr,
						 SPML_GLYPH_IMAGE_T* image_ptr,
						 double				rate
						 );
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
	}
#endif

#endif // _SPML_XAPI_H__
