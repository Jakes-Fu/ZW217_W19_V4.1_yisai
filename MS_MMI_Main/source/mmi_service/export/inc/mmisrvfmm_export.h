/*****************************************************************************
** File Name:      mmifmm_export.h                                                  *
** Author:                                                                   *
** Date:           06/04/2007                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:     file manager modue                                                                              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007       Robert           Create
******************************************************************************/
/*******************************************************************************
** Full Path Format: 
**      full path = device + ':\' + directory + '\' + filename
**      eg:  "D:\picture\abc.jpg"
**      "D" is device name of Udisk, SD card is represented by "E"
**      "picture" is directory name
**      "abc.jpg" is filename
**      use ":", "\" to seperate device, directory, filename (don't use '/')
**
** ע:
** 1. �ļ������ȡ��ļ��������ȡ��豸�����ȡ�ȫ·�������ȶ���unicode����
******************************************************************************/

#ifndef _MMISRVFMM_EXPORT_H_
#define _MMISRVFMM_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sfs.h"
#include "mmk_type.h"
//#include "guianim.h"
#include "mime_type.h"
#include "mmk_app.h"
//#include "mmi_filetask.h"
#include "mmi_custom_define.h"
#include "mmi_common.h"
//#include "ffs.h"
#if defined(DRM_SUPPORT)
#include "drm_api.h"
#endif


#include "mmi_filemgr.h"
#include "mmifilearray_export.h"
//#include "mmiset.h"
////#include "mmiset_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIFMM_FULL_FILENAME_LEN            MMIFMM_FILE_FILE_NAME_MAX_LEN   // �����豸����\·����\�ļ���
#define MMIFMM_PATHNAME_LEN                 MMIFMM_FILE_FULL_PATH_MAX_LEN   // ·������󳤶�
#define MMIFMM_FILENAME_LEN                 MMIFMM_FILE_FILE_NAME_MAX_LEN   // �ļ�������󳤶�
#define MMIFMM_PATH_NAME_OFFSET             3
        
#define MMIFMM_ELLIPSIS                     (g_mmifmm_ellipsis_wstr)//"\x00\x2e\x00\x2e\x00\x5c"    //"..\"
#define MMIFMM_ELLIPSIS_LEN                 3

#define MMIFMM_ELLIPSIS_3                   (g_mmifmm_ellipsis_3_wstr)//"\x00\x2e\x00\x2e\x00\x2e"    //"..."
#define MMIFMM_ELLIPSIS_3_LEN               3

//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIFMM_CSK2_DEFAULT_DIR             (g_mmifmm_csk2_default_dir_wstr)//"\x00\x43\x00\x73\x00\x74\x00\x61\x00\x72"          // Cstar �洢��Ŀ¼,"Cstar"
#define MMIFMM_CSK2_DEFAULT_DIR_LEN         5       
#endif

#define MMIFMM_PLAY_RING_DEFAULT_VOL   3        

#define MMIFMM_FILE_NUM                     MMIFMM_FILE_MAX_NUM     // �ڴ��б��������

#define MMIFMM_PATH_DEPTH                   MMIFMM_PATH_MAX_DEPTH      // �ɷ���·�������

#define MMIFMM_STORE_PATH_NUM               MMIFMM_STORE_PATH_MAX_NUM      // ����·��������
        
#define MMIMULTIM_FILEDATA_BUFFER_SIZE      (200*1024)      // ͼƬ��ȡ�����buffer size

#define MMIMULTIM_WBMP_BUF_MAX_SIZE         (200*1024)  //WbmpͼƬ��ѹ��buf���ֵ

#ifdef CMCC_UI_STYLE
//"Picture"
#define MMIMULTIM_DIR_PICTURE               (g_mmimultim_picture_dir_wstr)//"\x00\x50\x00\x69\x00\x63\x00\x74\x00\x75\x00\x72\x00\x65\x00\x00"
#define MMIMULTIM_DIR_PIC_LEN               7

//"Photos"
#define MMIMULTIM_DIR_PHOTOS               (g_mmimultim_photos_dir_wstr)//"\x00\x50\x00\x69\x00\x63\x00\x74\x00\x75\x00\x72\x00\x65\x00\x00"
#define MMIMULTIM_DIR_PHOTOS_LEN               6

#else
//"Picture"
#define MMIMULTIM_DIR_PICTURE               (g_mmimultim_picture_dir_wstr)//"\x00\x50\x00\x69\x00\x63\x00\x74\x00\x75\x00\x72\x00\x65\x00\x00"
#define MMIMULTIM_DIR_PIC_LEN               6
#endif

#ifdef BAIDU_DRIVE_SUPPORT
#define MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL (g_mmimultim_gallary_baidudrv_thumbnail_dir_wstr)
#define MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL_LEN 26

#define  MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD (g_mmimultim_gallary_baidudrv_download_dir_wstr)
#define MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD_LEN 24

#define MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD  (g_mmimultim_gallary_baidudrv_upload_dir_wstr)
#define MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD_LEN 22
#endif

//"Music"
#define MMIMULTIM_DIR_MUSIC                 (g_mmimultim_music_dir_wstr)//"\x00\x4d\x00\x75\x00\x73\x00\x69\x00\x63\x00\x00"
#define MMIMULTIM_DIR_MUSIC_LEN             5
//"Movie"
// 1389172: Ϊ�˱�֤�ֱ�����ͼƬ����Ƶ����������Ƶ�ļ�Ҳ������ͼƬ·����
#define MMIMULTIM_DIR_MOVIE                 (g_mmimultim_picture_dir_wstr) //(g_mmimultim_movie_dir_wstr)//"\x00\x4d\x00\x6f\x00\x76\x00\x69\x00\x65\x00\x00"
#define MMIMULTIM_DIR_MOVIE_LEN             6 //5

#ifdef MMI_KING_MOVIE_SUPPORT
//"KingMovie"
#define MMIMULTIM_DIR_KINGMOVIE             (g_mmimultim_kingmovie_dir_wstr)
#define MMIMULTIM_DIR_KINGMOVIE_LEN         9
#endif

//vcard
#if defined MMI_VCARD_SUPPORT
#define MMIMULTIM_DIR_VCARD                 (g_mmimultim_vcard_dir_wstr)
#endif
#ifdef MMI_VCALENDAR_SUPPORT
//vcalendar
#define MMIMULTIM_DIR_VCALENDAR 				(g_mmimultim_vcalendar_dir_wstr)
#endif

//ebook
#define MMIMULTIM_DIR_EBOOK                 (g_mmimultim_ebook_dir_wstr)

//filearray
//ebook
#define MMIMULTIM_DIR_FILEARRAY               (g_file_array_dir_wstr)
//"PB"
#define MMIMULTIM_DIR_PB                    (g_mmimultim_pb_dir_wstr)//"\x00\x50\x00\x42\x00\x00"
#define MMIMULTIM_DIR_PB_LEN                2
//"Alarm"
#define MMIMULTIM_DIR_ALARM                   (g_mmimultim_alarm_dir_wstr)//"\x00\x50\x00\x42\x00\x00"
#define MMIMULTIM_DIR_ALARM_LEN                5
#define MMIMULTIM_OTHER_DEFAULT_DIR          (g_other_dir_default)   
#define MMIMULTIM_OTHER_DEFAULT_DIR_LEN     5

#ifdef MMI_SSL_CERT_SUPPORT
#define MMIMULTIM_DIR_SSL_CERT                   (g_mmimultim_ssl_dir_wstr)//"\x00\x50\x00\x42\x00\x00"
#define MMIMULTIM_DIR_SSL_CERT_LEN          8
#endif

#define MMIMULTIM_DIR_JAVA            (g_mmimultim_java_dir_wstr)   
#define MMMIMULTIM_DIR_JAVA_LEN     4

//"Font"
#define MMIMULTIM_DIR_FONT                 (g_mmimultim_font_dir_wstr)//"\x00\x4d\x00\x6f\x00\x76\x00\x69\x00\x65\x00\x00"
#define MMIMULTIM_DIR_FONT_LEN             4

#define MMIMULTIM_DIR_SYSTEM_CHAR           'S', 'y', 's', 't', 'e', 'm'
#define MMIMULTIM_DIR_SYSTEM                (g_mmimultim_dir_system)   
#define MMIMULTIM_DIR_SYSTEM_LEN            6
        
#define MMIMULTIM_READ_PICTURE_FILE_MAX     (MMIDC_MAX_PHOTO_SIZE)  //please refer to function AllocPictureDataBuf (BLOCK_MEM_POOL_DC)
#define MMIMULTIM_BMP_PREVIEW_MAX_SIZE      (1024*100)              //please refer to GUI
#define MMIMULTIM_JPG_PREVIEW_MAX_SIZE      (MMIDC_MAX_PHOTO_SIZE)  //please refer to MMIDC_MAX_PHOTO_SIZE(mmidc_export.h)
#define MMIMULTIM_MIDI_PLAY_MAX_SIZE        (1024*300)              //max size of midi play, because when playing midi, it first read all file data,
                                                                    //while playing mp3(wave or aac), it read a little data once
//UDISK represented by
//SD Card Type: E:, F:, G:...
#ifdef DUAL_TCARD_SUPPORT
	#define MMI_SDCARD_MAX_NUM 2
#else
	#define MMI_SDCARD_MAX_NUM 1
#endif
#define MMIFMM_DEVICE_NAME_LEN              1
        //":"
#define MMIFILE_COLON                       0x003a
        //"\"
#define MMIFILE_SLASH                       0x005c
        //"."
#define MMIFILE_DOT                         0x002e
        //"*"
#define MMIFILE_STAR                        0x002a
        //" "
#define MMIFILE_SPACE                       0x0020

        //"C"
#define MMIFILE_C_LETTER                    0x0043
        //"D"
#define MMIFILE_D_LETTER                    0x0044
        //"E"
#define MMIFILE_E_LETTER                    0x0045
#if 0
        //"F"
#define MMIFILE_F_LETTER                    0x0046
        //"G"
#define MMIFILE_G_LETTER                    0x0047
        //"H"
#define MMIFILE_H_LETTER                    0x0048
#endif
//fixed by liyan.zhu for bugzilla 2751
// #ifdef MMI_GUI_STYLE_MINISCREEN
//     #define MMIFMM_WIN_TITLE_MAX_LEN            15
// 
// #else
//     //#define MMIFMM_WIN_TITLE_MAX_LEN            7
// #ifdef MMI_FMM_TITLE_BUTTON_SUPPORT
//     #define MMIFMM_WIN_TITLE_MAX_LEN            15
// #else
//     #define MMIFMM_WIN_TITLE_MAX_LEN            8
// #endif
// #endif
#define MMIFMM_WIN_TITLE_MAX_LEN            15
#define MMIFMM_TITLE_LABEL_MARGIN       24//udisk tab ���ڵ�ǰ������ֵ

//#if 1
//�ļ��洢λ�÷���
typedef enum
{
 	MMIFMM_STORE_TYPE_FIXED,     //����ת�����ļ����̶��洢��U�̻�SD����
// 	MMIFMM_STORE_TYPE_PORTABLE,  //����ת�����ļ������ȴ洢��U�̣������浽SD����
    MMIFMM_STORE_TYPE_PREFER,    //ָ�����ȴ洢λ��

    MMIFMM_STORE_TYPE_MAX
}MMIFMM_STORE_TYPE_E;

//ͼƬ��Ϊǽֽ,��¼����ϸ��Ϣ
typedef struct
{
    wchar          full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1];    //wallpaper name
    uint16          full_path_len;
    SFS_DATE_T	    wallpaper_modify_date;
    SFS_TIME_T	    wallpaper_modify_time;
} MMIFMM_AS_WALLPAPER_INFO;

typedef MMIFMM_AS_WALLPAPER_INFO        MMIMULTIM_AS_WALLPAPER_INFO;

        //�ļ�����
typedef enum
{
    MMIMULTIM_FILE_TYPE_NULL,

    MMIMULTIM_FILE_TYPE_PIC,
    MMIMULTIM_FILE_TYPE_MUSIC,
    MMIMULTIM_FILE_TYPE_MOVIE,
    
    MMIMULTIM_FILE_TYPE_MAX
} MMIMULTIM_FILE_TYPE_E;
//typedef MMIFMM_FILE_TYPE_E              MMIMULTIM_FILE_TYPE_E;

//������������
typedef enum
{
    MMIMULTIM_SET_RING_NULL,
    MMIMULTIM_SET_CALL_RING,        //��������
    MMIMULTIM_SET_MSG_RING,         //��Ϣ����
    MMIMULTIM_SET_ALARM_RING,       //��������
    MMIMULTIM_SET_RING_MAX
} MMIFMM_SET_RING_TYPE_E;
typedef MMIFMM_SET_RING_TYPE_E          MMIMULTIM_SET_RING_TYPE_E;

//file information
typedef struct
{
	wchar			    file_name[MMIFILE_FILE_NAME_MAX_LEN+1];
	uint16				file_name_len;
    wchar				device_name[MMIFILE_DEVICE_NAME_MAX_LEN+1];
    uint16				device_name_len;
    uint32				create_time;
    uint32			    file_size;
} MMIMULTIM_FILE_INFO_T;

