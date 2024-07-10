/****************************************************************************
** File Name:      mmi_appmsg.h                                            *
** Author:                                                                 *
** Date:           03/01/2005                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                  message                                                *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2005       Great.Tian         Create                                 *
**                                                                         *
****************************************************************************/
#ifndef _MMI_MSG_H_    
#define _MMI_MSG_H_   


/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif
 
/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/

#include "mmk_type.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmi_osbridge.h" 
//#include "env_mn_signal.h"    
#include "sig_code.h"   
/**-----------------------------------------------------------------------------*
 **                         Type Declaration                                    *
 **----------------------------------------------------------------------------*/
//以下的消息id定义范围为各个应用模块使用，id的范围为0x0100~ 0xe000,
//0x1500是APP_TIMER_EXPIRY，禁止使用

//SMS的范围为：  0x0100~~0x01ff    
#define MSG_SMS_GETPHONENUM			0x0100
#define MSG_SMS_DELSELECTED			0x0101
#define MSG_SMS_DELETE				0x0102
#define MSG_SMS_SELECTRESULT		0x0103
#define MSG_SMS_SETWORD				0x0104
#define MMISMS_UPDATE_LIST			0x0105
#define MSG_SMS_PLAY_RING			0x0106
#define MSG_SMS_FINISH_READ			0x0107
#define MSG_SMSCB_LIST_UPDATE		0x0108
#define MSG_SMS_CLOSE_WAIT_WIN		0x0109
#define MSG_SMS_VCARD_FILE_NAME     0x010a
#define	MMISMS_MSG_NEEDUPDATELIST   0x010b
#define MMISMS_MSG_UPDATELIST       0x010c
#define MMISMS_MSG_NEEDUPDATEMENU   0x010d
#define MMISMS_MSG_UPDATEMENU       0x010e
#define MMISMS_MSG_DELETE_NEXT      0x010f
#define MMISMS_MSG_READ_MSG         0x0110
#define MMISMS_MSG_CLOSE_UDISK_WIN  0x0111
#define MMISMS_MSG_UPDATE_NUMBER    0x0112
#define MMISMS_MSG_MOVE_NEXT        0x0113
#define MMISMS_MSG_EXPORT_NEXT      0x0114
#define MMISMS_MSG_COPY_NEXT        0x0115
#define MMISMS_MSG_UPDATE_TIMER     0x0116
#define MSG_SMS_NEW_SMS  0x0117
//MMS的范围为： 0x0200~~~0x02ff 
#define MSG_MMS_RECEIVED	        0x0200	//接收到彩信消息（支持后台的情况：下载完成后上报该消息；不支持后台的情况：接收到PUSH时上报该消息）
#define MSG_MMS_AUTODOWNLOAD_IND	0x0201	//启动自动下载消息
#define MSG_MMS_NETLINK_SUCCESS	    0x0202	//pdp激活成功消息
#define MSG_MMS_NETCONNECT_SUCCESS	0x0203	//网关连接或socket连接成功消息
#define MSG_MMS_DATA_RECEIVED	    0x0204	//收到数据消息
#define MSG_MMS_DATA_SENT	        0x0205	//发出数据消息
#define MSG_MMS_SEND_END	        0x0206	//发送结束消息
#define MSG_MMS_RECV_END	        0x0207	//接收结束消息
#define MSG_MMS_SAVE_END	        0x0208	//保存结束消息
#define MSG_MMS_DELETEMMS_END	    0x0209	//删除单条MMS结束消息
#define MSG_MMS_NETDISCONNECT	    0x020a	//彩信PDP去活完成
#define MSG_MMS_PLAY_SLIDE          0x020b
#define MSG_MMS_SEND_RETRYING	    0x020c	//彩信发送重试
#define MSG_MMS_RECV_RETRYING	    0x020d	//彩信接收重试
#define MSG_MMS_END_VIDEO           0x020e
#define MSG_MMS_PLAY_VIDEO          0x020f    
#define MSG_MMS_ACTIVE_RECV_SEND    0x0210  //彩信接收和发送激活使用消息
#define MSG_MMS_FINISH_READ         0x0211  //会话模式下获取彩信相关信息使用的消息
#define MSG_MMS_UPDATE_CHAT         0x0212

//wap的范围为：0x0300~~~0x03ff
#define MSG_BRW_START_ID				0x0300	//=== 浏览器消息号起始值 ===
#define MSG_BROWSER_MANAGER_START_ID				0x0370	//=== 浏览器管理消息号起始值 ===

		//=== 浏览器初始化消息 (0x0300---0x036f)===
#ifdef BROWSER_SUPPORT_DORADO
#define MSG_BRW_MSG_INIT_CNF                MSG_BRW_START_ID
#define MSG_BRW_MSG_EXIT_CNF                (MSG_BRW_START_ID+1)
#define MSG_BRW_MSG_TYPE_ENTER_FILENM_REQ   (MSG_BRW_START_ID+2)
#define MSG_BRW_MSG_TYPE_DL_CONFIRM_REQ     (MSG_BRW_START_ID+3)
#define MSG_BRW_MSG_TYPE_AUTH_REQ           (MSG_BRW_START_ID+4)
#define MSG_BRW_MSG_TYPE_CHANGE_URL_IND     (MSG_BRW_START_ID+5)
#define MSG_BRW_MSG_TYPE_NEW_PAGE_REQ_IND   (MSG_BRW_START_ID+6)
#define MSG_BRW_MSG_TYPE_UPDATE_CONTENT_IND (MSG_BRW_START_ID+7)
#define MSG_BRW_MSG_TYPE_PROGRESS_IND       (MSG_BRW_START_ID+8)
#define MSG_BRW_MSG_TYPE_ERROR_IND          (MSG_BRW_START_ID+9)    
#define MSG_BRW_MSG_TYPE_DOWNLOAD_OK_IND    (MSG_BRW_START_ID+10) 
#define MSG_BRW_MSG_TYPE_DOWNLOAD_ERR_IND   (MSG_BRW_START_ID+11)
#define MSG_BRW_MSG_TYPE_PAGE_GET_IND       (MSG_BRW_START_ID+12)		//获取到page的get cnf后利用此消息告知APP子媒体个数
#define MSG_BRW_MSG_TYPE_MEDIA_GET_ONE_IND  (MSG_BRW_START_ID+13)	//获取到一个子媒体时利用此消息告知APP
#define MSG_BRW_MSG_TYPE_DISPLAY_READY_IND  (MSG_BRW_START_ID+14)
#define MSG_BRW_MSG_TYPE_SSL_CERT_IND       (MSG_BRW_START_ID+15)
#define MSG_BRW_MSG_TYPE_REPAINT_READY_IND  (MSG_BRW_START_ID+16)
#define MSG_BRW_MSG_TYPE_PAGE_STOP_IND      (MSG_BRW_START_ID+17)
#define MSG_BRW_MSG_TYPE_OMA_DL_CONFIRM_REQ (MSG_BRW_START_ID+18)
#define MSG_BRW_MSG_TYPE_LAYOUTCTRL_DESTROY_REQ     (MSG_BRW_START_ID+19)
#define MSG_BRW_MSG_TYPE_PDP_ACTIVE_CNF     (MSG_BRW_START_ID+20)
#define MSG_BRW_MSG_TYPE_HTTP_REFRESH_IND            (MSG_BRW_START_ID+21)

#define	BRW_SNAPSHOT_FFS_PAGE_SAVE_CNF		(MSG_BRW_START_ID+30)
#define	BRW_SNAPSHOT_FFS_CSS_SAVE_CNF		(MSG_BRW_START_ID+31)
#define	BRW_SNAPSHOT_FFS_IMAGE_SAVE_ONE_CNF	(MSG_BRW_START_ID+32)
#define	BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF		(MSG_BRW_START_ID+33)
#define BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF		(MSG_BRW_START_ID+34)
#define	BRW_SNAPSHOT_FFS_IMAGE_OPEN_ONE_CNF	(MSG_BRW_START_ID+35)
#define MSG_BRW_ENTER_URL				    (MSG_BRW_START_ID+36)
#define MSG_BRW_PAINT_ADDRESSBAR_WHITE      (MSG_BRW_START_ID+37)
#define MSG_BRW_DELETE_SNAPSHOT             (MSG_BRW_START_ID+38)
#define MSG_BRW_EXIT_QUERY             (MSG_BRW_START_ID+39)//MS00191558是否退出浏览器
#define MSG_BRW_ENTER_SNAPSHOT_FILENAME				    (MSG_BRW_START_ID+40)
#define MSG_BRW_BK_OPEN_MAIN				    (MSG_BRW_START_ID+41)
#define MSG_BRW_BK_OPEN_MAIN_FINISH				    (MSG_BRW_START_ID+42)
#define MSG_BRW_BK_OPEN_STEP				    (MSG_BRW_START_ID+43)
#define MSG_BRW_BK_OPEN_STEP_FINISH				    (MSG_BRW_START_ID+44)
#define MSG_BRW_BK_SAVE				    (MSG_BRW_START_ID+45)
#define MSG_BRW_BK_SAVE_FINISH				    (MSG_BRW_START_ID+46)
#define MSG_BRW_MOVE_BOOKMARK             (MSG_BRW_START_ID+47) //
#define MSG_BRW_DELETE_BOOKMARK             (MSG_BRW_START_ID+48)
#define MSG_BRW_LOCAL_SAVE				    (MSG_BRW_START_ID+49)
#define MSG_BRW_STOP_BGSOUND            (MSG_BRW_START_ID+50)
#endif  //BROWSER_SUPPORT_DORADO
		//=== 浏览器管理初始化消息 (0x0370---0x03ff)===
#define MSG_BROWSER_MANAGER_MAKECALL_REQ   MSG_BROWSER_MANAGER_START_ID
#define MSG_BROWSER_MANAGER_SENDDTMF_REQ       (MSG_BROWSER_MANAGER_START_ID+1)
#define MSG_BROWSER_MANAGER_ADD_PBITEM_REQ     (MSG_BROWSER_MANAGER_START_ID+2)
#define MSG_BROWSER_MANAGER_OPEN_RTSP_IND      (MSG_BROWSER_MANAGER_START_ID+3)
#define MSG_BROWSER_MANAGER_SENDSMS_REQ        (MSG_BROWSER_MANAGER_START_ID+4)
#define MSG_BROWSER_MANAGER_SENDSMSTO_REQ      (MSG_BROWSER_MANAGER_START_ID+5)
#define MSG_BROWSER_MANAGER_SENDMMS_REQ        (MSG_BROWSER_MANAGER_START_ID+6)
#define MSG_BROWSER_MANAGER_SENDMMSTO_REQ      (MSG_BROWSER_MANAGER_START_ID+7)
#define MSG_BROWSER_MANAGER_SENDMAIL_REQ       (MSG_BROWSER_MANAGER_START_ID+8)
#define MSG_BROWSER_MANAGER_SENDMAILTO_REQ       (MSG_BROWSER_MANAGER_START_ID+9)
#define MSG_BROWSER_MANAGER_TEL_REQ   (MSG_BROWSER_MANAGER_START_ID+10)


