
#include "zfb_app.h"

// Color of bmp pixels  
#define ZFB_PIXEL_COLOR_R               0        
#define ZFB_PIXEL_COLOR_G               0
#define ZFB_PIXEL_COLOR_B               0 //0xff
#define ZFB_BI_RGB                      0L

#define ZFB_SYSTEM_START_YEAR    (1970)//TENCENT系统的起始年份

typedef struct 
{
    unsigned short 	wYear;
    unsigned short	nMonth;
    unsigned short	nDay;
    unsigned short	nHour;
    unsigned short	nMin;
    unsigned short	nSec;
}ZFBTime;

//#pragma pack(push, 2) //2字节对齐，不然会出问题
typedef struct
{
    uint16 bfType;
    uint16  bfSize;
    uint16  bfSize2; 
    uint16 bfReserved1;
    uint16 bfReserved2;
    uint16 bfOffBits;
    uint16 bfOffbits2;
} ZFB_BMPFILEHEADER;

typedef struct
{
  unsigned long  biSize;          /*Number of bytes required by the structure. */
  unsigned long  biWidth;         /*Width of the bitmap, in pixels. */
  int32  biHeight;        /*Height of the bitmap, in pixels. If biHeight is positive, the bitmap is a bottom-up DIB and its origin is the lower-left corner. If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.
                            If biHeight is negative, indicating a top-down DIB, */
  uint16   biPlanes;        /*number of planes for the target device. This value must be set to 1. */
  uint16   biBitCount;      /*bits-per-pixel. */
  unsigned long  biCompression;   /*Type of compression for a compressed bottom-up bitmap (top-down DIBs cannot be compressed)*/
  unsigned long  biSizeImage;     /*size, in bytes, of the image*/
  unsigned long  biXPelsPerMeter; /*horizontal resolution*/
  unsigned long  biYPelsPerMeter; /*Vertical Resolution*/
  unsigned long  biClrUsed;       /*Number of color indexes in the color table that are actually used by the bitmap*/
  unsigned long  biClrImportant;  /*Number of color indexes that are required for displaying the bitmap*/	
} ZFB_BMPINFOHEADER;


BOOLEAN ZFB_SIM_Exsit(void)
{
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;

    sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
    if(sim_ok_num ==0)
    {
        return FALSE;
    }
    return TRUE;
}

