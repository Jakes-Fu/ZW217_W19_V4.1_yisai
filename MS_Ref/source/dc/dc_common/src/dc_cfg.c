/******************************************************************************
 ** File Name:      dc_cfg.c                                           *
 ** Author:         Tim.zhu                                             *
 ** DATE:           11/16/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product configure dc parameter    *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/16/2009     Tim.zhu	  Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "ms_ref_dc_trc.h"
#include "os_api.h"
#include "dal_time.h"
#include "dc_cfg.h"
#include "sensor_drv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL const  DCAMERA_SIZE_T s_dc_sensor_size[]={
    {80,  80},
    {128, 160},
    {160, 128},
    {176, 220},
    {220, 176},
    {240, 320},
    {320, 240},
    {352, 288},
    {240, 400},
    {400, 240},
    {320, 480},
    {480, 320},
    {480, 640},
    {640, 480},
    {720, 480},
    {720, 576},
    {800, 600},
    {1024, 768},
    {960, 1280},
    {1280, 960},
    {1600, 1200},
    {2048, 1536},
    {2592, 1944},
    {3264, 2448}
};

LOCAL EXIF_GPS_INFO_T s_dc_gps_info = {0};
LOCAL EXIF_SPEC_DATE_TIME_T image_date_time = {0};
LOCAL EXIF_SPECIFIC_INFO_T s_dc_specific_info={0x00};
LOCAL JINF_EXIF_INFO_T s_dc_exif_info={0x00};
LOCAL EXIF_SPEC_OTHER_T s_dc_spec_other = {0};

/*lint -e{785}*/
LOCAL EXIF_PRI_DATA_STRUCT_T exif_prim_data = 
{
    1, //just Orientation valid
    1, //The 0th row is at the visual top of the image, and the 0th column is the visual left-hand side
    0
};

LOCAL const JINF_EXIF_INFO_T* s_dc_exif_info_ptr=&s_dc_exif_info;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  This function is used to get exif spec basic Param     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL void _DC_SetExifSpecificBasicParameter(void)
{
    EXIF_SPECIFIC_INFO_T* dc_specific_info_ptr=&s_dc_specific_info;

    dc_specific_info_ptr->basic.ColorSpace=1;
    dc_specific_info_ptr->basic.ComponentsConfiguration[0]=1;
    dc_specific_info_ptr->basic.ComponentsConfiguration[1]=2;
    dc_specific_info_ptr->basic.ComponentsConfiguration[2]=3;
    dc_specific_info_ptr->basic.ComponentsConfiguration[3]=0;
}

/*****************************************************************************/
//  Description:  This function is used to get exif spec user Param     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL void _DC_SetExifSpecificUserParameter(void)
{
    DC_PRODUCT_CFG_FUNC_TAB_T_PTR exif_ptr=DC_GetDcProductCfgFun();
    EXIF_SPECIFIC_INFO_T* dc_specific_info_ptr=&s_dc_specific_info;

    if(PNULL!=exif_ptr->get_exifspecuser)
    {
        dc_specific_info_ptr->user_ptr=(EXIF_SPEC_USER_T*)exif_ptr->get_exifspecuser(0x00);
    }
    else
    {
        dc_specific_info_ptr->user_ptr=NULL;
    }
}

/*****************************************************************************/
//  Description:  This function is used to get spec picture taking Param     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL void _DC_SetExifSpecificPicTakingParameter(void)
{
    EXIF_SPEC_PIC_TAKING_COND_T* img_sensor_exif_ptr=Sensor_GetSensorExifInfo();
    EXIF_SPECIFIC_INFO_T* dc_specific_info_ptr=&s_dc_specific_info;

    dc_specific_info_ptr->pic_taking_cond_ptr=img_sensor_exif_ptr;
}

