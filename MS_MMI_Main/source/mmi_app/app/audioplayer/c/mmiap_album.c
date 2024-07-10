/*****************************************************************************
** File Name:      mmiap_album.c                                             *
** Author:                                                                   *
** Date:           28/08/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio player                         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012       robert.wang       Create                                    *
******************************************************************************/
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT

#define _MMIAP_ALBUM_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"


#include "guilabel.h"
#include "guitext.h"
#include "guiedit.h"
#include "guistring.h"
#include "guiref_scale.h"
#include "guires.h"
#include "guiownerdraw.h"
#include "guibutton.h"
#include "guistatusbar.h"

//other module


//local module
#include "mmiap_position.h"
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmimp3_image.h"


#include "mmiap_setting.h"
#include "mmiap_list.h"
#include "mmimp3_export.h"
#include "mmiapwin_main.h"
#include "mmiap_play.h"
#include "mmiap_common.h"

#ifdef MMIAP_MUSIC_LIST_SUPPORT
#include "mmiap_list_control.h"
#endif


#include "mmiap_album.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMP3_MAX_ALBUM_SIZE           (800*1024)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MMIMP3_GLIDE_RATIO              4   //移动距离超过lcd的1/4,则切换
#define MMIMP3_GLIDE_AUTO_FRAME         10  //松开后自动的桢数

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

LOCAL MMIAP_ALBUM_MNG_INFO_T *s_mmiap_album_mng_info_ptr = PNULL;
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get ap album manage info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_ALBUM_MNG_INFO_T *GetAPAlbumMngInfoPtr(void)
{
    return s_mmiap_album_mng_info_ptr;
}

/*****************************************************************************/
//  Description : create ap album manage info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAPAlbumMngInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIAP_ALBUM_MNG_INFO_T *info_ptr = GetAPAlbumMngInfoPtr();
    uint32 info_size = sizeof(*info_ptr);


    if (PNULL == info_ptr)
    {
        info_ptr = SCI_ALLOCA(info_size);
    }

    if (PNULL != info_ptr)
    {
        s_mmiap_album_mng_info_ptr = info_ptr;

        SCI_MEMSET(info_ptr, 0, info_size);

        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : destroy album mng ino
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyAlbumMngInfo(
    void
)
{
    if (PNULL != s_mmiap_album_mng_info_ptr)
    {
        SCI_FREE(s_mmiap_album_mng_info_ptr);
    }
}

/*****************************************************************************/
//  Description : get glide tp start point
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T GetGlideTpStartPoint(
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();
    GUI_POINT_T point = {0};


    if (PNULL != mng_ptr)
    {
        point = mng_ptr->glide_tp_start_point;
    }

    return point;
}

/*****************************************************************************/
//  Description : set glide tp start point
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetGlideTpStartPoint(
    GUI_POINT_T point //in
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();


    if (PNULL != mng_ptr)
    {
        mng_ptr->glide_tp_start_point = point;
    }
}

/*****************************************************************************/
//  Description : get glide tp prev point
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL GUI_POINT_T GetGlideTpPrevPoint(
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();
    GUI_POINT_T point = {0};


    if (PNULL != mng_ptr)
    {
        point = mng_ptr->glide_tp_prev_point;
    }

    return point;
}

/*****************************************************************************/
//  Description : set glide tp prev point
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void SetGlideTpPrevPoint(
    GUI_POINT_T point //in
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();


    if (PNULL != mng_ptr)
    {
        mng_ptr->glide_tp_prev_point = point;
    }
}
/*****************************************************************************/
//  Description : get album info ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_ALBUM_INFO_T*  GetAlbumInfoHeadPtr(
    void
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();
    MMIMP3_ALBUM_INFO_T *album_ptr = PNULL;


    if (PNULL != mng_ptr)
    {
        album_ptr = mng_ptr->album_info;
    }

    return album_ptr;
}

/*****************************************************************************/
//  Description : get appointed album info ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_ALBUM_INFO_T*  GetAppointedAlbumInfoPtr(
    MMIAP_DIRECT_E  which_album //in
)
{
    MMIMP3_ALBUM_INFO_T *album_ptr = PNULL;
    MMIMP3_ALBUM_INFO_T *info_ptr = GetAlbumInfoHeadPtr();



    if (PNULL != info_ptr
            && which_album < MMIAP_DIRECT_MAX
       )
    {
        album_ptr = &info_ptr[which_album];
    }

    return album_ptr;
}

