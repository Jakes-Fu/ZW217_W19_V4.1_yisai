
/*****************************************************************************
 * head files.
 *****************************************************************************/

#include "sci_types.h"
#include "os_api.h"
#include "media_file_demux.h"

#include "helix_types.h"
#include "rm_parse.h"
#include "rv_depack.h"
#include "ra_depack.h"
#include "rv_decode.h"
#include "ra_decode.h"
#include "pack_utils.h"
#include "rm_parser_internal.h"
#include "ra_depack_internal.h"
#include "rv_depack_internal.h"
#include "ra_format_info.h"
#include "rv_format_info.h"
#include "packet_defines.h"


/*****************************************************************************
 * macro
 *****************************************************************************/
#define RM2YUV_INITIAL_READ_SIZE  16

#define PACKET_BUFFER_SIZE  (384*1024)

#if 0
#define USER_DEMUX_I_FRM  (0)
#define USER_DEMUX_P_FRM  (1)
#define USER_DEMUX_B_FRM  (2)
#endif

#define RMVB_DEMUX_STRM_NUM_MAX     0x4
#define RMVB_DEMUX_STRM_ID_AUDIO    0x0
#define RMVB_DEMUX_STRM_ID_VIDEO    0x1


#define RMVB_DEMUX_STRM_PACKET_LIST_LEN_AUD  0x30
#define RMVB_DEMUX_STRM_PACKET_LIST_LEN_VID  0x50


typedef void* RMVB_DEMUX_LIB_STRM_HANDLE;
typedef void* RMVB_DEMUX_LIB_HANDLE;


typedef struct DEMUX_STRM_PACKET_NODE_TAG
{
    struct DEMUX_STRM_PACKET_NODE_TAG *p_pre;
    struct DEMUX_STRM_PACKET_NODE_TAG *p_next;
    //
    DEMUX_STRM_PACKET_T packet;
    void   *p_list;
    void   *p_data_generator;
    void   *p_data_container;
}DEMUX_STRM_PACKET_NODE_T;


typedef struct
{
    void *p_buf;
    DEMUX_STRM_PACKET_NODE_T *p_first;
    uint32 node_num;
}DEMUX_STRM_PACKET_LIST_T;


typedef struct
{
    // handle and constant parameter.
    RMVB_DEMUX_LIB_STRM_HANDLE  strm_handle;
    DEMUX_STRM_INFO_T  strm_info;
    DEMUX_DATA_BUF_T    strm_udata;
    DEMUX_DATA_BUF_T    strm_cfgdata;
    //
    rm_parser_internal *p_rm_parser_internal;

    // varied parameters.
    uint32 strm_file_pos_cur;
    uint32 strm_demux_time_cur;
    uint32 strm_demux_time_target;
    uint32 strm_demux_packetnum_cur;
    uint32 strm_demux_packetnum_target;
    //
    uint32 frm_type_cur;
    uint32 need_resync; // when pre data lost, resync may be needed.
    uint32 need_skip;

    // strm packet list.
    DEMUX_STRM_PACKET_LIST_T strm_packet_list;
    
    // outport.
    uint32 outport_typ;     // mast or slave.
    uint32 outport_num;
    DEMUX_STRM_RX_SLAVE_T strm_outport[RMVB_DEMUX_STRM_NUM_MAX];

    uint32 b_used;
}RMVB_DEMUX_STRM_T;


typedef struct
{
    void *p_owner;      // for check.
    RMVB_DEMUX_LIB_HANDLE   rmvb_lib_handle;

    DEMUX_IN_SRC_T  source_in;
    DEMUX_MEM_OP_T  mem_op;
    DEMUX_CALLBACK_OP_T callback_op;

    DEMUX_FILE_INFO_T file_info;
    DEMUX_DATA_BUF_T  file_udata;
    //
    RMVB_DEMUX_STRM_T strm[RMVB_DEMUX_STRM_NUM_MAX];
    DEMUX_STRM_PACKET_LIST_T packet_list;
    //
    uint32 file_pos_cur;
    uint32 file_demux_time_cur;
    uint32 file_demux_time_target;
    uint32 is_firstseek_done;

}RMVB_DEMUX_CTX_T;


/*****************************************************************************
 * Local prototypes
 *****************************************************************************/


/*****************************************************************************
 * Local functions
 *****************************************************************************/
#define RMVB_PRT_ERR    SCI_TRACE_LOW//SCI_TraceLow0
#ifdef WIN32
#define RMVB_PRT_NONE   //(...)
#else
#define RMVB_PRT_NONE(...)
#endif


#define _RMVB_DIS_INT_      SCI_DisableIRQ()
#define _RMVB_EN_INT_       SCI_RestoreIRQ()


#define RMVB_MAX(a, b)   ( ((a) > (b)) ? (a) : (b) )


#ifdef _MEDIA_FILE_DEMUX_BIG_ENDIAN_
#define RMVB_4C_STR(a,b,c,d) ( (((a)&0xff)<<24) | (((b)&0xff)<<16) | (((c)&0xff)<<8) | ((d)&0xff) )
#else
#define RMVB_4C_STR(a,b,c,d) ( (((d)&0xff)<<24) | (((c)&0xff)<<16) | (((b)&0xff)<<8) | ((a)&0xff) )
#endif


static void _rmvb_demux_error(void* pError, HX_RESULT result, const char* pszMsg)
{
    RMVB_PRT_NONE( "rmvb_demux_error pError=0x%x result=0x%x msg=%s", pError, result, pszMsg);
}

static void *_rvmb_demux_malloc(void* pUserMem, UINT32 ulSize)
{
    void *p_mem = PNULL;

    p_mem = SCI_ALLOCA(ulSize);

    RMVB_PRT_NONE( "_rvmb_demux_malloc,pUserMem=%s, ulSize=%d, mem=0x%x",
                pUserMem, ulSize,p_mem);

    return p_mem;
}

static void _rvmb_demux_free(void* pUserMem, void* ptr)
{
    RMVB_PRT_NONE("rmvb_memory_free, pUserMem: %s, ptr: 0x%x", pUserMem, ptr);
    if (ptr != PNULL)
    {
        SCI_FREE(ptr);
        ptr = PNULL;
    }
}

static uint32 _rmvb_io_read(void*  pUserRead,
                         BYTE*   pBuf, /* Must be at least ulBytesToRead long */
                         UINT32  ulBytesToRead)
    
{
    DEMUX_IN_SRC_T *p_in_src;
    uint32 bytes_read = 0;

    RMVB_PRT_NONE("_rmvb_io_read,pUserRead=0x%x,pBuf=0x%x,ulBytesToRead=%d",
        pUserRead,pBuf,ulBytesToRead);

    if(PNULL != pUserRead)
    {
        p_in_src = (DEMUX_IN_SRC_T *)pUserRead;

        if(PNULL != p_in_src->pf_file_read)
        {
            bytes_read = (uint32)p_in_src->pf_file_read(
                                                p_in_src->p_file,
                                                pBuf,
                                                ulBytesToRead
                                                );
        }
    }

    return bytes_read;
}

static void _rmvb_io_seek(void*  pUserRead,
                         UINT32 ulOffset,
                         UINT32 ulOrigin)
{
    DEMUX_IN_SRC_T *p_in_src;
    uint32 seek_org = FDEMUX_SEEK_ORIGIN_SET;

    RMVB_PRT_NONE("_rmvb_io_seek,pUserRead=0x%x,ulOffset=%d,ulOrigin=%d",
        pUserRead,ulOffset,ulOrigin);

    if(PNULL != pUserRead)
    {
        p_in_src = (DEMUX_IN_SRC_T *)pUserRead;

        if(HX_SEEK_ORIGIN_CUR == ulOrigin)
        {
            seek_org = FDEMUX_SEEK_ORIGIN_CUR;
        }

        if(PNULL != p_in_src->pf_file_seek)
        {
            p_in_src->pf_file_seek(
                        p_in_src->p_file,
                        ulOffset,
                        seek_org
                        );
        }
    }
}

static int _rmvb_create_packetlist(DEMUX_STRM_PACKET_LIST_T *p_list,uint32 node_num)
{
    DEMUX_STRM_PACKET_NODE_T *p_node_0;
    DEMUX_STRM_PACKET_NODE_T *p_node_1;
    uint32 idx;

    if( (PNULL == p_list) || (0 == node_num) )
    {
        return SCI_ERROR;
    }

    RMVB_PRT_ERR("_rmvb_create_packetlist,p_list=0x%x,node_num=%d",p_list,node_num);

    p_list->p_buf = SCI_ALLOCA( sizeof(DEMUX_STRM_PACKET_NODE_T) * node_num );
    if(p_list->p_buf == PNULL)
    {
        return SCI_ERROR;
    }
    memset(p_list->p_buf, 0, sizeof(DEMUX_STRM_PACKET_NODE_T)*node_num);
    p_list->node_num = node_num;
    p_list->p_first = (DEMUX_STRM_PACKET_NODE_T *)p_list->p_buf;

    // init.
    p_node_0 = p_list->p_first;
    for(idx = 1; idx < node_num; idx++)
    {
        p_node_1 = p_node_0 + 1;
        // init the node.
        p_node_0->p_next = p_node_1;
        p_node_0->p_list = p_list;
        p_node_0->p_data_container = PNULL;
        p_node_0->p_data_generator = PNULL;
        // init next node.
        p_node_1->p_pre = p_node_0;
        //
        p_node_0 = p_node_1;
    }
    // the last node.
    p_node_0->p_next = p_list->p_first;
    p_node_0->p_list = p_list;
    p_node_0->p_data_container = PNULL;
    p_node_0->p_data_generator = PNULL;
    //
    p_list->p_first->p_pre = p_node_0;

    return SCI_SUCCESS;
}