/*****************************************************************************/
//  Description:  This function is used to get exif primary Param     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL void _DC_SetExifPrimaryParameter(void)
{
    JINF_EXIF_INFO_T* dc_exif_info_ptr=(JINF_EXIF_INFO_T* )s_dc_exif_info_ptr;
    DC_PRODUCT_CFG_FUNC_TAB_T_PTR exif_ptr=DC_GetDcProductCfgFun();

    dc_exif_info_ptr->primary.basic.YCbCrPositioning=1;
    dc_exif_info_ptr->primary.basic.XResolution.numerator=72;
    dc_exif_info_ptr->primary.basic.XResolution.denominator=1;
    dc_exif_info_ptr->primary.basic.YResolution.numerator=72;
    dc_exif_info_ptr->primary.basic.YResolution.denominator=1;
    dc_exif_info_ptr->primary.basic.ResolutionUnit=2;
    exif_prim_data.valid.Orientation = 1;
    exif_prim_data.Orientation = 1;
    dc_exif_info_ptr->primary.data_struct_ptr = &exif_prim_data;

    if(PNULL!=exif_ptr->get_exifprimarypridesc)
    {
        dc_exif_info_ptr->primary.img_desc_ptr=(EXIF_PRI_DESC_T*)exif_ptr->get_exifprimarypridesc(0x00);
    }
    else
    {
        dc_exif_info_ptr->primary.img_desc_ptr=NULL;
    }
    
}

/*****************************************************************************/
//  Description:  This function is used to get exif spec Param     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL void _DC_SetExifSpecificParameter(void)
{
    JINF_EXIF_INFO_T* dc_exif_info_ptr=(JINF_EXIF_INFO_T* )s_dc_exif_info_ptr;
    SCI_DATE_T       cur_date = {0};
    SCI_TIME_T       cur_time = {0};

    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);

    dc_exif_info_ptr->spec_ptr=&s_dc_specific_info;

    _DC_SetExifSpecificBasicParameter();
    _DC_SetExifSpecificUserParameter();
    _DC_SetExifSpecificPicTakingParameter();
    s_dc_spec_other.valid.ImageUniqueID = 0;
    sprintf((char*)s_dc_spec_other.ImageUniqueID,
                "SAMSUNG ANYCALL IMAGE%04d:%02d:%02d %02d:%02d:%02d",            
                cur_date.year, 
                cur_date.mon, 
                cur_date.mday,
                cur_time.hour, 
                cur_time.min,
                cur_time.sec);//overrun 33bytes,so decrease one byte space
    
    //SCI_TRACE_LOW:"_DC_SetExifSpecificParameter %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CFG_183_112_2_18_1_23_57_294,(uint8*)"s",s_dc_spec_other.ImageUniqueID);
    dc_exif_info_ptr->spec_ptr->other_ptr = &s_dc_spec_other;
    
}

/*****************************************************************************/
//  Description:  This function is used to get gps Param     
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL void _DC_SetExifGpsParameter(void)
{
    JINF_EXIF_INFO_T* dc_exif_info_ptr=(JINF_EXIF_INFO_T* )s_dc_exif_info_ptr;
#if 0
    s_dc_gps_info.GPSVersionID[0] = 2;
    s_dc_gps_info.GPSVersionID[1] = 2;
    s_dc_gps_info.GPSVersionID[2] = 0;
    s_dc_gps_info.GPSVersionID[3] = 0;    
    *(uint32*)&s_dc_gps_info.valid = (uint32)0x7F;
    s_dc_gps_info.GPSLatitudeRef[0] = 'N';
    s_dc_gps_info.GPSLatitude[0].numerator = 31;
    s_dc_gps_info.GPSLatitude[0].denominator = 1;    
    s_dc_gps_info.GPSLatitude[1].numerator = 2;
    s_dc_gps_info.GPSLatitude[1].denominator = 1;    
    s_dc_gps_info.GPSLatitude[2].numerator = 0;
    s_dc_gps_info.GPSLatitude[2].denominator = 1;    

    s_dc_gps_info.GPSLongitudeRef[0] = 'E';
    s_dc_gps_info.GPSLongitude[0].numerator = 121;
    s_dc_gps_info.GPSLongitude[0].denominator = 1;    
    s_dc_gps_info.GPSLongitude[1].numerator = 4;
    s_dc_gps_info.GPSLongitude[1].denominator = 1;    
    s_dc_gps_info.GPSLongitude[2].numerator = 0;
    s_dc_gps_info.GPSLongitude[2].denominator = 1;    

    s_dc_gps_info.GPSAltitudeRef = 0;
    s_dc_gps_info.GPSAltitude.numerator = 4;
    s_dc_gps_info.GPSAltitude.denominator = 1;
  #endif
    dc_exif_info_ptr->gps_ptr=NULL;//&s_dc_gps_info;
}

