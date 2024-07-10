/*****************************************************************************
** File Name:      mmi_resource.c                                               *
** Author:                                                                   *
** Date:           10/03/2009                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to control mmi module                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2008        andrew.zhang     Creat
******************************************************************************/



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : load resource and get every module offset
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL void CAF_GetComResHeadInfo( CAF_RES_T *pRes )
{    
    int32 i = 0;
    uint32 module_offset = 0;

    pRes->caf_res_header_ptr = (CAF_MMI_MODULE_TOTAL_INFO_T*)ResMalloc(pRes->res_location.store_type, sizeof(CAF_MMI_MODULE_TOTAL_INFO_T));
//#ifdef MULTI_THEME_SUPPORT 
#if 1
    pRes->caf_res_theme_info_ptr = (CAF_RESOURCE_HEADER_INFO*)ResMalloc(pRes->res_location.store_type, sizeof(CAF_RESOURCE_HEADER_INFO));
    ReadResource( pRes->res_handle, pRes->caf_res_theme_info_ptr, 0, sizeof(CAF_RESOURCE_HEADER_INFO), pRes->res_location.store_type );        
    ReadResource( pRes->res_handle, pRes->caf_res_header_ptr, pRes->caf_res_theme_info_ptr->default_resource_info.resource_offset, sizeof(CAF_MMI_MODULE_TOTAL_INFO_T), pRes->res_location.store_type );    
    SCI_ASSERT(pRes->caf_res_theme_info_ptr->addition_resource_num < CAF_MAX_ADDT_RESOURCE_NUM);
    //MMI_SetCurrentThemeNum(pRes->caf_res_theme_info_ptr->addition_resource_num);
#else
    ReadResource( pRes->res_handle, pRes->caf_res_header_ptr, 0, sizeof(CAF_MMI_MODULE_TOTAL_INFO_T), pRes->res_location.store_type );
#endif
    /* get module total number */
    pRes->res_module_total = 0;
    for(i = MMI_MAX_MODU_NUM-1; i >= 0; i --)
    {        
        if(NULL != pRes->caf_res_header_ptr->mmi_module_adrr[i])
        {   
            break;
        }        
    }
    pRes->res_module_total = i+1;

    SCI_ASSERT(pRes->res_module_total > 0);
    
    pRes->caf_res_module_arry = (CAF_MMIRES_MODULE_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(CAF_MMIRES_MODULE_INFO_T)*pRes->res_module_total );
    SCI_MEMSET(pRes->caf_res_module_arry, 0, (sizeof(CAF_MMIRES_MODULE_INFO_T)*pRes->res_module_total));
    
    for (i = 0; i < pRes->res_module_total; i ++)
    {        
//#ifdef MULTI_THEME_SUPPORT
#if 1
        module_offset = pRes->caf_res_theme_info_ptr->default_resource_info.resource_offset + pRes->caf_res_header_ptr->mmi_module_adrr[i];
#else
        module_offset = pRes->caf_res_header_ptr->mmi_module_adrr[i];
#endif

        if (0 == pRes->caf_res_header_ptr->mmi_module_adrr[i])
        {
            continue;
        }

        ReadResource( pRes->res_handle, &(pRes->caf_res_module_arry[i]), module_offset, sizeof(CAF_MMIRES_MODULE_INFO_T), pRes->res_location.store_type );
    }
}