/*****************************************************************************/
//  Description : 根据文件索引获取专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  GetAlbumInfo(
    uint32  file_index,             //in
    MMIMP3_ALBUM_INFO_T *album_ptr  //out
)
{
    BOOLEAN result = FALSE;
    wchar full_path_name[FMM_SEARCH_FILENAME_MAXLEN+1];
    uint16 full_path_len = FMM_SEARCH_FILENAME_MAXLEN;


    if (PNULL == album_ptr)
    {
        return result;
    }


    if(MMIMP3_GetFilePathInCurPlayList(file_index, full_path_name,&full_path_len))

    {
        result = MMIMP3_GetAlbumInfo(full_path_name,
                                     full_path_len,
                                     album_ptr
                                    );
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumInfo check file error! file_index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18043_112_2_18_2_5_15_362, (uint8*)"d", file_index);
    }

    return result;
}

/*****************************************************************************/
//  Description : 获取ID3文本描述信息并转换为wchar
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void GetAlbumWcharDescInfo(
    MMISRVAUD_ID3_TAG_T *tag_info_ptr, //in
    MMIMP3_ALBUM_INFO_T *album_ptr     //out
)
{
    uint16 str_len = 0;

    //artist
    if(strlen(tag_info_ptr->artist) > 0)
    {
        SCI_MEMSET(album_ptr->artist, 0, MMIMP3_COMMON_TAG_LEN);

        if(1 == tag_info_ptr->tag_code_type) //unicode
        {
            str_len = MMIAPICOM_Wstrlen((wchar*)tag_info_ptr->artist);
            str_len = MIN(str_len, MMIMP3_COMMON_TAG_LEN);
            MMIAPICOM_Wstrncpy(album_ptr->artist, (wchar*)tag_info_ptr->artist, str_len);
        }
        else
        {
            str_len = strlen(tag_info_ptr->artist);
            str_len = MIN(str_len, MMIMP3_COMMON_TAG_LEN);
            str_len = GUI_GBToWstr(album_ptr->artist, (uint8*)tag_info_ptr->artist, str_len);
        }

        //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumWcharDescInfo artist str_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18158_112_2_18_2_5_16_369, (uint8*)"d", str_len);
    }

    //album
    if(strlen(tag_info_ptr->album) > 0)
    {
        SCI_MEMSET(album_ptr->album, 0, MMIMP3_COMMON_TAG_LEN);

        if(1 == tag_info_ptr->tag_code_type) //unicode
        {
            str_len = MMIAPICOM_Wstrlen((wchar*)tag_info_ptr->album);
            str_len = MIN(str_len, MMIMP3_COMMON_TAG_LEN);
            MMIAPICOM_Wstrncpy(album_ptr->album, (wchar*)tag_info_ptr->album, str_len);
        }
        else
        {
            str_len = strlen(tag_info_ptr->album);
            str_len = MIN(str_len, MMIMP3_COMMON_TAG_LEN);
            str_len = GUI_GBToWstr(album_ptr->album, (uint8*)tag_info_ptr->album, str_len);
        }

        //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumWcharDescInfo album str_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18178_112_2_18_2_5_16_370, (uint8*)"d", str_len);
    }
}

/*****************************************************************************/
//  Description : 获取ID3文本描述信息并转换为wchar
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetAlbumImgDataFromFile(
    wchar  *file_name_ptr,    //in
    uint32 apic_offset,       //in
    uint32 apic_len,          //in
    uint8  *buf_ptr,          //out
    uint32 buf_len            //in
)
{
    BOOLEAN        result = 0;
    uint32         bytes_to_read = 0;
    uint32         read_size = 0;
    MMIFILE_HANDLE file_handle = 0;

    //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumImgDataFromFile apic_offset=%d, apic_len=%d buf_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18211_112_2_18_2_5_16_371, (uint8*)"ddd", apic_offset, apic_len, buf_len);

    //create file handle
    file_handle = MMIAPIFMM_CreateFile(file_name_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);

    if(SFS_INVALID_HANDLE == file_handle)
    {
        //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumImgDataFromFile create file error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18217_112_2_18_2_5_16_372, (uint8*)"");
        return FALSE;
    }

    //set file pos
    if(SFS_ERROR_NONE != MMIAPIFMM_SetFilePointer(file_handle, apic_offset, SFS_SEEK_BEGIN))
    {
        //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumImgDataFromFile set pos error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18224_112_2_18_2_5_16_373, (uint8*)"");
        MMIAPIFMM_CloseFile(file_handle);
        return FALSE;
    }

    //read file
    bytes_to_read = MIN(apic_len, buf_len);
    //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumImgDataFromFile min=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18231_112_2_18_2_5_16_374, (uint8*)"d", bytes_to_read);

    if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, buf_ptr, bytes_to_read, &read_size, NULL))
    {
        if(read_size == bytes_to_read)
        {
            //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumImgDataFromFile data len =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18236_112_2_18_2_5_16_375, (uint8*)"d", bytes_to_read);
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumImgDataFromFile bytes to read error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18241_112_2_18_2_5_16_376, (uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3]: GetAlbumImgDataFromFile read file error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18246_112_2_18_2_5_16_377, (uint8*)"");
    }

    //colse file handle
    MMIAPIFMM_CloseFile(file_handle);

    return (result);
}