int ZFB_2VM_ConvertTo_Bmp(uint8 * qData,uint32 DataLen,uint16 zoom_vol,uint8 ** ppImg, uint32 *pLen)
{
    unsigned int unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
    unsigned char* pRGBData, *pSourceData, *pDestData;
    uint8 * p_img = NULL;
//    FILE* f;
    ZFB_BMPFILEHEADER kFileHeader;
    ZFB_BMPINFOHEADER kInfoHeader;	
    //fs_rsp_msg_type rsp;
    uint32  bmp_buf_len = 0;
    uint32 head_len = 0;
    uint32 info_len = 0;
    uint32 handletmp;	
    int a=0;  
    uint8 cur_data = 0;
    uint8 cur_bit = 0;
    uint8 cur_val = 0;
    
    //矩阵的维数
    head_len = sizeof(ZFB_BMPFILEHEADER);
    info_len = sizeof(ZFB_BMPINFOHEADER);
    unWidth = DataLen;
    unWidthAdjusted = unWidth * zoom_vol * 3;
    if (unWidthAdjusted % 4)
    {
        unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
    }
    unDataBytes = unWidthAdjusted * unWidth * zoom_vol;
    bmp_buf_len = head_len+info_len+unDataBytes;
    if (!(pRGBData = (unsigned char*)SCI_ALLOC_APP(unDataBytes)))
    {
        return 1;
    }

    // Preset to white
    SCI_MEMSET(pRGBData, 0xff, unDataBytes);

    // Prepare bmp headers
    SCI_MEMSET(&kFileHeader, 0, sizeof(ZFB_BMPFILEHEADER));
    kFileHeader.bfType = 0x4d42;  // "BM"
    kFileHeader.bfSize = bmp_buf_len;
    kFileHeader.bfSize2 = bmp_buf_len>>16;
    kFileHeader.bfReserved1 = 0;
    kFileHeader.bfReserved2 = 0;
    kFileHeader.bfOffBits = (uint16)(sizeof(ZFB_BMPFILEHEADER) + sizeof(ZFB_BMPINFOHEADER));
    kFileHeader.bfOffbits2 =0;

    SCI_MEMSET(&kInfoHeader, 0, sizeof(ZFB_BMPINFOHEADER));
    kInfoHeader.biSize = sizeof(ZFB_BMPINFOHEADER);
    kInfoHeader.biWidth = unWidth * zoom_vol;
    kInfoHeader.biHeight = ((int)unWidth * zoom_vol);
    kInfoHeader.biPlanes = 1;
    kInfoHeader.biBitCount = 24;
    kInfoHeader.biCompression = ZFB_BI_RGB;
    kInfoHeader.biSizeImage = 0;
    kInfoHeader.biXPelsPerMeter = 0;
    kInfoHeader.biYPelsPerMeter = 0;
    kInfoHeader.biClrUsed = 0;
    kInfoHeader.biClrImportant = 0;

    // Convert QrCode bits to bmp pixels
    pSourceData = qData;
    for (y = 0; y < unWidth; y++)
    {
        pDestData = pRGBData + unWidthAdjusted * y * zoom_vol;
        for (x = 0; x < unWidth; x++)
        {
            a = y * unWidth + x;
            cur_data = pSourceData[a / 8];
            cur_bit = a%8;
            cur_bit = 7 - cur_bit;
            cur_val = cur_data & (0x01 << cur_bit);
            if (cur_val)
            {
                for (l = 0; l < zoom_vol; l++)
                {
                    for (n = 0; n < zoom_vol; n++)
                    {
                        *(pDestData + n * 3 + unWidthAdjusted * l) = ZFB_PIXEL_COLOR_B;
                        *(pDestData + 1 + n * 3 + unWidthAdjusted * l) = ZFB_PIXEL_COLOR_G;
                        *(pDestData + 2 + n * 3 + unWidthAdjusted * l) = ZFB_PIXEL_COLOR_R;
                    }
                }
            }
            pDestData += 3 * zoom_vol;
        }
    }
    p_img = SCI_ALLOC_APP(bmp_buf_len);
    if(p_img == NULL)
    {
        SCI_FREE(pRGBData);
        return 1;
    }
    SCI_MEMCPY(p_img,(uint8 *)&kFileHeader,head_len);
    SCI_MEMCPY(p_img+head_len,(uint8 *)&kInfoHeader,info_len);
    SCI_MEMCPY(p_img+head_len+info_len,(uint8 *)pRGBData,unDataBytes);
    * ppImg = p_img;
    *pLen = bmp_buf_len;
    SCI_FREE(pRGBData);

    return 0;
}