//CC的范围为 0x0400~~~0x04ff
#define MSG_MMICC_START_ID 0x0400
#define MSG_MMICC_STARTTIMER (MSG_MMICC_START_ID+0)
#define MSG_CC_SHOW_CUSTOM_PHOTO_MSG (MSG_MMICC_START_ID+1)	
#define MSG_CC_ACCEPT_WAIT_CALL_MSG (MSG_MMICC_START_ID+2)	
#define MSG_CC_ALLOW_SEND_DEMF_MSG (MSG_MMICC_START_ID+3)	
#define MSG_CC_CONNECT_OPEN_WINDOW (MSG_MMICC_START_ID+4)	
#define MSG_CC_CONNECT_UPDATE_BUTTON (MSG_MMICC_START_ID+5)	
#define MSG_CC_RELEASE_ALL_CALL (MSG_MMICC_START_ID+6)	
#define MSG_CC_NOPS_ECC_READY_SIM_ID_MSG (MSG_MMICC_START_ID+7)
#define MSG_CC_STOP_APPLET (MSG_MMICC_START_ID+8)

//PB 的范围为： 0x0500~~~0x05ff
//#define MMI_PB_MULTI_SELECT_LIST_RETURN 	0x0500
#define MMI_SELECT_CONTACT_CNF              0X0500
#define MMI_PB_ALERT_CLOSE_RETURN   		0X0502
#define MMI_PB_UPDATE_NV_RETURN   		    0X0504
#define MMI_PB_UPDATE_ADD_IN_NV_LIST_MSG	0X0505
#define MMI_PB_UPDATE_SEARCH_LIST_MSG		0X0506
#define MMI_PB_RELOAD_SEARCH_LIST_MSG		0x0507
#define MMI_PB_UPDATE_ADD_IN_SIM_LIST_MSG	0X0508
#define MMI_PB_COPY_CNF_MSG                 0X0509
#define MMI_PB_COPY_IND						0X050a
#define MSG_FDN_PIN2_SUCCESS				0X050b //@zhaohui add for fdn ,fdn中验证PIN2码成功消息
//#define MSG_RELOAD_SEARCH_LIST			0x050c//@zhaohui add for fdn, FDN列表 reload
#define MSG_PB_PROECT_VERIFY_IND			0X050d//
#define MMI_PB_ERASE_IND                    0X050e//Earse NV Record msg send to pb app
#define MMI_PB_ERASE_CNF_MSG                0X050f//Earse NV Record msg send to pb windows
#define MMI_PB_MOVE_GROUP_IND               0X0510//Move group msg send to pb app
#define MMI_PB_MOVE_GROUP_CNF_MSG           0X0511//Move group msg send to pb app
#define MMI_PB_SET_WIN_TITLE                0X0512//set window title
#define MMI_PB_GET_ALL_FILE_FINISH          0X0513
#define MMI_PB_LOAD_NEXT                    0X0514
#define MMI_PB_VCARD_SELECTED               0X0515
#define MMI_PB_GROUP_CHANGED                0x0516
#define MMI_PB_ASP_FIXID_SELECTED  	        0X0517
#define MMI_PB_ASP_USERDEF_SELECTED  	    0X0518
#define MMI_PB_RESET_LIST_MSG               0x0519
#define MSG_GROUP_NAME_CHANGED	            0x051a	//
#define MMI_PB_SELECT_CANCLE                0x051c
#define MMI_PB_PHOTO_SET_DEFAULT      0x051d
#define MMI_PB_DELETE_ASP_ID                0x051f
#define MMI_PB_GROUP_NUM_CHANGED            0x0520
#define MMI_PB_CHANGE_NUMBER_TYPE             0x0521
#define MMI_PB_SET_PHOTO_FILE            0x0522
#define MMI_PB_SET_DEFAULT_RING             0x0523
#define MMI_PB_PIN2_VERIFY_OK              0x0524
#define MMI_PB_CHANGE_IM_TYPE             0x0525
#define MMI_PB_CHANGE_BIRTHDAY            0x0526
#define MMI_PB_OPERATE_OK                 0x0527
#define MMI_PB_OPERATE_SELECT_ALL         0x0528
#define MMI_PB_OPERATE_UNSELECT_ALL       0x0529
#define MMI_PB_COPY_LOCATION_CHANGED       0x0530
#define MMI_PB_OPERATE_SELECT_SIM         0x0531
#define MMI_PB_OPERATE_SELECT_PHONE       0x0532
#define MMI_PB_SNS_DETAIL_CHANGED       0x0533
#ifdef BT_DIALER_SUPPORT
#define MMI_PB_PBAP_CONTACT_READ_CNF      0x0534  
#define MMI_PB_PBAP_CALLLOG_READ_CNF      0x0535 
#endif
//Call Log的范围为：  0x0600~~~0x06ff
#define MSG_SIM_ERROR_UPDATE_ICON           0x0600  //是否刷新sim error or no sim界面的icon显示，目前只包括电量和信号
#define MSG_SIM_ERROR_UPDATE_MP3			0x0601  //sim error or no sim界面的icon显示mp3 name
#define MSG_CL_SINGLE_SELECT_LIST_RETURN	0x0602	//选中CL中的一条记录以后返回
#define MSG_LOAD_PROTECT_LIST               0x0603	//加载保护信息列表
#define MMI_PB_READ_CNF                     0x0604  //读回执
#define MMI_PB_WRITE_CNF                    0x0605  //写回执
#define MMI_PB_DELETE_CNF                   0x0606  //删除回执
#define MMI_PB_SORT_CNF                     0x0607  //排序结束
#define MSG_GROUP_CHANGED	                0x0608
#define MMI_CL_MULTI_SELECT_LIST_RETURN     0x0609  //选中CL中的一条或多条记录以后返回
#define MIPB_SIG_SEARCH                     0x0610
#define MMI_PB_SEARCH_CNF                   0x0611
#define MMI_PB_UPDATE_SNS_ACTIVITY_CNF      0x0612  //refresh sns activity
#define MMI_CL_MAINMENU_UPDATE_INDEX        0x0613
#define MMI_CL_RELOAD_DATA                  0x0614


        
//setting的范围为：    0x0700~~~0x07ff
#define MSG_SET_NEW_PIN_AGAIN				0x0700
#define MSG_SET_NEW_PWD_AGAIN				0x0701
#define MSG_SET_NEW_PRIVACY_PWD_AGAIN		0x0702
#define MSG_SET_CANCEL_SET_PRIVACY_PROTECT  0x0703
#define MSG_SET_VALIDATE_PRIVACY_SUCCESS 0x0704
#define MSG_SET_SELECT_FIX_CALL_RING_RETURN	0x0705
#define MSG_SET_RESET_FACTORY_OVER_IND		0x0706
#define MSG_SET_CLEAN_USER_DATA_OVER_IND	0x0707
#define MSG_SET_CLEAN_DATE_IND				0x0708
#define MSG_SET_POWER_OFF_IND				0x0709
#define MSG_SET_SIM_FOR_MULTIM_MUSIC        0x070a
#define MSG_SET_WALLPAPER_WAIT              0x070b
#define MSG_SET_UPDATE_ALARMSET				0x070c		
#define MSG_SET_UPDATE_ALARMSET_DONE		0x070d
#define MSG_SET_SPACE_FOR_RESTART_SETRAT	0x070e
#define MSG_SET_RESET_NEED_WAIT_IND		    0x070f //need wait 
#define MSG_SET_OPENALARM_EXPIRED_WIN		0x0710
#define MSG_SET_DEL_ALARMSET_CNF   		    0x0711 //delete one alarm confim
#define MSG_SET_SELECT_FIX_MSG_RING_RETURN	0x0712
#define MSG_SET_BLACKLIST_LIST_UPDATE   	0x0713
#define MSG_SET_LOAD_PREFER_PLMN_LIST       0x0714
#define MSG_SET_UPDATE_POWER_SAVE_SETTING   0x0715
#ifdef SLIDESHOW_WALLPAPER_SUPPORT
#define MSG_SET_SLIDESHOW_FIND_PHOTO   0x0716
#endif
#ifdef MMI_INDIAN_SOS_SUPPORT
#define MSG_SET_SOS_NUMBER_LIST_UPDATE   0x0717
#define MSG_SET_SOS_SETTINGS_TYPE_LIST_UPDATE   	0x0718
#endif

//dc和多媒体的范围为：    0x0800~~~0x08ff
#define		MSG_MULTIM_SELECTED_RETURN		0x0800	      		/*!< select one image or music, return its type and buf */
#define		MSG_MULTIM_CLOSE_WAITING_WIN	0X0801
#define		MSG_MULTIM_CLOSE_DELETEALL_WAITING_WIN		0X0802
#define     MSG_MPEG4_UNMARK_ALL			0X0803
#define     MSG_UDISK_START_OPERATION       0x0806
//#define     MSG_MP3_PLAY_FROM_LIST          0x0807
//#define     MSG_MP3_PLAY_CALL_BACK          0x0808 //Mp3正常播放结束的消息

#define     MSG_MP3_CONTINUE                0x080d  //恢复Mp3的播放
//#define     MSG_MPEG4_PLAY_CALL_BACK        0x080e  //mpeg4循环播放
#define     MSG_MPEG4_UNMARK_INDIVIDUAL        0x080e
#define     MSG_UDISK_REORDER				0x080f

#define     MSG_DC_PLAY_SHUTTER_VOICE   0x0810 
#define     MSG_MMI_WRITE_DPIC_CNF      0x0811
#define     MSG_MMI_WRITE_DRING_CNF     0x0812
#define     MSG_MMI_SHUTDOWN_FILE_CNF   0x0813
#define     MSG_MMI_DEACTIVE_PS_CNF     0x0814
#define     MSG_MMIDC_CLOSE_WAITING_WIN 0x0815
#define     MSG_DC_FILE_WRITE_DONE      0x0816
#define     MSG_DC_FILE_READ_DONE       0x0817
#define     MSG_DC_FILE_DEL_DONE        0x0818
#define     MSG_DC_OPEN_MAIN_SHOOT_MSG  0x0819

#define		MSG_DC_CAPTURE_SUCCESS_MSG	0X081a
#define		MSG_DC_CAPTURE_FAILE_MSG	0X081b

#define		MSG_DC_REVIEW_SUCCESS_MSG	0X081c
#define		MSG_DC_REVIEW_FAILE_MSG		0X081d

#define		MSG_DC_START_SHOOT_OPERATION	0x081e
#define     MSG_DC_RETURN_KEY           0X081f //MSG_DC_START_STORAGE_OPERATION	0X081f
#define     MSG_DC_MMI_CMD                  0x0820 //MSG_DC_COPY_READ_DONE


