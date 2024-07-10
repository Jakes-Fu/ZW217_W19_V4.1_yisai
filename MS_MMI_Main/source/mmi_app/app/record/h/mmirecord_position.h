/*****************************************************************************
** File Name:      mmirecord_position.h                                      *
** Author:                                                                   *
** Date:           13/04/2007                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007     gang.tong              Creat
******************************************************************************/
#ifdef MMI_RECORD_SUPPORT

#ifndef _MMIRECORD_POSITION_H_
#define _MMIRECORD_POSITION_H_

#ifdef MMI_GUI_STYLE_TYPICAL
#if defined (MAINLCD_SIZE_176X220)
#define MMIRECORD_PORTRAIT_ANIM_HSPACE      0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      18

#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      8

#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    40
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT            20
#define MMIRECORD_TIME_OWNDRAW_X                            20
#define MMIRECORD_TIME_OWNDRAW_H_X                          14
#define MMIRECORD_TIME_OWNDRAW_Y                            6
#define MMIRECORD_TIME_OWNDRAW_H_Y                          0
#define MMIRECORD_TIME_FONT                                SONG_FONT_36
#define MMIRECORD_FILENAME_FONT                                CAF_FONT_12
#define MMIRECORD_TIME_SPACE                                   18
#elif defined (MAINLCD_SIZE_240X320)
#define MMIRECORD_PORTRAIT_ANIM_HSPACE      0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      20

#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      15

#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    45
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
#define MMIRECORD_TIME_OWNDRAW_X                            35
#define MMIRECORD_TIME_OWNDRAW_H_X                          15
#define MMIRECORD_TIME_OWNDRAW_Y                             6
#define MMIRECORD_TIME_OWNDRAW_H_Y                            6
#define MMIRECORD_TIME_FONT                                SONG_FONT_42
#define MMIRECORD_FILENAME_FONT                                CAF_FONT_16
#define MMIRECORD_TIME_SPACE                                      22
#elif defined (MAINLCD_SIZE_128X64)
#define MMIRECORD_PORTRAIT_ANIM_HSPACE      0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      12

#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      15

#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    45
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
#define MMIRECORD_TIME_OWNDRAW_X                            35
#define MMIRECORD_TIME_OWNDRAW_H_X                            0
#define MMIRECORD_TIME_OWNDRAW_Y                             6
#define MMIRECORD_TIME_OWNDRAW_H_Y                            6
#define MMIRECORD_TIME_FONT                                SONG_FONT_42
#define MMIRECORD_FILENAME_FONT                                CAF_FONT_14
#define MMIRECORD_TIME_SPACE                                      22

#elif defined (MAINLCD_SIZE_240X400)
#define MMIRECORD_PORTRAIT_ANIM_HSPACE      30
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      75

#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      20

#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    50
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
#define MMIRECORD_TIME_OWNDRAW_X                            35
#define MMIRECORD_TIME_OWNDRAW_H_X                          45
#define MMIRECORD_TIME_OWNDRAW_Y                            6
#define MMIRECORD_TIME_OWNDRAW_H_Y                          6
#define MMIRECORD_TIME_FONT                                SONG_FONT_52
#define MMIRECORD_FILENAME_FONT                                CAF_FONT_18
#define MMIRECORD_TIME_SPACE                                      25
#elif defined (MAINLCD_SIZE_320X480)
#define MMIRECORD_PORTRAIT_ANIM_HSPACE                 30
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      60

#define MMIRECORD_LANDSCAPE_ANIM_HSPACE                 10
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      30

#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    110
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT   20
#define MMIRECORD_TIME_OWNDRAW_X                            70
#define MMIRECORD_TIME_OWNDRAW_H_X                          90
#define MMIRECORD_TIME_OWNDRAW_Y                             55
#define MMIRECORD_TIME_OWNDRAW_H_Y                           20
#define MMIRECORD_TIME_FONT                                SONG_FONT_52
#define MMIRECORD_FILENAME_FONT                                CAF_FONT_18
#define MMIRECORD_TIME_SPACE                                      25
#elif  defined (MAINLCD_SIZE_128X160)
	#define MMIRECORD_PORTRAIT_ANIM_HSPACE      0
	#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      18
	#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
	#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      8    
	
	#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    40
	#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT            20
	#define MMIRECORD_TIME_OWNDRAW_X                            32
	#define MMIRECORD_TIME_OWNDRAW_H_X                          92
	#define MMIRECORD_TIME_OWNDRAW_Y                            112
	#define MMIRECORD_TIME_OWNDRAW_H_Y                          57
	#define MMIRECORD_TIME_FONT                                SONG_FONT_36
	#define MMIRECORD_FILENAME_FONT                                CAF_FONT_12

    #define MMIRECORD_POINTER_OWNDRAW_HEIGHT                    114                                                          
    #define MMIRECORD_TIME_SPACE                                       2                                                     
    #define MMIRECORD_TIME_VOLUME_FORM_HSPACE                    90                                                          
    #define MMIRECORD_POINT_OWNDRAW_HSPACE                    10                                                             
    #define MMIRECORD_TIME_OWNDRAW_WIDTH                            128                                                      
    #define MMIRECORD_BUTTON_FORM_X                                              0                                           
    #define MMIRECORD_BUTTON_FORM_Y                                              400                                         
    #define MMIRECORD_BUTTON_FORM_WIDTH                                     320                                              
    #define MMIRECORD_BUTTON_FORM_HEIGHT                                    80                                               
    #define MMIRECORD_ROTATE_POINTER_ANGLE                                 323//pointer ????                             
                                                                                                                             
    //Portrait position.                                                                                                     
    #define MMIRECORD_ANIM_X                                              24                                                 
    #define MMIRECORD_ANIM_Y                                              37                                                 
    #define MMIRECORD_ANIM_W                                              80                                                
    #define MMIRECORD_ANIM_H                                              50                                                 
                                                                                                                             
    //#define MMIRECORD_TIME_OWNDRAW_Y                             118                                                       
    #define MMIRECORD_TIME_OWNDRAW_W                            110                                                          
    #define MMIRECORD_TIME_OWNDRAW_H                             20                                                          
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_X                            0                                                            
    #define MMIRECORD_TIME_NUM_BG_Y                             20                                                           
    #define MMIRECORD_TIME_NUM_BG_W                            128                                                           
    //#define MMIRECORD_TIME_NUM_BG_H                             130                                                        
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_X                            22                                                       
    #define MMIRECORD_RECORD_RECORD_BG_Y                            37                                                       
    #define MMIRECORD_RECORD_RECORD_BG_W                            80                                                      
    #define MMIRECORD_RECORD_RECORD_BG_H                             43                                                      
                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_X                             201                                                        
    #define MMIRECORD_VOLUME_BUTTON_Y                             150                                                        
    #define MMIRECORD_VOLUME_BUTTON_W                             26                                                         
    #define MMIRECORD_VOLUME_BUTTON_H                             26                                                         
                                                                                                                             
    #define MMIRECORD_NEEDLE_BG_X                                     44                                                     
    //#define MMIRECORD_NEEDLE_BG_Y                                     194                                                  
    #define MMIRECORD_NEEDLE_BG_W                                    152                                                     
    #define MMIRECORD_NEEDLE_BG_H                                     64                                                     
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTRE_X                                     112                                                
    //#define MMIRECORD_NEEDLE_CENTRE_Y                                     263                                              
    #define MMIRECORD_NEEDLE_CENTRE_W                                     17                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H                                    17                                                  
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_X                                     53                                                   
    #define MMIRECORD_LEFT_BUTTON_Y                                     245                                                  
    #define MMIRECORD_LEFT_BUTTON_W                                    44                                                    
    #define MMIRECORD_LEFT_BUTTON_H                                     44                                                   
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_X                                     153                                                 
    #define MMIRECORD_RIGHT_BUTTON_Y                                     245                                                 
    #define MMIRECORD_RIGHT_BUTTON_W                                    44                                                   
    #define MMIRECORD_RIGHT_BUTTON_H                                     44                                                  
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_X                                     65                                            
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_X                                     131                                          
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_X                                     18                                            
    #define MMIRECORD_ISTYLE_BACK_BUTTON_Y                                     275                                           
    #define MMIRECORD_ISTYLE_BACK_BUTTON_W                                    28                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H                                     24                                            
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_X                                     194                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_Y                                     275                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_W                                    28                                           
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H                                     24                                          
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_X                                     7                                               
    #define MMIRECORD_PLAY_STOP_BUTTON_Y                                     179                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_W                                    24//18                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H                                     20//18                                          
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_X                                     40                                                  
    //#define MMIRECORD_NAME_LABEL_Y                                     239                                                 
    #define MMIRECORD_NAME_LABEL_W                                    128                                                    
    #define MMIRECORD_NAME_LABEL_H                                     20                                                    
    #define MMIRECORD_NAME_LABEL_Y_OFFSET                         8                                                          
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_X                                     5//4            //???                             
    //#define MMIRECORD_PROGRESS_BG_Y                                     265//232                                           
    #define MMIRECORD_PROGRESS_BG_W                                    118                                                   
    #define MMIRECORD_PROGRESS_BG_H                                     3                                                    
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_X                                     4                                                 
    //#define MMIRECORD_PROGRESS_POINT_Y                                     258                                             
    #define MMIRECORD_PROGRESS_POINT_W                                    4                                                 
    #define MMIRECORD_PROGRESS_POINT_H                                     4                                                
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_X                                     4                                                
    //#define MMIRECORD_PLAY_TIME_LABEL_Y                                     274                                            
    #define MMIRECORD_PLAY_TIME_LABEL_W                                    57                                               
    #define MMIRECORD_PLAY_TIME_LABEL_H                                     14                                               
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_X                                     68                                          
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                                     274                                        
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_W                                    57                                           
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H                                     14                                           
                                                                                                                             
    #define MMIRECORD_PLAY_BG_X                                                 0        //??????                      
    //#define MMIRECORD_PLAY_BG_Y                                                 234                                        
    #define MMIRECORD_PLAY_BG_W                                                128                                           
    #define MMIRECORD_PLAY_BG_H                                                 20                                           
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_X                                                 0  //?????????,??????
    #define MMIRECORD_NEED_REFRESH_BG_Y                                                 (MMI_STATUSBAR_HEIGHT+4)                 
    #define MMIRECORD_NEED_REFRESH_BG_W                                                128                                   
    #define MMIRECORD_NEED_REFRESH_BG_H                                                 (160-MMI_STATUSBAR_HEIGHT-30)        
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTER_POINT_X                                                 120                              
    //#define MMIRECORD_NEEDLE_CENTER_POINT_Y                                                 314                            
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                                                 2                         
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                                                 115                     
                                                                                                                             
                                            
    #define MMIRECORD_TIME_NUM_BG_H                                      80                                                 
    #define MMIRECORD_NEEDLE_BG_Y                                           194                                              
    #define MMIRECORD_NEEDLE_CENTRE_Y                                   263                                                  
    #define MMIRECORD_NAME_LABEL_X                                         0                                                
    #define MMIRECORD_NAME_LABEL_Y                                         0                                               
    #define MMIRECORD_PROGRESS_BG_Y                                     123//232                                             
    #define MMIRECORD_PROGRESS_POINT_Y                               121                                                     
    #define MMIRECORD_PLAY_TIME_LABEL_Y                               127                                                    
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                      127                                                         
    #define MMIRECORD_PLAY_BG_Y                                               120                                            
    #define MMIRECORD_NEEDLE_CENTER_POINT_Y                      314                                                         
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y       115                                                                 
                                                                                                                      
    //Landscape position.                                                                                                    
    #define MMIRECORD_ANIM_H_X                                              4                                               
    #define MMIRECORD_ANIM_H_Y                                              41                                              
    #define MMIRECORD_ANIM_H_W                                              80                                               
    #define MMIRECORD_ANIM_H_H                                              43                                               
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_H_X                            92                                                           
    #define MMIRECORD_TIME_NUM_BG_H_Y                            57                                                         
    #define MMIRECORD_TIME_NUM_BG_H_W                            68                                                         
    #define MMIRECORD_TIME_NUM_BG_H_H                             14                                                        
    #define MMIRECORD_TIME_SPACE_H                                      2                                                    
                                                                                                                             
    #define MMIRECORD_TIME_OWNDRAW_H_W                         90                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_H_X                             290                                                      
    #define MMIRECORD_VOLUME_BUTTON_H_Y                             22                                                       
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_H_X                            0                                                     
    #define MMIRECORD_RECORD_RECORD_BG_H_Y                             20                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_W                            88                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_H                             88                                                    
                                                                                                                             
    //#define MMIRECORD_NEEDLE_BG_H_X                                     167                                                
    //#define MMIRECORD_NEEDLE_BG_H_Y                                     83                                                 
    #define MMIRECORD_NEEDLE_BG_H_W                                    152                                                   
    #define MMIRECORD_NEEDLE_BG_H_H                                     64                                                   
                                                                                                                             
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTRE_H_X                                     123                                            
    //#define MMIRECORD_NEEDLE_CENTRE_H_Y                                     179                                            
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_H_X                                     183                                                
    #define MMIRECORD_LEFT_BUTTON_H_Y                                     156                                                
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_H_X                                     262                                               
    #define MMIRECORD_RIGHT_BUTTON_H_Y                                     156                                               
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_H_X                                     197                                         
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_H_X                                     245                                        
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_X                                     167                                         
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_Y                                     197                                         
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_X                                     291                                       
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_Y                                     197                                       
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_H_X                                     165                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H_Y                                     89                                            
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_H_X                                     180                                               
    //#define MMIRECORD_NAME_LABEL_H_Y                                     159                                               
    #define MMIRECORD_NAME_LABEL_H_W                                    150                                                  
    #define MMIRECORD_NAME_LABEL_H_H                                     15                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_H_X                                     169                                                
    //#define MMIRECORD_PROGRESS_BG_H_Y                                     185                                              
    #define MMIRECORD_PROGRESS_BG_H_W                                    153                                                 
    #define MMIRECORD_PROGRESS_BG_H_H                                     4                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_H_X                                     161                                             
    //#define MMIRECORD_PROGRESS_POINT_H_Y                                     178                                           
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_H_X                                     163                                            
    //#define MMIRECORD_PLAY_TIME_LABEL_H_Y                                     194                                          
    #define MMIRECORD_PLAY_TIME_LABEL_H_W                                    70                                              
    #define MMIRECORD_PLAY_TIME_LABEL_H_H                                     15                                             
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_X                                     230                                        
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                                     194                                      
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                                    80                                          
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_H                                     15                                         
                                                                                                                             
    #define MMIRECORD_PLAY_BG_H_X                                                 159                                        
    //#define MMIRECORD_PLAY_BG_H_Y                                                 154                                      
    #define MMIRECORD_PLAY_BG_H_W                                                161                                         
    #define MMIRECORD_PLAY_BG_H_H                                                 56                                         
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_H_X                                                 0                                  
    #define MMIRECORD_NEED_REFRESH_BG_H_Y                                                 MMI_STATUSBAR_HEIGHT               
    #define MMIRECORD_NEED_REFRESH_BG_H_W                                                320                                 
    #define MMIRECORD_NEED_REFRESH_BG_H_H                                                 (240-MMI_STATUSBAR_HEIGHT-30)      
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                                                 243                          
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                                                 203                          
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                                                 2                       
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                                                 105                   
                                                                                                                             
                                                                                                    
    #define MMIRECORD_NEEDLE_BG_H_X                                         167                                              
    #define MMIRECORD_NEEDLE_BG_H_Y                                          83                                              
    #define MMIRECORD_NEEDLE_CENTRE_H_X                                  123                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H_Y                                 179                                                  
    #define MMIRECORD_NAME_LABEL_H_X                                        170                                              
    #define MMIRECORD_NAME_LABEL_H_Y                                        159                                              
    #define MMIRECORD_PROGRESS_BG_H_Y                                     185                                                
    #define MMIRECORD_PROGRESS_POINT_H_Y                               178                                                   
    #define MMIRECORD_PLAY_TIME_LABEL_H_Y                               194                                                  
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                      194                                                       
    #define MMIRECORD_PLAY_BG_H_Y                                               154                                          
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_X                      243                                                       
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                      203                                                       
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y      90                                                                 
#else
#error
#endif

