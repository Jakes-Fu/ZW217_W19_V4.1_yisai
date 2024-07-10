    ######################################################################################
    #                                  Notice!!!                                         #
    #                          type of Low memory Support                                #                                                                       
    ######################################################################################

#-------------------------LOW_MEMORY_SUPPORT = 16X16----------------------------------------#  
ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 16X16)
    ifeq ($(strip $(MULTI_THEME_SUPPORT)), TRUE)
#    MCFLAG_OPT		+=  -DMMI_SAM_THEME
    endif 

    #ͨ����¼����֧���������
    #MCFLAG_OPT		+=  -DMMI_CL_NUM_MAX_SUPPORT
    #MCFLAG_OPT		+=  -DENG_SUPPORT
    #MCFLAG_OPT		+=  -DENG_MINI_SUPPORT
    #dorado������Ƿ���ʾtoolbar(��ʾtoolbarʱ,����ʾsoftkey);���������,�ÿ���Ĭ�ϴ�
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
    #MCFLAG_OPT		+=  -DBROWSER_TOOLBAR
    endif

    ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
      ifneq ($(strip $(MMI_MULTI_SIM_SYS)), NONE)
	    MCFLAG_OPT		+=  -DMMI_GPRS_SUPPORT
	    MCFLAG_OPT		+=  -DSTK_SUPPORT
      endif
    endif
    
    ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
        MCFLAG_OPT		+=  -DMMI_GPRS_SWITCH_SUPPORT
    endif

    #״̬��������ʾ��Ϣ
    #MCFLAG_OPT		+=  -DMMI_STATUSBAR_SCROLLMSG_SUPPORT

    ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
#            MCFLAG_OPT += -DPDA_TIME_ANIM_SUPPORT
    endif

    #����Ч��֧�֣���MP3ר��ͼƬ��
    #MCFLAG_OPT		+=  -DBRIGHT_EFFECT_SUPPORT

    #u200ͼƬ�༭��
    ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
    	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    		MCFLAG_OPT	+= -DPIC_EDIT_U200_SUPPORT
        endif
    endif
    
    #u200ͼƬ�����
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)    
    ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    MCFLAG_OPT  += -DPIC_PREVIEW_U200_SUPPORT
    #    PIC_QUICKVIEW_SUPPORT = TRUE
    endif
endif
    
    #MCFLAG_OPT	+= -DMMI_TURNON_BACKLIGHT_EFFECT_SUPPORT
    ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
        ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
    #����Ԥ����ʱ�䷭ҳ��Ч        
    #        MCFLAG_OPT += -DPDA_WEATHER_TIME_SE
        endif
    #    MCFLAG_OPT += -DPDA_WEATHER_SUPPORT_LOW_MEMORY    
    endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
    #    MCFLAG_OPT += -DPDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)), TRUE)
#	MCFLAG_OPT		+= -DFEATURE_BIG_ENDIAN

#	MCFLAG_OPT		+= -DSXH_APPLE_APP
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT
#	MCFLAG_OPT		+= -DSXH_APPLE_VP
#	MCFLAG_OPT		+= -DSXH_APPLE_RP
#	MCFLAG_OPT		+= -DSXH_APPLE_3GMUSIC
#	MCFLAG_OPT		+= -DSXH_APPLE_ONLINEGAME
#	MCFLAG_OPT		+= -DSXH_APPLE_VIDEODL
#	MCFLAG_OPT		+= -DSXH_APPLE_UMPLAY
#	MCFLAG_OPT		+= -DSXH_APPLE_BOOK
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT_DEMO
	

#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_APPMAN
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_VP
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_UMVCHAT
endif

#keylock effect start

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT    #�ᡢ�����Ǵ�����֧��

ifeq ($(strip $(LOW_MEMORY_KEYLOCK)), TRUE)
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_QQVGA
endif

ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
ifneq ($(strip $(MAINLCD_SIZE)),176X220)
#michael wang
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DC_SUPPORT_ROTATE
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
#michael wang
#����֧��
ifneq ($(strip $(MMI_RES_ORIENT)), LANDSCAPE)

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
#MCFLAG_OPT		+=  -DGUI_CTRL_SCROLL_KEY              #��Ҫ���������2ͬʱ��

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT

ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
endif

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE8_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_PSW_SUPPORT

ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE) 
ifeq ($(strip $(CAMERA_SUPPORT)), TRUE) 
#MCFLAG_OPT		+=  -DKEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
endif
endif
endif	# end of LANDSCAPE
endif
endif # end of TOUCHPANEL_TYPE

#keylock effect end

    ##ǽֽ��̬�����ڴ�,��ʹ�þ�̬�ڴ�
    ifeq ($(strip $(SRAM_SIZE)), 16MBIT)
    MCFLAG_OPT	+=  -DMMI_DYNA_ALLOC_WALLPAPER_BUF
    endif

    #GBK support
    ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)), TRUE)
    ifneq ($(strip $(FONT_HAN_SIMP_ONLY)), TRUE)
    ifneq ($(strip $(BITMAP_FONT_SLIM_SUPPORT)), TRUE)
    MCFLAG_OPT		+=  -DGBK_SUPPORT
    endif
    endif
    endif

    MCFLAG_OPT		+=  -DBLUETOOTH_HFG_PLAY_MUSIC_SUPPORT        
	ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			MCFLAG_OPT		+=  -DMMI_IM_PDA_SUPPORT        ###use pda im, when exist touch panel
	endif

	ifeq ($(strip $(EBOOK_SUPPORT)), TRUE)
		#MCFLAG_OPT += -DMMIEBOOK_OSDMENU_SUPPORT		#OSD�����˵�
		ifeq ($(strip $(SE_PAGE_TURN_SUPPORT)), TRUE)
			#MCFLAG_OPT += -DMMIEBOOK_PAGESLIDE_SUPPORT	#��ҳ��Ч
		endif
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			#MCFLAG_OPT += -DMMIEBOOK_BOOKSHELF_SUPPORT	#�������
		endif
	endif
	
	
	ifeq ($(strip $(GAME_SUPPORT)), TRUE)
		#��Ϸ: ������ 
		#MCFLAG_OPT		+=  -DGAME_BOXMAN_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_LINK_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_QUINTET_SUPPORT	
	endif
	
	MMIAP_SUPPORT = TRUE
	MCFLAG_OPT		+=  -DMMI_AUDIO_PLAYER_SUPPORT

  ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
          MCFLAG_OPT	+=	-DMMI_RECORD_SUPPORT
#	  MCFLAG_OPT	+=	-DMMI_AUTO_RECORD_SUPPORT
	endif
	
	ifeq ($(strip $(SCHEDULE_SUPPORT)), TRUE)
	    MCFLAG_OPT	+=	-DMMI_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_DAILY_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_WEEK_SCHEDULE_SUPPORT
	endif
	
  MCFLAG_OPT		+=  -DMMI_AUDIOPLAYER_ALBUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_SPECTRUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_MUSICMARK_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_SORT_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_LYRIC_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_MUSIC_LIST_SUPPORT
  #		#MP3
  
  #MCFLAG_OPT += -DMMI_DIAL_SEARCHING_SUPPORT #�����̶�̬����ƥ�书��
  #MCFLAG_OPT		+=  -DACCOUNT_AUTO_ADAPT_SUPPORT
		#�������ʻ��Զ�ƥ��
#	MCFLAG_OPT		+=  -DDATA_ACCOUNT_IN_TCARD_SUPPORT
    # define DATA_ACCOUNT_IN_TCARD_SUPPORT #�������˻����ݷ�T��
	MCFLAG_OPT		+=  -DCALL_TIMES_SUPPORT
		#ͨ����¼����
#	MCFLAG_OPT		+=  -DMMI_CALL_MINIMIZE_SUPPORT
		#ͨ����С��
#	MCFLAG_OPT += -DMMI_SPEEDDIAL_SUPPORT
			#��ݲ���
	MCFLAG_OPT += -DMMI_IP_CALL_SUPPORT	
	    #IP����
#	MCFLAG_OPT += -DMMI_MSD_SUPPORT
			#��������
	MCFLAG_OPT += -DMMI_BLACKLIST_SUPPORT
			#������
  #MCFLAG_OPT		+=  -DCL_SAVE_NAME_SUPPORT
		#ͨ����¼�д洢������		
	MCFLAG_OPT += -DMMI_DIALPANEL_DTMF_SUPPORT
			#������֧��DTMF
	#MCFLAG_OPT += -DMMI_POWER_ON_PWD_SOS_SUPPORT
			#�����������֧��SOS
  #MCFLAG_OPT  += -DMMI_VCALENDAR_SUPPORT
      #֧���ճ̵��롢���������ͺͽ���	
      
## CPHS CSP(ALS) SUPPORT
ifeq ($(strip $(CPHS_SUPPORT)), TRUE)
#	MCFLAG_OPT += -DMMI_CPHS_SUPPORT
	ifeq ($(strip $(CSP_SUPPORT)), TRUE)
#		MCFLAG_OPT += -DMMI_CSP_SUPPORT
	endif
endif

    MCFLAG_OPT += -DMMI_LDN_SUPPORT
		#LDN Support
ifneq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
#    MCFLAG_OPT +=  -DMMI_REPLACE_SMS_SUPPORT
    	#Replace SMS֧��
#    MCFLAG_OPT += -DMMI_STK_ICON_DISPLAY_SUPPORT
		  #STK ICON Display֧��
#    MCFLAG_OPT += -DMMI_SIM_LANGUAGE_SUPPORT
		  #SIM������֧��
		MCFLAG_OPT += -DMMI_SMSCB_SUPPORT
		  #С���㲥
endif
	
	ifneq ($(strip $(ORANGE_SUPPORT)), TRUE)
		#R8
		#MCFLAG_OPT += -DMMI_R8_SUPPORT
	endif
	
	#��������ģʽ
	ifeq ($(strip $(SMS_CHAT_SUPPORT)), TRUE)
   ifeq ($(strip $(MMS_SMS_IN_1_SUPPORT)), TRUE)
   	ifeq ($(strip $(CHATMODE_ONLY_SUPPORT)), TRUE)
			MCFLAG_OPT += -DMMI_SMS_CHAT_SUPPORT
		endif
	 endif
  endif
	
	#���ŵ�������
#	MCFLAG_OPT += -DMMI_SMS_EXPORT_SUPPORT
	
	#��ʱ����
#	MCFLAG_OPT += -DMMI_TIMERMSG_SUPPORT
	
	#�ܽӵ绰���Żظ�
#	MCFLAG_OPT += -DMMI_REJECT_CALL_BY_SMS_SUPPORT
	
	#������������
#	MCFLAG_OPT += -DMMI_SMS_FONT_SETTING_SUPPORT
	
	#��ȫ����
#	MCFLAG_OPT += -DMMI_SMS_SECURITYBOX_SUPPORT

    #ɾ��ȫ��
#   MCFLAG_OPT += -DMMI_SMS_DELALL_SUPPORT	
 		 
    #��������                             
#   MCFLAG_OPT += -DMMI_SMS_VOICE_MAIL_SUPPORT	

    #���Ÿ��ƹ���                  
#   MCFLAG_OPT += -DMMI_SMS_COPY_SUPPORT	
        
    #�����ƶ�����                  
#   MCFLAG_OPT += -DMMI_SMS_MOVE_SUPPORT

#	MCFLAG_OPT    +=    -DMMI_VCARD_SUPPORT   
	
# idle & widget start
ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)	
ifneq ($(strip $(MAINLCD_SIZE)), 176X220)					
	#MCFLAG_OPT		+= -DGRID_THUMBVIEW_SUPPORT			 #grid idle ����ͼ����
	#MCFLAG_OPT		+= -DGRID_SLIDE_WALLPAPER				 #idle ����ǽֽ������low memoryȫ��
	
	ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
	     #MCFLAG_OPT += -DMMI_WIDGET_WEATHER2			# 2 for PDA sytle; weather 1 for bar phone
       #MCFLAG_OPT += -DMMIWIDGET_WEATHER_ANIM   # ����Ԥ����ҳ����
	  endif
	endif
	
	#MCFLAG_OPT		+=  -DSCH_ASSISTANT_WIDGET    	 # �ճ�����		
	#MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifneq ($(strip $(FM_SUPPORT)), NONE)	
  #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif    
  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Img Viewer
ifeq ($(strip $(COUNTEDTIME_SUPPORT)), TRUE)
  #MCFLAG_OPT		+= -DMMI_GRID_COUNTEDTIME			   #Grid Countedtime
endif
  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
  #MCFLAG_OPT		+= -DMMI_GRID_CONTACT			       #Grid Contact  ��ϵ��
  #MCFLAG_OPT    +=  -DMMI_WIDGET_TIME						 #����ҳЧ����ʱ��  

	
ifeq ($(strip $(WORLD_CLOCK_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_CLOCK   			#����˫ʱ��
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_ACLOCK    #ģ��˫ʱ��
endif
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_SET    	#���ٿ��� wifi bt ������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_MENU    #��ݷ�ʽ ����ʹ��
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ALARM
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ACLOCK   			 #ģ��ʱ��
endif
ifeq ($(strip $(ASP_SUPPORT)), TRUE)  
    #MCFLAG_OPT       +=    -DMMI_WIDGET_AUTOANSWER    #��¼�� ����ʹ��		#�Ѳ���ʹ��   
endif
ifeq ($(strip $(SEARCH_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SEARCH   			 #��������
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETSEARCH  		 #��������
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETWORK   		 #��������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_MEMO   				 #��ǩ 
endif     ### 176X220 
ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
	  #MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
	  #MCFLAG_OPT		+= -DBAR_PHONE_TOUCH_WIDGET			 #bar phone touch idle sytle	   
ifneq ($(strip $(FM_SUPPORT)), NONE)
    #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  #MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
	  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar	
endif     ### 176X220     
endif			### touch panel
  
  ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE) 				#bar phone Ӧ���Զ������漸��widget
	  #MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
	  #MCFLAG_OPT		+= -DMMI_MINI_BAR_PHONE_WIDGET_SUPPORT #֧��Grid FM��Grid Mp3��Grid Calendar
ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
	  #MCFLAG_OPT 		+= -DMMI_WIDGET_WEATHER1				 # weather 	
endif	  
ifneq ($(strip $(FM_SUPPORT)), NONE)
    #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  #MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)	  
	  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Image Viewer
endif	  
	  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
	  #MCFLAG_OPT		+= -DMMI_WIDGET_SPEED_DIAL			 #Grid ���ٲ���	  
  endif
endif		# end of ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
# idle & widget end
	
	#3d rotate effect support.
	ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
	    #MCFLAG_OPT		+= -DSET_3D_STYLE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_ROTATE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_LIST_ROLL_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_MAIN_MENU_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_WIN_SLIDE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_WIN_FADE_SUPPORT
	    #MCFLAG_OPT		+= -DDYNAMIC_WALLPAPER_SUPPORT    
	    #MCFLAG_OPT		+= -DEFFECT_FIRE_WALLPAPER_SUPPORT
	    ifneq ($(strip $(MOTION_SENSOR_TYPE)), NONE) 
	        #MCFLAG_OPT		+= -DEFFECT_LCD_ROTATE_SUPPORT  #lcd rotate effect.
	    endif
	endif
	
	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
		ifeq ($(strip $(SE_ROLL_SUPPORT)), TRUE)
			#    MCFLAG_OPT		+= -DROLL_MAINMENU_SUPPORT
		endif
	endif
	
	ifeq ($(strip $(AMR_A2DP_SUPPORT)), TRUE)
    MCFLAG_OPT      += -DMMI_AMRBT_VPSUPPORT  			#VP��AMR��ʽ��BT֧��
	endif
  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
   	#MCFLAG_OPT		+= -DVIDEOTHUMBNAIL_SUPPORT      	#��Ƶ�ļ�����ͼ��ȡ
  endif

    #MCFLAG_OPT		+= -DTAKESCREENPHOTO_SUPPORT               #��Ļ����

		#MCFLAG_OPT		+= -DMENU_CRYSTALCUBE_SUPPORT
    #MCFLAG_OPT		+= -DMENU_CYCLONE_SUPPORT
    MCFLAG_OPT		+= -DRES_STORE_CACHE_SUPPORT           #resource store cache support.
    MCFLAG_OPT		+= -DRES_VECTOR_CACHE_SUPPORT          #resource vector cache support.
    #MCFLAG_OPT		+= -DPRINTSCREEN_SUPPORT               #��Ļץͼ 
    MCFLAG_OPT		+= -DMMI_AUTOTEST_SUPPORT              #auto test support.
    MCFLAG_OPT		+= -DMMI_AUTOTEST_WINIDNAME_SUPPORT              #auto test winidname support.
    
	  ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
        #MCFLAG_OPT += -DPIC_EDIT_FRAME_SUPPORT        	 #picture editor frame support.
    endif

    #MCFLAG_OPT      += -DCAMERA_FRAME_SUPPORT           #camera photo frame support.

    ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        #MCFLAG_OPT		+= -DEFFECT_ZOOM_SUPPORT           #zoom effect support.
    endif


    #320x480�ڴ����ޣ����¹��ܲ�����		
    ifneq ($(strip $(MAINLCD_SIZE)), 320X480)
      #ʹ��ˢ�����ٹ���
      MCFLAG_OPT		+= -DMMI_FPS_ACCELERATE_SUPPORT
      #ʹ��pop������������
      ifeq ($(strip $(MMI_PUBWIN_ANIM_SUPPORT)), TRUE)     
        #MCFLAG_OPT		+= -DMMI_MENUMASKLAYER_SUPPORT     #Menu Mask Layer
      endif
    endif
        
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
      ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        ifeq ($(strip $(SE_RIPPLE_SUPPORT)), TRUE)    
            #MCFLAG_OPT		+= -DEFFECT_RIPPLE_SUPPORT     #ripple effect support.  
        endif
      endif
    endif
     
    MCFLAG_OPT       +=    -DMMI_RES_LOW_COST
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RESOURCE
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_EBOOK    #Ebook�ü�
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_PIC_EDIT #PIC edit�ü�MMIPIC_PASTE_IMAGE_NUM����
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RING     #�����ü�
        
    #MCFLAG_OPT       +=    -DMMI_SETTING_LOW_MEMORY     #�滻��MMI_SETTING_CUTDOWN_MAINMENU��MMI_SETTING_CUTDOWN_WALLPAPER
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_MAINMENU  #���˵���Ч�ü�
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_WALLPAPER #ǽֽ�ü�
    
    MCFLAG_OPT       +=    -DMMI_DIAL_LOW_MEMORY
    MCFLAG_OPT       +=    -DMMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
		#MCFLAG_OPT       +=    -DMMI_PICREVIEW_LIST_LOW_MEMORY
endif 
    MCFLAG_OPT		+= -DPB_SUPPORT_LOW_MEMORY             #PB low memory
    MCFLAG_OPT		+= -DMMIPB_MAX_COUNT_200               #PB support 200.
#    MCFLAG_OPT		+= -DMMIPB_MOST_USED_SUPPORT           #PB support MOST_USED_CONTACT.
#    MCFLAG_OPT		+= -DMMIPB_TOOLBAR_SUPPORT           #PB support toolbar.    
    MCFLAG_OPT		+= -DMMIPB_SIMDN_SUPPORT           #PB support SIMDN.
#    MCFLAG_OPT		+= -DMMIPB_ADD_EXIST_CONTACT_SUPPORT           #PB support ADD_EXIST_CONTACT_SUPPORT
#    MCFLAG_OPT		+= -DMMIPB_SEARCH_RESULT_COLOR_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
    MCFLAG_OPT		+= -DMMIPB_GROUP_MANAGE_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
#    MCFLAG_OPT		+= -DMMIPB_SELECT_DISPLAY_LOCATION_SUPPORT           #PB support SELECT_DISPLAY_LOCATION
#    MCFLAG_OPT		+= -DMMIPB_SELECT_STORE_LOCATION_SUPPORT           #PB support SELECT_STORE_LOCATION     
#    MCFLAG_OPT		+= -DMMIPB_IM_MESSAGE_SUPPORT           #PB im message feild  
    MCFLAG_OPT		+= -DSMS_SUPPORT_LOW_MEMORY            #sms support 200.
    MCFLAG_OPT		+= -DCALENDAR_SUPPORT_LOW_MEMORY       #calendar/schedule support 50.               
    MCFLAG_OPT		+= -DAUDIO_LOW_MEMORY_SUPPORT	       #audio buffer support low memory.
    MCFLAG_OPT		+= -DBT_SUPPORT_LOW_MEMORY             #bt max device number, 10.
    MCFLAG_OPT		+= -DALARM_SUPPORT_LOW_MEMORY          #alarm max number:5
    MCFLAG_OPT		+= -DBOOKMARK_SUPPORT_LOW_MEMORY       #bookmark max number:10
    MCFLAG_OPT		+= -DTTS_SUPPORT_LOW_MEMORY            #tts support low memory, reduce c
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_BUFFER    #streaming support low memory, reduce buffer
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_CONFLICT  #streaming support low memory, conflict      
    MCFLAG_OPT		+= -DRUNTIME_DRAW_SUPPORT
    
    MCFLAG_OPT		+= -DMAIN_MENU_SUPPORT_LOW_MEMORY  #main menu low memory support:icon menu.
    #MCFLAG_OPT		+= -DUI_LAYER_SUPPORT_LOW_MEMORY   #ui layer support 20 max layer blt.    
    MCFLAG_OPT		+= -DPB_NODE_CHANGE_GROUP_SUPPORT_LOW_MEMORY
    MCFLAG_OPT		+= -DIA_JAVA_SUPPORT_LOW_MEMORY    #ia java support low memory project,allocate small memory to jvm if enable this macro
	  MCFLAG_OPT		+= -DLIST_SLIDE_SUPPORT_LOW_MEMORY #list windows forbid slide to support low memory
	  
	  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
#	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HISTORYLIST_SUPPORT   #��ʷ��¼
#	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT  #��ݴ���
#  	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HELP_SUPPORT  #Help����
		endif
		    
    ### mmi slide support
 #   MCFLAG_OPT		+=  -DMMI_SLIDE_SUPPORT 

		ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
			#MCFLAG_OPT		+=  -DBROWSER_SEARCH_SUPPORT	
		  #MCFLAG_OPT		+=  -DBROWSER_BOOKMARK_HISTORY_TAB

			ifneq ($(strip $(BROWSER_OPERA_START_PAGE)), TRUE)
			ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
				ifneq ($(strip $(MMI_SLIDE_SUPPORT)), NONE)
					#startpage is slide style
					#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_THEME1
			  else
			  	#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			  endif		
			else
				#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			endif
			endif
					
			#MCFLAG_OPT		+=  -DMMIDORADO_MY_NAV_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_FIX_NAV_SUPPORT
			#MCFLAG_OPT		+=  -DMMIDORADO_FIX_HMTL_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_BGSOUND_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIBROWSER_DISPLAY_BK_HISTORY_INFO
		  #MCFLAG_OPT		+=  -DMMIDORADO_INPUT_HISTORY_SUPPORT
	  endif
	  
	  ifneq ($(strip $(FM_SUPPORT)), NONE)
	    ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
			  MCFLAG_OPT      += -DMMI_FM_TIMER_RECORD_SUPPORT 
			  MCFLAG_OPT      += -DMMIFM_SUPPORT_FM_RECORD 
			endif 
		ifneq ($(strip $(ATV_SUPPORT)), $(strip $(FM_SUPPORT)))
				MCFLAG_OPT      += -DMMI_FM_NEED_HEADSET
		endif
		endif

		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#��������GO�������Ч���Ŀ��غ�                               
			#MCFLAG_OPT	+= -DGO_BINARY_STAR_MAIN_MENU_SUPPORT    #Binary star
			
			#MCFLAG_OPT	+= -DGO_SHUTTER_MAIN_MENU_SUPPORT  #shutter
			
			#MCFLAG_OPT	+= -DGO_SPHERE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CYLINDER_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SLOPE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_WHEEL_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_OCEANWAVE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_PEGTOP_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_TURNTABLE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BOXROT_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_ICONBOXROT_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BOOKPAGE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_MIDROTATE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SMALLITEMS_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CLASSIC_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_FLASH_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CONSTICK_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SYMMETRY_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BENZITEM_MAIN_MENU_SUPPORT		
		endif

		#�����˵����һ���л�goЧ����ICON
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#MCFLAG_OPT		+=  -DGO_MENU_ONE_ICON_CHANGE_STYLE
		endif

    ifeq ($(strip $(PDA_UI_SUPPORT_U_IDLE)), TRUE)
    	#MCFLAG_OPT += -DMMI_IDLE_WIDGET_CACHE
    endif
	  #֧�ֶ�����˵�����
	  #MCFLAG_OPT += -DMMI_MULTI_MAINMN    	
		#����Ⱦ�˵����
		#MCFLAG_OPT += -DPDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
		#�رճ���go�������������Ч��
		#MCFLAG_OPT		+= -DSLIDE_MAIN_MENU_SUPPORT       #slide style main menu support. 
		#MCFLAG_OPT		+= -DCUBE_MAIN_MENU_SUPPORT            #cube style main menu support. 
		#MCFLAG_OPT		+= -DTRAPEZOID_MAIN_MENU_SUPPORT       #trapezoid main menu support. 
		#MCFLAG_OPT		+= -DBINARY_STAR_MAIN_MENU_SUPPORT          #binary star style main menu support. 
		#MCFLAG_OPT		+= -DSHUTTER_MAIN_MENU_SUPPORT        #shutter main menu support.    
		#MCFLAG_OPT		+= -DCHARIOT_MAIN_MENU_SUPPORT        #chariot main menu support. 
		#MCFLAG_OPT		+= -DSPHERE_MAIN_MENU_SUPPORT          #sphere style main menu support. 
		#MCFLAG_OPT		+= -DCYLINDER_MAIN_MENU_SUPPORT        #cylinder main menu support. 

    #png image support high compress type.
    MCFLAG_OPT		+= -DMMI_RES_SLIM_SUPPORT

    #support app tool bar.
    #MCFLAG_OPT		+= -DGUI_CTRL_TOOLBAR_SUPPORT    

    #support app clipboard slider.
    #MCFLAG_OPT		+= -DCLIPBRD_SLIDER_SUPPORT  

    #support dynamic main menu.
    #MCFLAG_OPT		+= -DDYNAMIC_MAINMENU_SUPPORT 

    #support calendar slide effect.
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)    
        #MCFLAG_OPT		+= -DMMI_CALENDAR_SLIDE    
    endif  

    #support multi sim color support.
    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), SINGLE)
        #MCFLAG_OPT		+=  -DMMI_MULTISIM_COLOR_SUPPORT
    endif

    #support icon main menu style.
    ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
        MCFLAG_OPT		+=  -DMATRIX_MAIN_MENU_SUPPORT
    endif
    
#-------------------QCIF QQVGA MINI --------------------------#    
  MCFLAG_OPT       +=    -DMMI_SAVE_RUNING_NV_INFO
  MCFLAG_OPT       +=    -DMMI_SMS_MAX_NUM_SIM_40_ITEMS
  MCFLAG_OPT       +=    -DMMI_MMS_MAX_ONE_MMS_SIZE_MINI
  MCFLAG_OPT       +=    -DMMI_SMS_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_MMS_MINI_SUPPORT 
  MCFLAG_OPT	   +=    -DMMI_MMS_PREVIEW_LIMIT

ifeq ($(strip $(MAINLCD_SIZE)),128X160)  
  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT
	MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT  
   MCFLAG_OPT       +=    -DMMI_DISPLAYSET_MINI_SUPPORT
   MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT  
   MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
  MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_ALARM_MINI_LOW_COST   
   MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT    
#ͼƬ�ļ�����ͼ��ȡ
# MCFLAG_OPT		    +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT             
#С�ڴ��¼���list����style   
   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT 
   MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT   
endif  

ifeq ($(strip $(MAINLCD_SIZE)),176X220)  
  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT
  MCFLAG_OPT      +=    -DMMI_DISPLAYSET_MINI_SUPPORT   
  MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT  
  MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
  MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
  MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_ALARM_MINI_LOW_COST   
  MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT 
#ͼƬ�ļ�����ͼ��ȡ
 MCFLAG_OPT		     +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT 
#С�ڴ��¼���list����style   
   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT 
   MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT         
endif  

ifeq ($(strip $(MAINLCD_SIZE)),240X320)  

#  MCFLAG_OPT       +=    -DMMI_IDLE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_CALCULATOR_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT
#  MCFLAG_OPT      +=    -DMMI_DISPLAYSET_MINI_SUPPORT   
#  MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT  
#  MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
# MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_ALARM_MINI_LOW_COST   
  MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT 
#ͼƬ�ļ�����ͼ��ȡ
 MCFLAG_OPT		     +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT 
#С�ڴ��¼���list����style   
#   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT 
#   MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT         
endif  

  MCFLAG_OPT       +=    -DMMI_ENVSET_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_CHARGE_MINI_SUPPORT

  MCFLAG_OPT       +=    -DGUI_GRAPH_MINI_SUPPORT

  MCFLAG_OPT       +=    -DGUI_LIST_MINI_SUPPORT
  MCFLAG_OPT       +=    -DGUI_URL_MINI_SUPPORT
  MCFLAG_OPT       +=    -DGUI_STR_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_VIDEO_PLAYER_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_VIDEOPLAYER_MINI_FUNCTION
ifneq ($(strip $(KING_MOVIE_SUPPORT)),NONE)
  MCFLAG_OPT       +=    -DMMI_KINGMOVIE_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_KINGMOVIE_50_MOVIE_SUPPORT 
endif
  
  
	MCFLAG_OPT       +=    -DMMI_AUDIOPLAYER_OPT_MINI_SUPPORT
 
    

  MCFLAG_OPT       +=    -DMMI_PB_MINI_SUPPORT          
  MCFLAG_OPT       +=    -DMMI_FMM_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_GLYPH_CACHE_MINI_SUPPORT
                                             
 
 
#------------------����ͳ�ƹ���----------------
#	MCFLAG_OPT += -DSALES_SUPPORT
#----------------------------------------------
#pbģ������ֻ���һ����¼�ڵ绰����ĸ���
  MCFLAG_OPT        +=    -DMMI_PB_MAX_NV_NUM_ONE
#pbģ���Ƿ�֧��mempool
#  MCFLAG_OPT       +=    -DMMIPB_MEMPOOL_SUPPORT
#ATV�򿪴���ʱת��
  MCFLAG_OPT       +=    -DMMI_ATV_SWITCH_SCREEN_ON_OPEN
#�ļ��������ṩ�������ⲿģ�������ӿ�
  MCFLAG_OPT       +=    -DMMI_FMM_SORT_API_EXT_SUPPORT
#mp3 review ����
#  MCFLAG_OPT         +=  -DMMI_AUDIOPLAYER_REVIEW_SUPPORT
#����pb photo�ü�, PB contact photo feild
#  MCFLAG_OPT         +=  -DMMI_CONTACT_PHOTO_SUPPORT
#����pb ring�ü�, PB contact ring feild
#  MCFLAG_OPT         +=  -DMMI_CONTACT_RING_SUPPORT
#pb�޸�������󳤶�
   MCFLAG_OPT      +=  -DMMI_PB_NAME_LEN_20_BYTES  

#-------------------QCIF QQVGA MINI --------------------------#  

#��ص����ٷֱ���status bar����ʾ�Ĺ��ܺ� ���ú���˫��ع��ܻ��⣬˫�������£�������״̬������ʾ�����ǰ��Ϸ�����ʾ
ifneq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
#MCFLAG_OPT       +=    -DMMI_BATTERY_PERCENT_IN_STATUSBAR
endif
#����������ȡ����sfs֧��xfind����xmove�Ĺ��ܣ���sfs.mk���ж��������ܵĿ���
#������32x64��32x16,16x16֮���size���棬���ԣ�����Ҳֻ��64x64��128x64�д�
		#MCFLAG_OPT		+=  -DMMIFMM_XFIND_SUPPORT
		#MCFLAG_OPT		+=  -DMMIFMM_XMOVE_SUPPORT
endif		
#-------------------------LOW_MEMORY_SUPPORT = 32X16----------------------------------------#  
ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
    ifeq ($(strip $(MULTI_THEME_SUPPORT)), TRUE)
#    MCFLAG_OPT		+=  -DMMI_SAM_THEME
    endif 
    #ͨ����¼����֧���������
    #MCFLAG_OPT		+=  -DMMI_CL_NUM_MAX_SUPPORT
    #MCFLAG_OPT		+=  -DENG_SUPPORT
    #MCFLAG_OPT		+=  -DENG_MINI_SUPPORT
    #dorado������Ƿ���ʾtoolbar(��ʾtoolbarʱ,����ʾsoftkey);���������,�ÿ���Ĭ�ϴ�
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
    #MCFLAG_OPT		+=  -DBROWSER_TOOLBAR
    endif

    ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
	    MCFLAG_OPT		+=  -DMMI_GPRS_SUPPORT
    endif
    
    ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
        MCFLAG_OPT		+=  -DMMI_GPRS_SWITCH_SUPPORT
    endif

    #״̬��������ʾ��Ϣ
    #MCFLAG_OPT		+=  -DMMI_STATUSBAR_SCROLLMSG_SUPPORT

    ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
#            MCFLAG_OPT += -DPDA_TIME_ANIM_SUPPORT
    endif
    #����Ч��֧�֣���MP3ר��ͼƬ��
    #MCFLAG_OPT		+=  -DBRIGHT_EFFECT_SUPPORT
    #u200ͼƬ�༭��
    ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
    	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    		MCFLAG_OPT	+= -DPIC_EDIT_U200_SUPPORT
        endif
    endif
    
    #u200ͼƬ�����
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)    
    ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    MCFLAG_OPT  += -DPIC_PREVIEW_U200_SUPPORT
    #    PIC_QUICKVIEW_SUPPORT = TRUE
    endif
endif
    
    #MCFLAG_OPT	+= -DMMI_TURNON_BACKLIGHT_EFFECT_SUPPORT
    ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
        ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
    #����Ԥ����ʱ�䷭ҳ��Ч        
    #        MCFLAG_OPT += -DPDA_WEATHER_TIME_SE
        endif
    #    MCFLAG_OPT += -DPDA_WEATHER_SUPPORT_LOW_MEMORY    
    endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
    #    MCFLAG_OPT += -DPDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)), TRUE)
#	MCFLAG_OPT		+= -DFEATURE_BIG_ENDIAN

#	MCFLAG_OPT		+= -DSXH_APPLE_APP
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT
#	MCFLAG_OPT		+= -DSXH_APPLE_VP
#	MCFLAG_OPT		+= -DSXH_APPLE_RP
#	MCFLAG_OPT		+= -DSXH_APPLE_3GMUSIC
#	MCFLAG_OPT		+= -DSXH_APPLE_ONLINEGAME
#	MCFLAG_OPT		+= -DSXH_APPLE_VIDEODL
#	MCFLAG_OPT		+= -DSXH_APPLE_UMPLAY
#	MCFLAG_OPT		+= -DSXH_APPLE_BOOK
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT_DEMO
	

#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_APPMAN
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_VP
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_UMVCHAT
endif

#keylock effect start
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT    #�ᡢ�����Ǵ�����֧��

ifeq ($(strip $(LOW_MEMORY_KEYLOCK)), TRUE)
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_QQVGA
endif

ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
ifneq ($(strip $(MAINLCD_SIZE)),176X220)
#michael wang
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DC_SUPPORT_ROTATE
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
#michael wang
#����֧��
ifneq ($(strip $(MMI_RES_ORIENT)), LANDSCAPE)

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
MCFLAG_OPT		+=  -DGUI_CTRL_SCROLL_KEY              #��Ҫ���������2ͬʱ��

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT

ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
endif

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE8_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_PSW_SUPPORT

ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE) 
ifeq ($(strip $(CAMERA_SUPPORT)), TRUE) 
MCFLAG_OPT		+=  -DKEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
endif
endif
endif	# end of LANDSCAPE
endif
endif # end of TOUCHPANEL_TYPE

