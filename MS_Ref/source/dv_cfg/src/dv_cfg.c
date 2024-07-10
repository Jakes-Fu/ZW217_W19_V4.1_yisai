

#include "sci_types.h"
#include "os_api.h"
//
#include "dv_cfg.h"
#include "media_file_demux.h"
//
#include "video_codec_adptor.h"

/*****************************************************************************
 * macro
 *****************************************************************************/


/*****************************************************************************
 * Global prototypes.
 *****************************************************************************/
#ifdef DV_CFG_RMVB_DEMUX_SUPPORT
#ifndef WIN32
extern FILE_DEMUX_ENTRY_T g_rmvb_demux_entry;
#endif
#endif

#ifdef DV_CFG_RMVB_DEC_SUPPORT
#ifndef WIN32
extern VCODEC_ADP_DEC_ENTRY_T g_rmvb_dec_entry;
#endif
#endif


/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
LOCAL FILE_DEMUX_OBJ_T s_demux_extern_regist_table[] =
{
    // demux type.        // version.         //reserve0      //entries
#ifdef DV_CFG_RMVB_DEMUX_SUPPORT
#ifndef WIN32
    {"rmvb",    "rm",     0x0,0x0,            0x0,            &g_rmvb_demux_entry},
#endif
#endif
    {0},
    {0}
};


LOCAL VCODEC_ADP_DEC_OBJ_T s_vdec_extern_regist_table[] =
{
    // codec type.  // profile.  // level.   //reserve0.     //entries.
#ifdef DV_CFG_RMVB_DEC_SUPPORT
#ifndef WIN32
    {"rmvb",        0x0,         0x0,        0x0,            &g_rmvb_dec_entry},
#endif
#endif
    {0},
    {0}
};



/*****************************************************************************
 * Local functions
 *****************************************************************************/


/*****************************************************************************
 * Global functions
 *****************************************************************************/

/******************************************************************************/
// Description: Find out one demux obj from the register table.
// Author:     
// Input:      p_demux_main_type,the main name for this demuxer,e.g. "3gp","mp4".
// Output:     none.
// Return:     PNULL,  failed to find out one demux obj.
//             others, find out one matched demux obj.
// Note:       none
/******************************************************************************/
PUBLIC FILE_DEMUX_OBJ_T* ExtFileDemux_FindObj(const char *p_demux_main_type)
{
    FILE_DEMUX_OBJ_T *p_obj_hit = PNULL;
    FILE_DEMUX_OBJ_T *p_obj_tmp;
    uint32 obj_num;
    uint32 idx;

    if(PNULL == p_demux_main_type)
    {
        return PNULL;
    }
    obj_num = sizeof(s_demux_extern_regist_table)/sizeof(FILE_DEMUX_OBJ_T);

    SCI_TRACE_LOW("ExtFileDemux_FindObj,main_typ=%4s,obj_num=%d",p_demux_main_type,obj_num);

    for( idx = 0; idx < obj_num; idx++)
    {
        p_obj_tmp = &(s_demux_extern_regist_table[idx]);

        //SCI_TRACE_LOW("ExtFileDemux_FindObj,i=%d,main_type=%4s",i,p_obj_tmp->p_main_typ);

        if( (PNULL != p_obj_tmp->p_main_typ) && (PNULL != p_obj_tmp->p_demux_entry) )
        {
            if( !strcmp(p_demux_main_type, p_obj_tmp->p_main_typ) ) /*lint !e718 !e746*/
            {
                SCI_TRACE_LOW("ExtFileDemux_FindObj,hit, i=%d",idx);

                //hit one object.
                p_obj_hit = p_obj_tmp;
                break;
            }
        }
    }

    return p_obj_hit;
}


/******************************************************************************/
// Description: Find out one decoder obj from the register table.
// Author:     
// Input:      p_codec_name,the  name for this decoder,e.g. "mpeg4","h263".
// Output:     none.
// Return:     PNULL,  failed to find out one decoder obj.
//             others, find out one matched decoder obj.
// Note:       none
/******************************************************************************/
PUBLIC VCODEC_ADP_DEC_OBJ_T* ExtVideoDec_FindObj(const char *p_codec_name)
{
    VCODEC_ADP_DEC_OBJ_T *p_obj_hit = PNULL;
    VCODEC_ADP_DEC_OBJ_T *p_obj_tmp;
    uint32 obj_num;
    uint32 idx;

    if(PNULL == p_codec_name)
    {
        return PNULL;
    }
    obj_num = sizeof(s_vdec_extern_regist_table)/sizeof(VCODEC_ADP_DEC_OBJ_T);

    SCI_TRACE_LOW("ExtVideoDec_FindObj,codec_name=%4s,obj_num=%d",p_codec_name,obj_num);

    for( idx = 0; idx < obj_num; idx++)
    {
        p_obj_tmp = &(s_vdec_extern_regist_table[idx]);

        //SCI_TRACE_LOW("ExtFileDemux_FindObj,i=%d,main_type=%4s",i,p_obj_tmp->p_main_typ);

        if( (PNULL != p_obj_tmp->p_codec_name) && (PNULL != p_obj_tmp->p_dec_entry) )
        {
            if( !strcmp(p_codec_name, p_obj_tmp->p_codec_name) )
            {
                SCI_TRACE_LOW("ExtVideoDec_FindObj,hit, i=%d",idx);

                //hit one object.
                p_obj_hit = p_obj_tmp;
                break;
            }
        }
    }

    return p_obj_hit;
}