#else//else MMI_GUI_STYLE_TYPICAL
#ifdef MMI_PDA_SUPPORT//If PDA/else TK-BARPHONE;
#if defined MAINLCD_SIZE_320X480
#define MMIRECORD_PORTRAIT_ANIM_HSPACE                              0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE                        20
#define MMIRECORD_LANDSCAPE_ANIM_HSPACE                             10
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE                       30
#define MMIRECORD_POINTER_OWNDRAW_HEIGHT                            114
#define MMIRECORD_TIME_OWNDRAW_HEIGHT                               40
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT                      20
#define MMIRECORD_TIME_FONT                                         SONG_FONT_52
#define MMIRECORD_FILENAME_FONT                                     CAF_FONT_18
#define MMIRECORD_TIME_SPACE                                        4
#define MMIRECORD_TIME_VOLUME_FORM_HSPACE                           90
#define MMIRECORD_POINT_OWNDRAW_HSPACE                              10
#define MMIRECORD_TIME_OWNDRAW_WIDTH                                254
#define MMIRECORD_BUTTON_FORM_X                                     0
#define MMIRECORD_BUTTON_FORM_Y                                     400
#define MMIRECORD_BUTTON_FORM_WIDTH                                 320
#define MMIRECORD_BUTTON_FORM_HEIGHT                                80
#define MMIRECORD_ROTATE_POINTER_ANGLE                              323//pointer 旋转角度

//portrait position;
#define MMIRECORD_TIME_NUM_BG_X                                     15
#define MMIRECORD_TIME_NUM_BG_Y                                     33
#define MMIRECORD_TIME_NUM_BG_W                                     290

#define MMIRECORD_RECORD_RECORD_BG_Y                                63
#define MMIRECORD_TIME_OWNDRAW_Y                                    179
#define MMIRECORD_VOLUME_BUTTON_Y                                   246
#define MMIRECORD_VOLUME_BUTTON_X                                   273
#define MMIRECORD_NEEDLE_BG_Y                                       275

#define MMIRECORD_NEEDLE_CENTER_POINT_X                             160
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             448

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                      2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      84

#define MMIRECORD_LEFT_BUTTON_Y                                     411

#define MMIRECORD_PLAY_BG_X                                         0
#define MMIRECORD_PLAY_BG_W                                         320

#define MMIRECORD_PLAY_TIME_LABEL_W                                 160
#define MMIRECORD_PLAY_ALL_TIME_LABEL_W                             160

//Landscape position;
#define MMIRECORD_TIME_NUM_BG_H_X                                   8
#define MMIRECORD_TIME_NUM_BG_H_Y                                   29
#define MMIRECORD_TIME_NUM_BG_H_W                                   244

#define MMIRECORD_RECORD_RECORD_BG_H_Y                              91
#define MMIRECORD_TIME_OWNDRAW_H_Y                                  252
#define MMIRECORD_VOLUME_BUTTON_H_Y                                 44
#define MMIRECORD_VOLUME_BUTTON_H_X                                 441//no used;
#define MMIRECORD_NEEDLE_BG_H_Y                                     85

#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                           366
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           258

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                    2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    75

#define MMIRECORD_LEFT_BUTTON_H_Y                                   251

#define MMIRECORD_PLAY_BG_H_X                                       252
#define MMIRECORD_PLAY_BG_H_W                                       228

#define MMIRECORD_PLAY_TIME_LABEL_H_W                               112
#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                           112

#elif defined MAINLCD_SIZE_240X400
#define MMIRECORD_PORTRAIT_ANIM_HSPACE                              0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE                        20
#define MMIRECORD_LANDSCAPE_ANIM_HSPACE                             10
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE                       30
#define MMIRECORD_POINTER_OWNDRAW_HEIGHT                            114
#define MMIRECORD_TIME_OWNDRAW_HEIGHT                               40
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT                      20
#define MMIRECORD_TIME_FONT                                         SONG_FONT_52
#define MMIRECORD_FILENAME_FONT                                     CAF_FONT_13//CAF_FONT_18
#define MMIRECORD_TIME_SPACE                                        2
#define MMIRECORD_TIME_VOLUME_FORM_HSPACE                           90
#define MMIRECORD_POINT_OWNDRAW_HSPACE                              10
#define MMIRECORD_TIME_OWNDRAW_WIDTH                                254
#define MMIRECORD_BUTTON_FORM_X                                     0
#define MMIRECORD_BUTTON_FORM_Y                                     400
#define MMIRECORD_BUTTON_FORM_WIDTH                                 320
#define MMIRECORD_BUTTON_FORM_HEIGHT                                80
#define MMIRECORD_ROTATE_POINTER_ANGLE                              323//pointer 旋转角度

//portrait position;
#define MMIRECORD_TIME_NUM_BG_X                                     13
#define MMIRECORD_TIME_NUM_BG_Y                                     35
#define MMIRECORD_TIME_NUM_BG_W                                     214

#define MMIRECORD_RECORD_RECORD_BG_Y                                63
#define MMIRECORD_TIME_OWNDRAW_Y                                    165
#define MMIRECORD_VOLUME_BUTTON_Y                                   223
#define MMIRECORD_VOLUME_BUTTON_X                                   200
#define MMIRECORD_NEEDLE_BG_Y                                       246

#define MMIRECORD_NEEDLE_CENTER_POINT_X                             120
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             366

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                      2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      84

#define MMIRECORD_LEFT_BUTTON_Y                                     345

#define MMIRECORD_PLAY_BG_X                                         0
#define MMIRECORD_PLAY_BG_W                                         240

#define MMIRECORD_PLAY_TIME_LABEL_W                                 116
#define MMIRECORD_PLAY_ALL_TIME_LABEL_W                             116

//Landscape position;
#define MMIRECORD_TIME_NUM_BG_H_X                                   7
#define MMIRECORD_TIME_NUM_BG_H_Y                                   25
#define MMIRECORD_TIME_NUM_BG_H_W                                   198

#define MMIRECORD_RECORD_RECORD_BG_H_Y                              69
#define MMIRECORD_TIME_OWNDRAW_H_Y                                  185
#define MMIRECORD_VOLUME_BUTTON_H_Y                                 26
#define MMIRECORD_VOLUME_BUTTON_H_X                                 363 //no used;
#define MMIRECORD_NEEDLE_BG_H_Y                                     67

#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                           303
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           187

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                    2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    75

#define MMIRECORD_LEFT_BUTTON_H_Y                                   185

#define MMIRECORD_PLAY_BG_H_X                                       206
#define MMIRECORD_PLAY_BG_H_W                                       195

#define MMIRECORD_PLAY_TIME_LABEL_H_W                               85
#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                           85