#keylock effect end

    ##ǽֽ��̬�����ڴ�,��ʹ�þ�̬�ڴ�
    ifeq ($(strip $(SRAM_SIZE)), 16MBIT)
    MCFLAG_OPT	+=  -DMMI_DYNA_ALLOC_WALLPAPER_BUF
    endif

    #GBK support
    ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)), TRUE)
    ifneq ($(strip $(FONT_HAN_SIMP_ONLY)), TRUE)
    ifneq ($(strip $(BITMAP_FONT_SLIM_SUPPORT)), TRUE)
    MCFLAG_OPT		+=  -DGBK_SUPPORT
    endif
    endif
    endif

    MCFLAG_OPT		+=  -DBLUETOOTH_HFG_PLAY_MUSIC_SUPPORT        
	ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			MCFLAG_OPT		+=  -DMMI_IM_PDA_SUPPORT        ###use pda im, when exist touch panel
	endif

	ifeq ($(strip $(EBOOK_SUPPORT)), TRUE)
		#MCFLAG_OPT += -DMMIEBOOK_OSDMENU_SUPPORT		#OSD�����˵�
		ifeq ($(strip $(SE_PAGE_TURN_SUPPORT)), TRUE)
			#MCFLAG_OPT += -DMMIEBOOK_PAGESLIDE_SUPPORT	#��ҳ��Ч
		endif
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			#MCFLAG_OPT += -DMMIEBOOK_BOOKSHELF_SUPPORT	#�������
		endif
	endif
	
	
	ifeq ($(strip $(GAME_SUPPORT)), TRUE)
		#��Ϸ: ������ 
		#MCFLAG_OPT		+=  -DGAME_BOXMAN_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_LINK_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_QUINTET_SUPPORT	
	endif
	
	MMIAP_SUPPORT = TRUE
	MCFLAG_OPT		+=  -DMMI_AUDIO_PLAYER_SUPPORT

  ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
	MCFLAG_OPT	+=	-DMMI_RECORD_SUPPORT
	MCFLAG_OPT	+=	-DMMI_AUTO_RECORD_SUPPORT
	endif
	
	ifeq ($(strip $(SCHEDULE_SUPPORT)), TRUE)
	    MCFLAG_OPT	+=	-DMMI_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_DAILY_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_WEEK_SCHEDULE_SUPPORT
	endif
	
  MCFLAG_OPT		+=  -DMMI_AUDIOPLAYER_ALBUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_SPECTRUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_MUSICMARK_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_SORT_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_LYRIC_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_MUSIC_LIST_SUPPORT
  #		#MP3
  
  MCFLAG_OPT += -DMMI_DIAL_SEARCHING_SUPPORT #�����̶�̬����ƥ�书��
  #MCFLAG_OPT		+=  -DACCOUNT_AUTO_ADAPT_SUPPORT
		#�������ʻ��Զ�ƥ��
#	MCFLAG_OPT		+=  -DDATA_ACCOUNT_IN_TCARD_SUPPORT
    # define DATA_ACCOUNT_IN_TCARD_SUPPORT #�������˻����ݷ�T��
	MCFLAG_OPT		+=  -DCALL_TIMES_SUPPORT
		#ͨ����¼����
#	MCFLAG_OPT		+=  -DMMI_CALL_MINIMIZE_SUPPORT
		#ͨ����С��
#	MCFLAG_OPT += -DMMI_SPEEDDIAL_SUPPORT
			#��ݲ���
	MCFLAG_OPT += -DMMI_IP_CALL_SUPPORT	
	   #IP����
	MCFLAG_OPT += -DMMI_MSD_SUPPORT
			#��������
	MCFLAG_OPT += -DMMI_BLACKLIST_SUPPORT
			#������
  #MCFLAG_OPT		+=  -DCL_SAVE_NAME_SUPPORT
		#ͨ����¼�д洢������		
	MCFLAG_OPT += -DMMI_DIALPANEL_DTMF_SUPPORT
			#������֧��DTMF
	#MCFLAG_OPT += -DMMI_POWER_ON_PWD_SOS_SUPPORT
			#�����������֧��SOS
  #MCFLAG_OPT  += -DMMI_VCALENDAR_SUPPORT
      #֧���ճ̵��롢���������ͺͽ���	
      
## CPHS CSP(ALS) SUPPORT
ifeq ($(strip $(CPHS_SUPPORT)), TRUE)
	MCFLAG_OPT += -DMMI_CPHS_SUPPORT
	ifeq ($(strip $(CSP_SUPPORT)), TRUE)
		MCFLAG_OPT += -DMMI_CSP_SUPPORT
	endif
endif

    MCFLAG_OPT += -DMMI_LDN_SUPPORT
		#LDN Support
ifneq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
#    MCFLAG_OPT +=  -DMMI_REPLACE_SMS_SUPPORT
    	#Replace SMS֧��
#    MCFLAG_OPT += -DMMI_STK_ICON_DISPLAY_SUPPORT
		  #STK ICON Display֧��
    MCFLAG_OPT += -DMMI_SIM_LANGUAGE_SUPPORT

		  #SIM������֧��
		MCFLAG_OPT += -DMMI_SMSCB_SUPPORT
		  #С���㲥
endif
	
	ifneq ($(strip $(ORANGE_SUPPORT)), TRUE)
		#R8
		MCFLAG_OPT += -DMMI_R8_SUPPORT
	endif
	
	#��������ģʽ
	ifeq ($(strip $(SMS_CHAT_SUPPORT)), TRUE)
   ifeq ($(strip $(MMS_SMS_IN_1_SUPPORT)), TRUE)
   	ifeq ($(strip $(CHATMODE_ONLY_SUPPORT)), TRUE)
			MCFLAG_OPT += -DMMI_SMS_CHAT_SUPPORT
		endif
	 endif
  endif
	
	#���ŵ�������
#	MCFLAG_OPT += -DMMI_SMS_EXPORT_SUPPORT
	
	#��ʱ����
#	MCFLAG_OPT += -DMMI_TIMERMSG_SUPPORT
	
	#�ܽӵ绰���Żظ�
	MCFLAG_OPT += -DMMI_REJECT_CALL_BY_SMS_SUPPORT
	
	#������������
#	MCFLAG_OPT += -DMMI_SMS_FONT_SETTING_SUPPORT
	
	#��ȫ����
	MCFLAG_OPT += -DMMI_SMS_SECURITYBOX_SUPPORT

    #ɾ��ȫ��
#   MCFLAG_OPT += -DMMI_SMS_DELALL_SUPPORT	
 		 
    #��������                             
   MCFLAG_OPT += -DMMI_SMS_VOICE_MAIL_SUPPORT	

    #���Ÿ��ƹ���                  
   MCFLAG_OPT += -DMMI_SMS_COPY_SUPPORT	
        
    #�����ƶ�����                  
   MCFLAG_OPT += -DMMI_SMS_MOVE_SUPPORT
 		 
#	MCFLAG_OPT    +=    -DMMI_VCARD_SUPPORT   
	
# idle & widget start
ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)	
ifneq ($(strip $(MAINLCD_SIZE)), 176X220)					
	#MCFLAG_OPT		+= -DGRID_THUMBVIEW_SUPPORT			 #grid idle ����ͼ����
	#MCFLAG_OPT		+= -DGRID_SLIDE_WALLPAPER				 #idle ����ǽֽ������low memoryȫ��
	
	ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
	     #MCFLAG_OPT += -DMMI_WIDGET_WEATHER2			# 2 for PDA sytle; weather 1 for bar phone
       #MCFLAG_OPT += -DMMIWIDGET_WEATHER_ANIM   # ����Ԥ����ҳ����
	  endif
	endif
	
	#MCFLAG_OPT		+=  -DSCH_ASSISTANT_WIDGET    	 # �ճ�����		
	#MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifneq ($(strip $(FM_SUPPORT)), NONE)	
  #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif    
  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Img Viewer
ifeq ($(strip $(COUNTEDTIME_SUPPORT)), TRUE)
  #MCFLAG_OPT		+= -DMMI_GRID_COUNTEDTIME			   #Grid Countedtime
endif
  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
  #MCFLAG_OPT		+= -DMMI_GRID_CONTACT			       #Grid Contact  ��ϵ��
  #MCFLAG_OPT    +=  -DMMI_WIDGET_TIME						 #����ҳЧ����ʱ��  

	
ifeq ($(strip $(WORLD_CLOCK_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_CLOCK   			#����˫ʱ��
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_ACLOCK    #ģ��˫ʱ��
endif
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_SET    	#���ٿ��� wifi bt ������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_MENU    #��ݷ�ʽ ����ʹ��
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ALARM
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ACLOCK   			 #ģ��ʱ��
endif
ifeq ($(strip $(ASP_SUPPORT)), TRUE)  
    #MCFLAG_OPT       +=    -DMMI_WIDGET_AUTOANSWER    #��¼�� ����ʹ��		#�Ѳ���ʹ��   
endif
ifeq ($(strip $(SEARCH_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SEARCH   			 #��������
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETSEARCH  		 #��������
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETWORK   		 #��������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_MEMO   				 #��ǩ 
endif     ### 176X220 
ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
	  #MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
	  #MCFLAG_OPT		+= -DBAR_PHONE_TOUCH_WIDGET			 #bar phone touch idle sytle	   
ifneq ($(strip $(FM_SUPPORT)), NONE)
    #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  #MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
	  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar	
endif     ### 176X220     
endif			### touch panel
  
  ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE) 				#bar phone Ӧ���Զ������漸��widget
	  #MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
	  #MCFLAG_OPT 		+= -DMMI_WIDGET_WEATHER1				 # weather 	
endif	  
ifneq ($(strip $(FM_SUPPORT)), NONE)
    #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  #MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
	  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Image Viewer
	  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
	  #MCFLAG_OPT		+= -DMMI_WIDGET_SPEED_DIAL			 #Grid ���ٲ���	  
  endif
endif		# end of ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
# idle & widget end
	
	#3d rotate effect support.
	ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
	    #MCFLAG_OPT		+= -DSET_3D_STYLE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_ROTATE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_LIST_ROLL_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_MAIN_MENU_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_WIN_SLIDE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_WIN_FADE_SUPPORT
	    #MCFLAG_OPT		+= -DDYNAMIC_WALLPAPER_SUPPORT    
	    #MCFLAG_OPT		+= -DEFFECT_FIRE_WALLPAPER_SUPPORT
	    ifneq ($(strip $(MOTION_SENSOR_TYPE)), NONE) 
	        #MCFLAG_OPT		+= -DEFFECT_LCD_ROTATE_SUPPORT  #lcd rotate effect.
	    endif
	endif
	
	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
		ifeq ($(strip $(SE_ROLL_SUPPORT)), TRUE)
			#    MCFLAG_OPT		+= -DROLL_MAINMENU_SUPPORT
		endif
	endif
	
	ifeq ($(strip $(AMR_A2DP_SUPPORT)), TRUE)
    MCFLAG_OPT      += -DMMI_AMRBT_VPSUPPORT  			#VP��AMR��ʽ��BT֧��
	endif
  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
   	#MCFLAG_OPT		+= -DVIDEOTHUMBNAIL_SUPPORT      	#��Ƶ�ļ�����ͼ��ȡ
  endif

    #MCFLAG_OPT		+= -DTAKESCREENPHOTO_SUPPORT               #��Ļ����

		#MCFLAG_OPT		+= -DMENU_CRYSTALCUBE_SUPPORT
    #MCFLAG_OPT		+= -DMENU_CYCLONE_SUPPORT
    MCFLAG_OPT		+= -DRES_STORE_CACHE_SUPPORT           #resource store cache support.
    MCFLAG_OPT		+= -DRES_VECTOR_CACHE_SUPPORT          #resource vector cache support.
    #MCFLAG_OPT		+= -DPRINTSCREEN_SUPPORT               #��Ļץͼ 
    MCFLAG_OPT		+= -DMMI_AUTOTEST_SUPPORT              #auto test support.
    MCFLAG_OPT		+= -DMMI_AUTOTEST_WINIDNAME_SUPPORT              #auto test winidname support.
    
	  ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
        #MCFLAG_OPT += -DPIC_EDIT_FRAME_SUPPORT        	 #picture editor frame support.
    endif

    #MCFLAG_OPT      += -DCAMERA_FRAME_SUPPORT           #camera photo frame support.

    ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        #MCFLAG_OPT		+= -DEFFECT_ZOOM_SUPPORT           #zoom effect support.
    endif
    
    #320x480�ڴ����ޣ����¹��ܲ�����		
    ifneq ($(strip $(MAINLCD_SIZE)), 320X480)
      #ʹ��ˢ�����ٹ���
      MCFLAG_OPT		+= -DMMI_FPS_ACCELERATE_SUPPORT
      #ʹ��pop������������
      ifeq ($(strip $(MMI_PUBWIN_ANIM_SUPPORT)), TRUE)     
        #MCFLAG_OPT		+= -DMMI_MENUMASKLAYER_SUPPORT     #Menu Mask Layer
      endif
    endif
        
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
      ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        ifeq ($(strip $(SE_RIPPLE_SUPPORT)), TRUE)    
            #MCFLAG_OPT		+= -DEFFECT_RIPPLE_SUPPORT     #ripple effect support.  
        endif
      endif
    endif
    
    MCFLAG_OPT       +=    -DMMI_RES_LOW_COST
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RESOURCE
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_EBOOK    #Ebook�ü�
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_PIC_EDIT #PIC edit�ü�MMIPIC_PASTE_IMAGE_NUM����
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RING     #�����ü�
        
    #MCFLAG_OPT       +=    -DMMI_SETTING_LOW_MEMORY     #�滻��MMI_SETTING_CUTDOWN_MAINMENU��MMI_SETTING_CUTDOWN_WALLPAPER
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_MAINMENU  #���˵���Ч�ü�
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_WALLPAPER #ǽֽ�ü�
    
    MCFLAG_OPT       +=    -DMMI_DIAL_LOW_MEMORY
    MCFLAG_OPT       +=    -DMMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
		#MCFLAG_OPT       +=    -DMMI_PICREVIEW_LIST_LOW_MEMORY
endif 
    MCFLAG_OPT		+= -DPB_SUPPORT_LOW_MEMORY             #PB low memory
    MCFLAG_OPT		+= -DMMIPB_MAX_COUNT_200               #PB support 200.
#    MCFLAG_OPT		+= -DMMIPB_MOST_USED_SUPPORT           #PB support MOST_USED_CONTACT.
#    MCFLAG_OPT		+= -DMMIPB_TOOLBAR_SUPPORT           #PB support toolbar.    
    MCFLAG_OPT		+= -DMMIPB_SIMDN_SUPPORT           #PB support SIMDN.
#    MCFLAG_OPT		+= -DMMIPB_ADD_EXIST_CONTACT_SUPPORT           #PB support ADD_EXIST_CONTACT_SUPPORT
#    MCFLAG_OPT		+= -DMMIPB_SEARCH_RESULT_COLOR_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
#    MCFLAG_OPT		+= -DMMIPB_GROUP_MANAGE_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
#    MCFLAG_OPT		+= -DMMIPB_SELECT_DISPLAY_LOCATION_SUPPORT           #PB support SELECT_DISPLAY_LOCATION
#    MCFLAG_OPT		+= -DMMIPB_SELECT_STORE_LOCATION_SUPPORT           #PB support SELECT_STORE_LOCATION     
#    MCFLAG_OPT		+= -DMMIPB_IM_MESSAGE_SUPPORT           #PB im message feild  
    MCFLAG_OPT		+= -DSMS_SUPPORT_LOW_MEMORY            #sms support 200.
    MCFLAG_OPT		+= -DCALENDAR_SUPPORT_LOW_MEMORY       #calendar/schedule support 50.               
    MCFLAG_OPT		+= -DAUDIO_LOW_MEMORY_SUPPORT	       #audio buffer support low memory.
    MCFLAG_OPT		+= -DBT_SUPPORT_LOW_MEMORY             #bt max device number, 10.
    MCFLAG_OPT		+= -DALARM_SUPPORT_LOW_MEMORY          #alarm max number:5
    MCFLAG_OPT		+= -DBOOKMARK_SUPPORT_LOW_MEMORY       #bookmark max number:10
    MCFLAG_OPT		+= -DTTS_SUPPORT_LOW_MEMORY            #tts support low memory, reduce c
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_BUFFER    #streaming support low memory, reduce buffer
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_CONFLICT  #streaming support low memory, conflict      
    MCFLAG_OPT		+= -DRUNTIME_DRAW_SUPPORT
    
    MCFLAG_OPT		+= -DMAIN_MENU_SUPPORT_LOW_MEMORY  #main menu low memory support:icon menu.
    #MCFLAG_OPT		+= -DUI_LAYER_SUPPORT_LOW_MEMORY   #ui layer support 20 max layer blt.    
    MCFLAG_OPT		+= -DPB_NODE_CHANGE_GROUP_SUPPORT_LOW_MEMORY
	  MCFLAG_OPT		+= -DLIST_SLIDE_SUPPORT_LOW_MEMORY #list windows forbid slide to support low memory
	  
	  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
#	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HISTORYLIST_SUPPORT   #��ʷ��¼
#	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT  #��ݴ���
  	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HELP_SUPPORT  #Help����
		endif
		    
    ### mmi slide support
 #   MCFLAG_OPT		+=  -DMMI_SLIDE_SUPPORT 

		ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
			#MCFLAG_OPT		+=  -DBROWSER_SEARCH_SUPPORT	
		  #MCFLAG_OPT		+=  -DBROWSER_BOOKMARK_HISTORY_TAB
		  #MCFLAG_OPT		+=  -DBROWSER_SUPPORT_LOW_MEMORY

			ifneq ($(strip $(BROWSER_OPERA_START_PAGE)), TRUE)
			ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
				ifneq ($(strip $(MMI_SLIDE_SUPPORT)), NONE)
					#startpage is slide style
					#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_THEME1
			  else
			  	#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			  endif		
			else
				MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			endif
			endif
					
			#MCFLAG_OPT		+=  -DMMIDORADO_MY_NAV_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_FIX_NAV_SUPPORT
			#MCFLAG_OPT		+=  -DMMIDORADO_FIX_HMTL_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_BGSOUND_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIBROWSER_DISPLAY_BK_HISTORY_INFO
		  #MCFLAG_OPT		+=  -DMMIDORADO_INPUT_HISTORY_SUPPORT
	  endif

	  
	  ifneq ($(strip $(FM_SUPPORT)), NONE)
	    ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
			  MCFLAG_OPT      += -DMMI_FM_TIMER_RECORD_SUPPORT 
			  MCFLAG_OPT      += -DMMIFM_SUPPORT_FM_RECORD
			endif 
		ifneq ($(strip $(ATV_SUPPORT)), $(strip $(FM_SUPPORT)))
				MCFLAG_OPT      += -DMMI_FM_NEED_HEADSET
		endif
		endif
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#��������GO�������Ч���Ŀ��غ�                               
			#MCFLAG_OPT	+= -DGO_BINARY_STAR_MAIN_MENU_SUPPORT    #Binary star
			
			#MCFLAG_OPT	+= -DGO_SHUTTER_MAIN_MENU_SUPPORT  #shutter
			
			#MCFLAG_OPT	+= -DGO_SPHERE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CYLINDER_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SLOPE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_WHEEL_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_OCEANWAVE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_PEGTOP_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_TURNTABLE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BOXROT_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_ICONBOXROT_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BOOKPAGE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_MIDROTATE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SMALLITEMS_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CLASSIC_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_FLASH_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CONSTICK_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SYMMETRY_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BENZITEM_MAIN_MENU_SUPPORT		
		endif

		#�����˵����һ���л�goЧ����ICON
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#MCFLAG_OPT		+=  -DGO_MENU_ONE_ICON_CHANGE_STYLE
		endif

    ifeq ($(strip $(PDA_UI_SUPPORT_U_IDLE)), TRUE)
    	#MCFLAG_OPT += -DMMI_IDLE_WIDGET_CACHE
    endif
	  #֧�ֶ�����˵�����
	  #MCFLAG_OPT += -DMMI_MULTI_MAINMN       	
		#����Ⱦ�˵����
		#MCFLAG_OPT += -DPDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
		#�رճ���go�������������Ч��
		#MCFLAG_OPT		+= -DSLIDE_MAIN_MENU_SUPPORT       #slide style main menu support. 
		#MCFLAG_OPT		+= -DCUBE_MAIN_MENU_SUPPORT            #cube style main menu support. 
		#MCFLAG_OPT		+= -DTRAPEZOID_MAIN_MENU_SUPPORT       #trapezoid main menu support. 
		#MCFLAG_OPT		+= -DBINARY_STAR_MAIN_MENU_SUPPORT          #binary star style main menu support. 
		#MCFLAG_OPT		+= -DSHUTTER_MAIN_MENU_SUPPORT        #shutter main menu support.    
		#MCFLAG_OPT		+= -DCHARIOT_MAIN_MENU_SUPPORT        #chariot main menu support. 
		#MCFLAG_OPT		+= -DSPHERE_MAIN_MENU_SUPPORT          #sphere style main menu support. 
		#MCFLAG_OPT		+= -DCYLINDER_MAIN_MENU_SUPPORT        #cylinder main menu support. 

    #png image support high compress type.
    MCFLAG_OPT		+= -DMMI_RES_SLIM_SUPPORT

    #support app tool bar.
    #MCFLAG_OPT		+= -DGUI_CTRL_TOOLBAR_SUPPORT    

    #support app clipboard slider.
    #MCFLAG_OPT		+= -DCLIPBRD_SLIDER_SUPPORT  

    #support dynamic main menu.
    #MCFLAG_OPT		+= -DDYNAMIC_MAINMENU_SUPPORT 

    #support calendar slide effect.
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)    
        #MCFLAG_OPT		+= -DMMI_CALENDAR_SLIDE    
    endif  

    #support multi sim color support.
    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), SINGLE)
        #MCFLAG_OPT		+=  -DMMI_MULTISIM_COLOR_SUPPORT
    endif

    #support icon main menu style.
    ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
        MCFLAG_OPT		+=  -DMATRIX_MAIN_MENU_SUPPORT
    endif

#-------------------QCIF QQVGA MINI --------------------------# 
  MCFLAG_OPT       +=    -DMMI_SAVE_RUNING_NV_INFO
  MCFLAG_OPT       +=    -DMMI_SMS_MAX_NUM_SIM_40_ITEMS
  MCFLAG_OPT       +=    -DMMI_MMS_MAX_ONE_MMS_SIZE_MINI
  MCFLAG_OPT       +=    -DMMI_SMS_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_MMS_MINI_SUPPORT     
  MCFLAG_OPT	   +=    -DMMI_MMS_PREVIEW_LIMIT  

ifeq ($(strip $(MAINLCD_SIZE)),128X160)  
  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT

  MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT  
 
endif  
ifeq ($(strip $(MAINLCD_SIZE)),176X220)  
  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT

  MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT  

endif  
  
  MCFLAG_OPT       +=    -DMMI_ENVSET_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_CHARGE_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_DISPLAYSET_MINI_SUPPORT

  MCFLAG_OPT       +=    -DGUI_GRAPH_MINI_SUPPORT
  MCFLAG_OPT       +=    -DGUI_LIST_MINI_SUPPORT
  MCFLAG_OPT       +=    -DGUI_URL_MINI_SUPPORT
  MCFLAG_OPT       +=    -DGUI_STR_MINI_SUPPORT
  #MCFLAG_OPT       +=    -DMMI_VIDEO_PLAYER_MINI_SUPPORT

#  MCFLAG_OPT       +=    -DMMI_VIDEOPLAYER_MINI_FUNCTION
  MCFLAG_OPT       +=    -DMMI_KINGMOVIE_MINI_SUPPORT
	MCFLAG_OPT       +=    -DMMI_AUDIOPLAYER_OPT_MINI_SUPPORT

 
  

    
  MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT      
  MCFLAG_OPT       +=    -DMMI_PB_MINI_SUPPORT      
     
  MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
  MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
  MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT      

  MCFLAG_OPT       +=    -DMMI_ALARM_MINI_LOW_COST   
     
  MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT      
  MCFLAG_OPT       +=    -DMMI_FMM_MINI_SUPPORT

  MCFLAG_OPT       +=    -DMMI_GLYPH_CACHE_MINI_SUPPORT
 
#------------------����ͳ�ƹ���----------------
	#MCFLAG_OPT += -DSALES_SUPPORT
#----------------------------------------------
#pbģ���Ƿ�֧��mempool
#  MCFLAG_OPT       +=    -DMMIPB_MEMPOOL_SUPPORT
#ATV�򿪴���ʱת��
  MCFLAG_OPT       +=    -DMMI_ATV_SWITCH_SCREEN_ON_OPEN
#�ļ��������ṩ�������ⲿģ�������ӿ�
  MCFLAG_OPT       +=    -DMMI_FMM_SORT_API_EXT_SUPPORT
#ͼƬ�ļ�����ͼ��ȡ
# MCFLAG_OPT		   +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT  
#mp3 review ����
#  MCFLAG_OPT         +=  -DMMI_AUDIOPLAYER_REVIEW_SUPPORT
#����pb photo�ü�, PB contact photo feild
  MCFLAG_OPT         +=  -DMMI_CONTACT_PHOTO_SUPPORT
#����pb ring�ü�, PB contact ring feild
  MCFLAG_OPT         +=  -DMMI_CONTACT_RING_SUPPORT
#pb�޸�������󳤶�
   MCFLAG_OPT      +=  -DMMI_PB_NAME_LEN_20_BYTES  
  
#С�ڴ��¼���list����style   
   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT   
#-------------------QCIF QQVGA MINI --------------------------#      

#��ص����ٷֱ���status bar����ʾ�Ĺ��ܺ� ���ú���˫��ع��ܻ��⣬˫�������£�������״̬������ʾ�����ǰ��Ϸ�����ʾ
ifneq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
#MCFLAG_OPT       +=    -DMMI_BATTERY_PERCENT_IN_STATUSBAR
endif
#����������ȡ����sfs֧��xfind����xmove�Ĺ��ܣ���sfs.mk���ж��������ܵĿ���
#������32x64��32x16,16x16֮���size���棬���ԣ�����Ҳֻ��64x64��128x64�д�
		#MCFLAG_OPT		+=  -DMMIFMM_XFIND_SUPPORT
		#MCFLAG_OPT		+=  -DMMIFMM_XMOVE_SUPPORT
endif	
#-------------------------LOW_MEMORY_SUPPORT = 32X32----------------------------------------#  
ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X32)
    ifeq ($(strip $(MULTI_THEME_SUPPORT)), TRUE)
#    MCFLAG_OPT		+=  -DMMI_SAM_THEME
    endif 
    #ͨ����¼����֧���������
    #MCFLAG_OPT		+=  -DMMI_CL_NUM_MAX_SUPPORT
    #MCFLAG_OPT		+=  -DENG_MINI_SUPPORT
ifeq ($(strip $(MAINLCD_SIZE)), 240X320)
    MCFLAG_OPT		+=  -DENG_MINI_SUPPORT
endif 
    #dorado������Ƿ���ʾtoolbar(��ʾtoolbarʱ,����ʾsoftkey);���������,�ÿ���Ĭ�ϴ�
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
    #MCFLAG_OPT		+=  -DBROWSER_TOOLBAR
    endif

    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), NONE)
        ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
	    MCFLAG_OPT		+=  -DMMI_GPRS_SUPPORT
        endif
        #MCFLAG_OPT		+=  -DENG_SUPPORT
        MCFLAG_OPT		+=  -DSTK_SUPPORT
    endif
    
    ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
        MCFLAG_OPT		+=  -DMMI_GPRS_SWITCH_SUPPORT
    endif

    #״̬��������ʾ��Ϣ
    #MCFLAG_OPT		+=  -DMMI_STATUSBAR_SCROLLMSG_SUPPORT

    ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
