/*************************************************************************
 ** File Name:       mmiwww_nv.c                                           *
 ** Author:           kun.yu                                          *
 ** Date:              06/26/2009                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 06/26/2009     kun.yu      Create.                              *
*************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#include "sci_types.h"
#include "mmi_modu_main.h"
#include "mmiwww_nv.h"
#include "mmi_nv.h"

#ifdef BROWSER_SUPPORT_NETFRONT
/*the length of mmiwww nv*/
const uint16 www_nv_len[] =
{
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( uint16 ), 
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( uint16 ),
    sizeof( MMIWWW_KEY_T ),
    sizeof( uint8 ),
    sizeof(MMIWWW_LINK_SETTING_T),
    sizeof( uint8 ),
    sizeof( BOOLEAN ),
    sizeof( uint16 ),
    sizeof( MMIWWW_POINTER_MODE_E ),
    sizeof( char ) * (MAX_WEB_URL_LEN + 1),
};

/*****************************************************************************/
// 	Description : register mmiwww module nv len and max item
//	Global resource dependence : none
//  Author: kun.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_WWW, www_nv_len, sizeof(www_nv_len)/sizeof(uint16));
}

/*****************************************************************************/
// 	Description : write key name to nv list
//	Global resource dependence : none
//  Author: kun.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NV_WriteKeyItem( MMI_STRING_T *in_url )
{
    uint16 i;
    uint16 str_len = 0;
    MMIWWW_KEY_T key = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    
    MMINV_READ( MMINV_WWW_KEY, &key, return_value );

    for( i = 0; i < key.num; i++ )
    {
        str_len = (uint16)MAX(key.key_arr[i].wstr_len,in_url->wstr_len);
        if( !MMIAPICOM_Wstrncmp(key.key_arr[i].wstr, in_url->wstr_ptr, str_len ) )
        {
            return;
        }
    }
    
    if( key.num < MMIWWW_KEYNAME_NUM )
    {
        for( i = key.num; i > 0; i-- )
        {
            SCI_MEMSET(key.key_arr[i].wstr, 0, ((MMIWWW_MAX_KEYNAME_LEN+1)*sizeof(wchar)));
            MMI_WSTRNCPY(
                key.key_arr[i].wstr,
                MMIWWW_MAX_KEYNAME_LEN,
                key.key_arr[i-1].wstr,
                MMIWWW_MAX_KEYNAME_LEN,
                MMIWWW_MAX_KEYNAME_LEN
                );
            key.key_arr[i].wstr_len = key.key_arr[i-1].wstr_len;
        }
        SCI_MEMSET(key.key_arr[0].wstr, 0, ((MMIWWW_MAX_KEYNAME_LEN+1)*sizeof(wchar)));
        MMI_WSTRNCPY( 
            key.key_arr[0].wstr,
            MMIWWW_MAX_KEYNAME_LEN,
            in_url->wstr_ptr,
            MMIWWW_MAX_KEYNAME_LEN,
            MMIWWW_MAX_KEYNAME_LEN
            );
        key.key_arr[0].wstr_len = in_url->wstr_len;
        key.num++;
    }
    else
    {
        for( i = MMIWWW_KEYNAME_NUM - 1; i > 0; i-- )
        {
            SCI_MEMSET(key.key_arr[i].wstr, 0, ((MMIWWW_MAX_KEYNAME_LEN+1)*sizeof(wchar)));
            MMI_WSTRNCPY(
                key.key_arr[i].wstr,
                MMIWWW_MAX_KEYNAME_LEN,
                key.key_arr[i-1].wstr,
                MMIWWW_MAX_KEYNAME_LEN,
                MMIWWW_MAX_KEYNAME_LEN
                );
            key.key_arr[i].wstr_len = key.key_arr[i-1].wstr_len;
        }
        SCI_MEMSET(key.key_arr[0].wstr, 0, ((MMIWWW_MAX_KEYNAME_LEN+1)*sizeof(wchar)));
        MMI_WSTRNCPY( 
            key.key_arr[0].wstr,
            MMIWWW_MAX_KEYNAME_LEN,
            in_url->wstr_ptr,
            MMIWWW_MAX_KEYNAME_LEN,
            MMIWWW_MAX_KEYNAME_LEN
            );
        key.key_arr[0].wstr_len = in_url->wstr_len;
    }
    MMINV_WRITE( MMINV_WWW_KEY, &key );
}

#endif //#ifdef BROWSER_SUPPORT_NETFRONT