#elif defined MAINLCD_SIZE_240X320
#define MMIRECORD_PORTRAIT_ANIM_HSPACE                              0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE                        20
#define MMIRECORD_LANDSCAPE_ANIM_HSPACE                             10
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE                       30
#define MMIRECORD_POINTER_OWNDRAW_HEIGHT                            114
#define MMIRECORD_TIME_OWNDRAW_HEIGHT                               40
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT                      20
#define MMIRECORD_TIME_FONT                                         SONG_FONT_52
#define MMIRECORD_FILENAME_FONT                                     CAF_FONT_13//CAF_FONT_15
#define MMIRECORD_TIME_SPACE                                        2
#define MMIRECORD_TIME_VOLUME_FORM_HSPACE                           90
#define MMIRECORD_POINT_OWNDRAW_HSPACE                              10
#define MMIRECORD_TIME_OWNDRAW_WIDTH                                254
#define MMIRECORD_BUTTON_FORM_X                                     0
#define MMIRECORD_BUTTON_FORM_Y                                     400
#define MMIRECORD_BUTTON_FORM_WIDTH                                 320
#define MMIRECORD_BUTTON_FORM_HEIGHT                                80
#define MMIRECORD_ROTATE_POINTER_ANGLE                              323//pointer 旋转角度

//portrait position;
#define MMIRECORD_TIME_NUM_BG_X                                     13
#define MMIRECORD_TIME_NUM_BG_Y                                     30
#define MMIRECORD_TIME_NUM_BG_W                                     214

#define MMIRECORD_RECORD_RECORD_BG_Y                                42
#define MMIRECORD_TIME_OWNDRAW_Y                                    113
#define MMIRECORD_VOLUME_BUTTON_Y                                   155
#define MMIRECORD_VOLUME_BUTTON_X                                   201
#define MMIRECORD_NEEDLE_BG_Y                                       173

#define MMIRECORD_NEEDLE_CENTER_POINT_X                             120
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             293

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                      2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      84

#define MMIRECORD_LEFT_BUTTON_Y                                     265

#define MMIRECORD_PLAY_BG_X                                         0        //进度条大背景
#define MMIRECORD_PLAY_BG_W                                         240

#define MMIRECORD_PLAY_TIME_LABEL_W                                 116
#define MMIRECORD_PLAY_ALL_TIME_LABEL_W                             116

//Landscape position;
#define MMIRECORD_TIME_NUM_BG_H_X                                   4
#define MMIRECORD_TIME_NUM_BG_H_Y                                   22
#define MMIRECORD_TIME_NUM_BG_H_W                                   155

#define MMIRECORD_RECORD_RECORD_BG_H_Y                              66
#define MMIRECORD_TIME_OWNDRAW_H_Y                                  176
#define MMIRECORD_VOLUME_BUTTON_H_Y                                 25
#define MMIRECORD_VOLUME_BUTTON_H_X                                 290 //no used;
#define MMIRECORD_NEEDLE_BG_H_Y                                     63

#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                           240
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           183

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                    2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    75

#define MMIRECORD_LEFT_BUTTON_H_Y                                   176

#define MMIRECORD_PLAY_BG_H_X                                       159
#define MMIRECORD_PLAY_BG_H_W                                       161

#define MMIRECORD_PLAY_TIME_LABEL_H_W                               70
#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                           80