static int _rmvb_destroy_packetlist(DEMUX_STRM_PACKET_LIST_T *p_list)
{
    RMVB_PRT_ERR("_rmvb_destroy_packetlist,p_list=0x%x,node_num=%d",p_list,p_list->node_num);

    if( PNULL == p_list )
    {
        return SCI_ERROR;
    }
    if( p_list->p_buf != PNULL )
    {
        SCI_FREE(p_list->p_buf);
        //
        p_list->p_buf = PNULL;
        p_list->p_first = PNULL;
        p_list->node_num = 0;
    }

    return SCI_SUCCESS;
}

static DEMUX_STRM_PACKET_NODE_T *_rmvb_alloc_packetlist_node(DEMUX_STRM_PACKET_LIST_T *p_list)
{
    DEMUX_STRM_PACKET_NODE_T *p_node_head;
    DEMUX_STRM_PACKET_NODE_T *p_node_tail;
    DEMUX_STRM_PACKET_NODE_T *p_node;

    if( PNULL == p_list )
    {
        return PNULL;
    }

    RMVB_PRT_NONE("_rmvb_alloc_packetlist_node,p_list=0x%x,node_num=%d,p_first=0x%x",
        p_list,p_list->node_num,p_list->p_first
        );

    // remove one node from the header.
_RMVB_DIS_INT_;
    if(p_list->node_num == 0)
    {
        p_node = PNULL;
    }
    else if(p_list->node_num == 1)
    {
        p_node = p_list->p_first;
        p_node->p_pre = PNULL;
        p_node->p_next = PNULL;
        //
        p_list->p_first = PNULL;
        p_list->node_num = 0;
    }
    else
    {
        p_node = p_list->p_first;
        //
        p_node_head = p_node->p_next;
        p_node_tail = p_node->p_pre;
        p_node_head->p_pre = p_node_tail;
        p_node_tail->p_next = p_node_head;
        //
        p_list->p_first = p_node_head;
        p_list->node_num--;

        p_node->p_pre = PNULL;
        p_node->p_next = PNULL;
    }
_RMVB_EN_INT_;

#if 1
    if(PNULL != p_node)
    {
        p_node->packet.p_owner = (void *)p_node;
    }
#endif

    return p_node;
}

static int _rmvb_free_packetlist_node(DEMUX_STRM_PACKET_NODE_T *p_node)
{
    DEMUX_STRM_PACKET_LIST_T *p_list;
    DEMUX_STRM_PACKET_NODE_T *p_node_head;
    DEMUX_STRM_PACKET_NODE_T *p_node_tail;

    if( (PNULL == p_node) || (PNULL == p_node->p_list) )
    {
        return SCI_ERROR;
    }
    p_list = (DEMUX_STRM_PACKET_LIST_T *)p_node->p_list;


    RMVB_PRT_NONE("_rmvb_free_packetlist_node,p_list=0x%x,node_num=%d,p_node=0x%x",
        p_list,p_list->node_num,p_node
        );

    // add to the tail of the list.
_RMVB_DIS_INT_;
    if(p_list->node_num == 0)
    {
        p_list->p_first = p_node;
        p_node->p_next = p_node;
        p_node->p_pre = p_node;
    }
    else
    {
        p_node_head = p_list->p_first;
        p_node_tail = p_node_head->p_pre;
        //
        p_node_head->p_pre = p_node;
        p_node_tail->p_next = p_node;
        p_node->p_pre = p_node_tail;
        p_node->p_next = p_node_head;
    }
    p_list->node_num++;
_RMVB_EN_INT_;

    return SCI_SUCCESS;
}


// callback from app, works as the free port.
static int _rmvb_ra_block_free(DEMUX_STRM_PACKET_T *p_packet)
{
    DEMUX_STRM_PACKET_NODE_T *p_node;
    ra_depack *p_ra_depack;
    ra_block *p_ra_block;

    RMVB_PRT_NONE("_rmvb_ra_block_free,p_packet=0x%x",p_packet);

    if( (PNULL == p_packet) || (PNULL == p_packet->p_owner) )
    {
        return SCI_ERROR;
    }
    p_node = (DEMUX_STRM_PACKET_NODE_T *)p_packet->p_owner;
    p_ra_depack = (ra_depack *)p_node->p_data_generator;
    p_ra_block = (ra_block *)p_node->p_data_container;

    if( p_packet->used_cnt == 0)
    {
        return SCI_SUCCESS;
    }
    
_RMVB_DIS_INT_;
    p_packet->used_cnt--;
_RMVB_EN_INT_;
    if( p_packet->used_cnt == 0 )
    {
        // free the data.
        ra_depack_destroy_block(p_ra_depack, &p_ra_block);

        // TODO: free the packet container.
        _rmvb_free_packetlist_node(p_node);
    }

    return SCI_SUCCESS;
}

// callback from app, works as the free port.
static int _rmvb_rv_frame_free(DEMUX_STRM_PACKET_T *p_packet)
{
    DEMUX_STRM_PACKET_NODE_T *p_node;
    rv_depack *p_rv_depack;
    rv_frame *p_rv_frame;

    RMVB_PRT_NONE("_rmvb_rv_frame_free,p_packet=0x%x",p_packet);

    if( (PNULL == p_packet) || (PNULL == p_packet->p_owner) )
    {
        return SCI_ERROR;
    }
    p_node = (DEMUX_STRM_PACKET_NODE_T *)p_packet->p_owner;
    p_rv_depack = (rv_depack *)p_node->p_data_generator;
    p_rv_frame = (rv_frame *)p_node->p_data_container;

    if( p_packet->used_cnt == 0)
    {
        return SCI_SUCCESS;
    }

_RMVB_DIS_INT_;
    p_packet->used_cnt--;
_RMVB_EN_INT_;
    if( p_packet->used_cnt == 0 )
    {
        // free the rv_frame.
        rv_depack_destroy_frame(p_rv_depack, &p_rv_frame);

        // free the node.
        _rmvb_free_packetlist_node(p_node);
    }

    return SCI_SUCCESS;
}


// callback from rmvb lib. work as one slave receiver.
static HX_RESULT _ra_block_available(void* pAvail, UINT32 ulSubStream, ra_block* p_ra_block)
{
    RMVB_DEMUX_STRM_T   *p_strm;
    //
    DEMUX_STRM_PACKET_NODE_T *p_node;
    DEMUX_STRM_PACKET_T     *p_packet;
    DEMUX_STRM_RX_SLAVE_T   *p_outport;
    //
    uint32 cnt = 0;
    uint32 cnt_sendout = 0;

    if(PNULL == pAvail)
    {
        return HXR_INVALID_PARAMETER;
    }
    p_strm = (RMVB_DEMUX_STRM_T *)pAvail;

    RMVB_PRT_NONE("_ra_block_available,ulSubStream=%d,p_ra_block=0x%x",
                    ulSubStream,p_ra_block);

    // generator one node.
    p_node = _rmvb_alloc_packetlist_node( &(p_strm->strm_packet_list) );
    if(PNULL == p_node)
    {
        ra_depack_destroy_block( (ra_depack *)(p_strm->strm_handle), &p_ra_block);
        return HXR_UNEXPECTED;
    }
    p_node->p_data_generator = (void *)(p_strm->strm_handle);
    p_node->p_data_container = (void *)p_ra_block;

    // fill the packet.
    p_packet = &(p_node->packet);
    p_packet->p_data = p_ra_block->pData;
    p_packet->data_len = p_ra_block->ulDataLen;
    p_packet->time_stamp = p_ra_block->ulTimestamp;
    p_packet->frm_num = 0;
    //
    p_packet->flag = p_strm->frm_type_cur;
    //
    p_packet->pf_freeport = _rmvb_ra_block_free;
_RMVB_DIS_INT_;
    p_packet->used_cnt = p_strm->outport_num;
_RMVB_EN_INT_;

    // send out the packet to all the outports.
    for( cnt = 0; cnt < p_strm->outport_num; cnt++ )
    {
        p_outport = &(p_strm->strm_outport[cnt]);
        if(PNULL != p_outport->pf_rx_callback)
        {
            cnt_sendout++;
            p_outport->pf_rx_callback(p_outport->p_receiver,p_packet);
        }
    }

    // if no outport available, free this packet.
    if( cnt_sendout == 0)
    {
        // free the ra_packet.
        ra_depack_destroy_block( (ra_depack *)(p_strm->strm_handle), &p_ra_block);

        // free the node.
        _rmvb_free_packetlist_node(p_node);

        return HXR_NOINTERFACE;
    }

    return HXR_OK;
}