//disk format
typedef enum
{
    MMIFILE_FORMAT_NULL,

    MMIFILE_FORMAT_FAT12,
    MMIFILE_FORMAT_FAT16,
    MMIFILE_FORMAT_FAT32,
    MMIFILE_FORMAT_AUTO,  
    MMIFILE_FORMAT_OTHER,
    MMIFILE_FORMAT_MAX
}MMIFILE_DEVICE_FORMAT_E;
#if 0
// �豸����
typedef enum
{
    MMI_DEVICE_SYSTEM,    //C�̣�����ʾ���û���ֻ��Ӧ�ñ��������ļ�
    MMI_DEVICE_UDISK,    // U��
    MMI_DEVICE_SDCARD,   // ��1
	MMI_DEVICE_SDCARD_1, //��2
    MMI_DEVICE_NUM = MMI_SDCARD_MAX_NUM + 2
} MMIFILE_DEVICE_E;
#endif
//file information
typedef struct
{
    wchar               file_name[MMIFILE_FILE_NAME_MAX_LEN+1];
    uint16              file_name_len;
    uint32              create_time;
    uint32              file_size;
    wchar               device_name[MMIFILE_DEVICE_NAME_MAX_LEN+1];
    uint16              device_name_len;
    wchar               dir_name[MMIFILE_DIR_NAME_MAX_LEN+1];
    uint16              dir_name_len;
} MMIFILE_FILE_INFO_T;

//this is move from mmi_filemgr.h
//extern const wchar g_file_device_udisk[];
//extern const wchar g_file_device_sdcard[];
//extern const wchar g_file_device_sysname[];

#ifdef BAIDU_DRIVE_SUPPORT
extern wchar g_mmimultim_gallary_baidudrv_thumbnail_dir_wstr[]  ;
extern wchar g_mmimultim_gallary_baidudrv_download_dir_wstr[] ;
extern wchar g_mmimultim_gallary_baidudrv_upload_dir_wstr[] ;
#endif


#ifdef CMCC_UI_STYLE
extern wchar g_mmimultim_photos_dir_wstr[];
#endif
extern wchar g_mmimultim_picture_dir_wstr[];
extern wchar g_mmimultim_music_dir_wstr[];
extern wchar g_mmimultim_movie_dir_wstr[];
#ifdef MMI_KING_MOVIE_SUPPORT
extern wchar g_mmimultim_kingmovie_dir_wstr[];
#endif
extern wchar g_mmimultim_pb_dir_wstr[];
extern wchar g_other_dir_default[];
extern wchar g_mmimultim_dir_system[];
extern wchar g_mmimultim_alarm_dir_wstr[];
extern wchar g_mmimultim_java_dir_wstr[];
extern wchar g_mmimultim_font_dir_wstr[];
extern wchar g_mmimultim_vcard_dir_wstr[];
extern wchar g_mmimultim_ebook_dir_wstr[];
extern wchar g_file_array_dir_wstr[];
#ifdef MMI_VCALENDAR_SUPPORT
extern wchar g_mmimultim_vcalendar_dir_wstr[];
#endif

#ifdef MMI_SSL_CERT_SUPPORT
extern wchar g_mmimultim_ssl_dir_wstr[];
#endif


//#endif
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/



// ͼƬ����
typedef enum
{
    MMIFMM_PICTURE_TYPE_BMP,
    MMIFMM_PICTURE_TYPE_WBMP,
    MMIFMM_PICTURE_TYPE_JPG,
    MMIFMM_PICTURE_TYPE_GIF,
    MMIFMM_PICTURE_TYPE_PNG,
    MMIFMM_PICTURE_TYPE_UNKNOWN
} MMIFMM_PICTURE_TYPE_E;

// ��Դ�������Ĵ��ڵ�״̬����
typedef enum 
{
    MMIFMM_FMI_STATE_BROWSER,       // ���״̬
    MMIFMM_FMI_STATE_MARK,          // ���״̬
    MMIFMM_FMI_STATE_LOOKUP_PATH,   // ����·��״̬
    MMIFMM_FMI_STATE_LOOKUP_FILE,   // �����ļ�״̬
    MMIFMM_FMI_STATE_UNKNOWN        // ��ȷ��״̬
} MMIFMM_FMI_STATE_E;

// ��������
typedef enum
{
    MMIFMM_SORT_TYPE_NAME,  // ������
    MMIFMM_SORT_TYPE_TIME,  // ��ʱ��
    MMIFMM_SORT_TYPE_SIZE,  // ����С
    MMIFMM_SORT_TYPE_TYPE   // ������
} MMIFMM_SORT_TYPE_E;

// �ļ��б�־
typedef enum
{
    MMIFMM_FOLDER_TYPE_NORMAL,      // ��ͨ�ļ���
    MMIFMM_FOLDER_TYPE_PICTURE,     // ͼƬ�ļ���
    MMIFMM_FOLDER_TYPE_MUSIC,       // �����ļ���
    MMIFMM_FOLDER_TYPE_MOVIE,       // Ӱ���ļ���
#ifdef MMI_KING_MOVIE_SUPPORT
    MMIFMM_FOLDER_TYPE_KINGMOVIE,   // ��Ӱ��Ӱ���ļ���
#endif
    MMIFMM_FOLDER_TYPE_EBOOK,       // �������ļ���
    MMIFMM_FOLDER_TYPE_JAVA,        // Java �ļ���
	MMIFMM_FOLDER_TYPE_VCARD,        // Java �ļ���
    MMIFMM_FOLDER_TYPE_NUM     // �����ļ��У�����ʾ
}MMIFMM_FOLDER_TYPE_E;

// �ļ��Ƿ�ѡ��״̬
typedef enum
{
    MMIFMM_FILE_STATE_UNCHECKED,    // δѡ��
    MMIFMM_FILE_STATE_CHECKED,      // ѡ��
    MMIFMM_FILE_STATE_ALL_CHECKED   // ȫѡ��
} MMIFMM_FILE_STATE_E;

// ��������
typedef enum
{
    MMIFMM_READ_TYPE_ONLY_FILE, // ֻ���ļ�
    MMIFMM_READ_TYPE_ONLY_DIR,  // ֻ��Ŀ¼
    MMIFMM_READ_TYPE_FILE_DIR,  // ͬʱ���ļ���Ŀ¼
    MMIFMM_READ_TYPE_FILE_SUBDIR// ֻ���ļ��������ݹ������Ŀ¼���ļ�
} MMIFMM_READ_TYPE_E;

//�첽���ƺ�ɾ�����̵�״̬
typedef enum
{
    MMIFMM_COPYDEL_NON_STATE,   //�޲���״̬
    MMIFMM_COPY_INIT_STATE,     //���Ƴ�ʼ��״̬
    MMIFMM_COPY_QUERY_STATE,    //��ѯ��ǰ����״̬�������²�����
    MMIFMM_COPY_START_STATE,    //���ƿ�ʼ
    MMIFMM_COPY_READ_STATE,     //�첽��ȡ״̬
    MMIFMM_COPY_WRITE_STATE,    //�첽д�ļ�״̬
    MMIFMM_COPY_PROGRESS_STATE, //�첽���ƴ���״̬
    MMIFMM_COPY_END_STATE,      //���ƽ���״̬
    MMIFMM_DEL_INIT_STATE,      //ɾ����ʼ��״̬
    MMIFMM_DEL_QUERY_STATE,     //ɾ����ѯ״̬��������һ������
    MMIFMM_DEL_PROGRESS_STATE,  //ɾ�����ڽ��е�״̬
    MMIFMM_DEL_END_STATE        //ɾ������״̬
} MMIFMM_COPYDEL_STATE_E;
// �ļ�����
typedef enum
{
    MMIFMM_FILE_TYPE_NORMAL,    // ��ͨ�ļ�
    MMIFMM_FILE_TYPE_PICTURE,   // ͼƬ�ļ�
    MMIFMM_FILE_TYPE_MUSIC,     // �����ļ�
    MMIFMM_FILE_TYPE_MOVIE,     // Ӱ���ļ�
    MMIFMM_FILE_TYPE_EBOOK,     // ������
#ifdef  MMI_VCARD_SUPPORT
    MMIFMM_FILE_TYPE_VCARD,     // VCARD
#endif
#ifdef MMI_VCALENDAR_SUPPORT
    MMIFMM_FILE_TYPE_VCALENDAR, // VCALENDAR
#endif
    MMIFMM_FILE_TYPE_SZIP,      // COMѹ���ļ�
    MMIFMM_FILE_TYPE_JAVA,     // JAVA
	MMIFMM_FILE_TYPE_THEME,		// Theme
    MMIFMM_FILE_TYPE_FONT,     // Font
#ifdef DRM_SUPPORT 
    MMIFMM_FILE_TYPE_DRM,       //DRM �ļ�
#endif        
    MMIFMM_FILE_TYPE_BROWSER,   // NF�����֧�ֵ��ļ�
    MMIFMM_FILE_TYPE_URL,   // �����֧��URL�����ļ�
    MMIFMM_FILE_TYPE_MTV,       //MTV����
    MMIFMM_FILE_TYPE_FOLDER,    // �ļ���	
    MMIFMM_FILE_TYPE_NUM        // �ļ�������Ŀ
} MMIFMM_FILE_TYPE_E;

typedef struct 
{
    wchar                       file_name_ptr[MMIFILE_FULL_PATH_MAX_LEN+1];
    uint16                      file_name_len;  //wstr_len of uint8
    uint32                      file_size;
    MMIFMM_FILE_TYPE_E          file_type;      //picture or music or movie
    GUIANIM_TYPE_E              pic_type;       //if file_type is picutre, this variable is used
    MMISRVAUD_RING_FMT_E             music_type;     //if file_type is music, this variable is used
    MMICOM_VIDEO_TYPE_E             movie_type;    
    uint8                       *data_buf_ptr;
    uint32                      data_buf_offset;
}MMIFMM_SELECT_RETURN_T;

typedef MMIFMM_SELECT_RETURN_T          MULTIM_SELECT_RETURN_T;

// �ļ���������
typedef struct mmifmm_detail_data_tag
{
    uint32              time;                           // �޸�ʱ�� 
    uint32              file_size;                      // �ļ���С             
    MMIFMM_FILE_TYPE_E  type;                           // �ļ����ͣ�
    uint16              filename_len;                   // �ļ������ȣ��ַ���
    wchar               filename[MMIFMM_FILENAME_LEN+1];// �ļ���
    uint16              resolution_x;                   // ˮƽ�ֱ��ʣ�����ͼƬ
    uint16              resolution_y;                   // ��ֱ�ֱ��ʣ�����ͼƬ
    uint32              file_num;                       // �ļ���Ŀ
    uint32              folder_num;                     // Ŀ¼��Ŀ
#ifdef DRM_SUPPORT
    DRM_RIGHTS_T        drm_rights;                     //drm ��Ȩ��Ϣ
#endif

} MMIFMM_DETAIL_DATA_T;

// �ļ���Ϣ
typedef struct mmifmm_file_info_tag
{

    uint32                  time;                                   // ����ʱ��
    uint32                  file_size;                              // �ļ���С       
    uint16                  filename_len;                           // �ļ������ȣ��ַ���
    uint16                  pad;                                    // ���
    wchar                   *filename_ptr;//[MMIFMM_FULL_FILENAME_LEN +1];  // �ļ���
    MMIFMM_FILE_STATE_E     state;                                  // �Ƿ�ѡ�У����ڱ��״̬��
    MMIFMM_FILE_TYPE_E      type;                                   // �ļ����ͣ�
} MMIFMM_FILE_INFO_T;

// ����·��������
typedef struct mmifmm_path_info_tag
{
    uint16      path_depth;                         // ·�����
    uint16      pathname_len;                       // ·�������ȣ��ַ���
    uint32      reserved_pos;                       // ������λ��
    wchar       pathname[MMIFMM_PATHNAME_LEN + 1];   // �ļ���
} MMIFMM_PATH_INFO_T;

typedef struct mmifmm_path_data_info_tag
{
    uint16                  num;            // ����·����Ϣ�ĸ���
    uint16                  path_depth;     // ·�����
    uint16                  begin_pos;      // ��ʼλ�� , ���ڱ���·��
    uint16                  end_pos;        // ����λ��
    MMIFMM_PATH_INFO_T      path_info[MMIFMM_STORE_PATH_NUM];
} MMIFMM_PATH_DATA_INFO_T;

// ������Ϣ
typedef struct mmifmm_data_info_tag
{
    MMIFMM_FMI_STATE_E      state;                              // ����״̬
    MMIFMM_FOLDER_TYPE_E    folder;                             // �ļ�������
    MMIFMM_SORT_TYPE_E      sort;                               // ��������
    MMIFMM_FILE_STATE_E     checked;                            // �Ƿ���ѡ��
    MMIFILE_DEVICE_E        device;                             // �豸
    uint8                   path_is_valid;                      // ·���Ƿ���Ч
    uint16                  path_depth;                         // ·�����
    uint16                  pathname_len;                       // ·���ַ�����
    wchar                   pathname[MMIFMM_PATHNAME_LEN+1];    // ʼ�ձ��浱ǰ·��
    uint16                  file_num;                           // �ļ����ݵ�����
    uint16                  folder_num;                         // �ļ������ݵ�����
    uint16                  current_pos;                        // ��ǰ��λ�ã����ڷ���װ��
    uint16                  mark_num;                           // �����
    MMIFMM_FILE_INFO_T      *data[MMIFMM_FILE_NUM];             // ����path_is_valid�Ƿ����0��ȷ��data���ļ��Ƿ����ȫ·��
} MMIFMM_DATA_INFO_T;

// ·������
typedef struct  mmifmm_path_param_tag
{
    uint32      win_id;         //[IN] ֪ͨ�Ĵ���
    uint32      msg_id;         //[IN] ֪ͨ����Ϣ
    wchar       *full_path_ptr; //[OUT] ȫ·�� 
} MMIFMM_PATH_PARAM_T;

//typedef struct mmifmm_findfile_win_param_tag
//{
//    uint16              *find_path_ptr;  //����ȫ·��(Ŀ¼)
//    MMIFMM_DATA_INFO_T  *result_data_ptr;  //���ҽ������λ��
//    uint16              max_result_file_num; //���ҽ����������ļ���
//    uint32              win_id;  //֪ͨ����
//    uint32              msg_id;  //֪ͨ��Ϣ
// } MMIFMM_FINDFILE_WIN_PARAM_T;