#elif defined MAINLCD_SIZE_128X160
	#define MMIRECORD_PORTRAIT_ANIM_HSPACE      0
	#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      18
	#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
	#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      8    
	
	#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    40
	#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT            20
	#define MMIRECORD_TIME_OWNDRAW_X                            32
	#define MMIRECORD_TIME_OWNDRAW_H_X                          92
	#define MMIRECORD_TIME_OWNDRAW_Y                            112
	#define MMIRECORD_TIME_OWNDRAW_H_Y                          57
	#define MMIRECORD_TIME_FONT                                SONG_FONT_36
	#define MMIRECORD_FILENAME_FONT                                CAF_FONT_12

    #define MMIRECORD_POINTER_OWNDRAW_HEIGHT                    114                                                          
    #define MMIRECORD_TIME_SPACE                                       2                                                     
    #define MMIRECORD_TIME_VOLUME_FORM_HSPACE                    90                                                          
    #define MMIRECORD_POINT_OWNDRAW_HSPACE                    10                                                             
    #define MMIRECORD_TIME_OWNDRAW_WIDTH                            128                                                      
    #define MMIRECORD_BUTTON_FORM_X                                              0                                           
    #define MMIRECORD_BUTTON_FORM_Y                                              400                                         
    #define MMIRECORD_BUTTON_FORM_WIDTH                                     320                                              
    #define MMIRECORD_BUTTON_FORM_HEIGHT                                    80                                               
    #define MMIRECORD_ROTATE_POINTER_ANGLE                                 323//pointer ????                             
                                                                                                                             
    //Portrait position.                                                                                                     
    #define MMIRECORD_ANIM_X                                              24                                                 
    #define MMIRECORD_ANIM_Y                                              37                                                 
    #define MMIRECORD_ANIM_W                                              80                                                
    #define MMIRECORD_ANIM_H                                              50                                                 
                                                                                                                             
    //#define MMIRECORD_TIME_OWNDRAW_Y                             118                                                       
    #define MMIRECORD_TIME_OWNDRAW_W                            110                                                          
    #define MMIRECORD_TIME_OWNDRAW_H                             20                                                          
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_X                            0                                                            
    #define MMIRECORD_TIME_NUM_BG_Y                             20                                                           
    #define MMIRECORD_TIME_NUM_BG_W                            128                                                           
    //#define MMIRECORD_TIME_NUM_BG_H                             130                                                        
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_X                            22                                                       
    #define MMIRECORD_RECORD_RECORD_BG_Y                            37                                                       
    #define MMIRECORD_RECORD_RECORD_BG_W                            80                                                      
    #define MMIRECORD_RECORD_RECORD_BG_H                             43                                                      
                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_X                             201                                                        
    #define MMIRECORD_VOLUME_BUTTON_Y                             150                                                        
    #define MMIRECORD_VOLUME_BUTTON_W                             26                                                         
    #define MMIRECORD_VOLUME_BUTTON_H                             26                                                         
                                                                                                                             
    #define MMIRECORD_NEEDLE_BG_X                                     44                                                     
    //#define MMIRECORD_NEEDLE_BG_Y                                     194                                                  
    #define MMIRECORD_NEEDLE_BG_W                                    152                                                     
    #define MMIRECORD_NEEDLE_BG_H                                     64                                                     
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTRE_X                                     112                                                
    //#define MMIRECORD_NEEDLE_CENTRE_Y                                     263                                              
    #define MMIRECORD_NEEDLE_CENTRE_W                                     17                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H                                    17                                                  
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_X                                     53                                                   
    #define MMIRECORD_LEFT_BUTTON_Y                                     245                                                  
    #define MMIRECORD_LEFT_BUTTON_W                                    44                                                    
    #define MMIRECORD_LEFT_BUTTON_H                                     44                                                   
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_X                                     153                                                 
    #define MMIRECORD_RIGHT_BUTTON_Y                                     245                                                 
    #define MMIRECORD_RIGHT_BUTTON_W                                    44                                                   
    #define MMIRECORD_RIGHT_BUTTON_H                                     44                                                  
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_X                                     65                                            
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_X                                     131                                          
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_X                                     18                                            
    #define MMIRECORD_ISTYLE_BACK_BUTTON_Y                                     275                                           
    #define MMIRECORD_ISTYLE_BACK_BUTTON_W                                    28                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H                                     24                                            
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_X                                     194                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_Y                                     275                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_W                                    28                                           
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H                                     24                                          
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_X                                     7                                               
    #define MMIRECORD_PLAY_STOP_BUTTON_Y                                     179                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_W                                    24//18                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H                                     20//18                                          
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_X                                     40                                                  
    //#define MMIRECORD_NAME_LABEL_Y                                     239                                                 
    #define MMIRECORD_NAME_LABEL_W                                    128                                                    
    #define MMIRECORD_NAME_LABEL_H                                     20                                                    
    #define MMIRECORD_NAME_LABEL_Y_OFFSET                         8                                                          
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_X                                     5//4            //???                             
    //#define MMIRECORD_PROGRESS_BG_Y                                     265//232                                           
    #define MMIRECORD_PROGRESS_BG_W                                    118                                                   
    #define MMIRECORD_PROGRESS_BG_H                                     3                                                    
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_X                                     4                                                 
    //#define MMIRECORD_PROGRESS_POINT_Y                                     258                                             
    #define MMIRECORD_PROGRESS_POINT_W                                    4                                                 
    #define MMIRECORD_PROGRESS_POINT_H                                     4                                                
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_X                                     4                                                
    //#define MMIRECORD_PLAY_TIME_LABEL_Y                                     274                                            
    #define MMIRECORD_PLAY_TIME_LABEL_W                                    57                                               
    #define MMIRECORD_PLAY_TIME_LABEL_H                                     14                                               
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_X                                     68                                          
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                                     274                                        
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_W                                    57                                           
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H                                     14                                           
                                                                                                                             
    #define MMIRECORD_PLAY_BG_X                                                 0        //??????                      
    //#define MMIRECORD_PLAY_BG_Y                                                 234                                        
    #define MMIRECORD_PLAY_BG_W                                                128                                           
    #define MMIRECORD_PLAY_BG_H                                                 20                                           
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_X                                                 0  //?????????,??????
    #define MMIRECORD_NEED_REFRESH_BG_Y                                                 (MMI_STATUSBAR_HEIGHT+4)                 
    #define MMIRECORD_NEED_REFRESH_BG_W                                                128                                   
    #define MMIRECORD_NEED_REFRESH_BG_H                                                 (160-MMI_STATUSBAR_HEIGHT-30)        
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTER_POINT_X                                                 120                              
    //#define MMIRECORD_NEEDLE_CENTER_POINT_Y                                                 314                            
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                                                 2                         
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                                                 115                     
                                                                                                                             
                                            
    #define MMIRECORD_TIME_NUM_BG_H                                      80                                                 
    #define MMIRECORD_NEEDLE_BG_Y                                           194                                              
    #define MMIRECORD_NEEDLE_CENTRE_Y                                   263                                                  
    #define MMIRECORD_NAME_LABEL_X                                         0                                                
    #define MMIRECORD_NAME_LABEL_Y                                         0                                               
    #define MMIRECORD_PROGRESS_BG_Y                                     123//232                                             
    #define MMIRECORD_PROGRESS_POINT_Y                               121                                                     
    #define MMIRECORD_PLAY_TIME_LABEL_Y                               127                                                    
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                      127                                                         
    #define MMIRECORD_PLAY_BG_Y                                               120                                            
    #define MMIRECORD_NEEDLE_CENTER_POINT_Y                      314                                                         
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y       115                                                                 
                                                                                                                      
    //Landscape position.                                                                                                    
    #define MMIRECORD_ANIM_H_X                                              4                                               
    #define MMIRECORD_ANIM_H_Y                                              41                                              
    #define MMIRECORD_ANIM_H_W                                              80                                               
    #define MMIRECORD_ANIM_H_H                                              43                                               
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_H_X                            92                                                           
    #define MMIRECORD_TIME_NUM_BG_H_Y                            57                                                         
    #define MMIRECORD_TIME_NUM_BG_H_W                            68                                                         
    #define MMIRECORD_TIME_NUM_BG_H_H                             14                                                        
    #define MMIRECORD_TIME_SPACE_H                                      2                                                    
                                                                                                                             
    #define MMIRECORD_TIME_OWNDRAW_H_W                         90                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_H_X                             290                                                      
    #define MMIRECORD_VOLUME_BUTTON_H_Y                             22                                                       
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_H_X                            0                                                     
    #define MMIRECORD_RECORD_RECORD_BG_H_Y                             20                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_W                            88                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_H                             88                                                    
                                                                                                                             
    //#define MMIRECORD_NEEDLE_BG_H_X                                     167                                                
    //#define MMIRECORD_NEEDLE_BG_H_Y                                     83                                                 
    #define MMIRECORD_NEEDLE_BG_H_W                                    152                                                   
    #define MMIRECORD_NEEDLE_BG_H_H                                     64                                                   
                                                                                                                             
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTRE_H_X                                     123                                            
    //#define MMIRECORD_NEEDLE_CENTRE_H_Y                                     179                                            
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_H_X                                     183                                                
    #define MMIRECORD_LEFT_BUTTON_H_Y                                     156                                                
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_H_X                                     262                                               
    #define MMIRECORD_RIGHT_BUTTON_H_Y                                     156                                               
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_H_X                                     197                                         
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_H_X                                     245                                        
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_X                                     167                                         
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_Y                                     197                                         
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_X                                     291                                       
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_Y                                     197                                       
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_H_X                                     165                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H_Y                                     89                                            
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_H_X                                     180                                               
    //#define MMIRECORD_NAME_LABEL_H_Y                                     159                                               
    #define MMIRECORD_NAME_LABEL_H_W                                    150                                                  
    #define MMIRECORD_NAME_LABEL_H_H                                     15                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_H_X                                     169                                                
    //#define MMIRECORD_PROGRESS_BG_H_Y                                     185                                              
    #define MMIRECORD_PROGRESS_BG_H_W                                    153                                                 
    #define MMIRECORD_PROGRESS_BG_H_H                                     4                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_H_X                                     161                                             
    //#define MMIRECORD_PROGRESS_POINT_H_Y                                     178                                           
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_H_X                                     163                                            
    //#define MMIRECORD_PLAY_TIME_LABEL_H_Y                                     194                                          
    #define MMIRECORD_PLAY_TIME_LABEL_H_W                                    70                                              
    #define MMIRECORD_PLAY_TIME_LABEL_H_H                                     15                                             
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_X                                     230                                        
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                                     194                                      
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                                    80                                          
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_H                                     15                                         
                                                                                                                             
    #define MMIRECORD_PLAY_BG_H_X                                                 159                                        
    //#define MMIRECORD_PLAY_BG_H_Y                                                 154                                      
    #define MMIRECORD_PLAY_BG_H_W                                                161                                         
    #define MMIRECORD_PLAY_BG_H_H                                                 56                                         
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_H_X                                                 0                                  
    #define MMIRECORD_NEED_REFRESH_BG_H_Y                                                 MMI_STATUSBAR_HEIGHT               
    #define MMIRECORD_NEED_REFRESH_BG_H_W                                                320                                 
    #define MMIRECORD_NEED_REFRESH_BG_H_H                                                 (240-MMI_STATUSBAR_HEIGHT-30)      
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                                                 243                          
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                                                 203                          
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                                                 2                       
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                                                 105                   
                                                                                                                             
                                                                                                    
    #define MMIRECORD_NEEDLE_BG_H_X                                         167                                              
    #define MMIRECORD_NEEDLE_BG_H_Y                                          83                                              
    #define MMIRECORD_NEEDLE_CENTRE_H_X                                  123                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H_Y                                 179                                                  
    #define MMIRECORD_NAME_LABEL_H_X                                        170                                              
    #define MMIRECORD_NAME_LABEL_H_Y                                        159                                              
    #define MMIRECORD_PROGRESS_BG_H_Y                                     185                                                
    #define MMIRECORD_PROGRESS_POINT_H_Y                               178                                                   
    #define MMIRECORD_PLAY_TIME_LABEL_H_Y                               194                                                  
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                      194                                                       
    #define MMIRECORD_PLAY_BG_H_Y                                               154                                          
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_X                      243                                                       
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                      203                                                       
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y      90
#elif defined MAINLCD_SIZE_176X220
	#define MMIRECORD_PORTRAIT_ANIM_HSPACE      0
	#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      18
	#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
	#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      8    
	
	#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    40
	#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT            20
	#define MMIRECORD_TIME_OWNDRAW_X                            32
	#define MMIRECORD_TIME_OWNDRAW_H_X                          92
	#define MMIRECORD_TIME_OWNDRAW_Y                            112
	#define MMIRECORD_TIME_OWNDRAW_H_Y                          57
	#define MMIRECORD_TIME_FONT                                SONG_FONT_36
	#define MMIRECORD_FILENAME_FONT                                CAF_FONT_12

    #define MMIRECORD_POINTER_OWNDRAW_HEIGHT                    114                                                          
    #define MMIRECORD_TIME_SPACE                                       2                                                     
    #define MMIRECORD_TIME_VOLUME_FORM_HSPACE                    90                                                          
    #define MMIRECORD_POINT_OWNDRAW_HSPACE                    10                                                             
    #define MMIRECORD_TIME_OWNDRAW_WIDTH                            128                                                      
    #define MMIRECORD_BUTTON_FORM_X                                              0                                           
    #define MMIRECORD_BUTTON_FORM_Y                                              400                                         
    #define MMIRECORD_BUTTON_FORM_WIDTH                                     320                                              
    #define MMIRECORD_BUTTON_FORM_HEIGHT                                    80                                               
    #define MMIRECORD_ROTATE_POINTER_ANGLE                                 323//pointer ????                             
                                                                                                                             
    //Portrait position.                                                                                                     
    #define MMIRECORD_ANIM_X                                              24                                                 
    #define MMIRECORD_ANIM_Y                                              37                                                 
    #define MMIRECORD_ANIM_W                                              80                                                
    #define MMIRECORD_ANIM_H                                              50                                                 
                                                                                                                             
    //#define MMIRECORD_TIME_OWNDRAW_Y                             118                                                       
    #define MMIRECORD_TIME_OWNDRAW_W                            110                                                          
    #define MMIRECORD_TIME_OWNDRAW_H                             20                                                          
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_X                            0                                                            
    #define MMIRECORD_TIME_NUM_BG_Y                             20                                                           
    #define MMIRECORD_TIME_NUM_BG_W                            128                                                           
    //#define MMIRECORD_TIME_NUM_BG_H                             130                                                        
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_X                            22                                                       
    #define MMIRECORD_RECORD_RECORD_BG_Y                            37                                                       
    #define MMIRECORD_RECORD_RECORD_BG_W                            80                                                      
    #define MMIRECORD_RECORD_RECORD_BG_H                             43                                                      
                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_X                             201                                                        
    #define MMIRECORD_VOLUME_BUTTON_Y                             150                                                        
    #define MMIRECORD_VOLUME_BUTTON_W                             26                                                         
    #define MMIRECORD_VOLUME_BUTTON_H                             26                                                         
                                                                                                                             
    #define MMIRECORD_NEEDLE_BG_X                                     44                                                     
    //#define MMIRECORD_NEEDLE_BG_Y                                     194                                                  
    #define MMIRECORD_NEEDLE_BG_W                                    152                                                     
    #define MMIRECORD_NEEDLE_BG_H                                     64                                                     
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTRE_X                                     112                                                
    //#define MMIRECORD_NEEDLE_CENTRE_Y                                     263                                              
    #define MMIRECORD_NEEDLE_CENTRE_W                                     17                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H                                    17                                                  
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_X                                     53                                                   
    #define MMIRECORD_LEFT_BUTTON_Y                                     245                                                  
    #define MMIRECORD_LEFT_BUTTON_W                                    44                                                    
    #define MMIRECORD_LEFT_BUTTON_H                                     44                                                   
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_X                                     153                                                 
    #define MMIRECORD_RIGHT_BUTTON_Y                                     245                                                 
    #define MMIRECORD_RIGHT_BUTTON_W                                    44                                                   
    #define MMIRECORD_RIGHT_BUTTON_H                                     44                                                  
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_X                                     65                                            
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_X                                     131                                          
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_X                                     18                                            
    #define MMIRECORD_ISTYLE_BACK_BUTTON_Y                                     275                                           
    #define MMIRECORD_ISTYLE_BACK_BUTTON_W                                    28                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H                                     24                                            
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_X                                     194                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_Y                                     275                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_W                                    28                                           
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H                                     24                                          
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_X                                     7                                               
    #define MMIRECORD_PLAY_STOP_BUTTON_Y                                     179                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_W                                    24//18                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H                                     20//18                                          
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_X                                     40                                                  
    //#define MMIRECORD_NAME_LABEL_Y                                     239                                                 
    #define MMIRECORD_NAME_LABEL_W                                    128                                                    
    #define MMIRECORD_NAME_LABEL_H                                     20                                                    
    #define MMIRECORD_NAME_LABEL_Y_OFFSET                         8                                                          
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_X                                     5//4            //???                             
    //#define MMIRECORD_PROGRESS_BG_Y                                     265//232                                           
    #define MMIRECORD_PROGRESS_BG_W                                    118                                                   
    #define MMIRECORD_PROGRESS_BG_H                                     3                                                    
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_X                                     4                                                 
    //#define MMIRECORD_PROGRESS_POINT_Y                                     258                                             
    #define MMIRECORD_PROGRESS_POINT_W                                    4                                                 
    #define MMIRECORD_PROGRESS_POINT_H                                     4                                                
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_X                                     4                                                
    //#define MMIRECORD_PLAY_TIME_LABEL_Y                                     274                                            
    #define MMIRECORD_PLAY_TIME_LABEL_W                                    57                                               
    #define MMIRECORD_PLAY_TIME_LABEL_H                                     14                                               
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_X                                     68                                          
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                                     274                                        
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_W                                    57                                           
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H                                     14                                           
                                                                                                                             
    #define MMIRECORD_PLAY_BG_X                                                 0        //??????                      
    //#define MMIRECORD_PLAY_BG_Y                                                 234                                        
    #define MMIRECORD_PLAY_BG_W                                                128                                           
    #define MMIRECORD_PLAY_BG_H                                                 20                                           
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_X                                                 0  //?????????,??????
    #define MMIRECORD_NEED_REFRESH_BG_Y                                                 (MMI_STATUSBAR_HEIGHT+4)                 
    #define MMIRECORD_NEED_REFRESH_BG_W                                                128                                   
    #define MMIRECORD_NEED_REFRESH_BG_H                                                 (160-MMI_STATUSBAR_HEIGHT-30)        
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTER_POINT_X                                                 120                              
    //#define MMIRECORD_NEEDLE_CENTER_POINT_Y                                                 314                            
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                                                 2                         
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                                                 115                     
                                                                                                                             
                                            
    #define MMIRECORD_TIME_NUM_BG_H                                      80                                                 
    #define MMIRECORD_NEEDLE_BG_Y                                           194                                              
    #define MMIRECORD_NEEDLE_CENTRE_Y                                   263                                                  
    #define MMIRECORD_NAME_LABEL_X                                         0                                                
    #define MMIRECORD_NAME_LABEL_Y                                         0                                               
    #define MMIRECORD_PROGRESS_BG_Y                                     123//232                                             
    #define MMIRECORD_PROGRESS_POINT_Y                               121                                                     
    #define MMIRECORD_PLAY_TIME_LABEL_Y                               127                                                    
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                      127                                                         
    #define MMIRECORD_PLAY_BG_Y                                               120                                            
    #define MMIRECORD_NEEDLE_CENTER_POINT_Y                      314                                                         
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y       115                                                                 
                                                                                                                      
    //Landscape position.                                                                                                    
    #define MMIRECORD_ANIM_H_X                                              4                                               
    #define MMIRECORD_ANIM_H_Y                                              41                                              
    #define MMIRECORD_ANIM_H_W                                              80                                               
    #define MMIRECORD_ANIM_H_H                                              43                                               
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_H_X                            92                                                           
    #define MMIRECORD_TIME_NUM_BG_H_Y                            57                                                         
    #define MMIRECORD_TIME_NUM_BG_H_W                            68                                                         
    #define MMIRECORD_TIME_NUM_BG_H_H                             14                                                        
    #define MMIRECORD_TIME_SPACE_H                                      2                                                    
                                                                                                                             
    #define MMIRECORD_TIME_OWNDRAW_H_W                         90                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_H_X                             290                                                      
    #define MMIRECORD_VOLUME_BUTTON_H_Y                             22                                                       
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_H_X                            0                                                     
    #define MMIRECORD_RECORD_RECORD_BG_H_Y                             20                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_W                            88                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_H                             88                                                    
                                                                                                                             
    //#define MMIRECORD_NEEDLE_BG_H_X                                     167                                                
    //#define MMIRECORD_NEEDLE_BG_H_Y                                     83                                                 
    #define MMIRECORD_NEEDLE_BG_H_W                                    152                                                   
    #define MMIRECORD_NEEDLE_BG_H_H                                     64                                                   
                                                                                                                             
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTRE_H_X                                     123                                            
    //#define MMIRECORD_NEEDLE_CENTRE_H_Y                                     179                                            
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_H_X                                     183                                                
    #define MMIRECORD_LEFT_BUTTON_H_Y                                     156                                                
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_H_X                                     262                                               
    #define MMIRECORD_RIGHT_BUTTON_H_Y                                     156                                               
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_H_X                                     197                                         
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_H_X                                     245                                        
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_X                                     167                                         
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_Y                                     197                                         
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_X                                     291                                       
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_Y                                     197                                       
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_H_X                                     165                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H_Y                                     89                                            
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_H_X                                     180                                               
    //#define MMIRECORD_NAME_LABEL_H_Y                                     159                                               
    #define MMIRECORD_NAME_LABEL_H_W                                    150                                                  
    #define MMIRECORD_NAME_LABEL_H_H                                     15                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_H_X                                     169                                                
    //#define MMIRECORD_PROGRESS_BG_H_Y                                     185                                              
    #define MMIRECORD_PROGRESS_BG_H_W                                    153                                                 
    #define MMIRECORD_PROGRESS_BG_H_H                                     4                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_H_X                                     161                                             
    //#define MMIRECORD_PROGRESS_POINT_H_Y                                     178                                           
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_H_X                                     163                                            
    //#define MMIRECORD_PLAY_TIME_LABEL_H_Y                                     194                                          
    #define MMIRECORD_PLAY_TIME_LABEL_H_W                                    70                                              
    #define MMIRECORD_PLAY_TIME_LABEL_H_H                                     15                                             
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_X                                     230                                        
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                                     194                                      
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                                    80                                          
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_H                                     15                                         
                                                                                                                             
    #define MMIRECORD_PLAY_BG_H_X                                                 159                                        
    //#define MMIRECORD_PLAY_BG_H_Y                                                 154                                      
    #define MMIRECORD_PLAY_BG_H_W                                                161                                         
    #define MMIRECORD_PLAY_BG_H_H                                                 56                                         
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_H_X                                                 0                                  
    #define MMIRECORD_NEED_REFRESH_BG_H_Y                                                 MMI_STATUSBAR_HEIGHT               
    #define MMIRECORD_NEED_REFRESH_BG_H_W                                                320                                 
    #define MMIRECORD_NEED_REFRESH_BG_H_H                                                 (240-MMI_STATUSBAR_HEIGHT-30)      
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                                                 243                          
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                                                 203                          
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                                                 2                       
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                                                 105                   
                                                                                                                             
                                                                                                    
    #define MMIRECORD_NEEDLE_BG_H_X                                         167                                              
    #define MMIRECORD_NEEDLE_BG_H_Y                                          83                                              
    #define MMIRECORD_NEEDLE_CENTRE_H_X                                  123                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H_Y                                 179                                                  
    #define MMIRECORD_NAME_LABEL_H_X                                        170                                              
    #define MMIRECORD_NAME_LABEL_H_Y                                        159                                              
    #define MMIRECORD_PROGRESS_BG_H_Y                                     185                                                
    #define MMIRECORD_PROGRESS_POINT_H_Y                               178                                                   
    #define MMIRECORD_PLAY_TIME_LABEL_H_Y                               194                                                  
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                      194                                                       
    #define MMIRECORD_PLAY_BG_H_Y                                               154                                          
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_X                      243                                                       
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                      203                                                       
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y      90
#else
#error
#endif // lcd Size