// refer to "RvDec_GetPictureType" in rvdec decoder.
static uint32 _rv_get_frmtyp(RMVB_DEMUX_STRM_T *p_strm, uint8 first_byte)
{
	uint32 shift = 0;
	int32 type_tbl[4] = {0, 0, 1, 2};
    uint32 frm_type = PACKET_FLAG_FRM_UNKNOW;

    if(p_strm->strm_info.strm_sub_typ == RMVB_4C_STR('R','V','3','0'))
    {
        // RV8. unsupport now.
        shift = 3;
    }
    else if(p_strm->strm_info.strm_sub_typ == RMVB_4C_STR('R','V','4','0'))
    {
        // RV9.
        shift = 5;
    }
    else
    {
        return PACKET_FLAG_FRM_UNKNOW;
    }

	/* PTYPE */
    switch( type_tbl[(first_byte >> shift) & 0x3] )
    {
        case 0:
            frm_type = PACKET_FLAG_FRM_I;
            break;
            
        case 1:
            frm_type = PACKET_FLAG_FRM_P;
            break;
            
        case 2:
            frm_type = PACKET_FLAG_FRM_B;
            break;
            
        default:
            frm_type = PACKET_FLAG_FRM_UNKNOW;
            break;
    }

	return frm_type;
}


// callback from rmvb lib. work as one slave receiver.
static HX_RESULT _rv_frame_available(void* pAvail, UINT32 ulSubStream, rv_frame* p_frame)
{
    RMVB_DEMUX_STRM_T   *p_strm;
    //
    DEMUX_STRM_PACKET_NODE_T *p_node;
    DEMUX_STRM_PACKET_T     *p_packet;
    DEMUX_SEGMENT_T *p_segment;
    DEMUX_STRM_RX_SLAVE_T   *p_outport;
    //
    uint32 cnt = 0;
    uint32 cnt_sendout = 0;
    uint32 frm_type = PACKET_FLAG_FRM_UNKNOW;

    RMVB_PRT_NONE("_rv_frame_available,upAvail=0x%x,lSubStream=%d,p_frame=0x%x,data_len=%d,seg_num=%d",
                    pAvail,ulSubStream,p_frame,
                    p_frame->ulDataLen,p_frame->ulNumSegments
                    );

    if( (PNULL == pAvail) || (PNULL == p_frame) )
    {
        RMVB_PRT_ERR("_rv_frame_available,pAvail=0x%x,p_frame=0x%x",pAvail,p_frame);

        return HXR_INVALID_PARAMETER;
    }
    p_strm = (RMVB_DEMUX_STRM_T *)pAvail;

    frm_type = _rv_get_frmtyp(p_strm, *(p_frame->pData));
    p_strm->frm_type_cur = frm_type;
    //
    if( (PACKET_FLAG_FRM_B == frm_type) && (p_strm->need_skip > 0) )
    {
        RMVB_PRT_ERR("_rv_frame_available,skip B frame.");
        
        rv_depack_destroy_frame( (rv_depack *)(p_strm->strm_handle), &p_frame);
        return HXR_OK;
    }

    //
    if( (p_frame->ulNumSegments > MAX_SEGMENTS_IN_PACKET) )
    {
        RMVB_PRT_ERR("_rv_frame_available,ulNumSegments=%d is too large",p_frame->ulNumSegments);

        p_strm->need_resync = (PACKET_FLAG_FRM_B == frm_type)? 0 : 1;
        rv_depack_destroy_frame( (rv_depack *)(p_strm->strm_handle), &p_frame);
        return HXR_OK;
    }

    // generator one packet.
    p_node = _rmvb_alloc_packetlist_node( &(p_strm->strm_packet_list) );
    if(PNULL == p_node)
    {
        RMVB_PRT_ERR("_rv_frame_available,_rmvb_alloc_packetlist_node failed,node_num=%d,first=0x%x",
                        p_strm->strm_packet_list.node_num,
                        p_strm->strm_packet_list.p_first
                        );
        
        p_strm->need_resync = (PACKET_FLAG_FRM_B == frm_type)? 0 : 1;
        rv_depack_destroy_frame( (rv_depack *)(p_strm->strm_handle), &p_frame);
        return HXR_OK;
    }
    p_node->p_data_generator = (void *)(p_strm->strm_handle);
    p_node->p_data_container = (void *)p_frame;

    // fill the packet.
    p_packet = &(p_node->packet);
    p_packet->p_data   = p_frame->pData;
    p_packet->data_len = p_frame->ulDataLen;
    //
    for(p_packet->seg_num = 0; p_packet->seg_num < p_frame->ulNumSegments;)
    {
        p_segment = &(p_packet->segments[p_packet->seg_num]);
        p_segment->is_valid = p_frame->pSegment[p_packet->seg_num].bIsValid;
        p_segment->offset = p_frame->pSegment[p_packet->seg_num].ulOffset;

        p_packet->seg_num++;
    }
    //
    p_packet->time_stamp = p_frame->ulTimestamp;
    p_packet->frm_num = p_frame->usSequenceNum;
    p_packet->packet_num = 0;//p_frame->ulNumSegments;
    p_packet->is_last_packet = p_frame->bLastPacket;
    //
    if( PACKET_FLAG_FRM_I == frm_type)
    {
        p_packet->distance_to_nextkeyfrm = 0;
    }
    else
    {
        // caculate the distance to the next I frame.
        p_packet->distance_to_nextkeyfrm = rm_parseri_get_next_keyframe_interval(
                                                    p_strm->p_rm_parser_internal,
                                                    p_packet->time_stamp
                                                    );
    }
    p_packet->flag = frm_type;
#if 1
    if( p_strm->need_resync > 0 )
    {
        p_packet->flag |= PACKET_FLAG_TYP_BROKEN;
        if(PACKET_FLAG_FRM_I == frm_type)
        {
            p_strm->need_resync = 0;
        }
    }
#endif
    //
    p_packet->pf_freeport = _rmvb_rv_frame_free;
_RMVB_DIS_INT_;
    p_packet->used_cnt = p_strm->outport_num;
_RMVB_EN_INT_;

    // send out the packet to all the outports.
    for( cnt = 0; cnt < p_strm->outport_num; cnt++ )
    {
        p_outport = &(p_strm->strm_outport[cnt]);
        if(PNULL != p_outport->pf_rx_callback)
        {
            cnt_sendout++;
            p_outport->pf_rx_callback(p_outport->p_receiver,p_packet);
        }
    }

    RMVB_PRT_NONE("_rv_frame_available,cnt_sendout=%d",cnt_sendout);

    // if no outport available, free this packet.
    if( cnt_sendout == 0)
    {
        // free the rv_frame.
        rv_depack_destroy_frame( (rv_depack *)(p_strm->strm_handle), &p_frame);

        // free the node.
        _rmvb_free_packetlist_node(p_node);
        return HXR_NOINTERFACE;
    }

    return HXR_OK;
}

