/******************************************************************************
 ** File Name:      lzma_hal.h                                                *
 ** Author:         lichd                                                   *
 ** DATE:           06/01/2011                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    define trace interface just for testing usage             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/01/2011     lichd            Create                                    *
 ******************************************************************************/

#ifndef _LZMA_HAL_H_
#define _LZMA_HAL_H_

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define LZMA_UNZIP_NORAML             0x0000    
#define LZMA_UNZIP_FORCE_STOP         0x0001    
#define LZMA_UNZIP_AUTO_STOP          0x0002    

#define UNZIP_SYNC_MODE                0x0000
#define UNZIP_ASYNC_MODE               0x8000

#define UNZIP_TIMEOUT_MAX              10000   

#define LZMA_UNZIP_BUF_LEN            0x1400
/**---------------------------------------------------------------------------*
 **                         Protocol Define                                    *
 **---------------------------------------------------------------------------*/
typedef uint32 (*LZMA_CallBack)(void* param);


typedef struct LZMA_CFG_Tag
{
   uint32 src_addr;
   uint32 src_len;
   uint32 des_addr;
   uint32 des_len;
   uint32 cache_buf_addr;        //Used when mode configuration include LZMA_UNZIP_FORCE_STOP or LZMA_UNZIP_AUTO_STOP
   uint32 cache_buf_len;         //Used when mode configuration include LZMA_UNZIP_FORCE_STOP or LZMA_UNZIP_AUTO_STOP
   uint32 cache_buf_offset;      //Used when mode configuration include LZMA_UNZIP_FORCE_STOP or LZMA_UNZIP_AUTO_STOP
   LZMA_CallBack callback_func;  //Used when mode configuration include UNZIP_ASYNC_MODE
   uint32 work_mode;             //LZMA_UNZIP_NORAML  or   LZMA_UNZIP_FORCE_STOP    or   LZMA_UNZIP_AUTO_STOP
   uint32 opt_mode;              //UNZIP_SYNC_MODE    or   UNZIP_ASYNC_MODE 
}LZMA_CFG_T;

typedef struct LZMA_OUT_Tag
{
  uint32 real_size;
}LZMA_OUT_T;

extern uint32 s_lzma_hal_src_buf[];
uint32 LZMA_HAL_UnZip_U1(LZMA_OUT_T* unzip_out_ptr, LZMA_CFG_T* lzma_cfg_ptr);
uint32 LZMA_HAL_UnZip(LZMA_OUT_T* unzip_out_ptr, LZMA_CFG_T* lzma_cfg_ptr);
PUBLIC void LZMA_HAL_Init(void);
PUBLIC uint32 LZMA_HAL_GetUnZipStatus(void);

#if defined(CHIP_DRV_6531) || defined(CHIP_DRV_6531EFM)
uint32 LZMA_HAL_UnZipExt(uint32 , LZMA_OUT_T* , LZMA_CFG_T* );
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function  Declare                                 *
 **---------------------------------------------------------------------------*/


#endif //_LZMA_HAL_H_