/*****************************************************************************/
//  Description : 根据文件路径获取专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note: aoke modify
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMP3_GetAlbumInfo(
    const wchar * full_path_ptr,
    uint16 full_path_len,
    MMIMP3_ALBUM_INFO_T *album_ptr  //out
)
{
    BOOLEAN result = FALSE;
    BOOLEAN img_data_ok = FALSE;
    MMISRVAUD_CONTENT_INFO_T  mp3_file_info = {0};



    if (PNULL == full_path_ptr
            || PNULL == album_ptr
       )
    {
        return result;
    }

    if(MMIAPIFMM_IsFileExist(full_path_ptr, full_path_len))
    {
        if(MMISRVAUD_GetFileContentInfo(
                    full_path_ptr,  /*lint !e605*/
                    full_path_len,
                    &mp3_file_info
                ))
        {
            //get origin info ok
            result = TRUE;

            //convert artist, album description to wchar
            GetAlbumWcharDescInfo(&mp3_file_info.id3_tag, album_ptr);

            //if(MMK_IsFocusWin(MMIMP3_MAIN_PLAY_WIN_ID) && (MMIMP3_DISPLAY_ALBUM == APSETTING_GetDisplayStyle()))
            {
                //get album image data
                if(PNULL != album_ptr->img_data_ptr)
                {
                    //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_GetAlbumInfo img_data_ptr not NULL !, addr=0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18080_112_2_18_2_5_16_363, (uint8*)"d", album_ptr->img_data_ptr);

                    SCI_FREE(album_ptr->img_data_ptr);
                    album_ptr->img_data_ptr = PNULL;
                    album_ptr->data_size = 0;
                }

                //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_GetAlbumInfo apic_offset=%d apic_length=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18086_112_2_18_2_5_16_364, (uint8*)"dd", mp3_file_info.id3_tag.apic_offset, mp3_file_info.id3_tag.apic_length);

                if(mp3_file_info.id3_tag.apic_length > 0)
                {
                    album_ptr->data_size = MIN(MMIMP3_MAX_ALBUM_SIZE, mp3_file_info.id3_tag.apic_length);

                    album_ptr->img_data_ptr = (uint8*)SCI_ALLOCA(album_ptr->data_size);

                    if(PNULL != album_ptr->img_data_ptr)
                    {
                        SCI_MEMSET(album_ptr->img_data_ptr, 0, album_ptr->data_size);

                        //get album image data from file
                        img_data_ok = GetAlbumImgDataFromFile(
                                          full_path_ptr,  /*lint !e605*/
                                          mp3_file_info.id3_tag.apic_offset,
                                          album_ptr->data_size,
                                          album_ptr->img_data_ptr,
                                          album_ptr->data_size
                                      );

                        if(!img_data_ok)
                        {
                            //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_GetAlbumInfo GetAlbumImgDataFromFile fail!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18107_112_2_18_2_5_16_365, (uint8*)"");
                            //get data fail
                            SCI_FREE(album_ptr->img_data_ptr);
                            album_ptr->img_data_ptr = PNULL;
                            album_ptr->data_size = 0;
                        }
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_GetAlbumInfo img_data_ptr alloc failed!!!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18116_112_2_18_2_5_16_366, (uint8*)"");
                    }
                }
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_GetAlbumInfo file not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18123_112_2_18_2_5_16_367, (uint8*)"");
    }

    //SCI_TRACE_LOW:"[MMIMP3]: MMIMP3_GetAlbumInfo result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18126_112_2_18_2_5_16_368, (uint8*)"d", result);
    return (result);
}

/*****************************************************************************/
//  Description : 获取ID3文本描述信息并转换为wchar
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetAlbumWcharDescInfo(
    MMISRVAUD_ID3_TAG_T     *tag_info_ptr, //in
    MMIMP3_ALBUM_INFO_T *album_ptr     //out
)
{
    GetAlbumWcharDescInfo(tag_info_ptr, album_ptr);
}