//typedef struct mmifmm_createfolder_win_param_tag
//{
//    wchar       *full_path_ptr;  //��ǰĿ¼ȫ·��
//    uint16      new_folder_max_len; //�½�Ŀ¼������ֳ���,ucs2 len
//    wchar       *new_folder_ptr;
//    uint16      *new_folder_len_ptr;
//    uint32      win_id;  //֪ͨ����
//    uint32      msg_id;  //֪ͨ��Ϣ
//} MMIFMM_CREATEFOLDER_WIN_PARAM_T;
// 
//��¼���ƺ�ɾ��������λ�ã��Լ���Ҫ�����һЩ��̬����
typedef struct  mmifmm_copy_del_path_data_info_tag
{
    uint16                  path_depth;     //��¼��ǰĿ¼�����
    wchar                   cur_pathname[MMIFMM_PATHNAME_LEN +1];
    wchar                   copyfile_name[MMIFMM_PATHNAME_LEN +1];  //��Ҫ���Ƶ��ļ�����
    wchar                   cur_target_path[MMIFMM_PATHNAME_LEN +1];
    wchar                   copy_choose_path[MMIFMM_PATHNAME_LEN +1];  //�����û�ѡ��·��
    wchar                   user_scr_path[MMIFMM_PATHNAME_LEN +1];  //�����û���ʼ·��
    MMIFMM_PATH_INFO_T      cur_path_info;
    MMIFMM_PATH_INFO_T      next_path_info;
    MMIFMM_PATH_INFO_T      target_path_info;
    uint32                  win_id;                 //֪ͨ�Ĵ���
    uint32                  msg_id;                 //֪ͨ����Ϣ
    MMIFMM_COPYDEL_STATE_E  copy_state;
    SFS_HANDLE              sfs_find_handle;
    SFS_FIND_DATA_T         sfs_find_data;
    SFS_OVERLAPPED_T        s_r_overlapped;
    SFS_OVERLAPPED_T        s_w_overlapped;
    uint16                  search_folder_flag;     //��������Ƿ��Ѿ������ļ��л����ļ�������
    uint16                  copy_or_search_state;   //��¼��ǰ���ڲ��һ��Ǹ��Ƶ�״̬
    uint16                  mark_flag;              //��¼�Ƿ����Ѿ���ǹ����ļ�
    uint16                  cur_mark_num;           //��ǰ���ڲ������б�λ��
    uint16                  select_file_num;        // ѡ���ɾ�������ļ�����
    uint16                  completed_file_num;     // ���ɾ�����Ƶ��ļ�����
}MMIFMM_COPY_DEL_PATH_DATA_INFO_T;


//typedef struct mmifmm_rename_win_param_tag
//{
//    wchar       *full_path_ptr; //ȫ·��,����֮ǰ�����֣�����֮���ٱ��浽���
//    MMIFMM_FILE_TYPE_E     type;      //�ļ�����
//    uint32      win_id;         //֪ͨ����
//    uint32      msg_id;         //֪ͨ��Ϣ
//    uint32      max_file_lan;   //����Ŀ¼����󳤶�
// } MMIFMM_RENAME_WIN_PARAM_T;

//fixed by liyan.zhu for bugzilla 2760


typedef void (*MMIFMM_OpenWinCallBack)(MMIFMM_DATA_INFO_T *list_data_ptr,uint32 index);

typedef struct
{
    MMI_TEXT_ID_T leftsoft_id;
    MMI_TEXT_ID_T middlesoft_id;
    MMI_TEXT_ID_T rightsoft_id;   
}MMIFMM_SOFTKEY_TEXT_T;
typedef struct
{
    MMIFMM_OpenWinCallBack callback_ok;
    MMIFMM_OpenWinCallBack callback_web;
    MMIFMM_OpenWinCallBack callback_can;
}MMIFMM_OPENWIN_CALLBACK_T;

typedef struct
{
    wchar               * path_ptr;
    uint16              path_len;
    wchar               * filename;
    uint16              file_len;
    BOOLEAN             is_need_display;
    uint32              find_suffix_type;
    BOOLEAN             privacy_protect;
    MMIFMM_SOFTKEY_TEXT_T sk_text_id;
    MMIFMM_OPENWIN_CALLBACK_T callback;
}MMIFMM_OPENWIN_INFO_T;

typedef enum
{
    MMUFMM_BT_SEND_OPT,
    MMIFMM_DELETE_OPT,
    MMIFMM_DETAIL_OPT,
    MMIFMM_SUB_MARK_OPT,
    MMIFMM_CANCEL_MARK_OPT,
    MMIFMM_MARK_ALL_OPT,
    MMIFMM_CANCEL_ALL_MARK_OPT,
    MMIFMM_RENAME_OPT,
    MMIFMM_COPY_OPT,
    MMIFMM_MAX_OPT
} MMIFMM_OPTION_TYPE_E;

typedef struct
{
	const wchar      filepath_name[MMIFMM_FULL_FILENAME_LEN+1];    //�����ļ���ȫ·�������ļ�����
	uint16           filepath_len;		    //ȫ·������
	uint32		     file_size;             //�ļ��Ĵ�С
	BOOLEAN          is_temp_file;		
}MMIFMM_BT_SENDFILE_INFO_T;

//fixed by liyan.zhu for bugzilla 2760 move support
#ifndef MMI_FMM_MINI_SUPPORT
#define MMI_FMM_MOVE_SUPPORT 1
#endif

//fixed by liyan.zhu for bugzilla 2760 2012-05-22 fmm search support
#define MMI_FMM_SEARCH_SUPPORT 1

//#ifdef MMI_FMM_MOVE_SUPPORT
//typedef struct
//{
//    wchar    file_name[MMIFMM_FULL_FILENAME_LEN +1];
//    wchar    path_name[MMIFMM_PATHNAME_LEN + 1];
//}MMIFMM_MOVE_DATA_INFO_T;
// #endif

/*MMIFMM_SEARCH_FILE_INFO_T: ���������ݽṹ
getKey��һ��callback���������һ��32λ��Ψһֵ����ֵ��ÿ���ļ����ļ���һһ��Ӧ
�ṩ���ļ�ϵͳ�������Ч�ʣ�һ����˵��sfsֻ��עgetkey�ķ���ֵ��ֻ��������ͬ��key
���ļ�����ȥload��Щ�ļ���Ϣ������comp����ȥ���������ıȽ�,
��xfind_param->sort_type == FILEARRAY_SORT_TYPE_CUST ʱ�������������ʵ�֣�Ĭ��ΪPNULL*/
//typedef struct
//{
//    FILEARRAY           file_array;     
//    FILEARRAY_SORT_E    sort_type;      //����ʽ
//    BOOLEAN             is_recurse;   //IN:�Ƿ���Ҫ�ݹ�����
//    MMI_WIN_ID_T        win_id;         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
//    MMI_MESSAGE_ID_E    msg_id;
//    SFS_XFIND_GETKEY    getKey;			
//    SFS_XFIND_COMPARE   compareFunc;
// }MMIFMM_SEARCH_FILE_INFO_T;

extern const wchar g_mmifmm_ellipsis_wstr[];
extern const wchar g_mmifmm_ellipsis_3_wstr[];
//fixed by liyan.zhu for bugzilla 2760
#if 0
extern const wchar g_mmifmm_csk2_default_dir_wstr[];
#endif

#define FMM_FILTER_STRING_MAXLEN            120//240
#define FMM_FILTER_MAX_NUM                  10
#define FMM_SEARCH_FILENAME_MAXLEN          MMIFILE_FULL_PATH_MAX_LEN
#define MMIFMM_SIZE_STR_LEN                 30
#define MMIFMM_INVALID_INDEX                0xFFFFFFFF

//MMI_FMM_MOVE_SUPPORT��Ӧ����MMIFMM_XMOVE_SUPPORT��
//��MMIFMM_XMOVE_SUPPORTδ���壬��MMI_FMM_MOVE_SUPPORT����ʱ��move����ʹ�õ���MMI
//��copy&del�����������ļ�move�����MMI_FMM_MOVE_SUPPORTҲ���ˣ���move���ܱ��ر�

//#define MMI_FMM_MOVE_SUPPORT 1

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    FMM_OPENWIN_RET_NULL,

    FMM_OPENWIN_SUCCESS,
    FMM_OPENWIN_FAIL,
    FMM_OPENWIN_FILE_NOT_EXIST
}MMIFMM_OPENWIN_RET_E;

typedef enum
{
    FUNC_FIND_NULL, 

    FUNC_FIND_FILE,                 //�����ļ�
    FUNC_FIND_FOLDER,               //�����ļ���
    FUNC_FIND_FILE_AND_FOLDER,      //�����ļ����ļ���
    FUNC_FIND_FILE_AND_ALLFOLER,    //�����ļ��������ļ���
    FUNC_FIND_FOLDERS_WITH_SUBFILES  //�����ļ��Ͱ����ļ����ļ��У������ļ���Ҫ�������ļ������
}MMIFMM_FUNC_TYPE_E;

typedef enum
{
    WIN_DISP_NULL,  

    WIN_DISP_1LINE,     //�����ı���ʽ
    WIN_DISP_2LINE      //˫������ͼ��ʽ
}MMIFMM_WINDISP_TYPE_E;

//�����ļ��õ�ɸѡ�ṹ
typedef struct 
{
    //����ɸѡ�Ĺؼ��ַ���, ��"*.mp3"; ���ҿ����ж���ؼ��ַ����������ÿո����, ��"*.mp3 *.mid *.wav"
    //"*" ��ʾ�г������ļ����ļ���
    uint8   filter_str[FMM_FILTER_STRING_MAXLEN+2]; 
}MMIFMM_FILTER_T;

typedef enum
{
    MMIFMM_SELECT_MAIN_TYPE,
    MMIFMM_SELECT_SECOND_TYPE,
    MMIFMM_SELECT_MAX_TYPE
}MMIFMM_SELECT_TYPE_E;

typedef struct 
{
    //MMIFMM_DEVICE_E              select_device; //udisk or sd card or both
    uint32                       select_file[MMIFMM_SELECT_MAX_TYPE];//select music type, one bit present one type,//0xff represent all music type
    uint32                       max_size;//allow music max size, if maxsize = 0, represent no limited
    MMI_WIN_ID_T                 win_id;//win id which need to be returned
    const wchar                  *ticked_file_wstr;//file need to be ticked on
    uint16                       ticked_file_len;//file name len
    int32                        ring_vol;      //ring vol number
}MMIFMM_SELECT_INFO_T;


//�ص���������, is_success��ʾ��������Ƿ�ɹ�, ����� FILEARRAY �б�ķ�ʽ����
typedef void (*MMIFMM_OpenFileCallBack)(
                                        BOOLEAN     is_success,
                                        FILEARRAY   file_array
                                        );

typedef BOOLEAN (*MMIFMM_SelectFileCallBack)(
                                            uint16             total_mark_num,
                                            FILEARRAY_DATA_T   * file_data_ptr
                                          );
typedef struct
{
    wchar                   *path_ptr;      //Ĭ�ϴ�Ŀ¼�������NULL����Ӹ�Ŀ¼��
    uint16                  path_len;
    MMIFMM_FILTER_T         *filter_ptr;    //ɸѡ����
    MMIFMM_FUNC_TYPE_E      func_type;      //����
    BOOLEAN                 is_multi_sel;   //�Ƿ��ѡ
    BOOLEAN                 is_sel_folder; //�Ƿ�ѡ���ļ��У�����s_is_sel_folder
    BOOLEAN                 is_internal_call;
    PROCESSMSG_FUNC         handle_func;    //�Զ��崦����
    FILEARRAY               ret_array;      //�������
    MMIFMM_WINDISP_TYPE_E   disp_type;      //display type
    MMIFMM_OpenFileCallBack callback;        //�ص�����
    MMIFMM_SelectFileCallBack select_cbk; //��Ա��״̬��callback,�����ЩӦ��Ҫ�Ա�ǵ��ļ����ļ��н��в���,
                                            //���û���������󣬴�PNULL
}MMIFMM_OPEN_LAYER_WIN_INFO_T;

#define MMIFMM_FILE_TYPE(type)         (type & 0xFF000000)//24-31 bits:file type 
#define MMIFMM_MEDIA_TYPE(type)        (type & 0x00FFFFFF) //0-23 bits:media type ,22,23 only for drm
#define MMIFMM_UNKNOW_TYPE              0x00000000      //unknuow type
#define MMIFMM_SUFFIX_BASE              0x00000001      //base type //0x00000001
#define MMIFMM_FILE_TYPE_BASE           (MMIFMM_SUFFIX_BASE << 24)      //base type //0x00000001