#else//If PDA/else TK-BARPHONE;

#ifdef MAINLCD_SIZE_320X480

#define MMIRECORD_PORTRAIT_ANIM_HSPACE                              0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE                        20
#define MMIRECORD_LANDSCAPE_ANIM_HSPACE                             10
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE                       30
#define MMIRECORD_POINTER_OWNDRAW_HEIGHT                            114
#define MMIRECORD_TIME_OWNDRAW_HEIGHT                               40
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT                      20
#define MMIRECORD_TIME_FONT                                         SONG_FONT_52
#define MMIRECORD_FILENAME_FONT                                     CAF_FONT_18
#define MMIRECORD_TIME_SPACE                                        4
#define MMIRECORD_TIME_VOLUME_FORM_HSPACE                           90
#define MMIRECORD_POINT_OWNDRAW_HSPACE                              10
#define MMIRECORD_TIME_OWNDRAW_WIDTH                                254
#define MMIRECORD_BUTTON_FORM_X                                     0
#define MMIRECORD_BUTTON_FORM_Y                                     400
#define MMIRECORD_BUTTON_FORM_WIDTH                                 320
#define MMIRECORD_BUTTON_FORM_HEIGHT                                80
#define MMIRECORD_ROTATE_POINTER_ANGLE                              323//pointer 旋转角度

//portrait position;
#define MMIRECORD_TIME_NUM_BG_X                                     15
#define MMIRECORD_TIME_NUM_BG_Y                                     33
#define MMIRECORD_TIME_NUM_BG_W                                     290

#define MMIRECORD_RECORD_RECORD_BG_Y                                63
#define MMIRECORD_TIME_OWNDRAW_Y                                    179
#define MMIRECORD_VOLUME_BUTTON_Y                                   246
#define MMIRECORD_VOLUME_BUTTON_X                                   276
#define MMIRECORD_NEEDLE_BG_Y                                       255

#define MMIRECORD_NEEDLE_CENTER_POINT_X                             160
//#define MMIRECORD_NEEDLE_CENTER_POINT_Y                           468//448

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                      2
//#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                    124

#define MMIRECORD_LEFT_BUTTON_Y                                     371

#define MMIRECORD_PLAY_BG_X                                         0
#define MMIRECORD_PLAY_BG_W                                         320

#define MMIRECORD_PLAY_TIME_LABEL_W                                 160
#define MMIRECORD_PLAY_ALL_TIME_LABEL_W                             160

#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             428//448
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      84
#else
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             468//448
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      124
#endif

//Landscape position;
#define MMIRECORD_TIME_NUM_BG_H_X                                   8
#define MMIRECORD_TIME_NUM_BG_H_Y                                   29
#define MMIRECORD_TIME_NUM_BG_H_W                                   244

#define MMIRECORD_RECORD_RECORD_BG_H_Y                              81
#define MMIRECORD_TIME_OWNDRAW_H_Y                                  212
#define MMIRECORD_VOLUME_BUTTON_H_Y                                 44
#define MMIRECORD_VOLUME_BUTTON_H_X                                 4 //no used;
#define MMIRECORD_NEEDLE_BG_H_Y                                     85

#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                           366
//#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                         78

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                    2
//#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                  95

#define MMIRECORD_LEFT_BUTTON_H_Y                                   211

#define MMIRECORD_PLAY_BG_H_X                                       252
#define MMIRECORD_PLAY_BG_H_W                                       228

#define MMIRECORD_PLAY_TIME_LABEL_H_W                               112
#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                           112

#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           258
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    75
#else
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           278
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    95
#define MMIRECORD_NEEDLE_BG_Y                                       295
#define MMIRECORD_NEEDLE_BG_H_Y                                     105
#endif

#elif defined MAINLCD_SIZE_240X400
#define MMIRECORD_PORTRAIT_ANIM_HSPACE                              0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE                        20
#define MMIRECORD_LANDSCAPE_ANIM_HSPACE                             10
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE                       30
#define MMIRECORD_POINTER_OWNDRAW_HEIGHT                            114
#define MMIRECORD_TIME_OWNDRAW_HEIGHT                               40
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT                      20
#define MMIRECORD_TIME_FONT                                         SONG_FONT_52
#define MMIRECORD_FILENAME_FONT                                     CAF_FONT_13//CAF_FONT_18
#define MMIRECORD_TIME_SPACE                                        2
#define MMIRECORD_TIME_VOLUME_FORM_HSPACE                           90
#define MMIRECORD_POINT_OWNDRAW_HSPACE                              10
#define MMIRECORD_TIME_OWNDRAW_WIDTH                                254
#define MMIRECORD_BUTTON_FORM_X                                     0
#define MMIRECORD_BUTTON_FORM_Y                                     400
#define MMIRECORD_BUTTON_FORM_WIDTH                                 320
#define MMIRECORD_BUTTON_FORM_HEIGHT                                80
#define MMIRECORD_ROTATE_POINTER_ANGLE                              323//pointer 旋转角度

//portrait position;
#define MMIRECORD_TIME_NUM_BG_X                                     13
#define MMIRECORD_TIME_NUM_BG_Y                                     35
#define MMIRECORD_TIME_NUM_BG_W                                     214

#define MMIRECORD_RECORD_RECORD_BG_Y                                63
#define MMIRECORD_TIME_OWNDRAW_Y                                    165
#define MMIRECORD_VOLUME_BUTTON_Y                                   228
#define MMIRECORD_VOLUME_BUTTON_X                                   201
#define MMIRECORD_NEEDLE_BG_Y                                       228

#define MMIRECORD_NEEDLE_CENTER_POINT_X                             120
//#define MMIRECORD_NEEDLE_CENTER_POINT_Y                           378//366

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                      2
//#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                    114

#define MMIRECORD_LEFT_BUTTON_Y                                     315

#define MMIRECORD_PLAY_BG_X                                         0
#define MMIRECORD_PLAY_BG_W                                         240

#define MMIRECORD_PLAY_TIME_LABEL_W                                 116
#define MMIRECORD_PLAY_ALL_TIME_LABEL_W                             116

#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             348//366
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      84
#else
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             378//366
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      114
#endif

//Landscape position;
#define MMIRECORD_TIME_NUM_BG_H_X                                   7
#define MMIRECORD_TIME_NUM_BG_H_Y                                   25
#define MMIRECORD_TIME_NUM_BG_H_W                                   198

#define MMIRECORD_RECORD_RECORD_BG_H_Y                              54
#define MMIRECORD_TIME_OWNDRAW_H_Y                                  155
#define MMIRECORD_VOLUME_BUTTON_H_Y                                 25
#define MMIRECORD_VOLUME_BUTTON_H_X                                 363//no used;
#define MMIRECORD_NEEDLE_BG_H_Y                                     52

#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                           303
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           172

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                    2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    75

#define MMIRECORD_LEFT_BUTTON_H_Y                                   155