#            MCFLAG_OPT += -DPDA_TIME_ANIM_SUPPORT
    endif
    #����Ч��֧�֣���MP3ר��ͼƬ��
    #MCFLAG_OPT		+=  -DBRIGHT_EFFECT_SUPPORT
    #u200ͼƬ�༭��
    ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
    	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    		MCFLAG_OPT	+= -DPIC_EDIT_U200_SUPPORT
        endif
    endif
    
    #u200ͼƬ�����
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)    
    ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    MCFLAG_OPT  += -DPIC_PREVIEW_U200_SUPPORT
    #    PIC_QUICKVIEW_SUPPORT = TRUE
    endif
endif
    
    #MCFLAG_OPT	+= -DMMI_TURNON_BACKLIGHT_EFFECT_SUPPORT
    ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
        ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
    #����Ԥ����ʱ�䷭ҳ��Ч        
    #        MCFLAG_OPT += -DPDA_WEATHER_TIME_SE
        endif
    #    MCFLAG_OPT += -DPDA_WEATHER_SUPPORT_LOW_MEMORY    
    endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
    #    MCFLAG_OPT += -DPDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)), TRUE)
#	MCFLAG_OPT		+= -DFEATURE_BIG_ENDIAN

#	MCFLAG_OPT		+= -DSXH_APPLE_APP
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT
#	MCFLAG_OPT		+= -DSXH_APPLE_VP
#	MCFLAG_OPT		+= -DSXH_APPLE_RP
#	MCFLAG_OPT		+= -DSXH_APPLE_3GMUSIC
#	MCFLAG_OPT		+= -DSXH_APPLE_ONLINEGAME
#	MCFLAG_OPT		+= -DSXH_APPLE_VIDEODL
#	MCFLAG_OPT		+= -DSXH_APPLE_UMPLAY
#	MCFLAG_OPT		+= -DSXH_APPLE_BOOK
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT_DEMO
	

#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_APPMAN
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_VP
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_UMVCHAT
endif

#keylock effect start
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT    #�ᡢ�����Ǵ�����֧��

ifeq ($(strip $(LOW_MEMORY_KEYLOCK)), TRUE)
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_QQVGA
endif

ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
ifneq ($(strip $(MAINLCD_SIZE)),176X220)
ifneq ($(strip $(MAINLCD_SIZE)),240X320)
#michael wang
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DC_SUPPORT_ROTATE
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
#michael wang
#����֧��
ifneq ($(strip $(MMI_RES_ORIENT)), LANDSCAPE)

###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT

###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
###MCFLAG_OPT		+=  -DGUI_CTRL_SCROLL_KEY              #��Ҫ���������2ͬʱ��

###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT

ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
endif

###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT

###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE8_SUPPORT

###MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_PSW_SUPPORT

ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE) 
ifeq ($(strip $(CAMERA_SUPPORT)), TRUE) 
###MCFLAG_OPT		+=  -DKEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
endif
endif
endif	# end of LANDSCAPE
endif
endif
endif # end of TOUCHPANEL_TYPE

#keylock effect end

    ##ǽֽ��̬�����ڴ�,��ʹ�þ�̬�ڴ�
    ifeq ($(strip $(SRAM_SIZE)), 16MBIT)
    ###MCFLAG_OPT	+=  -DMMI_DYNA_ALLOC_WALLPAPER_BUF
    endif

    #GBK support
    ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)), TRUE)
    ifneq ($(strip $(FONT_HAN_SIMP_ONLY)), TRUE)
    ifneq ($(strip $(BITMAP_FONT_SLIM_SUPPORT)), TRUE)
    MCFLAG_OPT		+=  -DGBK_SUPPORT
    endif
    endif
    endif

    ###MCFLAG_OPT		+=  -DBLUETOOTH_HFG_PLAY_MUSIC_SUPPORT        
	ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			MCFLAG_OPT		+=  -DMMI_IM_PDA_SUPPORT        ###use pda im, when exist touch panel
	endif

	ifeq ($(strip $(EBOOK_SUPPORT)), TRUE)
		#MCFLAG_OPT += -DMMIEBOOK_OSDMENU_SUPPORT		#OSD�����˵�
		ifeq ($(strip $(SE_PAGE_TURN_SUPPORT)), TRUE)
			#MCFLAG_OPT += -DMMIEBOOK_PAGESLIDE_SUPPORT	#��ҳ��Ч
		endif
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			#MCFLAG_OPT += -DMMIEBOOK_BOOKSHELF_SUPPORT	#�������
		endif
	endif
	
	
	ifeq ($(strip $(GAME_SUPPORT)), TRUE)
		#��Ϸ: ������ 
		#MCFLAG_OPT		+=  -DGAME_BOXMAN_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_LINK_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_QUINTET_SUPPORT	
	endif
	
	###MMIAP_SUPPORT = TRUE
	MCFLAG_OPT		+=  -DMMI_AUDIO_PLAYER_SUPPORT

  ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
	MCFLAG_OPT	+=	-DMMI_RECORD_SUPPORT
	MCFLAG_OPT	+=	-DMMI_AUTO_RECORD_SUPPORT
	endif
	
	ifeq ($(strip $(SCHEDULE_SUPPORT)), TRUE)
	    MCFLAG_OPT	+=	-DMMI_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_DAILY_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_WEEK_SCHEDULE_SUPPORT
	endif
	
  	
ifneq ($(strip $(MAINLCD_SIZE)),240X320)
  MCFLAG_OPT		+=  -DMMI_AUDIOPLAYER_ALBUM_SUPPORT
endif
ifeq ($(strip $(MAINLCD_SIZE)),240X320)
  #MCFLAG_OPT		+=  -DMMI_AUDIOPLAYER_ALBUM_SUPPORT
endif
  #MCFLAG_OPT		+=  -DMMIAP_SPECTRUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_MUSICMARK_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_SORT_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_LYRIC_SUPPORT
  MCFLAG_OPT         +=  -DMMIAP_MUSIC_LIST_SUPPORT
  #		#MP3

	MCFLAG_OPT += -DMMI_DIAL_SEARCHING_SUPPORT #�����̶�̬����ƥ�书�� 
  MCFLAG_OPT		+=  -DACCOUNT_AUTO_ADAPT_SUPPORT
		#�������ʻ��Զ�ƥ��
#	MCFLAG_OPT		+=  -DDATA_ACCOUNT_IN_TCARD_SUPPORT
    # define DATA_ACCOUNT_IN_TCARD_SUPPORT #�������˻����ݷ�T��
	MCFLAG_OPT		+=  -DCALL_TIMES_SUPPORT
		#ͨ����¼����
#	MCFLAG_OPT		+=  -DMMI_CALL_MINIMIZE_SUPPORT
		#ͨ����С��
#	MCFLAG_OPT += -DMMI_SPEEDDIAL_SUPPORT
			#��ݲ���
	###MCFLAG_OPT += -DMMI_IP_CALL_SUPPORT	
	   #IP����
	###MCFLAG_OPT += -DMMI_MSD_SUPPORT
			#��������
	MCFLAG_OPT += -DMMI_BLACKLIST_SUPPORT
			#������
  #MCFLAG_OPT		+=  -DCL_SAVE_NAME_SUPPORT
		#ͨ����¼�д洢������		
	MCFLAG_OPT += -DMMI_DIALPANEL_DTMF_SUPPORT
			#������֧��DTMF
	#MCFLAG_OPT += -DMMI_POWER_ON_PWD_SOS_SUPPORT
			#�����������֧��SOS
  #MCFLAG_OPT  += -DMMI_VCALENDAR_SUPPORT
      #֧���ճ̵��롢���������ͺͽ���	
      
## CPHS CSP(ALS) SUPPORT
ifeq ($(strip $(CPHS_SUPPORT)), TRUE)
	MCFLAG_OPT += -DMMI_CPHS_SUPPORT
	ifeq ($(strip $(CSP_SUPPORT)), TRUE)
		MCFLAG_OPT += -DMMI_CSP_SUPPORT
	endif
endif
      
    MCFLAG_OPT += -DMMI_LDN_SUPPORT
		#LDN Support
ifneq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
#    MCFLAG_OPT +=  -DMMI_REPLACE_SMS_SUPPORT
    	#Replace SMS֧��
#    MCFLAG_OPT += -DMMI_STK_ICON_DISPLAY_SUPPORT
		  #STK ICON Display֧��

#################################################################    
#SIM������֧��;MMI_ONLY_CHINESE_DISPLAY_SUPPORT�򿪣�����ص�MMI_SIM_LANGUAGE_SUPPORT
ifeq ($(strip $(LOW_MEMORY_SUPPORT)),32X32)
	ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)),TRUE)
	   #MCFLAG_OPT += -DMMI_SIM_LANGUAGE_SUPPORT
	   MCFLAG_OPT  += -DMMI_ONLY_CHINESE_DISPLAY_SUPPORT
	else
		 MCFLAG_OPT += -DMMI_SIM_LANGUAGE_SUPPORT
	endif
endif
#################################################################	
		  
		MCFLAG_OPT += -DMMI_SMSCB_SUPPORT
		  #С���㲥
endif
	
	ifneq ($(strip $(ORANGE_SUPPORT)), TRUE)
		#R8
		MCFLAG_OPT += -DMMI_R8_SUPPORT
	endif
	
	#��������ģʽ
	ifeq ($(strip $(SMS_CHAT_SUPPORT)), TRUE)
   ifeq ($(strip $(MMS_SMS_IN_1_SUPPORT)), TRUE)
   	ifeq ($(strip $(CHATMODE_ONLY_SUPPORT)), TRUE)
			MCFLAG_OPT += -DMMI_SMS_CHAT_SUPPORT
		endif
	 endif
  endif
	
	#���ŵ�������
#	MCFLAG_OPT += -DMMI_SMS_EXPORT_SUPPORT
	
	#��ʱ����
#	MCFLAG_OPT += -DMMI_TIMERMSG_SUPPORT
	
	#�ܽӵ绰���Żظ�
	MCFLAG_OPT += -DMMI_REJECT_CALL_BY_SMS_SUPPORT
	
	#������������
#	MCFLAG_OPT += -DMMI_SMS_FONT_SETTING_SUPPORT
	
	#��ȫ����
	###MCFLAG_OPT += -DMMI_SMS_SECURITYBOX_SUPPORT

    #ɾ��ȫ��
#   MCFLAG_OPT += -DMMI_SMS_DELALL_SUPPORT	
 		 
    #��������                             
   MCFLAG_OPT += -DMMI_SMS_VOICE_MAIL_SUPPORT	

    #���Ÿ��ƹ���                  
   MCFLAG_OPT += -DMMI_SMS_COPY_SUPPORT	
        
    #�����ƶ�����                  
   MCFLAG_OPT += -DMMI_SMS_MOVE_SUPPORT
 		 
	MCFLAG_OPT    +=    -DMMI_VCARD_SUPPORT   
	
# idle & widget start
ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)	
ifneq ($(strip $(MAINLCD_SIZE)), 176X220)					
	#MCFLAG_OPT		+= -DGRID_THUMBVIEW_SUPPORT			 #grid idle ����ͼ����
	#MCFLAG_OPT		+= -DGRID_SLIDE_WALLPAPER				 #idle ����ǽֽ������low memoryȫ��
	
	ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
	     #MCFLAG_OPT += -DMMI_WIDGET_WEATHER2			# 2 for PDA sytle; weather 1 for bar phone
       #MCFLAG_OPT += -DMMIWIDGET_WEATHER_ANIM   # ����Ԥ����ҳ����
	  endif
	endif
	
	#MCFLAG_OPT		+=  -DSCH_ASSISTANT_WIDGET    	 # �ճ�����		
	#MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifneq ($(strip $(FM_SUPPORT)), NONE)	
  #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif    
  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Img Viewer
ifeq ($(strip $(COUNTEDTIME_SUPPORT)), TRUE)
  #MCFLAG_OPT		+= -DMMI_GRID_COUNTEDTIME			   #Grid Countedtime
endif
  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
  #MCFLAG_OPT		+= -DMMI_GRID_CONTACT			       #Grid Contact  ��ϵ��
  #MCFLAG_OPT    +=  -DMMI_WIDGET_TIME						 #����ҳЧ����ʱ��  

	
ifeq ($(strip $(WORLD_CLOCK_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_CLOCK   			#����˫ʱ��
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_ACLOCK    #ģ��˫ʱ��
endif
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_SET    	#���ٿ��� wifi bt ������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_MENU    #��ݷ�ʽ ����ʹ��
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ALARM
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ACLOCK   			 #ģ��ʱ��
endif
ifeq ($(strip $(ASP_SUPPORT)), TRUE)  
    #MCFLAG_OPT       +=    -DMMI_WIDGET_AUTOANSWER    #��¼�� ����ʹ��		#�Ѳ���ʹ��   
endif
ifeq ($(strip $(SEARCH_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SEARCH   			 #��������
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETSEARCH  		 #��������
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETWORK   		 #��������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_MEMO   				 #��ǩ 
endif     ### 176X220 
ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
	  #MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
	  #MCFLAG_OPT		+= -DBAR_PHONE_TOUCH_WIDGET			 #bar phone touch idle sytle	   
ifneq ($(strip $(FM_SUPPORT)), NONE)
    #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  #MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
	  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar	
endif     ### 176X220     
endif			### touch panel
  
  ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE) 				#bar phone Ӧ���Զ������漸��widget
	  MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
	  MCFLAG_OPT		+= -DMMI_MINI_BAR_PHONE_WIDGET_SUPPORT #֧��Grid FM��Grid Mp3��Grid Calendar
ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
	  MCFLAG_OPT 		+= -DMMI_WIDGET_WEATHER1				 # weather 	
endif	  
ifneq ($(strip $(FM_SUPPORT)), NONE)
    MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)	  
	  MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Image Viewer
endif	  
	  MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
	  #MCFLAG_OPT		+= -DMMI_WIDGET_SPEED_DIAL			 #Grid ���ٲ���	  
  endif
endif		# end of ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
# idle & widget end
	
	#3d rotate effect support.
	ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
	    #MCFLAG_OPT		+= -DSET_3D_STYLE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_ROTATE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_LIST_ROLL_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_3D_MAIN_MENU_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_WIN_SLIDE_SUPPORT
	    #MCFLAG_OPT		+= -DEFFECT_WIN_FADE_SUPPORT
	    #MCFLAG_OPT		+= -DDYNAMIC_WALLPAPER_SUPPORT    
	    #MCFLAG_OPT		+= -DEFFECT_FIRE_WALLPAPER_SUPPORT
	    ifneq ($(strip $(MOTION_SENSOR_TYPE)), NONE) 
	        #MCFLAG_OPT		+= -DEFFECT_LCD_ROTATE_SUPPORT  #lcd rotate effect.
	    endif
	endif
	
	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
		ifeq ($(strip $(SE_ROLL_SUPPORT)), TRUE)
			#    MCFLAG_OPT		+= -DROLL_MAINMENU_SUPPORT
		endif
	endif
	
	ifeq ($(strip $(AMR_A2DP_SUPPORT)), TRUE)
    MCFLAG_OPT      += -DMMI_AMRBT_VPSUPPORT  			#VP��AMR��ʽ��BT֧��
	endif
  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
   	MCFLAG_OPT		+= -DVIDEOTHUMBNAIL_SUPPORT      	#��Ƶ�ļ�����ͼ��ȡ
  endif

    #MCFLAG_OPT		+= -DTAKESCREENPHOTO_SUPPORT               #��Ļ����

		#MCFLAG_OPT		+= -DMENU_CRYSTALCUBE_SUPPORT
    #MCFLAG_OPT		+= -DMENU_CYCLONE_SUPPORT
    MCFLAG_OPT		+= -DRES_STORE_CACHE_SUPPORT           #resource store cache support.
    MCFLAG_OPT		+= -DRES_VECTOR_CACHE_SUPPORT          #resource vector cache support.
    #MCFLAG_OPT		+= -DPRINTSCREEN_SUPPORT               #��Ļץͼ 
    ###MCFLAG_OPT		+= -DMMI_AUTOTEST_SUPPORT              #auto test support.
    ###MCFLAG_OPT		+= -DMMI_AUTOTEST_WINIDNAME_SUPPORT              #auto test winidname support.
    
	  ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
        #MCFLAG_OPT += -DPIC_EDIT_FRAME_SUPPORT        	 #picture editor frame support.
    endif

    #MCFLAG_OPT      += -DCAMERA_FRAME_SUPPORT           #camera photo frame support.

    ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        #MCFLAG_OPT		+= -DEFFECT_ZOOM_SUPPORT           #zoom effect support.
    endif
    
    #320x480�ڴ����ޣ����¹��ܲ�����		
    ifneq ($(strip $(MAINLCD_SIZE)), 320X480)
      #ʹ��ˢ�����ٹ���
      MCFLAG_OPT		+= -DMMI_FPS_ACCELERATE_SUPPORT
      #ʹ��pop������������
      ifeq ($(strip $(MMI_PUBWIN_ANIM_SUPPORT)), TRUE)     
        MCFLAG_OPT		+= -DMMI_MENUMASKLAYER_SUPPORT     #Menu Mask Layer
      endif
    endif
        
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
      ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        ifeq ($(strip $(SE_RIPPLE_SUPPORT)), TRUE)    
            #MCFLAG_OPT		+= -DEFFECT_RIPPLE_SUPPORT     #ripple effect support.  
        endif
      endif
    endif
    
    MCFLAG_OPT       +=    -DMMI_RES_LOW_COST
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RESOURCE
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_EBOOK    #Ebook�ü�
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_PIC_EDIT #PIC edit�ü�MMIPIC_PASTE_IMAGE_NUM����
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RING     #�����ü�
        
    #MCFLAG_OPT       +=    -DMMI_SETTING_LOW_MEMORY     #�滻��MMI_SETTING_CUTDOWN_MAINMENU��MMI_SETTING_CUTDOWN_WALLPAPER
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_MAINMENU  #���˵���Ч�ü�
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_WALLPAPER #ǽֽ�ü�
    
    MCFLAG_OPT       +=    -DMMI_DIAL_LOW_MEMORY
    MCFLAG_OPT       +=    -DMMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
		#MCFLAG_OPT       +=    -DMMI_PICREVIEW_LIST_LOW_MEMORY
endif 
    MCFLAG_OPT		+= -DPB_SUPPORT_LOW_MEMORY             #PB low memory
    MCFLAG_OPT		+= -DMMIPB_MAX_COUNT_200               #PB support 200.
#    MCFLAG_OPT		+= -DMMIPB_MOST_USED_SUPPORT           #PB support MOST_USED_CONTACT.
#    MCFLAG_OPT		+= -DMMIPB_TOOLBAR_SUPPORT           #PB support toolbar.    
    MCFLAG_OPT		+= -DMMIPB_SIMDN_SUPPORT           #PB support SIMDN.
    MCFLAG_OPT		+= -DMMIPB_ADD_EXIST_CONTACT_SUPPORT           #PB support ADD_EXIST_CONTACT_SUPPORT
#    MCFLAG_OPT		+= -DMMIPB_SEARCH_RESULT_COLOR_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
    MCFLAG_OPT		+= -DMMIPB_GROUP_MANAGE_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT . 
#    MCFLAG_OPT		+= -DMMIPB_SELECT_DISPLAY_LOCATION_SUPPORT           #PB support SELECT_DISPLAY_LOCATION
#    MCFLAG_OPT		+= -DMMIPB_SELECT_STORE_LOCATION_SUPPORT           #PB support SELECT_STORE_LOCATION     
#    MCFLAG_OPT		+= -DMMIPB_IM_MESSAGE_SUPPORT           #PB im message feild  
    MCFLAG_OPT		+= -DSMS_SUPPORT_LOW_MEMORY            #sms support 200.
    MCFLAG_OPT		+= -DCALENDAR_SUPPORT_LOW_MEMORY       #calendar/schedule support 50.               
    MCFLAG_OPT		+= -DAUDIO_LOW_MEMORY_SUPPORT	       #audio buffer support low memory.
    MCFLAG_OPT		+= -DBT_SUPPORT_LOW_MEMORY             #bt max device number, 10.
    MCFLAG_OPT		+= -DALARM_SUPPORT_LOW_MEMORY          #alarm max number:5
    MCFLAG_OPT		+= -DBOOKMARK_SUPPORT_LOW_MEMORY       #bookmark max number:10
    MCFLAG_OPT		+= -DTTS_SUPPORT_LOW_MEMORY            #tts support low memory, reduce c
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_BUFFER    #streaming support low memory, reduce buffer
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_CONFLICT  #streaming support low memory, conflict      
    MCFLAG_OPT		+= -DRUNTIME_DRAW_SUPPORT
    
    MCFLAG_OPT		+= -DMAIN_MENU_SUPPORT_LOW_MEMORY  #main menu low memory support:icon menu.
    #MCFLAG_OPT		+= -DUI_LAYER_SUPPORT_LOW_MEMORY   #ui layer support 20 max layer blt.    
    MCFLAG_OPT		+= -DPB_NODE_CHANGE_GROUP_SUPPORT_LOW_MEMORY
	  MCFLAG_OPT		+= -DLIST_SLIDE_SUPPORT_LOW_MEMORY #list windows forbid slide to support low memory
	  
	  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
	    ###MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HISTORYLIST_SUPPORT   #��ʷ��¼
#	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT  #��ݴ���
  	    ###MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HELP_SUPPORT  #Help����
		endif
		    
    ### mmi slide support
 #   MCFLAG_OPT		+=  -DMMI_SLIDE_SUPPORT 

		ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
			#MCFLAG_OPT		+=  -DBROWSER_SEARCH_SUPPORT	
		  #MCFLAG_OPT		+=  -DBROWSER_BOOKMARK_HISTORY_TAB
		  #MCFLAG_OPT		+=  -DBROWSER_SUPPORT_LOW_MEMORY

			ifneq ($(strip $(BROWSER_OPERA_START_PAGE)), TRUE)
			ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
				ifneq ($(strip $(MMI_SLIDE_SUPPORT)), NONE)
					#startpage is slide style
					#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_THEME1
			  else
			  	#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			  endif		
			else
				MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			endif
			endif
					
			#MCFLAG_OPT		+=  -DMMIDORADO_MY_NAV_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_FIX_NAV_SUPPORT
			#MCFLAG_OPT		+=  -DMMIDORADO_FIX_HMTL_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_BGSOUND_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIBROWSER_DISPLAY_BK_HISTORY_INFO
		  #MCFLAG_OPT		+=  -DMMIDORADO_INPUT_HISTORY_SUPPORT
	  endif

	  
	  ifneq ($(strip $(FM_SUPPORT)), NONE)
	    ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
			  MCFLAG_OPT      += -DMMI_FM_TIMER_RECORD_SUPPORT 
			  MCFLAG_OPT      += -DMMIFM_SUPPORT_FM_RECORD
			endif 
		ifneq ($(strip $(ATV_SUPPORT)), $(strip $(FM_SUPPORT)))
				MCFLAG_OPT      += -DMMI_FM_NEED_HEADSET
		endif
		endif
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#��������GO�������Ч���Ŀ��غ�                               
			#MCFLAG_OPT	+= -DGO_BINARY_STAR_MAIN_MENU_SUPPORT    #Binary star
			
			#MCFLAG_OPT	+= -DGO_SHUTTER_MAIN_MENU_SUPPORT  #shutter
			
			#MCFLAG_OPT	+= -DGO_SPHERE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CYLINDER_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SLOPE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_WHEEL_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_OCEANWAVE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_PEGTOP_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_TURNTABLE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BOXROT_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_ICONBOXROT_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BOOKPAGE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_MIDROTATE_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SMALLITEMS_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CLASSIC_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_FLASH_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_CONSTICK_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SYMMETRY_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_BENZITEM_MAIN_MENU_SUPPORT		
		endif

		#�����˵����һ���л�goЧ����ICON
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#MCFLAG_OPT		+=  -DGO_MENU_ONE_ICON_CHANGE_STYLE
		endif

    ifeq ($(strip $(PDA_UI_SUPPORT_U_IDLE)), TRUE)
    	#MCFLAG_OPT += -DMMI_IDLE_WIDGET_CACHE
    endif
	  #֧�ֶ�����˵�����
	  #MCFLAG_OPT += -DMMI_MULTI_MAINMN       	
		#����Ⱦ�˵����
		#MCFLAG_OPT += -DPDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
		#�رճ���go�������������Ч��
		#MCFLAG_OPT		+= -DSLIDE_MAIN_MENU_SUPPORT       #slide style main menu support. 
		#MCFLAG_OPT		+= -DCUBE_MAIN_MENU_SUPPORT            #cube style main menu support. 
		#MCFLAG_OPT		+= -DTRAPEZOID_MAIN_MENU_SUPPORT       #trapezoid main menu support. 
		#MCFLAG_OPT		+= -DBINARY_STAR_MAIN_MENU_SUPPORT          #binary star style main menu support. 
		#MCFLAG_OPT		+= -DSHUTTER_MAIN_MENU_SUPPORT        #shutter main menu support.    
		#MCFLAG_OPT		+= -DCHARIOT_MAIN_MENU_SUPPORT        #chariot main menu support. 
		#MCFLAG_OPT		+= -DSPHERE_MAIN_MENU_SUPPORT          #sphere style main menu support. 
		#MCFLAG_OPT		+= -DCYLINDER_MAIN_MENU_SUPPORT        #cylinder main menu support. 

    #png image support high compress type.
    MCFLAG_OPT		+= -DMMI_RES_SLIM_SUPPORT

    #support app tool bar.
    #MCFLAG_OPT		+= -DGUI_CTRL_TOOLBAR_SUPPORT    

    #support app clipboard slider.
    #MCFLAG_OPT		+= -DCLIPBRD_SLIDER_SUPPORT  

    #support dynamic main menu.
    #MCFLAG_OPT		+= -DDYNAMIC_MAINMENU_SUPPORT 

    #support calendar slide effect.
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)    
        #MCFLAG_OPT		+= -DMMI_CALENDAR_SLIDE    
    endif  

    #support multi sim color support.
    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), SINGLE)
        #MCFLAG_OPT		+=  -DMMI_MULTISIM_COLOR_SUPPORT
    endif

    #support icon main menu style.
    ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
        MCFLAG_OPT		+=  -DMATRIX_MAIN_MENU_SUPPORT
    endif

#-------------------QCIF QQVGA MINI --------------------------# 
  MCFLAG_OPT       +=    -DMMI_SAVE_RUNING_NV_INFO
  MCFLAG_OPT       +=    -DMMI_SMS_MAX_NUM_SIM_40_ITEMS
# MCFLAG_OPT       +=    -DMMI_MMS_MAX_ONE_MMS_SIZE_MINI
  MCFLAG_OPT       +=    -DMMI_SMS_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_MMS_MINI_SUPPORT    
  MCFLAG_OPT	   +=    -DMMI_MMS_PREVIEW_LIMIT 

ifeq ($(strip $(MAINLCD_SIZE)),128X160)  
  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT
	MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT  
   MCFLAG_OPT       +=    -DMMI_DISPLAYSET_MINI_SUPPORT
   MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT  
   MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_ALARM_MINI_LOW_COST   
   MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT    
#ͼƬ�ļ�����ͼ��ȡ
# MCFLAG_OPT		    +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT             
#С�ڴ��¼���list����style   
   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT 
   MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT   
endif  

ifeq ($(strip $(MAINLCD_SIZE)),176X220)  
  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT
  MCFLAG_OPT      +=    -DMMI_DISPLAYSET_MINI_SUPPORT   
  MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT  
  MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
  MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_ALARM_MINI_LOW_COST   
  MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT 
#ͼƬ�ļ�����ͼ��ȡ
 MCFLAG_OPT		     +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT 
#С�ڴ��¼���list����style   
   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT 
   MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT         
endif  

ifeq ($(strip $(MAINLCD_SIZE)),240X320)  

#  MCFLAG_OPT       +=    -DMMI_IDLE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_CALCULATOR_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT
#  MCFLAG_OPT      +=    -DMMI_DISPLAYSET_MINI_SUPPORT   
#  MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT  
#  MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
#   MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
# MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_ALARM_MINI_LOW_COST   
  MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT 
#ͼƬ�ļ�����ͼ��ȡ
  MCFLAG_OPT		     +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT 
#С�ڴ��¼���list����style   
#   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT 
#   MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT         
endif  

  MCFLAG_OPT       +=    -DMMI_ENVSET_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_CHARGE_MINI_SUPPORT

  MCFLAG_OPT       +=    -DGUI_GRAPH_MINI_SUPPORT

  MCFLAG_OPT       +=    -DGUI_LIST_MINI_SUPPORT
  MCFLAG_OPT       +=    -DGUI_URL_MINI_SUPPORT
  MCFLAG_OPT       +=    -DGUI_STR_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_VIDEO_PLAYER_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_VIDEOPLAYER_MINI_FUNCTION
ifneq ($(strip $(KING_MOVIE_SUPPORT)),NONE)
  MCFLAG_OPT       +=    -DMMI_KINGMOVIE_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_KINGMOVIE_50_MOVIE_SUPPORT 
endif
  
  
	#MCFLAG_OPT       +=    -DMMI_AUDIOPLAYER_OPT_MINI_SUPPORT
 
    

  MCFLAG_OPT       +=    -DMMI_PB_MINI_SUPPORT          
  MCFLAG_OPT       +=    -DMMI_FMM_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_GLYPH_CACHE_MINI_SUPPORT
                                             
 
#------------------����ͳ�ƹ���----------------
    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), NONE)
	#MCFLAG_OPT += -DSALES_SUPPORT
    endif
#----------------------------------------------
#pbģ������ֻ���һ����¼�ڵ绰����ĸ���
  MCFLAG_OPT        +=    -DMMI_PB_MAX_NV_NUM_ONE
#pbģ���Ƿ�֧��mempool
  MCFLAG_OPT       +=    -DMMIPB_MEMPOOL_SUPPORT
#ATV�򿪴���ʱת��
  ###MCFLAG_OPT       +=    -DMMI_ATV_SWITCH_SCREEN_ON_OPEN
#�ļ��������ṩ�������ⲿģ�������ӿ�
  MCFLAG_OPT       +=    -DMMI_FMM_SORT_API_EXT_SUPPORT
#mp3 review ����
  MCFLAG_OPT         +=  -DMMI_AUDIOPLAYER_REVIEW_SUPPORT
#����pb photo�ü�, PB contact photo feild
  #MCFLAG_OPT         +=  -DMMI_CONTACT_PHOTO_SUPPORT
#����pb ring�ü�, PB contact ring feild
  #MCFLAG_OPT         +=  -DMMI_CONTACT_RING_SUPPORT
#pb�޸�������󳤶�
   MCFLAG_OPT      +=  -DMMI_PB_NAME_LEN_20_BYTES   
  
#-------------------QCIF QQVGA MINI --------------------------#      

#��ص����ٷֱ���status bar����ʾ�Ĺ��ܺ� ���ú���˫��ع��ܻ��⣬˫�������£�������״̬������ʾ�����ǰ��Ϸ�����ʾ
ifneq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
#MCFLAG_OPT       +=    -DMMI_BATTERY_PERCENT_IN_STATUSBAR
endif
#����������ȡ����sfs֧��xfind����xmove�Ĺ��ܣ���sfs.mk���ж��������ܵĿ���
#������32x64��32x16,16x16֮���size���棬���ԣ�����Ҳֻ��64x64��128x64�д�
		#MCFLAG_OPT		+=  -DMMIFMM_XFIND_SUPPORT
		#MCFLAG_OPT		+=  -DMMIFMM_XMOVE_SUPPORT
endif	
#-------------------------LOW_MEMORY_SUPPORT = 32X64----------------------------------------#  
ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X64)
    ifeq ($(strip $(MULTI_THEME_SUPPORT)), TRUE)
#    MCFLAG_OPT		+=  -DMMI_SAM_THEME
    endif 
    #ͨ����¼����֧���������
    #MCFLAG_OPT		+=  -DMMI_CL_NUM_MAX_SUPPORT
    #MCFLAG_OPT		+=  -DENG_SUPPORT
    #MCFLAG_OPT		+=  -DENG_MINI_SUPPORT
    #dorado������Ƿ���ʾtoolbar(��ʾtoolbarʱ,����ʾsoftkey);���������,�ÿ���Ĭ�ϴ�
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
    #MCFLAG_OPT		+=  -DBROWSER_TOOLBAR
    endif

    #״̬��������ʾ��Ϣ
    #MCFLAG_OPT		+=  -DMMI_STATUSBAR_SCROLLMSG_SUPPORT


    ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
	    MCFLAG_OPT		+=  -DMMI_GPRS_SUPPORT
    endif
    
    ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
        MCFLAG_OPT		+=  -DMMI_GPRS_SWITCH_SUPPORT
    endif
        
    ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
#            MCFLAG_OPT += -DPDA_TIME_ANIM_SUPPORT
    endif
    #����Ч��֧�֣���MP3ר��ͼƬ��
    #MCFLAG_OPT		+=  -DBRIGHT_EFFECT_SUPPORT
    #u200ͼƬ�༭��
    ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
    	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    		MCFLAG_OPT	+= -DPIC_EDIT_U200_SUPPORT
        endif
    endif
    
    #u200ͼƬ�����
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)    
    ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    MCFLAG_OPT  += -DPIC_PREVIEW_U200_SUPPORT
    #    PIC_QUICKVIEW_SUPPORT = TRUE
    endif
endif
    
    #MCFLAG_OPT	+= -DMMI_TURNON_BACKLIGHT_EFFECT_SUPPORT
    ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
        ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
    #����Ԥ����ʱ�䷭ҳ��Ч        
            #MCFLAG_OPT += -DPDA_WEATHER_TIME_SE
        endif
        MCFLAG_OPT += -DPDA_WEATHER_SUPPORT_LOW_MEMORY    
    endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
    MCFLAG_OPT += -DPDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)), TRUE)
#	MCFLAG_OPT		+= -DFEATURE_BIG_ENDIAN

#	MCFLAG_OPT		+= -DSXH_APPLE_APP
	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT
#	MCFLAG_OPT		+= -DSXH_APPLE_VP
#	MCFLAG_OPT		+= -DSXH_APPLE_RP
#	MCFLAG_OPT		+= -DSXH_APPLE_3GMUSIC
#	MCFLAG_OPT		+= -DSXH_APPLE_ONLINEGAME
#	MCFLAG_OPT		+= -DSXH_APPLE_VIDEODL
#	MCFLAG_OPT		+= -DSXH_APPLE_UMPLAY
#	MCFLAG_OPT		+= -DSXH_APPLE_BOOK
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT_DEMO
	

#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_APPMAN
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_VP
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_UMVCHAT
endif

#keylock effect start

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT    #�ᡢ�����Ǵ�����֧��

ifeq ($(strip $(LOW_MEMORY_KEYLOCK)), TRUE)
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_QQVGA
endif

ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
ifneq ($(strip $(MAINLCD_SIZE)),176X220)
#michael wang
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DC_SUPPORT_ROTATE
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
#michael wang
#����֧��
ifneq ($(strip $(MMI_RES_ORIENT)), LANDSCAPE)

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT
ifneq ($(strip $(MAINLCD_SIZE)),176X220)
ifneq ($(strip $(MAINLCD_SIZE)),128X160)
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
MCFLAG_OPT		+=  -DGUI_CTRL_SCROLL_KEY              #��Ҫ���������2ͬʱ��

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT

ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
endif

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE8_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_PSW_SUPPORT
endif
endif
ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE) 
ifeq ($(strip $(CAMERA_SUPPORT)), TRUE) 
MCFLAG_OPT		+=  -DKEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
endif
endif
endif	# end of LANDSCAPE
endif
endif # end of TOUCHPANEL_TYPE

#keylock effect end

    ##ǽֽ��̬�����ڴ�,��ʹ�þ�̬�ڴ�
    ifeq ($(strip $(SRAM_SIZE)), 32MBIT)
    MCFLAG_OPT	+=  -DMMI_DYNA_ALLOC_WALLPAPER_BUF
    endif

    #GBK support
    ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)), TRUE)
    ifneq ($(strip $(FONT_HAN_SIMP_ONLY)), TRUE)
    ifneq ($(strip $(BITMAP_FONT_SLIM_SUPPORT)), TRUE)
    MCFLAG_OPT		+=  -DGBK_SUPPORT
    endif
    endif
    endif

    MCFLAG_OPT		+=  -DBLUETOOTH_HFG_PLAY_MUSIC_SUPPORT        
	ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			MCFLAG_OPT		+=  -DMMI_IM_PDA_SUPPORT        ###use pda im, when exist touch panel
	endif

	ifeq ($(strip $(EBOOK_SUPPORT)), TRUE)
		#MCFLAG_OPT += -DMMIEBOOK_OSDMENU_SUPPORT		#OSD�����˵�
		ifeq ($(strip $(SE_PAGE_TURN_SUPPORT)), TRUE)
			#MCFLAG_OPT += -DMMIEBOOK_PAGESLIDE_SUPPORT	#��ҳ��Ч
		endif
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			#MCFLAG_OPT += -DMMIEBOOK_BOOKSHELF_SUPPORT	#�������
		endif
	endif
	
	
	ifeq ($(strip $(GAME_SUPPORT)), TRUE)
		#��Ϸ: ������ 
		#MCFLAG_OPT		+=  -DGAME_BOXMAN_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_LINK_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_QUINTET_SUPPORT	
	endif
	
	MMIAP_SUPPORT = TRUE
	MCFLAG_OPT		+=  -DMMI_AUDIO_PLAYER_SUPPORT

  ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
          MCFLAG_OPT	+=	-DMMI_RECORD_SUPPORT
	  MCFLAG_OPT	+=	-DMMI_AUTO_RECORD_SUPPORT
	endif
	
	ifeq ($(strip $(SCHEDULE_SUPPORT)), TRUE)
	    MCFLAG_OPT	+=	-DMMI_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_DAILY_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_WEEK_SCHEDULE_SUPPORT
	endif
	
  MCFLAG_OPT		+=  -DMMI_AUDIOPLAYER_ALBUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_SPECTRUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_MUSICMARK_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_SORT_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_LYRIC_SUPPORT
  MCFLAG_OPT         +=  -DMMIAP_MUSIC_LIST_SUPPORT
  #		#MP3
  
  MCFLAG_OPT += -DMMI_DIAL_SEARCHING_SUPPORT #�����̶�̬����ƥ�书��
  MCFLAG_OPT		+=  -DACCOUNT_AUTO_ADAPT_SUPPORT
		#�������ʻ��Զ�ƥ��
#	MCFLAG_OPT		+=  -DDATA_ACCOUNT_IN_TCARD_SUPPORT
    # define DATA_ACCOUNT_IN_TCARD_SUPPORT #�������˻����ݷ�T��
# MCFLAG_OPT		+=  -DCALL_TIMES_SUPPORT
		#ͨ����¼����
#	MCFLAG_OPT		+=  -DMMI_CALL_MINIMIZE_SUPPORT
		#ͨ����С��
#	MCFLAG_OPT += -DMMI_SPEEDDIAL_SUPPORT
			#��ݲ���
	MCFLAG_OPT += -DMMI_IP_CALL_SUPPORT	
	   #IP����
#	MCFLAG_OPT += -DMMI_MSD_SUPPORT
			#��������
	#MCFLAG_OPT += -DMMI_BLACKLIST_SUPPORT
			#������
  #MCFLAG_OPT		+=  -DCL_SAVE_NAME_SUPPORT
		#ͨ����¼�д洢������		
	MCFLAG_OPT += -DMMI_DIALPANEL_DTMF_SUPPORT
			#������֧��DTMF
	#MCFLAG_OPT += -DMMI_POWER_ON_PWD_SOS_SUPPORT
			#�����������֧��SOS
  #MCFLAG_OPT  += -DMMI_VCALENDAR_SUPPORT
      #֧���ճ̵��롢���������ͺͽ���	

## CPHS CSP(ALS) SUPPORT
ifeq ($(strip $(CPHS_SUPPORT)), TRUE)
	MCFLAG_OPT += -DMMI_CPHS_SUPPORT
	ifeq ($(strip $(CSP_SUPPORT)), TRUE)
		MCFLAG_OPT += -DMMI_CSP_SUPPORT
	endif
endif

ifneq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
#    MCFLAG_OPT +=  -DMMI_REPLACE_SMS_SUPPORT
    	#Replace SMS֧��
#    MCFLAG_OPT += -DMMI_STK_ICON_DISPLAY_SUPPORT
		  #STK ICON Display֧��
#    MCFLAG_OPT += -DMMI_SIM_LANGUAGE_SUPPORT
		  #SIM������֧��
		#MCFLAG_OPT += -DMMI_SMSCB_SUPPORT
		  #С���㲥
endif
		
	ifneq ($(strip $(ORANGE_SUPPORT)), TRUE)
		#R8
		MCFLAG_OPT += -DMMI_R8_SUPPORT
	endif
	
	#��������ģʽ
	ifeq ($(strip $(SMS_CHAT_SUPPORT)), TRUE)
   ifeq ($(strip $(MMS_SMS_IN_1_SUPPORT)), TRUE)
   	ifeq ($(strip $(CHATMODE_ONLY_SUPPORT)), TRUE)
			MCFLAG_OPT += -DMMI_SMS_CHAT_SUPPORT
		endif
	 endif
  endif
	
	
	#���ŵ�������
#	MCFLAG_OPT += -DMMI_SMS_EXPORT_SUPPORT
	
	#��ʱ����
#	MCFLAG_OPT += -DMMI_TIMERMSG_SUPPORT
	
	#�ܽӵ绰���Żظ�
#	MCFLAG_OPT += -DMMI_REJECT_CALL_BY_SMS_SUPPORT
	
	#������������
#	MCFLAG_OPT += -DMMI_SMS_FONT_SETTING_SUPPORT
	
	#��ȫ����
#	MCFLAG_OPT += -DMMI_SMS_SECURITYBOX_SUPPORT

    #ɾ��ȫ��
#   MCFLAG_OPT += -DMMI_SMS_DELALL_SUPPORT	
 		 
    #��������                             
   MCFLAG_OPT += -DMMI_SMS_VOICE_MAIL_SUPPORT	

    #���Ÿ��ƹ���                  
#   MCFLAG_OPT += -DMMI_SMS_COPY_SUPPORT	
        
    #�����ƶ�����                  
#   MCFLAG_OPT += -DMMI_SMS_MOVE_SUPPORT
 		 
#	MCFLAG_OPT    +=    -DMMI_VCARD_SUPPORT   
	
# idle & widget start
ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)	
ifneq ($(strip $(MAINLCD_SIZE)), 176X220)					
	#MCFLAG_OPT		+= -DGRID_THUMBVIEW_SUPPORT			 #grid idle ����ͼ����
	#MCFLAG_OPT		+= -DGRID_SLIDE_WALLPAPER				 #idle ����ǽֽ������low memoryȫ��
	
	ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
	     MCFLAG_OPT += -DMMI_WIDGET_WEATHER2			# 2 for PDA sytle; weather 1 for bar phone
       MCFLAG_OPT += -DMMIWIDGET_WEATHER_ANIM   # ����Ԥ����ҳ����
	  endif
	endif
	
	#MCFLAG_OPT		+=  -DSCH_ASSISTANT_WIDGET    	 # �ճ�����		
	#MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifneq ($(strip $(FM_SUPPORT)), NONE)	
  #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif    
  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Img Viewer
ifeq ($(strip $(COUNTEDTIME_SUPPORT)), TRUE)
  #MCFLAG_OPT		+= -DMMI_GRID_COUNTEDTIME			   #Grid Countedtime
endif
  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
  #MCFLAG_OPT		+= -DMMI_GRID_CONTACT			       #Grid Contact  ��ϵ��
  #MCFLAG_OPT    +=  -DMMI_WIDGET_TIME						 #����ҳЧ����ʱ��  

	
ifeq ($(strip $(WORLD_CLOCK_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_CLOCK   			#����˫ʱ��
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_ACLOCK    #ģ��˫ʱ��
endif
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_SET    	#���ٿ��� wifi bt ������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_MENU    #��ݷ�ʽ ����ʹ��
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ALARM
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ACLOCK   			 #ģ��ʱ��
endif
ifeq ($(strip $(ASP_SUPPORT)), TRUE)  
    #MCFLAG_OPT       +=    -DMMI_WIDGET_AUTOANSWER    #��¼�� ����ʹ��		#�Ѳ���ʹ��   
endif
ifeq ($(strip $(SEARCH_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SEARCH   			 #��������
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETSEARCH  		 #��������
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETWORK   		 #��������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_MEMO   				 #��ǩ 
endif     ### 176X220 
ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
	  MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
	  MCFLAG_OPT		+= -DBAR_PHONE_TOUCH_WIDGET			 #bar phone touch idle sytle	   
ifneq ($(strip $(FM_SUPPORT)), NONE)
    MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
	  MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar	
ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
	  MCFLAG_OPT 		+= -DMMI_WIDGET_WEATHER1				 # weather 	
endif		
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)  
	  MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Image Viewer	 
endif		   
endif     ### 176X220     
endif			### touch panel
  
  ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE) 				#bar phone Ӧ���Զ������漸��widget
	  #MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
	  MCFLAG_OPT 		+= -DMMI_WIDGET_WEATHER1				 # weather 	
endif	  
ifneq ($(strip $(FM_SUPPORT)), NONE)
    #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  #MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
	  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Image Viewer
	  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
	  #MCFLAG_OPT		+= -DMMI_WIDGET_SPEED_DIAL			 #Grid ���ٲ���	  
  endif
endif		# end of ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
# idle & widget end
	
	#3d rotate effect support.
	ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
	    MCFLAG_OPT		+= -DSET_3D_STYLE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_ROTATE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_LIST_ROLL_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_MAIN_MENU_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_WIN_SLIDE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_WIN_FADE_SUPPORT
	    #MCFLAG_OPT		+= -DDYNAMIC_WALLPAPER_SUPPORT    
	    #MCFLAG_OPT		+= -DEFFECT_FIRE_WALLPAPER_SUPPORT
	    ifneq ($(strip $(MOTION_SENSOR_TYPE)), NONE) 
	        MCFLAG_OPT		+= -DEFFECT_LCD_ROTATE_SUPPORT  #lcd rotate effect.
	    endif
	endif
	
	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
		ifeq ($(strip $(SE_ROLL_SUPPORT)), TRUE)
			#    MCFLAG_OPT		+= -DROLL_MAINMENU_SUPPORT
		endif
	endif
	
	ifeq ($(strip $(AMR_A2DP_SUPPORT)), TRUE)
    MCFLAG_OPT      += -DMMI_AMRBT_VPSUPPORT  			#VP��AMR��ʽ��BT֧��
	endif
  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
   	#MCFLAG_OPT		+= -DVIDEOTHUMBNAIL_SUPPORT      	#��Ƶ�ļ�����ͼ��ȡ
  endif

    #MCFLAG_OPT		+= -DTAKESCREENPHOTO_SUPPORT               #��Ļ����

		#MCFLAG_OPT		+= -DMENU_CRYSTALCUBE_SUPPORT
    #MCFLAG_OPT		+= -DMENU_CYCLONE_SUPPORT
    MCFLAG_OPT		+= -DRES_STORE_CACHE_SUPPORT           #resource store cache support.
    MCFLAG_OPT		+= -DRES_VECTOR_CACHE_SUPPORT          #resource vector cache support.
    #MCFLAG_OPT		+= -DPRINTSCREEN_SUPPORT               #��Ļץͼ 
    #MCFLAG_OPT		+= -DMMI_AUTOTEST_SUPPORT              #auto test support.
    #MCFLAG_OPT		+= -DMMI_AUTOTEST_WINIDNAME_SUPPORT              #auto test winidname support.
    
	  ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
        #MCFLAG_OPT += -DPIC_EDIT_FRAME_SUPPORT        	 #picture editor frame support.
    endif

    #MCFLAG_OPT      += -DCAMERA_FRAME_SUPPORT           #camera photo frame support.

    ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        MCFLAG_OPT		+= -DEFFECT_ZOOM_SUPPORT           #zoom effect support.
    endif
    
    #320x480�ڴ����ޣ����¹��ܲ�����		
    ifneq ($(strip $(MAINLCD_SIZE)), 320X480)
      #ʹ��ˢ�����ٹ���
      MCFLAG_OPT		+= -DMMI_FPS_ACCELERATE_SUPPORT
      #ʹ��pop������������
      ifeq ($(strip $(MMI_PUBWIN_ANIM_SUPPORT)), TRUE)     
        MCFLAG_OPT		+= -DMMI_MENUMASKLAYER_SUPPORT     #Menu Mask Layer
      endif
    endif
        
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
      ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        ifeq ($(strip $(SE_RIPPLE_SUPPORT)), TRUE)    
            MCFLAG_OPT		+= -DEFFECT_RIPPLE_SUPPORT     #ripple effect support.  
        endif
      endif
    endif
    
 #   MCFLAG_OPT       +=    -DMMI_RES_LOW_COST
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RESOURCE
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_EBOOK    #Ebook�ü�
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_PIC_EDIT #PIC edit�ü�MMIPIC_PASTE_IMAGE_NUM����
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RING     #�����ü�
        
    #MCFLAG_OPT       +=    -DMMI_SETTING_LOW_MEMORY     #�滻��MMI_SETTING_CUTDOWN_MAINMENU��MMI_SETTING_CUTDOWN_WALLPAPER
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_MAINMENU  #���˵���Ч�ü�
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_WALLPAPER #ǽֽ�ü�
    
    MCFLAG_OPT       +=    -DMMI_DIAL_LOW_MEMORY
    MCFLAG_OPT       +=    -DMMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
		#MCFLAG_OPT       +=    -DMMI_PICREVIEW_LIST_LOW_MEMORY
endif 
    MCFLAG_OPT		+= -DPB_SUPPORT_LOW_MEMORY             #PB low memory
    MCFLAG_OPT		+= -DMMIPB_MAX_COUNT_200               #PB support 200.
#    MCFLAG_OPT		+= -DMMIPB_MOST_USED_SUPPORT           #PB support MOST_USED_CONTACT.
#    MCFLAG_OPT		+= -DMMIPB_TOOLBAR_SUPPORT           #PB support toolbar.    
#    MCFLAG_OPT		+= -DMMIPB_SIMDN_SUPPORT           #PB support SIMDN.
    MCFLAG_OPT		+= -DMMIPB_ADD_EXIST_CONTACT_SUPPORT           #PB support ADD_EXIST_CONTACT_SUPPORT
#    MCFLAG_OPT		+= -DMMIPB_SEARCH_RESULT_COLOR_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
     MCFLAG_OPT		+= -DMMIPB_GROUP_MANAGE_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
#    MCFLAG_OPT		+= -DMMIPB_SELECT_DISPLAY_LOCATION_SUPPORT           #PB support SELECT_DISPLAY_LOCATION
#    MCFLAG_OPT		+= -DMMIPB_SELECT_STORE_LOCATION_SUPPORT           #PB support SELECT_STORE_LOCATION     
#    MCFLAG_OPT		+= -DMMIPB_IM_MESSAGE_SUPPORT           #PB im message feild  
    MCFLAG_OPT		+= -DSMS_SUPPORT_LOW_MEMORY            #sms support 200.
    MCFLAG_OPT		+= -DCALENDAR_SUPPORT_LOW_MEMORY       #calendar/schedule support 50.               
    MCFLAG_OPT		+= -DAUDIO_LOW_MEMORY_SUPPORT	       #audio buffer support low memory.
    MCFLAG_OPT		+= -DBT_SUPPORT_LOW_MEMORY             #bt max device number, 10.
    MCFLAG_OPT		+= -DALARM_SUPPORT_LOW_MEMORY          #alarm max number:5
    MCFLAG_OPT		+= -DBOOKMARK_SUPPORT_LOW_MEMORY       #bookmark max number:10
    MCFLAG_OPT		+= -DTTS_SUPPORT_LOW_MEMORY            #tts support low memory, reduce c
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_BUFFER    #streaming support low memory, reduce buffer
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_CONFLICT  #streaming support low memory, conflict      
    MCFLAG_OPT		+= -DRUNTIME_DRAW_SUPPORT
    
    MCFLAG_OPT		+= -DMAIN_MENU_SUPPORT_LOW_MEMORY  #main menu low memory support:icon menu.
    #MCFLAG_OPT		+= -DUI_LAYER_SUPPORT_LOW_MEMORY   #ui layer support 20 max layer blt.    
    MCFLAG_OPT		+= -DPB_NODE_CHANGE_GROUP_SUPPORT_LOW_MEMORY
	  MCFLAG_OPT		+= -DLIST_SLIDE_SUPPORT_LOW_MEMORY #list windows forbid slide to support low memory
	  
	  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HISTORYLIST_SUPPORT   #��ʷ��¼
#	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT  #��ݴ���
#  	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HELP_SUPPORT  #Help����
		endif
		    
    ### mmi slide support
 #   MCFLAG_OPT		+=  -DMMI_SLIDE_SUPPORT 

		ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
			#MCFLAG_OPT		+=  -DBROWSER_SEARCH_SUPPORT	
		  #MCFLAG_OPT		+=  -DBROWSER_BOOKMARK_HISTORY_TAB

			ifneq ($(strip $(BROWSER_OPERA_START_PAGE)), TRUE)
			ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
				ifneq ($(strip $(MMI_SLIDE_SUPPORT)), NONE)
					#startpage is slide style
					#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_THEME1
			  else
			  	MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			  endif		
			else
				MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			endif
			endif
					
			#MCFLAG_OPT		+=  -DMMIDORADO_MY_NAV_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_FIX_NAV_SUPPORT
			#MCFLAG_OPT		+=  -DMMIDORADO_FIX_HMTL_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_BGSOUND_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIBROWSER_DISPLAY_BK_HISTORY_INFO
		  #MCFLAG_OPT		+=  -DMMIDORADO_INPUT_HISTORY_SUPPORT
	  endif
	  
	  ifneq ($(strip $(FM_SUPPORT)), NONE)
	    ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
			  MCFLAG_OPT      += -DMMI_FM_TIMER_RECORD_SUPPORT  
			  MCFLAG_OPT      += -DMMIFM_SUPPORT_FM_RECORD
		  endif
		ifneq ($(strip $(ATV_SUPPORT)), $(strip $(FM_SUPPORT)))
				MCFLAG_OPT      += -DMMI_FM_NEED_HEADSET
		endif
		endif

		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#��������GO�������Ч���Ŀ��غ�                               
			MCFLAG_OPT	+= -DGO_BINARY_STAR_MAIN_MENU_SUPPORT    #Binary star
			
			MCFLAG_OPT	+= -DGO_SHUTTER_MAIN_MENU_SUPPORT  #shutter
			
			#MCFLAG_OPT	+= -DGO_SPHERE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CYLINDER_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_SLOPE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_WHEEL_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_OCEANWAVE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_PEGTOP_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_TURNTABLE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BOXROT_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_ICONBOXROT_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BOOKPAGE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_MIDROTATE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_SMALLITEMS_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CLASSIC_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_FLASH_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CONSTICK_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SYMMETRY_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BENZITEM_MAIN_MENU_SUPPORT		
		endif

		#�����˵����һ���л�goЧ����ICON
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			MCFLAG_OPT		+=  -DGO_MENU_ONE_ICON_CHANGE_STYLE
		endif

    ifeq ($(strip $(PDA_UI_SUPPORT_U_IDLE)), TRUE)
    	MCFLAG_OPT += -DMMI_IDLE_WIDGET_CACHE
    endif
	  #֧�ֶ�����˵�����
	  MCFLAG_OPT += -DMMI_MULTI_MAINMN       	
		#����Ⱦ�˵����
		#MCFLAG_OPT += -DPDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
		#�رճ���go�������������Ч��
		#MCFLAG_OPT		+= -DSLIDE_MAIN_MENU_SUPPORT       #slide style main menu support. 
		#MCFLAG_OPT		+= -DCUBE_MAIN_MENU_SUPPORT            #cube style main menu support. 
		#MCFLAG_OPT		+= -DTRAPEZOID_MAIN_MENU_SUPPORT       #trapezoid main menu support. 
		#MCFLAG_OPT		+= -DBINARY_STAR_MAIN_MENU_SUPPORT          #binary star style main menu support. 
		#MCFLAG_OPT		+= -DSHUTTER_MAIN_MENU_SUPPORT        #shutter main menu support.    
		#MCFLAG_OPT		+= -DCHARIOT_MAIN_MENU_SUPPORT        #chariot main menu support. 
		#MCFLAG_OPT		+= -DSPHERE_MAIN_MENU_SUPPORT          #sphere style main menu support. 
		#MCFLAG_OPT		+= -DCYLINDER_MAIN_MENU_SUPPORT        #cylinder main menu support. 

    #png image support high compress type.
    ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
        MCFLAG_OPT		+= -DMMI_RES_SLIM_SUPPORT
    endif

    #support app tool bar.
    #MCFLAG_OPT		+= -DGUI_CTRL_TOOLBAR_SUPPORT    

    #support app clipboard slider.
    #MCFLAG_OPT		+= -DCLIPBRD_SLIDER_SUPPORT  

    #support dynamic main menu.
    #MCFLAG_OPT		+= -DDYNAMIC_MAINMENU_SUPPORT 

    #support calendar slide effect.
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)    
        #MCFLAG_OPT		+= -DMMI_CALENDAR_SLIDE    
    endif  

    #support multi sim color support.
    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), SINGLE)
        #MCFLAG_OPT		+=  -DMMI_MULTISIM_COLOR_SUPPORT
    endif

    #support icon main menu style.
    ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
        MCFLAG_OPT		+=  -DMATRIX_MAIN_MENU_SUPPORT
    endif
    
#-------------------QCIF QQVGA MINI --------------------------#    

#  MCFLAG_OPT       +=    -DMMI_SMS_MAX_NUM_SIM_40_ITEMS
#  MCFLAG_OPT       +=    -DMMI_MMS_MAX_ONE_MMS_SIZE_MINI
#  MCFLAG_OPT       +=    -DMMI_SMS_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_MMS_MINI_SUPPORT     
#  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT
   MCFLAG_OPT	    +=    -DMMI_MMS_PREVIEW_LIMIT

   
#С�ڴ��¼���list����style   
#   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT    
#  MCFLAG_OPT       +=    -DMMI_ENVSET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_CHARGE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_DISPLAYSET_MINI_SUPPORT

#  MCFLAG_OPT       +=    -DGUI_GRAPH_MINI_SUPPORT

#  MCFLAG_OPT       +=    -DGUI_LIST_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_URL_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_STR_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_KINGMOVIE_MINI_SUPPORT   
ifeq ($(strip $(MAINLCD_SIZE)),176X220) 
   ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
       MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT
	 endif
#�����������֧��SOS   
	 MCFLAG_OPT += -DMMI_POWER_ON_PWD_SOS_SUPPORT  
endif
#  MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_PB_MINI_SUPPORT            
#  MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT               
     

      
      
#  MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_FMM_MINI_SUPPORT

#  MCFLAG_OPT       +=    -DMMI_GLYPH_CACHE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_AUDIOPLAYER_OPT_MINI_SUPPORT 
#------------------����ͳ�ƹ���----------------
	#MCFLAG_OPT += -DSALES_SUPPORT
#----------------------------------------------
#pbģ���Ƿ�֧��mempool
  MCFLAG_OPT       +=    -DMMIPB_MEMPOOL_SUPPORT
#ATV�򿪴���ʱת��
#  MCFLAG_OPT       +=    -DMMI_ATV_SWITCH_SCREEN_ON_OPEN
#�ļ��������ṩ�������ⲿģ�������ӿ�
  MCFLAG_OPT       +=    -DMMI_FMM_SORT_API_EXT_SUPPORT
  #ͼƬ�ļ�����ͼ��ȡ
 MCFLAG_OPT		   +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT 
#mp3 review ����
  MCFLAG_OPT         +=  -DMMI_AUDIOPLAYER_REVIEW_SUPPORT
#����pb photo�ü�, PB contact photo feild
  MCFLAG_OPT         +=  -DMMI_CONTACT_PHOTO_SUPPORT
#����pb ring�ü�, PB contact ring feild
  MCFLAG_OPT         +=  -DMMI_CONTACT_RING_SUPPORT
#pb�޸�������󳤶�
#   MCFLAG_OPT      +=  -DMMI_PB_NAME_LEN_20_BYTES 
#-------------------QCIF QQVGA MINI --------------------------#      

#��ص����ٷֱ���status bar����ʾ�Ĺ��ܺ� ���ú���˫��ع��ܻ��⣬˫�������£�������״̬������ʾ�����ǰ��Ϸ�����ʾ
ifneq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
#MCFLAG_OPT       +=    -DMMI_BATTERY_PERCENT_IN_STATUSBAR
endif    
#����������ȡ����sfs֧��xfind����xmove�Ĺ��ܣ���sfs.mk���ж��������ܵĿ���
#������32x64��32x16,16x16֮���size���棬���ԣ�����Ҳֻ��64x64��128x64�д�
		#MCFLAG_OPT		+=  -DMMIFMM_XFIND_SUPPORT
		#MCFLAG_OPT		+=  -DMMIFMM_XMOVE_SUPPORT
endif	
#-------------------------LOW_MEMORY_SUPPORT = 64X64----------------------------------------#  
ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 64X64)
ifneq ($(strip $(MAINLCD_SIZE)), 240X320)
    ifneq ($(strip $(KING_MOVIE_SUPPORT)), NONE)
        MCFLAG_OPT       +=    -DMMI_KINGMOVIE_MINI_SUPPORT     
    endif
    ifeq ($(strip $(MULTI_THEME_SUPPORT)), TRUE)
#    MCFLAG_OPT		+=  -DMMI_SAM_THEME
    endif 
    #ͨ����¼����֧���������
    #MCFLAG_OPT		+=  -DMMI_CL_NUM_MAX_SUPPORT
    #MCFLAG_OPT		+=  -DENG_MINI_SUPPORT
    #dorado������Ƿ���ʾtoolbar(��ʾtoolbarʱ,����ʾsoftkey);���������,�ÿ���Ĭ�ϴ�
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
    #MCFLAG_OPT		+=  -DBROWSER_TOOLBAR
    endif

    #״̬��������ʾ��Ϣ
    #MCFLAG_OPT		+=  -DMMI_STATUSBAR_SCROLLMSG_SUPPORT

    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), NONE)
        ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
	    MCFLAG_OPT		+=  -DMMI_GPRS_SUPPORT
            MCFLAG_OPT		+=  -DMMI_GPRS_SWITCH_SUPPORT
        endif
        MCFLAG_OPT		+=  -DENG_SUPPORT        
        MCFLAG_OPT		+=  -DSTK_SUPPORT        
    endif    

        
    ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
#            MCFLAG_OPT += -DPDA_TIME_ANIM_SUPPORT
    endif
    #����Ч��֧�֣���MP3ר��ͼƬ��
    #MCFLAG_OPT		+=  -DBRIGHT_EFFECT_SUPPORT
    #u200ͼƬ�༭��
    ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
    	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    		MCFLAG_OPT	+= -DPIC_EDIT_U200_SUPPORT
        endif
    endif
    
    #u200ͼƬ�����
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)    
    ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
    #    MCFLAG_OPT  += -DPIC_PREVIEW_U200_SUPPORT
    #    PIC_QUICKVIEW_SUPPORT = TRUE
    endif
endif
    
    #MCFLAG_OPT	+= -DMMI_TURNON_BACKLIGHT_EFFECT_SUPPORT
    ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
        ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
    #����Ԥ����ʱ�䷭ҳ��Ч        
            #MCFLAG_OPT += -DPDA_WEATHER_TIME_SE
        endif
        MCFLAG_OPT += -DPDA_WEATHER_SUPPORT_LOW_MEMORY    
    endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
    MCFLAG_OPT += -DPDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)), TRUE)
#	MCFLAG_OPT		+= -DFEATURE_BIG_ENDIAN

#	MCFLAG_OPT		+= -DSXH_APPLE_APP
	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT
#	MCFLAG_OPT		+= -DSXH_APPLE_VP
#	MCFLAG_OPT		+= -DSXH_APPLE_RP
#	MCFLAG_OPT		+= -DSXH_APPLE_3GMUSIC
#	MCFLAG_OPT		+= -DSXH_APPLE_ONLINEGAME
#	MCFLAG_OPT		+= -DSXH_APPLE_VIDEODL
#	MCFLAG_OPT		+= -DSXH_APPLE_UMPLAY
#	MCFLAG_OPT		+= -DSXH_APPLE_BOOK
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT_DEMO
	

#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_APPMAN
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_VP
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_UMVCHAT
endif

#keylock effect start

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT    #�ᡢ�����Ǵ�����֧��

ifeq ($(strip $(LOW_MEMORY_KEYLOCK)), TRUE)
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_QQVGA
endif

ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
ifneq ($(strip $(MAINLCD_SIZE)),176X220)
#michael wang
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DC_SUPPORT_ROTATE
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
#michael wang
#����֧��
ifneq ($(strip $(MMI_RES_ORIENT)), LANDSCAPE)

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT
ifneq ($(strip $(MAINLCD_SIZE)),176X220)
ifneq ($(strip $(MAINLCD_SIZE)),128X160)
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
MCFLAG_OPT		+=  -DGUI_CTRL_SCROLL_KEY              #��Ҫ���������2ͬʱ��

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT

ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
endif

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE8_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_PSW_SUPPORT
endif
endif
ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE) 
ifeq ($(strip $(CAMERA_SUPPORT)), TRUE) 
MCFLAG_OPT		+=  -DKEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
endif
endif
endif	# end of LANDSCAPE
endif
endif # end of TOUCHPANEL_TYPE

#keylock effect end

    ##ǽֽ��̬�����ڴ�,��ʹ�þ�̬�ڴ�
    ifeq ($(strip $(SRAM_SIZE)), 32MBIT)
    MCFLAG_OPT	+=  -DMMI_DYNA_ALLOC_WALLPAPER_BUF
    endif

    #GBK support
    ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)), TRUE)
    ifneq ($(strip $(FONT_HAN_SIMP_ONLY)), TRUE)
    ifneq ($(strip $(BITMAP_FONT_SLIM_SUPPORT)), TRUE)
    MCFLAG_OPT		+=  -DGBK_SUPPORT
    endif
    endif
    endif

    MCFLAG_OPT		+=  -DBLUETOOTH_HFG_PLAY_MUSIC_SUPPORT        
	ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			MCFLAG_OPT		+=  -DMMI_IM_PDA_SUPPORT        ###use pda im, when exist touch panel
	endif

	ifeq ($(strip $(EBOOK_SUPPORT)), TRUE)
		#MCFLAG_OPT += -DMMIEBOOK_OSDMENU_SUPPORT		#OSD�����˵�
		ifeq ($(strip $(SE_PAGE_TURN_SUPPORT)), TRUE)
			#MCFLAG_OPT += -DMMIEBOOK_PAGESLIDE_SUPPORT	#��ҳ��Ч
		endif
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			#MCFLAG_OPT += -DMMIEBOOK_BOOKSHELF_SUPPORT	#�������
		endif
	endif
	
	
	ifeq ($(strip $(GAME_SUPPORT)), TRUE)
		#��Ϸ: ������ 
		#MCFLAG_OPT		+=  -DGAME_BOXMAN_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_LINK_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_QUINTET_SUPPORT	
	endif
	
	MMIAP_SUPPORT = TRUE
	MCFLAG_OPT		+=  -DMMI_AUDIO_PLAYER_SUPPORT

  ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
          MCFLAG_OPT	+=	-DMMI_RECORD_SUPPORT
	  MCFLAG_OPT	+=	-DMMI_AUTO_RECORD_SUPPORT
	endif
	
	ifeq ($(strip $(SCHEDULE_SUPPORT)), TRUE)
	    MCFLAG_OPT	+=	-DMMI_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_DAILY_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_WEEK_SCHEDULE_SUPPORT
	endif
	
  MCFLAG_OPT		+=  -DMMI_AUDIOPLAYER_ALBUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_SPECTRUM_SUPPORT
  #MCFLAG_OPT		+=  -DMMIAP_MUSICMARK_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_SORT_SUPPORT
  #MCFLAG_OPT         +=  -DMMIAP_LYRIC_SUPPORT
  MCFLAG_OPT         +=  -DMMIAP_MUSIC_LIST_SUPPORT
  #		#MP3
  
  MCFLAG_OPT += -DMMI_DIAL_SEARCHING_SUPPORT #�����̶�̬����ƥ�书��
  MCFLAG_OPT		+=  -DACCOUNT_AUTO_ADAPT_SUPPORT
		#�������ʻ��Զ�ƥ��
#	MCFLAG_OPT		+=  -DDATA_ACCOUNT_IN_TCARD_SUPPORT
    # define DATA_ACCOUNT_IN_TCARD_SUPPORT #�������˻����ݷ�T��
	MCFLAG_OPT		+=  -DCALL_TIMES_SUPPORT
		#ͨ����¼����
#	MCFLAG_OPT		+=  -DMMI_CALL_MINIMIZE_SUPPORT
		#ͨ����С��
#	MCFLAG_OPT += -DMMI_SPEEDDIAL_SUPPORT
			#��ݲ���
	MCFLAG_OPT += -DMMI_IP_CALL_SUPPORT	
	   #IP����
#	MCFLAG_OPT += -DMMI_MSD_SUPPORT
			#��������
	MCFLAG_OPT += -DMMI_BLACKLIST_SUPPORT
			#������
  #MCFLAG_OPT		+=  -DCL_SAVE_NAME_SUPPORT
		#ͨ����¼�д洢������		
	MCFLAG_OPT += -DMMI_DIALPANEL_DTMF_SUPPORT
			#������֧��DTMF
	#MCFLAG_OPT += -DMMI_POWER_ON_PWD_SOS_SUPPORT
			#�����������֧��SOS
  #MCFLAG_OPT  += -DMMI_VCALENDAR_SUPPORT
      #֧���ճ̵��롢���������ͺͽ���	

## CPHS CSP(ALS) SUPPORT
ifeq ($(strip $(CPHS_SUPPORT)), TRUE)
	MCFLAG_OPT += -DMMI_CPHS_SUPPORT
	ifeq ($(strip $(CSP_SUPPORT)), TRUE)
		MCFLAG_OPT += -DMMI_CSP_SUPPORT
	endif
endif

ifneq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
#    MCFLAG_OPT +=  -DMMI_REPLACE_SMS_SUPPORT
    	#Replace SMS֧��
#    MCFLAG_OPT += -DMMI_STK_ICON_DISPLAY_SUPPORT
		  #STK ICON Display֧��
#    MCFLAG_OPT += -DMMI_SIM_LANGUAGE_SUPPORT
		  #SIM������֧��
		#MCFLAG_OPT += -DMMI_SMSCB_SUPPORT
		  #С���㲥
endif
	
	ifneq ($(strip $(ORANGE_SUPPORT)), TRUE)
		#R8
		MCFLAG_OPT += -DMMI_R8_SUPPORT
	endif
	
	#��������ģʽ
	ifeq ($(strip $(SMS_CHAT_SUPPORT)), TRUE)
   ifeq ($(strip $(MMS_SMS_IN_1_SUPPORT)), TRUE)
   	ifeq ($(strip $(CHATMODE_ONLY_SUPPORT)), TRUE)
			MCFLAG_OPT += -DMMI_SMS_CHAT_SUPPORT
		endif
	 endif
  endif
	
	
	#���ŵ�������
#	MCFLAG_OPT += -DMMI_SMS_EXPORT_SUPPORT
	
	#��ʱ����
#	MCFLAG_OPT += -DMMI_TIMERMSG_SUPPORT
	
	#�ܽӵ绰���Żظ�
	MCFLAG_OPT += -DMMI_REJECT_CALL_BY_SMS_SUPPORT
	
	#������������
#	MCFLAG_OPT += -DMMI_SMS_FONT_SETTING_SUPPORT
	
	#��ȫ����
#	MCFLAG_OPT += -DMMI_SMS_SECURITYBOX_SUPPORT

    #ɾ��ȫ��
#   MCFLAG_OPT += -DMMI_SMS_DELALL_SUPPORT	
 		 
    #��������                             
#   MCFLAG_OPT += -DMMI_SMS_VOICE_MAIL_SUPPORT	

    #���Ÿ��ƹ���                  
    MCFLAG_OPT += -DMMI_SMS_COPY_SUPPORT	
        
    #�����ƶ�����                  
    MCFLAG_OPT += -DMMI_SMS_MOVE_SUPPORT
 		 
	MCFLAG_OPT    +=    -DMMI_VCARD_SUPPORT   
	
# idle & widget start
ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)	
ifneq ($(strip $(MAINLCD_SIZE)), 176X220)					
	#MCFLAG_OPT		+= -DGRID_THUMBVIEW_SUPPORT			 #grid idle ����ͼ����
	#MCFLAG_OPT		+= -DGRID_SLIDE_WALLPAPER				 #idle ����ǽֽ������low memoryȫ��
	
	ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
	     MCFLAG_OPT += -DMMI_WIDGET_WEATHER2			# 2 for PDA sytle; weather 1 for bar phone
       MCFLAG_OPT += -DMMIWIDGET_WEATHER_ANIM   # ����Ԥ����ҳ����
	  endif
	endif
	
	#MCFLAG_OPT		+=  -DSCH_ASSISTANT_WIDGET    	 # �ճ�����		
	#MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifneq ($(strip $(FM_SUPPORT)), NONE)	
  #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif    
  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Img Viewer
ifeq ($(strip $(COUNTEDTIME_SUPPORT)), TRUE)
  #MCFLAG_OPT		+= -DMMI_GRID_COUNTEDTIME			   #Grid Countedtime
endif
  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
  #MCFLAG_OPT		+= -DMMI_GRID_CONTACT			       #Grid Contact  ��ϵ��
  #MCFLAG_OPT    +=  -DMMI_WIDGET_TIME						 #����ҳЧ����ʱ��  

	
ifeq ($(strip $(WORLD_CLOCK_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_CLOCK   			#����˫ʱ��
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_ACLOCK    #ģ��˫ʱ��
endif
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_SET    	#���ٿ��� wifi bt ������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_MENU    #��ݷ�ʽ ����ʹ��
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ALARM
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_ACLOCK   			 #ģ��ʱ��
endif
ifeq ($(strip $(ASP_SUPPORT)), TRUE)  
    #MCFLAG_OPT       +=    -DMMI_WIDGET_AUTOANSWER    #��¼�� ����ʹ��		#�Ѳ���ʹ��   
endif
ifeq ($(strip $(SEARCH_SUPPORT)), TRUE)
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SEARCH   			 #��������
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETSEARCH  		 #��������
endif    
    #MCFLAG_OPT       +=    -DMMI_WIDGET_NETWORK   		 #��������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_MEMO   				 #��ǩ 
endif     ### 176X220 
ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
	  MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
	  MCFLAG_OPT		+= -DBAR_PHONE_TOUCH_WIDGET			 #bar phone touch idle sytle	   
ifneq ($(strip $(FM_SUPPORT)), NONE)
    MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
	  MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar	
ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
	  MCFLAG_OPT 		+= -DMMI_WIDGET_WEATHER1				 # weather 	
endif		
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)  
	  MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Image Viewer	 
endif		   
endif     ### 176X220     
endif			### touch panel
  
  ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE) 				#bar phone Ӧ���Զ������漸��widget
	  #MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
	  MCFLAG_OPT 		+= -DMMI_WIDGET_WEATHER1				 # weather 	
endif	  
ifneq ($(strip $(FM_SUPPORT)), NONE)
    #MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  #MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
	  #MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Image Viewer
	  #MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
	  #MCFLAG_OPT		+= -DMMI_WIDGET_SPEED_DIAL			 #Grid ���ٲ���	  
  endif
endif		# end of ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
# idle & widget end
	
	#3d rotate effect support.
	ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
	    MCFLAG_OPT		+= -DSET_3D_STYLE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_ROTATE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_LIST_ROLL_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_MAIN_MENU_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_WIN_SLIDE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_WIN_FADE_SUPPORT
	    #MCFLAG_OPT		+= -DDYNAMIC_WALLPAPER_SUPPORT    
	    #MCFLAG_OPT		+= -DEFFECT_FIRE_WALLPAPER_SUPPORT
	    ifneq ($(strip $(MOTION_SENSOR_TYPE)), NONE) 
	        MCFLAG_OPT		+= -DEFFECT_LCD_ROTATE_SUPPORT  #lcd rotate effect.
	    endif
	endif
	
	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
		ifeq ($(strip $(SE_ROLL_SUPPORT)), TRUE)
			#    MCFLAG_OPT		+= -DROLL_MAINMENU_SUPPORT
		endif
	endif
	
	ifeq ($(strip $(AMR_A2DP_SUPPORT)), TRUE)
    MCFLAG_OPT      += -DMMI_AMRBT_VPSUPPORT  			#VP��AMR��ʽ��BT֧��
	endif
  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
   	MCFLAG_OPT		+= -DVIDEOTHUMBNAIL_SUPPORT      	#��Ƶ�ļ�����ͼ��ȡ
  endif

    #MCFLAG_OPT		+= -DTAKESCREENPHOTO_SUPPORT               #��Ļ����

		#MCFLAG_OPT		+= -DMENU_CRYSTALCUBE_SUPPORT
    #MCFLAG_OPT		+= -DMENU_CYCLONE_SUPPORT
    MCFLAG_OPT		+= -DRES_STORE_CACHE_SUPPORT           #resource store cache support.
    MCFLAG_OPT		+= -DRES_VECTOR_CACHE_SUPPORT          #resource vector cache support.
    #MCFLAG_OPT		+= -DPRINTSCREEN_SUPPORT               #��Ļץͼ 
    #MCFLAG_OPT		+= -DMMI_AUTOTEST_SUPPORT              #auto test support.
    #MCFLAG_OPT		+= -DMMI_AUTOTEST_WINIDNAME_SUPPORT              #auto test winidname support.
    
	  ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
        #MCFLAG_OPT += -DPIC_EDIT_FRAME_SUPPORT        	 #picture editor frame support.
    endif

    #MCFLAG_OPT      += -DCAMERA_FRAME_SUPPORT           #camera photo frame support.

    ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        MCFLAG_OPT		+= -DEFFECT_ZOOM_SUPPORT           #zoom effect support.
    endif
    
    #320x480�ڴ����ޣ����¹��ܲ�����		
    ifneq ($(strip $(MAINLCD_SIZE)), 320X480)
      #ʹ��ˢ�����ٹ���
      MCFLAG_OPT		+= -DMMI_FPS_ACCELERATE_SUPPORT
      #ʹ��pop������������
      ifeq ($(strip $(MMI_PUBWIN_ANIM_SUPPORT)), TRUE)     
        MCFLAG_OPT		+= -DMMI_MENUMASKLAYER_SUPPORT     #Menu Mask Layer
      endif
    endif
        
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
      ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        ifeq ($(strip $(SE_RIPPLE_SUPPORT)), TRUE)    
            MCFLAG_OPT		+= -DEFFECT_RIPPLE_SUPPORT     #ripple effect support.  
        endif
      endif
    endif
    
 #   MCFLAG_OPT       +=    -DMMI_RES_LOW_COST
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RESOURCE
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_EBOOK    #Ebook�ü�
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_PIC_EDIT #PIC edit�ü�MMIPIC_PASTE_IMAGE_NUM����
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RING     #�����ü�
        
    #MCFLAG_OPT       +=    -DMMI_SETTING_LOW_MEMORY     #�滻��MMI_SETTING_CUTDOWN_MAINMENU��MMI_SETTING_CUTDOWN_WALLPAPER
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_MAINMENU  #���˵���Ч�ü�
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_WALLPAPER #ǽֽ�ü�
    
    MCFLAG_OPT       +=    -DMMI_DIAL_LOW_MEMORY
    MCFLAG_OPT       +=    -DMMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
		#MCFLAG_OPT       +=    -DMMI_PICREVIEW_LIST_LOW_MEMORY
endif 
    MCFLAG_OPT		+= -DPB_SUPPORT_LOW_MEMORY             #PB low memory
    MCFLAG_OPT		+= -DMMIPB_MAX_COUNT_200               #PB support 200.
#    MCFLAG_OPT		+= -DMMIPB_MOST_USED_SUPPORT           #PB support MOST_USED_CONTACT.
#    MCFLAG_OPT		+= -DMMIPB_TOOLBAR_SUPPORT           #PB support toolbar.    
#    MCFLAG_OPT		+= -DMMIPB_SIMDN_SUPPORT           #PB support SIMDN.
    MCFLAG_OPT		+= -DMMIPB_ADD_EXIST_CONTACT_SUPPORT           #PB support ADD_EXIST_CONTACT_SUPPORT
#    MCFLAG_OPT		+= -DMMIPB_SEARCH_RESULT_COLOR_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
     MCFLAG_OPT		+= -DMMIPB_GROUP_MANAGE_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
#    MCFLAG_OPT		+= -DMMIPB_SELECT_DISPLAY_LOCATION_SUPPORT           #PB support SELECT_DISPLAY_LOCATION
#    MCFLAG_OPT		+= -DMMIPB_SELECT_STORE_LOCATION_SUPPORT           #PB support SELECT_STORE_LOCATION     
#    MCFLAG_OPT		+= -DMMIPB_IM_MESSAGE_SUPPORT           #PB im message feild  
    MCFLAG_OPT		+= -DSMS_SUPPORT_LOW_MEMORY            #sms support 200.
    MCFLAG_OPT		+= -DCALENDAR_SUPPORT_LOW_MEMORY       #calendar/schedule support 50.               
    MCFLAG_OPT		+= -DAUDIO_LOW_MEMORY_SUPPORT	       #audio buffer support low memory.
    MCFLAG_OPT		+= -DBT_SUPPORT_LOW_MEMORY             #bt max device number, 10.
    MCFLAG_OPT		+= -DALARM_SUPPORT_LOW_MEMORY          #alarm max number:5
    MCFLAG_OPT		+= -DBOOKMARK_SUPPORT_LOW_MEMORY       #bookmark max number:10
    MCFLAG_OPT		+= -DTTS_SUPPORT_LOW_MEMORY            #tts support low memory, reduce c
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_BUFFER    #streaming support low memory, reduce buffer
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_CONFLICT  #streaming support low memory, conflict      
    MCFLAG_OPT		+= -DRUNTIME_DRAW_SUPPORT
    
    MCFLAG_OPT		+= -DMAIN_MENU_SUPPORT_LOW_MEMORY  #main menu low memory support:icon menu.
    #MCFLAG_OPT		+= -DUI_LAYER_SUPPORT_LOW_MEMORY   #ui layer support 20 max layer blt.    
    MCFLAG_OPT		+= -DPB_NODE_CHANGE_GROUP_SUPPORT_LOW_MEMORY
	  MCFLAG_OPT		+= -DLIST_SLIDE_SUPPORT_LOW_MEMORY #list windows forbid slide to support low memory
	  
	  ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
	    MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HISTORYLIST_SUPPORT   #��ʷ��¼
      MCFLAG_OPT    += -DMMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT  #��ݴ���
  	  MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HELP_SUPPORT  #Help����
		endif
		    
    ### mmi slide support
 #   MCFLAG_OPT		+=  -DMMI_SLIDE_SUPPORT 

		ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
			#MCFLAG_OPT		+=  -DBROWSER_SEARCH_SUPPORT	
		  #MCFLAG_OPT		+=  -DBROWSER_BOOKMARK_HISTORY_TAB

			ifneq ($(strip $(BROWSER_OPERA_START_PAGE)), TRUE)
			ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
				ifneq ($(strip $(MMI_SLIDE_SUPPORT)), NONE)
					#startpage is slide style
					#MCFLAG_OPT		+=  -DBROWSER_START_PAGE_THEME1
			  else
			  	MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			  endif		
			else
				MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			endif
			endif
					
			#MCFLAG_OPT		+=  -DMMIDORADO_MY_NAV_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_FIX_NAV_SUPPORT
			#MCFLAG_OPT		+=  -DMMIDORADO_FIX_HMTL_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIDORADO_BGSOUND_SUPPORT
		  #MCFLAG_OPT		+=  -DMMIBROWSER_DISPLAY_BK_HISTORY_INFO
		  #MCFLAG_OPT		+=  -DMMIDORADO_INPUT_HISTORY_SUPPORT
	  endif
	  
	  ifneq ($(strip $(FM_SUPPORT)), NONE)
	    ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
			  MCFLAG_OPT      += -DMMI_FM_TIMER_RECORD_SUPPORT  
			  MCFLAG_OPT      += -DMMIFM_SUPPORT_FM_RECORD
		  endif
		ifneq ($(strip $(ATV_SUPPORT)), $(strip $(FM_SUPPORT)))
				MCFLAG_OPT      += -DMMI_FM_NEED_HEADSET
		endif
		endif

		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#��������GO�������Ч���Ŀ��غ�                               
			MCFLAG_OPT	+= -DGO_BINARY_STAR_MAIN_MENU_SUPPORT    #Binary star
			
			MCFLAG_OPT	+= -DGO_SHUTTER_MAIN_MENU_SUPPORT  #shutter
			
			#MCFLAG_OPT	+= -DGO_SPHERE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CYLINDER_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_SLOPE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_WHEEL_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_OCEANWAVE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_PEGTOP_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_TURNTABLE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BOXROT_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_ICONBOXROT_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BOOKPAGE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_MIDROTATE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_SMALLITEMS_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CLASSIC_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_FLASH_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CONSTICK_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SYMMETRY_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BENZITEM_MAIN_MENU_SUPPORT		
		endif

		#�����˵����һ���л�goЧ����ICON
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			MCFLAG_OPT		+=  -DGO_MENU_ONE_ICON_CHANGE_STYLE
		endif

    ifeq ($(strip $(PDA_UI_SUPPORT_U_IDLE)), TRUE)
    	MCFLAG_OPT += -DMMI_IDLE_WIDGET_CACHE
    endif
	  #֧�ֶ�����˵�����
	  MCFLAG_OPT += -DMMI_MULTI_MAINMN       	
		#����Ⱦ�˵����
		#MCFLAG_OPT += -DPDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
		#�رճ���go�������������Ч��
		#MCFLAG_OPT		+= -DSLIDE_MAIN_MENU_SUPPORT       #slide style main menu support. 
		#MCFLAG_OPT		+= -DCUBE_MAIN_MENU_SUPPORT            #cube style main menu support. 
		#MCFLAG_OPT		+= -DTRAPEZOID_MAIN_MENU_SUPPORT       #trapezoid main menu support. 
		#MCFLAG_OPT		+= -DBINARY_STAR_MAIN_MENU_SUPPORT          #binary star style main menu support. 
		#MCFLAG_OPT		+= -DSHUTTER_MAIN_MENU_SUPPORT        #shutter main menu support.    
		#MCFLAG_OPT		+= -DCHARIOT_MAIN_MENU_SUPPORT        #chariot main menu support. 
		#MCFLAG_OPT		+= -DSPHERE_MAIN_MENU_SUPPORT          #sphere style main menu support. 
		#MCFLAG_OPT		+= -DCYLINDER_MAIN_MENU_SUPPORT        #cylinder main menu support. 

    #png image support high compress type.
    ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
        MCFLAG_OPT		+= -DMMI_RES_SLIM_SUPPORT
    endif

    #support app tool bar.
    #MCFLAG_OPT		+= -DGUI_CTRL_TOOLBAR_SUPPORT    

    #support app clipboard slider.
    #MCFLAG_OPT		+= -DCLIPBRD_SLIDER_SUPPORT  

    #support dynamic main menu.
    #MCFLAG_OPT		+= -DDYNAMIC_MAINMENU_SUPPORT 

    #support calendar slide effect.
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)    
        #MCFLAG_OPT		+= -DMMI_CALENDAR_SLIDE    
    endif  

    #support multi sim color support.
    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), SINGLE)
        #MCFLAG_OPT		+=  -DMMI_MULTISIM_COLOR_SUPPORT
    endif

    #support icon main menu style.
    ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
        MCFLAG_OPT		+=  -DMATRIX_MAIN_MENU_SUPPORT
    endif
    