#define     MSG_DC_COPY_WRITE_DONE      0x0821
#define     MSG_DC_COPY_COMPLETE_DONE   0x0822
#define     MSG_DC_COPY_START_DONE      0x0823
#define     MSG_MULTIM_COPY_PIC_CNF          0x0824
#define     MSG_MULTIM_COPY_PIC_READ_DONE    0x0825
#define     MSG_MULTIM_COPY_PIC_WRITE_DONE   0x0826
#define     MSG_MULTIM_COPY_RING_CNF          0x0827
#define     MSG_MULTIM_COPY_RING_READ_DONE    0x0828
#define     MSG_MULTIM_COPY_RING_WRITE_DONE   0x0829

#define		MSG_DC_REVIEW_FULL_SCREEN_MSG		0x082a
#define		MSG_DC_OPEN_REVIEW_WIN_MSG			0x082b
#define		MSG_DC_AUTO_FOCUS_SUCCESS_MSG		0x082c
#define		MSG_DC_AUTO_FOCUS_FAILED_MSG		0x082d
#define    MSG_BRW_WRITE_FILE_CNF                0x082e  //@bei.wu for browser media 

//VC
#define		MSG_FILE_READ_WAITING_WIN	    0X0830
#define		MSG_FILE_WRITE_WAITING_WIN	    0X0831
#define		MSG_MMIVC_CLOSE_WAITING_WIN		0x0832	
#define		MSG_GET_DATA					0x0833	
#define		MSG_FILE_READ_MIDI_WAITING_WIN	0x0834
#define		MSG_VC_WAITWIN_LOSE_FOCUS	    0x0835

#define		MSG_MMICOPY_READ_CNF		    0x0836
#define		MSG_MMICOPY_WRITE_CNF		    0x0837
#define		MSG_MMICOPY_RESULT_CNF		    0x0838

#define     MSG_MPEG4_DISPLAY_CALLBACK      0x0839  
#define     MSG_MPEG4_END_CALLBACK          0x083a
#define     MSG_MPEG4_DELETE_ONE            0x083b      

#define     MSG_MPEG4_SNAPSHOT_WRITE_DONE   0x083c //@xingdong.li
#define     MSG_MPEG4_SNAPSHOT_NO_SPACE     0x083d //@xingdong.li

#define     MSG_MMI_START_FILE_CNF			0x0840 // @wancan.you udisk
#define     MSG_MMI_STARTUP_PS_CNF			0x0841 // @wancan.you udisk

#define     MSG_DV_FILE_DEL_DONE            0x0842
#define     MSG_RECORD_END                  0x0843

//#define     MSG_MPEG4_SKIP_CALLBACK         0x0844  
#define     MSG_MPEG4_LIST_MARK         0x0844  

#define		MSG_MMIDV_DEL_UNSAVED_MPEG4			0x0845
#define		MSG_MMIDV_CLOSE_RECORD_WIN			0x0846
#define     MSG_MPEG4_CURRENT_FILE_CHANGE	0x0847
#define     MSG_MPEG4_PLAY_CURRENT_FILE		0x0848
#define     MSG_MPEG4_PLAY_AFTER_INTERRUPT  0x0849
#define		MSG_MPEG4_GET_FOCUS_UPDATE		0X084a
#define		MSG_DV_SAVE_RECORD				0X084b
#define     MSG_DV_START_RECORD             0X084c

#define 	MSG_UDISK_START_UDISK           0x084f //james add for start udisk
#define 	MSG_UDISK_FORMAT_CNF			0x0850

#define     MSG_MPEG4_TIME_UPDATE			0x0851

#define     MSG_FM_PLAY_MSG                 0x0852
#define     MSG_FM_CONTINUE_MSG             0x0853
#define		MSG_DC_OPEN_MSG					0x0854
#define		MSG_DV_OPEN_MSG					0x0855
#define		MSG_DC_OPEN_STATUS_WIN_MSG		0x0856
#define		MSG_UDISK_USB_SERVICE_PC_CAMERA	0x0857

//MMI MPEG4 new file list
#define     MSG_MPEG4_UPDATE_LIST           0x0858
#define     MSG_MPEG4_FIND_ALL_MOVIE_FINISH 0x0859
#define     MSG_MPEG4_DEL_LIST_ITEM_START   0x085a
#define     MSG_MPEG4_DEL_NEXT_LIST_ITEM    0x085b
#define     MSG_MPEG4_OPEN_PLAYER           0x085c
#define     MSG_MPEG4_START_SEARCH          0x085d
#define     MSG_MPEG4_OPEN_MAIN_WIN         0x085e

//MMI MP3 MSG begin
#define MSG_MP3_INIT_WAIT                        0x085f
#define MSG_MP3_UPDATE_LIST                     0x0860
#define MSG_MP3_INIT_PLAYER_FINISH              0x0861
#define MSG_MP3_DEL_FILE_INFO                   0x0862
#define MSG_MP3_NEW_LIST_ADD_MUSIC_MSG          0x0863
#define MSG_MP3_LIST_MARKABLE                   0x0864
#define MSG_MP3_UPDATE_MARK_LIST                0x0865
#define MSG_MP3_DEL_MARK_INFO                   0x0866
#define MSG_MP3_OPEN_PLAYER                     0x0867
#define MSG_MP3_OPEN_PLAYER_WITH_INFO           0x0868
#define MSG_MP3_LOAD_ALL_LIST_FINISH            0x0869
#define MSG_MP3_LOAD_ALL_MUSIC_FINISH           0x086a
#define MSG_MP3_INIT_PLAYER_NEXT_STEP           0x086b
#define MSG_MP3_BEGIN_ADD_TO_LIST               0x086c
#define MSG_MP3_COPY_LIST                       0x086d
#ifdef MMIAP_LYRIC_SUPPORT
#define MSG_MP3_START_LIRIC                     0x086e
#endif
#define MSG_MP3_MUSIC_PLAY_END                  0x086f
#define MSG_MP3_PLAY_CUR_FILE                   0x0870
#define MSG_MP3_FOLDER_FULL                     0x0871
#define MSG_MP3_SHOW_SOFT_BAR                   0x0872
#ifdef MMIAP_LYRIC_SUPPORT
#define MSG_MP3_LYRIC_DISPLAY                   0x0873
#endif
#define MSG_MP3_PLAY_CALL_BACK                  0x0874
#define MSG_MP3_FATAL_ERROR                     0x0875
#define MSG_MP3_OPEN_MAIN_PLAY_WIN_SUCCESS      0x0876
#ifdef MMIAP_SORT_SUPPORT
#define MSG_APP_MP3_SORT                        0x0877
#endif
#define     MSG_MP3_DEL_CNF     0x0878
#define     MSG_MP3_DEL_DONE     0x0879
//pengliqing, 2006-6-2, multim module start
#define     MSG_MULTIM_WAITING                  0x087a
#define     MSG_FILE_READFILE_ASYN_CALLBACK     0x087b
#define     MSG_FILE_WRITEFILE_ASYN_CALLBACK    0x087c
#define     MSG_FILE_COPY_READ_CNF              0x087d
#define     MSG_FILE_COPY_WRITE_CNF             0x087e
#define     MSG_FILE_READFILE_BYOFFSET_ASYN_CALLBACK    0x087f
#define     MSG_FILE_WRITEFILE_BYOFFSET_ASYN_CALLBACK   0x0880
#define     MSG_FILE_ASYNOPERAT_FINISH          0x0881
//pengliqing, 2006-6-2, multim module end

//pengliqing, 2006-6-20, atc module start
#define     MSG_MMI_PC_SYNC_READY           0x0882
//pengliqing, 2006-6-20, atc module end
//usb message
#define     MSG_MMI_USB_SERVICE_STARTED     0x0883
#define     MSG_MMI_USB_SERVICE_STOPPED     0x0884

#define     MSG_MMI_AUTO_TEST_MODULE     0x0885

//pic
#define MSG_PIC_GET_DATA_CNF            0x0890  //vincent add at 03
#define MSG_PIC_DISPLAY_IND             0x0891  //vincent add for display decoded jpg
#define MSG_PIC_COPY_FILE               0x0892  //added, @robert.wang, 09-8-20, cr147752  for directly copy file 
#define MSG_PIC_CROP_SAVE            0x0893  //vincent add at 03
#define MSG_PICLIST_LOAD_END           0x0894
#define MSG_PICLIST_LOAD_STOP            0x0895
#define MSG_PICLIST_LOAD_START            0x0896
//DC/DV
#define MSG_DC_MMI_STATE               0x089A
#define MSG_REORD_PLAYING_PRGRESS_END_UPDATE      0x089B

//mp3
#define MSG_MP3_BG_PLAY_MUSIC     0x089C

#define MSG_MP3_LOAD_LISTS     0x089D  //bug 131703
#define MSG_MP3_SWITCH_BY_BT     0x089E  //Bug 187928
#define MSG_MP3_SWITCH_AUTO      0x089F 

//附件的范围：    0x0900~~~0x09ff
#define MSG_ACC_MSGBOX_CANCEL       0x0901
#define MSG_ACC_MSGBOX_OK           0x0902
#define MSG_ALM_ALERTING_END        0x0903             // send to virtual window in the state of power on
#define MSG_ALARM_UPDATE_PREVWIN	0x0904
#define MSG_TVOUT_STARTING      	0x0905
#define MSG_FEAST_UPDATE_IDLE_TXT	0x0906
#define MSG_FEAST_LIST_UPDATE       0x0907
#define MSG_FEAST_EDITRETURN        0x0908
#define	MSG_TVOUT_CLOSE_IND			0x0909
#define MSG_ACC_UPDATE_COUNTEDTIME	0x090a//倒计时刷新显示
#define	MSG_ALM_SELECTED_FM_RETURN		0x090b
#define MSG_SCH_GET_ALL_FILE_FINISH          0x090c
#define MSG_SCH_LOAD_NEXT_VCALENDAR          0x090e
#define		MSG_HANDLE_SDPLUG_IN_STEP_1		0x9037
#define		MSG_HANDLE_SDPLUG_IN_STEP_2		0x9038
#define		MSG_HANDLE_SDPLUG_IN_STEP_3		0x9039
#define		MSG_HANDLE_SDPLUG_OUT_STEP_1	0x903a
#define		MSG_HANDLE_SDPLUG_OUT_STEP_2	0x903b
#define		MSG_HANDLE_SDPLUG_OUT_STEP_3	0x903c
#define     MSG_HANDLE_SD_ERR               0x903d
#ifdef STOPWATCH_SUPPORT
#define MSG_ACC_UPDATE_STOPWATCH    0x090f
#endif
#define     MSG_ALM_START_RING               0x0910