#define MMIRECORD_PLAY_BG_H_X                                       205
//#define MMIRECORD_PLAY_BG_H_Y                                     154
#define MMIRECORD_PLAY_BG_H_W                                       195

#define MMIRECORD_PLAY_TIME_LABEL_H_W                               85
#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                           85

#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
#else
#define MMIRECORD_NEEDLE_BG_Y                                       258
#define MMIRECORD_NEEDLE_BG_H_Y                                     82
#endif

#elif defined MAINLCD_SIZE_240X320
#define MMIRECORD_PORTRAIT_ANIM_HSPACE                              0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE                        20
#define MMIRECORD_LANDSCAPE_ANIM_HSPACE                             10
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE                       30
#define MMIRECORD_POINTER_OWNDRAW_HEIGHT                            114
#define MMIRECORD_TIME_OWNDRAW_HEIGHT                               40
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT                      20
#define MMIRECORD_TIME_FONT                                         SONG_FONT_52
#define MMIRECORD_FILENAME_FONT                                     CAF_FONT_13 //CAF_FONT_15
#define MMIRECORD_TIME_SPACE                                        2
#define MMIRECORD_TIME_VOLUME_FORM_HSPACE                           90
#define MMIRECORD_POINT_OWNDRAW_HSPACE                              10
#define MMIRECORD_TIME_OWNDRAW_WIDTH                                254
#define MMIRECORD_BUTTON_FORM_X                                     0
#define MMIRECORD_BUTTON_FORM_Y                                     400
#define MMIRECORD_BUTTON_FORM_WIDTH                                 320
#define MMIRECORD_BUTTON_FORM_HEIGHT                                80
#define MMIRECORD_ROTATE_POINTER_ANGLE                              323//pointer 旋转角度

//portrait position;
#define MMIRECORD_TIME_NUM_BG_X                                     13
#define MMIRECORD_TIME_NUM_BG_Y                                     30
#define MMIRECORD_TIME_NUM_BG_W                                     214

#define MMIRECORD_RECORD_RECORD_BG_Y                                42
#define MMIRECORD_TIME_OWNDRAW_Y                                    113
#define MMIRECORD_VOLUME_BUTTON_Y                                   155
#define MMIRECORD_VOLUME_BUTTON_X                                   201
#define MMIRECORD_NEEDLE_BG_Y                                       163

#define MMIRECORD_NEEDLE_CENTER_POINT_X                             120
//#define MMIRECORD_NEEDLE_CENTER_POINT_Y                           314

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                      2
//#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                    115

#define MMIRECORD_LEFT_BUTTON_Y                                     245

#define MMIRECORD_PLAY_BG_X                                         0 //进度条大背景
#define MMIRECORD_PLAY_BG_W                                         240

#define MMIRECORD_PLAY_TIME_LABEL_W                                 116
#define MMIRECORD_PLAY_ALL_TIME_LABEL_W                             116

#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             283
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      84
#else
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             314
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      115
#define MMIRECORD_TIME_OWNDRAW_Y                                    118
#define MMIRECORD_NEEDLE_BG_Y                                       194
#endif

//Landscape position;
#define MMIRECORD_TIME_NUM_BG_H_X                                   4
#define MMIRECORD_TIME_NUM_BG_H_Y                                   22
#define MMIRECORD_TIME_NUM_BG_H_W                                   155

#define MMIRECORD_RECORD_RECORD_BG_H_Y                              56
#define MMIRECORD_TIME_OWNDRAW_H_Y                                  156
#define MMIRECORD_VOLUME_BUTTON_H_Y                                 22
#define MMIRECORD_VOLUME_BUTTON_H_X                                 290//no used;
#define MMIRECORD_NEEDLE_BG_H_Y                                     53

//#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                         243
//#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                         203

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                    2
//#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                  105

#define MMIRECORD_LEFT_BUTTON_H_Y                                   156

#define MMIRECORD_PLAY_BG_H_X                                       159
#define MMIRECORD_PLAY_BG_H_W                                       161

#define MMIRECORD_PLAY_TIME_LABEL_H_W                               70
#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                           80

#ifndef RECORD_BARPHONE_UI_STYLE_SUPPORT
#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                           243
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           173
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    75
#else
#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                           243
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           203
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    90
#define MMIRECORD_NEEDLE_BG_H_Y                                     83
#endif

#elif MAINLCD_SIZE_176X220
#define MMIRECORD_PORTRAIT_ANIM_HSPACE                              0
#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE                        18
#define MMIRECORD_LANDSCAPE_ANIM_HSPACE                             0
#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE                       8

#define MMIRECORD_TIME_OWNDRAW_HEIGHT                               40
#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT                      20

#define MMIRECORD_TIME_FONT                                         SONG_FONT_36
#define MMIRECORD_FILENAME_FONT                                     CAF_FONT_12

#define MMIRECORD_POINTER_OWNDRAW_HEIGHT                            114
#define MMIRECORD_TIME_SPACE                                        2
#define MMIRECORD_TIME_VOLUME_FORM_HSPACE                           90
#define MMIRECORD_POINT_OWNDRAW_HSPACE                              10
#define MMIRECORD_TIME_OWNDRAW_WIDTH                                254
#define MMIRECORD_BUTTON_FORM_X                                     0
#define MMIRECORD_BUTTON_FORM_Y                                     400
#define MMIRECORD_BUTTON_FORM_WIDTH                                 320
#define MMIRECORD_BUTTON_FORM_HEIGHT                                80
#define MMIRECORD_ROTATE_POINTER_ANGLE                              323//pointer 旋转角度                             

//portrait position;
#define MMIRECORD_TIME_NUM_BG_X                                     0
#define MMIRECORD_TIME_NUM_BG_Y                                     24
#define MMIRECORD_TIME_NUM_BG_W                                     176

#define MMIRECORD_RECORD_RECORD_BG_Y                                49
#define MMIRECORD_TIME_OWNDRAW_Y                                    153
#define MMIRECORD_VOLUME_BUTTON_Y                                   5 //no used;
#define MMIRECORD_VOLUME_BUTTON_X                                   5 //no used;
#define MMIRECORD_NEEDLE_BG_Y                                       13 //no used;

#define MMIRECORD_NEEDLE_CENTER_POINT_X                             120
#define MMIRECORD_NEEDLE_CENTER_POINT_Y                             314

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                      2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                      115

#define MMIRECORD_LEFT_BUTTON_Y                                     18 //no used;

#define MMIRECORD_PLAY_BG_X                                         0 //进度条大背景
#define MMIRECORD_PLAY_BG_Y                                         170
#define MMIRECORD_PLAY_BG_W                                         176
#define MMIRECORD_PROGRESS_BG_Y                                     177

#define MMIRECORD_PLAY_TIME_LABEL_W                                 80
#define MMIRECORD_PLAY_ALL_TIME_LABEL_W                             70

//Landscape position;
#define MMIRECORD_TIME_NUM_BG_H_X                                   0
#define MMIRECORD_TIME_NUM_BG_H_Y                                   25
#define MMIRECORD_TIME_NUM_BG_H_W                                   117

#define MMIRECORD_RECORD_RECORD_BG_H_Y                              58
#define MMIRECORD_TIME_OWNDRAW_H_Y                                  78
#define MMIRECORD_VOLUME_BUTTON_H_Y                                 22 //no used;
#define MMIRECORD_VOLUME_BUTTON_H_X                                 4 //no used;
#define MMIRECORD_NEEDLE_BG_H_Y                                     53 //no used;

#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                           243
#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                           203

#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                    2
#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                    90

#define MMIRECORD_LEFT_BUTTON_H_Y                                   39

#define MMIRECORD_PLAY_BG_H_X                                       117
#define MMIRECORD_PLAY_BG_H_Y                                       126
#define MMIRECORD_PLAY_BG_H_W                                       103
#define MMIRECORD_PROGRESS_BG_H_Y                                   133


#define MMIRECORD_PLAY_TIME_LABEL_H_W                               51
#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                           51



#elif defined MAINLCD_SIZE_128X160
	#define MMIRECORD_PORTRAIT_ANIM_HSPACE      0
	#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      18
	#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
	#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      8    
	
	#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    40
	#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT            20
	#define MMIRECORD_TIME_OWNDRAW_X                            32
	#define MMIRECORD_TIME_OWNDRAW_H_X                          92
	#define MMIRECORD_TIME_OWNDRAW_Y                            112
	#define MMIRECORD_TIME_OWNDRAW_H_Y                          57
	#define MMIRECORD_TIME_FONT                                SONG_FONT_36
	#define MMIRECORD_FILENAME_FONT                                CAF_FONT_12