#-------------------QCIF QQVGA MINI --------------------------#    

#  MCFLAG_OPT       +=    -DMMI_SMS_MAX_NUM_SIM_40_ITEMS
#  MCFLAG_OPT       +=    -DMMI_MMS_MAX_ONE_MMS_SIZE_MINI
#  MCFLAG_OPT       +=    -DMMI_SMS_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_MMS_MINI_SUPPORT     
#  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT

   
#С�ڴ��¼���list����style   
#   MCFLAG_OPT       +=    -DMMI_THEME_LIST_MINI_SUPPORT    
#  MCFLAG_OPT       +=    -DMMI_ENVSET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_CHARGE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_DISPLAYSET_MINI_SUPPORT

#  MCFLAG_OPT       +=    -DGUI_GRAPH_MINI_SUPPORT

#  MCFLAG_OPT       +=    -DGUI_LIST_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_URL_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_STR_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_KINGMOVIE_MINI_SUPPORT   
ifeq ($(strip $(MAINLCD_SIZE)),176X220) 
   ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), FALSE)
       MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT
	 endif
#�����������֧��SOS   
	 MCFLAG_OPT += -DMMI_POWER_ON_PWD_SOS_SUPPORT 
	 MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT 
endif
ifeq ($(strip $(MAINLCD_SIZE)),128X160)
		MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT 
endif
#  MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_PB_MINI_SUPPORT                  
#  MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT               
     

      
      
#  MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_FMM_MINI_SUPPORT

#  MCFLAG_OPT       +=    -DMMI_GLYPH_CACHE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_AUDIOPLAYER_OPT_MINI_SUPPORT 
#------------------����ͳ�ƹ���----------------
ifneq ($(strip $(MMI_MULTI_SIM_SYS)), NONE)
    #MCFLAG_OPT += -DSALES_SUPPORT
endif
#----------------------------------------------
#pbģ������ֻ���һ����¼�ڵ绰����ĸ���
  #MCFLAG_OPT        +=    -DMMI_PB_MAX_NV_NUM_ONE
#pbģ���Ƿ�֧��mempool
  MCFLAG_OPT       +=    -DMMIPB_MEMPOOL_SUPPORT
#ATV�򿪴���ʱת��
#  MCFLAG_OPT       +=    -DMMI_ATV_SWITCH_SCREEN_ON_OPEN
#�ļ��������ṩ�������ⲿģ�������ӿ�
  MCFLAG_OPT       +=    -DMMI_FMM_SORT_API_EXT_SUPPORT
#ͼƬ�ļ�����ͼ��ȡ
  MCFLAG_OPT		   +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT 
#mp3 review ����
  MCFLAG_OPT         +=  -DMMI_AUDIOPLAYER_REVIEW_SUPPORT
#����pb photo�ü�, PB contact photo feild
  MCFLAG_OPT         +=  -DMMI_CONTACT_PHOTO_SUPPORT
#����pb ring�ü�, PB contact ring feild
  MCFLAG_OPT         +=  -DMMI_CONTACT_RING_SUPPORT
#pb�޸�������󳤶�
#   MCFLAG_OPT      +=  -DMMI_PB_NAME_LEN_20_BYTES 
#-------------------QCIF QQVGA MINI --------------------------#      

#��ص����ٷֱ���status bar����ʾ�Ĺ��ܺ� ���ú���˫��ع��ܻ��⣬˫�������£�������״̬������ʾ�����ǰ��Ϸ�����ʾ
ifneq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
#MCFLAG_OPT       +=    -DMMI_BATTERY_PERCENT_IN_STATUSBAR
endif    
#����������ȡ����sfs֧��xfind����xmove�Ĺ��ܣ���sfs.mk���ж��������ܵĿ���
#������32x64��32x16,16x16֮���size���棬���ԣ�����Ҳֻ��64x64��128x64�д�
	MCFLAG_OPT		+=  -DMMIFMM_XFIND_SUPPORT
	MCFLAG_OPT		+=  -DMMIFMM_XMOVE_SUPPORT
endif
ifeq ($(strip $(MAINLCD_SIZE)), 240X320)
     #LDN SUPPORT
     MCFLAG_OPT += -DMMI_LDN_SUPPORT
     
    ifeq ($(strip $(MULTI_THEME_SUPPORT)), TRUE)
#    MCFLAG_OPT		+=  -DMMI_SAM_THEME
    endif
		ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
		MCFLAG_OPT		+=  -DMMI_VIDEOWALLPAPER_SUPPORT
		endif	
		
    #ͨ����¼����֧���������
    MCFLAG_OPT		+=  -DMMI_CL_NUM_MAX_SUPPORT
ifneq ($(strip $(MMI_MULTI_SIM_SYS)), NONE)
    #MCFLAG_OPT		+=  -DENG_SUPPORT
    MCFLAG_OPT		+=  -DSTK_SUPPORT
    #MCFLAG_OPT		+=  -DENG_MINI_SUPPORT
    ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
        MCFLAG_OPT		+=  -DMMI_GPRS_SUPPORT
        MCFLAG_OPT		+=  -DMMI_GPRS_SWITCH_SUPPORT
    endif
endif
    #dorado������Ƿ���ʾtoolbar(��ʾtoolbarʱ,����ʾsoftkey);���������,�ÿ���Ĭ�ϴ�
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
    MCFLAG_OPT		+=  -DBROWSER_TOOLBAR
    endif

    #״̬��������ʾ��Ϣ
    MCFLAG_OPT		+=  -DMMI_STATUSBAR_SCROLLMSG_SUPPORT

    

    
    ifeq ($(strip $(PDA_UI_SUPPORT_U_IDLE)), TRUE)
    MCFLAG_OPT += -DMMI_IDLE_WIDGET_CACHE
    endif
    
    #����Ч��֧�֣���MP3ר��ͼƬ��
    #MCFLAG_OPT		+=  -DBRIGHT_EFFECT_SUPPORT
    
    ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
#            MCFLAG_OPT += -DPDA_TIME_ANIM_SUPPORT
    endif

    #u200ͼƬ�༭��
    ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
    	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
        		MCFLAG_OPT	+= -DPIC_EDIT_U200_SUPPORT
        endif
    endif
    
    #u200ͼƬ�����
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
    ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
        MCFLAG_OPT  += -DPIC_PREVIEW_U200_SUPPORT
    #    PIC_QUICKVIEW_SUPPORT = TRUE
    endif
endif

    #MCFLAG_OPT	+= -DMMI_TURNON_BACKLIGHT_EFFECT_SUPPORT
    ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
        MCFLAG_OPT += -DPDA_WEATHER_SUPPORT_LOW_MEMORY    
    endif

ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
        MCFLAG_OPT += -DPDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)), TRUE)
#	MCFLAG_OPT		+= -DFEATURE_BIG_ENDIAN

#	MCFLAG_OPT		+= -DSXH_APPLE_APP
	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT
#	MCFLAG_OPT		+= -DSXH_APPLE_VP
#	MCFLAG_OPT		+= -DSXH_APPLE_RP
#	MCFLAG_OPT		+= -DSXH_APPLE_3GMUSIC
#	MCFLAG_OPT		+= -DSXH_APPLE_ONLINEGAME
#	MCFLAG_OPT		+= -DSXH_APPLE_VIDEODL
#	MCFLAG_OPT		+= -DSXH_APPLE_UMPLAY
#	MCFLAG_OPT		+= -DSXH_APPLE_BOOK
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT_DEMO
	

#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_APPMAN
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_VP
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_UMVCHAT
endif

    ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
        MCFLAG_OPT	+=	-DMMI_RECORD_SUPPORT
        MCFLAG_OPT	+=	-DMMI_AUTO_RECORD_SUPPORT
    endif
    
  ifeq ($(strip $(SCHEDULE_SUPPORT)), TRUE)
	    MCFLAG_OPT	+=	-DMMI_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_DAILY_SCHEDULE_SUPPORT
	    MCFLAG_OPT	+=	-DMMI_WEEK_SCHEDULE_SUPPORT
	endif

#keylock effect start

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_SUPPORT

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT    #�ᡢ�����Ǵ�����֧��

ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
#michael wang
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_DC_SUPPORT_ROTATE
ifeq ($(strip $(MAINLCD_SIZE)), 320X480)
MCFLAG_OPT		+=  -DMMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
endif
#michael wang

#����֧��
ifneq ($(strip $(MMI_RES_ORIENT)), LANDSCAPE)

MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
#MCFLAG_OPT		+=  -DGUI_CTRL_SCROLL_KEY              #��Ҫ���������2ͬʱ��

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT

#ifeq ($(strip $(MMI_3DUI_SUPPORT)), TRUE)
#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
#endif

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_TYPE8_SUPPORT

#MCFLAG_OPT		+=  -DMMI_KEY_LOCK_EFFECT_PSW_SUPPORT

#ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE) 
#ifeq ($(strip $(CAMERA_SUPPORT)), TRUE) 
#MCFLAG_OPT		+=  -DKEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
#endif
#endif
endif	# end of LANDSCAPE
endif # end of TOUCHPANEL_TYPE
#keylock effect end

    ##ǽֽ��̬�����ڴ�,��ʹ�þ�̬�ڴ�
    ifeq ($(strip $(SRAM_SIZE)), 32MBIT)
    MCFLAG_OPT	+=  -DMMI_DYNA_ALLOC_WALLPAPER_BUF
    endif

    #GBK support
    ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)), TRUE)
    ifneq ($(strip $(FONT_HAN_SIMP_ONLY)), TRUE)
    ifneq ($(strip $(BITMAP_FONT_SLIM_SUPPORT)), TRUE)
    MCFLAG_OPT		+=  -DGBK_SUPPORT
    endif
    endif
    endif

    MCFLAG_OPT		+=  -DBLUETOOTH_HFG_PLAY_MUSIC_SUPPORT        
	ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
		MCFLAG_OPT		+=  -DMMI_IM_PDA_SUPPORT        ###use pda im, when exist touch panel
	endif

	ifeq ($(strip $(EBOOK_SUPPORT)), TRUE)
		MCFLAG_OPT += -DMMIEBOOK_OSDMENU_SUPPORT		#OSD�����˵�
		ifeq ($(strip $(SE_PAGE_TURN_SUPPORT)), TRUE)
			MCFLAG_OPT += -DMMIEBOOK_PAGESLIDE_SUPPORT	#��ҳ��Ч
		endif
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
			MCFLAG_OPT += -DMMIEBOOK_BOOKSHELF_SUPPORT	#�������
		endif
	endif
	
	
	ifeq ($(strip $(GAME_SUPPORT)), TRUE)
		#��Ϸ: ������ 
		MCFLAG_OPT		+=  -DGAME_BOXMAN_SUPPORT
		#��Ϸ: ������
		MCFLAG_OPT		+=  -DGAME_LINK_SUPPORT
		#��Ϸ: ������
		#MCFLAG_OPT		+=  -DGAME_QUINTET_SUPPORT															
	endif
	
	MMIAP_SUPPORT = TRUE
	MCFLAG_OPT		+=  -DMMI_AUDIO_PLAYER_SUPPORT
  MCFLAG_OPT		+=  -DMMI_AUDIOPLAYER_ALBUM_SUPPORT
  MCFLAG_OPT		+=  -DMMIAP_SPECTRUM_SUPPORT
  MCFLAG_OPT		+=  -DMMIAP_MUSICMARK_SUPPORT
  MCFLAG_OPT         +=  -DMMIAP_SORT_SUPPORT
  MCFLAG_OPT         +=  -DMMIAP_LYRIC_SUPPORT
  MCFLAG_OPT         +=  -DMMIAP_MUSIC_LIST_SUPPORT
  #		#MP3
  
  MCFLAG_OPT += -DMMI_DIAL_SEARCHING_SUPPORT #�����̶�̬����ƥ�书��
  MCFLAG_OPT		+=  -DACCOUNT_AUTO_ADAPT_SUPPORT
		#�������ʻ��Զ�ƥ��
	MCFLAG_OPT		+=  -DDATA_ACCOUNT_IN_TCARD_SUPPORT
	MCFLAG_OPT		+=  -DACCOUNT_AUTO_ADAPT_FULLNTAC_SUPPORT  #APNȫ����ʱ�汾
    # define DATA_ACCOUNT_IN_TCARD_SUPPORT #�������˻����ݷ�T��
	MCFLAG_OPT		+=  -DCALL_TIMES_SUPPORT
		#ͨ����¼����
	MCFLAG_OPT		+=  -DMMI_CALL_MINIMIZE_SUPPORT
		#ͨ����С��
	MCFLAG_OPT += -DMMI_SPEEDDIAL_SUPPORT
			#��ݲ���
	MCFLAG_OPT += -DMMI_IP_CALL_SUPPORT
			#IP����
	MCFLAG_OPT += -DMMI_MSD_SUPPORT
			#��������
	MCFLAG_OPT += -DMMI_BLACKLIST_SUPPORT
			#������
  #MCFLAG_OPT		+=  -DCL_SAVE_NAME_SUPPORT
		#ͨ����¼�д洢������		
	MCFLAG_OPT += -DMMI_DIALPANEL_DTMF_SUPPORT
			#������֧��DTMF
	#MCFLAG_OPT += -DMMI_POWER_ON_PWD_SOS_SUPPORT
			#�����������֧��SOS
  #MCFLAG_OPT  += -DMMI_VCALENDAR_SUPPORT
      #֧���ճ̵��롢���������ͺͽ���
      

## CPHS CSP(ALS) SUPPORT
ifeq ($(strip $(CPHS_SUPPORT)), TRUE)
	MCFLAG_OPT += -DMMI_CPHS_SUPPORT
	ifeq ($(strip $(CSP_SUPPORT)), TRUE)
		MCFLAG_OPT += -DMMI_CSP_SUPPORT
	endif
endif

ifneq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
    MCFLAG_OPT +=  -DMMI_REPLACE_SMS_SUPPORT
    	#Replace SMS֧��
    MCFLAG_OPT += -DMMI_STK_ICON_DISPLAY_SUPPORT
		  #STK ICON Display֧��
		MCFLAG_OPT += -DMMI_SMSCB_SUPPORT
		  #С���㲥
endif
	
	ifneq ($(strip $(ORANGE_SUPPORT)), TRUE)
		#R8
		MCFLAG_OPT += -DMMI_R8_SUPPORT
	endif
	
	#��������ģʽ
	ifeq ($(strip $(SMS_CHAT_SUPPORT)), TRUE)
      ifeq ($(strip $(MMS_SMS_IN_1_SUPPORT)), TRUE)
   	    ifeq ($(strip $(CHATMODE_ONLY_SUPPORT)), TRUE)
			MCFLAG_OPT += -DMMI_SMS_CHAT_SUPPORT
		endif
	  endif
    endif
	
	#���ŵ�������
	MCFLAG_OPT += -DMMI_SMS_EXPORT_SUPPORT
	
	#��ʱ����
#	MCFLAG_OPT += -DMMI_TIMERMSG_SUPPORT
	
	#�ܽӵ绰���Żظ�
	MCFLAG_OPT += -DMMI_REJECT_CALL_BY_SMS_SUPPORT
	
	#������������
	MCFLAG_OPT += -DMMI_SMS_FONT_SETTING_SUPPORT
	
	#��ȫ����
	MCFLAG_OPT += -DMMI_SMS_SECURITYBOX_SUPPORT
 		
 	#ɾ��ȫ��
 	MCFLAG_OPT += -DMMI_SMS_DELALL_SUPPORT
 	
 	#��������                             
        MCFLAG_OPT += -DMMI_SMS_VOICE_MAIL_SUPPORT	

        #���Ÿ��ƹ���                             
        MCFLAG_OPT += -DMMI_SMS_COPY_SUPPORT
	
        #�����ƶ�����
        MCFLAG_OPT += -DMMI_SMS_MOVE_SUPPORT		
  
	MCFLAG_OPT    +=    -DMMI_VCARD_SUPPORT   

# idle & widget start
ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)					
	MCFLAG_OPT		+= -DGRID_THUMBVIEW_SUPPORT			 #grid idle ����ͼ����
	#MCFLAG_OPT		+= -DGRID_SLIDE_WALLPAPER				 #idle ����ǽֽ������low memoryȫ��
	
	ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
		ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
	     MCFLAG_OPT += -DMMI_WIDGET_WEATHER2			# 2 for PDA sytle; weather 1 for bar phone
	  endif
	endif
	
	MCFLAG_OPT		+=  -DSCH_ASSISTANT_WIDGET    	 # �ճ�����		
	MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifneq ($(strip $(FM_SUPPORT)), NONE)	
  MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif    
  MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Img Viewer
ifeq ($(strip $(COUNTEDTIME_SUPPORT)), TRUE)
  MCFLAG_OPT		+= -DMMI_GRID_COUNTEDTIME			   #Grid Countedtime
endif
  MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar
  MCFLAG_OPT		+= -DMMI_GRID_CONTACT			       #Grid Contact  ��ϵ��
  MCFLAG_OPT    +=  -DMMI_WIDGET_TIME						 #����ҳЧ����ʱ��  

	
ifeq ($(strip $(WORLD_CLOCK_SUPPORT)), TRUE)
    MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_CLOCK   			#����˫ʱ��
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    MCFLAG_OPT       +=    -DMMI_WIDGET_DUAL_ACLOCK    #ģ��˫ʱ��
endif
endif    
    MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_SET    	#���ٿ��� wifi bt ������
    #MCFLAG_OPT       +=    -DMMI_WIDGET_SHORTCUT_MENU    #��ݷ�ʽ ����ʹ��
    MCFLAG_OPT       +=    -DMMI_WIDGET_ALARM
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)    
    MCFLAG_OPT       +=    -DMMI_WIDGET_ACLOCK   			 #ģ��ʱ��
endif
ifeq ($(strip $(ASP_SUPPORT)), TRUE)  
    #MCFLAG_OPT       +=    -DMMI_WIDGET_AUTOANSWER    #��¼�� ����ʹ��		#�Ѳ���ʹ��   
endif
ifeq ($(strip $(SEARCH_SUPPORT)), TRUE)
    MCFLAG_OPT       +=    -DMMI_WIDGET_SEARCH   			 #��������
endif
ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)    
    MCFLAG_OPT       +=    -DMMI_WIDGET_NETSEARCH  		 #��������
endif    
    MCFLAG_OPT       +=    -DMMI_WIDGET_NETWORK   		 #��������
    MCFLAG_OPT       +=    -DMMI_WIDGET_MEMO   				 #��ǩ  
endif			### touch panel
  
  ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE) 				#bar phone Ӧ���Զ������漸��widget
	  MCFLAG_OPT		+= -DBAR_PHONE_WIDGET			       #bar phone idle sytle
ifeq ($(strip $(MAINLCD_LOGIC_ANGLE)), 0)
#    MCFLAG_OPT += -DMMIWIDGET_SPECIAL_EFFECT   	# barphone widget ��Ч�Ͷ������ Ŀǰֻ������BARPHONE QVGA ����
endif	  
ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
	  MCFLAG_OPT 		+= -DMMI_WIDGET_WEATHER1				 # weather 	
    #MCFLAG_OPT += -DPDA_WEATHER_TIME_SE			    #����Ԥ����ʱ�䷭ҳ��Ч  
    #MCFLAG_OPT += -DMMIWIDGET_WEATHER_ANIM   	# ����Ԥ��get focuʱ�Ķ�����ԭ�а汾����HTC
ifeq ($(strip $(MAINLCD_LOGIC_ANGLE)), 0)
#    MCFLAG_OPT += -DMMIWIDGET_WEATHER_SE   	# ����Ԥ������ �¶�����Ŀǰֻ������BARPHONE QVGA ����
endif
endif	  
ifneq ($(strip $(FM_SUPPORT)), NONE)
    MCFLAG_OPT		+= -DMMI_WIDGET_FM							 #Grid FM  
endif      
	  MCFLAG_OPT		+= -DMMI_GRID_MP3			           #Grid Mp3
ifeq ($(strip $(MAINLCD_LOGIC_ANGLE)), 0)
#    MCFLAG_OPT += -DMMIWIDGET_MP3_SE   				# TSF ���Ķ���
endif	
  
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)	  
	  MCFLAG_OPT		+= -DMMI_GRID_IMG_VIEWER			   #Grid Image Viewer
endif	  
	  MCFLAG_OPT		+= -DMMI_GRID_CALENDAR			     #Grid Calendar	  
ifeq ($(strip $(ANALOG_CLK_SUPPORT)), TRUE)
	  MCFLAG_OPT		+= -DMMI_WIDGET_ACLOCK			     #3d clock	  

ifeq ($(strip $(MAINLCD_LOGIC_ANGLE)), 0)	  
#	  MCFLAG_OPT		+= -DMMIWIDGET_ACLOCK_SE
endif
endif
	  MCFLAG_OPT		+= -DMMI_WIDGET_SPEED_DIAL			 #Grid ���ٲ���	 
endif
endif		# end of ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
# idle & widget end
	
	#3d rotate effect support.
	ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)
	    MCFLAG_OPT		+= -DSET_3D_STYLE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_ROTATE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_LIST_ROLL_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_3D_MAIN_MENU_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_WIN_SLIDE_SUPPORT
	    MCFLAG_OPT		+= -DEFFECT_WIN_FADE_SUPPORT
	    #MCFLAG_OPT		+= -DDYNAMIC_WALLPAPER_SUPPORT    
	    #MCFLAG_OPT		+= -DEFFECT_FIRE_WALLPAPER_SUPPORT
	    ifneq ($(strip $(MOTION_SENSOR_TYPE)), NONE) 
	        MCFLAG_OPT		+= -DEFFECT_LCD_ROTATE_SUPPORT  #lcd rotate effect.
	    endif
	endif
	
	ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE)
      ifeq ($(strip $(SECMENU_ANIM_SUPPORT)), TRUE)
          MCFLAG_OPT += -DMMIMENU_PLAY_ANIM
      endif
	endif
	
	ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
		ifeq ($(strip $(SE_ROLL_SUPPORT)), TRUE)
			#    MCFLAG_OPT		+= -DROLL_MAINMENU_SUPPORT
		endif
	endif
	
	ifeq ($(strip $(AMR_A2DP_SUPPORT)), TRUE)
    MCFLAG_OPT      += -DMMI_AMRBT_VPSUPPORT  			#VP��AMR��ʽ��BT֧��
	endif

    MCFLAG_OPT		+= -DTAKESCREENPHOTO_SUPPORT               #��Ļ����

		#MCFLAG_OPT		+= -DMENU_CRYSTALCUBE_SUPPORT
    #MCFLAG_OPT		+= -DMENU_CYCLONE_SUPPORT
    MCFLAG_OPT		+= -DRES_STORE_CACHE_SUPPORT           #resource store cache support.
    MCFLAG_OPT		+= -DRES_VECTOR_CACHE_SUPPORT          #resource vector cache support.
    MCFLAG_OPT		+= -DPRINTSCREEN_SUPPORT               #��Ļץͼ 
    MCFLAG_OPT		+= -DMMI_AUTOTEST_SUPPORT              #auto test support.
    MCFLAG_OPT		+= -DMMI_AUTOTEST_WINIDNAME_SUPPORT              #auto test winidname support.
    
	  ifeq ($(strip $(PIC_EDITOR_SUPPORT)), TRUE)
        MCFLAG_OPT += -DPIC_EDIT_FRAME_SUPPORT        	 #picture editor frame support.
    endif

    MCFLAG_OPT      += -DCAMERA_FRAME_SUPPORT           #camera photo frame support.

    ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        MCFLAG_OPT		+= -DEFFECT_ZOOM_SUPPORT           #zoom effect support.
    endif

    ifeq ($(strip $(MMI_PUBWIN_ANIM_SUPPORT)), TRUE)     
        MCFLAG_OPT		+= -DMMI_MENUMASKLAYER_SUPPORT     #Menu Mask Layer
    endif
        
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE) 
      ifeq ($(strip $(UI_P3D_SUPPORT)), TRUE)     
        ifeq ($(strip $(SE_RIPPLE_SUPPORT)), TRUE)    
            MCFLAG_OPT		+= -DEFFECT_RIPPLE_SUPPORT     #ripple effect support.  
        endif
      endif
    endif
    
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RESOURCE
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_EBOOK  #EBOOK�ü�
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_PIC_EDIT #PIC edit�ü�MMIPIC_PASTE_IMAGE_NUM����
    MCFLAG_OPT       +=    -DMMI_LOW_MEMORY_RING #�����ü�
    
    #MCFLAG_OPT       +=    -DMMI_SETTING_LOW_MEMORY  #�滻��MMI_SETTING_CUTDOWN_MAINMENU��MMI_SETTING_CUTDOWN_WALLPAPER
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_MAINMENU  #���˵��ü�
    MCFLAG_OPT       +=    -DMMI_SETTING_CUTDOWN_WALLPAPER #ǽֽ�ü�
    
    MCFLAG_OPT       +=    -DMMI_DIAL_LOW_MEMORY
    MCFLAG_OPT       +=    -DMMI_KEY_LOCK_SCROLL_EFFECT_SUPPORT_RED_TOG
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)    
		MCFLAG_OPT       +=    -DMMI_PICREVIEW_LIST_LOW_MEMORY
endif 
    MCFLAG_OPT		+= -DPB_SUPPORT_LOW_MEMORY             #PB low memory   
    MCFLAG_OPT		+= -DMMIPB_MOST_USED_SUPPORT           #PB support MOST_USED_CONTACT.
    MCFLAG_OPT		+= -DMMIPB_TOOLBAR_SUPPORT           #PB support toolbar.    
    MCFLAG_OPT		+= -DMMIPB_SIMDN_SUPPORT           #PB support SIMDN.
    MCFLAG_OPT		+= -DMMIPB_ADD_EXIST_CONTACT_SUPPORT           #PB support ADD_EXIST_CONTACT_SUPPORT
    MCFLAG_OPT		+= -DMMIPB_SEARCH_RESULT_COLOR_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .
    MCFLAG_OPT		+= -DMMIPB_GROUP_MANAGE_SUPPORT           #PB support SEARCH_RESULT_COLOR_SUPPORT .  
    MCFLAG_OPT		+= -DMMIPB_SELECT_DISPLAY_LOCATION_SUPPORT           #PB support SELECT_DISPLAY_LOCATION
    MCFLAG_OPT		+= -DMMIPB_SELECT_STORE_LOCATION_SUPPORT           #PB support SELECT_STORE_LOCATION      
#   MCFLAG_OPT		+= -DSMS_SUPPORT_LOW_MEMORY            #sms support 200.
    MCFLAG_OPT		+= -DCALENDAR_SUPPORT_LOW_MEMORY       #calendar/schedule support 50.               
    MCFLAG_OPT		+= -DAUDIO_LOW_MEMORY_SUPPORT	       #audio buffer support low memory.
    MCFLAG_OPT		+= -DBT_SUPPORT_LOW_MEMORY             #bt max device number, 10.
    MCFLAG_OPT		+= -DALARM_SUPPORT_LOW_MEMORY          #alarm max number:5
    MCFLAG_OPT		+= -DBOOKMARK_SUPPORT_LOW_MEMORY       #bookmark max number:10
    MCFLAG_OPT		+= -DTTS_SUPPORT_LOW_MEMORY            #tts support low memory, reduce c
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_BUFFER    #streaming support low memory, reduce buffer
    MCFLAG_OPT		+= -DSTRM_SUPPORT_LOW_MEMORY_CONFLICT  #streaming support low memory, conflict      
    MCFLAG_OPT		+= -DRUNTIME_DRAW_SUPPORT
    
    MCFLAG_OPT		+= -DMAIN_MENU_SUPPORT_LOW_MEMORY  #main menu low memory support:icon menu.
    #MCFLAG_OPT		+= -DUI_LAYER_SUPPORT_LOW_MEMORY   #ui layer support 20 max layer blt.    
    MCFLAG_OPT		+= -DPB_NODE_CHANGE_GROUP_SUPPORT_LOW_MEMORY
	  MCFLAG_OPT		+= -DLIST_SLIDE_SUPPORT_LOW_MEMORY #list windows forbid slide to support low memory
	  
    ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
        ifeq ($(strip $(VIDEOPLAYER_MINI_FUNCTION)), TRUE)
            MCFLAG_OPT		+=  -DMMI_VIDEOPLAYER_MINI_FUNCTION
        endif
        
        ifneq ($(strip $(VIDEOPLAYER_MINI_FUNCTION)), TRUE)  
            MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HISTORYLIST_SUPPORT   #��ʷ��¼
            MCFLAG_OPT    += -DMMI_VIDEOPLAYER_SHORTCUTMENU_SUPPORT  #��ݴ���
            MCFLAG_OPT    += -DMMI_VIDEOPLAYER_HELP_SUPPORT  #Help����
            MCFLAG_OPT		+= -DVIDEOTHUMBNAIL_SUPPORT      	#��Ƶ�ļ�����ͼ��ȡ
            MCFLAG_OPT		+=  -DMMI_VIDEOWALLPAPER_SUPPORT
	     #MCFLAG_OPT		+=  -DMMI_VIDEOPLAYER_FIRST_SEARCH #������Ĭ���б�Ϊ��ʱ���ŷ������� 
        endif
    endif




		    
    ### mmi slide support
ifeq ($(strip $(TOUCH_PANEL_SUPPORT)), TRUE)    
    MCFLAG_OPT		+=  -DMMI_SLIDE_SUPPORT 
endif	
		ifeq ($(strip $(BROWSER_SUPPORT_DORADO)), TRUE)
			MCFLAG_OPT		+=  -DBROWSER_SEARCH_SUPPORT	
		  MCFLAG_OPT		+=  -DBROWSER_BOOKMARK_HISTORY_TAB
		
			ifneq ($(strip $(BROWSER_OPERA_START_PAGE)), TRUE)
			ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
				ifneq ($(strip $(MMI_SLIDE_SUPPORT)), NONE)
					#startpage is slide style
					MCFLAG_OPT		+=  -DBROWSER_START_PAGE_THEME1
			  else
			  	MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			  endif		
			else
				MCFLAG_OPT		+=  -DBROWSER_START_PAGE_DEFAULT
			endif
			endif
		
			MCFLAG_OPT		+=  -DMMIDORADO_MY_NAV_SUPPORT
		  MCFLAG_OPT		+=  -DMMIDORADO_FIX_NAV_SUPPORT
			#MCFLAG_OPT		+=  -DMMIDORADO_FIX_HMTL_SUPPORT
		  MCFLAG_OPT		+=  -DMMIDORADO_BGSOUND_SUPPORT
			MCFLAG_OPT		+=  -DMMIDORADO_INPUT_HISTORY_SUPPORT
			MCFLAG_OPT		+=  -DMMIBROWSER_DISPLAY_BK_HISTORY_INFO
	  endif
	  
	  ifneq ($(strip $(FM_SUPPORT)), NONE)
	    ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
			MCFLAG_OPT      += -DMMI_FM_TIMER_RECORD_SUPPORT 
			MCFLAG_OPT      += -DMMIFM_SUPPORT_FM_RECORD 
			endif
		ifneq ($(strip $(ATV_SUPPORT)), $(strip $(FM_SUPPORT)))
				MCFLAG_OPT      += -DMMI_FM_NEED_HEADSET
		endif
		endif
		
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			#��������GO�������Ч���Ŀ��غ�
			                                    
			MCFLAG_OPT	+= -DGO_BINARY_STAR_MAIN_MENU_SUPPORT    #Binary star
			
			MCFLAG_OPT	+= -DGO_SHUTTER_MAIN_MENU_SUPPORT  #shutter
			
			#MCFLAG_OPT	+= -DGO_SPHERE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CYLINDER_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_SLOPE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_WHEEL_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_OCEANWAVE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_PEGTOP_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_TURNTABLE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BOXROT_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_ICONBOXROT_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BOOKPAGE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_MIDROTATE_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_SMALLITEMS_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CLASSIC_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_FLASH_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_CONSTICK_MAIN_MENU_SUPPORT
			
			#MCFLAG_OPT	+= -DGO_SYMMETRY_MAIN_MENU_SUPPORT
			
			MCFLAG_OPT	+= -DGO_BENZITEM_MAIN_MENU_SUPPORT		
		endif
		#�����˵����һ���л�goЧ����ICON
		ifeq ($(strip $(PDA_UI_SUPPORT_MANIMENU_GO)), TRUE)
			MCFLAG_OPT		+=  -DGO_MENU_ONE_ICON_CHANGE_STYLE
		endif
		
		#����Ⱦ�˵����
		MCFLAG_OPT += -DPDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
		#�رճ���go�������������Ч��
		#MCFLAG_OPT		+= -DSLIDE_MAIN_MENU_SUPPORT       #slide style main menu support. 
		#MCFLAG_OPT		+= -DCUBE_MAIN_MENU_SUPPORT            #cube style main menu support. 
		#MCFLAG_OPT		+= -DTRAPEZOID_MAIN_MENU_SUPPORT       #trapezoid main menu support. 
		#MCFLAG_OPT		+= -DBINARY_STAR_MAIN_MENU_SUPPORT          #binary star style main menu support. 
		#MCFLAG_OPT		+= -DSHUTTER_MAIN_MENU_SUPPORT        #shutter main menu support.    
		#MCFLAG_OPT		+= -DCHARIOT_MAIN_MENU_SUPPORT        #chariot main menu support. 
		#MCFLAG_OPT		+= -DSPHERE_MAIN_MENU_SUPPORT          #sphere style main menu support. 
		#MCFLAG_OPT		+= -DCYLINDER_MAIN_MENU_SUPPORT        #cylinder main menu support. 

    #support app tool bar.
    MCFLAG_OPT		+= -DGUI_CTRL_TOOLBAR_SUPPORT	

    #support app clipboard slider.
    MCFLAG_OPT		+= -DCLIPBRD_SLIDER_SUPPORT  

    #support dynamic main menu.
    MCFLAG_OPT		+= -DDYNAMIC_MAINMENU_SUPPORT 

    #support calendar slide effect.
    ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)    
        MCFLAG_OPT		+= -DMMI_CALENDAR_SLIDE    
    endif

    #support multi sim color support.
    ifneq ($(strip $(MMI_MULTI_SIM_SYS)), SINGLE)
        MCFLAG_OPT		+=  -DMMI_MULTISIM_COLOR_SUPPORT
    endif 

    #support icon main menu style.
    MCFLAG_OPT		+=  -DMATRIX_MAIN_MENU_SUPPORT

#-------------------QCIF QQVGA MINI --------------------------#     
#  MCFLAG_OPT       +=    -DMMI_SMS_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_MMS_MINI_SUPPORT     
#  MCFLAG_OPT       +=    -DMMI_DIAL_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_CALLSET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_CALLCONTROL_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_ENVSET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_CHARGE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_PHONESET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_DISPLAYSET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_SECURITYSET_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_GRAPH_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_IDLE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_LIST_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_URL_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_STR_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_VIDEO_PLAYER_MINI_SUPPORT
   #�˴���64X64��MINI_KINGMOVIE֧��
   MCFLAG_OPT       +=    -DMMI_KINGMOVIE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_AUDIOPLAYER_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_ATV_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_MAINMENU_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_FM_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_PICVIEW_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_PB_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_ONLY_IDLE_DISP_STATUSBAR      
#  MCFLAG_OPT       +=    -DMMI_CL_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_BROWSER_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_CALENDAR_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_GAME_BOXMAN_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_CALCULATOR_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_MEMO_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_ALARM_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_COUNTEDTIME_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_UNITCONV_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_WORLDCLOCK_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_EBOOK_MINI_SUPPORT      
#  MCFLAG_OPT       +=    -DMMI_FMM_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DGUI_LISTITEM_STYLE_MINI_SUPPORT
#  MCFLAG_OPT       +=    -DMMI_KEYLOCK_MINI_SUPPORT
   
  MCFLAG_OPT       +=    -DGUI_CTRL_ICONFOLDER_SUPPORT
  MCFLAG_OPT       +=    -DGUI_ICONLIST_SUPPORT
  MCFLAG_OPT       +=    -DGUI_DROPDOWNLIST_SUPPORT
  MCFLAG_OPT       +=    -DGUI_FORM_DOCKABLE_SUPPORT
  MCFLAG_OPT       +=    -DGUI_PRGBOX_LETTERSCROLL_SUPPORT
  MCFLAG_OPT       +=    -DGUI_STB_SCROLL_SUPPORT

#------------------����ͳ�ƹ���----------------
	#MCFLAG_OPT += -DSALES_SUPPORT
#----------------------------------------------
#pbģ���Ƿ�֧��mempool
  MCFLAG_OPT       +=    -DMMIPB_MEMPOOL_SUPPORT
#ATV�򿪴���ʱת��
#  MCFLAG_OPT       +=    -DMMI_ATV_SWITCH_SCREEN_ON_OPEN
#�ļ��������ṩ�������ⲿģ�������ӿ�
  MCFLAG_OPT       +=    -DMMI_FMM_SORT_API_EXT_SUPPORT
#ͼƬ�ļ�����ͼ��ȡ
  MCFLAG_OPT		   +=    -DMMI_FMM_PICTHUMBNAIL_SUPPORT 
#popmenu ȫ����ʾ
#  MCFLAG_OPT       +=    -DGUI_FULLSCREEN_POPMENU_SUPPORT
#mp3 review ����
  MCFLAG_OPT         +=  -DMMI_AUDIOPLAYER_REVIEW_SUPPORT
#pb�޸�������󳤶�
#   MCFLAG_OPT      +=  -DMMI_PB_NAME_LEN_20_BYTES  
#����listbox�Զ���item style��layout
#   MCFLAG_OPT      +=     -DGUI_LIST_CUSTOMITEMSTYLE_SUPPORT  
#����С������������ĵ�GUI style
#   MCFLAG_OPT      +=     -DMMI_GUI_STYLE_MINISCREEN
#-------------------QCIF QQVGA MINI --------------------------# 
    
#��ص����ٷֱ���status bar����ʾ�Ĺ��ܺ� ���ú���˫��ع��ܻ��⣬˫�������£�������״̬������ʾ�����ǰ��Ϸ�����ʾ
ifneq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
MCFLAG_OPT       +=    -DMMI_BATTERY_PERCENT_IN_STATUSBAR
endif

#����������ȡ����sfs֧��xfind����xmove�Ĺ��ܣ���sfs.mk���ж��������ܵĿ���
#������32x64��32x16,16x16֮���size���棬���ԣ�����Ҳֻ��64x64��128x64�д�
	MCFLAG_OPT		+=  -DMMIFMM_XFIND_SUPPORT
	MCFLAG_OPT		+=  -DMMIFMM_XMOVE_SUPPORT
endif

endif	

#--------------------------------------------COMMON-----------------------------------------------------------------#
    ######################################################################################
    #                                  Notice!!!                                         #
    #                            other macro define                                      #                                                                       
    ######################################################################################
    
###########from gui.mk/mmk.mk/mmk_sys.mk begin    
#SMS number Config
ifeq ($(strip $(SMS_ITEM_NUMBER)), 50)
MCFLAG_OPT		+=  -DMMI_SMS_MAX_NUM_NV_50_ITEMS
endif
ifeq ($(strip $(SMS_ITEM_NUMBER)), 100)
MCFLAG_OPT		+=  -DMMI_SMS_MAX_NUM_NV_100_ITEMS
endif
ifeq ($(strip $(SMS_ITEM_NUMBER)), 200)
MCFLAG_OPT		+=  -DMMI_SMS_MAX_NUM_NV_200_ITEMS
endif
ifeq ($(strip $(SMS_ITEM_NUMBER)), 1000)
MCFLAG_OPT		+=  -DMMI_SMS_MAX_NUM_NV_1000_ITEMS
endif

    #TV ScalingDown support
ifeq ($(strip $(APP_TVSCALINGDOWN_ENABLE)), TRUE)
MCFLAG_OPT		+=  -DMMI_TVSCALINGDOWN_ENABLE
endif

#GBK support
#��Ҫ��ʱ���������ж�
#ifeq ($(strip $(PLATFORM)), SC6800H)
#MCFLAG_OPT_GBK_TEMP		?=  -DGBK_SUPPORT
#endif

#ifeq ($(strip $(PLATFORM)), SC8800H)
#MCFLAG_OPT_GBK_TEMP		?=  -DGBK_SUPPORT
#endif

#MCFLAG_OPT += $(MCFLAG_OPT_GBK_TEMP)
#end of GBK support