#ifdef ZDT_ZFB_SUPPORT
#define     MSG_ZDB_STATUS_INIT               0x0940
#define     MSG_ZDB_STATUS_BIND               0x0941
#define     MSG_ZDB_STATUS_PAY                 0x0942
#define     MSG_ZDB_STATUS_OBTAIN            0x0943
#define     MSG_ZDB_STATUS_ERR                 0x0944
#define     MSG_ZDB_STATUS_ERR_INIT         0x0945
#define     MSG_ZDB_STATUS_ERR_BIND        0x0946
#define     MSG_ZDB_STATUS_ERR_PAY          0x0947
#define     MSG_ZDB_STATUS_ERR_OBTAIN     0x0948
#define     MSG_ZDB_BINDING_START              0x0949
#define     MSG_ZDB_BINDING_OK              0x094a
#define     MSG_ZDB_BINDING_ERROR        0x094b
#define     MSG_ZDB_PAYING_OK              0x094c
#define     MSG_ZDB_PAYING_ERROR        0x094d
#define     MSG_ZDB_UNBINDING_START              0x094e
#define     MSG_ZDB_UNBINDING_OK              0x094f
#define     MSG_ZDB_UNBINDING_ERROR        0x0950
#define     MSG_ZDB_LOGON_ID_OK              0x0951
#define     MSG_ZDB_LOGON_ID_ERROR        0x0952
#define     MSG_ZDB_NICK_NAME_OK              0x0953
#define     MSG_ZDB_NICK_NAME_ERROR        0x0954
#endif

//stk的范围为:     0x0a00~~~0x0aff
#define     MSG_STK_SET_MENU_ATV    0x0A00
#define     MSG_STK_SET_UP_IDLE_MODE    0x0A01

//pengliqing, 2006-6-20, schedule, alarm module start
#define     MSG_SCH_EVENT_EDIT_FINISH       0x0b01
#define     MSG_SCH_LIST_UPDATE             0x0b02
#define     MSG_SCH_LIST_DEL_ALL            0x0b03    // 删除所有日程
#define     MSG_SCH_LIST_DEL_ONE            0x0b04    // 删除指定的一个事件
#define     MSG_SCH_LIST_DEL_CURDATE        0x0b05    // 删除当前日期的所有日程
#define     MSG_ALM_SETTING_ALARM           0x0b06    // 闹钟设置
#define     MSG_SCH_LIST_UPDATE_DATE        0x0b07		//查看日程列表中更新
#define     MSG_SCH_EVENT_ARRIVE            0x0b08
#define     MSG_SCH_WIDGET_THUMBER           0x0b09 
#define     MSG_SCH_DAILY_UPDATE            0x0b0a
#define     MSG_SCH_WIDGET_UPDATE            0x0b0b

//pengliqing, 2006-6-20, schedule, alarm module end

//IDLE的范围为: 　0x0c00 ~ 0x0cff
#define		MSG_IDLE_UPDATE_DATETIME	0x0c01
#define		MSG_IDLE_UPDATE_ICON		0x0c02
#define		MSG_IDLE_UPDATE_NETWORK		0x0c03
#define		MSG_IDLE_VIBRA				0x0c04
#define		MSG_IDLE_UPDATE_MP3			0x0c05
#define		MSG_KEYLOCK					0x0c06
#define		MSG_POWER_ON_CHARGE_START	0x0c07
#define     MSG_IDLE_USB                0x0c08
#define     MSG_IDLE_SET_DATATIME       0x0c09
#define		MSG_KEY_LOCK_CLOSE_WIN		0x0c0a
#define		MSG_IDLE_UPDATE_COUNTEDTIME	0x0c0b//倒计时刷新显示，@zhaohui,counted time
#define		MSG_DIAL_UPDATE_DATETIME	0x0c0c
//@xiangjie idle module 2006.11.21 begin
#define		MSG_IDLE_SHORTCUT_UPDATE_FOCUS	0x0c0d
#define		MSG_IDLE_DIAL_GRAB_PARENT_WIN	0x0c0e
#define		MSG_IDLE_DIAL_PB_UPDATE_IND   0x0c0f
#define		MSG_IDLE_DIAL_RESET_EDIT_IND   0x0c10
//@xiangjie idle module 2006.11.21 end
//@xingdong.li add for key lock begin
#define		MSG_SLEEP_LCD	            0x0c0e
//@xingdong.li end
#define     MSG_KEYLOCK_MISSED_CALL      0x0c11
#define     MSG_KEYLOCK_UNREAD_MESSAGE   0x0c12
#define     MSG_KEYLOCK_UNDEAL_EVENT     0x0c13
#define     MSG_APPLET_MANAGER_UPDATE    0x0c14 //add for applet manager
#define		MSG_IDLE_UPDATE_WELCOME_WORDS   0x0c15
#define		MSG_IDLE_UPDATE_TIME_SECOND 	0x0c16


//WIDGET START
#define     MSG_WIDGET_TP_PRESS_DOWN     0x0c30 //add for widget tp press down
#define     MSG_WIDGET_TP_PRESS_LONG_DOWN     0x0c31 //add for widget tp press down
#define     MSG_WIDGET_TP_PRESS_UP       0x0c32 //add for widget tp press up
#define     MSG_WIDGET_TP_PRESS_MOVE     0x0c33 //add for widget tp press move
#define     MSG_WIDGET_TP_PRESS_LONG_UP  0x0c34 //add for widget tp press long up

#define     MSG_WIDGET_ACTIVE            0x0c40 //add for widget active
#define     MSG_WIDGET_DEACTIVE          0x0c41 //add for widget deactive
#define     MSG_WIDGET_MODIFY_RECT       0x0c42 //add for widget modify rect
#define     MSG_WIDGET_MOVE_START        0x0c43 //add for widget move start
#define     MSG_WIDGET_MOVE_END          0x0c44 //add for widget move end
#define     MSG_WIDGET_PAGE_SLIDE_START  0x0c45 //add for widget Slide page start
#define     MSG_WIDGET_PAGE_SLIDE_END    0x0c46 //add for widget Slide page end

#define     MSG_GRID_PEN_MENU_ITEM       0x0c50 //click menu item from grid idle.
#define     MSG_GRID_ADD_WIDGET          0x0c51  //ADD widget.
#define     MSG_GRID_REMOVE_WIDGET       0x0c52 //remove widget
#define     MSG_GRID_SLIDE_PAGE          0x0c53 //slide page msg.
#define     MSG_GRID_UPDATE_DATA         0x0c54 //post to update data.
#define     MSG_GRID_UPDATE_PAGE         0x0c55 //post to update grid page.
#define     MSG_GRID_DEACTIVE_WIDGET     0x0c56
#define     MSG_GRID_ACTIVE_WIDGET       0x0c57 
#define     MSG_GRID_PLAY_SOFTKEY_ENTER_ANIM       0x0c58 
#define     MSG_GRID_REMOVE_ITEM         0x0c59 //remove item message.

#define     MSG_GRID_UIDLE_UPDATE_VW     0x0c5A //to update videop wallpaper.
#define     MSG_WIDGET_UPDATE_SOFTKEY     0x0c5B //to update BARPHONE widget softkey

#define     MSG_WIDGET_ELVE_START_ANIM   0x0c61 //add for widget elve.
#define     MSG_WIDGET_ELVE_SET_STATUS   0x0c62 //add for widget elve.
#define     MSG_WIDGET_MOVE_BAR_TO_IDLE  0x0c65 //add for widget BAR_TO_IDLE
#define     MSG_WIDGET_MOVE_IDLE_TO_BAR  0x0c66 //add for widget IDLE_TO_BAR
#define		MSG_WIDGET_MP3_SWITCH		 0x0c67 //WIDGET MP3 SWITCH MUSIC

//WIDGET END

#define	    MSG_SIMLOCK_UPDATE_COUNTDOWN 0x0c1e //add for SIM Lock Countdown






#if defined(PDA_UI_DROPDOWN_WIN)
#define     MSG_DROPDOWNWIN_OPEN         0x0cf0 //add for dropdown win open
#define     MSG_DROPDOWNWIN_CLOSE         0x0cf1 //add for dropdown win close
#endif

//VCard:       0x0d000~ 0x0dff
#define    MSG_VCALENDAR_WRITE_SMS_CNF          0x0d04
#define    MSG_VCALENDAR_SAVE_SCH_CNF            0x0d05
#define    MSG_VCALENDAR_SAVE_IND              0x0d06
#define    MSG_SCH_DELETE_IND              0x0d07
#define    MSG_SCH_DELETE_CNF              0x0d08
//VCard end

//begin define the msg of blue tooth 0x0e000~ 0x0eff
#define	   MSG_BT_SEARCH_FINISHED		    0x0e00
#define	   MSG_BT_SEARCH_CANCELED 		    0x0e01
#define    MSG_BT_DEL_RECEIVED_RING         0x0e05
#define	   MSG_BT_ACCEPT_INCOMING_CALL		0x0e06
#define	   MSG_BT_REJECT_INCOMING_CALL		0x0e07
#define	   MSG_BT_CANCEL_OUTGOING_CALL		0x0e08
#define	   MSG_BT_CANCEL_CALL				0x0e09
#define	   MSG_BT_A2DP_PLAY					0x0e0a
#define	   MSG_BT_A2DP_STOP					0x0e0b
#define	   MSG_BT_A2DP_PAUSE				0x0e0c
#define	   MSG_BT_AUTO_CLOSE_WIN			0x0e0d
#define	   MSG_BT_DEVICE_LIST_UPDATE		0x0e0e
#define    MSG_BT_HS_SPK_VOL_CHANGE         0x0e0f  //mdy by qing.yu@for cr238867

#define    MSG_BT_UPDATE_MAINMENU			0X0e10	//add by arvin.zhang@for cr229544
#define    MSG_BT_CONTACT_SYNC_CNF          0X0e11  //add by yanyan.an for bt pdap
#define    MSG_BT_CALLLOG_SYNC_CNF          0X0e12  //add by yanyan.an for bt pdap
#define    MSG_BT_PBAP_TP_CONNECT_SUCCESS   0X0e13  //add by yanyan.an for bt pdap

#ifdef  BT_MAP_SUPPORT
#define    MSG_BT_MAP_CONNECT_SUCCESS          0X0e14  //for map
#define    MSG_BT_SET_FOLDER_SUCCESS           0X0e15  //for map
#define    MSG_BT_SET_FOLDER_LEVEL_1_SUCCESS   0X0e16  //for map
#define    MSG_BT_SET_FOLDER_LEVEL_2_SUCCESS   0X0e17  //for map
#define    MSG_BT_SET_FOLDER_LEVEL_3_SUCCESS   0X0e18  //for map
#define    MSG_BT_MSG_SYNC_CNF                 0X0e19  //for map
#define    MSG_BT_GET_FOLDER_LIST              0X0e1a  //for map
#define    MSG_BT_GET_MESSAGE_LIST             0X0e1b  //for map
#define    MSG_BT_MAP_REGISTER_SUCCESS         0x0e1c  //for map
#define    MSG_BT_PUSH_SUCCESS                 0x0e1d  //for map
#define    MSG_BT_MAP_EVENT_DELIVERY_SUCCESS   0x0e1e  //for map
#define    MSG_BT_SEND_ITEM                    0x0e1f  //for map
#define    MSG_BT_GET_MESSAGE_BODY             0X0e20 //for map
#endif
//end define the msg of blue tooth 