//picture type
#define MMIFMM_PICTURE_GIF              ((MMIFMM_FILE_TYPE_PICTURE * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE<<0))//0x00000002
#define MMIFMM_PICTURE_BMP              ((MMIFMM_FILE_TYPE_PICTURE * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE<<1))//0x00000004
#define MMIFMM_PICTURE_WBMP             ((MMIFMM_FILE_TYPE_PICTURE * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE<<2))//0x00000008
#define MMIFMM_PICTURE_PNG              ((MMIFMM_FILE_TYPE_PICTURE * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE<<3))//0x00000010
#define MMIFMM_PICTURE_JPEG             ((MMIFMM_FILE_TYPE_PICTURE * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE<<4))//0x00000020
#define MMIFMM_PICTURE_JPG              MMIFMM_PICTURE_JPEG//((MMIFMM_FILE_TYPE_PICTURE * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE<<5))
//drm type
//#ifdef DRM_SUPPORT 
#define MMIFMM_DM_FILE                  (MMIFMM_SUFFIX_BASE<<22)//0x00400000
#define MMIFMM_DCF_FILE                 (MMIFMM_SUFFIX_BASE<<23)//0x00800000
//#endif
//music type 
#define MMIFMM_MUSIC_MP3                ((MMIFMM_FILE_TYPE_MUSIC * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<0)) //0x00000100
#define MMIFMM_MUSIC_WMA                ((MMIFMM_FILE_TYPE_MUSIC * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<1)) //0x00000200
#define MMIFMM_MUSIC_MID                ((MMIFMM_FILE_TYPE_MUSIC * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<2))//0x00000400
#define MMIFMM_MUSIC_MIDI               MMIFMM_MUSIC_MID
#define MMIFMM_MUSIC_AMR                ((MMIFMM_FILE_TYPE_MUSIC * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<3))//0x00000800
#define MMIFMM_MUSIC_ACC                ((MMIFMM_FILE_TYPE_MUSIC * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<4))//0x00001000
#define MMIFMM_MUSIC_M4A                ((MMIFMM_FILE_TYPE_MUSIC * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<5))//0x00002000
#define MMIFMM_MUSIC_WAV                ((MMIFMM_FILE_TYPE_MUSIC * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<6))//0x00004000

// Ӱ���ļ�
#define MMIFMM_MOVIE_RMVB               ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<0))//0x00008000
#define MMIFMM_MOVIE_FLV                ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<1))//0x00010000
#define MMIFMM_MOVIE_MP4                ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<2))//0x00020000
#define MMIFMM_MOVIE_3GP                ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<3))//0x00040000
#define MMIFMM_MOVIE_AVI                ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<4))//0x01000000
#ifdef MMI_KING_MOVIE_SUPPORT
#define MMIFMM_MOVIE_KMV                ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<5))//32λ�����ˣ���ʱ��������ɡ�
#define MMIFMM_MOVIE_SMV                ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<6))
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
#define MMIFMM_MOVIE_HMV                ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<7))
#endif
#endif
//������
#define MMIFMM_MOVIE_TXT                ((MMIFMM_FILE_TYPE_EBOOK * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE))//0x00080000
#define MMIFMM_MOVIE_LRC                ((MMIFMM_FILE_TYPE_EBOOK * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE<< 1))//0x00100000
// COMѹ���ļ�
#define MMIFMM_MOVIE_SZIP               ((MMIFMM_FILE_TYPE_SZIP * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE))//0x00200000
//VCARD
#if defined MMI_VCARD_SUPPORT
#define MMIFMM_MOVIE_VCF                ((MMIFMM_FILE_TYPE_VCARD * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE))//0x00400000
#endif
//Vcalendar
#ifdef MMI_VCALENDAR_SUPPORT
#define MMIFMM_MOVIE_VCS				((MMIFMM_FILE_TYPE_VCALENDAR * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE))//32λ�����ˣ���ʱ��������ɡ�
#endif
//MTV
#define MMIFMM_MOVIE_MTV                ((MMIFMM_FILE_TYPE_MTV * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE))//Ox00800000
//JAVA
#define MMIFMM_JAVA_JAD                 ((MMIFMM_FILE_TYPE_JAVA * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE))//Ox02000000
#define MMIFMM_JAVA_JAR                 ((MMIFMM_FILE_TYPE_JAVA * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE<<1))//0x04000000

//THEME
#define MMIFMM_THEME_QB					((MMIFMM_FILE_TYPE_THEME * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE))

// NF�����֧�ֵ��ļ�
#define MMIFMM_BROWSER_HTM              ((MMIFMM_FILE_TYPE_BROWSER * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE))//Ox08000000
#define MMIFMM_BROWSER_HTML             ((MMIFMM_FILE_TYPE_BROWSER * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<1))//0x10000000
#define MMIFMM_BROWSER_XML              ((MMIFMM_FILE_TYPE_BROWSER * MMIFMM_FILE_TYPE_BASE) +(MMIFMM_SUFFIX_BASE<<2))//0x20000000
// Font
#define MMIFMM_FONT                     ((MMIFMM_FILE_TYPE_FONT * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE))//0x40000000

#ifdef MRAPP_SUPPORT
#define MMIFMM_MRAPP_MRP                (MMIFMM_FONT|MMIFMM_SUFFIX_BASE)//0x40000001
#endif
#ifdef HERO_ENGINE_SUPPORT 
#define MMIFMM_HERO_TOO                (MMIFMM_FONT|MMIFMM_SUFFIX_BASE)
#endif
#define MMIFMM_BROWSER_URL          ((MMIFMM_FILE_TYPE_URL * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_SUFFIX_BASE))

#define MMIFMM_PIC_ALL                  ((MMIFMM_FILE_TYPE_PICTURE * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_FILE_ALL >> 10)) //(MMIFMM_PICTURE_JPG|MMIFMM_PICTURE_GIF|MMIFMM_PICTURE_BMP|MMIFMM_PICTURE_WBMP|MMIFMM_PICTURE_PNG)//0x0000001f
#define MMIFMM_MUSIC_ALL                ((MMIFMM_FILE_TYPE_MUSIC * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_FILE_ALL >> 10))//(MMIFMM_MUSIC_MP3|MMIFMM_MUSIC_WMA|MMIFMM_MUSIC_MID|MMIFMM_MUSIC_AMR|MMIFMM_MUSIC_ACC|MMIFMM_MUSIC_M4A|MMIFMM_MUSIC_WAV|MMIFMM_MUSIC_MIDI)//0x0001ff00
#define MMIFMM_MOVIE_ALL                ((MMIFMM_FILE_TYPE_MOVIE * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_FILE_ALL >> 10))//(MMIFMM_MOVIE_MP4|MMIFMM_MOVIE_3GP|MMIFMM_MOVIE_AVI|MMIFMM_MOVIE_FLV|MMIFMM_MOVIE_RMVB)//cr227510 maryxiao
#define MMIFMM_TXT_ALL                  ((MMIFMM_FILE_TYPE_EBOOK * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_FILE_ALL >> 10))//(MMIFMM_MOVIE_TXT|MMIFMM_MOVIE_LRC)
#define MMIFMM_DRM_ALL                  (MMIFMM_DM_FILE|MMIFMM_DCF_FILE)
#define MMIFMM_JAVA_ALL                 ((MMIFMM_FILE_TYPE_JAVA * MMIFMM_FILE_TYPE_BASE) + (MMIFMM_FILE_ALL >> 10))
#define MMIFMM_FILE_ALL                 0xffffffff


#ifdef SEARCH_SUPPORT
#define MMISEARCH_SUFFIX_MAX_LEN           10  //include dot
#endif
/**--------------------------------------------------------------------------*
**                         GLOBAL VALUE DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 

/*****************************************************************************/
//  Description : �Ƚ�����
//  Global resource dependence : 
//  Author: Robert Lu
//  Note: 
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileName(
                                     const void     *i1_data,   // first data
                                     const void     *i2_data    // second data
                                     ) ;

/*****************************************************************************/
//  Description : �Ƚ��ļ���С
//  Global resource dependence : 
//  Author: Robert Lu
//  Note: 
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileSize(
                                     const void     *i1_data,   // first data
                                     const void     *i2_data    // second data
                                     );

/*****************************************************************************/
//  Description : �Ƚ��ļ�ʱ��
//  Global resource dependence : 
//  Author: Robert Lu
//  Note: 
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileTime(
                                     const void     *i1_data,   // first data
                                     const void     *i2_data    // second data
                                     );

/*****************************************************************************/
//  Description : �Ƚ�����
//  Global resource dependence : 
//  Author: Robert Lu
//  Note: 
/*****************************************************************************/
PUBLIC int MMIAPIFMM_CompareFileType(
                                     const void     *i1_data,   // first data
                                     const void     *i2_data    // second data
                                     );

/*****************************************************************************/
//  Description : ����ǰĿ¼�е��ļ����ݣ�
//  Global resource dependence :
//  Author: robert.lu
//  Note: �ļ����棬û��ʵ��
/*****************************************************************************/
PUBLIC uint16 MMIAPIFMM_GetCurrentPathFile(// �����ļ���Ŀ¼������
                                           const wchar          *full_path_ptr,     // ȫ·��,��������
                                           //MMIFMM_READ_TYPE_E   type,               // ����
                                           MMIFMM_DATA_INFO_T   *data_ptr         // �������
                                           //uint16               *output_filename_ptr// ������ݵ��ļ���
                                           );
/*****************************************************************************/
//  Description : get file type
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIFMM_GetFileType(
                                                const wchar      *suffix_name_ptr,
                                                uint16      suffix_name_len
                                                );

/*****************************************************************************/
//  Description : convert file type to fmm file type
//  Global resource dependence : 
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIFMM_ConvertFileType(
                                                const wchar      *suffix_name_ptr,
                                                uint16      suffix_name_len
                                                );

/*****************************************************************************/
//  Description : convert file type to movie type
//  Global resource dependence : 
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMICOM_VIDEO_TYPE_E MMIAPIFMM_ConvertMovieFileType(
                                                 const wchar    *suffix_name_ptr,
                                                  uint16    suffix_name_len
                                                  );
/*****************************************************************************/
//  Description : convert file type to music type
//  Global resource dependence : 
//  Author: Robert Lu
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIFMM_ConvertMusicFileType(
                                                  const wchar    *suffix_name_ptr,
                                                  uint16    suffix_name_len
                                                  );

/*****************************************************************************/
//  Description : get the type of folder
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FOLDER_TYPE_E MMIAPIFMM_GetFolderType(
                                                    const wchar      *path_ptr,
                                                    uint16      path_len,
                                                    BOOLEAN     is_include_path
                                                    );

/*****************************************************************************/
//  Description : sort
//  Global resource dependence :
//  Author: robert.lu
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPIFMM_FileDataSort(
                                  MMIFMM_DATA_INFO_T    *list_data_ptr,
                                  MMIFMM_SORT_TYPE_E    sort
                                  );

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence : 
//  Author:robert.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_StopOperaSDPlug(void);

                                       
/*****************************************************************************/
//  Description : handle copy task callback
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyCallBack(
                         uint16     result
                         );

/*****************************************************************************/
//  Description : handle copy task callback
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyFileExistInform(void);



/*****************************************************************************/
//  Description : get fmm data
//  Global resource dependence : none
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_DATA_INFO_T * MMIAPIFMM_GetFmmData(void);

/*****************************************************************************/
//  Description : ֹͣ����/ɾ�����ļ�����
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_ExitFileOption(void);

/*****************************************************************************/
//  Description :�Ƿ����ļ�����/ɾ������
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFileOption(void);

/*****************************************************************************/
//  Description : ���µ�ǰ���ļ��б�
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_UpdateCurrentFileLIst(void);

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : ���µ�ǰ���ļ��б��һ��
//  Global resource dependence :For DRM�������֮���ͼ�����
//  Author: liyan.zhu
//  Note:2012.9.8
/*****************************************************************************/
PUBLIC void MMIAPIFMM_UpdateCurrentLIstItem(void);
#endif

