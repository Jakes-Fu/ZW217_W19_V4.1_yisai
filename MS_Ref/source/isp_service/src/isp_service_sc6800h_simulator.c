#include "os_api.h"
#include "isp_service.h"

/**------------------------------------------------------------------------------------*
 **                         ENUM define                                		       *
 **------------------------------------------------------------------------------------*/
typedef enum
{
    ISP_OP_READ=0x01,		
    ISP_OP_WRITE,
    ISP_OP_READ_WRITE,
    ISP_OP_MAX
}ISP_OP_MODE_E;

PUBLIC uint32 ISP_Align(uint32 pLen, uint32 align_coeff, uint32 align_len)
{
    uint32 align_data=pLen+align_coeff;
    
    align_data=((align_data>>align_len)<<align_len);

    return align_data;
}

PUBLIC void  ISP_ServiceGetMutex(void)
{
}

PUBLIC void  ISP_ServicePutMutex(void)
{
}

PUBLIC void ISP_ServiceInit(void)
{
}

PUBLIC void ISP_ServiceOpen(void)
{
}
PUBLIC void ISP_ServiceClose (void)
{
}

PUBLIC int32 ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ID_E id, int (*func)())
{
    return SCI_SUCCESS;
}
PUBLIC uint32 ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    return SCI_SUCCESS;
}

PUBLIC uint32 ISP_ServiceSetCaptureParam(ISP_JPEG_PARAM_T *param_ptr)
{
    return SCI_SUCCESS;
}

PUBLIC uint32 ISP_ServiceGetCaptureData(ISP_CAPTRUE_DATA_T *data_info_ptr)
{
    return SCI_SUCCESS;
}

PUBLIC uint32 ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T *param_ptr)
{   
    return SCI_SUCCESS;
}

PUBLIC uint32 ISP_ServiceStartReview(void)
{
}

PUBLIC uint32  ISP_ServiceSetScaleParam(ISP_SCALE_PARAM_T *param_ptr)
{
    return SCI_SUCCESS;
}

PUBLIC uint32  ISP_ServiceStartScale(void)
{
    return SCI_SUCCESS;
}

PUBLIC uint32  ISP_ServiceSetMjpegParam(ISP_MJPEG_PARAM_T *param_ptr)
{
    return SCI_SUCCESS;
}

PUBLIC uint32  ISP_ServiceStartMjpeg(void)
{	
    return SCI_SUCCESS;
}