/*****************************************************************************/
//  Description : load image resource infomation
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL void CAF_GetImageResInfo(CAF_RES_T *pRes)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 offset = 0;
    uint32 module_offset = 0;   
    uint32 count = 0;
    IMG_FLAG_T image_flag = {0};    
    MMI_IMG_DATA_T image_data = {0};

    
    pRes->caf_image_module_arry = (CAF_RES_COMMON_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(CAF_RES_COMMON_INFO_T)*pRes->res_module_total);
    SCI_MEMSET(pRes->caf_image_module_arry, 0, (sizeof(CAF_RES_COMMON_INFO_T)*pRes->res_module_total));
    for (i = 0; i < pRes->res_module_total; i ++)
    {                        
        //计算模块相对位移
//#ifdef MULTI_THEME_SUPPORT
#if 1
        module_offset = pRes->caf_res_theme_info_ptr->default_resource_info.resource_offset+pRes->caf_res_header_ptr->mmi_module_adrr[i];
#else
        module_offset = pRes->caf_res_header_ptr->mmi_module_adrr[i];
#endif
        
        if(0 == module_offset)
        {
            continue;
        }
        //得到当前模块image的数量
        count = pRes->caf_res_module_arry[i].mmi_data_table[CAF_MMI_RT_IMAGE].uCount;
        if (count!=0)
        {
            offset = module_offset+(uint32)pRes->caf_res_module_arry[i].mmi_data_table[CAF_MMI_RT_IMAGE].pData;
            ReadResource( pRes->res_handle, &image_data, offset, sizeof(MMI_IMG_DATA_T), pRes->res_location.store_type );
            
            //分配count RES_IMAGE_INFO_T数据空间
            pRes->caf_image_module_arry[i].common_info_arry = (CAF_RES_UNIT_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(CAF_RES_UNIT_INFO_T)*count);
            pRes->caf_image_module_arry[i].common_total = count;
            
            for (j = 0; j < count; j ++)
            {
                ReadResource( pRes->res_handle, &(pRes->caf_image_module_arry[i].common_info_arry[j].data_addr), (uint32)(image_data.pImgData) + module_offset+j*sizeof(uint32), sizeof(uint32), pRes->res_location.store_type );
                ReadResource( pRes->res_handle, &image_flag, module_offset+(uint32)(image_data.pFlag)+(j)*sizeof(IMG_FLAG_T), sizeof(IMG_FLAG_T), pRes->res_location.store_type );
                pRes->caf_image_module_arry[i].common_info_arry[j].data_size = image_flag.nSize;
//#ifdef MULTI_THEME_SUPPORT
#if 1
                pRes->caf_image_module_arry[i].common_info_arry[j].cur_theme = 0;
#endif
            }
        }   
    }
}

/*****************************************************************************/
//  Description : load anim resource infomation
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL void CAF_GetAnimResInfo(CAF_RES_T *pRes )
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 offset = 0;
    uint32 module_offset = 0;   
    uint32 count = 0;    
    ANIM_FLAG_T anim_flag = {0};
    MMI_ANIM_DATA_T anim_data = {0};

    //SCI_TRACE_LOW("GetAnimResInfo ");
        
    pRes->caf_anim_module_arry = (CAF_RES_COMMON_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(CAF_RES_COMMON_INFO_T)*pRes->res_module_total);
    SCI_MEMSET(pRes->caf_anim_module_arry, 0, (sizeof(CAF_RES_COMMON_INFO_T)*pRes->res_module_total));
    for (i = 0; i < pRes->res_module_total; i ++)
    {                
        //计算模块相对位移
//#ifdef MULTI_THEME_SUPPORT
#if 1
        module_offset = pRes->caf_res_theme_info_ptr->default_resource_info.resource_offset+pRes->caf_res_header_ptr->mmi_module_adrr[i];
#else
        module_offset = pRes->caf_res_header_ptr->mmi_module_adrr[i];
#endif
        
        if(0 == module_offset)
        {
            continue;
        }
        //得到当前模块anim的数量
        count = pRes->caf_res_module_arry[i].mmi_data_table[CAF_MMI_RT_ANIM].uCount;
        if (count!=0)
        {
            offset = module_offset+(uint32)pRes->caf_res_module_arry[i].mmi_data_table[CAF_MMI_RT_ANIM].pData;
            ReadResource( pRes->res_handle, &anim_data, offset, sizeof(MMI_ANIM_DATA_T), pRes->res_location.store_type );
            
            //分配count RES_ANIM_INFO_T数据空间
            pRes->caf_anim_module_arry[i].common_info_arry = (CAF_RES_UNIT_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(CAF_RES_UNIT_INFO_T)*count);
            pRes->caf_anim_module_arry[i].common_total = count;
            
            for (j = 0; j < count; j ++)
            {
                ReadResource( pRes->res_handle, &(pRes->caf_anim_module_arry[i].common_info_arry[j].data_addr), (uint32)(anim_data.pAnimData) + module_offset+j*sizeof(uint32), sizeof(uint32), pRes->res_location.store_type );
                ReadResource( pRes->res_handle, &anim_flag, module_offset+(uint32)(anim_data.pFlag)+(j)*sizeof(ANIM_FLAG_T), sizeof(ANIM_FLAG_T), pRes->res_location.store_type );
                pRes->caf_anim_module_arry[i].common_info_arry[j].data_size = anim_flag.nDataSize;

//#ifdef MULTI_THEME_SUPPORT
#if 1
                pRes->caf_anim_module_arry[i].common_info_arry[j].cur_theme = 0;
#endif
            }
        }   
    }
}

