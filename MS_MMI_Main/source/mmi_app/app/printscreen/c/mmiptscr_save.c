/****************************************************************************
** File Name:      mmiptscr_save.c                                               *
** Author:         bin.ji                                                  *
** Date:           04/14/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/14/2011        bin.ji            Create
** 
****************************************************************************/

#define MMIPTSCR_SAVE_C

#include "mmi_app_printscreen_trc.h"
#ifdef PRINTSCREEN_SUPPORT

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmifmm_export.h"
#include "mmiptscr_save.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/

//write buffer size
#define WRITE_BUFFER_SIZE       (2000 * 3)

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

//bmp header
struct {
  uint8 ident[2];
  uint8 filesize[4];
  uint8 reserved[4];
  uint8 dataoffset[4];
  uint8 headersize[4];
  uint8 width[4];
  uint8 height[4];
  uint8 planes[2];
  uint8 bitsperpixel[2];
  uint8 compression[4];
  uint8 datasize[4];
  uint8 hres[4];
  uint8 vres[4];
  uint8 colors[4];
  uint8 importantcolors[4];
  //    u8 pad[2];
} bmpheader;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

//uint32 systemColorDepth = 16;
LOCAL const uint32 systemRedShift = 11;
LOCAL const uint32 systemGreenShift = 6;
LOCAL const uint32 systemBlueShift = 0;

LOCAL const uint32 color32systemRedShift = 16;
LOCAL const uint32 color32systemGreenShift = 8;
LOCAL const uint32 color32systemBlueShift = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
                      
/*****************************************************************************/
//     Description : put dword
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void utilPutDword(uint8 *p, uint32 value);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : put dword
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void utilPutDword(uint8 *p, uint32 value)
{
  *p++ = value & 255;
  *p++ = (value >> 8) & 255;
  *p++ = (value >> 16) & 255;
  *p = (value >> 24) & 255;
}

/*****************************************************************************/
//     Description : 将原始图像数据输出为bmp格式图片
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPTSCR_WriteBMPFile(const wchar *u16filename, uint32 w, uint32 h, uint8 *pix, uint32 ColorDepth)
{
    //uint8 writeBuffer[512 * 10 * 3];
    uint8 *writeBuffer = PNULL;
    //uint32 fsz = sizeof(bmpheader) + w*h*3; 
    //FILE *fp = fopen(fileName,"wb");
    MMIFILE_HANDLE fp = 0;
    //uint8 *b = writeBuffer;
    uint8 *b = PNULL;
    uint32 sizeX = w;
    uint32 sizeY = h;
    uint32 x = 0, y = 0;
    uint32 write_size = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;    
    //int nLineBytes = ((nWidth*nBitCnt+31)>>5)<<2;
    uint32 fsz = 0; 
    uint32 nLineBytes = 0;
    const uint32 nBitCnt = 24;
    
    if (PNULL == pix)
    {
        //buffer为空
        return FALSE;
    }
    fp = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
    
    if(SFS_INVALID_HANDLE == fp)
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_WriteBMPFile MMIAPIFMM_CreateFile fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_SAVE_128_112_2_18_2_47_35_11,(uint8*)"");
        return FALSE;
    }
#if 0   //输出原始数据
    file_ret = MMIAPIFMM_WriteFile(fp, pix, w*h*(ColorDepth>>3), &write_size, PNULL);
    MMIAPIFMM_CloseFile(fp);
    return TRUE;
#endif

    SCI_MEMSET(&bmpheader, 0, sizeof(bmpheader));
    
    bmpheader.ident[0] = 'B';
    bmpheader.ident[1] = 'M';

    nLineBytes = ((w*nBitCnt+31)>>5)<<2;
    fsz = sizeof(bmpheader) + nLineBytes*h; 
    utilPutDword(bmpheader.filesize, fsz);
    utilPutDword(bmpheader.dataoffset, 0x36);
    utilPutDword(bmpheader.headersize, 0x28);
    utilPutDword(bmpheader.width, w);
//    utilPutDword(bmpheader.height, h);
    utilPutDword(bmpheader.height, h);
    utilPutDword(bmpheader.planes, 1);
    utilPutDword(bmpheader.bitsperpixel, 24);
    utilPutDword(bmpheader.datasize, 3*w*h);
    