#GBK support
ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)), TRUE)
ifneq ($(strip $(FONT_HAN_SIMP_ONLY)), TRUE)
ifneq ($(strip $(BITMAP_FONT_SLIM_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DGBK_SUPPORT
endif
endif
endif

#BIG5 support
ifeq ($(strip $(IM_TRAD_CHINESE_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DBIG5_SUPPORT
endif

#���ڸ����ı�,��Ҫ�ϸ�ļ����ַ����Ŀ��
#��Ҫ��ʱ���������ж�
#arabic, thai, hindi, hebrew, tibet������Ϊ�����ı�
ifeq ($(strip $(IM_ARABIC_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_PERSIAN_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_URDU_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_THAI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_HINDI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_HEBREW_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_BENGALI_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

ifeq ($(strip $(IM_MYANMAR_SUPPORT)), TRUE)
MCFLAG_OPT_COMPLEX_TEMP		?=  -DCOMPLEX_TEXT_SCRIPT
endif

MCFLAG_OPT += $(MCFLAG_OPT_COMPLEX_TEMP)

#���ڸ����ı�,��Ҫ�ϸ�ļ����ַ����Ŀ��

########### POP MENU STYLE ###########
# main pop menu
#  1. GUIPOPMENU_POP_POP_SUPPORT
#		normal popup menu
#		submenu also popup
#
#  2. GUIPOPMENU_POP_CONTEXT_SUPPORT
#	  	look like POP_LINK, but no link arrow
#		submenu is context menu
#
#  3. GUIPOPMENU_OPTIONS_CONTEXT_SUPPORT
#		look like grid. and be more menu if more than 6 items
#		submenu is context menu
######################################

ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
		MCFLAG_OPT      += -DGUILIST_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUITAB_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUIEDIT_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUITITLE_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUISOFTKEY_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUIIM_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUIICONLIST_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUIPRGBOX_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUISECMENU_PDASTYLE_SUPPORT
		MCFLAG_OPT      += -DGUIPOPMENU_POP_CONTEXT_SUPPORT
		MCFLAG_OPT      += -DMMIPUBWIN_PDASTYLE_SUPPORT
		
else
		
		ifneq ($(strip $(MMI_GUI_STYLE_TYPICAL)), TRUE)
			ifneq ($(strip $(MAINLCD_SIZE)),176X220)
			ifneq ($(strip $(MAINLCD_SIZE)),128X160)
				MCFLAG_OPT      += -DGUILIST_PDASTYLE_SUPPORT
				MCFLAG_OPT      += -DGUITAB_PDASTYLE_SUPPORT
				MCFLAG_OPT      += -DGUITITLE_PDASTYLE_SUPPORT
				MCFLAG_OPT      += -DGUIIM_PDASTYLE_SUPPORT
				MCFLAG_OPT      += -DGUIICONLIST_PDASTYLE_SUPPORT
				MCFLAG_OPT      += -DGUIPRGBOX_PDASTYLE_SUPPORT
				MCFLAG_OPT      += -DGUISECMENU_PDASTYLE_SUPPORT	
				MCFLAG_OPT      += -DMMIPUBWIN_PDASTYLE_SUPPORT	
			endif
			endif								
				#MCFLAG_OPT      += -DGUIEDIT_PDASTYLE_SUPPORT
				##MCFLAG_OPT    += -DGUISOFTKEY_PDASTYLE_SUPPORT
				MCFLAG_OPT      += -DGUIPOPMENU_POP_POP_SUPPORT
				
		else
				MCFLAG_OPT      += -DGUIPOPMENU_POP_POP_SUPPORT
		endif
endif


###########from gui.mk/mmk.mk/mmk_sys.mk end    

ifeq ($(strip $(MET_MEX_SUPPORT)), TRUE)
MCFLAG_OPT += -DMEX_PLATFORM__FOR_SPRD6800H     ### չѶƽ̨����
                                        # Option1:SPRD6600L
                                        # Option2:SPRD6800H
MCFLAG_OPT += -DMEX_SPRD_CODE_VERSION=0x10A6530 ###չѶ����汾��
                                        # Option1:0x09A0937
                                        # Option2:0x09A1025
                                        # Option3:0x09A1109
                                        # Option4:0x09A1116                                        
                                        # Option5:0x10A1109
                                        # Option6:0x10A6530
MCFLAG_OPT += -DMEX_MTK_HAS_ZLIB=0       ###�������Ƿ���zlib
                                        # Option1:0
                                        # Option2:1
                                        
ifneq ($(strip $(MEX_G7_UI_SUPPORT)), TRUE)
MCFLAG_OPT += -DMEX_EXTERNALLIB_GUI
endif

ifeq ($(strip $(MET_MEX_SIMULATOR)), TRUE)
MCFLAG_OPT += -DMEX_PLATFORM_SIMULATOR -DMEX_TARGET_SERVER
else
MCFLAG_OPT += -DMEX_PLATFORM_SPREAD -DMEX_TARGET_SERVER
endif
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)), TRUE)
#	MCFLAG_OPT		+= -DFEATURE_BIG_ENDIAN

#	MCFLAG_OPT		+= -DSXH_APPLE_APP
	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT
#	MCFLAG_OPT		+= -DSXH_APPLE_VP
#	MCFLAG_OPT		+= -DSXH_APPLE_RP
#	MCFLAG_OPT		+= -DSXH_APPLE_3GMUSIC
#	MCFLAG_OPT		+= -DSXH_APPLE_ONLINEGAME
#	MCFLAG_OPT		+= -DSXH_APPLE_VIDEODL
#	MCFLAG_OPT		+= -DSXH_APPLE_UMPLAY
#	MCFLAG_OPT		+= -DSXH_APPLE_BOOK
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO
#	MCFLAG_OPT		+= -DSXH_APPLE_UMVCHAT_DEMO
	

#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_APPMAN
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_VP
#	MCFLAG_OPT		+= -DAPPLE_ROM_APP_UMVCHAT
endif


ifeq ($(strip $(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)), TRUE)        
        #all livewallpapers converted macro ; 
        #-DLIVE_WALLPAPER_SUPPORT_XXX=1, ��ʾ��Դ������ROM�
        #-DLIVE_WALLPAPER_SUPPORT_XXX=0, ��ʾ��Դ������T Flash��                         
  #��̬��ֽ֮LIGHTGRID ��Ч            ### LIGHTGRID_SE1        
	ifeq ($(strip $(LIGHTGRID_WALLPAPER_SUPPORT)), TCARD)                   
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_LIGHTGRID=0
	else
	  ifeq ($(strip $(LIGHTGRID_WALLPAPER_SUPPORT)), ROM)  
	  	MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_LIGHTGRID=1
	  endif
	endif	
	#��̬��ֽ֮COLLISION ��Ч            ### COLLISION_SE2
	ifeq ($(strip $(COLLISION_WALLPAPER_SUPPORT)), TCARD)                            
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_COLLISION=0
	else
	  ifeq ($(strip $(COLLISION_WALLPAPER_SUPPORT)), ROM)  
	  	MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_COLLISION=1
	  endif
	endif
	#��̬��ֽ֮COLLISION Balloon ��Ч### COLLISION_SE3
	ifeq ($(strip $(COLLISION_BALLOON_WALLPAPER_SUPPORT)), TCARD)                    
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON=0
	else
	  ifeq ($(strip $(COLLISION_BALLOON_WALLPAPER_SUPPORT)), ROM)
	  	MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_COLLISION_BALLOON=1
	  endif
	endif
	#��̬��ֽ֮COLLISION Menu ��Ч   ### COLLISION_SE4
	ifeq ($(strip $(COLLISION_MENU_WALLPAPER_SUPPORT)), TCARD)                             
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_COLLISION_MENU=0
	else
		ifeq ($(strip $(COLLISION_MENU_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_COLLISION_MENU=1
		endif
	endif
	#��̬��ֽ֮���Թ�Ļ��Ч   ### LINEAR_1
	ifeq ($(strip $(LINEAR_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_LINEAR=0
	else
		ifeq ($(strip $(LINEAR_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_LINEAR=1
		endif
	endif
	#��̬��ֽ֮POP ��Ч                     ### POP_2
	ifeq ($(strip $(POP_WALLPAPER_SUPPORT)), TCARD)   
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_POP=0
	else
		ifeq ($(strip $(POP_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_POP=1
		endif
	endif
	#��̬��ֽ֮BEE ��Ч                        ### BEE_3
	ifeq ($(strip $(BEE_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_BEE=0
	else
		ifeq ($(strip $(BEE_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_BEE=1
		endif
	endif
	#��̬��ֽ֮GALAXY ��Ч              ### GALAXY_4
	ifeq ($(strip $(GALAXY_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_GALAXY=0
	else
		ifeq ($(strip $(GALAXY_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_GALAXY=1
		endif
	endif
	#��̬��ֽ֮GALAXY ��Ч               ### SAKURA_5
	ifeq ($(strip $(SAKURA_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_SAKURA=0
	else
		ifeq ($(strip $(SAKURA_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_SAKURA=1
		endif
	endif
	#��̬��ֽ֮GALAXY ��Ч          ### SEAWORLD_6
	ifeq ($(strip $(SEAWORLD_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_SEAWORLD=0
	else
		ifeq ($(strip $(SEAWORLD_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_SEAWORLD=1
		endif
	endif
	#��̬��ֽ֮RIPPLE��Ч                  ### RIPPLE_7
	ifeq ($(strip $(RIPPLE_WALLPAPER_SUPPORT)), TCARD)  
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_RIPPLE=0
	else
		ifeq ($(strip $(RIPPLE_WALLPAPER_SUPPORT)), ROM)  
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_RIPPLE=1
		endif
	endif
	#��̬��ֽ֮LittleCat ��Ч              ### LITTLECAT_8
	ifeq ($(strip $(LITTLECAT_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_LITTLECAT=0
	else
		ifeq ($(strip $(LITTLECAT_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_LITTLECAT=1
		endif
	endif
	#��̬��ֽ֮COLLISION ��Ч         ### SWEETTREE_9
	ifeq ($(strip $(SWEETTREE_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_SWEETTREE=0
	else
		ifeq ($(strip $(SWEETTREE_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_SWEETTREE=1
		endif
	endif
	#��̬��ֽ֮WINDMILL ��Ч            ### WINDMILL_10
	ifeq ($(strip $(WINDMILL_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_WINDMILL=0
	else
		ifeq ($(strip $(WINDMILL_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_WINDMILL=1
		endif
	endif
	#��̬��ֽ֮RAINBOWCITY ��Ч   ### RAINBOWCITY_11
	ifeq ($(strip $(RAINBOWCITY_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_RAINBOWCITY=0
	else
		ifeq ($(strip $(RAINBOWCITY_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_RAINBOWCITY=1
		endif
	endif
	#��̬��ֽ֮PAPERTOWN ��Ч        ### PAPERTOWN_12
	ifeq ($(strip $(PAPERTOWN_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_PAPERTOWN=0
	else
		ifeq ($(strip $(PAPERTOWN_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_PAPERTOWN=1
		endif
	endif
	#��̬��ֽ֮AQUARIUM ��Ч            ### AQUARIUM_13
	ifeq ($(strip $(AQUARIUM_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_AQUARIUM=0
	else
		ifeq ($(strip $(AQUARIUM_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_AQUARIUM=1
		endif
	endif
	#��̬��ֽ֮GIRLSWING��Ч           ### GIRLSWING_14_
	ifeq ($(strip $(GIRLSWING_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_GIRLSWING=0
	else
		ifeq ($(strip $(GIRLSWING_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_GIRLSWING=1
		endif
	endif
	#��̬��ֽ֮3D SAKURA��Ч   ### SAKURA_3D_15
	ifeq ($(strip $(SPRING_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_SPRING=0
	else
		ifeq ($(strip $(SPRING_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_SPRING=1
		endif
	endif
	#��̬��ֽ֮3D NEST��Ч   ### NEST
	ifeq ($(strip $(NEST_WALLPAPER_SUPPORT)), TCARD)
		MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_NEST=0
	else
		ifeq ($(strip $(NEST_WALLPAPER_SUPPORT)), ROM)
			MCFLAG_OPT += -DLIVE_WALLPAPER_SUPPORT_NEST=1
		endif
	endif	
endif
    
#ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
#	MCFLAG_OPT		+=  -DMMI_GPRS_SUPPORT
#endif
    
ifeq ($(strip $(CAT_MVOICE_SUPPORT)), TRUE)
MCFLAG_OPT		+=     -DCAT_MVOICE_DEMO_SUPPORT
endif

    
MCFLAG_OPT      +=  -DUSB_LOG -DUSB_VUART
#MCFLAG_OPT     +=  -DMUSIC_PLAYER_SUPPORT

#���ػ�����ͨ����Դ�е�OPEN_MP4.3gp�ļ����ţ�Ĭ�Ϲرմ˹���
#MCFLAG_OPT      += -DMMI_POWERONOFFMP4_ENABLE

ifeq ($(strip $(MAINLCD_SIZE)), 128X64)
MCFLAG_OPT		+=  -DENGTD_SUPPORT
else
#�����֧��GZIP
MCFLAG_OPT		+=  -DBROWSER_GZIP_SUPPORT 

ifneq ($(strip $(UZONE_SUPPORT)), NONE)
#UZONE
MCFLAG_OPT		+=  -DMMIUZONE_SUPPORT
endif


#TD����ģʽ����
    ifeq ($(strip $(APP_PRODUCT_DM)), TRUE)
        MCFLAG_OPT		+=  -DPRODUCT_DM
        MCFLAG_OPT		+=  -DENGTD_SUPPORT
    endif
endif

MCFLAG_OPT		+=  -DKEY_RING_THROUGH_RECEIVER
		#���������Ŵ�receiver����

###MCFLAG_OPT		+=  -DTP_RING_THROUGH_RECEIVER
		#���������Ŵ�receiver����

#MCFLAG_OPT		+=  -DCMCC_IDLE_DISPLAY    
        #�ƶ����ƹ淶�Ĵ���������ʾ

ifeq ($(strip $(QBTHEME_SUPPORT)), TRUE)
ifeq ($(strip $(VECTOR_FONT_SUPPORT)), DEFAULT)
MCFLAG_OPT		+=  -DFILE_FONT_SUPPORT
		#��̬���������
endif
endif 
ifeq ($(strip $(FONT_TYPE_SUPPORT)), DE_BITMAP)
MCFLAG_OPT		+=  -DMULTI_LAN_SUPPORT_DE
endif
ifeq ($(strip $(FONT_TYPE_SUPPORT)), CSTAR_BITMAP)
MCFLAG_OPT		+=  -DSCRIPTEASY_SUPPORT
endif
ifeq ($(strip $(FONT_TYPE_SUPPORT)), CSTAR_BITMAP_SLIM)
MCFLAG_OPT		+=  -DSCRIPTEASY_SUPPORT
endif

#�����֧��Zawgyi��ʾ��Ĭ�Ϲر�
ifeq ($(strip $(IM_MYANMAR_SUPPORT)), TRUE)
ifeq ($(strip $(VECTOR_FONT_SUPPORT)), ETRUMP)
#MCFLAG_OPT		+=  -DMULTI_LAN_MYANMAR_ZAWGYI_SUPPORT
endif
endif

#ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
#MCFLAG_OPT		+=  -DMMI_GPRS_SWITCH_SUPPORT
#endif

#MCFLAG_OPT		+=  -DMMI_RES_TEXT_ALL_UNICODE_SUPPORT
            #Resource��TEXT��Դ��ʹ��Unicode
            #MS_Code\make\perl_script\ResGen\ResView.ini
            #define MMI_RES_TEXT_ALL_UNICODE_SUPPORT---->AllUseUnicode = 1
            #Not define MMI_RES_TEXT_ALL_UNICODE_SUPPORT---->AllUseUnicode = 0

ifeq ($(strip $(ORANGE_SUPPORT)), TRUE)
    MCFLAG_OPT      += -DMMI_USSD_DISPLAY_TIMEOUT_SUPPORT
		#USSD��ʱ�ر�

ifneq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
    MCFLAG_OPT		+=  -DMMI_SMS_MULTI_PARAM_SUPPORT
		#����Ϣ�����Ϣ����֧��
endif

    MCFLAG_OPT		+=  -DMMI_SMS_CONCATENATED_SETTING_SUPPORT
		#��������Ϣ����֧��

    MCFLAG_OPT		+=  -DMMI_SMS_ALPHABET_SETTING_SUPPORT
		#����Ϣ���뷽ʽ����֧��

    MCFLAG_OPT		+=  -DMMI_SMS_REPLY_PATH_SUPPORT
		#Reply path֧��

    MCFLAG_OPT += -DMMI_CONCATENATED_SMS_SEND_QUERY_SUPPORT
		#�����ŷ�����ʾ֧��

    MCFLAG_OPT += -DMMI_IDLE_NO_SIM_SOS_SUPPORT
		#��SIM��Idle��ʾSOS

    #MCFLAG_OPT += -DMMI_IDLE_FDN_ICON_SUPPORT
		#Status bar��ʾFDNͼ��
ifneq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
    MCFLAG_OPT += -DMMI_PREFER_PLMN_SUPPORT
		#��ѡ����
    MCFLAG_OPT += -DMMI_LDN_SUPPORT
		#LDN Support
endif

    MCFLAG_OPT += -DMMI_CC_STK_MENU_SUPPORT
		#CC����ʾSTK�˵�

    MCFLAG_OPT += -DMMI_ECT_MENU_SUPPORT
		#ECT�˵�֧��
		
    MCFLAG_OPT += -DMMI_CRSS_SUPPORT
                ## MMI_CRSS_SUPPORT
    MCFLAG_OPT += -DMMI_FDN_DIAL_TO_PANEL_SUPPORT
		#FDN����ǰ�༭֧��

    MCFLAG_OPT += -DMMI_PB_EXT_NUM_SUPPORT
		#PB EXT����֧��
endif

#����ģʽ
ifeq ($(strip $(OMADL_SUPPORT)), TRUE)    
    MCFLAG_OPT		+=  -DOMADL_SUPPORT
endif

ifneq ($(strip $(DRM_SUPPORT)),)  
   ifneq ($(strip $(DRM_SUPPORT)), NONE)    
        MCFLAG_OPT		+=  -DDRM_SUPPORT_$(strip $(DRM_SUPPORT))  -DDRM_SUPPORT
   else
        MCFLAG_OPT		+=  -DDRM_SUPPORT_NONE
   endif
	 ifeq ($(strip $(DRM_SUPPORT)), V1)
			  MCFLAG_OPT   += -DMMI_DRM_SURPORT_FL_TYPE
    	  MCFLAG_OPT   += -DMMI_DRM_SURPORT_CD_TYPE
			  MCFLAG_OPT   += -DMMI_DRM_SURPORT_SD_TYPE
	 endif
   ifeq ($(strip $(DRM_SUPPORT)), FL)
    	 MCFLAG_OPT   += -DMMI_DRM_SURPORT_FL_TYPE
	 endif
   ifeq ($(strip $(DRM_SUPPORT)), CD)
    	 MCFLAG_OPT   += -DMMI_DRM_SURPORT_CD_TYPE
	 endif
   ifeq ($(strip $(DRM_SUPPORT)), SD)
    	 MCFLAG_OPT   += -DMMI_DRM_SURPORT_SD_TYPE
	 endif
endif

ifeq ($(strip $(UPDATE_TIME_SUPPORT)),TRUE)
    ##  MMI_UPDATE_TIME_SUPPORT #for mmi
    MCFLAG_OPT		+=  -DMMI_UPDATE_TIME_SUPPORT
    ##  MMI_UPDATE_TIME_WHEN_STARTUP
    MCFLAG_OPT		+=  -DMMI_UPDATE_TIME_WHEN_STARTUP
    ##  MMI_NET_TIME_SUPPORT
#    MCFLAG_OPT		+=  -DMMI_NET_TIME_SUPPORT	
#	 MMI_NET_TIME_SUPPORT(��ҳ����ʱ��) is easy to get time, however it will get the false nettime sometimes so close it
ifeq ($(strip $(GPRS_SUPPORT)), TRUE)
    ifeq ($(strip $(SNTP_SUPPORT)), TRUE)    
        MCFLAG_OPT		+=  -DMMI_SNTP_SUPPORT
    endif
endif    
endif


#˫��� ȡ���� ����DUAL_BATTERY_SUPPORT���Ƿ��
ifeq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
	MCFLAG_OPT       +=    -DMMI_DUAL_BATTERY_SUPPORT
endif



ifeq ($(strip $(SYNCTOOL_SUPPORT)), TRUE)
MCFLAG_OPT      += -DMMI_SYNCTOOL_SUPPORT
endif

ifeq ($(strip $(CARD_LOG)), TRUE)
MCFLAG_OPT      += -DCARD_LOG
endif

# Auto Testing Support
ifeq ($(strip $(ATEST_SUPPORT)), TRUE)
MCFLAG_OPT  	+=  -D_ATEST_
endif

#GSM/TD˫ģ, ��ʱ����
ifeq ($(strip $(APP_DUALMODE_ENABLE)), TRUE)
MCFLAG_OPT		+=  -DMMI_DUALMODE_ENABLE
endif

ifeq ($(strip $(DEV_MANAGE_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DDM_SUPPORT
else
ifeq ($(strip $(DM_ONLY_SELFREG_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DDM_ONLY_SELFREG_SUPPORT
endif
endif

ifeq ($(strip $(ASP_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DMMI_ENABLE_ASP
endif

ifeq ($(strip $(DCD_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DMMI_ENABLE_DCD
endif

MCFLAG_OPT		+=  -DIM_ENGLISH_SUPPORT

ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)
MCFLAG_OPT		+=  -DTOUCH_PANEL_SUPPORT
endif
ifeq ($(strip $(TOUCHPANEL_TYPE)), NONE)
###MCFLAG_OPT		+=  -DRECORD_BARPHONE_UI_STYLE_SUPPORT
endif
ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE) 
ifeq ($(strip $(CAMERA_SUPPORT)), TRUE) 
MCFLAG_OPT		+=  -DKEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
endif
endif



ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT		+=  -DP3D_AUDIO_EQ_CONFLICT
endif

#6600L and 8800H support p3d when mp3bg
ifeq ($(strip $(PLATFORM)), SC6600L)
MCFLAG_OPT		+=  -DP3D_WHEN_MP3BG_SUPPORT
endif
ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT		+=  -DP3D_WHEN_MP3BG_SUPPORT
endif

ifeq ($(strip $(JAVA_SUPPORT)), IA)
MCFLAG_OPT		+=  -DJAVA_OTA_SUPPORT
#MCFLAG_OPT		+=  -DMMI_JAVA_PIM
MCFLAG_OPT		+=  -DIA_JAVA_SUPPORT_LOW_MEMORY    #ia java support low memory project,allocate small memory to jvm if enable this macro
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
MCFLAG_OPT		+= -DP3D_WHEN_MP3BG_SUPPORT -DFPGA_6800H_POWER_ON -DPLATFORM_SC6800H -DAUDIO_SC6800H  -DDC_SC6800H \
                           -DMV_EFFECT_SUPPORT	
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MCFLAG_OPT		+= -DP3D_WHEN_MP3BG_SUPPORT  -DAUDIO_SC6800H  -DDC_SC6800H \
                           -DMV_EFFECT_SUPPORT	
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT		+= -DP3D_WHEN_MP3BG_SUPPORT  -DAUDIO_SC6800H  -DDC_SC6800H \
                           -DMV_EFFECT_SUPPORT	
endif

ifeq ($(strip $(PIM_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DPIM_SUPPORT
endif

ifeq ($(strip $(BT_FTS_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DMMIBT_FTS_SUPPORT
endif

ifeq ($(strip $(ASSERT_TYPE_SUPPORT)), ASSERT)
MCFLAG_OPT		+=  -DMMI_ASSERT_MODE
endif

#֧��Mocorʾ������
ifeq ($(strip $(MMI_MOCOR_SAMPLE)), TRUE)
MCFLAG_OPT		+=  -DMMI_MOCOR_SAMPLE
endif

ifeq ($(strip $(UI_SPRITE_ENABLE)), TRUE)
MCFLAG_OPT		+=  -DMMI_SPRITE_ENABLE
endif

#֧��streaming
ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DMMI_STREAMING_PLAYER_SUPPORT
endif
endif
    
ifeq ($(strip $(TMVB_DEC_SUPPORT)), TRUE)
MCFLAG_OPT += -DMMI_RMVB_SUPPORT
endif


#֧��kmovie
ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
MCFLAG_OPT		+=  -DMMI_KING_MOVIE_SUPPORT
MCFLAG_OPT		+=  -DMMI_KING_MOVIE_SUPPORT_EXTEND
MCFLAG_OPT		+=  -DMMI_KING_MOVIE_SUPPORT_HMV
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
MCFLAG_OPT		+=  -DMMI_KING_MOVIE_SUPPORT
MCFLAG_OPT		+=  -DMMI_KING_MOVIE_SUPPORT_EXTEND
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
MCFLAG_OPT		+=  -DMMI_KING_MOVIE_SUPPORT
endif
endif
endif

ifeq ($(strip $(UMEM_SUPPORT)),	TRUE)
MCFLAG_OPT += -DMMI_UDISK_MEM_ENABLE
endif

ifeq ($(strip $(DV_UDISK_SUPPORT)),TRUE)
###MCFLAG_OPT += -DMMI_DV_UDISK_ENABLE
endif

ifeq ($(strip $(MULTI_LAN_SUPPORT)),CSTAR)
#����һ�ְ��������߲�˹���˿ӡ
#MCFLAG_OPT              +=  -DMMI_SPECIAL_ARABIC_PERSIAN
#����һ��̩���˿ӡ
#MCFLAG_OPT              +=  -DMMI_SPECIAL_THAI
#����һ��ӡ�����˿ӡ
#MCFLAG_OPT              +=  -DMMI_SPECIAL_HINDI
endif

#32X32���̲ü����˹���
#cstar ���뷨������֧��SD������U�̣�����֧��������ļ��Ĵ洢�����ʼ��书��
ifneq ($(strip $(LOW_MEMORY_SUPPORT)), 32X32)
ifeq ($(strip $(IM_ENGINE)),CSTAR)
		ifeq ($(strip $(MSDC_CARD_SUPPORT)), TRUE)
				MCFLAG_OPT		+=  -DMMI_CSTAR_UDB_SUPPORT
		else
				ifeq ($(strip $(UMEM_SUPPORT)), TRUE)
						MCFLAG_OPT		+=  -DMMI_CSTAR_UDB_SUPPORT
				endif
		endif
endif
endif

#BIG5 support
ifeq ($(strip $(IM_TRAD_CHINESE_SUPPORT)), TRUE)
MCFLAG_OPT		+=  -DBIG5_SUPPORT
endif

##picview module
ifeq ($(strip $(PIC_VIEWER_SUPPORT)), TRUE)
ifeq ($(strip $(PIC_ZOOM_SUPPORT)), TRUE)
    MCFLAG_OPT	+=  -DPICVIEW_VIEW_ZOOM
    MCFLAG_OPT	+=  -DPICVIEW_F_VIEW_ZOOM
endif
endif
ifneq ($(strip $(ATV_SUPPORT)), NONE)
    ifneq ($(strip $(LOW_MEMORY_SUPPORT)), 32X32)
			ifeq ($(strip $(AVI_ENC_SUPPORT)), TRUE)
				MCFLAG_OPT += -DMMI_ATV_RECORD_SUPPORT
			endif
			ifeq ($(strip $(TYPE_3GP_SUPPORT)), MP4)
				###MCFLAG_OPT += -DMMI_ATV_RECORD_SUPPORT
			endif
	endif
endif
##end picview module

#���grid idleΪtrue����Ҫshortcut set��menu
ifeq ($(strip $(MMI_GRID_IDLE_SUPPORT)), TRUE)
		MCFLAG_OPT       += -DMMI_DISABLE_SHORTCUT_SET_SUPPORT
endif
		MCFLAG_OPT       += -DMMI_NETWORK_NAME_SUPPORT
		# support MMI_NETWORK_NAME_SUPPORT macro
		
    MCFLAG_OPT += -DMMI_CRSS_SUPPORT
                ## MMI_CRSS_SUPPORT	

#----------------------------------------------#

ifeq ($(strip $(MAINLCD_SIZE)),240X320)
ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X32)
  MCFLAG_OPT       +=    -DMMI_MINI_QVGA_UI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_KEYLOCK_MINI_SUPPORT  
endif
ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 16X16)
  MCFLAG_OPT       +=    -DMMI_MINI_QVGA_UI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_KEYLOCK_MINI_SUPPORT
endif
endif

ifeq ($(strip $(MAINLCD_SIZE)),176X220)
  MCFLAG_OPT       +=    -DMMI_CALLCONTROL_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_DIAL_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_ONLY_IDLE_DISP_STATUSBAR
ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
  MCFLAG_OPT	     +=	   -DMMI_RECORD_MINI_SUPPORT
endif
  MCFLAG_OPT       +=    -DMMI_AUDIOPLAYER_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_FM_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_IDLE_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_KEYLOCK_MINI_SUPPORT 
  MCFLAG_OPT       +=    -DMMI_ATV_MINI_SUPPORT 
  MCFLAG_OPT       +=    -DMMI_VIDEO_PLAYER_MINI_LAYOUT
#����С������������ĵ�GUI style
   MCFLAG_OPT      +=     -DMMI_GUI_STYLE_MINISCREEN
#popmenu ȫ����ʾ
   MCFLAG_OPT       +=    -DGUI_FULLSCREEN_POPMENU_SUPPORT 

   MCFLAG_OPT       +=    -DMMI_ALARM_MINI_SUPPORT
   MCFLAG_OPT       +=    -DMMI_GAME_BOXMAN_MINI_SUPPORT
   MCFLAG_OPT       +=    -DMMI_CALCULATOR_MINI_SUPPORT 
   MCFLAG_OPT       +=    -DMMI_MEMO_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_COUNTEDTIME_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_UNITCONV_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_WORLDCLOCK_MINI_SUPPORT    
   MCFLAG_OPT       +=    -DMMI_SECMENU_ICON_DEFAULT 
endif 

ifeq ($(strip $(MAINLCD_SIZE)),128X160)
  MCFLAG_OPT       +=    -DMMI_CALLCONTROL_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_DIAL_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_ONLY_IDLE_DISP_STATUSBAR
ifeq ($(strip $(RECORD_SUPPORT)), TRUE)
  MCFLAG_OPT	     +=	   -DMMI_RECORD_MINI_SUPPORT
endif
  MCFLAG_OPT       +=    -DMMI_AUDIOPLAYER_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_FM_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_IDLE_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_KEYLOCK_MINI_SUPPORT
  MCFLAG_OPT       +=    -DMMI_VIDEO_PLAYER_MINI_LAYOUT
  MCFLAG_OPT       +=    -DMMI_ATV_MINI_SUPPORT  

#����С������������ĵ�GUI style
   MCFLAG_OPT      +=     -DMMI_GUI_STYLE_MINISCREEN 
#popmenu ȫ����ʾ
   MCFLAG_OPT       +=    -DGUI_FULLSCREEN_POPMENU_SUPPORT 
   
   MCFLAG_OPT       +=    -DMMI_ALARM_MINI_SUPPORT
   MCFLAG_OPT       +=    -DMMI_GAME_BOXMAN_MINI_SUPPORT
   MCFLAG_OPT       +=    -DMMI_CALCULATOR_MINI_SUPPORT 
   MCFLAG_OPT       +=    -DMMI_MEMO_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_COUNTEDTIME_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_UNITCONV_MINI_SUPPORT      
   MCFLAG_OPT       +=    -DMMI_WORLDCLOCK_MINI_SUPPORT  
   MCFLAG_OPT       +=    -DMMI_SECMENU_ICON_DEFAULT
endif   

#ʹ��bookmark��ʱ��ŷ����ڴ沢��ȡ���ӿ쿪���ٶ�
ifeq ($(strip $(BROWSER_SUPPORT)), TRUE)
   MCFLAG_OPT       +=    -DMMI_BROWSER_BOOKMARK_ALLOC_SUPPORT
endif

ifneq ($(strip $(OPTION_MENU_SUPPORT)), FALSE)
  MCFLAG_OPT		+=  -DMMI_OPTIONMENU_SUPPORT
endif


#С���м��ʹ��ͼƬ
ifeq ($(strip $(MID_SOFTKEY_USE_IMG_SUPPORT)),TRUE)
  MCFLAG_OPT       +=    -DGUI_MID_SOFTKEY_USE_IMG
endif

#128X160��176X220�ֱ����������finish��cancel����ʾAINM_BT_SEARCHING
ifneq ($(strip $(BLUETOOTH_SUPPORT)), NONE)
	ifeq ($(strip $(MAINLCD_SIZE)), 128X160)
  	MCFLAG_OPT       +=    -DMMI_BLUETOOTH_MINI_SUPPORT
  endif
  ifeq ($(strip $(MAINLCD_SIZE)), 176X220)
  	MCFLAG_OPT       +=    -DMMI_BLUETOOTH_MINI_SUPPORT
  endif
endif
#-----------------------------------------------#
ifeq ($(strip $(PDA_UI_SUPPORT_U_IDLE)), TRUE)
MCFLAG_OPT += -DMMI_IDLE_WIDGET_CACHE
endif

ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
MCFLAG_OPT += -DMMI_PDA_PREVIEW_BUTTON_SUPPORT
endif

#MCFLAG_OPT += -DMMI_PLMN_LOAD_DEFAULT_SUPPORT
##support MMI_PLMN_LOAD_DEFAULT_SUPPORT macro

ifeq ($(strip $(SOUNDBANK_MODE)), COMPACT)
##MCFLAG_OPT  	+=  -DMMI_SOUNDBANK_MODE_COMPACT
endif

ifeq ($(strip $(TOUCHPANEL_TYPE)), MULTITP)
	MCFLAG_OPT += -DTOUCH_GESTURE_ENABLE
else
	ifeq ($(strip $(TOUCHPANEL_TYPE)), HW)
	ifeq ($(strip $(TPC_TYPE)), DOUBLE)
		MCFLAG_OPT += -DTOUCH_GESTURE_ENABLE
	endif
	endif
endif

ifeq ($(strip $(MENU_SWEETS_SUPPORT)), TRUE)
MCFLAG_OPT += -DMMI_RES_OUTSIDE_SUPPORT
endif

ifeq ($(strip $(MMI_PDA_SUPPORT)), TRUE)
MCFLAG_OPT += -DMMI_PDA_PREVIEW_BUTTON_SUPPORT
endif

ifeq ($(strip $(MMI_RES_ORIENT)), LANDSCAPE)         
	ifneq ($(strip $(TOUCHPANEL_TYPE)), NONE)          
		MCFLAG_OPT += -DMMI_GRID_REMOVE_SHORTCUT_ICON    
	endif                                              
endif

ifeq ($(strip $(MMI_MULTI_SIM_SYS)), DUAL)
    MCFLAG_OPT +=  -DMMI_DUALSIM_CALL_DIVERT_SUPPORT
    #  MMI_DUALSIM_CALL_DIVERT_SUPPORT
    # MMIPB_SIMDN_SUPPORT must define 
endif

ifeq ($(strip $(SIM_LOCK_SUPPORT)), TRUE)
	MCFLAG_OPT += -DMMI_SIM_LOCK_SUPPORT
	## MMI_SIM_LOCK_SUPPORT
endif

ifeq ($(strip $(TIMER_MSG_SUPPORT)), TRUE)
  MCFLAG_OPT += -DMMI_TIMERMSG_SUPPORT
endif

ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
ifneq ($(strip $(WEATHER_SUPPORT_GOOGLE)), TRUE)
MCFLAG_OPT += -DMMI_WEATHER_JSON_PARSE 
#MMI_WEATHER_SUPPORT_WUNDERGROUND��MMIWEATHER_SUPPORT_SPRD_SRVֻ�ܿ�����һ�������ұ����MMI_WEATHER_JSON_PARSE 
MCFLAG_OPT += -DMMI_WEATHER_SUPPORT_WUNDERGROUND   
#MCFLAG_OPT += -DMMIWEATHER_SUPPORT_SPRD_SRV     
endif    
endif
MCFLAG_OPT += -DMMI_IDLE_FDN_ICON_SUPPORT
#Status bar��ʾFDNͼ��

#SPRD_BTĬ��֧��BQB
#ifeq ($(strip $(BLUETOOTH_SUPPORT)),SPRD_BT)
#	MCFLAG_OPT += -DBT_BQB_SUPPORT
#endif

#SPRD_BTĬ��֧�ֽӴ�绰ʱ������������ź���Ͳ֮����Ƶ�л�
ifeq ($(strip $(BLUETOOTH_SUPPORT)), SPRD_BT)
   MCFLAG_OPT       +=    -DMMI_CC_SWITCH_BT_HF_HH
endif


#ifeq ($(strip $(BT_SPP_SUPPORT)),TRUE)
#	MCFLAG_OPT += -DBT_SPP_FILE_SUPPORT
#endif

#A2DP���ʷ�Ϊ����HIGH��MID��LOW
ifeq ($(strip $(BT_A2DP_SUPPORT)),TRUE)
	ifeq ($(strip $(MMI_BT_A2DP_QUALITY)),HIGH)
		MCFLAG_OPT += -DMMI_BT_HIGH_QUALITY
	else
		ifeq ($(strip $(MMI_BT_A2DP_QUALITY)),MID)
			MCFLAG_OPT += -DMMI_BT_MID_QUALITY
		else
			MCFLAG_OPT += -DMMI_BT_LOW_QUALITY
		endif
	endif
endif
ifeq ($(strip $(BITMAP_FONT_SLIM_SUPPORT)),TRUE)
  ifeq ($(strip $(MAINLCD_SIZE)),240X320)#240X320
    MCFLAG_OPT += -DMMI_DEFAULT_FONT_20_SUPPORT #Ĭ������Ϊ20��
  endif	#240X320
  ifeq ($(strip $(FONT_TYPE_SUPPORT)),CSTAR_BITMAP)
	  MCFLAG_OPT += -DCSTAR_BITMAP_SLIM
  endif	
  ifeq ($(strip $(FONT_TYPE_SUPPORT)),CSTAR_BITMAP_SLIM)
	  MCFLAG_OPT += -DCSTAR_BITMAP_SLIM
  endif	
  ifeq ($(strip $(FONT_TYPE_SUPPORT)),DE_BITMAP)
	  MCFLAG_OPT += -DDE_BITMAP_SLIM
	  ifeq ($(strip $(LOW_MEMORY_SUPPORT)),32X32)#32X32	  
		  ifeq ($(strip $(MAINLCD_SIZE)),240X320)#240X320
				  ifeq ($(strip $(IM_SIMP_CHINESE_SUPPORT)),TRUE)# simplified_chinese
				      MCFLAG_OPT += -DUSING_16_SLIM_EXTREME  #//���ּ���ü�  
				  endif# simplified_chinese  
		  endif	#240X320      
	  endif#32X32
  endif	
endif

#pb��¼�������Ϊ50
ifeq ($(strip $(PHONEBOOK_ITEM_NUMBER)),50)
	MCFLAG_OPT		+=  -DMMIPB_RECODE_MAX_50
endif
#pb��¼�������Ϊ100
ifeq ($(strip $(PHONEBOOK_ITEM_NUMBER)),100)
	MCFLAG_OPT		+=  -DMMIPB_RECODE_MAX_100
endif
#pb��¼�������Ϊ150
ifeq ($(strip $(PHONEBOOK_ITEM_NUMBER)),150)
	MCFLAG_OPT		+=  -DMMIPB_RECODE_MAX_150
endif
#pb��¼�������Ϊ300
ifeq ($(strip $(PHONEBOOK_ITEM_NUMBER)),300)
	MCFLAG_OPT		+=  -DMMIPB_RECODE_MAX_300
endif
#pb��¼�������Ϊ500
ifeq ($(strip $(PHONEBOOK_ITEM_NUMBER)),500)
	MCFLAG_OPT		+=  -DMMIPB_RECODE_MAX_500
endif
#pb��¼�������Ϊ1000
ifeq ($(strip $(PHONEBOOK_ITEM_NUMBER)),1000)
	ifeq ($(strip $(NANDBOOT_SUPPORT)),TRUE)
		MCFLAG_OPT		+=  -DMMIPB_RECODE_MAX_1000
	else
		MCFLAG_OPT		+=  -DMMIPB_RECODE_MAX_500
	endif
endif
ifneq ($(strip $(TOUCHPANEL_TYPE)),NONE)
	ifeq ($(strip $(LOW_MEMORY_SUPPORT)),32X32)
	  ifeq ($(strip $(MAINLCD_SIZE)),240X320)
	      MCFLAG_OPT		+=  -DMMI_MAINMANU_NO_TPMOVE
	      MCFLAG_OPT		+=  -DMMI_SECMENU_ICON_DEFAULT
	  endif	
	endif
endif

#MRAPP:Ӧ��ѡ�񿪹�
#����ع�ע���漸��Ӧ��ѡ��ĺ꣬ȷ����ʹ�õ�Ӧ����رն�Ӧ�ĺ꣬�Խ�Լ����ռ�
#���ȣ���Ҫȷ�������İ汾�Ƿ�Ϊoversea version

	#MCFLAG_OPT		+=  -D__MRAPP_OVERSEA__

#Ȼ��,���ݾ�����Ҫ��������Ӧ��Ӧ��

ifeq ($(strip $(MRAPP_SUPPORT)),TRUE)

#���òü��汾��MRAPP--�˴�ָͨ��ȥ��ð���������ģ��Ӷ��ü�һЩͼƬ��Դ
	MCFLAG_OPT		+=  -DMRAPP_SLIM

	ifeq ($(strip $(__MRAPP_OVERSEA__)),TRUE)  #����汾
		#MCFLAG_OPT		+=  -DMRAPP_MRPSTORE_OPEN  #MRP Store
  	#MCFLAG_OPT		+=  -DMRAPP_SKYBUDDY_OPEN  #Skybuddy
  	#MCFLAG_OPT		+=  -DMRAPP_MSN_OPEN			 #Msn
  	#MCFLAG_OPT		+=  -DMRAPP_FACEBOOK_OPEN  #Facebook
  	#MCFLAG_OPT		+=  -DMRAPP_TWITTER_OPEN   #Twitter
  	#MCFLAG_OPT		+=  -DMRAPP_SKYPE_OPEN     #Skype
  	#MCFLAG_OPT		+=  -DMRAPP_YAHOO_OPEN     #Yahoo Message
  else
  	#MCFLAG_OPT		+=  -DMRAPP_NETGAME_OPEN   #ð������
  	MCFLAG_OPT		+=  -DMRAPP_COOKIE_OPEN	   #����
  	#MCFLAG_OPT		+=  -DMRAPP_EBOOK_OPEN	   #ð�����
  	#MCFLAG_OPT		+=  -DMRAPP_MSN_OPEN		   #MSN
  	#MCFLAG_OPT		+=  -DMRAPP_FETION_OPEN	   #����
  	MCFLAG_OPT		+=  -DMRAPP_SDKQQ_OPEN	   #QQ
  	#MCFLAG_OPT		+=  -DMRAPP_KAIXIN_OPEN	   #������
  	#MCFLAG_OPT		+=  -DMRAPP_CLOCK_OPEN	   #����ʱ��
  	#MCFLAG_OPT		+=  -DMRAPP_XLWB_OPEN      #����΢��
  	#MCFLAG_OPT		+=  -DMRAPP_BIRD_OPEN	     #��ŭС��
  	#MCFLAG_OPT		+=  -DMRAPP_BYDR_OPEN	     #������� 
  	#MCFLAG_OPT		+=  -DMRAPP_SHOUXIN_OPEN   #����     
  	MCFLAG_OPT		+=  -D__MRAPP_DOWNLOAD__     #��������   
  	#MCFLAG_OPT		+=  -DMRAPP_QIPAI_OPEN	   #ð������ 
  	#MCFLAG_OPT		+=  -DMRAPP_DDZ_OPEN	     #������     
  	#MCFLAG_OPT		+=  -DMRAPP_FARM_OPEN      #ð��ũ�� 
  	#MCFLAG_OPT		+=  -DMRAPP_TIANQI_OPEN    #�������� 
  	#MCFLAG_OPT		+=  -DMRAPP_TOOL_OPEN      #ʵ�ù���
  	#MCFLAG_OPT		+=  -DMRAPP_SIMUGAME_OPEN  #NESģ����
	endif 
  ifeq ($(strip $(__SKY_MM_STREAMLINED__)),TRUE)
  	#MCFLAG_OPT		+=  -DMRAPP_MPLIVE_MAINMENU_OPEN  #�ֻ����� 
		#MCFLAG_OPT		+=  -DMRAPP_MPCHAT_MAINMENU_OPEN  #��Ƶ����
	endif
endif #MRAPP_SUPPORT

ifeq ($(strip $(LOW_MEMORY_SUPPORT)),32X32)
  ifeq ($(strip $(MAINLCD_SIZE)),240X320)
      MCFLAG_OPT +=  -DIDLE_MAINMENU_LOW_MEMORY_SUPPORT
      MCFLAG_OPT +=  -DMMI_KEYLOCK_FIXEDBG_SUPPORT
  endif	
endif

ifneq ($(strip $(ENG_SUPPORT)),FALSE)
    MCFLAG_OPT		+=  -DENG_SUPPORT
endif

ifneq ($(strip $(BT_DIALER_SUPPORT)),FALSE)
    MCFLAG_OPT		+=  -DMMI_BTDIALER_TRACE_BUFFER
endif

#MCFLAG_OPT += -DACCOUNT_AUTO_ADAPT_BY_SPN_PNN #����������Ӫ�̣�ͨ��spn��pnn����ƥ��
#MCFLAG_OPT += -DMMI_CLEAR_CODE_SUPPORT # clear code
#MCFLAG_OPT += -DDATA_CONNECT_DUAL #dual sim data connection
#MCFLAG_OPT += -DCALL_REC_FILE_SEPERATE #calling record by contacts' name

MCFLAG_OPT +=  -DMMI_RESET_PHONE_AUTO_PIN
# MMI_RESET_PHONE_AUTO_PIN support when #assert

MCFLAG_OPT += -DCALL_AUTO_CONNECT
MCFLAG_OPT +=  -DMMI_VOLTE_SUPPORT
MCFLAG_OPT +=  -DMMI_2G_4G_SUPPORT
MCFLAG_OPT +=  -DBT_TRANSPORT_MODE_SELECTION
#MCFLAG_OPT+=  -DMMI_INDIAN_SOS_SUPPORT