#define MMIRECORD_POINTER_OWNDRAW_HEIGHT                            114
#define MMIRECORD_TIME_SPACE                                        2
#define MMIRECORD_TIME_VOLUME_FORM_HSPACE                           90
#define MMIRECORD_POINT_OWNDRAW_HSPACE                              10
#define MMIRECORD_TIME_OWNDRAW_WIDTH                                128
#define MMIRECORD_BUTTON_FORM_X                                     0
#define MMIRECORD_BUTTON_FORM_Y                                     400
#define MMIRECORD_BUTTON_FORM_WIDTH                                 320
#define MMIRECORD_BUTTON_FORM_HEIGHT                                80
#define MMIRECORD_ROTATE_POINTER_ANGLE                              323//pointer 旋转角度                             

    //Portrait position.                                                                                                     
    #define MMIRECORD_ANIM_X                                              24                                                 
    #define MMIRECORD_ANIM_Y                                              37                                                 
    #define MMIRECORD_ANIM_W                                              80                                                
    #define MMIRECORD_ANIM_H                                              50                                                 
                                                                                                                             
    //#define MMIRECORD_TIME_OWNDRAW_Y                             118                                                       
    #define MMIRECORD_TIME_OWNDRAW_W                            110                                                          
    #define MMIRECORD_TIME_OWNDRAW_H                             20                                                          
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_X                            0                                                            
    #define MMIRECORD_TIME_NUM_BG_Y                             20                                                           
    #define MMIRECORD_TIME_NUM_BG_W                            128                                                           
    //#define MMIRECORD_TIME_NUM_BG_H                             130                                                        
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_X                            24                                                       
    #define MMIRECORD_RECORD_RECORD_BG_Y                            37                                                       
    #define MMIRECORD_RECORD_RECORD_BG_W                            80                                                      
    #define MMIRECORD_RECORD_RECORD_BG_H                             43                                                      
                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_X                             201                                                        
    #define MMIRECORD_VOLUME_BUTTON_Y                             150                                                        
    #define MMIRECORD_VOLUME_BUTTON_W                             26                                                         
    #define MMIRECORD_VOLUME_BUTTON_H                             26                                                         
                                                                                                                             
    #define MMIRECORD_NEEDLE_BG_X                                     44                                                     
    //#define MMIRECORD_NEEDLE_BG_Y                                     194                                                  
    #define MMIRECORD_NEEDLE_BG_W                                    152                                                     
    #define MMIRECORD_NEEDLE_BG_H                                     64                                                     
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTRE_X                                     112                                                
    //#define MMIRECORD_NEEDLE_CENTRE_Y                                     263                                              
    #define MMIRECORD_NEEDLE_CENTRE_W                                     17                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H                                    17                                                  
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_X                                     53                                                   
    #define MMIRECORD_LEFT_BUTTON_Y                                     245                                                  
    #define MMIRECORD_LEFT_BUTTON_W                                    44                                                    
    #define MMIRECORD_LEFT_BUTTON_H                                     44                                                   
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_X                                     153                                                 
    #define MMIRECORD_RIGHT_BUTTON_Y                                     245                                                 
    #define MMIRECORD_RIGHT_BUTTON_W                                    44                                                   
    #define MMIRECORD_RIGHT_BUTTON_H                                     44                                                  
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_X                                     65                                            
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_X                                     131                                          
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_X                                     18                                            
    #define MMIRECORD_ISTYLE_BACK_BUTTON_Y                                     275                                           
    #define MMIRECORD_ISTYLE_BACK_BUTTON_W                                    28                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H                                     24                                            
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_X                                     194                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_Y                                     275                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_W                                    28                                           
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H                                     24                                          
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_X                                     7                                               
    #define MMIRECORD_PLAY_STOP_BUTTON_Y                                     179                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_W                                    24//18                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H                                     20//18                                          
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_X                                     40                                                  
    //#define MMIRECORD_NAME_LABEL_Y                                     239                                                 
    #define MMIRECORD_NAME_LABEL_W                                    128                                                    
    #define MMIRECORD_NAME_LABEL_H                                     20                                                    
    #define MMIRECORD_NAME_LABEL_Y_OFFSET                         8                                                          
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_X                                     5//4            //???                             
    //#define MMIRECORD_PROGRESS_BG_Y                                     265//232                                           
    #define MMIRECORD_PROGRESS_BG_W                                    118                                                   
    #define MMIRECORD_PROGRESS_BG_H                                     3                                                    
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_X                                     4                                                 
    //#define MMIRECORD_PROGRESS_POINT_Y                                     258                                             
    #define MMIRECORD_PROGRESS_POINT_W                                    4                                                 
    #define MMIRECORD_PROGRESS_POINT_H                                     4                                                
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_X                                     4                                                
    //#define MMIRECORD_PLAY_TIME_LABEL_Y                                     274                                            
    #define MMIRECORD_PLAY_TIME_LABEL_W                                    57                                               
    #define MMIRECORD_PLAY_TIME_LABEL_H                                     14                                               
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_X                                     68                                          
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                                     274                                        
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_W                                    57                                           
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H                                     14                                           
                                                                                                                             
    #define MMIRECORD_PLAY_BG_X                                                 0        //??????                      
    //#define MMIRECORD_PLAY_BG_Y                                                 234                                        
    #define MMIRECORD_PLAY_BG_W                                                128                                           
    #define MMIRECORD_PLAY_BG_H                                                 20                                           
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_X                                                 0  //?????????,??????
    #define MMIRECORD_NEED_REFRESH_BG_Y                                                 (MMI_STATUSBAR_HEIGHT+4)                 
    #define MMIRECORD_NEED_REFRESH_BG_W                                                128                                   
    #define MMIRECORD_NEED_REFRESH_BG_H                                                 (160-MMI_STATUSBAR_HEIGHT-30)        
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTER_POINT_X                                                 120                              
    //#define MMIRECORD_NEEDLE_CENTER_POINT_Y                                                 314                            
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                                                 2                         
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                                                 115                     
                                                                                                                             
                                            
    #define MMIRECORD_TIME_NUM_BG_H                                      80                                                 
    #define MMIRECORD_NEEDLE_BG_Y                                           194                                              
    #define MMIRECORD_NEEDLE_CENTRE_Y                                   263                                                  
    #define MMIRECORD_NAME_LABEL_X                                         0                                                
    #define MMIRECORD_NAME_LABEL_Y                                         0                                               
    #define MMIRECORD_PROGRESS_BG_Y                                  123                                             
    #define MMIRECORD_PROGRESS_POINT_Y                               121                                                     
    #define MMIRECORD_PLAY_TIME_LABEL_Y                               127                                                    
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                      127                                                         
    #define MMIRECORD_PLAY_BG_Y                                               120                                            
    #define MMIRECORD_NEEDLE_CENTER_POINT_Y                      314                                                         
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y       115                                                                 
                                                                                                                      
    //Landscape position.                                                                                                    
    #define MMIRECORD_ANIM_H_X                                              4                                               
    #define MMIRECORD_ANIM_H_Y                                              41                                              
    #define MMIRECORD_ANIM_H_W                                              80                                               
    #define MMIRECORD_ANIM_H_H                                              43                                               
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_H_X                            0                                                           
    #define MMIRECORD_TIME_NUM_BG_H_Y                            19                                                         
    #define MMIRECORD_TIME_NUM_BG_H_W                            88                                                         
    #define MMIRECORD_TIME_NUM_BG_H_H                             88                                                        
    #define MMIRECORD_TIME_SPACE_H                                      2                                                    
                                                                                                                             
    #define MMIRECORD_TIME_OWNDRAW_H_W                         90                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_H_X                             290                                                      
    #define MMIRECORD_VOLUME_BUTTON_H_Y                             22                                                       
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_H_X                            4                                                     
    #define MMIRECORD_RECORD_RECORD_BG_H_Y                            41                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_W                            80                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_H                             43                                                    
                                                                                                                             
    //#define MMIRECORD_NEEDLE_BG_H_X                                     167                                                
    //#define MMIRECORD_NEEDLE_BG_H_Y                                     83                                                 
    #define MMIRECORD_NEEDLE_BG_H_W                                    152                                                   
    #define MMIRECORD_NEEDLE_BG_H_H                                     64                                                   
                                                                                                                             
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTRE_H_X                                     123                                            
    //#define MMIRECORD_NEEDLE_CENTRE_H_Y                                     179                                            
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_H_X                                     183                                                
    #define MMIRECORD_LEFT_BUTTON_H_Y                                     156                                                
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_H_X                                     262                                               
    #define MMIRECORD_RIGHT_BUTTON_H_Y                                     156                                               
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_H_X                                     197                                         
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_H_X                                     245                                        
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_X                                     167                                         
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_Y                                     197                                         
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_X                                     291                                       
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_Y                                     197                                       
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_H_X                                     165                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H_Y                                     89                                            
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_H_X                                     180                                               
    //#define MMIRECORD_NAME_LABEL_H_Y                                     159                                               
    #define MMIRECORD_NAME_LABEL_H_W                                    150                                                  
    #define MMIRECORD_NAME_LABEL_H_H                                     15                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_H_X                                     90                                                
    //#define MMIRECORD_PROGRESS_BG_H_Y                                     185                                              
    #define MMIRECORD_PROGRESS_BG_H_W                                    68                                                 
    #define MMIRECORD_PROGRESS_BG_H_H                                     3                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_H_X                                     92                                             
    //#define MMIRECORD_PROGRESS_POINT_H_Y                                     178                                           
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_H_X                                     90                                            
    //#define MMIRECORD_PLAY_TIME_LABEL_H_Y                                     194                                          
    #define MMIRECORD_PLAY_TIME_LABEL_H_W                                    35                                              
    #define MMIRECORD_PLAY_TIME_LABEL_H_H                                    11                                             
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_X                                     124                                        
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                                     194                                      
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                                    35                                          
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_H                                    11                                         
                                                                                                                             
    #define MMIRECORD_PLAY_BG_H_X                                                 88                                        
    //#define MMIRECORD_PLAY_BG_H_Y                                                 154                                      
    #define MMIRECORD_PLAY_BG_H_W                                                71                                         
    #define MMIRECORD_PLAY_BG_H_H                                                 20                                         
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_H_X                                                 0                                  
    #define MMIRECORD_NEED_REFRESH_BG_H_Y                                                 MMI_STATUSBAR_HEIGHT               
    #define MMIRECORD_NEED_REFRESH_BG_H_W                                                160                                 
    #define MMIRECORD_NEED_REFRESH_BG_H_H                                                (128 - MMI_STATUSBAR_HEIGHT - 11)      
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                                                 243                          
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                                                 203                          
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                                                 2                       
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                                                 105                   
                                                                                                                             
                                                                                                    
    #define MMIRECORD_NEEDLE_BG_H_X                                         167                                              
    #define MMIRECORD_NEEDLE_BG_H_Y                                          83                                              
    #define MMIRECORD_NEEDLE_CENTRE_H_X                                  123                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H_Y                                 179                                                  
    #define MMIRECORD_NAME_LABEL_H_X                                        170                                              
    #define MMIRECORD_NAME_LABEL_H_Y                                        159                                              
    #define MMIRECORD_PROGRESS_BG_H_Y                                     94                                                
    #define MMIRECORD_PROGRESS_POINT_H_Y                               178                                                   
    #define MMIRECORD_PLAY_TIME_LABEL_H_Y                               96                                                  
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                      96                                                       
    #define MMIRECORD_PLAY_BG_H_Y                                  88                                          
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_X                      243                                                       
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                      203                                                       
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y      90                                                                 
 