int ZFB_BarCode_ConvertTo_Bmp(uint16* pBarCode,uint32 BarCodeLen,uint16 zoom_vol,uint8 ** ppImg, uint32 *pLen)
{
    unsigned int unWidth, unHeiht,x, y, l, n, unWidthAdjusted, unDataBytes;
    unsigned char* pRGBData,  *pDestData;
    uint16 *pSourceData;
    uint8 * p_img = NULL;
//    FILE* f;
    ZFB_BMPFILEHEADER kFileHeader;
    ZFB_BMPINFOHEADER kInfoHeader;	
    //fs_rsp_msg_type rsp;
    uint32  bmp_buf_len = 0;
    uint32 head_len = 0;
    uint32 info_len = 0;
    uint32 handletmp;	
    int a=0;  
    uint16 cur_data = 0;
    uint16 cur_bit = 0;
    uint16 cur_val = 0;
    uint32 pixlen;
    
    pixlen = BarCodeLen*11+1;
    if(1)
    {
        //矩阵的维数
        head_len = sizeof(ZFB_BMPFILEHEADER);
        info_len = sizeof(ZFB_BMPINFOHEADER);
        unHeiht  = pixlen/2;
        unWidth = pixlen;
        unWidthAdjusted = unWidth * zoom_vol * 3;
        if (unWidthAdjusted % 4)
        {
            unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
        }
        unDataBytes = unWidthAdjusted * unHeiht * zoom_vol;
        bmp_buf_len = head_len+info_len+unDataBytes;
        if (!(pRGBData = (unsigned char*)SCI_ALLOC_APP(unDataBytes)))
        {
            return 1;
        }

        // Preset to white
        SCI_MEMSET(pRGBData, 0xff, unDataBytes);

        // Prepare bmp headers
	 SCI_MEMSET(&kFileHeader, 0, sizeof(ZFB_BMPFILEHEADER));
        kFileHeader.bfType = 0x4d42;  // "BM"
	kFileHeader.bfSize = bmp_buf_len;
	kFileHeader.bfSize2 = bmp_buf_len>>16;
        kFileHeader.bfReserved1 = 0;
        kFileHeader.bfReserved2 = 0;
        kFileHeader.bfOffBits = (uint16)(sizeof(ZFB_BMPFILEHEADER) + sizeof(ZFB_BMPINFOHEADER));
	kFileHeader.bfOffbits2 =0;

	SCI_MEMSET(&kInfoHeader, 0, sizeof(ZFB_BMPINFOHEADER));
        kInfoHeader.biSize = sizeof(ZFB_BMPINFOHEADER);
        kInfoHeader.biWidth = unWidth * zoom_vol;
        kInfoHeader.biHeight = ((int)unHeiht * zoom_vol);
        kInfoHeader.biPlanes = 1;
        kInfoHeader.biBitCount = 24;
        kInfoHeader.biCompression = ZFB_BI_RGB;
        kInfoHeader.biSizeImage = 0;
        kInfoHeader.biXPelsPerMeter = 0;
        kInfoHeader.biYPelsPerMeter = 0;
        kInfoHeader.biClrUsed = 0;
        kInfoHeader.biClrImportant = 0;

        // Convert QrCode bits to bmp pixels
        pSourceData = pBarCode;
        for (y = 0; y < unHeiht; y++)
        {
            pDestData = pRGBData + unWidthAdjusted * y * zoom_vol;
            for (x = 0; x < unWidth; x++)
            {
                a = x/11;
                if(a == (BarCodeLen-1) || a == BarCodeLen)
                {
                    //停止位有12个Bit
                    cur_data = pSourceData[BarCodeLen-1];
                    cur_bit = unWidth-x-1;
                    cur_val = cur_data & (0x0001 << cur_bit);
                }
                else
                {
                    cur_data = pSourceData[a];
                    cur_bit = 10 - (x%11);
                    cur_val = cur_data & (0x0001 << cur_bit);
                }
                if (cur_val)
                {
                    for (l = 0; l < zoom_vol; l++)
                    {
                        for (n = 0; n < zoom_vol; n++)
                        {
                            *(pDestData + n * 3 + unWidthAdjusted * l) = ZFB_PIXEL_COLOR_B;
                            *(pDestData + 1 + n * 3 + unWidthAdjusted * l) = ZFB_PIXEL_COLOR_G;
                            *(pDestData + 2 + n * 3 + unWidthAdjusted * l) = ZFB_PIXEL_COLOR_R;
                        }
                    }
                }
                pDestData += 3 * zoom_vol;
            }
        }
        p_img = SCI_ALLOC_APP(bmp_buf_len);
        if(p_img == NULL)
        {
            SCI_FREE(pRGBData);
            return 1;
        }
        SCI_MEMCPY(p_img,(uint8 *)&kFileHeader,head_len);
        SCI_MEMCPY(p_img+head_len,(uint8 *)&kInfoHeader,info_len);
        SCI_MEMCPY(p_img+head_len+info_len,(uint8 *)pRGBData,unDataBytes);
        * ppImg = p_img;
        *pLen = bmp_buf_len;
        SCI_FREE(pRGBData);
    }
    else
    {
       // MSG_HIGH("NULL returned",0,0,0);
        return 1;
    }

    return 0;
}

