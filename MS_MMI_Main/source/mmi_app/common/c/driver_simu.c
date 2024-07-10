/*******************************************************************************
 ** FileName:      mmidc_extern_simu.c                                             *
 ** Author:        jibin                                       *
 ** Date:          2006/7/24                                                   *
 ** CopyRight:     2006, Spreadtrum, Incoporated, All right reserved            *
 ** Description:   define mmidc extern interface                                       *
 *******************************************************************************
 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2006/7/24      jibin      Created                          *
 *******************************************************************************/

#define DRIVER_SIMU_C

#ifdef __cplusplus
	extern "C"
	{
#endif

#include "std_header.h"
//#include "sci_types.h"
//#include "os_api.h"
//#include "ffs.h"
#include "mmk_type.h"
//#include "mmifmm_export.h"
//#include "dal_dcamera.h"


//PUBLIC DCAMERA_RETURN_VALUE_E DCAMERA_SetAutoFocus(DCAMERA_AF_INFO_T const * af_info_ptr);                      
uint32 MergeExif_app(    //Return: Size of Exif file ; if  return == 0, main or Thunmbail jpeg error
                            uint8 * pBuf,             //Buffer save the EXIF;
                            uint32 Thumboffset,       //Thunboffset: thumb offset from the start of pBuf;
                            uint32 Thunmbsize,        //Thumbsize: size of Thunmbnail image;
                            uint32 Mainoffset,        //Mainoffset: main offset from the start of pBuf;
                            uint32 Mainsize,		  //Mainsize: size of Main image;
                            uint8 MergeType           //MergeType : 0: Spreadtrum image; 1: External image
                            )
{
	return 0;
}
uint8 SplitExif_app( //return 0: not Exif return 1: Exif
	                        uint8 * pBuf,             //pBuf: Image buffer address
	                        uint32 imgSize,           //imgSize: Image Size;
	                        uint32 *pThumboffset,     // pThumboffset: Thumb image offset from pBuf;
	                        uint32 *pThunmbsize,      //pThunmbsize: Thumb image size;
	                        uint32 *pMainoffset,      // pMainoffset: Main image offset from pBuf;
	                        uint32 *pMainsize         // pMainsize: Main image size;
                            )
{
	return 0;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif