/*****************************************************************************
** File Name:      mmivp_thumbnail.c                                   *
** Author:                                                                   *
** Date:           2012/1/4                                            *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/1/4    
******************************************************************************/
 #ifdef  VIDEOTHUMBNAIL_SUPPORT   

#include "mmi_app_video_player_trc.h"
#include "sci_types.h"
#include "mmivp_export.h"
#include "dal_player.h"
#include "mmivp_fsm.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include <string.h>
#include "mmivp_export.h"
#include "os_api.h"
#include "dal_player.h"
#include "sig_code.h"
#include "mmivp_id.h"
#include "mmi_default.h"
#include "mmivp_internal.h"
#ifdef  MMI_KING_MOVIE_SUPPORT 
#include "mmikm_export.h"
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         GLOBAL DEFINITION                                 */
/*---------------------------------------------------------------------------*/

LOCAL VP_THUMBNAIL_T  s_thumbnail_info={0};// ����ͼ��Ϣ�ṹ�� 
LOCAL BOOLEAN               s_is_dplayer_stopingstate=FALSE;// �Ƿ� ����ֹͣdplayer,������Ч��APP_VP_ONE_FRAME_END           


/*****************************************************************************/
//  Description : ��Dplayer��Ϊ��ȡfull_path_ptr ����ͼ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
LOCAL  DPLAYER_RETURN_E   Thumbnail_OpenDplayer(wchar *full_path_ptr) ;


/*****************************************************************************/
//  Description :��Ƶ����������ȡʱ����ʼ ���� ��Ƶ����������ȡ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  Thumbnail_VideoQueueRun(void);
/*****************************************************************************/
//  Description :��Ƶ����������ȡʱ���Ӷ����� ɾ�� ��Ƶ���е�һ��
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
LOCAL BOOLEAN Thumbnail_VideoDelete(void);
/*****************************************************************************/
//  Description : ��Ƶ����������ȡʱ����Ҫ��ȫֹͣ��ǰ��Ƶ�����ܽ�����һ�� ����ͼ��ȡ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL  void  StopCurrentDPLAYER(void);


/*****************************************************************************/
//  Description : frame end notify call back
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CallbackOneFrameEndNotify(int32 frame_num);

/*****************************************************************************/
//  Description : frame end notify call back
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void CallbackThumbnaiEndNotify(int32 frame_num); 
/*****************************************************************************/
//  Description : SendSignalToAPP
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendSignalToCurPlayingTask(uint16 signalType, uint16 data, uint32 task_id);
/*****************************************************************************/
//  Description : Get the supported_type of a movie.
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  VP_SUPPORTED_STATUS_TYPE_E CheckSupportedTypeFromMedia(DPLAYER_MEDIA_INFO_T *media_ptr);

/*****************************************************************************/
//  Description : HandleOpenNextVideo
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void HandleOpenNextVideo(void)
{

    //SCI_TRACE_LOW:"[VP]:HandleOpenNextVideo!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_94_112_2_18_3_5_25_368,(uint8*)"");
    
   // if(!s_is_dplayer_stopingstate)// �첽�£�������һ��ֹͣ����Ϣ  
   // {
  //       SCI_TRACE_LOW("[VP]:HandleOpenNextVideo  error stopping state !!");
  //       return;
  //   }
    
   if(s_thumbnail_info.is_videoqueue)
    {
    if(s_thumbnail_info.dphandle)
    {
#ifdef MMI_KING_MOVIE_SUPPORT
        VP_dplayerExtCtlDeltSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.dphandle, s_thumbnail_info.sec_handle);
#endif
        DPLAYER_Close(s_thumbnail_info.dphandle);
#ifdef  MMI_KING_MOVIE_SUPPORT 
        VP_dplayerCloseSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.sec_handle,s_thumbnail_info.b_sprd_kmv, PNULL);
#endif    
        DPLAYER_Release();
        s_thumbnail_info.dphandle=PNULL;
    }

    s_thumbnail_info.is_loading=FALSE; //һ��ʵ�����н��� 
    s_thumbnail_info.image_data=PNULL;

    Thumbnail_VideoDelete();
    MMIAPIVP_ThumbnailResume();//��ʼ������һ���ļ� 
    }
   else
    {
        //SCI_TRACE_LOW:"[VP]:HandleOpenNextVideo  error state , not video queue"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_126_112_2_18_3_5_25_369,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description :�յ���һ�� ���ųɹ���Ϣ, ��ȡ����ͼ����UI�ص�����  
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void HandleGetThumbnaiData(void)
{
    DPLAYER_IMAGE_DATA_T image_data={0};
    uint16 image_dataheight=0;
    uint16 image_datawidth=0;
    DPLAYER_RETURN_E result=DPLAYER_ERROR;

    if(s_is_dplayer_stopingstate)//dplayerֹͣ�У�������Ч��APP_VP_ONE_FRAME_END APP_VP_ONE_FRAME_REARCH��Ϣ
    {
        //SCI_TRACE_LOW:"[VP]:HandleGetThumbnaiData stopping!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_143_112_2_18_3_5_26_370,(uint8*)"");
        return;
    }
    if(s_thumbnail_info.is_videoqueue && s_thumbnail_info.videoqueue->videonum == 0)
    {
        //SCI_TRACE_LOW:"[VP]:HandleGetThumbnaiData error msg!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_148_112_2_18_3_5_26_371,(uint8*)"");
        return;
    }
     if(s_thumbnail_info.is_loading  && s_thumbnail_info.dphandle !=PNULL)
    {
        result=DPLAYER_GetNextFrameData(s_thumbnail_info.dphandle,PNULL,&image_data,PNULL);  
    }

    if( result != DPLAYER_ERROR)
    {
        image_dataheight =image_data.coordinate.trim_h;
        image_datawidth = image_data.coordinate.trim_w;

    //SCI_TRACE_LOW:"[VP]:HandleGetThumbnaiData height=%d,width=%d,datalen=%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_161_112_2_18_3_5_26_372,(uint8*)"ddd",image_dataheight,image_datawidth,image_data.buf.oneplane.len);
    
    s_thumbnail_info.tn_callback (&image_data);//����UI  �ص���Ϣ 

    if( s_thumbnail_info.is_videoqueue)
    {
        StopCurrentDPLAYER();//����ļ���ȡ����ֹͣ��ǰDPLAYER ���ȴ���ǰDPLAYER��ȫֹͣ�����ܽ�����һ��
    }
    else
    {
       if(s_thumbnail_info.is_dplayerhandle)
        {
            DPLAYER_Pause(s_thumbnail_info.dphandle);
            SCI_MEMSET((void*)(&s_thumbnail_info),0x00,sizeof(VP_THUMBNAIL_T));//����״̬
        }
       else
        {
            MMIAPIVP_ThumbnailClose();//�����ļ���ȡ��ֱ�ӹر�
        }

    }
    }
    else
    {
      //�Ѿ�����ֹͣ���ţ�mmi����Ҫ������stop
    }

}


/*****************************************************************************/
//  Description : ��ȡ  ��ǰlcd �Ƕȣ���������������л����»�ȡ������ͼ�첽����
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC LCD_ANGLE_E  MMIAPIVP_ThumbnailGetAngle(void)
{
   return s_thumbnail_info.lcd_angle;
}
/*****************************************************************************/
//  Description : ��ȡ  ��ǰ��Ƶ�����д���������Ƶ����
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPIVP_ThumbnailGetVideoQueueNum(void)
{
    uint8 qnum=0;
    VP_THUMBNAIL_VIDEOQUEUE     *videoqueue=s_thumbnail_info.videoqueue;

    if(videoqueue == NULL)
    {
        qnum = 0;
    }
    else
    {
        qnum = videoqueue->videonum;
    }
    SCI_TRACE_LOW("[VP]:MMIAPIVP_ThumbnailGetVideoQueueNum   videonum:%d!",qnum);
    return qnum;
    

}
/*****************************************************************************/
//  Description : ���õ�ǰlcd �Ƕȣ���������������л����»�ȡ������ͼ�첽����
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIVP_ThumbnailSetAngle(LCD_ANGLE_E lcd_angle)
{
     s_thumbnail_info.lcd_angle = lcd_angle;
}
/*****************************************************************************/
//  Description : ����DPLAYER ���Ų���
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIVP_ThumbnailSetDPLAYERParam(DPLAYER_DISPLAY_PARAM_T *display_param)
{
    SCI_MEMCPY((void *)(&s_thumbnail_info.display_param), (void *)display_param, sizeof(DPLAYER_DISPLAY_PARAM_T));
}
/*****************************************************************************/
//  Description : MMIAPIVP_ThumbnailGetVideo
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  wchar*  MMIAPIVP_ThumbnailGetVideo(void)
{
    //SCI_TRACE_LOW:"[VP]:VP_ThumbnailGetVideoPathname"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_212_112_2_18_3_5_26_373,(uint8*)"");
    if(s_thumbnail_info.is_open)
    {
      return  s_thumbnail_info.full_path_ptr;
    }
    else
    {
        return PNULL;
    }
     
}
/*****************************************************************************/
//  Description :     �Ƿ� ���ڽ���һ����Ƶ����ͼ��ȡ  ����DPLAYER_Play  ���ڲ���һ����Ƶ 
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_IsThumbnailRunning(void)
{
    BOOLEAN result=FALSE;
    //SCI_TRACE_LOW:"[VP]:MMIAPIVP_IsThumbnailRunning isloading:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_230_112_2_18_3_5_26_374,(uint8*)"d",s_thumbnail_info.is_loading);
    if(s_thumbnail_info.is_loading)
    {
        result = TRUE;
    }

    return result;
      
}
/*****************************************************************************/
//  Description :����ͼ��ȡ�Ƿ��Ѿ�����,MMIAPIVP_ThumbnailOpen ���óɹ���Ϊtrue,MMIAPIVP_ThumbnailClose��Ϊfalse
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_IsThumbnailOpen(void)
{
    BOOLEAN result=FALSE;
    //SCI_TRACE_LOW:"[VP]:MMIAPIVP_IsThumbnailOpen isopen:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_246_112_2_18_3_5_26_375,(uint8*)"d",s_thumbnail_info.is_open);
    if(s_thumbnail_info.is_open)
    {
        result = TRUE;
    }

    return result;
      
}
/*****************************************************************************/
//  Description :  ��Ƶ����������ȡʱ�����б�β�����һ����Ƶ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIVP_ThumbnailVideoAdd(wchar  *video_fullpath_name)
{
    BOOLEAN result=FALSE;
    uint8 qnum=0;
    VP_THUMBNAIL_VIDEOQUEUE     *videoqueue=s_thumbnail_info.videoqueue;
    //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailVideoAdd  !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_264_112_2_18_3_5_26_376,(uint8*)"");
   if(NULL == video_fullpath_name  || 0 == MMIAPICOM_Wstrlen(video_fullpath_name) || NULL == videoqueue)
    {
        return result;
    }
  
   if(videoqueue->videonum >=  THUMBNAIL_QUEUENUM)
    {
        //SCI_TRACE_LOW:"[VP]:VP_ThumbnailAddVideo full queue!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_272_112_2_18_3_5_26_377,(uint8*)"");
        //Thumbnail_VideoDelete(); //   ������ʱ�������֮ǰ �������Ƶ������ 
        //qnum=videoqueue->videonum;
        //SCI_MEMSET(videoqueue->thumbnail_name[qnum],0x00,(MMIFMM_FULL_FILENAME_LEN+1)*sizeof(wchar));
        //MMIAPICOM_Wstrncpy(videoqueue->thumbnail_name[qnum], video_fullpath_name,MMIAPICOM_Wstrlen(video_fullpath_name));
        // videoqueue->videonum++;
    }
   else
    {
        qnum=videoqueue->videonum;
        SCI_MEMSET(videoqueue->thumbnail_name[qnum],0x00,(MMIFMM_FULL_FILENAME_LEN+1)*sizeof(wchar));
        MMIAPICOM_Wstrncpy(videoqueue->thumbnail_name[qnum], video_fullpath_name,MMIAPICOM_Wstrlen(video_fullpath_name));
        videoqueue->videonum++;
        result=TRUE;
    }
    //SCI_TRACE_LOW:"[VP]:VP_ThumbnailAddVideo videonum:%d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_287_112_2_18_3_5_26_378,(uint8*)"d",videoqueue->videonum);

    return result;
}


/*****************************************************************************/
//  Description : VP_ThumbnailOpenDPLAYER , ��������ͼ��ȡ���� ���򿪳ɹ�s_thumbnail_info.is_open��Ϊtrue
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailOpen(
                                      wchar          *full_path_name_ptr,    //in��Ҫ��ȡ����ͼ�ı�����Ƶ�ļ�ȫ·�� 
                                      uint16            full_path_len,      //in,
                                      uint32            seektime,      //in��Ҫ��ȡ����ͼ�ı�����Ƶ�ļ�����ʱ���
                                      BOOLEAN  (*vp_callback)(DPLAYER_IMAGE_DATA_T *image_data),//in,ui�Ļص����� 
                                      DPLAYER_DISPLAY_PARAM_T *display_param,//in,DPLAYER���Ų��� 
                                      BOOLEAN   is_videoqueue) //in,�Ƿ�Ϊ��Ƶ�б�������ȡ
{
    BOOLEAN result=FALSE;
     //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailOpen !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_306_112_2_18_3_5_26_379,(uint8*)"");
#ifndef BRINGUP_DV_HARD_CODE
    if(full_path_name_ptr ==PNULL || display_param==PNULL || 0 == MMIAPICOM_Wstrlen(full_path_name_ptr))
    {
        return FALSE;
    }

    if(s_thumbnail_info.is_open)
    {
        //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailOpen is open now!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_315_112_2_18_3_5_26_380,(uint8*)"");
        return FALSE;
    }
/*
{   ��ӡ�ļ���
    uint16  index = MMIFMM_GetFinallySlashIndex(full_path_name_ptr,(full_path_len -1)); 
    wchar  list_file_name [MMIFMM_FULL_FILENAME_LEN+1]= {0}; 
    uint16          list_name_len  = 0;
    list_name_len = (uint16)(full_path_len - 1 - index);
    MMIAPICOM_Wstrncpy( list_file_name,  full_path_name_ptr+index + 1, list_name_len);
    //SCI_TRACE_LOW:"[VP]:MMIVP_GetSeekPointImgeData list_file_name :0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x,,0x%0x,0x%0x,0x%0x,0x%0x,0x%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_325_112_2_18_3_5_26_381,(uint8*)"ddddddddddddddd",list_file_name[0], list_file_name[1],list_file_name[2],list_file_name[3],list_file_name[4],list_file_name[5],list_file_name[6],list_file_name[7],list_file_name[8],list_file_name[9],list_file_name[10],list_file_name[11],list_file_name[12],list_file_name[13],list_file_name[14]);
}
*/
    SCI_MEMSET((void*)(&s_thumbnail_info),0x00,sizeof(VP_THUMBNAIL_T));
    //s_thumbnail_info.is_loading=TRUE;
    s_thumbnail_info.is_open=TRUE;
    s_thumbnail_info.seektime=seektime;
    s_thumbnail_info.lcd_angle=GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
    s_thumbnail_info.tn_callback= vp_callback;
    s_thumbnail_info.taskid=P_APP;
    s_thumbnail_info.is_videoqueue=is_videoqueue;
    SCI_MEMCPY((void *)(&s_thumbnail_info.display_param), (void *)display_param, sizeof(DPLAYER_DISPLAY_PARAM_T));

    
    if(s_thumbnail_info.is_videoqueue)
    {
        s_thumbnail_info.videoqueue=SCI_ALLOCA(sizeof(VP_THUMBNAIL_VIDEOQUEUE));//��Ƶ����������ȡʱ���� ������Ƶ�б�ռ�
        if(s_thumbnail_info.videoqueue == PNULL)
        {
            //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailOpen alloc mem error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_344_112_2_18_3_5_26_382,(uint8*)"");
            SCI_MEMSET((void*)(&s_thumbnail_info),0x00,sizeof(VP_THUMBNAIL_T));
            return FALSE;
        }
        SCI_MEMSET((void*)(s_thumbnail_info.videoqueue),0x00,sizeof(VP_THUMBNAIL_VIDEOQUEUE)); 
        MMIAPIVP_ThumbnailVideoAdd(full_path_name_ptr);// ��Ƶ����������ȡʱ���������Ƶ���б�
        result=Thumbnail_VideoQueueRun();// ��Ƶ����������ȡʱ����ʼ ��Ƶ����������ȡ
        if(!result)//����ʧ�ܣ����йر� 
        {
            MMIAPIVP_ThumbnailClose();
        }
       
    }
    else
    {
        MMIAPICOM_Wstrncpy(s_thumbnail_info.full_path_ptr, full_path_name_ptr,full_path_len);
        s_thumbnail_info.full_path_len=full_path_len;
       if(Thumbnail_OpenDplayer(full_path_name_ptr) != SCI_SUCCESS)//����ʧ�ܣ����йر� 
        {
              MMIAPIVP_ThumbnailClose();
        }
       else
        {
            result=TRUE;
        }
    }
#endif
	SCI_TRACE_LOW("[VP]:MMIAPIVP_ThumbnailOpen result=%d", result);
    return result;
}



/*****************************************************************************/
//  Description : VP_ThumbnailOpenDPLAYER , ��������ͼ��ȡ���� ���򿪳ɹ�s_thumbnail_info.is_open��Ϊtrue
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailOpenByHandle(
                                      VP_HANDLE  vphandle,
                                      uint32            seektime,      //in��Ҫ��ȡ����ͼ�ı�����Ƶ�ļ�����ʱ���
                                      BOOLEAN  (*vp_callback)(DPLAYER_IMAGE_DATA_T *image_data),//in,�Ƿ�Ϊ��Ƶ�б�������ȡ
                                      DPLAYER_DISPLAY_PARAM_T *display_param)//in,DPLAYER���Ų���  
{

    DPLAYER_RETURN_E    dplayer_result=DPLAYER_ERROR;
    DPLAYER_HANDLE dphandle=PNULL;
    
     BOOLEAN result=FALSE;
     SCI_TRACE_LOW("[VP]:MMIAPIVP_ThumbnailOpenByHandle !dphandle:0x%0x,seektime:%d",vphandle,seektime);
     
     
    if(vphandle == NULL )
    {
        return FALSE;
    }
       
    if(s_thumbnail_info.is_open)
    {
        //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailOpen is open now!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_315_112_2_18_3_5_26_380,(uint8*)"");
        return FALSE;
    }
    dphandle=((VP_FSM_T *)vphandle) ->dplayer_handle;
    SCI_MEMSET((void*)(&s_thumbnail_info),0x00,sizeof(VP_THUMBNAIL_T));
    //s_thumbnail_info.is_loading=TRUE;
    s_thumbnail_info.is_open=TRUE;
    s_thumbnail_info.seektime=seektime;
    s_thumbnail_info.lcd_angle=GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID);
    s_thumbnail_info.tn_callback= vp_callback;
    s_thumbnail_info.taskid=P_APP;
    s_thumbnail_info.is_videoqueue=FALSE;
    s_thumbnail_info.is_dplayerhandle = TRUE;
    if(display_param != NULL)
    {
        SCI_MEMCPY((void *)(&s_thumbnail_info.display_param), (void *)display_param, sizeof(DPLAYER_DISPLAY_PARAM_T));
    }
    s_thumbnail_info.dphandle = dphandle;
    s_thumbnail_info.is_loading=TRUE; // ��������һ������  
    s_is_dplayer_stopingstate=FALSE;
    

    if(s_thumbnail_info.dphandle > 0)
    {

        if(display_param != NULL)
        {
            MMIAPIVP_FSMSwitchAudioSupport(vphandle,FALSE);
            MMIAPIVP_FSMSwitchDisplayMode(vphandle,display_param,TRUE);   
        }
	
        DPLAYER_SetFrameEndNotify(s_thumbnail_info.dphandle, CallbackOneFrameEndNotify);
        dplayer_result=DPLAYER_Resume(s_thumbnail_info.dphandle); 
        SCI_TRACE_LOW("[VP]:MMIAPIVP_ThumbnailOpenByHandle DPLAYER_Play suceess,dplayer_result :%d!",dplayer_result);
        if(dplayer_result == DPLAYER_SUCCESS)
        {
            result=TRUE;
        }

    }

    if(!result)
    {
        SCI_MEMSET((void*)(&s_thumbnail_info),0x00,sizeof(VP_THUMBNAIL_T));//����״̬
    }

    return result;
}



/*****************************************************************************/
//  Description : ��Ƶ����������ȡʱ���ָ� ��Ƶ����������ȡ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailResume(void) 
{
    BOOLEAN result=FALSE;

     //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailResume !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_384_112_2_18_3_5_26_383,(uint8*)"");
     
    if(s_thumbnail_info.is_loading)
    {
        //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailResume  thumnail is loading!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_388_112_2_18_3_5_26_384,(uint8*)"");
        return FALSE;
    }
    if(s_thumbnail_info.videoqueue->videonum >0)
    {
        Thumbnail_VideoQueueRun();
    }

    return result;
}

/*****************************************************************************/
//  Description : ��Ƶ����������ȡʱ����ʱ���  ��Ƶ����������ȡ��ͨ��VP_ThumbnailResume �ָ� 
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
PUBLIC BOOLEAN MMIAPIVP_ThumbnailStop(void) 
{
    BOOLEAN result=FALSE;

     //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailStop !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_407_112_2_18_3_5_26_385,(uint8*)"");
     
    if(s_thumbnail_info.is_loading)
    {
        if(s_thumbnail_info.dphandle) //�ر� ��ǰ��  DPLAYER
        {
            DPLAYER_Stop(s_thumbnail_info.dphandle); 
            DPLAYER_Close(s_thumbnail_info.dphandle);
            DPLAYER_Release();
            s_thumbnail_info.dphandle=PNULL;
            result=TRUE;
        }
    }
   s_thumbnail_info.is_loading=FALSE;
   s_is_dplayer_stopingstate=FALSE;
    return result;
}
/*****************************************************************************/
//  Description : �ر�����ͼ��ȡ���� ��ֹͣdplyer������״̬���������Ƶ���У����ͷ��б������Դ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
PUBLIC  void  MMIAPIVP_ThumbnailClose(void)
{
        //SCI_TRACE_LOW:"[VP]:MMIAPIVP_ThumbnailClose!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_430_112_2_18_3_5_26_386,(uint8*)"");
        if(!s_thumbnail_info.is_dplayerhandle)
        {
            if(s_thumbnail_info.dphandle)//ֹͣdplyer
            {
                DPLAYER_Stop(s_thumbnail_info.dphandle); 
#ifdef MMI_KING_MOVIE_SUPPORT
            VP_dplayerExtCtlDeltSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.dphandle, s_thumbnail_info.sec_handle);
#endif
            DPLAYER_Close(s_thumbnail_info.dphandle);
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerCloseSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.sec_handle,s_thumbnail_info.b_sprd_kmv,PNULL);
#endif    
                DPLAYER_Release();
            }
        }
        if(s_thumbnail_info.videoqueue !=   PNULL)//�ͷ��ڴ� 
        {
            SCI_Free(s_thumbnail_info.videoqueue);
            s_thumbnail_info.videoqueue = PNULL;
        }
        SCI_MEMSET((void*)(&s_thumbnail_info),0x00,sizeof(VP_THUMBNAIL_T));//����״̬
}

/*****************************************************************************/
//  Description : ��Ƶ����������ȡʱ����Ҫ��ȫֹͣ��ǰ��Ƶ�����ܽ�����һ�� ����ͼ��ȡ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL  void  StopCurrentDPLAYER(void)
{
        //SCI_TRACE_LOW:"[VP]:StopCurrentDPLAYER!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_458_112_2_18_3_5_26_387,(uint8*)"");
        if(s_thumbnail_info.dphandle)
        {
            s_is_dplayer_stopingstate=TRUE;
            DPLAYER_Stop(s_thumbnail_info.dphandle); 
        }
}

/*****************************************************************************/
//  Description : ��Ƶ����������ȡʱ����ʼ ��Ƶ����������ȡ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  Thumbnail_VideoQueueRun(void)
{
    BOOLEAN result=FALSE;
    //SCI_TRACE_LOW:"[VP]:Thumbnail_VideoQueueRun  !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_473_112_2_18_3_5_26_388,(uint8*)"");

   if( !s_thumbnail_info.is_videoqueue)//��־λ�Ƿ���ȷ 
    {
         //SCI_TRACE_LOW:"[VP]:Thumbnail_VideoQueueRun is not videoqueue !"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_477_112_2_18_3_5_26_389,(uint8*)"");
         return result;
    }
    if(s_thumbnail_info.videoqueue->videonum == 0)//�����Ƿ�Ϊ�� 
    {
         //SCI_TRACE_LOW:"[VP]:Thumbnail_VideoQueueRun  queue empty !"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_482_112_2_18_3_5_26_390,(uint8*)"");
         return result;
    }
    while((s_thumbnail_info.videoqueue->videonum > 0))
    {
        if (Thumbnail_OpenDplayer(s_thumbnail_info.videoqueue->thumbnail_name[0]) == SCI_SUCCESS)
        {
            result=TRUE;
            break;
        }
        else
        {
            //SCI_TRACE_LOW:"[VP]:Thumbnail_VideoQueueRun   fail !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_494_112_2_18_3_5_26_391,(uint8*)"");
            Thumbnail_VideoDelete();//���Ų��ɹ���ɾ����������һ�� 
        }
    }

        return result;
}


/*****************************************************************************/
//  Description : Get the supported_type of a movie.
//  Global resource dependence :  none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL  VP_SUPPORTED_STATUS_TYPE_E CheckSupportedTypeFromMedia(DPLAYER_MEDIA_INFO_T *media_ptr)
{
    VP_SUPPORTED_STATUS_TYPE_E      supported_type  = VP_SUPPORTED_STATUS_TYPE_MAX;
    
    if (PNULL == media_ptr)
    {
        //VP_STRACE_LOW:"[VP] CheckSupportedTypeFromMedia param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_513_112_2_18_3_5_26_392,(uint8*)"");
        return VP_SUPPORTED_NEITHER;
    }
    //VP_STRACE_LOW:"[VP] CheckSupportedTypeFromMedia video_support = %d, audio_support = %d , video_type = %d, audio_type = %d, height = %d, width = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_522_112_2_18_3_5_26_393,(uint8*)"dddddd", media_ptr->video_info.video_support,media_ptr->audio_info.audio_support,media_ptr->video_info.video_type,media_ptr->audio_info.audio_type,media_ptr->video_info.height,media_ptr->video_info.width);

    if (1 == media_ptr->audio_info.audio_support
        && 1 == media_ptr->video_info.video_support)
    {
        supported_type = VP_SUPPORTED_BOTH;
    }
    else if (1 == media_ptr->audio_info.audio_support)
    {
        supported_type = VP_SUPPORTED_AUDIO_ONLY;
    }
    else if(1 == media_ptr->video_info.video_support)
    {
        supported_type = VP_SUPPORTED_VIDEO_ONLY;
    }
    else
    {
        supported_type = VP_SUPPORTED_NEITHER;
    }   
    
    return supported_type;
}

/*****************************************************************************/
//  Description :  ��Dplayer��Ϊ��ȡfull_path_ptr ����ͼ
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
LOCAL  DPLAYER_RETURN_E   Thumbnail_OpenDplayer(wchar *full_path_ptr) 
{

    DPLAYER_RETURN_E    dplayer_result=DPLAYER_ERROR;
    void                    *argv[2]        = {0}; 
    DPLAYER_MEDIA_INFO_T    p_info={0}; 
    VP_SUPPORTED_STATUS_TYPE_E  support_type=VP_SUPPORTED_STATUS_TYPE_MAX;

     //SCI_TRACE_LOW:"[VP]:Thumbnail_OpenDplayer !"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_557_112_2_18_3_5_26_394,(uint8*)"");
     if(s_thumbnail_info.is_loading)
    {
        //SCI_TRACE_LOW:"[VP]:Thumbnail_OpenDplayer is loading now!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_560_112_2_18_3_5_26_395,(uint8*)"");
        return dplayer_result;
    }

    SCI_MEMSET(s_thumbnail_info.full_path_ptr, 0x00, MMIFMM_FILE_FILE_NAME_MAX_LEN*sizeof(wchar));
    MMIAPICOM_Wstrcpy(s_thumbnail_info.full_path_ptr, full_path_ptr); 
    #ifdef MMI_KING_MOVIE_SUPPORT
    s_thumbnail_info.b_sprd_kmv=FALSE;
    s_thumbnail_info.sec_handle=PNULL;
    #endif
    s_thumbnail_info.is_loading=TRUE; // ��������һ������  
    s_is_dplayer_stopingstate=FALSE;
    
    argv[0] = (void*)full_path_ptr;
    argv[1] = (void *)((uint32)DPLAYER_FA_MOD_RB);
    DPLAYER_Init();

#ifdef MMI_KING_MOVIE_SUPPORT
        MMIAPIKM_CkeckSupportfile(s_thumbnail_info.full_path_ptr);
        s_thumbnail_info.sec_handle = VP_dplayerOpenSecurty(argv[0],&(s_thumbnail_info.b_sprd_kmv), PNULL);
#endif
    s_thumbnail_info.dphandle = DPLAYER_Open(DPLAYER_OPEN_FROM_FILE, 2, argv);
    //SCI_TRACE_LOW:"[VP]:Thumbnail_OpenDplayer   s_thumbnail_info.dphandle:0X%0x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_582_112_2_18_3_5_26_396,(uint8*)"d",s_thumbnail_info.dphandle);
    if(s_thumbnail_info.dphandle > 0)
    {
#ifdef MMI_KING_MOVIE_SUPPORT   
		VP_dplayerExtCtlAddSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.dphandle);
#endif        

        dplayer_result = DPLAYER_GetMovieInfo(s_thumbnail_info.dphandle,&p_info);
        if (DPLAYER_ERROR_UNSUPPORTED_FORMAT == dplayer_result)
        {
            support_type = VP_SUPPORTED_NEITHER;
        }
        else 
        {
            support_type = CheckSupportedTypeFromMedia(&p_info);
        }
        if(support_type != VP_SUPPORTED_BOTH  && support_type !=VP_SUPPORTED_VIDEO_ONLY )
        {
            //SCI_TRACE_LOW:"[VP]:Thumbnail_OpenDplayer CheckSupportedTypeFromMediaInfo fail !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_600_112_2_18_3_5_26_397,(uint8*)"");
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerExtCtlDeltSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.dphandle, s_thumbnail_info.sec_handle);
#endif
            DPLAYER_Close(s_thumbnail_info.dphandle);
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerCloseSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.sec_handle,s_thumbnail_info.b_sprd_kmv, PNULL);
#endif
                DPLAYER_Release();
                s_thumbnail_info.dphandle=PNULL;
                s_thumbnail_info.is_loading=FALSE; 
                return  DPLAYER_ERROR;  
        }
        
        dplayer_result      = DPLAYER_SetDisplayParam(s_thumbnail_info.dphandle,&(s_thumbnail_info.display_param));
        if(dplayer_result == SCI_SUCCESS)
        {
             if(DPLAYER_SetTime(s_thumbnail_info.dphandle,s_thumbnail_info.seektime)  == SCI_SUCCESS)
            {
                DPLAYER_SetFrameEndNotify(s_thumbnail_info.dphandle, CallbackOneFrameEndNotify);
                        //���ò��Ž����Ļص�����
                DPLAYER_SetEndNotify(s_thumbnail_info.dphandle, CallbackThumbnaiEndNotify);     
                dplayer_result=DPLAYER_Play(s_thumbnail_info.dphandle); 
                if(dplayer_result != DPLAYER_SUCCESS)
                {
                    //SCI_TRACE_LOW:"[VP]:Thumbnail_OpenDplayer DPLAYER_Play fail !"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_625_112_2_18_3_5_27_398,(uint8*)"");
                        #ifdef  MMI_KING_MOVIE_SUPPORT 
                            VP_dplayerExtCtlDeltSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.dphandle, s_thumbnail_info.sec_handle);
                        #endif
                        DPLAYER_Close(s_thumbnail_info.dphandle);
                        #ifdef  MMI_KING_MOVIE_SUPPORT 
                            VP_dplayerCloseSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.sec_handle,s_thumbnail_info.b_sprd_kmv,PNULL);
                        #endif
                        DPLAYER_Release();
                        s_thumbnail_info.dphandle=PNULL;
                        s_thumbnail_info.is_loading=FALSE; 
                    }
                }
                 else
                {
                    //SCI_TRACE_LOW:"[VP]:Thumbnail_OpenDplayer DPLAYER_SetTime fail !"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_640_112_2_18_3_5_27_399,(uint8*)"");
                    #ifdef  MMI_KING_MOVIE_SUPPORT 
                        VP_dplayerExtCtlDeltSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.dphandle, s_thumbnail_info.sec_handle);
                    #endif
                    DPLAYER_Close(s_thumbnail_info.dphandle);
                    #ifdef  MMI_KING_MOVIE_SUPPORT 
                        VP_dplayerCloseSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.sec_handle,s_thumbnail_info.b_sprd_kmv, PNULL);
                    #endif
                    DPLAYER_Release();
                    s_thumbnail_info.dphandle=PNULL;
                    s_thumbnail_info.is_loading=FALSE; 

                }
            }
            else
            {
                //SCI_TRACE_LOW:"[VP]:Thumbnail_OpenDplayer DPLAYER_SetDisplayParam fail !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_656_112_2_18_3_5_27_400,(uint8*)"");
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerExtCtlDeltSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.dphandle, s_thumbnail_info.sec_handle);
#endif
            DPLAYER_Close(s_thumbnail_info.dphandle);
#ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerCloseSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.sec_handle,s_thumbnail_info.b_sprd_kmv,PNULL);
#endif
                DPLAYER_Release();
                s_thumbnail_info.dphandle=PNULL;
                s_thumbnail_info.is_loading=FALSE; 
            }
        
    }
    else
    {
        //SCI_TRACE_LOW:"[VP]:Thumbnail_OpenDplayer DPLAYER_Open fail !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_672_112_2_18_3_5_27_401,(uint8*)"");
    #ifdef  MMI_KING_MOVIE_SUPPORT 
            VP_dplayerCloseSecurty(s_thumbnail_info.full_path_ptr,s_thumbnail_info.sec_handle,s_thumbnail_info.b_sprd_kmv,PNULL);
    #endif
        s_thumbnail_info.dphandle=PNULL;
        s_thumbnail_info.is_loading=FALSE; 
        dplayer_result = DPLAYER_ERROR;
    }
    return dplayer_result;
}

/*****************************************************************************/
//  Description :��Ƶ����������ȡʱ���Ӷ����� ɾ�� ��Ƶ���е�һ��
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/  
LOCAL BOOLEAN Thumbnail_VideoDelete(void)
{
    BOOLEAN result=FALSE;
    uint8 indx=0;
    VP_THUMBNAIL_VIDEOQUEUE     *videoqueue=s_thumbnail_info.videoqueue;
    
    //SCI_TRACE_LOW:"[VP]:Thumbnail_VideoDelete Video pre num:%d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_692_112_2_18_3_5_27_402,(uint8*)"d",videoqueue->videonum);
    if(videoqueue->videonum==0)
    {
        //SCI_TRACE_LOW:"[VP]:Thumbnail_VideoDelete  queue empty !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_695_112_2_18_3_5_27_403,(uint8*)"");
        SCI_MEMSET((void*)(videoqueue),0x00,sizeof(VP_THUMBNAIL_VIDEOQUEUE));
        return result;
    }
    for(indx=0;indx<videoqueue->videonum;indx++)
    {
        SCI_MEMSET(videoqueue->thumbnail_name[indx],0x00,(MMIFMM_FULL_FILENAME_LEN+1)*sizeof(wchar));
        MMIAPICOM_Wstrcpy(videoqueue->thumbnail_name[indx], videoqueue->thumbnail_name[indx+1]);
    }
    videoqueue->videonum--;
    //SCI_TRACE_LOW:"[VP]:Thumbnail_VideoDelete Video post num:%d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_705_112_2_18_3_5_27_404,(uint8*)"d",videoqueue->videonum);
    return result;
    
}


/*****************************************************************************/
//  Description : SendSignalToAPP
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SendSignalToCurPlayingTask(uint16 signalType, uint16 data, uint32 task_id)
{
    MmiAppCopyMsgS *sendSignal = PNULL;    
  //  VP_STRACE_LOW("[VP] SendSignalToCurPlayingTask signalType = %d, data = %d, task_id = %d",signalType,data,task_id);

    MmiCreateSignal(signalType, sizeof(MmiAppCopyMsgS), (MmiSignalS**)&sendSignal);   
    sendSignal->data = data;
    MmiSendSignal(task_id, (MmiSignalS*)sendSignal); 
}

/*****************************************************************************/
//  Description : frame end notify call back
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void CallbackThumbnaiEndNotify(int32 frame_num) 
{
    //SCI_TRACE_LOW:"[VP]:CallbackThumbnaiEndNotify,dphandle:0X%0x,is_loading:%d,is_stopping:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_732_112_2_18_3_5_27_405,(uint8*)"ddd", s_thumbnail_info.dphandle, s_thumbnail_info.is_loading ,s_is_dplayer_stopingstate);
    if (PNULL != s_thumbnail_info.dphandle  && s_thumbnail_info.is_loading )//&& s_is_dplayer_stopingstate
    {
        SendSignalToCurPlayingTask(APP_VP_ONE_FRAME_END, (uint16)frame_num, s_thumbnail_info.taskid);   
    }   
}  

/*****************************************************************************/
//  Description : frame end notify call back
//  Global resource dependence : 
//  Author:  
//  Note: 
/*****************************************************************************/
LOCAL void CallbackOneFrameEndNotify(int32 frame_num) 
{
    //SCI_TRACE_LOW:"[VP]:CallbackOneFrameEndNotify,dphandle:0X%0x,is_loading:%d,is_stopping:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVP_THUMBNAIL_746_112_2_18_3_5_27_406,(uint8*)"ddd", s_thumbnail_info.dphandle, s_thumbnail_info.is_loading ,s_is_dplayer_stopingstate);
    if (PNULL != s_thumbnail_info.dphandle  && s_thumbnail_info.is_loading)
    {
        SendSignalToCurPlayingTask(APP_VP_ONE_FRAME_REARCH, (uint16)frame_num, s_thumbnail_info.taskid);   
    }   
}  
#endif

