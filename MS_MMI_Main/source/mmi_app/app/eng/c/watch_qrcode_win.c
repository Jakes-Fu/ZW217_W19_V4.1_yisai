/*****************************************************************************
** File Name:      watch_bt_qrcode_win.c                                     *
** Author:         bin.wang1                                                 *
** Date:           03/23/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description: display bluetooth connect qrcode                             *
******************************************************************************
**                         Important Edit History
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 03/13/2020           bin.wang1             Create                         *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "window_parse.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "guifont.h"
#include "guilabel.h"
#include "os_api.h"
#include "guicommon.h"
#include "mmidisplay_data.h"
#include "mmi_modu_main.h"
#include "mmicom_data.h"
//#include "mmi_id.h"

#include "qrinput.h"
#include "qrencode.h"

#include "mmicom_trace.h"
#include "watch_qrcode_export.h"

#include "mmieng_id.h"



/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

#define QR_CODE_SIZE                    DP2PX_VALUE(4)    //each point size
#define QR_CODE_VERSION                 DP2PX_VALUE(4)    //each point size
#define QR_CODE_MAX_LEN                 (100)
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

typedef struct
{
    int32   inputStrLen;
    uint8*  pInputStr;
    QRcode  qrcode;
}QRCODE_WIN_DATA_S;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

LOCAL QRCODE_WIN_DATA_S s_qrcode_win_data = { 0 };

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get qr code with num string
//  Global resource dependence : 
//  Author:
//  Note:  need free QRcode * and QRcode *->data after using
//            strLen is string lenth
/*****************************************************************************/

LOCAL QRcode * QRCodeGetNumQRCode( unsigned char * pNumStr, int32 strLen )
{
    QRinput*    stream = PNULL;
    QRcode*     qrcode = PNULL;

    int         qrver   = QR_CODE_VERSION;
    QRecLevel   qrlevel = QR_ECLEVEL_M;

    QRencodeMode  qrmode = QR_MODE_AN;
    
    TRACE_APP_COMMON("QRCodeGetNumQRCode");

    if( NULL == pNumStr)
    {
        TRACE_APP_COMMON("pNumStr is PNULL");
        return NULL;
    }

    stream = QRinput_new2( qrver, qrlevel );

    QRinput_append( stream, qrmode, (int)strLen,  pNumStr );

    qrcode = QRcode_encodeInput(stream);

    SCI_FREE( stream );

    return qrcode;
}

LOCAL void QRCodeDestroyWinData( void )
{
    if( s_qrcode_win_data.pInputStr != PNULL )
    {
        SCI_FREE( s_qrcode_win_data.pInputStr );
    }

    if( s_qrcode_win_data.qrcode.data != PNULL )
    {
        SCI_FREE( s_qrcode_win_data.qrcode.data );
    }

    memset( &s_qrcode_win_data, 0x00, sizeof(s_qrcode_win_data) );
}

LOCAL void QRCodeGetWinQRCodeData( void )
{
    QRcode* pQRcode = PNULL;

    if( PNULL == s_qrcode_win_data.pInputStr )
    {
        TRACE_APP_COMMON("s_qrcode_win_data.pInputStr is PNULL!");
        return;
    }

    pQRcode = QRCodeGetNumQRCode( (unsigned char*)s_qrcode_win_data.pInputStr, s_qrcode_win_data.inputStrLen );
    if( pQRcode != PNULL )
    {
        if( pQRcode->data != PNULL )
        {
            s_qrcode_win_data.qrcode.data = SCI_ALLOC_APP( (pQRcode->width*pQRcode->width)+1 );
            if( s_qrcode_win_data.qrcode.data != PNULL )
            {
                s_qrcode_win_data.qrcode.version = pQRcode->version;
                s_qrcode_win_data.qrcode.width = pQRcode->width;
                SCI_MEMCPY( s_qrcode_win_data.qrcode.data, pQRcode->data, pQRcode->width*pQRcode->width );
            }
            SCI_FREE( pQRcode->data );
        }
        SCI_FREE( pQRcode );
    }
}

/*****************************************************************************/
//  Description : draw qr code picture
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/