#if (defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)) || (defined(MMI_SMS_SYN_SUPPORT))
#define    MSG_BT_SPP_SEND_ITEM                0x0e21 //for spp
#endif

//@samboo vt user input
#define  APPSIG_MPLH324M_MSG_IND_USER_INPUT_ALPHANUMERIC 0x0e0f
//begin define the msg of streaming
#define APPSIG_MPLH324M_MSG_CLOSE_CNF   0x0e10
#define MSG_ST_UPDATA  0x0e11
//@zhaohui add for vt start
#define APPSIG_MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_START 0x0e12
#define APPSIG_MPLH324M_MSG_VIDEO_RX_NO_SIGNAL_STOP 0x0e13
#define APPSIG_MPLH324M_MSG_CLOSE_IND  0x0e14//@zhaohui add ,cr83903
#define APPSIG_MPLH324M_MSG_MEDIA_INACTIVE_IND 0x0e15
#define APPSIG_MPLH324M_MSG_MEDIA_ACTIVE_IND 0x0e16

#define MSG_VT_REF_ERROR_IND 0X0e17
#define APPSIG_MPLH324M_MSG_STARTMEDIA  0x0e18



#define MSG_VT_REMOTE_ALERTING_IND 0x1580
//@applezhang add for st start
#define  MSG_ST_CALLBACK          0x0e51
//@applezhang add for st alarm
#define  MSG_ST_ALARM_MSG         0x0e52
#define  MSG_ST_END_CALLBACK      0x0e53
#define  MSG_ST_GET_FOCUS_UPDATE  0x0e54
#define  MSG_ST_CURRENT_FILE_CHANGE 0x0e55
#define  MSG_ST_PLAY_CURRENT_FILE   0x0e56

#define  MSG_ST_BUFFERING_BEGIN   0x0e57
#define  MSG_ST_BUFFERING_END     0x0e58  
#define  MSG_ST_PLAY_ERROR        0x0e59
#define  MSG_ST_PLAY_STREAMING    0x0e5a  
//end define the msg of streaming

#define  MSG_AUDIO_CALLACK                  0x0e22
#define  MSG_AUDIO_LAST_KEY_END_NOTIFY      0x0e23

//mobile tv  0x0e30
#define	   MSG_MTV_ESG_SUC_CNF              0x0e30
#define	   MSG_MTV_ESG_ERR_IND              0x0e31
#define	   MSG_MBBMS_INIT_IND               0x0e32
#define	   MSG_MBBMS_PRE_EXIT_MSG           0x0e33
#define	   MSG_MBBMS_SETTING_SAVE          0x0e34
//mobile tv

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
#define MMISMS_BT_GET_FOLDER_LIST       0x0e61
#define MMISMS_BT_GET_FOLDER_LIST_CNF   0x0e62
#define MMISMS_BT_SET_FOLDER            0x0e63
#define MMISMS_BT_SET_FOLDER_CNF        0x0e64
#define MMISMS_BT_GET_MSG_LIST          0x0e65
#define MMISMS_BT_GET_MSG_LIST_CNF      0x0e66
#define MMISMS_BT_GET_MESSAGE           0x0e67
#define MMISMS_BT_REGISTER_NOTIFY       0x0e68
#define MMISMS_BT_GET_MESSAGE_CNF       0x0e69
#define MMISMS_MSG_BT_READ_MESSAGE      0x0e6a
#endif

// explorer msg 0x0f00 ~ 0x0f20
//#define  MSG_FMM_NEW_FOLDER_CNF                     0x0f00
//#define  MSG_FMM_SEARCH_CNF                         0x0f01
//#define  MSG_FMM_SORT_END_CNF                       0x0f02
//#define  MSG_FMM_RENAME_END_CNF                     0x0f03
//#define  MSG_FMM_DELETE_END_CNF                     0x0f04
//#define    MSG_FMM_OPEN_WIN_CHOOSE_PATH             0x0f05
//#define    MSG_FMM_COPY_CHOOSE_PATH_RETURN          0x0f06
//#define    MSG_FMM_OPERATE_ONE_FILE_RETURN          0x0f07
//#define    MSG_FMM_SELECT_MARK_FILE                 0x0f08
//#define    MSG_FMM_OPERATE_FILE_BEGIN               0x0f09
//#define    MSG_FMM_COPY_END_CNF                     0x0f0a
//#define    MSG_FMM_UPDATE_LIST_DATA                 0x0f0b
//#define    MSG_FMM_MUSIC_PLAY_END                   0x0f0c
//#define    MSG_FMM_OPENFILEWIN_STARTSEARCH          0x0f0d
//#define    MSG_FMM_OPENFILEWIN_SEARCH_END           0x0f0e
// #define    MSG_FMM_OPENFILEWIN_RELOAD               0x0f0f


#define	   MSG_FMM_NEW_FOLDER_CNF		            0x0f00
#define	   MSG_FMM_SEARCH_CNF		                0x0f01
#define	   MSG_FMM_SORT_END_CNF		                0x0f02
#define	   MSG_FMM_RENAME_END_CNF		            0x0f03
#define	   MSG_FMM_DELETE_END_CNF		            0x0f04
#define    MSG_FMM_COPY_FILE_CIRCLE                 0x0f05
#define    MSG_FMM_DEL_FILE_CIRCLE                  0x0f06
#define    MSG_FMM_OPEN_WIN_CHOOSE_PATH             0x0f07
#define    MSG_FMM_COPY_CHOOSE_PATH_RETURN          0x0f08
#define    MSG_FMM_OPERATE_ONE_FILE_RETURN          0x0f09
#define    MSG_FMM_SELECT_MARK_FILE                 0x0f0a
#define    MSG_FMM_OPERATE_FILE_BEGIN               0x0f0b
#define	   MSG_FMM_COPY_END_CNF		                0x0f0c
#define    MSG_FMM_COPY_FILE_WRITE_CNF              0x0f0d
#define    MSG_FMM_COPY_FILE_READ_CNF               0x0f0e
#define    MSG_FMM_UPDATE_LIST_DATA                 0x0f0f
#define    MSG_FMM_MUSIC_PLAY_END                   0x0f10
#define	   MSG_FMM_OPENFILEWIN_STARTSEARCH		    0x0f14
#define	   MSG_FMM_OPENFILEWIN_SEARCH_END		    0x0f15
#define	   MSG_FMM_OPENFILEWIN_RELOAD			    0x0f16
#define	   MSG_FMM_SET_SEARCH_SEL_DIRECTLY 		    0x0f17

#define    MSG_PUB_START_APP_WAIT                  0x0f18
#define    MSG_FMM_ABORT                           0x0f19
#define    MSG_FMM_FILE_EXIST                      0x0f1a
#define    MSG_FMM_MOVE_MARK_FILE                  0x0f1b
#define    MSG_FMM_MOVE_END_CNF                    0x0f1c
#define    MSG_FMM_CLOSE_LAYER_WIN_BY_CLIENT       0x0f1d
#define    MSG_FMM_FORMAT                          0x0f1e //CR188283
#define     MSG_KUR_PLAY_CALL_BACK              0x0f30
#define     MSG_KUR_START_READ_PIC              0x0f31
#define     MSG_KUR_PLAY_START                  0x0f32
#define     MSG_KUR_OPEN_PLAYER                 0x0f33
#define     MSG_KUR_OPEN_PLAYER_WITH_INFO       0x0f34
#define     MSG_KUR_LOAD_ALL_KURO_FINISH        0x0f35
#define     MSG_KUR_FATAL_ERROR                 0x0f36
#define     MSG_FS_DELETE_CNF                   0x0f37
#define	    MSG_FMM_SEARCH_ONE_FOLDER_CNF		0x0f38
#define     MSG_FMM_LIST_REPLACE_ITEM_DATA      0x0f39

#ifdef _SW_ZDT_PRODUCT_
#define     MSG_USRE_LOG_AT_SAVE			 0x0f1f
#define     MSG_USRE_LOG_SOCKET_SAVE		 0x0f20
#define     MSG_USRE_LOG_BUF_SAVE			 0x0f21
#define     MSG_USRE_LOG_EXT_SAVE			 0x0f22
#define     MSG_USRE_LOG_DEL_ALL			 0x0f23
#define     MSG_USRE_LOG_SAVE			     0x0f24
#endif

//explorer
//ocr
#define	MSG_OCR_RECOG_OVER	                	0X0fe0
#define	MSG_OCR_RECOG_FAIL	                	0X0fe1
#define	MSG_OCR_AF_SUCCESS                 		0X0fe2
#define	MSG_OCR_AF_FAIL                    	 	0X0fe3

//begin define the msg of eng 0X1101~ 0x11FF
#define MSG_ENG_PCLINK_PING_SUC 0X1101
#define MSG_ENG_PCLINK_PING_FAIL 0X1102
#define MSG_ENG_L1_GET_CNF      0X1103
#define MSG_ENG_L1_GET_CNF      0X1103
#define MSG_ENG_IQ_RESULT_CNF   0X1104
#define MSG_ENG_AUDIO_REFREH    0x1105
#define MSG_ENG_MTV_PLAY_CNF    0x1106
#define MSG_ENG_MTV_SEARCH_CNF  0x1107

#define MSG_ENG_UPGRADING_WARN  0x1108
#define MSG_ENG_UPGRADING_ALLOW  0x1109

#define MSG_ENG_MEMTESTER_CONTINUE_PLAY  0x110a
#define MSG_ENG_SENSORHUB_TEST       0x110b

#if defined ENG_BAND_FREQ_SUPPORT
#define  MSG_ENG_FREQ_EDIT_UPDATE           0x1110
#define  MSG_ENG_FREQ_LIST_UPDATE           0x1111
#endif
//FM Audio message begin :0X1201 
#define  MSG_APP_FM_ADJ_VOLUME           0x1201
#define  MSG_APP_FM_ADJ_FREQ             0x1202
#define  MSG_APP_FM_ADJ_CHANNEL          0x1203
#define  MSG_APP_FM_SWITCH_AUDIO_DEVICE  0x1204
#define  MSG_APP_FM_SEARCH_VALID_FREQ    0x1205
#define  MSG_APP_FM_FAIL                 0x1206
#define  MSG_APP_FM_EDIT_UPDATE          0x1207
#define  MSG_APP_FM_HEADSET_PLUG_OUT     0x1208
#define  MSG_APP_FM_HEADSET_PLUG_IN      0x1209
#define  MSG_APP_FM_AUTOSEARCH_START     0x120c
#define  MSG_APP_FM_AUTOSEARCH_END       0x120a
#define  MSG_APP_FM_GETSEARCH_PARAM      0x120b