uint32  ZDT_HTTP_File_WriteExt(MMIFILE_HANDLE file_handle,uint8  * data_ptr, uint32 data_len)
{
    uint16 j = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    uint32 transbyte = 0;
    uint32 all_transbyte = 0;
    BOOLEAN ret = TRUE;
    if(file_handle == SFS_INVALID_HANDLE)
    {
        return 0;
    }

    do
    {
        transbyte = 0;
        file_ret = MMIFILE_WriteFile(
        	file_handle, 
        	data_ptr, 
        	data_len, 
        	&transbyte, 
        	NULL
        	);
        
        if(SFS_ERROR_NONE != file_ret)
        {
            ret = FALSE;
            break;
        }
        all_transbyte += transbyte;
    }while(transbyte != 0 && all_transbyte < data_len);
    

    return all_transbyte;
	
}
BOOLEAN  ZFB_DBFile_Write(char * path,char * filename, uint8 * data_ptr, uint32  data_len)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    BOOLEAN ret = FALSE;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     
     if(MMIFILE_IsFileExist(full_path,full_path_len))
     {
         MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
     }
        
	file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
	if(SFS_INVALID_HANDLE != file_handle)
	{
            transbyte = ZDT_HTTP_File_WriteExt(file_handle,data_ptr,data_len);
		if(transbyte > 0)
		{
			ret = TRUE;
		}
		MMIFILE_CloseFile(file_handle);
	}
       return ret;
	
}

BOOLEAN  ZFB_DBFile_Exist(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     
     return MMIFILE_IsFileExist(full_path,full_path_len);	
}

uint32  ZFB_DBFile_GetSize(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    MMIFILE_HANDLE file_handle = 0;
    uint32   file_size          = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;

     file_handle = MMIAPIFMM_CreateFile(full_path, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        MMIAPIFMM_CloseFile(file_handle);
    }
    return file_size;	
}

BOOLEAN  ZFB_DBFile_Read(char * path, char * filename, void *buffer, uint32 bytes_to_read, uint32 *bytes_read)
{
    BOOLEAN ret = FALSE;
    FFS_ERROR_E error = FFS_NO_ERROR;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    uint32 transbyte = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
	  
    for(i = 0; i < tmp_len; i++)
    {
        full_path[full_path_len++] = file_dev_ptr[i];
    }
        
    full_path[full_path_len++] = ':';
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
    full_path_len += tmp_len;
        
    if(MMIFILE_IsFileExist(full_path, full_path_len))
    {
        file_handle = MMIFILE_CreateFile(full_path, SFS_MODE_OPEN_EXISTING |SFS_MODE_READ, NULL, NULL);
    }
    else
    {
        return FALSE;
    }

    if(SFS_INVALID_HANDLE != file_handle)
    {
    	file_ret = MMIFILE_ReadFile(
                              file_handle, 
                              buffer, 
                              bytes_to_read, 
                              bytes_read, 
                              NULL
                             );
        
        if(SFS_ERROR_NONE != file_ret)
        {
            error = FFS_ERROR_INVALID_PARAM;
        }
        else
        {

            ret = TRUE;
        }
        MMIFILE_CloseFile(file_handle);
    }

    return ret;
}

