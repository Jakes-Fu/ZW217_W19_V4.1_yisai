/**************************************************************************
 ** File Name:      cat_mvoice.h                                          *
 ** Author:         Hai.Li                                                *
 ** Date:           12/21/2010                                            *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.     *
 ** Description:    This file inludes prototypes of cat mvoice functions  *
***************************************************************************
***************************************************************************
 **                        Edit History                                   *
 ** ----------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                           *
 ** 12/21/2010     Hai.Li           Create.                               *
***************************************************************************/
#ifndef _CAT_SAMPLE_H_
#define _CAT_SAMPLE_H_

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "audio_api.h"
#include "asm.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         Enum definitions                                 *
 **--------------------------------------------------------------------------*/
#define MIC_DELAY_PROCESS 0
#define MIC_DELAY 2   //(COUNT*128)*20*MIC_DELAY/160=COUNT*MIC_DELAY*16=960ms

#define MIC_SAMPLERATE (16000)
#define MIC_BUF_LEN (160*128*2) //MIC_BUF_LEN >COUNT*MIC_DELAY*128*2=128*120


typedef enum
{
	CATMV_END_TYPE_FIX_TIME=0, 
	CATMV_END_TYPE_AUTO_TIME
}CATMV_END_TYPE_E;

typedef enum
{
	CATMV_CHECKEND_TYPE_80=0, 
	CATMV_CHECKEND_TYPE_100, 
	CATMV_CHECKEND_TYPE_200, 
	CATMV_CHECKEND_TYPE_400, 
	CATMV_CHECKEND_TYPE_800, 
	CATMV_CHECKEND_TYPE_1200 
}CATMV_CHECKEND_TYPE_E;

typedef void (* CATMV_DATEGETCALLBACK)(int16 *pbuffer,
    uint32 src_size         /*!<data count*/
    );
    
 typedef void (* CATMV_DATAPROCESSCALLBACK)(
    int16 *src_data_pt, int16 *data_out_pt, uint16 src_size , uint16 *out_size          /*!<data count*/
    ); 
    
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: This function is to reg cat mvoice codec
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC HAUDIOCODEC CatMv_RegCodecPlugger(
    HCOLLISION hCollision,
    AUDIO_CODEC_EXT_CFGINFO_T * ptCodecExtCfgInfo
    );

/*****************************************************************************/
//  Description:    This function is to register an express plugger. 
//  Author:         Hai.Li
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOEXP CatMv_RegExpPlugger( void );


/*****************************************************************************/
//! \Description: This function is to process micsample data
//! \Author:         
//! \param process_type: the type of data process.
//! \param data_count: the process data count at a time.
//! \param mute_count: stop record according as the param mute_count .
//! \Return: 
//         0 - //start record and save the valid data 
//         1 - //start record and have no valid data ,Only ready record.
//         2 - //stop record
//! \Note:           
/*****************************************************************************/
PUBLIC uint32  CatMv_Dataprocess(
    uint16 process_type,
    uint8 data_count,
    uint8 start_mute_count,
    uint8 end_mute_count, 
    uint32 *scale
    );
/*****************************************************************************/
//! \Description: This function is to  reg cat process callback
//! \Author:
//! \param pCallback1: callback funcation for saving data in record voice.
//! \param pCallback2: callback funcation for processing data in play voice.
//! \param pdata_buf:  data buffer for record and play.
//  Note:
/*****************************************************************************/
PUBLIC void CatMv_RegCallback(
    CATMV_DATEGETCALLBACK pCallback1,
    CATMV_DATAPROCESSCALLBACK pCallback2,
    uint16 *pdata_buf
    );


/*****************************************************************************/
//! \Description: This function is to set process para.
//! \Author:
//! \param base_value:  base value of getting mic valide data .
//! \param dt_value: the rang of mute data  between sample value and base value.
//! \Note:
/*****************************************************************************/
PUBLIC void CatMv_SetPara(
    uint8    base_value,
    uint8    dt_value);


/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP CatMv_RegExpPlugger(
//!    void
//!    )
//! \param  BOOLEAN on_flag  TRUE is on, FALSE is off 
//! \return void no return
//! \brief  Description:  This function is to on /off an expression.
//! \author Author:  Hai.Li
//! \note   Note:Nothing.
/*****************************************************************************/   
PUBLIC void CatMv_RegExpOnOff( BOOLEAN on_flag);


#ifdef __cplusplus
    }
#endif

#endif /* _CAT_SAMPLE_H_ */