//RECORD  message begin :0X1230 
#define     MSG_RECORD_START_PLAY           0X1231
#define     MSG_INSERT_RECORD               0X1232
#define     MSG_STOP_RECORD                 0X1233
#define     MSG_RECORD_CALL_BACK            0X1234
#define     MSG_START_RECORD               0X1235
#define     MSG_STOP_RECORD_PLAY            0X1236
#define     MSG_RECORD_PLAY_CALL_BACK       0X1237  //录音正常播放结束的消息
#define     MSG_RECORD_PLAY_RING_CALL_BACK  0X1238  //播放来电铃声或者闹钟铃声结束的消息
#define     MSG_RECORD_DELETE_ONE           0X1239
#define     MSG_RECORD_FILE_DELETED         0x123a
#define     MSG_UPDATE_VOL_ICON             0x123b
#define     MSG_RECORD_STOP_PLAY            0x123c
#define     MSG_RECORD_PAUSE_PLAY           0x123d
#define     MSG_PAUSE_RECORD                0x123e
#define     MSG_RESUME_RECORD               0x1240
#define     MSG_RECORD_RESUME_PLAY          0x1241
#define     MSG_OPEN_OPTION_WIN             0x1242


//picture viewer
#define MSG_PICVIEWER_START         0x1300
#define MSG_PICVIEWER_LOAD_FINISH   MSG_PICVIEWER_START     //load pictures finish
#define MSG_PICVIEWER_DELETE_CNF    (MSG_PICVIEWER_START+1) //delete pictures
#define MSG_PICVIEWER_RELOAD_FILE   (MSG_PICVIEWER_START+2) //reload, 会导致重新搜索file array
#define MSG_PICVIEWER_UPDATE_LIST   (MSG_PICVIEWER_START+3) //update list, 不会重新搜索file array
#define MSG_PICVIEWER_MODIFY_VIEW   (MSG_PICVIEWER_START+4) //modify view
#define MSG_PICVIEWER_MIN_ZOOM_OUT  (MSG_PICVIEWER_START+5) //min zoom out
#define MSG_PICVIEWER_PIC_STATE     (MSG_PICVIEWER_START+6) //pic state
#define MSG_PICVIEWER_LOAD_CONTINUE (MSG_PICVIEWER_START+7) //load pictures continue

// watch record使用picview模块的message id
#define MSG_WATCHRECORD_LOAD_START  (MSG_PICVIEWER_START + 8)
#define MSG_WATCHRECORD_LOAD_FINISH (MSG_PICVIEWER_START + 9)

//weather forecast 0X1400~ 0x14FF
#if defined(WEATHER_SUPPORT)
#define     MSG_WEATHER_START                   0x1400
#define     MSG_WEATHER_CONN_FAILED_CNF         (MSG_WEATHER_START + 0)  //socket连接失败
#define     MSG_WEATHER_WGT_UPDATE_IND          (MSG_WEATHER_START + 1)  //更新天气请求
#define     MSG_WEATHER_WGT_UPDATE_CNF          (MSG_WEATHER_START + 2)  //更新天气完成
#define     MSG_WEATHER_WGT_SWITCH_CITY_IND     (MSG_WEATHER_START + 3)  //城市切换请求
#define     MSG_WEATHER_WGT_SWITCH_CITY_CNF     (MSG_WEATHER_START + 4)  //城市切换完成
#define     MSG_WEATHER_PARSE_START_IND         (MSG_WEATHER_START + 5)  //已获取到天气数据，通知app开始解析。
#define     MSG_WEATHER_PARSE_SUCCESS_CNF       (MSG_WEATHER_START + 6)  //数据解析成功，
#define     MSG_WEATHER_PARSE_FAILED_CNF        (MSG_WEATHER_START + 7)  //数据解析失败。
#define     MSG_WEATHER_AUTO_UPDATE_IND         (MSG_WEATHER_START + 8)  //自动更新时间到
#define     MSG_WEATHER_PDP_ACTIVE_CNF          (MSG_WEATHER_START + 9)  //PDP激活成功
#define     MSG_WEATHER_PDP_ACTIVE_FIILED_CNF   (MSG_WEATHER_START + 10) //PDP激活失败
#define     MSG_WEATHER_UPDATE_CANCEL_IND       (MSG_WEATHER_START + 11) //取消更新天气
#define     MSG_WEATHER_UPDATE_CANCEL_CNF       (MSG_WEATHER_START + 12) //取消更新天气
#define     MSG_WEATHER_HTTP_CLOSE_CNF          (MSG_WEATHER_START + 13) //HTTP连接关闭
#define     MSG_WEATHER_NEED_ANIM               (MSG_WEATHER_START + 14) //需要动态特效
#define     MSG_WEATHER_START_ANIM              (MSG_WEATHER_START + 15) //开始显示动态特效
#define     MSG_WEATHER_FORBID_ANIM             (MSG_WEATHER_START + 16) //屏蔽天气特效
#define     MSG_WEATHER_ADD_CITY_UPDATE_IND     (MSG_WEATHER_START + 17)  //添加城市时连接网络
#define     MSG_WEATHER_ERROR_DESCRIPTION_CNF   (MSG_WEATHER_START + 18)  //ERROR  prompt
#define     MSG_WEATHER_NEED_APPEND_COUNTRY_LIST_CNF  (MSG_WEATHER_START + 19) //need append country list
#define     MSG_WEATHER_ONLY_COUNTRY_INFO       (MSG_WEATHER_START + 20)
#define     MSG_WEATHER_TOO_MANY_COUNTRY        (MSG_WEATHER_START + 21)
#define     MSG_WEATHER_RETRY_EN_NAME_CNF       (MSG_WEATHER_START + 22)  //ERROR  prompt
#define     MSG_WEATHER_END                     (MSG_WEATHER_START + 30)
#endif


//1-key backup msg range : 0x1500 -- 0x15FF
#ifdef IKEYBACKUP_SUPPORT
#define MSG_IKB_SIG_BEGIN               0x1500        
#define MSG_IKB_BACKUP_DATA_REQ         0x1501  /*1501 : APP To IKeybackup.  */
#define MSG_IKB_BACKUP_DATA_RES         0x1502  /*1502 : IKeybackup To APP.  */
#define MSG_IKB_BACKUP_ITEM_DATA_REQ    0x1503  /*1503 : IKeybackup To other.*/
#define MSG_IKB_BACKUP_ITEM_DATA_RES    0x1504  /*1504 : other To IKeybackup.*/    
#define MSG_IKB_RESTORE_DATA_REQ        0x1511  /*1511 : APP To IKeybackup.  */
#define MSG_IKB_RESTORE_DATA_RES        0x1512  /*1512 : IKeybackup To APP.  */
#define MSG_IKB_RESTORE_ITEM_DATA_REQ   0x1513  /*1513 : IKeybackup To other.*/
#define MSG_IKB_RESTORE_ITEM_DATA_RES   0x1514  /*1514 : other To IKeybackup.*/    
#define MSG_IKB_LOAD_PACK_DATA_REQ      0x1521  /*1521 : file task to app.   */  
#define MSG_IKB_GET_ALL_PACK_DATA_CNF   0x1522  /*1522 : file task to app.   */
#define MSG_IKB_LOAD_ALL_NODE_REQ       0x1523  /*1523 : app to ikeybackup.  */
#define MSG_IKB_LOAD_ALL_NODE_RES       0x1524  /*1524 : IKeybackup To app.  */
#define MSG_IKB_UPDATE_PACK_LIST        0x1525  /*1525 : update list.        */
#define MSG_IKB_PROGRESS_VALUE_RES      0x1526  /*1526 : show progress value.*/
#define MSG_IKB_CANCEL_ACTION_REQ       0x1527  /*1527 : cancel bk/res req.  */
#define MSG_IKB_CANCEL_ACTION_RES       0x1528  /*1528 : cancel bk/res res.  */

#define MSG_SMS_IKB_BACKUP_ITEM_DATA_REQ    0x1531  /*1531 : IKeybackup To SMS.*/
#define MSG_SMS_IKB_RESTORE_ITEM_DATA_REQ   0x1532  /*1532 : IKeybackup To SMS.*/

#define MSG_IKB_SIG_END                 0x15ff  
#endif


// added by @arthur for java
// java msg range : 0x2001 -- 0x2020  ( total number : 31 )
#define 	MSG_JAVA_LIST_INFO		           0x2001
#define		MSG_JAVA_PROGRESS		           0x2002
#define		MSG_JAVA_DEL_CNF		           0x2003
#define     MSG_JAVA_WEBINSTALL_URL_INFORM     0x2004
#define     MSG_JAVA_CLOSE                      0x2005
#define     MSG_UPDATE_LIST                    0x2006
#define     MSG_OPEN_FOREGROUND                0x2007
#define     MSG_OPEN_TEXTEDIT_WIN              0x2008
#define     MSG_EXIT_TEXTEDIT_WIN              0x2009
#define     MSG_JAVA_LCD_FLUSH				0x200a
#define     MSG_JAVA_TIMER_NOTIFY				0x200b
/*Start of  wangxiaolin 2009.7.2 for pim */
#define     MSG_JAVA_CHECK_OK                  0x200c                   
#define     MSG_JAVA_INSTALL_OK                (MSG_JAVA_CHECK_OK+0x01)
#define   MSG_JAVA_CHECK_FAIL                             (MSG_JAVA_CHECK_OK+0x02)
#define   MSG_JAVA_INSTALL_FAIL                         (MSG_JAVA_CHECK_OK+0x03)
#define   MSG_JAVA_INSTALL_CANCEL                     (MSG_JAVA_CHECK_OK+0x04)
#define   MSG_JAVA_CHECK_AMS                   (MSG_JAVA_CHECK_OK+0x05)
#define   MSG_JAVA_INSTALL_START                         (MSG_JAVA_CHECK_OK+0x06)
#define   MSG_JAVA_OTA_START                   (MSG_JAVA_CHECK_OK+0x07)
#define   MSG_JAVA_OTA_DOWNLOAD_START        (MSG_JAVA_CHECK_OK+0x08)
#define   MSG_CLOSE_WEB_WIN                     (MSG_JAVA_CHECK_OK+0x09)
#define   MSG_JAVA_OTA_INSTALL_START        (MSG_JAVA_CHECK_OK+0x0a)
#define   MSG_JAVA_OTA_STOP                     (MSG_JAVA_CHECK_OK+0x0b)
#define   MSG_JAVA_OUT_OF_MEMORY            (MSG_JAVA_CHECK_OK+0x0c)
#define   MSG_JAVA_INSTALL_WAIT               (MSG_JAVA_CHECK_OK+0x0d)
#define   MSG_JAVA_CLOSE_APP_WIN              (MSG_JAVA_CHECK_OK+0x0e)
#define   MSG_JAVA_AUTORUNAPP                 (MSG_JAVA_CHECK_OK+0x0f)
#define   MSG_JAVA_LINK_TIMER                  (MSG_JAVA_CHECK_OK+0x10)
#define   MSG_JAVA_CONNECT_TIMER             (MSG_JAVA_CHECK_OK+0x11)
#define   MSG_JAVA_RECEIVE_TIMER                (MSG_JAVA_CHECK_OK+0x12)
#define   MSG_JAVA_STOP_SOCKET_TIMER             (MSG_JAVA_CHECK_OK+0x13)
#define   MSG_JAVA_UDP_LINK                      (MSG_JAVA_CHECK_OK+0x14)
#define   MSG_JAVA_PAINT                       (MSG_JAVA_CHECK_OK+0x15)
#define   MSG_JAVA_VM_END_IND             (MSG_JAVA_CHECK_OK+0x16) 
#define   MSG_JAVA_MEDIA_STOPTIME_TIMER             (MSG_JAVA_CHECK_OK+0x17) 
#define   MSG_JAVA_VM_SUSPEND_IND   (MSG_JAVA_CHECK_OK+0x18) 
#define   MSG_JAVA_WAITWIN_END      (MSG_JAVA_CHECK_OK+0x19) 
#define   MSG_JAVA_DEACTIVE_PDP      (MSG_JAVA_CHECK_OK+0x20) 
#define   MSG_JAVA_SEND_TIMER       (MSG_JAVA_CHECK_OK+0x21)
#define   MSG_JAVA_PLATFORM_REQUEST      (MSG_JAVA_CHECK_OK+0x22)
#define   MSG_JAVA_AUDIO_PLAY_AGAIN      (MSG_JAVA_CHECK_OK+0x23) 