BOOLEAN  ZFB_DBFile_RemoveFile(char * path, char * filename)
{
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;
    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
        
     for(i = 0; i < tmp_len; i++)
     {
         full_path[full_path_len++] = file_dev_ptr[i];
     }
        
     full_path[full_path_len++] = ':';
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
     full_path_len += tmp_len;
     full_path[full_path_len++] = '\\';
     tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)filename, SCI_STRLEN(filename));
     full_path_len += tmp_len;
     if(MMIFILE_IsFileExist(full_path,full_path_len))
     {
        MMIFILE_DeleteFile((const uint16 *)full_path,PNULL);
     }
     return TRUE;	
}

BOOLEAN  ZFB_DBFile_RemoveDir(char * path	)
{
    BOOLEAN ret = FALSE;
    SFS_HANDLE	hFind    = PNULL;
    SFS_FIND_DATA_T sfsfind             = {0};
    SFS_ERROR_E		result              = SFS_NO_ERROR;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0}; 
    uint16  full_path_len = 0;
    wchar * file_dev_ptr;
    uint16   tmp_len = 0;
    uint16 i = 0;

    if(MMIFILE_GetDeviceStatus(MMIFILE_GetDevicePath(MMI_DEVICE_UDISK), MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        file_dev_ptr = MMIFILE_GetDevicePath(MMI_DEVICE_UDISK);
        tmp_len = MMIFILE_GetDevicePathLen(MMI_DEVICE_UDISK);
    }
    else
    {
        return FALSE;
    }   
	  
    for(i = 0; i < tmp_len; i++)
    {
        full_path[full_path_len++] = file_dev_ptr[i];
    }

    full_path[full_path_len++] = ':';
    full_path[full_path_len++] = '\\';
    tmp_len = GUI_GBToWstr(&full_path[full_path_len], (const uint8*)path, SCI_STRLEN(path));
    full_path_len += tmp_len;        
    if( '//' != full_path[full_path_len-1] && 0x5c != full_path[full_path_len-1] )
    {
        full_path[full_path_len++] = 0x5c;
    }

    full_path[full_path_len] = 0x2a;
    full_path[full_path_len+1] = 0;

    hFind  = SFS_FindFirstFile(full_path,&sfsfind );

    if( hFind == PNULL)
    {
        return TRUE;
    }

    do
    {
        uint32 uNameLen = 0;
        uNameLen = MMIAPICOM_Wstrlen( (wchar *)sfsfind.name );
        SCI_MEMCPY(&full_path[full_path_len], sfsfind.name, MIN(uNameLen*2+2, MMIFILE_FULL_PATH_MAX_LEN));
        full_path[full_path_len+uNameLen] = '\0';

        // 是目录
        if( sfsfind.attr & SFS_ATTR_DIR )
        {
        }
        else
        {
            //删除文件
            result = MMIFILE_DeleteFile(full_path, PNULL);

            if( SFS_NO_ERROR != result )
            {
                SFS_FindClose(hFind);
                return FALSE;
            }
        }

        result = SFS_FindNextFile(hFind, &sfsfind);

    }while(SFS_NO_ERROR == result);

    SFS_FindClose(hFind);

    return TRUE;
}

//解决支付宝闰年的天数计算错误问题 added by bao 20240102
static BOOLEAN  ZFB_IsLeapYear(uint32 year)
{
	BOOLEAN ret_val = FALSE;

	ret_val = (BOOLEAN)((!(year % 4) && (year % 100)) || !(year % 400));

	return ret_val;
}

static uint32 ZFBTimeAllDay(ZFBTime a)   
{     
    uint32   notrun[12] = {31,28,31,30,31,30,31,31,30,31,30,31};   
    uint32   x = 0, i = 0;   
    
    //解决支付宝闰年的天数计算错误问题 Modified by bao 20240102
    //x = (a.wYear)*365 + a.wYear/4 + a.wYear/400 - a.wYear/100;
    
    for(i = 0; i < a.wYear; i++)
    {
        x += (365 + ZFB_IsLeapYear(i));
    }
    
    if (ZFB_IsLeapYear(a.wYear))
    {   
        notrun[1] = 29;
    }
    
    for (i = 0; i < a.nMonth; i++)
    {   
        if (i > 0)
        {   
            x += notrun[i-1];   
        }   
    }   
    
    x += a.nDay;
    
    return  x;   
}