static int _create_audio_stream(RMVB_DEMUX_STRM_T *p_strm,rm_stream_header *p_rm_strm_hdr,uint32 strm_id)
{
    ra_depack*          pRaDepack        = HXNULL;
    ra_format_info*     pRaInfo          = HXNULL;
    //
    HX_RESULT retVal;
    UINT32 ulCodec4CC_be;
    uint32 codec4cc_str;
    //
    ra_depack_internal *p_ra_depack_internal;

    RMVB_PRT_ERR("_create_audio_stream,id=%d",strm_id);

    if( (PNULL == p_strm) || (PNULL != p_strm->strm_handle) )
    {
        // the stream has been used.
        return SCI_ERROR;
    }

    /* Create the RealAudio depacketizer */
    pRaDepack = ra_depack_create2((void *)p_strm,
                               _ra_block_available,
                               NULL,
                               _rmvb_demux_error,
                               "ra_depack",
                               _rvmb_demux_malloc,
                               _rvmb_demux_free);
    if (!pRaDepack)
    {
        RMVB_PRT_ERR("_create_audio_stream, pRaDepack malloc fail");
        return SCI_ERROR;
    }
    if(ra_depack_init(pRaDepack, p_rm_strm_hdr)!= HXR_OK)
    {
        RMVB_PRT_ERR("_create_audio_stream, rv_depack_init  fail");

        ra_depack_destroy(&pRaDepack);
        return SCI_ERROR;
    }
    p_strm->strm_handle = (RMVB_DEMUX_LIB_STRM_HANDLE)pRaDepack;
    p_ra_depack_internal= (ra_depack_internal *)pRaDepack;

    /*
     * Get the codec 4CC of substream 0. We 
     * arbitrarily choose substream 0 here.
     */
    ulCodec4CC_be = ra_depack_get_codec_4cc(pRaDepack, 0);
    codec4cc_str = RMVB_4C_STR( ((ulCodec4CC_be>>24)&0xff),((ulCodec4CC_be>>16)&0xff), \
                                ((ulCodec4CC_be>>8 )&0xff),((ulCodec4CC_be)&0xff)  \
                              );
    RMVB_PRT_ERR("_create_audio_stream, Audio Codec4CC: 0x%x", codec4cc_str);

    /*
     * Get the bitstream header information. Again,
     * we arbitrarily choose substream 0.
     */
    retVal = ra_depack_get_codec_init_info(pRaDepack, 0, &pRaInfo);
    if (retVal != HXR_OK)
    {
        RMVB_PRT_ERR("_create_audio_stream, ra_depack_get_codec_init_info fail");

        ra_depack_destroy(&pRaDepack);
        p_strm->strm_handle = PNULL;
        return SCI_ERROR;
    }

    // save configure data for audio.
    p_strm->strm_cfgdata.len = pRaInfo->ulOpaqueDataSize;
    if(p_strm->strm_cfgdata.len > 0)
    {
        p_strm->strm_cfgdata.p_buf = SCI_ALLOCA(p_strm->strm_cfgdata.len);
        if (PNULL == p_strm->strm_cfgdata.p_buf)
        {
            RMVB_PRT_ERR("_create_audio_stream, p_extra malloc fail");

            ra_depack_destroy_codec_init_info(pRaDepack, &pRaInfo);
            ra_depack_destroy(&pRaDepack);
            p_strm->strm_handle = PNULL;
            return SCI_ERROR;
        }
        if(codec4cc_str == RMVB_4C_STR('c', 'o', 'o', 'k'))
        {
            memcpy(p_strm->strm_cfgdata.p_buf, pRaInfo->pOpaqueData, p_strm->strm_cfgdata.len);
        }
        else
        {
            p_strm->strm_cfgdata.len--;
            memcpy(p_strm->strm_cfgdata.p_buf, &(pRaInfo->pOpaqueData[1]), p_strm->strm_cfgdata.len);
        }
    }
    RMVB_PRT_ERR("_create_audio_stream,i_extra=%d,cfg_datasize=%d",
        p_strm->strm_cfgdata.len,
        pRaInfo->ulOpaqueDataSize
        );

    //
    p_strm->strm_info.strm_id = strm_id;
    p_strm->strm_info.strm_main_typ = RMVB_4C_STR('a','u','d',' ');
    p_strm->strm_info.strm_sub_typ = codec4cc_str;
    p_strm->strm_info.codec_cfg_data_len = p_strm->strm_cfgdata.len;
    p_strm->strm_info.strm_duration_ms = p_rm_strm_hdr->ulDuration;
    //
    p_strm->strm_info.strm_fmt.a_fmt.samplerate = pRaInfo->ulSampleRate;
    p_strm->strm_info.strm_fmt.a_fmt.channels = pRaInfo->usNumChannels;
    p_strm->strm_info.strm_fmt.a_fmt.avg_bitrate = 0;
    p_strm->strm_info.strm_fmt.a_fmt.bitspersample = pRaInfo->usBitsPerSample;
    p_strm->strm_info.strm_fmt.a_fmt.bytes_per_compressfrm = pRaInfo->ulBitsPerFrame;
    p_strm->strm_info.strm_fmt.a_fmt.blk_align_size = p_ra_depack_internal->pSubStreamHdr[0].ulInterleaveBlockSize;

    ra_depack_destroy_codec_init_info(pRaDepack, &pRaInfo);

    if( SCI_SUCCESS != _rmvb_create_packetlist(&(p_strm->strm_packet_list),RMVB_DEMUX_STRM_PACKET_LIST_LEN_AUD ) )
    {
        if(PNULL != p_strm->strm_cfgdata.p_buf)
        {
            SCI_FREE(p_strm->strm_cfgdata.p_buf);
        }
        ra_depack_destroy(&pRaDepack);
        p_strm->strm_handle = PNULL;
        return SCI_ERROR;
    }

    p_strm->b_used = SCI_TRUE;

    return SCI_SUCCESS;
}

static int _destroy_audio_stream(RMVB_DEMUX_STRM_T *p_strm)
{
    ra_depack   *pRaDepack = HXNULL;

    if(PNULL == p_strm)
    {
        return SCI_ERROR;
    }

    if (PNULL != p_strm->strm_cfgdata.p_buf)
    {
        SCI_FREE(p_strm->strm_cfgdata.p_buf);
        p_strm->strm_cfgdata.p_buf = PNULL;
    }

    if( PNULL != p_strm->strm_handle)
    {
        pRaDepack = (ra_depack*)p_strm->strm_handle; 
        ra_depack_destroy(&pRaDepack);
        p_strm->strm_handle = PNULL;
    }

    if( PNULL != p_strm->strm_packet_list.p_first )
    {
        _rmvb_destroy_packetlist(&(p_strm->strm_packet_list));
    }

    return SCI_SUCCESS;
}

static int _create_video_stream(RMVB_DEMUX_STRM_T *p_strm, rm_stream_header *p_rm_strm_hdr,uint32 strm_id)
{
    rv_depack*          pRvDepack        = HXNULL;
    rv_format_info*     pRvInfo          = HXNULL;
    //
    HX_RESULT retVal;
    UINT32 ulCodec4CC_be;
    uint32 codec4cc_str;

    RMVB_PRT_ERR("_create_video_stream,id=%d",strm_id);

    if( (PNULL == p_strm) || (PNULL != p_strm->strm_handle) )
    {
        // the stream has been used.
        return SCI_ERROR;
    }

    /* Create the RealVideo depacketizer */
    pRvDepack = rv_depack_create2((void *)p_strm,
                               _rv_frame_available,
                               NULL,
                               _rmvb_demux_error,
                               "rv_depack",
                               _rvmb_demux_malloc,
                               _rvmb_demux_free);
    if (!pRvDepack)
    {
        RMVB_PRT_ERR("_create_video_stream, rv_depack_create2  fail");
        return SCI_ERROR;
    }
    /* Initialize the RA depacketizer with the stream header */
    if(rv_depack_init(pRvDepack, p_rm_strm_hdr)!= HXR_OK)
    {
        RMVB_PRT_ERR("_create_video_stream, rv_depack_init  fail");

        rv_depack_destroy(&pRvDepack);
        return SCI_ERROR;
    }
    p_strm->strm_handle = (RMVB_DEMUX_LIB_STRM_HANDLE)pRvDepack;

    /* Get the bitstream header information */
    retVal = rv_depack_get_codec_init_info(pRvDepack, &pRvInfo);
    if (retVal != HXR_OK)
    {
        RMVB_PRT_ERR("_create_video_stream, rv_depack_get_codec_init_info  fail");

        rv_depack_destroy(&pRvDepack);
        p_strm->strm_handle = PNULL;
        return SCI_ERROR;
    }

    // save configure data for video.
    p_strm->strm_cfgdata.len = pRvInfo->ulOpaqueDataSize;
    if(p_strm->strm_cfgdata.len > 0)
    {
        p_strm->strm_cfgdata.p_buf = SCI_ALLOCA(p_strm->strm_cfgdata.len);
        if (PNULL == p_strm->strm_cfgdata.p_buf)
        {
            RMVB_PRT_ERR("_create_video_stream, p_extra malloc fail");

            rv_depack_destroy_codec_init_info(pRvDepack, &pRvInfo);
            ra_depack_destroy(&pRvDepack);
            p_strm->strm_handle = PNULL;
            return SCI_ERROR;
        }
    }
    RMVB_PRT_NONE("_create_video_stream,i_extra=%d",p_strm->strm_cfgdata.len);

    /* Get the Codec ID from rv format info*/
    ulCodec4CC_be = pRvInfo->ulSubMOFTag;
    codec4cc_str = RMVB_4C_STR( ((ulCodec4CC_be>>24)&0xff),((ulCodec4CC_be>>16)&0xff), \
                                ((ulCodec4CC_be>>8 )&0xff),((ulCodec4CC_be)&0xff)  \
                              );
    RMVB_PRT_ERR("_create_video_stream, Video Codec4CC: 0x%x", codec4cc_str);
    
    p_strm->strm_info.strm_id = strm_id;
    p_strm->strm_info.strm_main_typ = RMVB_4C_STR('v','i','d',' ');
    p_strm->strm_info.strm_sub_typ = codec4cc_str;
    p_strm->strm_info.codec_cfg_data_len = p_strm->strm_cfgdata.len;
    p_strm->strm_info.strm_duration_ms = p_rm_strm_hdr->ulDuration;
    //
    p_strm->strm_info.strm_fmt.v_fmt.width = pRvInfo->usWidth;
    p_strm->strm_info.strm_fmt.v_fmt.height = pRvInfo->usHeight;
    //
    p_strm->strm_info.strm_fmt.v_fmt.frm_rate = pRvInfo->ufFramesPerSecond;
    p_strm->strm_info.strm_fmt.v_fmt.frm_rate_base = 1<<16;
    p_strm->strm_info.strm_fmt.v_fmt.avg_bitrate = 0;

#if 1
    if( ( ( (pRvInfo->ufFramesPerSecond)>>16 ) > 15 ) || 
        ( pRvInfo->usWidth * pRvInfo->usHeight >= 640*480) )
    {
        p_strm->need_skip =  1;
    }
#endif

    rv_depack_destroy_codec_init_info(pRvDepack, &pRvInfo);

    if( SCI_SUCCESS != _rmvb_create_packetlist(&(p_strm->strm_packet_list),RMVB_DEMUX_STRM_PACKET_LIST_LEN_VID ) )
    {
        if(PNULL != p_strm->strm_cfgdata.p_buf)
        {
            SCI_FREE(p_strm->strm_cfgdata.p_buf);
        }
        ra_depack_destroy(&pRvDepack);
        p_strm->strm_handle = PNULL;
        return SCI_ERROR;
    }
    
    p_strm->b_used = SCI_TRUE;

    return SCI_SUCCESS;
}