#ifdef JAVA_SUPPORT_MYRIAD
//the following definitions are added for JAVA events, from 0x2030 to 0x2038
#define   MSG_JAVA_EVENT_BASE             0x2030
#define   MSG_JAVA_TRIGGER_JBED_REFRESH      (MSG_JAVA_EVENT_BASE+0x01)
#define   MSG_JAVA_NOTIFY_TASKLIST_UPDATE    (MSG_JAVA_EVENT_BASE+0x02)
#define   MSG_JAVA_NOTIFY_DELETED            (MSG_JAVA_EVENT_BASE+0x03)
#define   MSG_JAVA_NOTIFY_NAMS_EXIT          (MSG_JAVA_EVENT_BASE+0x04)
#define   MSG_JAVA_MMI_EVENT                 (MSG_JAVA_EVENT_BASE+0x05)
#define   MSG_JAVA_LOCAL_INSTALL_UPDATE_LIST (MSG_JAVA_EVENT_BASE+0x06)
#define   MSG_LOCAL_INSTALL_SEARCH_END       (MSG_JAVA_EVENT_BASE+0x07)
#define   MSG_LOCAL_INSTALL_DELETE_CNF       (MSG_JAVA_EVENT_BASE+0x08)
#define   MSG_JAVA_OTA_INSTALL_START               (MSG_JAVA_EVENT_BASE+0x09)
#endif

//pim msg range : 0x2041 -- 0x2060  ( total number : 31 )
#ifdef PIM_SUPPORT  //add by wangxiaolin 2009.07.01
#define PIM_MSG_CONN_EST  0x2041
#define PIM_MSG_DATA_SENT (PIM_MSG_CONN_EST+1)
#define PIM_MSG_RCV_DONE (PIM_MSG_CONN_EST+2)
#define PIM_MSG_NEXT_STEP (PIM_MSG_CONN_EST+3)//
#define PIM_MSG_SELECT_TIMER (PIM_MSG_CONN_EST+4)//
#define PIM_MSG_COMM_ERROR (PIM_MSG_CONN_EST+5)//
#define PIM_MSG_SYNC_FAIL (PIM_MSG_CONN_EST+6)//
#define PIM_MSG_DIS_SEND (PIM_MSG_CONN_EST+7)//
#define PIM_MSG_DIS_RECE (PIM_MSG_CONN_EST+8)//0x0e2e
#define MSG_PDP_ACTIVE_FAIL (PIM_MSG_CONN_EST+9)
#define MSG_PDP_ACTIVE_SUCCESS (PIM_MSG_CONN_EST+10)
#define PIM_MSG_STOP (PIM_MSG_CONN_EST+11)
#define PIM_MSG_START (PIM_MSG_CONN_EST+12)
#endif
/*End   of  wangxiaolin 2009.7.2 for pim */

//WWW的范围为：  0x2100~~0x21ff
enum {
    MSG_WWW_LIST_MATCHKEY = 0x2100,
    MSG_WWW_LIST_MATCHURL,
    MSG_WWW_UPDATE_LOAD_PERCENT,
    MSG_WWW_PAGE_LOAD_START,
    MSG_WWW_PAGE_LOAD_FINISH,
    MSG_WWW_RESET_LIST,
    MSG_WWW_PAGE_SECURE_INFO,
    MSG_WWW_PAGE_CERTIFICATE,
    MSG_WWW_NOTIFY_BG_SOUND,
    MSG_WWW_NOTIFY_CHANGE_WIN,
    MSG_WWW_NOTIFY_UPLOAD,
    MSG_WWW_NOTIFY_UPLOAD_END,
    MSG_DL_DOWNLOAD_START,
    MSG_DL_EXCEED_TASK_NUM,
    MSG_DL_PDP_CONNECT_IND,
    MSG_DL_PDP_DEACTIVE_IND,
    MSG_WWW_REQUEST_JSCONF,
    MSG_WWW_REQUEST_JSPROMPT,
    MSG_WWW_DELETE_SYNC_MSG,
    MSG_WWW_REQUEST_WMLJSCONF,
    MSG_WWW_REQUEST_WMLJSPROMPT,
    MSG_WWW_STOP_APPLET,
    MSG_WWW_NO_MEMORY,
    MSG_WWW_AUTH_MSG,
    MSG_WWW_QUERY_NETWORK,
    MSG_WWW_ASYN_LOSE_FOCUS
};


//dm  module begin
#define MSG_DM_DWNLD_PROGRESS                  0X2200
#define MSG_DM_SOFTWARE_UPDATE                 0X2201
#define MSG_DM_LOCK_DEVICE                 	   0X2202 
#define MSG_DM_UNLOCK_DEVICE                   0X2203
#define MSG_DM_SESSION_CHANGE                  0x2204 
#define MSG_DM_NOTIFY_ALERT                    0x2205 
#define MSG_DM_SESSION_EXIT                    0x2106

#ifdef ASP_SUPPORT
#define     MSG_ASP_START_RECORD               0x2009
#define     MSG_ASP_PLAY_CALL_BACK            0x2010  //录音正常播放结束的消息
#define     MSG_ASP_CLOSE_RECORD_WIN        0x2011
#define     MSG_ASP_CLOSE_RELEASING_WIN        0x2012
#endif
//dm module end
//pub win message id: 0x3000- 0x3100
#define     MSG_PUB_PROGRESS_SET_TOTAL_SIZE 	0x3000
#define     MSG_PUB_PROGRESS_UPDATE_SIZE 		0x3001
//Sam@Dec20 SelectSIMFunc
#define 	  MSG_PUB_SELECT_SIM_OK				0x3002
#define 	  MSG_PUB_SELECT_SIM_NONE			0x3003


//mbbms
#ifdef MBBMS_SUPPORT
#define MSG_APP_MBBMS_REFRESH_LIST            0x3100
#define MSG_APP_MBBMS_CLOSE_NOTIFY_OK         0x3101
#define MSG_APP_MBBMS_CLOSE_NOTIFY_CANCEL     0x3102
#define MSG_APP_MBBMS_INIT_CMMB               0x3103
#define MSG_APP_MBBMS_PLAY_PROCESS_ERROR      0x3104
#define MSG_APP_MBBMS_WAIT_GET_FOCUS          0x3105
#define MSG_APP_MBBMS_SWITCH_TO_MBBMS         0x3106
#define MSG_APP_MBBMS_PLAY_CONTENT            0X3107
#endif

//atv
#ifdef ATV_SUPPORT
#define MSG_APP_ATV_REFRESH_LIST              0x3110
#define MSG_APP_ATV_FATAL_ERROR               0x3111
#define MSG_APP_ATV_START                     0x3112
#define MSG_APP_ATV_BRIGHTNESS                0x3113
#define MSG_APP_ATV_RECORD_END                0x3114
#define MSG_ATV_GET_FOCUS                     0x3115
#define MSG_APP_ATV_PLAY_SERVICE              0x3116
#define MSG_ATV_SENSOR                        0x3117

#define MSG_APP_SCAN_PROCESS_IND              0x3119
#define MSG_APP_ATV_WEAK_SINGAL               0x311a
#define MSG_APP_ATV_NORMAL_SINGAL             0x311b
#define MSG_APP_ATV_SEARCH_CNF                0x311c
#define MSG_ATV_NO_SPACE                      0x311d
//ref callback msg define  0x3120 - 0x312f
#define MSG_APP_ATV_CALLBACK_MSG_START        0x3120
#endif

//DRM message id:0x3150-3160
#ifdef DRM_SUPPORT
#define     MSG_DRM_GET_PUSH_INFO               0x3150
#define     MSG_DRM_RENAME_END_CNF              0x3151
#define     MSG_DRM_RENAME_CANCEL_CNF           0x3152
#define     MSG_DRM_NOT_CLEAN_SPACE             0x3153
#define     MSG_DRM_CLEAN_SPACE_SUC             0x3154
#endif

#ifdef SEARCH_SUPPORT
#define MSG_SEARCH_FILE_LOAD_ALL_LIST_FINISH      0x3201
#define MSG_SEARCH_LIST_APPEND_MATCH_ITEM 0x3202
#define MSG_SEARCH_LIST_REAPPEND_MATCH_ITEM 0x3203
#define MSG_SEARCH_START_SEARCH 0x3204
#define MSG_SEARCH_FILE_START_UPDATE_TIMER           0x3205
#define MSG_SEARCH_UPDATE_BUTTON_VIEW 0x3206
#define MSG_SEARCH_OPEN_TYPE_BOX    0x3207
#define MSG_SEARCH_SET_PROMPT_ITEM_INVALID    0x3208
#define MSG_SEARCH_PB_START_UPDATE_TIMER           0x3209
#endif

#ifdef PUSH_EMAIL_SUPPORT

