

#ifdef FM_SUPPORT
#ifdef MMI_PDA_SUPPORT

#if 0
#ifdef MMI_LOW_MEMORY_RESOURCE
RES_ADD_IMG(FM_PDA_BG,"\\MMI_RES_##\\IMAG\\FM_PDA_BG.jpg",IMG_CMP_JPG_SCI,0)
#else
RES_ADD_IMG(FM_PDA_BG,"\\MMI_RES_##\\IMAG\\FM_PDA_BG.bmp",IMG_CMP_BMP_16_565_RLE,0)    
#endif

RES_ADD_IMG(FM_PDA_HEADSET,"\\MMI_RES_##\\IMAG\\FM_PDA_HEADSET.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_LEFT_GR,"\\MMI_RES_##\\IMAG\\FM_PDA_LEFT_GR.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_LEFT_PR,"\\MMI_RES_##\\IMAG\\FM_PDA_LEFT_PR.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_LEFT_UN,"\\MMI_RES_##\\IMAG\\FM_PDA_LEFT_UN.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_RIGHT_GR,"\\MMI_RES_##\\IMAG\\FM_PDA_RIGHT_GR.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_RIGHT_PR,"\\MMI_RES_##\\IMAG\\FM_PDA_RIGHT_PR.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_RIGHT_UN,"\\MMI_RES_##\\IMAG\\FM_PDA_RIGHT_UN.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_SILENCE,"\\MMI_RES_##\\IMAG\\FM_PDA_SILENCE.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_SILENCE_FOCUS,"\\MMI_RES_##\\IMAG\\FM_PDA_SILENCE_FOCUS.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_SPEAKER,"\\MMI_RES_##\\IMAG\\FM_PDA_SPEAKER.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_SPK_GR,"\\MMI_RES_##\\IMAG\\FM_PDA_SPK_GR.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_SPK_PR,"\\MMI_RES_##\\IMAG\\FM_PDA_SPK_PR.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_SPK_UN,"\\MMI_RES_##\\IMAG\\FM_PDA_SPK_UN.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_VOLUME_FOCUSED,"\\MMI_RES_##\\IMAG\\FM_PDA_VOLUME_FOCUSED.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_VOLUME_GR,"\\MMI_RES_##\\IMAG\\FM_PDA_VOLUME_GR.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_VOLUME_RELEASED,"\\MMI_RES_##\\IMAG\\FM_PDA_VOLUME_RELEASED.bmp",IMG_CMP_BMP_16_565_RLE,0)

RES_ADD_IMG(FM_PDA_ADJ_BUT,"\\MMI_RES_##\\IMAG\\FM_PDA_ADJ_BUT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_IPHONE_ADJ_BUT,"\\MMI_RES_##\\IMAG\\FM_PDA_IPHONE_ADJ_BUT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_0,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_0.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_1,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_2,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_3,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_4,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_5,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_5.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_6,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_6.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_7,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_7.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_8,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_8.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_NUMBER_9,"\\MMI_RES_##\\IMAG\\FM_PDA_NUMBER_9.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_POINT,"\\MMI_RES_##\\IMAG\\FM_PDA_POINT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_SLIDE_BG,"\\MMI_RES_##\\IMAG\\FM_PDA_SLIDE_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_SLIDE_LINE,"\\MMI_RES_##\\IMAG\\FM_PDA_SLIDE_LINE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_SLIDE_SHADOW,"\\MMI_RES_##\\IMAG\\FM_PDA_SLIDE_SHADOW.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_LIST_FOCUSED,"\\MMI_RES_##\\IMAG\\FM_PDA_LIST_FOCUSED.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_LIST_RELEASED,"\\MMI_RES_##\\IMAG\\FM_PDA_LIST_RELEASED.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_PDA_LIST_GRAY,"\\MMI_RES_##\\IMAG\\FM_PDA_LIST_GRAY.bmp",IMG_CMP_BMP_16_565_RLE,0)

RES_ADD_IMG(FM_PDA_RETURN_BUTTON_PRESSED,"\\MMI_RES_##\\IMAG\\FM_PDA_RETURN_BUTTON_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_PDA_MENU_BUTTON_PRESSED,"\\MMI_RES_##\\IMAG\\FM_PDA_MENU_BUTTON_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMG_FM_RECORDER,"\\MMI_RES_##\\IMAG\\IMG_FM_RECORDER.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMG_FM_BACKGROUND_PLAY,"\\MMI_RES_##\\IMAG\\IMG_FM_BACKGROUND_PLAY.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif

RES_ADD_IMG(IMAGE_FM_RETURN_BUTTON_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RETURN_BUTTON_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_RETURN_BUTTON_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RETURN_BUTTON_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_MENU_BUTTON_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_MENU_BUTTON_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_MENU_BUTTON_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_MENU_BUTTON_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_FM_AJUST_NEEDLE,"\\MMI_RES_##\\IMAG\\IMAGE_FM_AJUST_NEEDLE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_AJUST_PAUSE_BG,"\\MMI_RES_##\\IMAG\\IMAGE_FM_AJUST_PAUSE_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_AJUST_PLAY_BG,"\\MMI_RES_##\\IMAG\\IMAGE_FM_AJUST_PLAY_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_AJUST_PAUSE,"\\MMI_RES_##\\IMAG\\IMAGE_FM_AJUST_PAUSE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_AJUST_PLAY,"\\MMI_RES_##\\IMAG\\IMAGE_FM_AJUST_PLAY.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_BG,"\\MMI_RES_##\\IMAG\\IMAGE_FM_BG.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
RES_ADD_IMG(IMAGE_FM_LEFT_DEFAUT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LEFT_DEFAUT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_LEFT_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LEFT_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_LEFT_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LEFT_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_LIST_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LIST_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_LIST_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LIST_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_LIST_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LIST_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_0,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_0.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_1,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_1.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_2,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_2.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_3,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_3.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_4,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_4.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_5,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_5.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_6,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_6.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_7,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_7.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_8,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_8.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_9,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_9.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_POINT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_POINT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_MHZ,"\\MMI_RES_##\\IMAG\\IMAGE_FM_MHZ.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_NUMBER_BG,"\\MMI_RES_##\\IMAG\\IMAGE_FM_NUMBER_BG.png",IMG_CMP_PNG_SABM_RLE|SABM_HOR_STRETCH,0)
RES_ADD_IMG(IMAGE_FM_RIGHT_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RIGHT_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_RIGHT_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RIGHT_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_RIGHT_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RIGHT_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_SILENCE_DEFAUT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_SILENCE_DEFAUT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_SILENCE_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_SILENCE_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_SILENCE_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_SILENCE_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)

RES_ADD_IMG(IMAGE_FM_VOLUME_DEFAUT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_VOLUME_DEFAUT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_PREVIEW_ICON,"\\MMI_RES_##\\IMAG\\IMAGE_FM_PREVIEW_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_VOLUME_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_VOLUME_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_VOLUME_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_VOLUME_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#else
//[[define image infomation
/*
RES_ADD_IMG(FM_ADJ_BUT,"\\MMI_RES_##\\IMAG\\FM_ADJ_BUT.png",IMG_CMP_PNG_ARGB_RLE,0)	
//RES_ADD_IMG(FM_ADJ_FONT_BG,"\\MMI_RES_##\\IMAG\\FM_ADJ_FONT_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_ADJ_LEFT_PR,"\\MMI_RES_##\\IMAG\\FM_ADJ_LEFT_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_ADJ_RIGHT_PR,"\\MMI_RES_##\\IMAG\\FM_ADJ_RIGHT_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_ADJ_RIGHT_GR,"\\MMI_RES_##\\IMAG\\FM_ADJ_RIGHT_GR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_ADJ_RIGHT_UN,"\\MMI_RES_##\\IMAG\\FM_ADJ_RIGHT_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(FM_AUOT_UN,"\\MMI_RES_##\\IMAG\\FM_AUOT_UN.bmp",IMG_CMP_BMP_16_565_RLE,0)
//RES_ADD_IMG(FM_AUTO_UP_UN,"\\MMI_RES_##\\IMAG\\FM_AUTO_UP_UN.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(FM_DOWN_PR,"\\MMI_RES_##\\IMAG\\FM_DOWN_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_DOWN_UN,"\\MMI_RES_##\\IMAG\\FM_DOWN_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_FM_ADJ_BG,"\\MMI_RES_##\\IMAG\\FM_FM_ADJ_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_KPD_BG,"\\MMI_RES_##\\IMAG\\FM_KPD_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_LEFT_PR,"\\MMI_RES_##\\IMAG\\FM_LEFT_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_LEFT_UN,"\\MMI_RES_##\\IMAG\\FM_LEFT_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_RIGHT_PR,"\\MMI_RES_##\\IMAG\\FM_RIGHT_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_RIGHT_UN,"\\MMI_RES_##\\IMAG\\FM_RIGHT_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_SPK_GR,"\\MMI_RES_##\\IMAG\\FM_SPK_GR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_SPK_PR,"\\MMI_RES_##\\IMAG\\FM_SPK_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_SPK_UN,"\\MMI_RES_##\\IMAG\\FM_SPK_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_ADJ_LEFT_UN,"\\MMI_RES_##\\IMAG\\FM_ADJ_LEFT_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_ADJ_LEFT_GR,"\\MMI_RES_##\\IMAG\\FM_ADJ_LEFT_GR.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(FM_RADIO_ICON,"\\MMI_RES_##\\IMAG\\FM_RADIO_ICON.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_UP_GR,"\\MMI_RES_##\\IMAG\\FM_UP_GR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_DOWN_GR,"\\MMI_RES_##\\IMAG\\FM_DOWN_GR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_LEFT_GR,"\\MMI_RES_##\\IMAG\\FM_LEFT_GR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_RIGHT_GR,"\\MMI_RES_##\\IMAG\\FM_RIGHT_GR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_SPEAKER,"\\MMI_RES_##\\IMAG\\FM_SPEAKER.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_HEADSET,"\\MMI_RES_##\\IMAG\\FM_HEADSET.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_UP_PR,"\\MMI_RES_##\\IMAG\\FM_UP_PR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_UP_UN,"\\MMI_RES_##\\IMAG\\FM_UP_UN.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_VOLUME_RELEASED,"\\MMI_RES_##\\IMAG\\FM_VOLUME_RELEASED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_SILENCE,"\\MMI_RES_##\\IMAG\\FM_SILENCE.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_VOLUME_GR,"\\MMI_RES_##\\IMAG\\FM_VOLUME_GR.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_SILENCE_FOCUS,"\\MMI_RES_##\\IMAG\\FM_SILENCE_FOCUS.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(FM_VOLUME_FOCUSED,"\\MMI_RES_##\\IMAG\\FM_VOLUME_FOCUSED.png",IMG_CMP_PNG_ARGB_RLE,0)
*/
RES_ADD_IMG(IMAGE_FM_ADJ_BG,"\\MMI_RES_##\\IMAG\\IMAGE_FM_ADJ_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_ADJ_BUT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_ADJ_BUT.png",IMG_CMP_PNG_ARGB_RLE,0)
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
RES_ADD_IMG(IMAGE_FM_ADJ_LEFT_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_ADJ_LEFT_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_ADJ_LEFT_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_ADJ_LEFT_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_ADJ_LEFT_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_ADJ_LEFT_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_ADJ_RIGHT_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_ADJ_RIGHT_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_ADJ_RIGHT_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_ADJ_RIGHT_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_ADJ_RIGHT_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_ADJ_RIGHT_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#ifndef MMI_FM_MINI_SUPPORT
RES_ADD_IMG(IMAGE_FM_BG,"\\MMI_RES_##\\IMAG\\IMAGE_FM_BG.png",IMG_CMP_PNG_SABM_RLE|SABM_TILE,0)
#endif
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
RES_ADD_IMG(IMAGE_FM_DOWN_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_DOWN_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_DOWN_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_DOWN_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_DOWN_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_DOWN_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
RES_ADD_IMG(IMAGE_FM_LEFT_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LEFT_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_LEFT_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LEFT_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_LEFT_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_LEFT_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_PAUSE_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_PAUSE_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_PAUSE_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_PAUSE_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_PAUSE_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_PAUSE_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_PLAY_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_PLAY_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_PLAY_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_PLAY_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_RIGHT_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RIGHT_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_RIGHT_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RIGHT_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_RIGHT_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RIGHT_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
RES_ADD_IMG(IMAGE_FM_SILENCE_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_SILENCE_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_SILENCE_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_SILENCE_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_UP_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_UP_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_UP_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_UP_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_UP_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_UP_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_VOLUME_DEFAULT,"\\MMI_RES_##\\IMAG\\IMAGE_FM_VOLUME_DEFAULT.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_VOLUME_DISABLED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_VOLUME_DISABLED.png",IMG_CMP_PNG_ARGB_RLE,0)
RES_ADD_IMG(IMAGE_FM_VOLUME_PRESSED,"\\MMI_RES_##\\IMAG\\IMAGE_FM_VOLUME_PRESSED.png",IMG_CMP_PNG_ARGB_RLE,0)
#endif
#endif
//RES_ADD_IMG(IMAGE_FM_RECORD_BG,"\\MMI_RES_##\\IMAG\\IMAGE_FM_RECORD_BG.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMG_FM_SAVE_FREQ,"\\MMI_RES_##\\IMAG\\IMG_FM_SAVE_FREQUENCY.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMG_FM_OPEN_SPEAKER,"\\MMI_RES_##\\IMAG\\IMG_FM_OPEN_SPEAKER.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMG_FM_AUTO_SEARCH_AND_SAVE,"\\MMI_RES_##\\IMAG\\IMG_FM_AUTO_SEARCH_AND_SAVE.png",IMG_CMP_PNG_ARGB_RLE,0)
//RES_ADD_IMG(IMG_FM_CLOSE_SPEAKER,"\\MMI_RES_##\\IMAG\\IMG_FM_CLOSE_SPEAKER.png",IMG_CMP_PNG_ARGB_RLE,0)






//]]define image infomation


//[[define anim infomation
//]]define anim infomation


//[[define string infomation
#if 1
//RES_ADD_STRING(TXT_FM_RADIO,"FM")    //replaced by common ID TXT_FM
//RES_ADD_STRING(TXT_FM_OPTION,"Option")      //case3��defined but not used
//RES_ADD_STRING(TXT_FM_SETTING,"Setting")    //case3��defined but not used
//RES_ADD_STRING(TXT_FM_SENSI,"Sensitivity")   //case3��defined but not used
//RES_ADD_STRING(TXT_FM_SOUNDMODE,"Sound mode")   //case3��defined but not used
#ifdef MMI_FM_MINI_SUPPORT
RES_ADD_STRING(TXT_FM_TITLE,"FM")  //128X160�ֱ���FM���ڵ�TITLE
#endif
#if defined(MMI_FM_NEED_HEADSET) || defined (MMI_WIDGET_FM)
RES_ADD_STRING(TXT_FM_INPUT_HEADSET,"Please input headset!")
#endif
RES_ADD_STRING(TXT_FM_INIT_FAIL,"Init fail, please retry!")
//RES_ADD_STRING(TXT_FM_CLOSE_FM,"Close FM")           //case3��defined but not used
//RES_ADD_STRING(TXT_FM_SAVE_FREQ,"Save frequency")  //replacedby common ID STXT_SAVE
//RES_ADD_STRING(TXT_FM_DEL_FREQ,"Delete frequency")    //case3��defined but not used
//RES_ADD_STRING(TXT_FM_MANUAL_MODE,"Manual mode")   //case3��defined but not used
//RES_ADD_STRING(TXT_FM_PREDEFINED_MODE,"Pre-defined mode")     //case3��defined but not used
//RES_ADD_STRING(TXT_FM_AUTO_MODE,"Auto mode")                  //case3��defined but not used
RES_ADD_STRING(TXT_FM_FULL_AUTO_SEARCH,"Full auto search")
RES_ADD_STRING(TXT_FM_BACKGROUND_PLAY,"Background play")
RES_ADD_STRING(TXT_FM_HEADSET_MODE,"Headset mode")
RES_ADD_STRING(TXT_FM_SPEAKER_MODE,"Speaker mode")
//RES_ADD_STRING(TXT_FM_SENSI_LOW,"Low")     //case3��defined but not used
//RES_ADD_STRING(TXT_FM_SENSI_MIDDLE,"Middle")   //case3��defined but not used
//RES_ADD_STRING(TXT_FM_SENSI_HIGH,"High")   //case3��defined but not used
//RES_ADD_STRING(TXT_FM_NO_PREFREQ,"No pre-defined freq")     //case3��defined but not used
//RES_ADD_STRING(TXT_FM_PREFREQ_FULL,"Pre-defined freq full !")    //case3��defined but not used
//RES_ADD_STRING(TXT_FM_ENABLE_BGPLAY,"Active bgplay")       //case3��defined but not used
//RES_ADD_STRING(TXT_FM_DISABLE_BGPLAY,"Deactive bgplay")        //case3��defined but not used
//RES_ADD_STRING(TXT_FM_SAVE_SUCESS,"Save success")    //case3��defined but not used
//RES_ADD_STRING(TXT_FM_DEL_SUCESS,"Delete success")       //case3��defined but not used
RES_ADD_STRING(TXT_FM_CANNOT_USE,"FM can't use")
#if !defined(MMI_FM_MINI_SUPPORT) && !defined( MMI_MINI_QVGA_UI_SUPPORT)
RES_ADD_STRING(TXT_FM_HEADSET_PLUG_OUT,"FM radio will be closed without headset!")
#endif
//RES_ADD_STRING(TXT_FM_CHANNEL_LIST,"Channel list")  //replaced by common ID TXT_COMM_CHANNEL_LIST
//RES_ADD_STRING(TXT_FM_MANUAL_SEARCH,"Manual search")    //replaced by common ID TXT_MANUAL_SEARCH
//RES_ADD_STRING(TXT_FM_HELP,"Help")  //replaced by common ID  TXT_HELP
RES_ADD_STRING(TXT_FM_SEARCH_CHANNEL,"Search channel...")
RES_ADD_STRING(TXT_FM_NO_CHANNEL,"No channel")
RES_ADD_STRING(TXT_FM_CHANNEL_NOT_EXIST,"This channel not exist!")
RES_ADD_STRING(TXT_FM_PLAY,"Play")
RES_ADD_STRING(TXT_FM_DELETE_CHA_QUERY,"Delete this channel!")
RES_ADD_STRING(TXT_FM_DELETE_ALL_CHA_QUERY,"Delete all channel!")
RES_ADD_STRING(TXT_FM_CHANNEL_NAME,"Channel name")
RES_ADD_STRING(TXT_FM_FREQEUENCY,"Freqeuency(MHZ)")
RES_ADD_STRING(TXT_FM_FREQEUENCY_COLON,"Freqeuency(MHZ):")
RES_ADD_STRING(TXT_FM_CHANNEL_ANME_COLON,"Channel name:")
//RES_ADD_STRING(TXT_FM_SAVA_AS,"Save to") //replaced by common ID TXT_COMM_SAVE_TO
//RES_ADD_STRING(TXT_FM_SAVE_CHANNEL,"Save channel")
RES_ADD_STRING(TXT_FM_OVERWRITE_CHANNEL,"Overwrite old channel?")
RES_ADD_STRING(TXT_HELP_CONTENT,"Help")
#ifdef FM_RDS_SUPPORT
RES_ADD_STRING(TXT_FM_RDS_ENABLE, "Enable RDS")
RES_ADD_STRING(TXT_FM_RDS_DISABLE, "Disable RDS")
#endif
RES_ADD_STRING(TXT_FM_INVALID_FREQ,"Invalid Freqency")
//RES_ADD_STRING(TXT_CONTINUE,"Continue")           //replaced by common ID TXT_COMMON_COMMON_CONTINUE
//RES_ADD_STRING(TXT_FM_RENAME,"Rename")        //case3��defined but not used
//RES_ADD_STRING(TXT_FM_RECORD_FILE_LOCATION,"Record File Storage")  //replaced by common ID STXT_STORE_MEDIUM
//RES_ADD_STRING(TXT_FM_RECORD_FILE_LIST,"Record File List")    //replaced by common ID TXT_COMMON_RECORD_FILE_LIST
//RES_ADD_STRING(TXT_FM_UDISK,"UDISK")             //replaced by common ID TXT_COMMON_UDISK
//RES_ADD_STRING(TXT_FM_SD_CARD,"SD CRAD")  //replaced by common ID TXT_COMMON_SD_CARD
//RES_ADD_STRING(TXT_FM_SUCCESS,"OK")        //case3��defined but not used
#ifdef MMIFM_SUPPORT_FM_RECORD
RES_ADD_STRING(TXT_FM_CANCEL_RECORD,"Cancel Record")
//RES_ADD_STRING(TXT_FM_RECORD_FILE_EXIST,"File already exists")    //replaced by common ID TXT_FILE_EXISTED
RES_ADD_STRING(TXT_FM_RECORD_FILE_NOT_EXIST,"File not exists")
//RES_ADD_STRING(TXT_FM_RECORD_NEW_NAME_TOO_LONG,"new name too long")      //case3��defined but not used
//RES_ADD_STRING(TXT_FM_RECORD_NEW_NAME_INVALID,"Invalid filename")    //replaced by common ID TXT_DC_NAMEINVALID
RES_ADD_STRING(TXT_FM_RECORD_DELETE_FILES,"Delete Old File list")
#endif
#ifdef MMI_FM_TIMER_RECORD_SUPPORT
//RES_ADD_STRING(TXT_NO_SD,"No sd card")  //replaced by common ID TXT_NO_SD_CARD_ALERT
RES_ADD_STRING(TXT_FM_RECORD_TIMER,"Record Timing")
RES_ADD_STRING(TXT_FM_TIME_FREQUENCY,"Time and Frequency")
//RES_ADD_STRING(TXT_FM_START_TIME,"Start time")  //replaced by common ID TXT_START_TIME
RES_ADD_STRING(TXT_FM_RECORD_TIME_LENGTH,"RECORD TIME LENGTH(Min)")
RES_ADD_STRING(TXT_FM_RECORD_FREQUENCY,"RECORD FREQUENCY(Mhz)")
//RES_ADD_STRING(TXT_FM_RECORD_PERIOD,"RECORD PERIOD")     //case3��defined but not used
RES_ADD_STRING(TXT_ENABLE_FM_RECORD,"Enable FM Record")
RES_ADD_STRING(TXT_ENABLE_FM_RECORD_HINT,"Need Enable FM Record")
RES_ADD_STRING(TXT_DISABLE_FM_RECORD,"Disable FM Record")
RES_ADD_STRING(TXT_ENABLE_FM_RECORD_ALREADY,"Enable FM Record Success")
RES_ADD_STRING(TXT_DISABLE_FM_RECORD_ALREADY,"Disable FM Record Success")
RES_ADD_STRING(TXT_INVALID_ENDURE_TIME,"INVALID ENDURE TIME")
//RES_ADD_STRING(TXT_FM_TIMER_SAVE_PROMPT,"Saved,plug in earphone when start to record!")
RES_ADD_STRING(TXT_UDISK_DISABEL_FM_RECORD,"Udisk in use,can not open FM Record!")
#ifdef ATV_SUPPORT
RES_ADD_STRING(TXT_FM_PROMPT_CLOSE_ATV,"Open FM Record will close ATV.continue?")
#endif
#endif
#ifdef MMIFM_SUPPORT_FORMAT_AMR
RES_ADD_STRING(TXT_FM_FORMATWAV,"wav")
RES_ADD_STRING(TXT_FM_FORMATAMR,"AMR")
#endif
#endif
#endif
//]]define string infomation


//[[define data infomation
//]]define data infomation