PUBLIC void DC_SetExifImagePixel(uint32 width, uint32 height)
{
    EXIF_SPECIFIC_INFO_T* dc_specific_info_ptr=&s_dc_specific_info;

    dc_specific_info_ptr->basic.PixelXDimension = width;
    dc_specific_info_ptr->basic.PixelYDimension = height;    
}

PUBLIC void DC_SetExifImageDataTime(void)
{
    EXIF_SPECIFIC_INFO_T* dc_specific_info_ptr=&s_dc_specific_info;
    SCI_DATE_T                   cur_date = {0};
    SCI_TIME_T                    cur_time = {0};

    TM_GetSysDate(&cur_date);
    TM_GetSysTime(&cur_time);
    
    sprintf((char *)image_date_time.DateTimeOriginal, 
                "%04d:%02d:%02d %02d:%02d:%02d", 
                cur_date.year, 
                cur_date.mon, 
                cur_date.mday,
                cur_time.hour, 
                cur_time.min,
                cur_time.sec);
    strcpy((char*)image_date_time.DateTimeDigitized, (char*)image_date_time.DateTimeOriginal); /*lint !e628 -e628*/
    image_date_time.valid.DateTimeOriginal = 1;
    image_date_time.valid.DateTimeDigitized = 1;

    //SCI_TRACE_LOW:"DC_SetExifImageDataTime %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CFG_252_112_2_18_1_23_58_295,(uint8*)"s",image_date_time.DateTimeOriginal);

    dc_specific_info_ptr->date_time_ptr = &image_date_time;

}
/*****************************************************************************/
//  Description:  This function is used to get exif Param    
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC JINF_EXIF_INFO_T* DC_GetExifParameter(void)
{
    _DC_SetExifPrimaryParameter();
    _DC_SetExifSpecificParameter();
    _DC_SetExifGpsParameter();
    
    //SCI_TRACE_LOW:"DC_GetExifParameter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_CFG_266_112_2_18_1_23_58_296,(uint8*)"");
    return (JINF_EXIF_INFO_T* )s_dc_exif_info_ptr;
}

/******************************************************************************
// Description: get the size struct  pointer of specific type
// Author:      Tim.zhu
// Input:       uint32 type: img size
// Output:      none
// Return:      sensor size struct pointer
******************************************************************************/ 
PUBLIC DCAMERA_SIZE_T* DC_GetSensorSizeStPtr(uint32 type)
{
    return (DCAMERA_SIZE_T *)(&s_dc_sensor_size[type]);
}

/*****************************************************************************/
//  Description:  This function is used to get dc Param    
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC DC_PRODUCT_CFG_T_PTR DC_GeProductCfgPtr(void)
{
    DC_PRODUCT_CFG_FUNC_TAB_T_PTR dc_productcfgfunptr=DC_GetDcProductCfgFun();
    DC_PRODUCT_CFG_T_PTR dc_productcfgptr=NULL;

    if(PNULL!=dc_productcfgfunptr->get_productcfg)
    {
        dc_productcfgptr=(DC_PRODUCT_CFG_T_PTR)dc_productcfgfunptr->get_productcfg(0x00);
    }
    
    return dc_productcfgptr;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