#elif defined MAINLCD_SIZE_176X220
	#define MMIRECORD_PORTRAIT_ANIM_HSPACE      0
	#define MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE      18
	#define MMIRECORD_LANDSCAPE_ANIM_HSPACE      0
	#define MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE      8    
	
	#define MMIRECORD_TIME_OWNDRAW_HEIGHT                    40
	#define MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT            20
	#define MMIRECORD_TIME_OWNDRAW_X                            41
	#define MMIRECORD_TIME_OWNDRAW_H_X                          124
	#define MMIRECORD_TIME_OWNDRAW_Y                            153
	#define MMIRECORD_TIME_OWNDRAW_H_Y                          78
	#define MMIRECORD_TIME_FONT                                SONG_FONT_36
	#define MMIRECORD_FILENAME_FONT                                CAF_FONT_12

    #define MMIRECORD_POINTER_OWNDRAW_HEIGHT                    114                                                          
    #define MMIRECORD_TIME_SPACE                                       2                                                     
    #define MMIRECORD_TIME_VOLUME_FORM_HSPACE                    90                                                          
    #define MMIRECORD_POINT_OWNDRAW_HSPACE                    10                                                             
    #define MMIRECORD_TIME_OWNDRAW_WIDTH                            254                                                      
    #define MMIRECORD_BUTTON_FORM_X                                              0                                           
    #define MMIRECORD_BUTTON_FORM_Y                                              400                                         
    #define MMIRECORD_BUTTON_FORM_WIDTH                                     320                                              
    #define MMIRECORD_BUTTON_FORM_HEIGHT                                    80                                               
    #define MMIRECORD_ROTATE_POINTER_ANGLE                                 323//pointer ????                             
                                                                                                                             
    //Portrait position.                                                                                                     
    #define MMIRECORD_ANIM_X                                              33                                                 
    #define MMIRECORD_ANIM_Y                                              49                                                 
    #define MMIRECORD_ANIM_W                                              110                                                
    #define MMIRECORD_ANIM_H                                              57                                                 
                                                                                                                             
    //#define MMIRECORD_TIME_OWNDRAW_Y                             118                                                       
    #define MMIRECORD_TIME_OWNDRAW_W                            110                                                          
    #define MMIRECORD_TIME_OWNDRAW_H                             20                                                          
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_X                            0                                                            
    #define MMIRECORD_TIME_NUM_BG_Y                             24                                                           
    #define MMIRECORD_TIME_NUM_BG_W                            176                                                           
    //#define MMIRECORD_TIME_NUM_BG_H                             130                                                        
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_X                            33                                                       
    #define MMIRECORD_RECORD_RECORD_BG_Y                            49                                                       
    #define MMIRECORD_RECORD_RECORD_BG_W                            110                                                      
    #define MMIRECORD_RECORD_RECORD_BG_H                             57                                                      
                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_X                             201                                                        
    #define MMIRECORD_VOLUME_BUTTON_Y                             150                                                        
    #define MMIRECORD_VOLUME_BUTTON_W                             26                                                         
    #define MMIRECORD_VOLUME_BUTTON_H                             26                                                         
                                                                                                                             
    #define MMIRECORD_NEEDLE_BG_X                                     44                                                     
    //#define MMIRECORD_NEEDLE_BG_Y                                     194                                                  
    #define MMIRECORD_NEEDLE_BG_W                                    152                                                     
    #define MMIRECORD_NEEDLE_BG_H                                     64                                                     
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTRE_X                                     112                                                
    //#define MMIRECORD_NEEDLE_CENTRE_Y                                     263                                              
    #define MMIRECORD_NEEDLE_CENTRE_W                                     17                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H                                    17                                                  
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_X                                     53                                                   
    #define MMIRECORD_LEFT_BUTTON_Y                                     245                                                  
    #define MMIRECORD_LEFT_BUTTON_W                                    44                                                    
    #define MMIRECORD_LEFT_BUTTON_H                                     44                                                   
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_X                                     153                                                 
    #define MMIRECORD_RIGHT_BUTTON_Y                                     245                                                 
    #define MMIRECORD_RIGHT_BUTTON_W                                    44                                                   
    #define MMIRECORD_RIGHT_BUTTON_H                                     44                                                  
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_X                                     65                                            
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_X                                     131                                          
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_X                                     18                                            
    #define MMIRECORD_ISTYLE_BACK_BUTTON_Y                                     275                                           
    #define MMIRECORD_ISTYLE_BACK_BUTTON_W                                    28                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H                                     24                                            
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_X                                     194                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_Y                                     275                                         
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_W                                    28                                           
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H                                     24                                          
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_X                                     7                                               
    #define MMIRECORD_PLAY_STOP_BUTTON_Y                                     179                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_W                                    24//18                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H                                     20//18                                          
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_X                                     40                                                  
    //#define MMIRECORD_NAME_LABEL_Y                                     239                                                 
    #define MMIRECORD_NAME_LABEL_W                                    176                                                    
    #define MMIRECORD_NAME_LABEL_H                                     23                                                    
    #define MMIRECORD_NAME_LABEL_Y_OFFSET                         8                                                          
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_X                                     6//4            //???                             
    //#define MMIRECORD_PROGRESS_BG_Y                                     265//232                                           
    #define MMIRECORD_PROGRESS_BG_W                                    164                                                   
    #define MMIRECORD_PROGRESS_BG_H                                     3                                                    
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_X                                     7                                                 
    //#define MMIRECORD_PROGRESS_POINT_Y                                     258                                             
    #define MMIRECORD_PROGRESS_POINT_W                                    3                                                 
    #define MMIRECORD_PROGRESS_POINT_H                                     3                                                
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_X                                     4                                                
    //#define MMIRECORD_PLAY_TIME_LABEL_Y                                     274                                            
    #define MMIRECORD_PLAY_TIME_LABEL_W                                    80                                               
    #define MMIRECORD_PLAY_TIME_LABEL_H                                     14                                               
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_X                                     101                                          
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                                     274                                        
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_W                                    70                                           
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H                                     14                                           
                                                                                                                             
    #define MMIRECORD_PLAY_BG_X                                                 0        //??????                      
    //#define MMIRECORD_PLAY_BG_Y                                                 234                                        
    #define MMIRECORD_PLAY_BG_W                                                176                                           
    #define MMIRECORD_PLAY_BG_H                                                 26                                           
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_X                                                 0  //?????????,??????
    #define MMIRECORD_NEED_REFRESH_BG_Y                                                 (MMI_STATUSBAR_HEIGHT + 6)                 
    #define MMIRECORD_NEED_REFRESH_BG_W                                                176                                   
    #define MMIRECORD_NEED_REFRESH_BG_H                                                 (220-MMI_STATUSBAR_HEIGHT-30)        
                                                                                                                             
    #define MMIRECORD_NEEDLE_CENTER_POINT_X                                                 120                              
    //#define MMIRECORD_NEEDLE_CENTER_POINT_Y                                                 314                            
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_X                                                 2                         
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y                                                 115                     
                                                                                                                             
                                            
    #define MMIRECORD_TIME_NUM_BG_H                                      108                                                 
    #define MMIRECORD_NEEDLE_BG_Y                                           194                                              
    #define MMIRECORD_NEEDLE_CENTRE_Y                                   263                                                  
    #define MMIRECORD_NAME_LABEL_X                                         0                                                
    #define MMIRECORD_NAME_LABEL_Y                                         0                                               
    #define MMIRECORD_PROGRESS_BG_Y                                     175//232                                             
    #define MMIRECORD_PROGRESS_POINT_Y                               173                                                     
    #define MMIRECORD_PLAY_TIME_LABEL_Y                               180                                                    
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_Y                      180                                                         
    #define MMIRECORD_PLAY_BG_Y                                               170                                            
    #define MMIRECORD_NEEDLE_CENTER_POINT_Y                      314                                                         
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_Y       115                                                                 
                                                                                                                      
    //Landscape position.                                                                                                    
    #define MMIRECORD_ANIM_H_X                                              36                                               
    #define MMIRECORD_ANIM_H_Y                                              117                                              
    #define MMIRECORD_ANIM_H_W                                              93                                               
    #define MMIRECORD_ANIM_H_H                                              23                                               
                                                                                                                             
    #define MMIRECORD_TIME_NUM_BG_H_X                            0                                                           
    #define MMIRECORD_TIME_NUM_BG_H_Y                             25                                                         
    #define MMIRECORD_TIME_NUM_BG_H_W                            117                                                         
    #define MMIRECORD_TIME_NUM_BG_H_H                             126                                                        
    #define MMIRECORD_TIME_SPACE_H                                      1                                                    
                                                                          

    #define MMIRECORD_TIME_OWNDRAW_H_W                         90                                            
                                                                                                                             
    #define MMIRECORD_VOLUME_BUTTON_H_X                             290                                                      
    #define MMIRECORD_VOLUME_BUTTON_H_Y                             22                                                       
                                                                                                                             
    #define MMIRECORD_RECORD_RECORD_BG_H_X                            4                                                     
    #define MMIRECORD_RECORD_RECORD_BG_H_Y                             58                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_W                            110                                                    
    #define MMIRECORD_RECORD_RECORD_BG_H_H                             57                                                    
                                                                                                                             
    //#define MMIRECORD_NEEDLE_BG_H_X                                     167                                                
    //#define MMIRECORD_NEEDLE_BG_H_Y                                     83                                                 
    #define MMIRECORD_NEEDLE_BG_H_W                                    152                                                   
    #define MMIRECORD_NEEDLE_BG_H_H                                     64                                                   
                                                                                                                             
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTRE_H_X                                     123                                            
    //#define MMIRECORD_NEEDLE_CENTRE_H_Y                                     179                                            
                                                                                                                             
    #define MMIRECORD_LEFT_BUTTON_H_X                                     183                                                
    #define MMIRECORD_LEFT_BUTTON_H_Y                                     156                                                
                                                                                                                             
    #define MMIRECORD_RIGHT_BUTTON_H_X                                     262                                               
    #define MMIRECORD_RIGHT_BUTTON_H_Y                                     156                                               
                                                                                                                             
    #define MMIRECORD_ISTYLE_LEFT_BUTTON_H_X                                     197                                         
    #define MMIRECORD_ISTYLE_RIGHT_BUTTON_H_X                                     245                                        
                                                                                                                             
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_X                                     167                                         
    #define MMIRECORD_ISTYLE_BACK_BUTTON_H_Y                                     197                                         
                                                                                                                             
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_X                                     291                                       
    #define MMIRECORD_ISTYLE_OPTION_BUTTON_H_Y                                     197                                       
                                                                                                                             
    #define MMIRECORD_PLAY_STOP_BUTTON_H_X                                     165                                           
    #define MMIRECORD_PLAY_STOP_BUTTON_H_Y                                     89                                            
                                                                                                                             
    //#define MMIRECORD_NAME_LABEL_H_X                                     180                                               
    //#define MMIRECORD_NAME_LABEL_H_Y                                     159                                               
    #define MMIRECORD_NAME_LABEL_H_W                                    150                                                  
    #define MMIRECORD_NAME_LABEL_H_H                                     15                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_BG_H_X                                     120                                                
    //#define MMIRECORD_PROGRESS_BG_H_Y                                     185                                              
    #define MMIRECORD_PROGRESS_BG_H_W                                    97                                                 
    #define MMIRECORD_PROGRESS_BG_H_H                                     3                                                  
                                                                                                                             
    #define MMIRECORD_PROGRESS_POINT_H_X                                     130                                             
    //#define MMIRECORD_PROGRESS_POINT_H_Y                                     178                                           
                                                                                                                             
    #define MMIRECORD_PLAY_TIME_LABEL_H_X                                     116                                            
    //#define MMIRECORD_PLAY_TIME_LABEL_H_Y                                     194                                          
    #define MMIRECORD_PLAY_TIME_LABEL_H_W                                    51                                              
    #define MMIRECORD_PLAY_TIME_LABEL_H_H                                     15                                             
                                                                                                                             
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_X                                     168                                        
    //#define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                                     194                                      
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_W                                    51                                          
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_H                                     15                                         
                                                                                                                             
    #define MMIRECORD_PLAY_BG_H_X                                                 117                                        
    //#define MMIRECORD_PLAY_BG_H_Y                                                 154                                      
    #define MMIRECORD_PLAY_BG_H_W                                                103                                         
    #define MMIRECORD_PLAY_BG_H_H                                                 26                                         
                                                                                                                             
    #define MMIRECORD_NEED_REFRESH_BG_H_X                     0                                  
    #define MMIRECORD_NEED_REFRESH_BG_H_Y                     (MMI_STATUSBAR_HEIGHT + 8)             
    #define MMIRECORD_NEED_REFRESH_BG_H_W                     220                                 
    #define MMIRECORD_NEED_REFRESH_BG_H_H                     (176 - MMI_STATUSBAR_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - 8)      
                                                                                                                             
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_X                                                 243                          
    //#define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                                                 203                          
                                                                                                                             
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_X                                                 2                       
    //#define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y                                                 105                   
                                                                                                                             
                                                                                                    
    #define MMIRECORD_NEEDLE_BG_H_X                                         167                                              
    #define MMIRECORD_NEEDLE_BG_H_Y                                          83                                              
    #define MMIRECORD_NEEDLE_CENTRE_H_X                                  123                                                 
    #define MMIRECORD_NEEDLE_CENTRE_H_Y                                 179                                                  
    #define MMIRECORD_NAME_LABEL_H_X                                        0                                              
    #define MMIRECORD_NAME_LABEL_H_Y                                        0                                              
    #define MMIRECORD_PROGRESS_BG_H_Y                                     133                                                
    #define MMIRECORD_PROGRESS_POINT_H_Y                               131                                                   
    #define MMIRECORD_PLAY_TIME_LABEL_H_Y                               139                                                  
    #define MMIRECORD_PLAY_ALL_TIME_LABEL_H_Y                      139                                                       
    #define MMIRECORD_PLAY_BG_H_Y                                               126                                          
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_X                      243                                                       
    #define MMIRECORD_NEEDLE_CENTER_POINT_H_Y                      203                                                       
    #define MMIRECORD_NEEDLE_ROTATE_POINT_OFFSET_H_Y      90                                                                 

                                                                        
#else
#error
#endif


#endif // If PDA/else TK-BARPHONE;
#endif//MMI_GUI_STYLE_TYPICAL
#endif//_MMIRECORD_POSITION_H_
#endif//MMI_RECORD_SUPPORT