static int _destroy_video_stream(RMVB_DEMUX_STRM_T *p_strm)
{
    rv_depack   *pRvDepack = HXNULL;

    if(PNULL == p_strm)
    {
        return SCI_ERROR;
    }

    if (PNULL != p_strm->strm_cfgdata.p_buf)
    {
        SCI_FREE(p_strm->strm_cfgdata.p_buf);
        p_strm->strm_cfgdata.p_buf = PNULL;
    }
    //
    if( PNULL != p_strm->strm_handle)
    {
        pRvDepack = (rv_depack *)p_strm->strm_handle;
        rv_depack_destroy(&pRvDepack);
        p_strm->strm_handle = PNULL;
    }

    if( PNULL != p_strm->strm_packet_list.p_first )
    {
        _rmvb_destroy_packetlist(&(p_strm->strm_packet_list));
    }

    return SCI_SUCCESS;
}


static int _add_strm_info_node_to_list(DEMUX_STRM_INFO_LIST_T *p_list,DEMUX_STRM_INFO_T *p_strm_info)
{
    DEMUX_STRM_INFO_NODE_T *p_node_cur;
    DEMUX_STRM_INFO_NODE_T *p_node_last;

    if( (PNULL == p_list) || (PNULL == p_strm_info) )
    {
        RMVB_PRT_ERR("Demux_rmvb,_add_strm_info_node_to_list, param error.");
        return SCI_ERROR;
    }
    p_node_cur = SCI_ALLOCA(sizeof(DEMUX_STRM_INFO_NODE_T));
    if (PNULL == p_node_cur)
    {
        RMVB_PRT_ERR("Demux_rmvb, _add_strminfo_node, malloc failed");
        return SCI_ERROR;
    }
    memcpy(&(p_node_cur->strm_info),p_strm_info,sizeof(DEMUX_STRM_INFO_T));

    if( p_list->node_cnt == 0 )
    {
        p_list->p_first = p_node_cur;
        p_node_cur->p_strm_node_pre = p_node_cur;
        p_node_cur->p_strm_node_next = p_node_cur;
    }
    else
    {
        p_node_last = p_list->p_first->p_strm_node_pre;
        //
        p_node_last->p_strm_node_next = p_node_cur;
        p_list->p_first->p_strm_node_pre = p_node_cur;
        //
        p_node_cur->p_strm_node_next = p_list->p_first;
        p_node_cur->p_strm_node_pre = p_node_last;
    }
    p_list->node_cnt++;

    return SCI_SUCCESS;
}


static int _free_all_strm_info_node_in_list(DEMUX_STRM_INFO_LIST_T *p_list)
{
    DEMUX_STRM_INFO_NODE_T *p_node_cur;
    DEMUX_STRM_INFO_NODE_T *p_node_next;

    if( (PNULL == p_list) || (0 == p_list->node_cnt) )
    {
        RMVB_PRT_ERR("Demux_rmvb,_free_all_strm_info_node_in_list, NULL");
        return SCI_ERROR;
    }

    p_node_cur = p_list->p_first;
    while(p_list->node_cnt > 0)
    {
        if(PNULL != p_node_cur)
        {
            p_node_next = p_node_cur->p_strm_node_next;
            SCI_FREE(p_node_cur);

            p_node_cur = p_node_next;
        }
        p_list->node_cnt--;
    }

    return SCI_SUCCESS;
}


static int _rvmb_demux_free_resource(RMVB_DEMUX_CTX_T *p_rvmb_demux_ctx)
{
    DEMUX_MEM_OP_T    *p_mem_op;
    //
    rm_parser *p_rm_parser;
    rm_parser_internal *p_rm_parser_internal;

    RMVB_PRT_ERR("_rvmb_demux_free_resource");

    if(PNULL == p_rvmb_demux_ctx)
    {
        return SCI_SUCCESS;
    }
    p_mem_op = &(p_rvmb_demux_ctx->mem_op);
    p_rm_parser = (rm_parser *)p_rvmb_demux_ctx->rmvb_lib_handle;
    p_rm_parser_internal = (rm_parser_internal *)p_rm_parser;

    // free audio stream.
    _destroy_audio_stream(&(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_AUDIO]));

    // free video stream.
    _destroy_video_stream(&(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_VIDEO]));

    // free demux lib handle and file cache buffer.
    if(PNULL != p_rm_parser)
    {
        SCI_FREE(p_rm_parser_internal->pPacketBuffer);
        p_rm_parser_internal->pPacketBuffer = PNULL;
        //
        rm_parseri_index_free(p_rm_parser_internal);
        //
        rm_parser_destroy(&p_rm_parser);
        p_rvmb_demux_ctx->rmvb_lib_handle = PNULL;
    }

    // free the strm info list.
    _free_all_strm_info_node_in_list(&(p_rvmb_demux_ctx->file_info.strm_info_lst));

    // freee demux adapter handle.
    SCI_FREE(p_rvmb_demux_ctx);

    return SCI_SUCCESS;
}