LOCAL void QRCodeDrawQRcode( GUI_LCD_DEV_INFO const* dev_info_ptr, QRcode* qrcode_ptr )
{
    int32           row;
    int32           line;
    int32           width;
    unsigned char*  p;
    GUI_RECT_T      rect;
    uint16          x_offset = 0;
    uint16          y_offset = 0;
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;

    TRACE_APP_COMMON("QRCodeDrawQRcode");

    if( PNULL == qrcode_ptr )
    {
        TRACE_APP_COMMON("qrcode_ptr is PNULL!");
        return;
    }

    if( PNULL == qrcode_ptr->data )
    {
        TRACE_APP_COMMON("qrcode_ptr->data is PNULL!");
        return;
    }

    width   = qrcode_ptr->width;
    p       = qrcode_ptr->data;

    //get lcd width and height
    if (GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height))// Bug 1628933
    {
        x_offset = (lcd_width - width * QR_CODE_SIZE)/2;
        y_offset = (lcd_height - width * QR_CODE_SIZE)/2;
    }

    for( row = 0; row < width; row++ ) 
    {
        rect.top = y_offset + row * QR_CODE_SIZE;

        for( line = 0; line < width; line++) 
        {
            rect.left = x_offset + line * QR_CODE_SIZE;
            rect.right = rect.left + QR_CODE_SIZE;
            rect.bottom = rect.top + QR_CODE_SIZE;
            if( *p&1 )
            {
                GUI_FillRect( dev_info_ptr, rect, MMI_BLACK_COLOR );
            }
            p++;
        }
    }
}

LOCAL void QRCode_Main_OPEN_WINDOW( MMI_WIN_ID_T win_id )
{
    QRCodeGetWinQRCodeData();
}

LOCAL void QRCode_Main_FULL_PAINT( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T          bgRect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO    lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };

    // bg color
    GUI_FillRect( &lcd_dev_info, bgRect, MMI_WHITE_COLOR );

    QRCodeDrawQRcode( (const GUI_LCD_DEV_INFO *)&lcd_dev_info, &(s_qrcode_win_data.qrcode) );
}

LOCAL void QRCode_Main_APP_CANCEL( MMI_WIN_ID_T win_id )
{
    QRCodeDestroyWinData();
    MMK_CloseWin( win_id );
}

/*****************************************************************************/
//  Description : handle qrcode window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E HandleQRCodeWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            QRCode_Main_OPEN_WINDOW( win_id );
            break;
        }

        case MSG_FULL_PAINT:
        {
            QRCode_Main_FULL_PAINT( win_id );
            break;
        }
        case MSG_APP_RED:
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            QRCode_Main_APP_CANCEL( win_id );
            break;
        }

        default:
            break;
    }

    return recode;
}

WINDOW_TABLE( MMIQRCODE_MAIN_WIN_TAB ) = 
{
    WIN_ID(MMIQRCODE_MAIN_WIN_ID),
    WIN_HIDE_STATUS, 
    WIN_FUNC((uint32)HandleQRCodeWinMsg),
    END_WIN
};

/*****************************************************************************/
//  Description : open qr code windows
//  Global resource dependence : 
//  Author:
//  Note: only support digit qrcode now
/*****************************************************************************/
PUBLIC void WatchQRCode_MainWin_Enter( uint8* input_str_ptr, int32 input_str_Len )
{

    if( PNULL == input_str_ptr )
    {
        TRACE_APP_COMMON("pStr is PNULL!!");
        return;
    }
    QRCodeDestroyWinData();

    s_qrcode_win_data.pInputStr = (uint8*)SCI_ALLOC_APPZ( sizeof(uint8)*(input_str_Len+1) );

    if( PNULL == s_qrcode_win_data.pInputStr )
    {
        TRACE_APP_COMMON("s_qrcode_win_data.pInputStr is PNULL!!");
        return;
    }
    SCI_MEMCPY( s_qrcode_win_data.pInputStr, input_str_ptr, MIN(input_str_Len,QR_CODE_MAX_LEN) );
    s_qrcode_win_data.inputStrLen = MIN(input_str_Len,QR_CODE_MAX_LEN);

    MMK_CreateWin((uint32*)MMIQRCODE_MAIN_WIN_TAB, PNULL);
}
/* this is draw directly*/

PUBLIC void WatchQRCode_Draw( uint8* input_str_ptr, int32 input_str_Len )
{
    QRcode* pQRcode = PNULL;

    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    pQRcode = QRCodeGetNumQRCode( (unsigned char*)input_str_ptr, input_str_Len );

    QRCodeDrawQRcode( (const GUI_LCD_DEV_INFO *)&lcd_dev_info, pQRcode );

}