#define MSG_MAIL_START  0x1600
#define MSG_MAIL_UPDATE_CHARGE_INFO MSG_MAIL_START
#define MSG_MAIL_CLOSE_WAITING_WIN  (MSG_MAIL_START + 1)
#define MSG_MAIL_READ               (MSG_MAIL_START + 2) //读取邮件
#define MSG_MAIL_FINISH_READ        (MSG_MAIL_START + 3) //读取邮件完毕
#define MSG_MAIL_UPDATE_LIST        (MSG_MAIL_START + 4) //更新邮件列表
#define MSG_MAIL_NEEDUPDATELIST     (MSG_MAIL_START + 5) //设置需要更新列表
#define MSG_MAIL_DELETE_NEXT        (MSG_MAIL_START + 6) //删除邮件
#define MSG_MAIL_DELETE_FINISH      (MSG_MAIL_START + 7) //删除邮件完毕
#define MSG_MAIL_CANCEL_DELETE      (MSG_MAIL_START + 8) //取消删除邮件
#define MSG_MAIL_UPDATE_MENU        (MSG_MAIL_START + 9) //更新主菜单
#define MSG_MAIL_NEEDUPDATEMENU     (MSG_MAIL_START + 10) //设置需要更新主菜单
#define MSG_MAIL_LIST_CHANGE        (MSG_MAIL_START + 11) //邮件列表变化
#define MSG_MAIL_RETRATTACH_RESULT  (MSG_MAIL_START + 12) //接收附件结果
#define MSG_MAIL_RETRMAIL_RESULT    (MSG_MAIL_START + 13) //接收邮件结果
#define MSG_MAIL_SAVE_FINISH        (MSG_MAIL_START + 14) //保存邮件完毕通知
#define MSG_MAIL_SEND_FINISH        (MSG_MAIL_START + 15) //发送邮件完毕通知
#define MSG_MAIL_UPDATE_ACC_LIST    (MSG_MAIL_START + 16) //更新附件列表
#define MSG_MAIL_RESET_FACTORY_OVER (MSG_MAIL_START + 17) //恢复出厂设置结束通知
#define MSG_MAIL_DEL_ACCOUNT_FINISH (MSG_MAIL_START + 18) //删除帐号所有邮件结束通知
#define MSG_MAIL_CHECK_PASSWD_RESULT  (MSG_MAIL_START + 19) //验证密码结果
#define MSG_MAIL_RECV_MAIL_PROGRESS (MSG_MAIL_START + 20) //接收邮件进度通知
#define MSG_MAIL_COPY_ACC_CNF       (MSG_MAIL_START + 21)

#define MSG_MAIL_END    (MSG_MAIL_START + 0xFF)
#endif /* PUSH_EMAIL_SUPPORT  */

#ifdef VIDEO_PLAYER_SUPPORT
#define MSG_VP_LOADING_BEGIN     0x4100
#define MSG_VP_LOADING_END       0x4101
#define MSG_VP_BEGIN_PLAY        0x4102
#define MSG_VP_AUTH_IND          0x4103
#define MSG_VP_PLAY_END          0x4104
#define MSG_VP_PREPARE_IND       0x4105
#define MSG_VP_STOP_IND          0x4106
#define MSG_VP_FATAL_ERROR       0x4107
#define MSG_VP_SEEK_CNF          0x4108
#define MSG_VP_PAUSE_CNF         0x4109
#define MSG_VP_PLAY_CNF          0x410a
#define MSG_VP_BUFFER_END_IND    0x410b
#define MSG_VP_BUFFERING_IND     0x410c
#define MSG_VP_PROCESS_IND       0x410d
#define MSG_VP_UPDATE_BEGIN      0x410e
#define MSG_VP_UPDATE_END        0x410f
#define MSG_VP_DELETE_BEGIN      0x4110
#define MSG_VP_DELETE_NEXT        0x4111
#define MSG_VP_FILE_OPTION       0x4112
#define MSG_VP_REPEAT_SET       0x4113
#define MSG_VP_RANDOM_SET       0x4114
#define MSG_VP_OPEN_VOL_CONTROL_PANEL 0x4115
#define MSG_VP_SEEK              0x4116
#define MSG_VP_OPEN_BRIGHT_CONTROL_PANE 0x4117
#define MSG_VP_INTERNAL_MSG_BEGIN 0x4120
#define MSG_VP_INTERNAL_MSG_END    0x4200
#define  MSG_PLAY_MORE_ANIMATION   0x4300
#define  MSG_VP_DEL_MARKITEM_INFO  0x4400
#define MSG_VP_CHECK_NEED_RESEARCH     0x4500
#endif
#ifdef SXH_APPLE_SUPPORT
#define    MSG_APPLE_INIT               0x3101
#endif

#ifdef ASP_SUPPORT
#define MMIASP_MSG_UPDATELIST       0x5000
#endif

#define MMI_MSG_RECORDTORING        0x5001

#ifdef APP_STORE_SUPPORT
#define MSG_APPSTORE_INSTALL_BY_DEBUG_MODE  0x5100
#define MSG_APPSTORE_UNINSTALL_BY_DEBUG_MODE  0x5101
#endif

#ifdef DYNAMIC_WATCHFACE_SUPPORT
#define MSG_WATCHFACE_INSTALL_BY_DEBUG    0x5200
#define MSG_WATCHFACE_UNINSTALL_BY_DEBUG  0x5201
#define MSG_WATCHFACE_FACTORY_RESET       0x5202
#endif

#ifdef QQ_SUPPORT
#define MSG_QQ_ACTIVE_GPRS 0x6000
#endif

#ifdef QBTHEME_SUPPORT
#define MSG_UPDATE_QBTHEME_LIST 0x6100
#define MSG_QBTHEME_UNZIP 0x6101
#endif

#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
#define MSG_PHONE_SIM_LOGIC_PLUG_OUT_IND  0x6200
#define MSG_PHONE_SIM_LOGIC_PLUG_IN_IND   0x6201
#endif

#ifdef DCD_SUPPORT
//added by wz for dcd
// dcd msg range : 0xff01 -- 0xff20  ( total number : 10 )
#define MSG_DCD_LIST_UPDATE                  0xff01
#define MSG_DCD_TAB_UPDATE                   0xff02
#define MSG_MULTIM_COPY_RESULT_CNF    0xff03
#define MSG_DCD_UPDATE_FINISH              0xff04
#define MSG_DCD_MAN_UPDATE_ERROR      0xff05
#define MSG_DCD_SYNCHRONIZE_DISPLAY  0xff06
#define MSG_DCD_RESET_START                  0xff07
#define MSG_DCD_RESET_FINISH                 0xff08
#endif //
#if defined(MMI_UPDATE_TIME_SUPPORT)
#define     MSG_SOCKET_PDP_ACTIVE_SUCCSEE 	    0x7000
#define     MSG_SOCKET_CONNECT_SUCCSEE           0x7001
#define     MSG_SOCKET_WRITE_SUCCSEE 		    0x7002
#define     MSG_SOCKET_READ_SUCCSEE 		    0x7003
#endif

#ifdef FLASH_SUPPORT
#define     MSG_FLASH_UPDATE_WINDOW 	    0x8001
#define     MSG_FLASH_END                   0x8009
#endif
#ifdef MENU_CRYSTALCUBE_SUPPORT
#define  MSG_MENU_CRYSTALCUBE_BEGIN    0x800a    //for crystal_cube
#define MSG_MENU_CRYSTALCUBE_END       0x8031
#endif

#ifdef MENU_CYCLONE_SUPPORT
#define  MSG_MENU_CYCLONE_BEGIN    0x8032        //for cyclone
#define MSG_MENU_CYCLONE_END       0x8059

#endif

#ifdef MMI_KING_MOVIE_SUPPORT
#define MSG_KM_LOADING_BEGIN          0x9100
#define MSG_KM_LOADING_END            0x9101  
#define MSG_KM_UPDATE_LIST            0x9102
#define MSG_KM_UPDATE_MAIN_WIN        0x9103
#define MSG_KM_LOADING_SAVE           0x9104
#endif

#ifdef MMI_MEMO_SUPPORT
#define     MSG_MEMO_OPEN_MAIN 	    0x9001
#define     MSG_MEMO_OPEN_MAIN_FINISH 	    0x9002
#define     MSG_MEMO_SAVE_FILE 	    0x9003
#define     MSG_MEMO_SAVE_FILE_FINISH 	    0x9004
#define     MSG_MEMO_DELETE_FILE	    0x9005
#endif

#if defined(IM_ENGINE_SOGOU)
#define MSG_SG_UPD_WAIT_BASE			0xa000
#endif

#if defined(WRE_SUPPORT)
#define MSG_WRE_ENTRY_INIT              0xa0F0
#define MSG_WRE_STARTAPP_INIT           0xa0F1
#define MSG_WRE_SYNSTARTUP_INIT         0xa0F2
#endif

#ifdef MMISRV_SUPPORT
#ifdef MMISRV_AUDIO_SUPPORT
#define MSG_SRVAUD_BT_EVENT         0xA100
#endif
#endif

#if defined(MENU_SWEETS_SUPPORT)
#define MSG_MENU_FULL_PAINT_END         0Xa200
#endif

#if defined(OPERA_MOBILE_SUPPORT_V11) || defined(OPERA_MINI_SUPPORT_VER42) || defined(OPERA_MINI_SUPPORT_VER6)
#define     MSG_MMI_OPERA_START                 0xb000
#define     MSG_MMI_OPERA_END                   (MSG_MMI_OPERA_START + 100)
#define     MSG_MMI_OPERA_ASYNC_EXIT      (MSG_MMI_OPERA_END +1)
#endif

#ifdef SNS_SUPPORT
#define MSG_SNS_OPERA_RESULT  (0xc001)
#define MSG_SNS_SHARE_TO_BROWSER  (0xc002)
#endif
#define MSG_VR_LOADING_END 0xd001
#define MSG_VR_DELETE_BEGIN 0xd002
#ifdef AUDIOPLAYER_IT_SUPPORT
#define MSG_CASE_BEGIN 0xd003
#endif

#ifdef BAIDU_DRIVE_SUPPORT
#define MSG_BAIDU_DRV_START (0xd030)
#define MSG_BAIDU_DRV_UPLOAD_PHOTO_NOTIFY  (0xd031)
#define MSG_BAIDU_DRV_UPLOAD_PHOTO_CNF  (0xd032)
#define MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_NOTIFY  (0xd033)
#define MSG_BAIDU_DRV_DOWNLOAD_THUMBNAIL_CNF  (0xd034)
#define MSG_BAIDU_DRV_DOWNLOAD_PHOTO_NOTIFY (0xd035)
#define MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF (0xd036)
#define MSG_BAIDU_DRV_INIT_CNF (0xd037)
#define MSG_BAIDU_DRV_ERROR_NOTIFY (0xd038)
#define MSG_BAIDU_DRV_CANCEL_CNF (0xd039)
#define MSG_BAIDU_DRV_DELETE_PHOTO_NOTIFY (0xd03a)
#define MSG_BAIDU_DRV_SIGNIN_DONE (0xd03b)
#define MSG_BAIDU_DRV_SIGNOFF_CNF (0xd03c)
#define MSG_BAIDU_DRV_SHOW_QR (0xd03d)
#define MSG_BAIDU_DRV_GET_TOKEN_CNF (0xd03e)
#define MSG_BAIDU_DRV_CAPTURED_PIC_LOAD (0xd03f)
#define MSG_BAIDU_DRV_END (0xd040)
#endif

/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* END _MMI_MSG_H_  */