static uint32 ZFBTimeInterval(ZFBTime a, ZFBTime b)   
{   
    uint32   x_days = 0, y_days = 0;   
    uint32   sum = 0;   

    //SCI_TRACE_LOW("[QQ]ZFBTimeInterval");

    x_days = ZFBTimeAllDay(a);   
    y_days = ZFBTimeAllDay(b);   
    
    sum = (x_days-y_days)*24 + (a.nHour -b.nHour) - 8;//hours 差   
    return  (sum*3600 + (a.nMin-b.nMin)*60 + (a.nSec - b.nSec));   
}

static void ZFBGetDateTime(ZFBTime* time_ptr)
{
    SCI_DATE_T date = {0};
    SCI_TIME_T time = {0};
    
    //SCI_TRACE_LOW("[QQ]ZFBGetDateTime");

    if (PNULL == time_ptr)
    {
        return;
    }

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);

    time_ptr->wYear = date.year;
    time_ptr->nMonth = date.mon;
    time_ptr->nDay = date.mday;
    time_ptr->nHour = time.hour;
    time_ptr->nMin = time.min; 
    time_ptr->nSec = time.sec;
}

extern uint32 ZFBGetSystemTime(void)
{
    ZFBTime time = {0};
    ZFBTime startTime = {0};
    ZFBTime time1 = {0};

    ZFBGetDateTime(&time1);

    //SCI_TRACE_LOW("[QQ]QGetSystemTime");

    startTime.wYear = ZFB_SYSTEM_START_YEAR;
    startTime.nMonth = 1;
    startTime.nDay = 1;
    startTime.nHour = 0;
    startTime.nMin = 0;
    startTime.nSec = 0;

    time.wYear =  time1.wYear;
    time.nMonth = time1.nMonth;
    time.nDay = time1.nDay;
    time.nHour = time1.nHour;
    time.nMin = time1.nMin;
    time.nSec = time1.nSec;

    return ZFBTimeInterval(time, startTime);
}

#ifdef WIN32
extern int64 ZFBGetSystemTimeMS(void)
#else
extern long long ZFBGetSystemTimeMS(void)
#endif
{
#ifdef WIN32
    static int64 s_last_utc_ms = 0;
    static uint32 s_last_tick_count = 0;
    int64 ret = 0;
    int64 temp = 0;
#else
    static long long s_last_utc_ms = 0;
    static uint32 s_last_tick_count = 0;
    long long ret = 0;
    long long temp = 0;
#endif
    static uint32 s_last_cur_tick = 0;
    uint32 cur_tick;
    uint32 cur_sec;
#ifdef ALIPAY_DEBUG
    char tmp_buf[200] = {0};
#endif

    cur_tick = SCI_GetTickCount();
        
    if(s_last_utc_ms > 0 && s_last_tick_count > 0 && cur_tick > s_last_tick_count && cur_tick >= s_last_cur_tick)
    {
        cur_tick = cur_tick - s_last_tick_count;
        temp = cur_tick;
        ret = s_last_utc_ms + temp;
    }
    else
    {
        s_last_tick_count = cur_tick;
        cur_tick = cur_tick%1000;
        cur_sec = ZFBGetSystemTime();
        temp = cur_sec;
        s_last_utc_ms = temp*1000;
        temp = cur_tick;
        s_last_utc_ms += temp;
        ret = s_last_utc_ms;
    }
    s_last_cur_tick = cur_tick;
#ifdef ALIPAY_DEBUG
    sprintf(tmp_buf,"alipay ZFBGetSystemTimeMS ret=%lld,s_last_utc_ms=%lld",ret,s_last_utc_ms);
    SCI_TRACE_LOW(tmp_buf);
#endif
    return ret;
}