/*****************************************************************************/
//  Description : get dev info and check if have enough space
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsDevEnoughSpace(uint32 size,MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : get pic data for pic detail info
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetPictureWidthHeight(
                                        const wchar       *full_path_ptr,
                                        const uint16      full_path_len,
                                        uint32            file_size,
                                        uint16            *width_ptr,
                                        uint16            *height_ptr
                                        );
/*****************************************************************************/
//  Description : initialize Multim
//  Global resource dependence : none
//  Author: Liqing.Peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitFolders(void);

/*****************************************************************************/
//  Description : initialize NV about multim wallpaper
//  Global resource dependence : none
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitWallPaper(void);

/*****************************************************************************/
//  Description : ����ͼƬΪǽֽ
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToWallpaper(
                                           BOOLEAN      is_pre_wallpaper,   //�Ƿ��ǻָ�ǰһ��ǽֽ
                                           const wchar  *full_path_ptr,
                                           uint16       full_path_len
                                           );

/*****************************************************************************/
//  Description : ����ͼƬΪǽֽ,�����ͼƬ�ü�������ʽ֧�֣����Ƚ���ͼƬ�ü�
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToWallpaperEx(
                                           BOOLEAN      is_pre_wallpaper,   //�Ƿ��ǻָ�ǰһ��ǽֽ
                                           const wchar  *full_path_ptr,
                                           uint16       full_path_len
                                           );

/*****************************************************************************/
//  Description : ����ͼƬΪ���ػ�����/����?�����ͼƬ�ü�������ʽ֧�֣����Ƚ���ͼƬ�ü�
//  Global resource dependence : 
//  Author: liyan.zhu
//  Note: copy MMIAPIFMM_SetPictureToWallpaperEx
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetPictureToPowerAndSaverEx(
													 //BOOLEAN      is_pre_wallpaper,   //�Ƿ��ǻָ�ǰһ��ǽֽ
													 const wchar  *full_path_ptr,
													 uint16       full_path_len
													 );
/*****************************************************************************/
//  Description : set win title
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetFileNameWinTitle(
                                  MMI_WIN_ID_T    win_id, 
                                  const wchar     *file_name_ptr,
                                  uint16          file_name_len,
                                  BOOLEAN         is_refresh
                                  );

/*****************************************************************************/
//  Description : set music file to ring
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetMusicToRing(
                                MMIFMM_SET_RING_TYPE_E   ring_type,
                                DPARAM                      param,
                                uint16                      *full_name_ptr,
                                uint16                      full_name_len
                                );
/*****************************************************************************/
//  Description : is file wallpaper file
//  Global resource dependence : none
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsWallPaperFile(
                                  const wchar *full_path_ptr,
                                  uint16    full_path_len
                                  );


/*****************************************************************************/
//  Description :if is FmmMainWinOpen
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFmmMainWinOpen( void );



/*****************************************************************************/
//  Description : check memory card status
//  Return: 
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CheckMemoryCard(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : handle del task callback
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
void MMIAPIFMM_DelCallBack(uint16     result);

/*****************************************************************************/
//  Description : get file info form fullpath
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetFileInfoFormFullPath(
                                      const wchar *full_file_name,       //in
                                      const uint32 filename_len,          //in
                                      MMIFILE_FILE_INFO_T *file_info//out
                                      );

/*****************************************************************************/
//  Description : init system folder
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitSystemFolder(
                                     const wchar   *device_ptr,
                                     uint16  device_len,
                                     const wchar   *dir_ptr,
                                     uint16  dir_len
                                     );

/*****************************************************************************/
// 	Description: get the prefer disk
//	Global resource dependence:
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetDefaultDisk(void);

/*****************************************************************************/
// 	Description: get the number of all available disks
//	Global resource dependence:
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIFMM_GetAllAvarialeDiskNum(void);

/*****************************************************************************/
// 	Description: is enough space
//	Global resource dependence:
//  Author: aoke.hu
//	Note:������������ȡ����ռ�Ҫ����̷�.��ת�������ģ�����ʹ�ã��������գ�¼��¼����
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsEnoughSpace(
                                     MMIFMM_STORE_TYPE_E store_type,
                                     MMIFILE_DEVICE_E prefer_devie,
                                     uint32 min_free_size,
                                     MMIFILE_DEVICE_E *device_ptr  //[out]
                                     );

/*****************************************************************************/
//  Description : get system file full path for read
//  Author:aoke.hu
//  Note: check from MMI_DEVICE_SYSTEM
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForReadEx(
                                                 BOOLEAN        is_fix_store,           //[IN]
                                                 BOOLEAN        is_from_cdisk,          //[IN]
                                                 const wchar    *path_name_ptr,         //[IN]
                                                 uint16         path_name_len,          //[IN]                                                 
                                                 const wchar    *file_name_ptr,         //[IN]
                                                 uint16         file_name_len,          //[IN]
                                                 wchar          *full_path_name_ptr,    //[OUT]
                                                 uint16         *full_path_len_ptr      //[OUT]                                                 
                                                 );

/*****************************************************************************/
//  Description : get system file full path for write
//  Author:aoke.hu
//  Note: check from MMI_DEVICE_SYSTEM
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForWriteEx(
                                                  BOOLEAN        is_fix_store,          //[IN]TRUE, is fixed. FALSE, is portable
                                                  BOOLEAN        is_from_cdisk,         //[IN]
                                                  const wchar    *path_name_ptr,        //[IN]
                                                  uint16         path_name_len,         //[IN]                                                 
                                                  const wchar    *file_name_ptr,        //[IN]
                                                  uint16         file_name_len,         //[IN]
                                                  uint32         min_free_size,         //[IN]
                                                  wchar          *full_path_name_ptr,   //[OUT]
                                                  uint16         *full_path_len_ptr     //[OUT]                                                 
                                                  );

/*****************************************************************************/
//  Description : get system file full path for read
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForRead(
                                                 BOOLEAN        is_fix_store,           //[IN]
                                                 const wchar    *path_name_ptr,         //[IN]
                                                 uint16         path_name_len,          //[IN]                                                 
                                                 const wchar    *file_name_ptr,         //[IN]
                                                 uint16         file_name_len,          //[IN]
                                                 wchar          *full_path_name_ptr,    //[OUT]
                                                 uint16         *full_path_len_ptr      //[OUT]                                                 
                                                 );

/*****************************************************************************/
//  Description : get system file full path for write
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForWrite(
                                                  BOOLEAN        is_fix_store,          //[IN]TRUE, is fixed. FALSE, is portable
                                                  const wchar    *path_name_ptr,        //[IN]
                                                  uint16         path_name_len,         //[IN]                                                 
                                                  const wchar    *file_name_ptr,        //[IN]
                                                  uint16         file_name_len,         //[IN]
                                                  uint32         min_free_size,         //[IN]
                                                  wchar          *full_path_name_ptr,   //[OUT]
                                                  uint16         *full_path_len_ptr     //[OUT]                                                 
                                                  );

/*****************************************************************************/
//  Description : get system file full path for read
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForReadUdiskPrefer(
                                                 BOOLEAN        is_fix_store,           //[IN]
                                                 BOOLEAN        is_from_cdisk,          //[IN]
                                                 const wchar    *path_name_ptr,         //[IN]
                                                 uint16         path_name_len,          //[IN]                                                 
                                                 const wchar    *file_name_ptr,         //[IN]
                                                 uint16         file_name_len,          //[IN]
                                                 wchar          *full_path_name_ptr,    //[OUT]
                                                 uint16         *full_path_len_ptr      //[OUT]                                                 
                                                 );


/*****************************************************************************/
//  Description : get system file full path for write
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetSysFileFullPathForWriteUdiskPrefer(
                                                  BOOLEAN        is_fix_store,          //[IN]TRUE, is fixed. FALSE, is portable
                                                  BOOLEAN        is_from_cdisk,         //[IN]
                                                  const wchar    *path_name_ptr,        //[IN]
                                                  uint16         path_name_len,         //[IN]                                                 
                                                  const wchar    *file_name_ptr,        //[IN]
                                                  uint16         file_name_len,         //[IN]
                                                  uint32         min_free_size,         //[IN]
                                                  wchar          *full_path_name_ptr,   //[OUT]
                                                  uint16         *full_path_len_ptr     //[OUT]                                                 
                                                  );

/*****************************************************************************/
//  Description : create sysfile dir, if success return TRUE, else return FALSE
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CreateSysFileDir(
                                        MMIFILE_DEVICE_E    dev_type,               //[IN]
                                        const wchar     *path_name_ptr,         //[IN]
                                        uint16          path_name_len,          //[IN]
                                        MMIFILE_ERROR_E *file_err_ptr           //[OUT] can be null
                                        );

/*****************************************************************************/
//  Description : Combine Sys Path
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CombineSysPath(
                         MMIFILE_DEVICE_E    dev_type,              //[IN]
                         const wchar    *path_name_ptr,         //[IN]
                         uint16         path_name_len,          //[IN]
                         const wchar    *file_name_ptr,         //[IN]
                         uint16         file_name_len,          //[IN]
                         wchar          *full_path_name_ptr,    //[OUT]
                         uint16         *full_path_len_ptr      //[OUT]
                         );

/*****************************************************************************/
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
//  Return: 
//  Global resource dependence : 
//  Author: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileStop(void);

/*****************************************************************************/
// Description : show file error type  
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_PreviewPicture(wchar *full_path_name_ptr);

/*****************************************************************************/
// Description : preview current video file
// Global resource dependence : 
// Author:  murphy.xie
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_PreviewVideo(wchar *full_path_name_ptr);


/*****************************************************************************/
// Description : copy next  
// Global resource dependence : 
// Author:  
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_CopyNext(void);

/*****************************************************************************/
// Description : setting file detail
// Global resource dependence : 
// Author: YING.XU
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetFolderDetail(
                                 MMIFMM_DETAIL_DATA_T  *detail_data,
                                 MMI_CTRL_ID_T              ctrl_id,
                                 MMI_WIN_ID_T            win_id
                                 );

/*****************************************************************************/
// Description : setting folder detail cnf
// Global resource dependence : 
// Author: YING.XU
// Note: 
/*****************************************************************************/
PUBLIC void   MMIAPIFMM_SetFolderDetailCNF(
                                 MMIFMM_DETAIL_DATA_T  *detail_data,
                                 MMI_CTRL_ID_T              ctrl_id,
                                 MMI_WIN_ID_T            win_id
                                 );
//fixed by liyan.zhu for bugzilla 2760 2012-05-22 fmm search support
#ifdef MMI_FMM_SEARCH_SUPPORT
/*****************************************************************************/
// Description : �������Ƿ���Ч
// Global resource dependence : 
// Author: Ming.Song
// Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_FmmFindStringIsValid(
                                   MMI_STRING_T   *file_name_ptr  //in:
                                   );
#endif
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
// Description : ���ļ��б���
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/ 
PUBLIC void MMIAPIFMM_OpenFMMMainWin(void);

/*****************************************************************************/
// Description : close�ļ��б���
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/ 
PUBLIC void MMIAPIFMM_CloseFMMMainWin(void);
#endif

/*****************************************************************************/
// Description : �ļ�����
// Global resource dependence : 
// Author:aoke.hu
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_DetailFileData(
                                     MMIFMM_DETAIL_DATA_T* detail_data  // ����
                                     );

//fixed by liyan.zhu for bugzilla 2760
#if defined(DRM_SUPPORT)
/*****************************************************************************/
//  Description : convert mime type
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIFMM_ConvertMimeType(
                                                MIME_TYPE_E mime_type
                                                );

/*****************************************************************************/
// Description : add drm file detail
// Global resource dependence : 
// Author:haiwu.chen
// Note: ����Э��Ŀǰֻ�ܸ���������Ȩ������Ŀǰʵ�ֵ�����Ȩ��������ʾ
/*****************************************************************************/
PUBLIC void MMIAPIFMM_AddDrmFileDetail(
    const uint16 *file_name,
    uint32 access_mode, 
    uint32 share_mode, 
    uint32 file_attri,
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: �Ƿ����Ԥ�����������Ԥ��������Ҫ��ʾ����
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UpdateListIconData(
    MMI_CTRL_ID_T ctrl_id, 
    uint16 index,
    const wchar *file_dir_ptr,              //�ļ������ļ�������
    const wchar *file_path_ptr              //�ļ�ȫ·��
);

/*****************************************************************************/
//  Description : can this drm file execute
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ���������ڸ���ѡ���ļ����е�ͼ��
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UpdateSelectListIconData(
    wchar *file_path_ptr,
    uint16 file_path_len
);

/*****************************************************************************/
//  Description : create drm pop menu
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIFMM_DrmOpenDownloadMenuWin(wchar *full_path);
#endif

/*****************************************************************************/
// Description : open file  
// Global resource dependence : 
// Author: xiaoming.ren
// RETRUN: 
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenFile(wchar *full_path_name_ptr);

/*****************************************************************************/
//  Description : MMIAPIFMM_ShowTxtContent
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIAPIFMM_ShowTxtContent(wchar *full_path_name_ptr);


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////// ���½ӿڲ����Ǵ�mmi_filemgr.h�ƹ�����      /////////////
///////////////////���Ķ���ӿڶ�ֻ�ܷ���mmifmm_export.h///////////////
///////////////////**************************************************//////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//#if 1
/*****************************************************************************/
//  Description : split full pathname to filepath and filename/dir
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: �������ȫ·����full_path_ptr���ֽ��·������full_path_ptr��+�ļ�����file_name_ptr��
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SplitLastName(uint16* full_path_ptr,             //[in],[out]
                                     uint16* full_path_len_ptr,         //[in],[out]
                                     uint16* file_name_ptr,             //[out]
                                     uint16* file_name_len_ptr          //[out]
                                     );

/*****************************************************************************/
//  Description : split full pathname to device, dir, filename
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SplitFullPath(      //֧�ֶ༶Ŀ¼
                              const wchar      *full_path_ptr,     //in
                              uint16           full_path_len,      //in, unicode����
                              wchar            *device_ptr,        //out, device_xxx_ptr, dir_xxx_ptr, file_name_xxx_ptr ������NULL, ����������һ������NULL
                              uint16           *device_len_ptr,    //out, unicode����
                              wchar            *dir_ptr,           //out
                              uint16           *dir_len_ptr,       //out, unicode����
                              wchar            *file_name_ptr,     //out
                              uint16           *file_name_len_ptr  //out, unicode����
                              );

/*****************************************************************************/
// 	Description : split full path name to path,name and suffix
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SplitFullPathExt(
                                     const wchar     *full_path_ptr, //in
                                     uint16          full_path_len,  //in
                                     wchar           *path_ptr,      //in/out:may PNULL
                                     uint16          *path_len_ptr,  //in/out:may PNULL,path_ptr��Ϊ��ʱ,�˴�������max len
                                     wchar           *name_ptr,      //in/out:may PNULL,don't include suffix
                                     uint16          *name_len_ptr,  //in/out:may PNULL,name_ptr��Ϊ��ʱ,�˴�������max len
                                     wchar           *suffix_ptr,    //in/out:may PNULL,include dot
                                     uint16          *suffix_len_ptr //in/out:may PNULL,suffix_ptr��Ϊ��ʱ,�˴�������max len
                                     );

/*****************************************************************************/
//  Description : combine full pathname from device, file type, filename
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CombineFullPath(    //֧�ֶ༶Ŀ¼
                              const wchar     *device_ptr,        //in
                              uint16           device_len,         //in, unicode����
                              const wchar     *dir_ptr,           //in
                              uint16           dir_len,            //in, unicode����
                              const wchar     *file_name_ptr,     //in, file_name_ptr ������NULL
                              uint16           file_name_len,      //in, , unicode����
                              wchar           *full_path_ptr,     //out
                              uint16           *full_path_len_ptr  //out, unicode����
                              );

/*****************************************************************************/
//  Description : combin absolut path according to root, dir, name
//  Global resource dependence :                                
//  Author: gang.tong
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIFMM_CombineFullPathEx(
                                          wchar * abs_path_ptr,   // [out] the absolute path name.
                                          uint32 abs_path_len,    // [in]  the buffer length of abs_path_ptr.
                                          MMIFILE_DEVICE_E file_dev,  // [in]  the file dev type.
                                          const wchar* dir_ptr,  // [in]  the directory name
                                          //                    BOOLEAN dir_is_ucs2,    // [in]  whether the directory is in ucs2
                                          const wchar* name_ptr//, // [in]  the file name 
                                          //                    BOOLEAN name_is_ucs2    // [in]  whether the file is in ucs2
                                          );
/*****************************************************************************/
//  Description : split file name to two part: name and suffix
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SplitFileName(
                              const wchar      *file_name_ptr,     //in
                              uint16           file_name_len,      //in, unicode����
                              wchar            *name_ptr,          //out, name_xxx_ptr, suffix_xxx_ptr ������NULL, ����������һ������NULL
                              uint16           *name_len_ptr,      //out, unicode����
                              wchar            *suffix_ptr,        //out
                              uint16           *suffix_len_ptr     //out, unicode����
                              );

/*****************************************************************************/
//  Description : get file size string
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_GetFileSizeString(
                             uint32  size,
                             wchar*  str_ptr,
                             uint16  wstr_len,
							 BOOLEAN is_rounding
                             );

/*****************************************************************************/
//  Description : Init file system
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_InitFileTasks(void);


/*****************************************************************************/
//  Description : Init file system
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC SFS_ERROR_E MMIAPIFMM_RegisterDevice(
                                const wchar *device_ptr,    //in
                                uint16      device_len      //in, unicode����
                                );

/*****************************************************************************/
//  Description : unregister device 
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_UnRegisterDevice(
                                const wchar *device_ptr,    //in
                                uint16      device_len      //in, unicode����
                                );


/*****************************************************************************/
//  Description : get device status, ok or not ok
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_FormatDevice(
                            const wchar *device_ptr,    //in
                            uint16      device_len,     //in, unicode����
                            MMIFILE_DEVICE_FORMAT_E     format  //in
                            );

/*****************************************************************************/
//  Description : format device asyn
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_FormatDeviceAsyn(
                                 const wchar                *device_ptr,        //in
                                 uint16                     device_len,         //in, unicode����
                                 MMIFILE_DEVICE_FORMAT_E    format,             //in
                                 MMI_WIN_ID_T               win_id,             //in
                                 MMI_MESSAGE_ID_E           msg_id              //in
                                );

/*****************************************************************************/
//  Description : get device status, ok or not ok
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceStatus(
                                const wchar *device_ptr,    //in
                                uint16      device_len      //in, unicode����
                                );

/*****************************************************************************/
//  Description : set sd status
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceTypeStatus(MMIFILE_DEVICE_E memory_type);

/*****************************************************************************/
//  Description : �����豸���ͷ��ظ��豸��TEXT ID
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceName(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : �����豸���ͷ���û�и��豸���ַ���
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetAlertTextIDByDev(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : �����豸���ͷ��ظ��豸������Text id
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceMemStatusText(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : �����豸���ͷ��ظ��豸�ĸ����ַ�����Text id
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceCopyText(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIAPIFMM_GetDevicePath(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIFMM_GetDevicePathLen(MMIFILE_DEVICE_E device_type);
//
///*****************************************************************************/
////  Description : Callback
////  Global resource dependence : 
////  Author:
////  Note: 
///*****************************************************************************/
// PUBLIC wchar*  MMIFILE_GetRootDir(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
// PUBLIC wchar* MMIFILE_GetPathByFileDevice(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
// PUBLIC uint16 MMIFILE_GetPathLenByFileDevice(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : get file device type by sd card type
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
// PUBLIC FILE_DEV_E_T MMIFILE_GetFileDeviceType(MMIFILE_DEVICE_E sd_type);

/*****************************************************************************/
//  Description : get sd card type type by file device
//  Global resource dependence : 
//  Author: gang.tong
//  Note:
/*****************************************************************************/ 
// PUBLIC MMIFILE_DEVICE_E MMIFILE_GetDeviceTypeByFileDev(FILE_DEV_E_T fs_dev);



/*****************************************************************************/
//  Description : get file device type by sd card type
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
// PUBLIC FILE_DEV_E_T MMIFILE_GetFileDeviceByFilter(uint8 file_filter);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
// PUBLIC uint8 MMIFILE_GetFileFilterByDevice(FILE_DEV_E_T file_dev);

/*****************************************************************************/
//  Description : �����û�ѡ����豸���ͣ��������ʺϵ��豸����
//  Global resource dependence : 
//  Author: 
//  Note: ����û�ѡ����豸ʣ��ռ�������С�ռ�Ҫ�󣬷����û����ã�
//        ����û�ѡ����豸ʣ��ռ䲻����������С�ռ�Ҫ�󣬷��ص�һ�����ʣ��ռ���豸����
//        ���U�̺����еĴ洢���������ڣ�����Ĭ�ϵ�sd��
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_GetSuitableFileDev(
												  MMIFILE_DEVICE_E user_sel_dev,//IN: 
												   uint32 min_free_space,//IN:
												   MMIFILE_DEVICE_E *suitable_dev_ptr//OUT:
												   );
											
/*****************************************************************************/
//  Description : create directory
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CreateDir(
                          const wchar      *full_path_ptr,     //in
                          uint16           full_path_len       //in, unicode����
                          );

/*****************************************************************************/
//  Description : create directory with RO or HIDDEN attr
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetAttr(
                               const wchar       *full_path_ptr,     //in
                               uint16            full_path_len,       //in, ˫�ֽ�Ϊ��λ
                               BOOLEAN           is_hidden,         //in, �Ƿ�Ϊ�������� 
                               BOOLEAN           is_ro,             //in, �Ƿ�Ϊֻ������ 
                               BOOLEAN           is_system,         //in, �Ƿ�Ϊϵͳ����
                               BOOLEAN           is_arch           //in, �Ƿ�Ϊ�浵����
                               );

/*****************************************************************************/
//  Description : get file information
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetFileInfo(
                            const wchar             *full_path_ptr,     //in, ȫ�ļ���
                            uint16                  full_path_len,      //in, ȫ�ļ�������, unicode����
                            uint32                  *file_size_ptr,     //out,�ļ���С
							SFS_DATE_T				*modify_date,
							SFS_TIME_T				*modify_time
                            );


/*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFileExist(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len       //in, unicode����
                            );

/*****************************************************************************/
//  Description : check whether folder is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsFolderExist(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len       //in, unicode����
                            );

/*****************************************************************************/
//  Description : get device free space
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceFreeSpace(
                                    const wchar *device_ptr,        //in
                                    uint16      device_len,         //in, unicode����
                                    uint32      *free_high_ptr,     //out
                                    uint32      *free_low_ptr       //out
                                    );

/*****************************************************************************/
//  Description : get device used space
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceUsedSpace(
                                    const wchar *device_ptr,        //in
                                    uint16      device_len,         //in, unicode����
                                    uint32      *used_high_ptr,     //out
                                    uint32      *used_low_ptr       //out
                                    );

/*****************************************************************************/
//  Description : rename file
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_RenameFile(
                            const wchar     *full_path_ptr,     //in
                            uint16          full_path_len,      //in, unicode����
                            wchar          *new_file_name_ptr, //in
                            uint16          new_file_name_len   //in, unicode����
                            );
/*****************************************************************************/
//  Description : rename device name
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_SetDeviceLabel(
                            const wchar   *device_path,     //in
                            char          *label_name,      //in
                            uint8          label_name_len
                            );
/*****************************************************************************/
//  Description : get device label
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_GetDeviceLabel(
                            const wchar   *device_path,     //in
                            char          *label_name       //out
                            );
/*****************************************************************************/
//  Description : check device name whether it is Udisk
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetDeviceTypeByPath(
                              const wchar *device_ptr,    //in
                              uint16    device_len      //in, ˫�ֽ�Ϊ��λ
                              );

/*****************************************************************************/
//  Description : read file data syn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFilesDataSyn(
                                const wchar             *full_path_ptr,     //in
                                uint16                  full_path_len,      //in, unicode����
                                uint8                   *buf_ptr,           //out
                                uint32                  to_read_size        //in
                                );

/*****************************************************************************/
//  Description : write file data syn, create file always and will delete file
//                existing
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_WriteFilesDataSyn(
                                  const wchar            *full_path_ptr,     //in
                                  uint16                  full_path_len,      //in, unicode����
                                  const uint8             *buf_ptr,           //in
                                  uint32                  to_write_size       //in
                                  );

/*****************************************************************************/
//  Description : delete file syn
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_DeleteFileSyn(
                              const wchar                  *full_path_ptr,     //in
                              uint16                  full_path_len       //in, unicode����
                              );

/*****************************************************************************/
//  Description : copy file
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_CopyFile(const wchar *sour_filename, const wchar * dest_filename);
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : read file data by offset syn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFileDataByOffsetSyn(
                                        const wchar             *full_path_ptr,     //in
                                        uint16                  full_path_len,      //in, unicode����
                                        uint8                   *buf_ptr,           //out
                                        uint32                  to_read_size,       //in
                                        uint32                  offset              //in
                                        );
#endif
//fixed by liyan.zhu for bugzilla 2760
//#ifdef PIC_EDITOR_SUPPORT
/*****************************************************************************/
//  Description : read file data asyn
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFilesDataAsyn(
                                  const wchar             *full_path_ptr,   //in
                                  uint16                  full_path_len,    //in, unicode����
                                  uint8                   *buf_ptr,         //out
                                  uint32                  to_read_size,     //in
                                  SFS_HANDLE              *file_handle_ptr, //in/out
                                  MMI_WIN_ID_T            win_id,           //in:�����ڷ���Ϣ,0�����ؼ�����Ϣ
                                  MMI_CTRL_ID_T           ctrl_id,          //in:���ؼ�����Ϣ,0�������ڷ���Ϣ
                                  MMI_MESSAGE_ID_E        msg_id            //in
                                  );
//#endif
//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : read file data asyn with file handle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFilesDataAsynEx(
    MMIFILE_HANDLE          file_handle,        //in
    uint8                   *buf_ptr,           //out
    uint32                  to_read_size,       //in
    MMI_WIN_ID_T            win_id,             //in:�����ڷ���Ϣ,0�����ؼ�����Ϣ
    MMI_CTRL_ID_T           ctrl_id,            //in:���ؼ�����Ϣ,0�������ڷ���Ϣ
    MMI_MESSAGE_ID_E        msg_id              //in
);
#endif

/*****************************************************************************/
//  Description : write file data asyn
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_WriteFilesDataAsyn(
                                   const wchar             *full_path_ptr,     //in
                                   uint16                  full_path_len,      //in, unicode����
                                   uint8                   *buf_ptr,           //in
                                   uint32                  to_write_size,      //in
                                   MMI_WIN_ID_T            win_id,             //in, �첽д�ļ����ݽ�����,��Ҫ����Ϣ֪ͨ�� win id
                                   MMI_MESSAGE_ID_E        msg_id              //in, �첽д�ļ����ݽ�����,��Ҫ���͵���Ϣ, ��һ��BOOLEAN�Ĳ���
                                   );

//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : read file data asyn with file handle
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_ReadFilesDataAsynEx(
    MMIFILE_HANDLE          file_handle,        //in
    uint8                   *buf_ptr,           //out
    uint32                  to_read_size,       //in
    MMI_WIN_ID_T            win_id,             //in:�����ڷ���Ϣ,0�����ؼ�����Ϣ
    MMI_CTRL_ID_T           ctrl_id,            //in:���ؼ�����Ϣ,0�������ڷ���Ϣ
    MMI_MESSAGE_ID_E        msg_id              //in
);

/*****************************************************************************/
//  Description : write file data by offset asyn, file must exist
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_WriteFileDataByOffsetAsyn(
                                          const wchar             *full_path_ptr,     //in
                                          uint16                  full_path_len,      //in, unicode����
                                          uint8                   *buf_ptr,           //in
                                          uint32                  to_write_size,      //in
                                          uint32                  offset,             //in
                                          MMI_WIN_ID_T            win_id,             //in
                                          MMI_MESSAGE_ID_E        msg_id              //in
                                          );
#endif
/*****************************************************************************/
//  Description : delete file asyn
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_DeleteFileAsyn(
                               const wchar             *full_path_ptr,     //in
                               uint16                  full_path_len,      //in, unicode����
                               MMI_WIN_ID_T            win_id,             //in, �첽ɾ���ļ�������,��Ҫ����Ϣ֪ͨ�� win id
                               MMI_MESSAGE_ID_E        msg_id              //in, �첽ɾ���ļ�������,��Ҫ���͵���Ϣ, ��һ��BOOLEAN�Ĳ���
                               );

/*****************************************************************************/
//  Description : asyn operation confirm
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC void MMIAPIFMM_OperationAsynCnf(
                              void                  *param_ptr, 
                              MMI_MESSAGE_ID_E      callback_msg_id
                              );

/*****************************************************************************/
//  Description :start copy one file
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CopyOneFile(
                            const wchar                 *src_full_path,
                            uint16                      src_path_len,       //in, unicode����
                            const wchar                 *dest_full_path,
                            uint16                      dest_path_len,      //in, unicode����
                            MMI_WIN_ID_T                win_id,
                            MMI_MESSAGE_ID_E            msg_id
                            );

/*****************************************************************************/
//  Description : read file data in copy file, can't be used alone
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CopyReadSrc(
                            SFS_ERROR_E     error
                            );

/*****************************************************************************/
//  Description : write file data in copy file, can't be used alone
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_CopyWriteDest(
                              SFS_ERROR_E   error
                              );

/*****************************************************************************/
//  Description : is file end
//  Global resource dependence:
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPIFMM_IsEndOfFile(  //TRUE: cur position is the end of the file.
                                   SFS_HANDLE handle
                                   );

/*****************************************************************************/
//  Description : Get a char from handle
//  Global resource dependence : 
//  Author: haiyang.hu
//  Note:
/*****************************************************************************/ 
PUBLIC SFS_ERROR_E MMIAPIFMM_GetChar(
                                   SFS_HANDLE handle, //[in]
                                   void*  char_ptr //[out]
                                   );

/*****************************************************************************/
//  Description : create file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_HANDLE MMIAPIFMM_CreateFile(
                                    const wchar *file_name, 
                                    uint32 access_mode,
                                    uint32 share_mode,  //Must be NULL,File System have not realize it 
                                    uint32 file_attri       //Must be NULL,File System have not realize it 
                                    );

/*****************************************************************************/
//  Description : create file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_CreateFileAsyn(
                                    const wchar *file_name, 
                                    uint32 access_mode,
                                    uint32 share_mode,  //Must be NULL,File System have not realize it 
                                    uint32 file_attri,     //Must be NULL,File System have not realize it 
									MMI_WIN_ID_T            win_id,   //in
									MMI_MESSAGE_ID_E        msg_id,    //in
									MMIFILE_HANDLE        *file_handle_ptr//OUT
                                    );

/*****************************************************************************/
//  Description : delete file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_DeleteFile(
                                    const wchar *name,
                                    MMIFILE_OVERLAPPED_T *overlapped_ptr
                                    );

/*****************************************************************************/
//  Description : create directiory
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_CreateDirectory(
                                        const wchar *path
                                        );

/*****************************************************************************/
//  Description : delete all files in folder
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_DeleteTreeEx(
	const wchar *pathName,
	const wchar *fileName,
    BOOLEAN is_del_dir);

/*****************************************************************************/
//  Description : delete folder and it's files in it, only can delete it's sub empty folder
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_DeleteTree(
	const wchar *pathName,
	const wchar *fileName);

/*****************************************************************************/
//  Description : delete diretory
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_DeleteDirectory(
                                        const wchar *path
                                        );

/*****************************************************************************/
//  Description : find first file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_HANDLE MMIAPIFMM_FindFirstFile(
                                        const wchar *find, 
                                        uint16 find_len,
                                        MMIFILE_FIND_DATA_T *fd
                                        );