/*****************************************************************************/
//  Description : 获取ID3文本描述信息并转换为wchar
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetAlbumImgDataFromFile(
    wchar  *file_name_ptr,    //in
    uint32 apic_offset,       //in
    uint32 apic_len,          //in
    uint8  *buf_ptr,          //out
    uint32 buf_len            //in
)
{
    return GetAlbumImgDataFromFile(file_name_ptr, apic_offset, apic_len, buf_ptr, buf_len);
}

/*****************************************************************************/
//  Description : 获取前一首专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void GetPreMp3AlbumInfo(void)
{
    uint32 index = 0;

    //SCI_TRACE_LOW:"[MMIMP3] GetPreMp3AlbumInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18278_112_2_18_2_5_16_378, (uint8*)"");

    if(MMIAPIMP3_GetPreOrNextMp3Index(TRUE, &index))
    {
        GetAlbumInfo(index, GetAppointedAlbumInfoPtr(MMIAP_DIRECT_LEFT));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3] GetPreMp3AlbumInfo get pre index error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18286_112_2_18_2_5_16_379, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 获取当前的专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void GetCurMp3AlbumInfo(void)
{
    uint32 index = MMIMP3_GetCurPlayFileIndex();
    uint32 total_num = MMIMP3_GetCurPlayListTotalNum();

    //SCI_TRACE_LOW:"[MMIMP3] GetCurMp3AlbumInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18299_112_2_18_2_5_16_380, (uint8*)"");

    if(total_num > 0)
    {
        GetAlbumInfo(index, GetAppointedAlbumInfoPtr(MMIAP_DIRECT_MID));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3] GetCurMp3AlbumInfo total_num == 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18307_112_2_18_2_5_16_381, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 获取下一首专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void GetNextMp3AlbumInfo(void)
{
    uint32 index = 0;

    //SCI_TRACE_LOW:"[MMIMP3] GetNextMp3AlbumInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18319_112_2_18_2_5_16_382, (uint8*)"");

    if(MMIAPIMP3_GetPreOrNextMp3Index(FALSE, &index))
    {
        GetAlbumInfo(index, GetAppointedAlbumInfoPtr(MMIAP_DIRECT_RIGHT));
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMP3] GetNextMp3AlbumInfo get next index error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18327_112_2_18_2_5_16_383, (uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 获取全部的专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_GetAllAlbumInfo(void)
{
    MMIAP_ReleaseAllAlbumInfo();

    GetPreMp3AlbumInfo();
    GetCurMp3AlbumInfo();
    GetNextMp3AlbumInfo();
}

/*****************************************************************************/
//  Description : 清空专辑信息
//  Global resource dependence :
//  Author: paul.huang
//  Note: 清除album的信息，但不释放其内存，然后再恢复其信息
/*****************************************************************************/
LOCAL void ReleaseAppointedAlbumInfo(
    BOOLEAN is_prev, //in
    MMIMP3_ALBUM_INFO_T *info_ptr //head pointer
)
{


    //SCI_TRACE_LOW:"[MMIMP3] MMIAP_ReleaseAlbumInfo, is_prev = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18427_112_2_18_2_5_16_385, (uint8*)"d", is_prev);


    if (PNULL == info_ptr)
    {
        return;
    }



    if(is_prev)
    {
        if(PNULL != info_ptr[MMIAP_DIRECT_MAX - 1].img_data_ptr)
        {
            SCI_FREE(info_ptr[MMIAP_DIRECT_MAX - 1].img_data_ptr);
        }
    }
    else
    {
        if(PNULL != info_ptr[0].img_data_ptr)
        {
            SCI_FREE(info_ptr[0].img_data_ptr);
        }
    }

    SCI_MEMSET(info_ptr, 0, sizeof(MMIMP3_ALBUM_INFO_T) * MMIAP_DIRECT_MAX);
}

/*****************************************************************************/
//  Description : 更新全部的专辑信息
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_UpdateAllAlbumInfo(BOOLEAN is_prev)
{
    uint8 i = 0;
    uint8 total_num = MMIAP_DIRECT_MAX;
    MMIMP3_ALBUM_INFO_T   mp3_album_info[MMIAP_DIRECT_MAX] = {0};
    MMIMP3_ALBUM_INFO_T *info_ptr = GetAlbumInfoHeadPtr();



    if (PNULL == info_ptr)
    {
        return;
    }


    SCI_MEMCPY(mp3_album_info, info_ptr, sizeof(mp3_album_info));

    if(is_prev)
    {
        ReleaseAppointedAlbumInfo(TRUE, info_ptr);

        for(i = 0; i < total_num - 1; i ++)
        {
            SCI_MEMCPY(&info_ptr[i + 1], &mp3_album_info[i], sizeof(MMIMP3_ALBUM_INFO_T));
        }

        GetPreMp3AlbumInfo();
    }
    else
    {
        ReleaseAppointedAlbumInfo(FALSE, info_ptr);

        for(i = 0; i < total_num - 1; i ++)
        {
            SCI_MEMCPY(&info_ptr[i], &mp3_album_info[i + 1], sizeof(MMIMP3_ALBUM_INFO_T));
        }

        GetNextMp3AlbumInfo();
    }
}

/*****************************************************************************/
//  Description : update all album information.
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_UpdateAllAlbumInfo(BOOLEAN is_prev)
{
    MMIAP_UpdateAllAlbumInfo(is_prev);
}

/*****************************************************************************/
//  Description : 清空全部的专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ReleaseAllAlbumInfo(void)
{
    uint32 i = 0;
    uint32 total_num = MMIAP_DIRECT_MAX;
    MMIMP3_ALBUM_INFO_T *info_ptr = GetAlbumInfoHeadPtr();
    MMIMP3_ALBUM_INFO_T *cur_ptr = PNULL;


    if (PNULL == info_ptr)
    {
        return;
    }


    //SCI_TRACE_LOW:"[MMIMP3] MMIAP_ReleaseAllAlbumInfo pre-addr:0x%x size%d cur-addr:0x%x size%d next-addr:0x%x size%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_WINTAB_18394_112_2_18_2_5_16_384, (uint8*)"dddddd",\
                 info_ptr[MMIAP_DIRECT_LEFT].img_data_ptr,
                 info_ptr[MMIAP_DIRECT_LEFT].data_size,
                 info_ptr[MMIAP_DIRECT_MID].img_data_ptr,
                 info_ptr[MMIAP_DIRECT_MID].data_size,
                 info_ptr[MMIAP_DIRECT_RIGHT].img_data_ptr,
                 info_ptr[MMIAP_DIRECT_RIGHT].data_size
                );




    for (i = 0; i < total_num; i++)
    {
        cur_ptr = &info_ptr[i];
        if (PNULL != cur_ptr) /*lint !e774*/
        {
            if (PNULL != cur_ptr->img_data_ptr)
            {
                SCI_FREE(cur_ptr->img_data_ptr);
            }
        }
    }

    SCI_MEMSET(info_ptr, 0, sizeof(MMIMP3_ALBUM_INFO_T) * total_num);

}

/*****************************************************************************/
//  Description : 清空专辑封面信息，释放内存
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ReleaseAllAlbumImgInfo(void)
{
    uint32 i = 0;
    uint32 total_num = MMIAP_DIRECT_MAX;
    MMIMP3_ALBUM_INFO_T *info_ptr = GetAlbumInfoHeadPtr();
    MMIMP3_ALBUM_INFO_T *cur_ptr = PNULL;



    if (PNULL == info_ptr)
    {
        return;
    }

    for (i = 0; i < total_num; i++)
    {
        cur_ptr = &info_ptr[i];
        if (PNULL != cur_ptr) /*lint !e774*/
        {
            if (PNULL != cur_ptr->img_data_ptr)
            {
                SCI_FREE(cur_ptr->img_data_ptr);
                cur_ptr->data_size = 0;
            }
        }
    }

    SCI_TRACE_LOW("MMIAP_ReleaseAllAlbumImgInfo");
}

/*****************************************************************************/
//  Description : 获取当前mp3专辑信息，包括歌手、专辑图片数据等.
//  Global resource dependence :
//  Author:xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurMp3AlbumInfo(
    MMIMP3_ALBUM_INFO_T *album_ptr  //in/out
)
{
    MMIMP3_ALBUM_INFO_T *cur_album_ptr = GetAppointedAlbumInfoPtr(MMIAP_DIRECT_MID);

    if(PNULL == album_ptr
            || PNULL == cur_album_ptr
      )
    {
        return FALSE;
    }

    //copy album data
    SCI_MEMCPY(album_ptr, cur_album_ptr, sizeof(MMIMP3_ALBUM_INFO_T));

    return TRUE;
}

/*****************************************************************************/
//  Description : 获取前一首或者下一首mp3索引
//  Global resource dependence :
//  Author:xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetPreOrNextMp3Index(
    BOOLEAN is_prev,
    uint32  *index_ptr //out
)
{
    BOOLEAN                 result = FALSE;
    uint32                  total_num = MMIMP3_GetCurPlayListTotalNum();
    uint16 file_index = 0;


    SCI_TRACE_LOW("[MMIMP3] MMIAPIMP3_GetPreOrNextMp3Index Total_num=%d ", \
                  total_num);

    if((0 == total_num)
            || (PNULL == index_ptr)
      )
    {
        return result;
    }

    result = MMIMP3_GetPreOrNextPlayFileIndex(
                 is_prev,
                 &file_index
             );

    if (result)
    {
        *index_ptr = file_index;
    }

    return (result);
}

/*****************************************************************************/
//  Description : 获取当前mp3专辑信息，包括歌手、专辑图片数据等.
//  Global resource dependence :
//  Author:xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_GetCurMp3AlbumInfo(
    MMIMP3_ALBUM_INFO_T *album_ptr  //in/out
)
{
    return MMIMP3_GetCurMp3AlbumInfo(album_ptr);
}