// open a demux and check the format.
static DEMUX_HANDLE rvmb_demux_open( DEMUX_IN_SRC_T        *p_in_src,
                                     DEMUX_MEM_OP_T        *p_mem_op,
                                     DEMUX_CALLBACK_OP_T   *p_callback_op
                                   )
{
    RMVB_DEMUX_CTX_T *p_rvmb_demux_ctx;
    DEMUX_FILE_INFO_T *p_file_info;
    //
    RMVB_DEMUX_STRM_T *p_strm;
    unsigned char *p_peek;
    //
    rm_parser *p_rm_parser;
    rm_parser_internal  *p_rm_parser_internal;
    rm_stream_header    *p_rm_strm_hdr  = HXNULL;
    //
    HX_RESULT retVal;
    uint32 idx;

    RMVB_PRT_ERR("rvmb_demux_open");

#if 0
    if( (PNULL == p_mem_op->pf_mem_malloc) || (PNULL == p_mem_op->pf_mem_free) )
    {
        return PNULL;
    }
#endif
    if( (PNULL == p_in_src->pf_file_read) || (PNULL == p_in_src->pf_file_peek) || 
        (PNULL == p_in_src->pf_file_seek)
      )
    {
        return PNULL;
    }

    p_rvmb_demux_ctx = SCI_ALLOCA(sizeof(RMVB_DEMUX_CTX_T));
    if(PNULL == p_rvmb_demux_ctx)
    {
        return PNULL;
    }
    memset(p_rvmb_demux_ctx,0,sizeof(RMVB_DEMUX_CTX_T));
    
    //init.
    p_rvmb_demux_ctx->p_owner = (void *)p_rvmb_demux_ctx;
    memcpy(&p_rvmb_demux_ctx->source_in,p_in_src,sizeof(DEMUX_IN_SRC_T));
    memcpy(&p_rvmb_demux_ctx->mem_op,p_mem_op,sizeof(DEMUX_MEM_OP_T));
    memcpy(&p_rvmb_demux_ctx->callback_op,p_callback_op,sizeof(DEMUX_CALLBACK_OP_T));

    // ---------------check the file.------------------------
    /* a little test to see if it could be a rm */
    if(!p_in_src->pf_file_peek(p_in_src->p_file, &p_peek, RM2YUV_INITIAL_READ_SIZE ))
    {
        RMVB_PRT_ERR("rvmb_demux_open, peek fail");
        _rvmb_demux_free_resource(p_rvmb_demux_ctx);
        return PNULL;
    }
    /* Make sure this is an .rm file */
    if (!rm_parser_is_rm_file(p_peek, RM2YUV_INITIAL_READ_SIZE))
    {
        RMVB_PRT_ERR("rvmb_demux_open, is not an .rm file.");
        _rvmb_demux_free_resource(p_rvmb_demux_ctx);
        return PNULL;
    }

    /* Create the parser struct */
    p_rm_parser = rm_parser_create2(NULL, 
                                    _rmvb_demux_error, 
                                    "rm_parser", 
                                    _rvmb_demux_malloc, 
                                    _rvmb_demux_free
                                    );
    if (!p_rm_parser)
    {
        RMVB_PRT_ERR("rvmb_demux_open, parser create fail");
        _rvmb_demux_free_resource(p_rvmb_demux_ctx);
        return PNULL;
    }
    p_rm_parser_internal = (rm_parser_internal*)p_rm_parser;
    p_rvmb_demux_ctx->rmvb_lib_handle = (RMVB_DEMUX_LIB_HANDLE)p_rm_parser;

    // TODO: why the packet buffer of lib is allocated here?
#if 1
    p_rm_parser_internal->pPacketBuffer = SCI_ALLOCA(PACKET_BUFFER_SIZE);
    if (p_rm_parser_internal->pPacketBuffer == PNULL)
    {
        RMVB_PRT_ERR("rvmb_demux_open, buffer malloc fail");

        _rvmb_demux_free_resource(p_rvmb_demux_ctx);
        return PNULL;
    }
    p_rm_parser_internal->ulPacketBufferSize = PACKET_BUFFER_SIZE;
    p_rm_parser_internal->ulUsedBufferSize = 0;
    p_rm_parser_internal->ulCurBufferOffset = 0;
#endif

    /* Set the FILE* into the parser */
    retVal = rm_parser_init_io( p_rm_parser, 
                                (void *)&p_rvmb_demux_ctx->source_in,
                                _rmvb_io_read,
                                _rmvb_io_seek);
    if (retVal != HXR_OK)
    {
        RMVB_PRT_ERR("rvmb_demux_open, parser init io fail");

        _rvmb_demux_free_resource(p_rvmb_demux_ctx);
        return PNULL;
    }

    // get the file info.
    /* Read all the headers at the beginning of the .rm file */
    retVal = rm_parser_read_headers(p_rm_parser);
    if (retVal != HXR_OK)
    {
        RMVB_PRT_ERR("rvmb_demux_open, read headers fail");

        _rvmb_demux_free_resource(p_rvmb_demux_ctx);
        return PNULL;
    }
    p_file_info = &p_rvmb_demux_ctx->file_info;
    p_file_info->media_duration_ms = p_rm_parser_internal->propHdr.duration;

    /* Get the number of streams */
    p_file_info->strm_num = rm_parser_get_num_streams(p_rm_parser);
    if (p_file_info->strm_num == 0)
    {
        RMVB_PRT_ERR("rvmb_demux_open, no stream in file.");

        _rvmb_demux_free_resource(p_rvmb_demux_ctx);
        return PNULL;
    }

    // create the stream and get the stream info.
    /* Now loop through the stream headers and find the audio and video */
    for (idx = 0; (idx < p_file_info->strm_num) && (retVal == HXR_OK); idx++)
    {
        retVal = rm_parser_get_stream_header(p_rm_parser, idx, &p_rm_strm_hdr);
        if (retVal == HXR_OK)
        {
            if (rm_stream_is_realaudio(p_rm_strm_hdr))
            {
                p_strm = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_AUDIO]);
                if( SCI_SUCCESS == _create_audio_stream(p_strm,p_rm_strm_hdr,idx) )
                {
                    p_strm->p_rm_parser_internal = p_rm_parser_internal;
                    p_rm_parser_internal->ulAudioStreamNum = idx;

                    // add one strm info node into the list.
                    _add_strm_info_node_to_list(&(p_file_info->strm_info_lst),&(p_strm->strm_info) );
                }
            }
            else if(rm_stream_is_realvideo(p_rm_strm_hdr))
            {
                p_strm = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_VIDEO]);
                if( SCI_SUCCESS == _create_video_stream(p_strm,p_rm_strm_hdr,idx) )
                {
                    p_strm->p_rm_parser_internal = p_rm_parser_internal;
                    p_rm_parser_internal->ulVideoStreamNum = idx;

                    // add one strm info node into the list.
                    _add_strm_info_node_to_list(&(p_file_info->strm_info_lst),&(p_strm->strm_info) );
                }
            }
            /* Destroy the stream header */
            rm_parser_destroy_stream_header(p_rm_parser, &p_rm_strm_hdr);
        }
    }

    if (SCI_FALSE == p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_AUDIO].b_used &&
        SCI_FALSE == p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_VIDEO].b_used)
    {
        RMVB_PRT_ERR("rvmb_demux_open, neigher audio nor video stream is valid.");

        _rvmb_demux_free_resource(p_rvmb_demux_ctx);
        return PNULL;
    }

    // load index for seek.
    rm_parseri_index_load(p_rm_parser_internal);

    return (DEMUX_HANDLE)p_rvmb_demux_ctx;
}


// close demux.
static int rvmb_demux_close(DEMUX_HANDLE *p_demux)
{
    RMVB_DEMUX_CTX_T *p_rvmb_demux_ctx = (RMVB_DEMUX_CTX_T *)p_demux;

    // check.
    if( (p_rvmb_demux_ctx == PNULL) || (p_rvmb_demux_ctx->p_owner!= (void *)p_demux) )
    {
        return SCI_ERROR;
    }
    
    // free all the source.
    _rvmb_demux_free_resource(p_rvmb_demux_ctx);

    return SCI_SUCCESS;
}

// get the media file information/user data.
static int rvmb_demux_get_file_info(DEMUX_HANDLE *p_demux, DEMUX_FILE_INFO_T **pp_file_info )
{
    RMVB_DEMUX_CTX_T *p_rvmb_demux_ctx;

    // check.
    if( (PNULL == p_demux) || (PNULL == pp_file_info) )
    {
        return SCI_ERROR;
    }
    p_rvmb_demux_ctx = (RMVB_DEMUX_CTX_T *)p_demux;
    if( p_rvmb_demux_ctx->p_owner!= (void *)p_demux )
    {
        return SCI_ERROR;
    }

    *pp_file_info = &(p_rvmb_demux_ctx->file_info);

    return SCI_SUCCESS;
}

static int rvmb_demux_get_file_udata(DEMUX_HANDLE *p_demux, DEMUX_DATA_BUF_T *p_des_buf )
{
    return SCI_ERROR;
}

// create/destroy the stream.
static DEMUX_STRM_HANDLE rvmb_demux_create_strm(DEMUX_HANDLE *p_demux, uint32 strm_id)
{
    RMVB_DEMUX_CTX_T *p_rvmb_demux_ctx = PNULL;
    RMVB_DEMUX_STRM_T *p_strm_audio;
    RMVB_DEMUX_STRM_T *p_strm_video;

    RMVB_PRT_ERR("rvmb_demux_create_strm, strm_id: %d", strm_id);

    // check.
    p_rvmb_demux_ctx = (RMVB_DEMUX_CTX_T *)p_demux;
    if( (PNULL == p_rvmb_demux_ctx ) ||
        ( (void *)p_rvmb_demux_ctx != p_rvmb_demux_ctx->p_owner )
      )
    {
        return PNULL;
    }
    p_strm_audio = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_AUDIO]);
    p_strm_video = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_VIDEO]);
    
    if( (p_strm_audio->strm_handle != PNULL) && 
        (p_strm_audio->strm_info.strm_id == strm_id)
      )
    {
        return (DEMUX_STRM_HANDLE)p_strm_audio;
    }
    else if( (p_strm_video->strm_handle != PNULL) && 
             (p_strm_video->strm_info.strm_id == strm_id)
           )
    {
        return (DEMUX_STRM_HANDLE)p_strm_video;
    }
    
    return PNULL;
}

static int rvmb_demux_destroy_strm(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm)
{
    return SCI_SUCCESS;
}

// add/del the stream outport.// connect/disconnect to the outport.
static int rvmb_demux_add_strm_outport(DEMUX_HANDLE *p_demux, 
                                DEMUX_STRM_HANDLE p_strm,
                                DEMUX_STRM_RX_SLAVE_T *p_outport )
{
    RMVB_DEMUX_STRM_T   *p_strm_ctx;
    DEMUX_STRM_RX_SLAVE_T *p_strm_outport_slave;
    uint32 cnt;

    if( (PNULL == p_demux) || (PNULL == p_strm) || (PNULL == p_outport))
    {
        return SCI_ERROR;
    }
    p_strm_ctx = (RMVB_DEMUX_STRM_T *)p_strm;

    RMVB_PRT_ERR("rvmb_demux_add_strm_outport,p_strm=0x%x,outport_num=%d", 
        p_strm,p_strm_ctx->outport_num
        );

    if(p_strm_ctx->outport_num >= RMVB_DEMUX_STRM_NUM_MAX)
    {
        return SCI_ERROR;
    }

    for(cnt = 0; cnt < RMVB_DEMUX_STRM_NUM_MAX;cnt++)
    {
        p_strm_outport_slave = &(p_strm_ctx->strm_outport[cnt]);
        if( (PNULL == p_strm_outport_slave->pf_rx_callback) &&
            (PNULL != p_outport->pf_rx_callback)
          )
        {
            // the outport is useable.
            memcpy(p_strm_outport_slave,p_outport,sizeof(DEMUX_STRM_RX_SLAVE_T));
_RMVB_DIS_INT_;
            p_strm_ctx->outport_num++;
_RMVB_EN_INT_;

            break;
        }
    }

    return SCI_SUCCESS;
}