/*****************************************************************************/
//  Description : find nezt file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_FindNextFile(
                                        MMIFILE_HANDLE file_handle, 
                                        MMIFILE_FIND_DATA_T *fd
                                        );

/*****************************************************************************/
//  Description : find close
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_FindClose(
                                    MMIFILE_HANDLE file_handle
                                    );

/*****************************************************************************/
//  Description : get file size
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC uint32 MMIAPIFMM_GetFileSize(
                            MMIFILE_HANDLE file_handle
                            );

/*****************************************************************************/
//  Description : set file size
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_SetFileSize(
                                    MMIFILE_HANDLE file_handle, 
                                    uint32 size
                                    );

/*****************************************************************************/
//  Description : read file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_ReadFile(
                                MMIFILE_HANDLE file_handle,
                                void *buffer,
                                uint32 bytes_to_read,
                                uint32 *bytes_read,
                                MMIFILE_OVERLAPPED_T *overlapped
                                );

/*****************************************************************************/
//  Description : write file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_WriteFile(
                                MMIFILE_HANDLE file_handle,
                                const void *buffer,
                                uint32 bytes_to_write,
                                uint32 *bytes_written,
                                MMIFILE_OVERLAPPED_T *overlapped
                                );

/*****************************************************************************/
//  Description : set file pointer
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC uint32 MMIAPIFMM_SetFilePointer(
                                MMIFILE_HANDLE file_handle,
                                int32 offset,
                                uint32 origin
                                );