/*****************************************************************************/
//  Description : load ring resource infomation
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL void CAF_GetRingResInfo(CAF_RES_T *pRes)
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 offset = 0;
    uint32 module_offset = 0;   
    uint32 count = 0;    
    MMIAUD_RING_DATA_INFO_T ring_info = {0};        
    
    pRes->caf_ring_module_arry = (CAF_RES_COMMON_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(CAF_RES_COMMON_INFO_T)*pRes->res_module_total);
    SCI_MEMSET(pRes->caf_ring_module_arry, 0, (sizeof(CAF_RES_COMMON_INFO_T)*pRes->res_module_total));
    
    for (i = 0; i < pRes->res_module_total; i ++)
    {             
        //计算模块相对位移
        //#ifdef MULTI_THEME_SUPPORT
#if 1
        module_offset = pRes->caf_res_theme_info_ptr->default_resource_info.resource_offset+pRes->caf_res_header_ptr->mmi_module_adrr[i];
#else
        module_offset = pRes->caf_res_header_ptr->mmi_module_adrr[i];
#endif
        
        if(0 == module_offset)
        {
            continue;
        }
        //得到当前模块ring的数量
        count = pRes->caf_res_module_arry[i].mmi_data_table[CAF_MMI_RT_RING].uCount;
        if (count!=0)
        {
            offset = module_offset+(uint32)pRes->caf_res_module_arry[i].mmi_data_table[CAF_MMI_RT_RING].pData;
            //ReadResource( pRes->res_handle, &ring_info, offset, sizeof(MMIAUD_RING_DATA_INFO_T), pRes->res_location.store_type );
            
            //分配count RES_ANIM_INFO_T数据空间
            pRes->caf_ring_module_arry[i].common_info_arry = (CAF_RES_UNIT_INFO_T*)ResMalloc( pRes->res_location.store_type, sizeof(CAF_RES_UNIT_INFO_T)*count);
            pRes->caf_ring_module_arry[i].common_total = count;
            
            for (j = 0; j < count; j ++)
            {
                //ReadResource( pRes->res_handle, &(pRes->caf_ring_module_arry[i].common_info_arry[j].data_addr), (uint32)(ring_info.data_ptr) + module_offset+j*sizeof(uint32), sizeof(uint32), pRes->res_location.store_type );
                ReadResource( pRes->res_handle, &ring_info, offset, sizeof(MMIAUD_RING_DATA_INFO_T), pRes->res_location.store_type );
                
                //pRes->caf_ring_module_arry[i].common_info_arry[j].data_addr = (uint32)(ring_info.data_ptr) + j*sizeof(uint32);
                pRes->caf_ring_module_arry[i].common_info_arry[j].data_addr = (uint32)(ring_info.data_ptr);
                
                pRes->caf_ring_module_arry[i].common_info_arry[j].data_size = ring_info.data_len;
                
                //#ifdef MULTI_THEME_SUPPORT
#if 1
                pRes->caf_ring_module_arry[i].common_info_arry[j].cur_theme = (uint8)ring_info.type;
#endif
                offset += sizeof(MMIAUD_RING_DATA_INFO_T);
            }
        }   
    }
}

/*****************************************************************************/
//  Description : load text resource infomation
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL void CAF_LoadTextRes(CAF_RES_T *pRes )
{
    uint8 * data_buffer = PNULL;
    uint32 * offset_buffer = PNULL;
    uint32  size= 0;
    uint32  i = 0 ;    
    uint32  offset = 0;
    uint32  data_offset = 0;
    uint32  count = 0;    
    KSP_TEXT_T *ksp_text_ptr = PNULL;
    RES_MODULE_DATA_T *module_data_ptr = PNULL;
    MMISET_LANGUAGE_TYPE_E  language_type = MMISET_EDEFAULT_LANGUAGE;
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;        
    uint32 index = 0;
    
    /* read the type of language */
    MMINV_READ(MMINV_SET_LANGUAGE_TYPE,&language_type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_SET_LANGUAGE_TYPE,&language_type);
    }

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //做一次实际的系统语言类型的转换，nv中的数据包含 auto类型，需要转换为确切的语言。
    language_type = (MMISET_LANGUAGE_TYPE_E)MMIAPISET_ConvertNVLangToSysType(language_type);
