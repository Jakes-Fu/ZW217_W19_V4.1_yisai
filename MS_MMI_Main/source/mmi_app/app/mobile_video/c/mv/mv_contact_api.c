#define _MV_CONTACT_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_id.h"
#include "mmi_appmsg.h"
#include "mmisms_export.h"
#include "mmisms_app.h" 
#include "_Types.h"
#include "sfs.h"
#include "mv_debug.h"
#include "sci_api.h"
#include "mv_contact_api.h"
#include "mv_base_api.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL ContactDialogCallbackProc s_cm_callback = PNULL;
LOCAL _ContactItem * s_cm_contactitem = PNULL;
LOCAL _VOID * s_cm_param = PNULL;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
	PHONEBOOK_NAME,
	PHONEBOOK_PHONENUM
}PHONEBOOK_ID;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/************************************************************************/
//初始化一条联系人信息
//Global resource dependence
//Author:jiang min
//note:为联系人分配空间(contactdialog_release()释放)
/************************************************************************/
LOCAL void InitContactItem(_ContactItem * item);

/************************************************************************/
//设置联系人信息
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
LOCAL void SetContactItem(_ContactItem * item,PHONEBOOK_ID id,MMI_STRING_T *str);



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/************************************************************************/
//获得联系人信息
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
PUBLIC _ContactItem * MV_GetContactInfo(uint32 handle, uint16 max_len)
{
	_ContactItem *tmpcontact = PNULL;
	_ContactItem *tmpcontactnext = PNULL;
	MMI_TEXT_ID_T number_label = TXT_NULL;
	MMIPB_BCD_NUMBER_T bcd_number = {0};
	MMI_STRING_T name_str = {0};
	MMI_STRING_T tmp_buf_num = {0};
	uint16 tele_len = 0;
	uint8 tele_num[MMISMS_NUMBER_MAX_LEN+2] = {0};
    uint16 pb_select_num=0;
    uint8 pb_select_index=0;

    UNUSED(max_len);
	pb_select_num = MMIAPIPB_GetSelectCount((MMIPB_HANDLE_T)handle);

    //MV_TRACE_LOW:"[MV] GetContactInfo pb_select_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_CONTACT_API_79_112_2_18_2_39_10_14,(uint8*)"d",pb_select_num);

	if (pb_select_num > 0)
	{		   
	   name_str.wstr_len = MMISMS_PBNAME_MAX_LEN + 1;
	   name_str.wstr_ptr = SCI_ALLOCA((MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar));
	   if(PNULL == name_str.wstr_ptr)
       {
           return PNULL;
       }	      
	   tmp_buf_num.wstr_len = MMISMS_PBNAME_MAX_LEN+1;
	   tmp_buf_num.wstr_ptr = SCI_ALLOCA((MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar));
	   if(PNULL == tmp_buf_num.wstr_ptr)
       {
           return PNULL;
       }
       
	   for (pb_select_index = 0; pb_select_index < pb_select_num; pb_select_index++)
	   {
		   SCI_MEMSET(name_str.wstr_ptr,0,((MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar)));
           SCI_MEMSET(tmp_buf_num.wstr_ptr,0,(tmp_buf_num.wstr_len*sizeof(wchar)));

		   MMIAPIPB_GetSelectNumber((MMIPB_HANDLE_T)handle, &bcd_number,MMIPB_BCD_NUMBER_MAX_LEN,pb_select_index);
	       MMIAPIPB_GetSelectName((MMIPB_HANDLE_T)handle, &name_str,MMISMS_PBNAME_MAX_LEN,pb_select_index);
	   
	   	   tmpcontactnext = (_ContactItem *)SCI_ALLOCA(sizeof(_ContactItem));
		   if(PNULL == tmpcontactnext)
           {
               return PNULL;
           }
		   SCI_MEMSET(tmpcontactnext,0,sizeof(_ContactItem));

		    if (0 == pb_select_index)
            {
                s_cm_contactitem = tmpcontactnext;
                tmpcontact = tmpcontactnext;
            }
			else if(PNULL != tmpcontact)
			{
				tmpcontact->m_pNext = tmpcontactnext;
				tmpcontact = tmpcontactnext;				
			}
		   
		   InitContactItem(tmpcontactnext);
		   SetContactItem(tmpcontactnext,PHONEBOOK_NAME,&name_str);
           tele_len = MMIAPIPB_BCDNumberToString(&bcd_number, MMISMS_NUMBER_MAX_BCD_LEN + 2, tele_num);
// 		   tele_len = MMIAPICOM_GenDispNumber(
// 			   MMIPB_GetNumberTypeFromUint8(bcd_number.npi_ton),
// 			   (uint8)MIN(MMISMS_NUMBER_MAX_BCD_LEN,bcd_number.number_len),
// 			   bcd_number.number,
// 			   tele_num,
// 			   MMISMS_NUMBER_MAX_LEN+2
// 			   );
		   MMIAPICOM_StrToWstr((uint8 *)tele_num,tmp_buf_num.wstr_ptr);//,MIN(tmp_buf_num.wstr_len,tele_len));
		   SetContactItem(tmpcontactnext,PHONEBOOK_PHONENUM,&tmp_buf_num);
		
	   } 	
	   SCI_FREE(tmp_buf_num.wstr_ptr);
	   SCI_FREE(name_str.wstr_ptr);

	}
	return s_cm_contactitem;
}



