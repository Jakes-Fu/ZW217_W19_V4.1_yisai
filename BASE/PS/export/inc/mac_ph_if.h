/******************************************************************************
 ** File Name:      mac_ph.h                                                  *
 ** Author:         Jason.Gao & Chris.Jin                                     *
 ** Date:           01/11/2001                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    These file includes functions used by MAC and LAYER1      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME                     Description:                      *
 **---------------------------------------------------------------------------*
 ** 01/11/2001     Jason.Gao & Chris.Jin    Create.                           *
 ******************************************************************************/
#ifndef _MAC_PH_IF_H__
#define _MAC_PH_IF_H__

/*****************************************************************************/
//  Description :
//                In every block period, layer1 calls MACPH_PhReadMacBlocks() function to get an array of block
//                data to send in next block period. In dynamic allocation mode, layer1 sends some of them
//                if the USF value in downlink block decoded in certain slot matches, or none of them is
//                sent to network side if the USF in every slot doesn't match.  Then layer calls
//                MACPH_PhMacAckBlocks() to inform MAC layer how many blocks data are sent. MAC will
//                reset the send pointer to next MAC/RLC block which is unsent. In fixed allocation mode,
//                MAC layer keeps the tx block pattern map, so MACPH_PhReadMacBlocks() function returns a
//                few blocks data which need to be sent in next block period unconditionally. Layer1
//                will send them unconditionally.
//        Parameters:
//            block_ptr ,    ????¨ºy¡Á¨¦¦Ì????????¨°?¨¢11¨¬?¡ê?¨ºy¡Á¨¦?D¨°¨¤¡ä?¡¤?control block, data block¡ê?polling block¡ä¨®¨ºy¡Á¨¦?2?a¨º?¡ä?¡¤??¡ê
//            polling_slot_index_arr , index array,0x0-0x07 means polled and 0xff means no polled .
//            slot_mask   ,  ¦Ì¡À¨º¡Àfixed allocation¨º¡À¡ê?byte ¦Ì?????bit ¡ä¨²¡À¨ª????time slot,
//                           ¡ä¨®?????e¡ê?¡ä¨²¡À¨ª0 timeslot ,1 timeslot ...?¡ê
//
//            frame_number , ¨º?¨¦?DD¦Ì??¨¬¨ºy¡ê?¨®?¨®¨²?D??polling queue ¨¤?¦Ì??a??¨º?¡¤??¨¦¨°?¡¤¡é?¨ª?¡ê

//  Global resource dependence :
//  Author: Jason.Gao
//  Note:
/*****************************************************************************/
void MACPH_PhReadMacBlocks(
    BLOCK_STRUCT_PTR_ARR_T  block_struct_ptr_arr,       //block pointer array(in/out)
    POLLING_TS_INDEX_ARR_T  polling_slot_index_arr,     //polling slot index array(in/out)
    uint8                    *slot_mask_ptr,            //slot mask pointer(in/out)
    ABSOLUTE_FRAME_NUMBER_T frame_number                //frame number
);

/*****************************************************************************/
//  Description :
//        Layer1 call this function to note how many blocks have been sent by layer1. We should delete the
//        blocks in Control, Polling and Rlc Data Queue.
//  Global resource dependence :
//  Author: Jason.Gao
//  Note:
/*****************************************************************************/
void MACPH_PhAckMacSentNum(
    uint16 send_num,         //number of blocks that have been sent by layer 1
    ABSOLUTE_FRAME_NUMBER_T frame_number    //frame number
);

/*****************************************************************************/
//  Description :
//        GPRS TBF Optimize. Layer1 call the function to find if the data belong to this MS.
//  Global resource dependence :
//  Author: xiangxin.gu
//  Note:
/*****************************************************************************/
BOOLEAN MACPH_IsDataToMe(TBF_DIRECTION_T tbf_direction, uint8 *data);

#endif