#endif

    index = GetCurLangIndex(language_type);
    
    /* 集中申请MMISET_MAX_LANGUAGE个KSP_TEXT_T数据空间,存放每种语言的text信息 */
    /* 这个buffer是用来读取每个project module中文字模块的多国语言信息的，
        然后再根据当前的语言language_type来选择对应的文字 */
    //if (PNULL == ksp_text_ptr)
    {
        ksp_text_ptr = (KSP_TEXT_T*)ResMalloc( pRes->res_location.store_type, sizeof(KSP_TEXT_T)*MMISET_MAX_LANGUAGE);
        SCI_ASSERT(ksp_text_ptr!=PNULL);
    }

    
    for (i =0; i<pRes->res_module_total; i++)
    {   
        if(0 == pRes->caf_res_header_ptr->mmi_module_adrr[i])
        {
            continue;
        }

        /* malloc module buffer */
        module_data_ptr = (RES_MODULE_DATA_T*)ResMalloc( pRes->res_location.store_type, sizeof(RES_MODULE_DATA_T));
        SCI_MEMSET(module_data_ptr, 0x00, sizeof(RES_MODULE_DATA_T));

        /* get every project module info */
        data_offset = (uint32)pRes->caf_res_module_arry[i].mmi_data_table[CAF_MMI_RT_TEXT].pData;
        count = (uint32)pRes->caf_res_module_arry[i].mmi_data_table[CAF_MMI_RT_TEXT].uCount;
//#ifdef MULTI_THEME_SUPPORT
#if 1
        offset =pRes->caf_res_theme_info_ptr->default_resource_info.resource_offset + pRes->caf_res_header_ptr->mmi_module_adrr[i] + data_offset;
#else
        offset = pRes->caf_res_header_ptr->mmi_module_adrr[i] + data_offset;
#endif
        /* get multi country language info of every project module */
        SCI_MEMSET(ksp_text_ptr, 0x00, (sizeof(KSP_TEXT_T)*MMISET_MAX_LANGUAGE));
        ReadResource( pRes->res_handle, ksp_text_ptr, offset, sizeof(KSP_TEXT_T)*MMISET_MAX_LANGUAGE, pRes->res_location.store_type );

        /* get offset of indicated language text data */
//#ifdef MULTI_THEME_SUPPORT
#if 1
        offset = (uint32)ksp_text_ptr[index].pTxtTable+ pRes->caf_res_header_ptr->mmi_module_adrr[i] + pRes->caf_res_theme_info_ptr->default_resource_info.resource_offset;
#else
        offset = (uint32)ksp_text_ptr[index].pTxtTable + pRes->caf_res_header_ptr->mmi_module_adrr[i];
#endif
        size = (uint32)(ksp_text_ptr[index].nbText) * 4;//该语言该模块的text data大小
        
        if ( (0 != size) && (0 != count) )
        {
            offset_buffer = (uint32 *)ResMalloc( pRes->res_location.store_type, count*sizeof(uint32));
            SCI_ASSERT(offset_buffer != PNULL);
            SCI_MEMSET(offset_buffer, 0x00, (count*sizeof(uint32)));            
            ReadResource( pRes->res_handle, offset_buffer, offset, count*sizeof(uint32), pRes->res_location.store_type );
                      
//#ifdef MULTI_THEME_SUPPORT
#if 1
            data_offset = *offset_buffer+ pRes->caf_res_header_ptr->mmi_module_adrr[i] + pRes->caf_res_theme_info_ptr->default_resource_info.resource_offset;
#else
            data_offset = *offset_buffer + pRes->caf_res_header_ptr->mmi_module_adrr[i];
#endif
            data_buffer = (uint8 *)ResMalloc( pRes->res_location.store_type, size);
            SCI_ASSERT(data_buffer != PNULL);
            SCI_MEMSET(data_buffer, 0x00, sizeof(size));            
            ReadResource( pRes->res_handle, data_buffer,data_offset,size, pRes->res_location.store_type );

            //为每个text节点指定一个head数据空间
            module_data_ptr->module_id = i;
            module_data_ptr->size = size;
            module_data_ptr->unit_data_ptr = data_buffer;
            module_data_ptr->unit_offset_arry = offset_buffer;

            /* add module to link */
            if(0 == i)
            {
                SCI_ASSERT(PNULL == pRes->text_prj_module_link_ptr);
                pRes->text_prj_module_link_ptr = MMILINK_CreateHead((uint32)module_data_ptr);
            }
            else
            {
                MMILINK_AddNodeBeforeBaseNode(pRes->text_prj_module_link_ptr, MMILINK_CreateNode((uint32)module_data_ptr));
            }                        
        }
    }
    /* free 多国语言buffer */
    //if ( PNULL != ksp_text_ptr)
    {
        ResFree((uint8*)ksp_text_ptr);
        ksp_text_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : load resource infomation from a resource file
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CAF_LoadResourceData(CAF_RES_T * res_info_ptr)
{
    if(res_info_ptr == NULL)
    {
        return FALSE;
    }
    
    res_info_ptr->res_handle = OpenResource( res_info_ptr->res_location.store_type, res_info_ptr->res_location.res_file );        
    
    /* get resource header info */
    CAF_GetComResHeadInfo( res_info_ptr );
    /* get image info */
    CAF_GetImageResInfo(res_info_ptr);
    /* get animation info */
    CAF_GetAnimResInfo(res_info_ptr);
    
    /* get ring info */
    CAF_GetRingResInfo(res_info_ptr);
    
    /* load text content */
    CAF_LoadTextRes(res_info_ptr);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : get text resource
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL const wchar* CAFRES_GetText(
                                   MMI_TEXT_ID_T      text_id,     //[in] the text id 
                                   CAF_HANDLE_T       handle_id,
                                   MMI_STRING_T       *str_ptr
                                   )    //[out] the data of text
{
    uint8* re_str_ptr = PNULL;    
    uint32  com_id  = 0;
    CAF_RES_T*  caf_res_ptr = PNULL;
    uint16 module_id = 0;
    uint16 text_index = 0;    
    uint32 offset = 0;        
    MMI_LINK_NODE_T * module_link_node_ptr = PNULL;
    RES_MODULE_DATA_T* prj_module_ptr = PNULL;

    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(text_id))
    {
        return PNULL;
    }

    if( MMI_SYS_RES_FLAG == ( text_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }
    
    text_id = text_id & ( ~MMI_SYS_RES_FLAG );

    if( 0 == com_id )
    {
        return PNULL;
    }
    
    caf_res_ptr = GetComCafResInfo(com_id);   
    SCI_ASSERT(PNULL != caf_res_ptr);

    module_id = MMI_GET_MODU_ID(text_id);
    text_index = MMICOM_GET_WORD_LB(text_id);    
        
    module_link_node_ptr = MMILINK_SearchNode(caf_res_ptr->text_prj_module_link_ptr, TRUE, FoundModuleIDCallback, module_id, 0);
    SCI_ASSERT(module_link_node_ptr != PNULL);

    prj_module_ptr = (RES_MODULE_DATA_T*)(module_link_node_ptr->data);
    offset = prj_module_ptr->unit_offset_arry[text_index-1] - prj_module_ptr->unit_offset_arry[0];
    re_str_ptr = (uint8 *)(prj_module_ptr->unit_data_ptr + offset);    

    SCI_ASSERT( PNULL != str_ptr );
    if(MMI_IsTextNull(text_id))
    {
        //不正确的text_id
        return PNULL;
    }

    switch (*re_str_ptr)
    {
    case 0x80:
        str_ptr->wstr_ptr  = (uint16*)(re_str_ptr + 4); //for memory alignment
        //str_ptr->is_ucs2 = TRUE;
        str_ptr->wstr_len  = *((uint16*)(re_str_ptr + 2)) / sizeof(wchar);//(((*(re_str_ptr + 2)<<8) & 0xFF00) + *(re_str_ptr + 3))/sizeof(wchar);
        break;

    case 0x81://????
        SCI_ASSERT (0);
        break;

    case 0x82:
        SCI_ASSERT (0);
        break;

    default:
        GetAsciiResText(str_ptr, re_str_ptr);
        break;
     }

    return str_ptr->wstr_ptr;
}

/*****************************************************************************/
//  Description : get image resource
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL uint8* CAFRES_GetImage(
                              MMI_IMAGE_ID_T image_id, 
                              CAF_HANDLE_T handle_id, 
                              uint32 *size_ptr
                              )
{
    uint32  com_id  = 0;
    uint8 * ret_ptr = PNULL;
    RES_NODE_DATA_T* res_node_ptr = PNULL;      
    CAF_RES_T*  caf_res_ptr = PNULL;

    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(image_id))
    {
        return PNULL;
    }
    
    if( MMI_SYS_RES_FLAG == ( image_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }

    image_id = image_id & ( ~MMI_SYS_RES_FLAG );
    
    if( 0 == com_id )
    {
        return PNULL;
    }
       
    caf_res_ptr = GetComCafResInfo(com_id);   
    SCI_ASSERT(PNULL != caf_res_ptr);

    /* get image node */
    res_node_ptr = GetResNode(caf_res_ptr, image_id, handle_id, (MMI_RES_TYPE_E)CAF_MMI_RT_IMAGE);
    SCI_ASSERT(res_node_ptr != PNULL);
    
    /* set image size */
    if (NULL != size_ptr)
    {
        *size_ptr = res_node_ptr->size;
    }
    /* set image data ptr */
    ret_ptr = res_node_ptr->data_ptr;
    SCI_ASSERT(ret_ptr!=PNULL);
    return (uint8*)ret_ptr;
}

/*****************************************************************************/
//  Description : get anim resource
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
LOCAL uint8* CAFRES_GetAnim(MMI_ANIM_ID_T anim_id, CAF_HANDLE_T handle_id, uint32 *size_ptr)
{
    uint32  com_id  = 0;
    uint8 * ret_ptr = PNULL;
    RES_NODE_DATA_T* res_node_ptr = PNULL;      
    CAF_RES_T*  caf_res_ptr = PNULL;

    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(anim_id))
    {
        return PNULL;
    }
    
    com_id  = GetComIdByHandle( handle_id );

    if( MMI_SYS_RES_FLAG == ( anim_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }

    anim_id = anim_id & ( ~MMI_SYS_RES_FLAG );

    if( 0 == com_id )
    {
        return PNULL;
    }
    
    caf_res_ptr = GetComCafResInfo(com_id);   
    SCI_ASSERT(PNULL != caf_res_ptr);

    /* get animation node */
    res_node_ptr = GetResNode(caf_res_ptr, anim_id, handle_id, (MMI_RES_TYPE_E)CAF_MMI_RT_ANIM);    
    SCI_ASSERT(res_node_ptr != PNULL);
    
    /* set animation size */
    if (NULL != size_ptr)
    {
        *size_ptr = res_node_ptr->size;
    }
    /* set animation data ptr */
    ret_ptr = res_node_ptr->data_ptr;
    SCI_ASSERT(ret_ptr!=PNULL);
    return (uint8*)ret_ptr;
}

LOCAL BOOLEAN CAFRES_GetRingInfo(
                                  MMI_RING_ID_T ring_id,
                                  CAF_HANDLE_T handle_id,
                                  MMIAUD_RING_DATA_INFO_T * pRingInfo
                                  )
{
    uint32  com_id = 0;
    RES_NODE_DATA_T * res_node_ptr = PNULL;
    CAF_RES_T*  caf_res_ptr = PNULL;

    /* check resource id validate */
    if(0 == MMICOM_GET_WORD_LB(ring_id))
    {
        return PNULL;
    }
    
    com_id  = GetComIdByHandle( handle_id );
    
    if( MMI_SYS_RES_FLAG == ( ring_id & MMI_SYS_RES_FLAG ) )
    {
        /* get image from system default resource */
        com_id = GetComIdByHandle( MMK_GetFirstAppletHandle() );
    }
    else
    {
        /* get image from the resource of dynamic module */
        com_id = GetComIdByHandle( handle_id );
    }

    ring_id = ring_id & ( ~MMI_SYS_RES_FLAG );

    if( 0 == com_id )
    {
        return FALSE;
    }

    caf_res_ptr = GetComCafResInfo(com_id);   
    SCI_ASSERT(PNULL != caf_res_ptr);

    /* get ring node */
    res_node_ptr = GetResNode(caf_res_ptr, ring_id, handle_id, (MMI_RES_TYPE_E)CAF_MMI_RT_RING);    
    SCI_ASSERT(PNULL != res_node_ptr);
    
    pRingInfo->data_ptr = res_node_ptr->data_ptr;    
    pRingInfo->data_len = res_node_ptr->size;
    pRingInfo->type = res_node_ptr->data_type;

    return TRUE;
}

/*****************************************************************************/
//  Description : load resource data and node data struct
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note:
/*****************************************************************************/
RES_NODE_DATA_T* CAF_LoadResNode(const CAF_RES_T * com_res_info_ptr, uint32 res_id, CAF_HANDLE_T handle_id, CAF_MMI_RES_TYPE_E res_type)
{
    uint32      project_module_offset = 0;
    uint32      res_unit_offset = 0;
    uint32      res_unit_size = 0;
    uint16      prj_module_id   = MMI_GET_MODU_ID(res_id);
    uint16      res_unit_index  = MMICOM_GET_WORD_LB(res_id);
    uint8*      data_buffer_ptr= PNULL;
    RES_NODE_DATA_T * res_node_ptr = PNULL;
    MMIAUD_RING_DATA_INFO_T ring_info = {0};    
    uint32      data_type = 0;    
    
    if(PNULL != com_res_info_ptr)
    {
        /* get project module offset */
        project_module_offset = com_res_info_ptr->caf_res_header_ptr->mmi_module_adrr[prj_module_id];
        //#ifdef MULTI_THEME_SUPPORT  
#if 1
        project_module_offset += com_res_info_ptr->caf_res_theme_info_ptr->default_resource_info.resource_offset; 
#endif 
        /* get the offset of resource unit in project module */
        switch(res_type)
        {
        case CAF_MMI_RT_IMAGE:            
            res_unit_offset = com_res_info_ptr->caf_image_module_arry[prj_module_id].common_info_arry[res_unit_index-1].data_addr + project_module_offset;
            res_unit_size   = com_res_info_ptr->caf_image_module_arry[prj_module_id].common_info_arry[res_unit_index-1].data_size;
            break;            
        case CAF_MMI_RT_ANIM:            
            res_unit_offset = com_res_info_ptr->caf_anim_module_arry[prj_module_id].common_info_arry[res_unit_index-1].data_addr + project_module_offset;
            res_unit_size   = com_res_info_ptr->caf_anim_module_arry[prj_module_id].common_info_arry[res_unit_index-1].data_size;
            break;            
        case CAF_MMI_RT_DATA:   //暂时不支持
            SCI_ASSERT(0);
            break;
        case CAF_MMI_RT_RING:
            res_unit_offset = com_res_info_ptr->caf_ring_module_arry[prj_module_id].common_info_arry[res_unit_index-1].data_addr + project_module_offset;
            res_unit_size   = com_res_info_ptr->caf_ring_module_arry[prj_module_id].common_info_arry[res_unit_index-1].data_size;
            data_type = com_res_info_ptr->caf_ring_module_arry[prj_module_id].common_info_arry[res_unit_index-1].cur_theme;
            break;            
        default:
            SCI_ASSERT(0);
            break;
        }
        
        /* malloc buffer for load resource data */
        //SCI_TRACE_LOW("[RES]:res_unit_size = %d",res_unit_size);
        data_buffer_ptr = (uint8*)ResMalloc( com_res_info_ptr->res_location.store_type, res_unit_size );
        SCI_ASSERT(data_buffer_ptr!=PNULL);
        SCI_MEMSET(data_buffer_ptr, 0x00, res_unit_size);
        
        /* load resource data */
        ReadResource( com_res_info_ptr->res_handle, data_buffer_ptr, res_unit_offset, res_unit_size, com_res_info_ptr->res_location.store_type );
        
        /* config resource dynamic node */
        res_node_ptr = (RES_NODE_DATA_T*)ResMalloc( com_res_info_ptr->res_location.store_type, sizeof(RES_NODE_DATA_T) );
        SCI_ASSERT(PNULL != res_node_ptr);
        SCI_MEMSET(res_node_ptr, 0x00, sizeof(RES_NODE_DATA_T));
        
        res_node_ptr->com_id        = com_res_info_ptr->com_id;
        res_node_ptr->resource_id   = res_id;
        res_node_ptr->type          = res_type;
        res_node_ptr->data_ptr      = data_buffer_ptr;
        res_node_ptr->data_type     = data_type;
        res_node_ptr->size          = res_unit_size;                
    }
    
    return res_node_ptr;    
}