//    fwrite(&bmpheader, 1, sizeof(bmpheader), fp);
    file_ret = MMIAPIFMM_WriteFile(fp, &bmpheader, sizeof(bmpheader), &write_size, PNULL);
	if(SFS_ERROR_NONE != file_ret)
	{
	    MMIAPIFMM_CloseFile(fp);
		return FALSE;
	}
    if (nLineBytes > WRITE_BUFFER_SIZE)
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_WriteBMPFile WriteBMPFile bmp is to large! w = %d, nLineBytes = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_SAVE_163_112_2_18_2_47_35_12,(uint8*)"dd", w, nLineBytes);
        MMIAPIFMM_CloseFile(fp);
        return FALSE;
    }
    writeBuffer = SCI_ALLOCA(WRITE_BUFFER_SIZE);
    b = writeBuffer;
    if (PNULL == writeBuffer)
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_WriteBMPFile WriteBMPFile alloc fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPTSCR_SAVE_171_112_2_18_2_47_35_13,(uint8*)"");
        MMIAPIFMM_CloseFile(fp);
        return FALSE;
    }
    //  switch(systemColorDepth) 
    switch (ColorDepth)
    {
    case 16:
        {
            uint16 *p = (uint16 *)(pix+(w)*(h-1)*2); // skip first black line
            for( y = 0; y < sizeY; y++) {
                for( x = 0; x < sizeX; x++) {
                    uint16 v = *p++;
                    
                    *b++ = ((v >> systemBlueShift) & 0x01f) << 3; // B      
                    *b++ = ((v >> systemGreenShift) & 0x001f) << 3; // G 
                    *b++ = ((v >> systemRedShift) & 0x001f) << 3; // R
                }
                p -= 2*(w);
//                fwrite(writeBuffer, 1, 3*w, fp);
                file_ret = MMIAPIFMM_WriteFile(fp, writeBuffer, nLineBytes/*3*w*/, &write_size, PNULL);
	            if(SFS_ERROR_NONE != file_ret)
	            {
                    SCI_FREE(writeBuffer);
                    MMIAPIFMM_CloseFile(fp);
		            return FALSE;
	            }                
                b = writeBuffer;
            }
        }
        break;
    case 24:
        {
            uint8 *pixU8 = (uint8 *)pix+3*w*(h-1);
            for( y = 0; y < sizeY; y++) {
                for( x = 0; x < sizeX; x++) {
                        *b++ = *pixU8++; // B
                        *b++ = *pixU8++; // G
                        *b++ = *pixU8++; // R                         
/*
                    if(systemRedShift > systemBlueShift) {
                        *b++ = *pixU8++; // B
                        *b++ = *pixU8++; // G
                        *b++ = *pixU8++; // R           
                    } else {
                        uint32 red = *pixU8++;
                        uint32 green = *pixU8++;
                        uint32 blue = *pixU8++;
                        
                        *b++ = blue;
                        *b++ = green;
                        *b++ = red;
                    }
                    */
                }
                pixU8 -= 2*3*w;
//                fwrite(writeBuffer, 1, 3*w, fp);
                file_ret = MMIAPIFMM_WriteFile(fp, writeBuffer, nLineBytes, &write_size, PNULL);
	            if(SFS_ERROR_NONE != file_ret)
	            {
                    SCI_FREE(writeBuffer);
                    MMIAPIFMM_CloseFile(fp);
		            return FALSE;
	            }                
                b = writeBuffer;
            }
        }
        break;
    case 32:
        {
            uint32 *pixU32 = (uint32 *)(pix+4*w*(h-1));
            for( y = 0; y < sizeY; y++) {
                for( x = 0; x < sizeX; x++) {
                    uint32 v = *pixU32++;
                    
                    *b++ = ((v >> color32systemBlueShift) & 0x00ff); // B     
                    *b++ = ((v >> color32systemGreenShift) & 0x00ff); // G
                    *b++ = ((v >> color32systemRedShift) & 0x00ff); // R
                }
                pixU32 -= 2*w;                
//                fwrite(writeBuffer, 1, 3*w, fp);
                file_ret = MMIAPIFMM_WriteFile(fp, writeBuffer, nLineBytes, &write_size, PNULL);
	            if(SFS_ERROR_NONE != file_ret)
	            {
                    SCI_FREE(writeBuffer);
                    MMIAPIFMM_CloseFile(fp);
		            return FALSE;
	            }
                
                b = writeBuffer;
            }
        }
        break;
    default:
        SCI_FREE(writeBuffer);
        MMIAPIFMM_CloseFile(fp);
        return FALSE;
    }
    SCI_FREE(writeBuffer);
    MMIAPIFMM_CloseFile(fp);
    
    return TRUE;
}

#endif  //PRINTSCREEN_SUPPORT