/*****************************************************************************/
//  Description : is tp point in valid album rect
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpValidAlbumRect(GUI_POINT_T *point_ptr)
{
    BOOLEAN     result = FALSE;
    GUI_RECT_T  rect_h = MMIMP3_ALBUM_ANIM_RECT_H;
    GUI_RECT_T  rect_v = MMIMP3_ALBUM_ANIM_RECT_V;
    uint16      up_osd_height = 0;
    uint16      down_osd_height = 0;
    GUI_RECT_T  rect = MMITHEME_IsMainScreenLandscape() ? rect_h : rect_v;

    if(MMIAP_IsOpenOsdPanel())
    {
#ifndef MMI_RES_LOW_COST
        GUIRES_GetImgWidthHeight(PNULL, &up_osd_height, IMAGE_MP3_OSD_BG_UP, MMIMP3_MAIN_PLAY_WIN_ID);
#endif
        GUIRES_GetImgWidthHeight(PNULL, &down_osd_height, IMAGE_MP3_OSD_BG_DOWN, MMIMP3_MAIN_PLAY_WIN_ID);

        rect.top    += up_osd_height;
        rect.bottom -= down_osd_height;
    }

    if(GUI_PointIsInRect(*point_ptr, rect))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle album image glide tp press down msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleAlbumImgGlideTpDown
(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    DPARAM param //in
)
{
    GUI_POINT_T     tp_point = {0};
    uint32          total_num = MMIMP3_GetCurPlayListTotalNum();
    BOOLEAN is_start = FALSE;



    if (PNULL == win_ptr
            || PNULL == param
       )
    {
        return;
    }



    if(MMIMP3_DISPLAY_ALBUM != APSETTING_GetDisplayStyle()
            || 0 == total_num)
    {
        SCI_TRACE_LOW("[MMIMP3]: ALBUMWIN_HandleImgGlideTpDown style=%d total_num=%d",\
                      APSETTING_GetDisplayStyle(), total_num);
        return;
    }

    //get point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);

    //is tp osd panel button
    is_start = IsTpValidAlbumRect(&tp_point);
    MMIAP_SetAlbumTpStart(is_start);

    if(is_start)
    {
        //save point
        SetGlideTpStartPoint(tp_point);
        SetGlideTpPrevPoint(tp_point);
    }

    MMIAP_SetAlbumGlideMove(FALSE);
}

/*****************************************************************************/
//  Description : handle album image glide tp press move msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleAlbumImgGlideTpMove
(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    DPARAM param
)
{
    int16           move_x = 0;
    GUI_POINT_T     tp_point = {0};
    MMI_CTRL_ID_T   left_ctrl_id  = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_LEFT);
    MMI_CTRL_ID_T   mid_ctrl_id   = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);
    MMI_CTRL_ID_T   right_ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_RIGHT);
    GUI_RECT_T      album_rect_h = MMIMP3_ALBUM_ANIM_RECT_H;
    uint32          total_num = MMIMP3_GetCurPlayListTotalNum();

    GUI_POINT_T glide_start_point = {0};
    GUI_POINT_T glide_prev_point = {0};


    if (PNULL == win_ptr
            || PNULL == param
       )
    {
        return;
    }


    glide_start_point = GetGlideTpStartPoint();
    glide_prev_point = GetGlideTpPrevPoint();

    if(MMIAP_IsAlbumTpStart()
            && MMIMP3_DISPLAY_ALBUM == APSETTING_GetDisplayStyle()
            && 0 != total_num)
    {
        //get point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        //横屏下超出专辑图片区域需特别处理
        if(MMITHEME_IsMainScreenLandscape())
        {
            //起点、当前点均不在专辑图片区
            if(!GUI_PointIsInRect(glide_start_point, album_rect_h)
                    && !GUI_PointIsInRect(tp_point, album_rect_h))
            {
                return;
            }

            //当前点超出专辑图片区
            if(GUI_PointIsInRect(glide_start_point, album_rect_h)
                    && !GUI_PointIsInRect(tp_point, album_rect_h))
            {
                tp_point.x = album_rect_h.right;
            }
        }

        //calc move x
        move_x = (int16)(tp_point.x - glide_prev_point.x);

        if(0 != move_x)
        {
            //set previous tp point
            SetGlideTpPrevPoint(tp_point);

            //sest control rect and display rect
            MMIAP_SetAnimCtrlRectDisplayRect(move_x, left_ctrl_id);
            MMIAP_SetAnimCtrlRectDisplayRect(move_x, right_ctrl_id);
            MMIAP_SetAnimCtrlRectDisplayRect(move_x, mid_ctrl_id);

            //glide is move
            MMIAP_SetAlbumGlideMove(TRUE);

            //close osd panel
            if(MMIAP_IsOpenOsdPanel())
            {
                MMIAP_CloseMp3OsdPanel(TRUE);

                //set reopen flag
                MMIAP_SetReopenOsdPanelFlag(TRUE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : set album tp start
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAlbumTpStart(
    BOOLEAN is_start //in
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();



    if (PNULL != mng_ptr)
    {
        mng_ptr->is_tp_start = is_start;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_SetAlbumTpStart is_start=%d",\
                  is_start);
}

/*****************************************************************************/
//  Description : get album tp start
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsAlbumTpStart(
    void
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();
    BOOLEAN is_start = FALSE;



    if (PNULL != mng_ptr)
    {
        is_start = mng_ptr->is_tp_start;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_IsAlbumTpStart is_start=%d",\
                  is_start);

    return is_start;
}

/*****************************************************************************/
//  Description : set album glide move
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAlbumGlideMove(
    BOOLEAN is_move //in
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();


    if (PNULL != mng_ptr)
    {
        mng_ptr->is_glide_move = is_move;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_SetAlbumGlideMove is_move=%d",\
                  is_move);
}

/*****************************************************************************/
//  Description : get album glide move
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsAlbumGlideMove(
    void
)
{
    MMIAP_ALBUM_MNG_INFO_T *mng_ptr = GetAPAlbumMngInfoPtr();
    BOOLEAN is_move = FALSE;


    if (PNULL != mng_ptr)
    {
        is_move = mng_ptr->is_glide_move;
    }

    SCI_TRACE_LOW("[MMIAP] MMIAP_IsAlbumGlideMove is_move=%d",\
                  is_move);

    return is_move;
}

/*****************************************************************************/
//  Description : handle album image glide tp press up msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_HandleAlbumImgGlideTpUp
(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    DPARAM param
)
{
    BOOLEAN ret = FALSE;
    BOOLEAN         is_prev = FALSE;
    BOOLEAN         is_next = FALSE;
    int16           move_x = 0;
    int16           move_width = 0;
    int16           move_offset = 0;
    int16           move_all = 0;
    int16           move_space = 0;
    uint16          lcd_width = 0;
    uint16          i = 0;
    //uint16          start_index = 0;
    GUI_POINT_T     tp_point = {0};
    uint32          total_num = MMIMP3_GetCurPlayListTotalNum();
    MMI_CTRL_ID_T   left_ctrl_id  = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_LEFT);
    MMI_CTRL_ID_T   mid_ctrl_id   = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_MID);
    MMI_CTRL_ID_T   right_ctrl_id = MMIAP_GetAnimCtrlID(MMIAP_DIRECT_RIGHT);
    GUI_RECT_T      anim_full_rect_h = MMIMP3_ALBUM_ANIM_RECT_H;
    GUI_RECT_T      anim_full_rect_v = MMIMP3_ALBUM_ANIM_RECT_V;
    GUI_RECT_T      anim_full_rect = MMITHEME_IsMainScreenLandscape() ? anim_full_rect_h : anim_full_rect_v;

    GUI_POINT_T glide_start_point = {0};
    GUI_POINT_T glide_prev_point = {0};



    if (PNULL == win_ptr)
    {
        return ret;
    }


    glide_start_point = GetGlideTpStartPoint();
    glide_prev_point = GetGlideTpPrevPoint();

    if(MMIAP_IsAlbumTpStart()
            && MMIMP3_DISPLAY_ALBUM == APSETTING_GetDisplayStyle()
            && 0 != total_num
      )  /*lint !e774*/
    {
        MMIAP_SetAlbumTpStart(FALSE);

        //get point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);

        if(MMITHEME_IsMainScreenLandscape())
        {
            //起点、当前点均不在专辑图片区
            if(!GUI_PointIsInRect(glide_start_point, anim_full_rect_h)
                    && !GUI_PointIsInRect(tp_point, anim_full_rect_h))
            {
                return ret;
            }

            //当前点超出专辑图片区
            if(GUI_PointIsInRect(glide_start_point, anim_full_rect_h)
                    && !GUI_PointIsInRect(tp_point, anim_full_rect_h))
            {
                tp_point.x = anim_full_rect_h.right;
            }
        }

        //calc move x
        move_x = (int16)(tp_point.x - glide_start_point.x);

        //get anim rect width
        lcd_width = (uint16)(anim_full_rect.right - anim_full_rect.left + 1);

        //is current picture
        if(abs(move_x) < (lcd_width / MMIMP3_GLIDE_RATIO))
        {
            //reset
            move_width = (int16)(glide_start_point.x - glide_prev_point.x);
        }
        else//previous or next picture
        {
            //set move space
            if(0 < move_x)
            {
                //previous
                is_prev = TRUE;
                move_width = (int16)(lcd_width + glide_start_point.x - glide_prev_point.x);
            }
            else
            {
                //next
                is_next = TRUE;
                move_width = (int16)(glide_start_point.x - glide_prev_point.x - lcd_width);
            }
        }

        //auto move
        for(i = 1; i < MMIMP3_GLIDE_AUTO_FRAME; i++)
        {
            //set move space
            if(i <= (MMIMP3_GLIDE_AUTO_FRAME / 2))
            {
                move_offset = (int16)((2 * move_width * i * i) / 100);
            }
            else
            {
                move_offset = (int16)(move_width - (2 * move_width * (MMIMP3_GLIDE_AUTO_FRAME - i) * (MMIMP3_GLIDE_AUTO_FRAME - i)) / 100);
            }

            //set move space
            move_space = (int16)(move_offset - move_all);
            move_all   = (int16)(move_all + move_space);

            if(0 == move_space)
            {
                break;
            }
            else
            {
                //set control rect and display rect
                MMIAP_SetAnimCtrlRectDisplayRect(move_space, left_ctrl_id);
                MMIAP_SetAnimCtrlRectDisplayRect(move_space, right_ctrl_id);
                MMIAP_SetAnimCtrlRectDisplayRect(move_space, mid_ctrl_id);

                //glide is move
                MMIAP_SetAlbumGlideMove(TRUE);

                //close osd panel
                if(MMIAP_IsOpenOsdPanel())
                {
                    MMIAP_CloseMp3OsdPanel(TRUE);

                    //set reopen flag
                    MMIAP_SetReopenOsdPanelFlag(TRUE);
                }

                //update
                MMITHEME_UpdateRect();
            }
        }

        //modify control id and load
        MMIAP_ModifyAnimGlideCtrlId(is_prev, is_next);

        //set control rect and display rect
        MMIAP_SetLeftAnimRect(TRUE);
        MMIAP_SetRightAnimRect(TRUE);
        MMIAP_SetMidAnimRect(TRUE);
        //update
        MMITHEME_UpdateRect();

        //play pre or next mp3
        if(is_prev)
        {
            MMIAPIMP3_GoToPreOrNextMp3(TRUE);
        }
        else if(is_next)
        {
            MMIAPIMP3_GoToPreOrNextMp3(FALSE);
        }
        else
        {
            //nothing to do
        }

        if(is_prev || is_next)
        {
            //re-get album info and set anim param, before this prev or next mp3 need start play
            MMIAP_UpdateAllAlbumInfo(is_prev);
            MMIAP_DisplayAlbumTitleInfo(TRUE); //update title info
            MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_LEFT, FALSE);
            MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_MID, FALSE);
            MMIAP_SetAnimCtrlParam(MMIAP_DIRECT_RIGHT, FALSE);
        }
    }

    if(MMIAP_IsAlbumGlideMove()) /*lint !e774*/
    {
        ret = TRUE;


        MMIAP_SetAlbumGlideMove(FALSE);

        if(MMIAP_IsNeedReopenOsdPanel())
        {
            //open osd panel
            MMIAP_OpenMp3OsdPanel(TRUE);

            //reset flag
            MMIAP_SetReopenOsdPanelFlag(FALSE);
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description : init album mng info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitAlbumMngInfo(
    void
)
{
    BOOLEAN ret = FALSE;


    ret = CreateAPAlbumMngInfo();
    if (!ret)
    {
        return ret;
    }


    return ret;
}
#endif

