/*****************************************************************
 **  File Name: 	brw_dlinterface.h 							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:download interface declaration                  *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  16/04/2009		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef BRW_DLINTERFACE_H
#define BRW_DLINTERFACE_H

#define BRW_INVALID_OMA_DL_SESSION_ID   0

/*
 *	desc:start a download request
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 *  return:TRUE if success,eles FALSE
 */
PUBLIC BOOLEAN BrwStartDL(BRW_INFO_T* brw_info_ptr, WAP_REQUEST_T* req_ptr, char *hear_ptr);
/*
 *	desc:cancel a download request
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 *  return:TRUE if success,eles FALSE
 */
PUBLIC BOOLEAN  BrwCancelDL(WAP_REQUEST_T* req_ptr,BRW_INFO_T* brw_info_ptr);
/*
 *	desc: free download relevant resource
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 */
PUBLIC  void BrwFreeDLResource(WAP_REQUEST_T* req_ptr,BRW_INFO_T* brw_info_ptr);
/*
 *	desc:process download success case
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 */
PUBLIC void BrwProcDLSuccess(WAP_REQUEST_T* req_ptr,BRW_INFO_T* brw_info_ptr);
#ifdef OMADL_SUPPORT
/*
 *	desc:start a oma download
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwStartOMADL(WAP_REQUEST_T* req_ptr,
            BRW_INFO_T* brw_info_ptr,
            const uint16* file_name_ptr,
            uint16 file_name_len);

/*
 *	desc:oma download confirm response
 *  param:wap_req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 *        file_name_ptr unicode format file name pointer
 *        file_name_len length of file name
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwOMADLResp(WAP_REQUEST_T* wap_req_ptr,BRW_INFO_T* brw_info_ptr,const uint16* file_name_ptr,uint16 file_name_len);
/*
 *	desc:process http task's signal:OMA Download
 *  param:msg_id  message id
 *        param   message parameter
 *  TRUE if msg is http's message,else FALSE
 */
PUBLIC BOOLEAN BrwProcODMMsg(uint32 msg_id,void* param);
#endif

PUBLIC BOOLEAN BrwWspDownloadDataIn(
									WAP_REQUEST_T *wap_req_ptr,
									uint8 *buf_ptr,
									uint32 data_size,
									HTTP_REQ_INFO_T *http_req_ptr);

/*****************************************************************************/
// Description : Get Default File Name
// Global resource dependence :
// Author: fen.xie
// Note:  copy from :MMIDL_GetDefaultFileName
/*****************************************************************************/
PUBLIC BOOLEAN Brw_DLGetDefaultFileName(wchar *file_name_ptr, 
                uint32 name_len, 
                const uint8 * url_ptr, 
                BOOLEAN is_url_can_without_slash,
                const uint8 * http_header_ptr, 
                uint32 mime_type);

#ifdef BROWSER_GZIP_SUPPORT
/*****************************************************************************/
// Description : set s_dl_temp_file_name_ptr(alloc memory)
// Global resource dependence :
// Author:  fen.xie
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwSetDlTempFilename(const uint16*file_name_ptr, uint16 file_name_len);

/*****************************************************************************/
// Description : delete the file of s_dl_temp_file_name_ptr and free it
// Global resource dependence :
// Author:  fen.xie
// Note:
/*****************************************************************************/
PUBLIC void BrwDelDlTempFile(void);
#endif

#endif