/*****************************************************************************/
//  Description : get file pointer
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC int32 MMIAPIFMM_GetFilePointer(
                                MMIFILE_HANDLE file_handle,
                                uint32 origin
                                );

/*****************************************************************************/
//  Description : close file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E MMIAPIFMM_CloseFile(
                                    MMIFILE_HANDLE file_handle
                                    );
/*****************************************************************************/
//  Description : check and get validate device
//  Global resource dependence : 
//  Author: gang.tong
//  Note: ָ���豸������Чʱ�����δ�SD����UDISK�����豸
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetValidateDevice(MMIFILE_DEVICE_E original_device);

/*****************************************************************************/
//  Description : get file path len
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
//PUBLIC uint32 MMIAPIFILE_GetFilePathLen( const wchar * pWString );

/*****************************************************************************/
//  Description : Callback
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_Callback( uint16     result);

/*****************************************************************************/
//  Description : append file
//  Global resource dependence : 
//  Author: 
//  Note: append append_file_name from append_postion to base_file_name,
//        if is_delete_append_file is TRUE, after append, the append_file must be deleted
//  Return: return file result
/*****************************************************************************/ 
PUBLIC MMIFILE_ERROR_E  MMIAPIFMM_AppendFile(
                                    wchar *base_file_name,
									wchar *append_file_name,
									uint32 append_postion,
									BOOLEAN is_delete_append_file,
									uint32  *appended_size_ptr
                                    );
                                    
/*****************************************************************************/
//  Description : ����ʣ��ռ�ϴ��device
//  Global resource dependence : 
//  Author: charlie.wang
//  Note:
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetMaxFreeDeviceType(void);

/*****************************************************************************/
//  Description : ���ص�һ����Ч��device
//  Global resource dependence : 
//  Author: 
//  Note:���û����Ч�ķ���FS_INVALID
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstValidDevice(void);

/*****************************************************************************/
//  Description : ���ص�һ����Ч��sd card device type 
//  Global resource dependence : 
//  Author: 
//  Note:���û����Ч�ķ���MMI_DEVICE_NUM
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstValidSD(void);

/*****************************************************************************/
//  Description : ���ص�һ�����ڵ�device�����device��������Ч��
//  Global resource dependence : 
//  Author: 
//  Note:���û�д��ڵķ���FS_INVALID
/*****************************************************************************/ 
PUBLIC MMIFILE_DEVICE_E MMIAPIFMM_GetFirstExistDevice(void);

/*****************************************************************************/
//  Description : �����豸���ͷ��ظ��豸ʣ��������Text id
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_TEXT_ID_T  MMIAPIFMM_GetDeviceFreeSpaceText(MMIFILE_DEVICE_E device_type);

/*****************************************************************************/
//  Description : �����豸���ͷ��ظ��豸��ͼ��
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_IMAGE_ID_T  MMIAPIFMM_GetOSDImageIDByDeviceType(MMIFILE_DEVICE_E device_type);
/*****************************************************************************/
//  Description : has exist file when move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_MoveFileExist(void);

/*****************************************************************************/
//  Description : �����豸���ͷ��ظ��豸��ͼ��
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/ 
PUBLIC MMI_IMAGE_ID_T  MMIAPIFMM_GetStorageIcon(MMIFILE_DEVICE_E device_type);
//#endif
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : is drm file by suffix name
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note: ͨ����׺����ǲ���drm�ļ�(�˽ӿڴ����������ʹ��MMIAPIDRM_IsDRMFile�ӿ�)
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_IsDrmFileBySuffix(
    wchar *file_name_ptr, //in:
    uint16  file_name_len   //in:
);
#endif

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////// ���½ӿڲ����Ǵ�mmifmm_interface.h�ƹ�����      /////////////
///////////////////���Ķ���ӿڶ�ֻ�ܷ���mmifmm_export.h///////////////
///////////////////**************************************************//////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description : get icon id by fmm data info
//  Global resource dependence :
//  Author: james.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIFMM_GetIconIdByFileType( MMIFMM_FILE_TYPE_E  file_type, MMIFILE_DEVICE_E device);

/*****************************************************************************/
//  Description : ��һ������, ��ʾĳһ��Ŀ¼, ���ѵ�ǰĿ¼�������ļ��к�
//                ���з���ɸѡ�������ļ��г���
//  Return: �û��ڴ�����ѡ���ļ���, ͨ���ص��������ؽ������һ������
//  ����˵��:
//      ret_array�� һ��Ҫ�ģ����ΪNULL���򲻷����ļ�
//      callback��  һ��Ҫ�ģ����ΪNULL���򲻷����ļ�
// 
//      path_ptr/path_len����Ŀ¼�������NULL����Ӹ�Ŀ¼��
//      filter_ptr��  �����ؼ��ַ���������"*.mp3"
// 
//      func_type:  һ���� FUNC_FIND_FILE_AND_ALLFOLER����ѡ���ļ���ͬʱ��������ļ�����ʾ����
//      is_multi_sel���Ƿ񷵻ض���ļ�������ѡ�񵥸��ļ�����FALSE
// 
//      handle_func��   �����ò�������NULL
//
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIAPIFMM_OpenFileWinByLayer(
                                wchar                   *path_ptr,      //Ĭ�ϴ�Ŀ¼�������NULL����Ӹ�Ŀ¼��
                                uint16                  path_len,
                                MMIFMM_FILTER_T         *filter_ptr,    //ɸѡ����
                                MMIFMM_FUNC_TYPE_E      func_type,      //����
                                BOOLEAN                 is_multi_sel,   //�Ƿ��ѡ
                                PROCESSMSG_FUNC         handle_func,    //�Զ��崦����
                                FILEARRAY               ret_array,      //�������
                                MMIFMM_OpenFileCallBack callback,       //�ص�����
                                BOOLEAN                 is_sel_folder
                                );


/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileInPath(
                            const wchar         *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id
                            );

/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�ͬʱ����
//  Return: 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileInPathAndSort(
                            const wchar             *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id,
                            FILEARRAY_SORT_E    sort_type       //��������
                            );

/*****************************************************************************/
//  Description : ��ָ��Ŀ¼�������ļ�
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileInPathEx(
                            const wchar        *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16             path_len,
							const wchar        *default_path_ptr,//Ԥ��·��
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY_SORT_E    sort_type,      //��������
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id
                            );

//@CR241955 2011.05.27
/*****************************************************************************/
//  Description :��ָ��Ŀ¼��������ͬʱָ����������
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFileInPathBySort(
                            const wchar         *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                            uint16              path_len,
                            MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                            BOOLEAN             is_recurse,     //�����Ƿ�ݹ�
                            MMIFMM_FUNC_TYPE_E  func_type,      //����
                            FILEARRAY_SORT_E    sort_type,      //��������
                            FILEARRAY           file_array,     //out, ���
                            MMI_WIN_ID_T        win_id,         //���win_id��msg_id��Ϊ0�������ͬ����ʽ�����ļ�
                            MMI_MESSAGE_ID_E    msg_id
                            );

/*****************************************************************************/
//  Description : ��MMIFMM_SearchFileInPath���ʹ��
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
//PUBLIC void MMIAPIFMM_SearchFileCallBack(void);

/*****************************************************************************/
//  Description : open any fmm win
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
/*PUBLIC void MMIAPIFMM_OpenExplorerExt(
                                      const wchar * path_ptr,
                                      uint16   path_len,
                                      const wchar * filename,
                                      uint16   file_len,
                                      BOOLEAN  is_need_display,
                                      uint32   find_suffix_type
                                      );*/




/*****************************************************************************/
//  Description : ����������ת�����ַ�����ʽ
//  Global resource dependence : 
//  Author:liqingpeng
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIFMM_GetFilerInfo(MMIFMM_FILTER_T* filter,uint32 suffix_type);

/*****************************************************************************/
//  Description :
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SetOpenFileWinSoftkey(
                                       MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                       MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                                       MMI_TEXT_ID_T   rightsoft_id   // the right softkey id
                                       );

/*****************************************************************************/
//  Description : get icon id by file name
//  Global resource dependence :
//  Author: james.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIAPIFMM_GetFileTypeByFileName(
                                                       const wchar  *file_name,
                                                       uint16       file_name_len
                                                       );


/*****************************************************************************/
//  Description : get free space in percent
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPIFMM_GetFreeSpaceInPercent(const wchar *device_ptr);

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
/*****************************************************************************/
//  Description : select a font from fmm
//  Global resource dependence : 
//  Author: james.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFMM_OpenSelectFontWin(MMIFMM_SELECT_INFO_T *select_info);
#endif

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : open music file
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenMusicFileList(void);

/*****************************************************************************/
//  Description : open movie file
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_OpenMovieFileList(void);
#endif //CMCC_UI_STYLE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function  Declare                                                     */
/* Description:  Hanlde at cmd for fmm auto test cases                                                           */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#ifdef MMI_SYNCTOOL_SUPPORT
/*****************************************************************************/
//  Description : create folders in same path
//  Global resource dependence : s_fmm_current_path
//  Parameter: folders number
//  Author: shuyan.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMCreateFolderHori(uint16 hori_num);

/*****************************************************************************/
//  Description : create folders across path
//  Global resource dependence : s_fmm_current_path
//  Parameter: folders number
//  Author: shuyan.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  Atest_FMMCreateFolderVerti(uint16 Veri_num);
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    End of Atest  Function  Declare                                                       */
/* Description:  Hanlde at cmd for fmm auto test cases                                                           */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#endif

/*****************************************************************************/
//  Description : move file call back
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_MoveCallBack(uint16 result);
/*****************************************************************************/
//  Description : copy is from move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:�����move����copy,��ΪTRUE
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_IsCopyFromMove(void);
/*****************************************************************************/
//  Description : has exist file when move
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_MoveFileExist(void);

//fixed by liyan.zhu for bugzilla 2760
#if 0
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFMM_SearchFile(                            
                                const wchar             *path_ptr,      //in, ����Ŀ¼���������NULL�����������д洢��
                                uint16              path_len,
                                MMIFMM_FILTER_T     *filter_ptr,    //ɸѡ����
                                MMIFMM_FUNC_TYPE_E  func_type,      //����
                                MMIFMM_SEARCH_FILE_INFO_T * search_file_ptr
                                );
#endif                                
/*****************************************************************************/
//  Description : ��һ������, ��ʾĳһ��Ŀ¼, ���ѵ�ǰĿ¼�������ļ��к�
//                ���з���ɸѡ�������ļ��г���
//  Return: �û��ڴ�����ѡ���ļ���, ͨ���ص��������ؽ������һ������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
MMIFMM_OPENWIN_RET_E MMIAPIFMM_OpenFileWinByLayerEx(MMIFMM_OPEN_LAYER_WIN_INFO_T *open_layer_info);  

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIFMM_ConvertFileType					 MMIAPIFMM_ConvertFileType