static int rvmb_demux_del_strm_outport(DEMUX_HANDLE *p_demux, 
                                DEMUX_STRM_HANDLE p_strm, 
                                DEMUX_STRM_RX_SLAVE_T *p_outport )
{
    RMVB_DEMUX_STRM_T   *p_strm_ctx;
    DEMUX_STRM_RX_SLAVE_T *p_strm_outport_slave;
    uint32 cnt;

    if( (PNULL == p_demux) || (PNULL == p_strm) || (PNULL == p_outport))
    {
        return SCI_ERROR;
    }
    p_strm_ctx = (RMVB_DEMUX_STRM_T *)p_strm;

    for(cnt = 0; cnt < RMVB_DEMUX_STRM_NUM_MAX; cnt++)
    {
        p_strm_outport_slave = &(p_strm_ctx->strm_outport[cnt]);
        if(p_outport->pf_rx_callback == p_strm_outport_slave->pf_rx_callback)
        {
            // free the outport.
            memset(p_strm_outport_slave,0,sizeof(DEMUX_STRM_RX_SLAVE_T));

_RMVB_DIS_INT_;
            if(p_strm_ctx->outport_num > 0)
            {
                p_strm_ctx->outport_num--;
            }
_RMVB_EN_INT_;

            break;
        }
    }

    return SCI_SUCCESS;
}



// get stream config/user data.
static int rvmb_demux_get_strm_cfgdata(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, DEMUX_DATA_BUF_T *p_des_buf )
{
    RMVB_DEMUX_CTX_T  *p_rvmb_demux_ctx = PNULL;
    RMVB_DEMUX_STRM_T *p_strm_audio;
    RMVB_DEMUX_STRM_T *p_strm_video;

    RMVB_PRT_ERR("rvmb_demux_get_strm_cfgdata, p_strm: 0x%x)", p_strm);
    // check.
    p_rvmb_demux_ctx = (RMVB_DEMUX_CTX_T *)p_demux;
    if( (PNULL == p_rvmb_demux_ctx ) ||
        ( (void *)p_rvmb_demux_ctx != p_rvmb_demux_ctx->p_owner )
      )
    {
        return SCI_ERROR;
    }

    //
    p_strm_audio = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_AUDIO]);
    p_strm_video = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_VIDEO]);
    //
    if(p_strm_audio == (RMVB_DEMUX_STRM_T *)p_strm)
    {
        if(p_des_buf->len >= p_strm_audio->strm_cfgdata.len)
        {
            if(p_strm_audio->strm_cfgdata.len > 0)
            {
                memcpy( p_des_buf->p_buf, 
                        p_strm_audio->strm_cfgdata.p_buf,
                        p_strm_audio->strm_cfgdata.len);
            }
        }
        else
        {
            return SCI_ERROR;
        }
    }
    else if(p_strm_video == (RMVB_DEMUX_STRM_T *)p_strm)
    {
        if(p_des_buf->len >= p_strm_video->strm_cfgdata.len)
        {
            if(p_strm_video->strm_cfgdata.len > 0)
            {
                memcpy( p_des_buf->p_buf, 
                        p_strm_video->strm_cfgdata.p_buf,
                        p_strm_video->strm_cfgdata.len);
            }
        }
        else
        {
            return SCI_ERROR;
        }
    }
    else
    {
        return SCI_ERROR;
    }

    return SCI_SUCCESS;
}

static int rvmb_demux_get_strm_udata(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, DEMUX_DATA_BUF_T *p_des_buf )
{
    return SCI_ERROR;
}

// file/stream seek, ms.
int rvmb_demux_file_seek(DEMUX_HANDLE *p_demux, uint32 new_time)
{
    RMVB_DEMUX_CTX_T  *p_rvmb_demux_ctx = PNULL;
    DEMUX_IN_SRC_T    *p_in_src;
    //
    RMVB_DEMUX_STRM_T *p_strm_audio;
    RMVB_DEMUX_STRM_T *p_strm_video;
    //
    rm_parser  *p_parser;
    rm_parser_internal  *p_rm_parser_internal;
    //
    ra_depack  *p_ra_depack;
    ra_depack_internal *p_ra_depack_internal;
    //
    rv_depack  *p_rv_depack;
    rv_depack_internal *p_rv_depack_internal;
    //
    HX_RESULT   retVal = HXR_OK;
    //
    uint8     *p_peek;
    uint32    read_len = RM_PARSER_PACKET_HEADER_SIZE;

    RMVB_PRT_ERR("rvmb_demux_file_seek, time: %d", new_time);
    // check.
    p_rvmb_demux_ctx = (RMVB_DEMUX_CTX_T *)p_demux;
    if( (PNULL == p_rvmb_demux_ctx ) ||
        ( (void *)p_rvmb_demux_ctx != p_rvmb_demux_ctx->p_owner )
      )
    {
        return SCI_ERROR;
    }
    p_in_src = &(p_rvmb_demux_ctx->source_in);
    //
    p_strm_audio = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_AUDIO]);
    p_strm_video = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_VIDEO]);
    //
    p_parser = (rm_parser *)p_rvmb_demux_ctx->rmvb_lib_handle;
    p_rm_parser_internal = (rm_parser_internal *)p_parser;
    //
    p_ra_depack = (ra_depack *)p_strm_audio->strm_handle;
    p_ra_depack_internal = (ra_depack_internal *)p_ra_depack;
    //
    p_rv_depack = (rv_depack *)p_strm_video->strm_handle;
    p_rv_depack_internal = (rv_depack_internal *)p_rv_depack;

    // seek
    retVal = rm_parser_seek(p_parser, new_time);
//    if(HXR_OK != retVal)
    {
        RMVB_PRT_ERR("rvmb_demux_file_seek, rm_parser_seek, retVal=0x%x",retVal);
//        return SCI_ERROR;
    }
    if( (!p_rvmb_demux_ctx->is_firstseek_done) && (0 == new_time) )
    {
        p_rm_parser_internal->ulCurFileOffset -= RM_PARSER_PACKET_HEADER_SIZE;
        p_in_src->pf_file_seek( p_in_src->p_file,
                                p_rm_parser_internal->ulCurFileOffset,
                                FDEMUX_SEEK_ORIGIN_SET
                              );
    }
    RMVB_PRT_ERR("rvmb_demux_file_seek, ulCurFileOffset=%d",
        p_rm_parser_internal->ulCurFileOffset);
    
    if(!p_in_src->pf_file_peek(p_in_src->p_file,&p_peek, RM_PARSER_PACKET_HEADER_SIZE ))
    {
        RMVB_PRT_ERR("rvmb_demux_file_seek,peek fail");
        return SCI_ERROR;
    }
    p_peek += 6;
    read_len -= 6;
    p_rvmb_demux_ctx->file_demux_time_cur = rm_unpack32(&p_peek, &read_len);
    p_rvmb_demux_ctx->file_demux_time_target = p_rvmb_demux_ctx->file_demux_time_cur;
    
    if(PNULL != p_ra_depack_internal)
    {
        p_ra_depack_internal->pSubStreamHdr[0].bSeeked = 1;  // select substream 0
        //
        p_strm_audio->strm_demux_time_cur = p_rvmb_demux_ctx->file_demux_time_cur;
        p_strm_audio->strm_demux_time_target = p_rvmb_demux_ctx->file_demux_time_cur;
    }
    if(PNULL != p_rv_depack_internal)
    {
        p_rv_depack_internal->bSeeked = 1;
        //
        p_strm_video->strm_demux_time_cur = p_rvmb_demux_ctx->file_demux_time_cur;
        p_strm_video->strm_demux_time_target = p_rvmb_demux_ctx->file_demux_time_cur;
    }

    //
    p_rm_parser_internal->ulUsedBufferSize = 0;
    p_rm_parser_internal->ulCurBufferOffset = 0;

    p_rvmb_demux_ctx->is_firstseek_done = 1;

    return SCI_SUCCESS;
}

static int rvmb_demux_strm_seek(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, uint32 new_time)
{
    return SCI_ERROR;
}