/************************************************************************/
//设置联系人信息
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
LOCAL void SetContactItem(_ContactItem * item,PHONEBOOK_ID id,MMI_STRING_T *str)
{
	if (PNULL == item || PNULL == str)
	{
        return;
	}	
	//MV_TRACE_LOW:"[MV] SetContactItem id =%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_CONTACT_API_155_112_2_18_2_39_10_15,(uint8*)"d",SetContactItem);

	switch(id)
	{
	case PHONEBOOK_NAME:
		MMIAPICOM_Wstrncpy(item->m_strName,str->wstr_ptr, MMISMS_PBNAME_MAX_LEN);
		break;
	case PHONEBOOK_PHONENUM:
		MMIAPICOM_Wstrncpy(item->m_strTel, str->wstr_ptr, MMIPB_BCD_NUMBER_MAX_LEN);
		break;
	default:
		break;
	}
	return;
}

/************************************************************************/
//初始化一条联系人信息
//Global resource dependence
//Author:jiang min
//note:为联系人分配空间(contactdialog_release()释放)
/************************************************************************/
LOCAL void InitContactItem(_ContactItem * item)
{
	

	item->m_strName = PNULL;
	item->m_strTel = PNULL;
	item->m_pNext = PNULL;

	//MV_TRACE_LOW:"[MV] [InitContactItem]run in InitContactItem"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_CONTACT_API_183_112_2_18_2_39_10_16,(uint8*)"");
	item->m_strName = (_WCHAR*)SCI_ALLOCA((MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar));
	item->m_strTel =  (_WCHAR*)SCI_ALLOCA((MMIPB_BCD_NUMBER_MAX_LEN+1)*sizeof(wchar));
	if (PNULL == item->m_strName
        || PNULL == item->m_strTel)
	{
        return;
	}
	SCI_MEMSET(item->m_strName,0,((MMISMS_PBNAME_MAX_LEN+1)*sizeof(wchar)));
	SCI_MEMSET(item->m_strTel,0,((MMIPB_BCD_NUMBER_MAX_LEN+1)*sizeof(wchar)));
	//MV_TRACE_LOW:"[MV] [InitContactItem] exit InitContactItem"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_CONTACT_API_193_112_2_18_2_39_10_17,(uint8*)"");
}


/************************************************************************/
//MV_ContactDialogCallbackProc
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
PUBLIC void MV_ContactDialogCallbackProc(TContactDialogResultEnum nResult)
{
    //MV_TRACE_LOW:"[MV] MV_ContactDialogCallbackProc nResult = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_CONTACT_API_203_112_2_18_2_39_10_18,(uint8*)"d",nResult);
    if (EContactDialogResult_OK == nResult)
    {
        s_cm_callback(PNULL, s_cm_param, nResult, s_cm_contactitem);
    }
    else
    {
        s_cm_callback(PNULL, s_cm_param, nResult, PNULL);
    }
}

/************************************************************************/
//CM_ContactDialog_Create
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
_HANDLE CM_ContactDialog_Create(ContactDialogCallbackProc pCallbackProc,
										_VOID* pParam )
{
    
	MMI_WIN_ID_T win_id = MMI_CONTACT_WIN_ID;
	s_cm_param = pParam;
	s_cm_callback = pCallbackProc;
	
    if (PNULL == s_cm_param
        || PNULL == s_cm_callback)
    {
        //MV_TRACE_LOW:"[MV] CM_ContactDialog_Create input param is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_CONTACT_API_229_112_2_18_2_39_10_19,(uint8*)"");
        return 0;
    }

	if (!MMK_IsOpenWin(win_id))
	{
	     //MV_TRACE_LOW:"--WD[CM_ContactDialog_Create]contact window d'not open"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_CONTACT_API_235_112_2_18_2_39_10_20,(uint8*)"");
	     CM_SendSignalToMMI(MVIDEO_CONTACT_OPEN, 0);	     
	}
				
	return (_HANDLE)win_id;	
	
}

/************************************************************************/
//CM_ContactDialog_Release
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
_BOOLEAN CM_ContactDialog_Release(_HANDLE hDialog)
{
	_ContactItem *headitem = PNULL;
	_ContactItem *ptmp = PNULL;
    
	if (MMI_CONTACT_WIN_ID != (MMI_WIN_ID_T)hDialog)
    {
        return _FALSE;
    }

	//MV_TRACE_LOW:"run in CM_ContactDialog_Release"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_CONTACT_API_257_112_2_18_2_39_10_21,(uint8*)"");
	headitem = s_cm_contactitem;
	while (PNULL != headitem)
	{
		ptmp = headitem->m_pNext;

		if (headitem->m_strName != PNULL)
		{
		    SCI_FREE(headitem->m_strName);
		}

		if (headitem->m_strTel != PNULL)
		{
    		SCI_FREE(headitem->m_strTel);
        }

		SCI_FREE(headitem);
		headitem = ptmp;
	}
	s_cm_callback = PNULL;
	s_cm_param = PNULL;	
	s_cm_contactitem = PNULL;	
	return _TRUE;
}

/************************************************************************/
//CM_ContactDialog_Run
//Global resource dependence
//Author:jiang min
//note:
/************************************************************************/
_BOOLEAN CM_ContactDialog_Run (_HANDLE hDialog)
{

	MMI_WIN_ID_T win_id = MMI_CONTACT_WIN_ID;

	if (win_id != (MMI_WIN_ID_T)hDialog)
	{
        return _FALSE;
    }
	   
	if (MMK_IsOpenWin(win_id))
	{
		return _TRUE;
	}
	
	return _FALSE;
	
}