#define MMIFMM_ConvertMovieFileType	 		 MMIAPIFMM_ConvertMovieFileType

#define MMIFMM_ConvertMusicFileType 	 		 MMIAPIFMM_ConvertMusicFileType

#define MMIFMM_CopyCallBack		 			 MMIAPIFMM_CopyCallBack

#define MMIFMM_CopyFileExistInform 				 MMIAPIFMM_CopyFileExistInform

#define MMIFMM_UpdateCurrentFileLIst	 		 MMIAPIFMM_UpdateCurrentFileLIst

#ifdef DRM_SUPPORT
#define MMIFMM_UpdateCurrentLIstItem	 		 MMIAPIFMM_UpdateCurrentLIstItem
#endif

#define MMIFMM_IsDevEnoughSpace			 	 MMIAPIFMM_IsDevEnoughSpace

#define MMIFMM_GetPictureWidthHeight	 		 MMIAPIFMM_GetPictureWidthHeight

#define MMIAPIMULTIM_Init				 		 MMIAPIFMM_InitFolders

#define MMIAPIMULTIM_InitWallPaper		 		 MMIAPIFMM_InitWallPaper

#define MMIAPIMULTIM_SetPictureToWallpaper		 MMIAPIFMM_SetPictureToWallpaper

#define MMIAPIMULTIM_SetPictureToWallpaperEx	 MMIAPIFMM_SetPictureToWallpaperEx

#define MMIAPIMULTIM_SetPictureToPowerAndSaverEx  MMIAPIFMM_SetPictureToPowerAndSaverEx

#define MMIAPIMULTIM_SetMusicToRing			 MMIAPIFMM_SetMusicToRing

#define MMIAPIMULTIM_IsWallPaperFile			 MMIAPIFMM_IsWallPaperFile

#define MMIFMM_ChangeWallPaperFileName		 MMIAPIFMM_ChangeWallPaperFileName

#define MMIAPIMULTIM_CloseWaitSetWallpaper		 MMIAPIFMM_CloseWaitSetWallpaper

#define MMIAPIMULTIM_HandleWallpaperWaitMsg	 MMIAPIFMM_HandleWallpaperWaitMsg

#define MMIAPIMULTIM_OpenWaitSetWallpaper		 MMIAPIFMM_OpenWaitSetWallpaper

#define MMIMULTIM_GetWallPaperInfo			 MMIAPIFMM_GetWallPaperInfo

//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIMULTIM_SetRandomNameEditbox		 MMIAPIFMM_SetRandomNameEditbox
#endif

//#define MMIFMM_PromptFileErrorType				 MMIAPIFMM_PromptFileErrorType

#define MMIFMM_DelCallBack					 MMIAPIFMM_DelCallBack

#define MMIFILE_InitSystemFolder				 MMIAPIFMM_InitSystemFolder

#define MMIFILE_GetDefaultDisk					 MMIAPIFMM_GetDefaultDisk

#define MMIFILE_IsEnoughSpace					 MMIAPIFMM_IsEnoughSpace

#define MMIFILE_GetSysFileFullPathForReadEx		 MMIAPIFMM_GetSysFileFullPathForReadEx

#define MMIFILE_GetSysFileFullPathForWriteEx	         MMIAPIFMM_GetSysFileFullPathForWriteEx

#define MMIFILE_GetSysFileFullPathForRead		 MMIAPIFMM_GetSysFileFullPathForRead

#define MMIFILE_GetSysFileFullPathForWrite		 MMIAPIFMM_GetSysFileFullPathForWrite

#define MMIFILE_CreateSysFileDir				 MMIAPIFMM_CreateSysFileDir

#define MMIFILE_CombineSysPath				MMIAPIFMM_CombineSysPath

#define MMIFMM_PreviewPicture					 MMIAPIFMM_PreviewPicture

//#define MMIFMM_PreviewVideo					 MMIAPIFMM_PreviewVideo

//#define MMIFMM_SetFolderDetail					 MMIAPIFMM_SetFolderDetail

//#define MMIFMM_SetFolderDetailCNF				 MMIAPIFMM_SetFolderDetailCNF

//#define MMIFMM_GetDrmIconId 					 MMIAPIFMM_GetDrmIconId

#define MMIFMM_UpdateListIconData				 MMIAPIFMM_UpdateListIconData

#define MMIFMM_UpdateSelectListIconData		 MMIAPIFMM_UpdateSelectListIconData

//////////////////////////////////define for mmi_filemgr.h   ////////////////////////////////////////

#define MMIFILE_SplitLastName					 MMIAPIFMM_SplitLastName

#define MMIFILE_SplitFullPath 					 MMIAPIFMM_SplitFullPath

#define MMIFILE_SplitFullPathExt				         MMIAPIFMM_SplitFullPathExt

#define MMIFILE_CombineFullPath				 MMIAPIFMM_CombineFullPath

#define MMIFILE_CombineFullPathEx				 MMIAPIFMM_CombineFullPathEx

#define MMIFILE_SplitFileName					 MMIAPIFMM_SplitFileName

#define MMIFMM_GetFileSizeString				 MMIAPIFMM_GetFileSizeString

#define MMIFILE_Init							 MMIAPIFMM_InitFileTasks

#define MMIFILE_RegisterDevice					 MMIAPIFMM_RegisterDevice

#define MMIFILE_UnRegisterDevice				 MMIAPIFMM_UnRegisterDevice

#define MMIFILE_FormatDevice					 MMIAPIFMM_FormatDevice

#define MMIFILE_FormatDeviceAsyn				 MMIAPIFMM_FormatDeviceAsyn

#define MMIFILE_GetDeviceStatus				 MMIAPIFMM_GetDeviceStatus

#define MMIAPISD_GetStatus					 MMIAPIFMM_GetDeviceTypeStatus

#define MMIFILE_GetDeviceName					 MMIAPIFMM_GetDeviceName

#define MMIFILE_GetAlertTextIDByDev			 MMIAPIFMM_GetAlertTextIDByDev

#define MMIFILE_GetDeviceMemStatusText		 MMIAPIFMM_GetDeviceMemStatusText

#define MMIFILE_GetDeviceCopyText				 MMIAPIFMM_GetDeviceCopyText

#define MMIFILE_GetDevicePath					 MMIAPIFMM_GetDevicePath

#define MMIFILE_GetDevicePathLen				 MMIAPIFMM_GetDevicePathLen

#define MMIFILE_GetSuitableFileDev				 MMIAPIFMM_GetSuitableFileDev

#define MMIFILE_CreateDir						 MMIAPIFMM_CreateDir

#define MMIFILE_SetAttr						 MMIAPIFMM_SetAttr

#define MMIFILE_GetFileInfo						 MMIAPIFMM_GetFileInfo

//#define MMIFILE_IsFileExist						 MMIAPIFMM_IsFileExist

//#define MMIFILE_IsFolderExist					 MMIAPIFMM_IsFolderExist

#define MMIFILE_GetDeviceFreeSpace				 MMIAPIFMM_GetDeviceFreeSpace

#define MMIFILE_GetDeviceUsedSpace				 MMIAPIFMM_GetDeviceUsedSpace

#define MMIFILE_RenameFile					 MMIAPIFMM_RenameFile

#define MMIFILE_GetDeviceTypeByPath			 MMIAPIFMM_GetDeviceTypeByPath

#define MMIFILE_ReadFilesDataSyn				 MMIAPIFMM_ReadFilesDataSyn

#define MMIFILE_WriteFilesDataSyn				 MMIAPIFMM_WriteFilesDataSyn

#define MMIFILE_DeleteFileSyn					 MMIAPIFMM_DeleteFileSyn.

#define MMIFILE_CopyFile						 MMIAPIFMM_CopyFile
//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIFILE_ReadFileDataByOffsetSyn			 MMIAPIFMM_ReadFileDataByOffsetSyn
#endif
//fixed by liyan.zhu for bugzilla 2760
//#ifdef PIC_EDITOR_SUPPORT
#define MMIFILE_ReadFilesDataAsyn				 MMIAPIFMM_ReadFilesDataAsyn
//#endif
//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIFILE_ReadFilesDataAsynEx			 MMIAPIFMM_ReadFilesDataAsynEx
#endif
#define MMIFILE_WriteFilesDataAsyn				 MMIAPIFMM_WriteFilesDataAsyn

//fixed by liyan.zhu for bugzilla 2760
#if 0
#define MMIFILE_ReadFilesDataAsynEx			 MMIAPIFMM_ReadFilesDataAsynEx

#define MMIFILE_WriteFileDataByOffsetAsyn		 MMIAPIFMM_WriteFileDataByOffsetAsyn
#endif
#define MMIFILE_DeleteFileAsyn					 MMIAPIFMM_DeleteFileAsyn

#define MMIFILE_OperationAsynCnf				 MMIAPIFMM_OperationAsynCnf

#define MMIFILE_CopyOneFile					 MMIAPIFMM_CopyOneFile

#define MMIFILE_CopyReadSrc					 MMIAPIFMM_CopyReadSrc

#define MMIFILE_CopyWriteDest					 MMIAPIFMM_CopyWriteDest

#define MMIFILE_IsEndOfFile					 MMIAPIFMM_IsEndOfFile

#define MMIFILE_GetChar						 MMIAPIFMM_GetChar

//#define MMIFILE_CreateFile						 MMIAPIFMM_CreateFile

#define MMIFILE_CreateFileAsyn					 MMIAPIFMM_CreateFileAsyn

//#define MMIFILE_DeleteFile						 MMIAPIFMM_DeleteFile

#define MMIFILE_CreateDirectory					 MMIAPIFMM_CreateDirectory

#define MMIFILE_DeleteTreeEx					 MMIAPIFMM_DeleteTreeEx

#define MMIFILE_DeleteTree						 MMIAPIFMM_DeleteTree

//#define MMIFILE_DeleteDirectory					 MMIAPIFMM_DeleteDirectory

//#define MMIFILE_FindFirstFile					 MMIAPIFMM_FindFirstFile

//#define MMIFILE_FindNextFile					 MMIAPIFMM_FindNextFile

//#define MMIFILE_FindClose						 MMIAPIFMM_FindClose

//#define MMIFILE_GetFileSize						 MMIAPIFMM_GetFileSize

#define MMIFILE_SetFileSize						 MMIAPIFMM_SetFileSize

//#define MMIFILE_ReadFile						 MMIAPIFMM_ReadFile

//#define MMIFILE_WriteFile						 MMIAPIFMM_WriteFile

#define MMIFILE_SetFilePointer					 MMIAPIFMM_SetFilePointer

#define MMIFILE_GetFilePointer					 MMIAPIFMM_GetFilePointer

//#define MMIFILE_CloseFile						 MMIAPIFMM_CloseFile

#define MMIFILE_GetValidateDevice				 MMIAPIFMM_GetValidateDevice

#define MMIAPIFILE_Callback					 MMIAPIFMM_Callback

#define MMIFILE_AppendFile						 MMIAPIFMM_AppendFile

#define MMIFILE_GetMaxFreeDeviceType			 MMIAPIFMM_GetMaxFreeDeviceType

#define MMIFILE_GetFirstValidDevice				 MMIAPIFMM_GetFirstValidDevice

#define MMIFILE_GetFirstValidSD					 MMIAPIFMM_GetFirstValidSD

#define MMIFILE_GetFirstExistDevice				 MMIAPIFMM_GetFirstExistDevice

#define MMIFILE_GetDeviceFreeSpaceText			 MMIAPIFMM_GetDeviceFreeSpaceText

#define MMIFILE_GetOSDImageIDByDeviceType	 MMIAPIFMM_GetOSDImageIDByDeviceType

#define MMIFILE_GetStorageIcon					 MMIAPIFMM_GetStorageIcon

#ifdef DRM_SUPPORT
#define MMIFILE_IsDrmFileBySuffix				 MMIAPIFMM_IsDrmFileBySuffix
#endif

//////////////////////////////////      mmifmm_interface.h    ///////////////////////////////////

#define MMIFMM_GetIconIdByFileType				 MMIAPIFMM_GetIconIdByFileType

#define MMIFMM_OpenFileWinByLayer				 MMIAPIFMM_OpenFileWinByLayer

#define MMIFMM_SearchFileInPath				 MMIAPIFMM_SearchFileInPath

#define MMIFMM_SearchFileInPathEx				 MMIAPIFMM_SearchFileInPathEx

#define MMIFMM_SearchFileInPathBySort			 MMIAPIFMM_SearchFileInPathBySort

//#define MMIFMM_SearchFileCallBack			         MMIAPIFMM_SearchFileCallBack

#define MMIFMM_GetOpenFileWinCurFile			 MMIAPIFMM_GetOpenFileWinCurFile
 
#define MMIFMM_GetFilerInfo					 MMIAPIFMM_GetFilerInfo

#define MMIFMM_SetOpenFileWinSoftkey			 MMIAPIFMM_SetOpenFileWinSoftkey

#define MMIFMM_GetFileTypeByFileName			 MMIAPIFMM_GetFileTypeByFileName

#define FILE_OpenMatchedItem					 MMIAPIFMM_OpenMatchedItem

#define FILE_SetMatchItemData					 MMIAPIFMM_SetMatchItemData

#define MMIAPIFILE_StartLocalSearch				 MMIAPIFMM_StartLocalSearch

#define MMIAPIFILE_FreeLocalSearchResource		 MMIAPIFMM_FreeLocalSearchResource

#define MMIAPIFILE_GetFileSearchMatchedNum		 MMIAPIFMM_GetFileSearchMatchedNum

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