// get the current time of stream, ms.
static int rvmb_demux_strm_gettime(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm)
{
    RMVB_DEMUX_CTX_T  *p_rvmb_demux_ctx = PNULL;
    RMVB_DEMUX_STRM_T *p_strm_audio;
    RMVB_DEMUX_STRM_T *p_strm_video;
    int strm_time_cur = 0;

    RMVB_PRT_ERR("rvmb_demux_strm_gettime, p_strm: %d", p_strm);
    // check.
    p_rvmb_demux_ctx = (RMVB_DEMUX_CTX_T *)p_demux;
    if( (PNULL == p_rvmb_demux_ctx ) ||
        ( (void *)p_rvmb_demux_ctx != p_rvmb_demux_ctx->p_owner )
      )
    {
        return 0;
    }
    //
    p_strm_audio = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_AUDIO]);
    p_strm_video = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_VIDEO]);
    //
    if(p_strm_audio == (RMVB_DEMUX_STRM_T *)p_strm)
    {
        strm_time_cur = p_strm_audio->strm_demux_time_cur;
    }
    else if(p_strm_video == (RMVB_DEMUX_STRM_T *)p_strm)
    {
        strm_time_cur = p_strm_video->strm_demux_time_cur;
    }
    else
    {
        strm_time_cur = p_rvmb_demux_ctx->file_demux_time_cur;
    }

    return strm_time_cur;
}

// demux the packet one by one. for AV invterval file. 
// packet/frame data will send to output port by callback func.
static int rvmb_demux_demux_file_packet(DEMUX_HANDLE *p_demux,uint32 packet_num)
{
    return SCI_ERROR;
}

static int rvmb_demux_demux_file_period(DEMUX_HANDLE *p_demux,uint32 period_ms)
{
    RMVB_DEMUX_CTX_T  *p_rvmb_demux_ctx = PNULL;
    RMVB_DEMUX_STRM_T *p_strm_audio;
    RMVB_DEMUX_STRM_T *p_strm_video;
    DEMUX_STRM_RX_SLAVE_T   *p_outport;
    //
    rm_parser  *p_parser;
    ra_depack  *p_ra_depack;
    rv_depack  *p_rv_depack;
    //
    rm_packet  *p_packet = PNULL;
    HX_RESULT   retVal = HXR_OK;
    uint32  do_cnt = 0;
    uint32  do_useful_cnt = 0;
    uint32  outpot_cnt = 0;
    int  outbuf_status = DEMUX_IO_BUF_STATUS_AF;//almost full.

    RMVB_PRT_ERR("rvmb_demux_demux_file_period,%d",period_ms);

    // check.
    p_rvmb_demux_ctx = (RMVB_DEMUX_CTX_T *)p_demux;
    if( (PNULL == p_rvmb_demux_ctx ) ||
        ( (void *)p_rvmb_demux_ctx != p_rvmb_demux_ctx->p_owner )
      )
    {
        RMVB_PRT_ERR("rvmb_demux_demux_file_period, p_demux=0x%x",p_rvmb_demux_ctx);
        return SCI_ERROR;
    }
    //
    p_parser = (rm_parser *)p_rvmb_demux_ctx->rmvb_lib_handle;
    p_strm_audio = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_AUDIO]);
    p_strm_video = &(p_rvmb_demux_ctx->strm[RMVB_DEMUX_STRM_ID_VIDEO]);

    p_strm_audio->strm_demux_time_target += period_ms;
    p_strm_video->strm_demux_time_target += period_ms;
    //
    while( (p_strm_audio->strm_demux_time_cur < p_strm_audio->strm_demux_time_target) ||
           (p_strm_video->strm_demux_time_cur < p_strm_video->strm_demux_time_target)
        )
    {
        // check the rx buffer status. if is almost full, break.
        for( outpot_cnt = 0; outpot_cnt < p_strm_audio->outport_num; outpot_cnt++ )
        {
            p_outport = &(p_strm_audio->strm_outport[outpot_cnt]);
            if(PNULL != p_outport->pf_check_rx_buf_status)
            {
                outbuf_status = (p_outport->pf_check_rx_buf_status)(p_outport->p_receiver);
                if( (DEMUX_IO_BUF_STATUS_AF == outbuf_status) ||
                    (DEMUX_IO_BUF_STATUS_F == outbuf_status)
                  )
                {
                    RMVB_PRT_ERR("rvmb_demux_demux_file_period,audio, outbuf_status=%d",outbuf_status);
                    return SCI_SUCCESS;
                }
            }
        }
        for( outpot_cnt = 0; outpot_cnt < p_strm_video->outport_num; outpot_cnt++ )
        {
            p_outport = &(p_strm_video->strm_outport[outpot_cnt]);
            if(PNULL != p_outport->pf_check_rx_buf_status)
            {
                outbuf_status = (p_outport->pf_check_rx_buf_status)(p_outport->p_receiver);
                if( (DEMUX_IO_BUF_STATUS_AF == outbuf_status) ||
                    (DEMUX_IO_BUF_STATUS_F == outbuf_status)
                  )
                {
                    RMVB_PRT_ERR("rvmb_demux_demux_file_period,video, outbuf_status=%d",outbuf_status);
                    return SCI_SUCCESS;
                }
            }
        }

        // check the tx packet status. if is empty, break.
        if((SCI_TRUE == p_strm_audio->b_used)&&(p_strm_audio->strm_packet_list.node_num == 0 ))
        {
            RMVB_PRT_ERR("rvmb_demux_demux_file_period, audio node_num=0");
            return SCI_SUCCESS;
        }
        if((SCI_TRUE == p_strm_video->b_used)&&(p_strm_video->strm_packet_list.node_num == 0 ))
        {
            RMVB_PRT_ERR("rvmb_demux_demux_file_period, video node_num=0");
            return SCI_SUCCESS;
        }

        // do demux.
        retVal = rm_parser_get_packet(p_parser, &p_packet);

        RMVB_PRT_NONE("rvmb_demux_demux_file_period,retVal=%d",retVal);
        
        if( (HXR_OK == retVal) && ( PNULL != p_packet ) )
        {
            if((SCI_TRUE == p_strm_audio->b_used)&&( p_packet->usStream == p_strm_audio->strm_info.strm_id))
            {
                do_useful_cnt++;
                p_ra_depack = (ra_depack *)p_strm_audio->strm_handle;
                //
                retVal = ra_depack_add_packet(p_ra_depack, p_packet);
                RMVB_PRT_NONE("rvmb_demux_demux_file_period, audio depack ret=%d",retVal);
                //
                p_strm_audio->strm_demux_time_cur = p_packet->ulTime;

                if(HX_KEYFRAME_RULE == p_packet->ucASMRule)
                {
                    p_strm_audio->frm_type_cur = PACKET_FLAG_FRM_I;
                }
                else
                {
                    p_strm_audio->frm_type_cur = PACKET_FLAG_FRM_UNKNOW;
                }
            }
            else if( (SCI_TRUE == p_strm_video->b_used)&&(p_packet->usStream == p_strm_video->strm_info.strm_id ))
            {
                do_useful_cnt++;
                p_rv_depack = (rv_depack *)p_strm_video->strm_handle;
                //
                retVal = rv_depack_add_packet(p_rv_depack, p_packet);
                RMVB_PRT_NONE("rvmb_demux_demux_file_period, video depack ret=%d",retVal);
                //
                p_strm_video->strm_demux_time_cur = p_packet->ulTime;
            }
            else
            {
                RMVB_PRT_ERR("Demux_rmvb, Demux, undefined packet,usStream=%d",p_packet->usStream);
            }

            rm_parser_destroy_packet(p_parser, &p_packet);
        }

        do_cnt++;
        if( do_cnt > do_useful_cnt + 100 )
        {
            // if do nothing for many times, break out.
            RMVB_PRT_ERR("rvmb_demux_demux_file_period,time out,%d,%d",do_cnt,do_useful_cnt);
            break;
        }
    }

    return (HXR_OK == retVal)? SCI_SUCCESS : SCI_ERROR;
}


// demux the steam seperately. for AV seperate file.
static int rvmb_demux_demux_strm_packet(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm, uint32 packet_num)
{
    return SCI_ERROR;
}
static int rvmb_demux_demux_strm_period(DEMUX_HANDLE *p_demux, DEMUX_STRM_HANDLE p_strm,uint32 period_ms)
{
    return SCI_ERROR;
}


// IOControl for externtion.
static int rvmb_demux_IOControl(DEMUX_HANDLE *p_demux, uint32 cmd, void *p_param)
{
    return SCI_ERROR;
}


// Global entry, registed in user demux table.
PUBLIC FILE_DEMUX_ENTRY_T g_rmvb_demux_entry =
{
    rvmb_demux_open,
    rvmb_demux_close,
    //
    rvmb_demux_get_file_info,
    rvmb_demux_get_file_udata,
    //
    rvmb_demux_create_strm,
    rvmb_demux_destroy_strm,
    //
    rvmb_demux_add_strm_outport,
    rvmb_demux_del_strm_outport,
    //
    rvmb_demux_get_strm_cfgdata,
    rvmb_demux_get_strm_udata,
    //
    rvmb_demux_file_seek,
    rvmb_demux_strm_seek,
    rvmb_demux_strm_gettime,
    //
    rvmb_demux_demux_file_packet,
    rvmb_demux_demux_file_period,
    //
    rvmb_demux_demux_strm_packet,
    rvmb_demux_demux_strm_period,
    //
    rvmb_demux_IOControl
};




