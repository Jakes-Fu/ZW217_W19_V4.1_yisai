/******************************************************************************
 ** File Name:      cardlog_uart.c                                                 *
 ** Author:                                                             *
 ** DATE:                                                           *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/

#ifdef SPRD_RAW_CARD_LOG

#include "os_api.h"
#include "sci_types.h"
#include "com_drvapi.h"
#include "cardlog_pal.h"
#include "sio.h"
#include "priority_system.h"
#include "sci_api.h"

#define CARDLOG_BUFSIZE_INSECT 32
#define CARDLOG_BUFSIZE (512*CARDLOG_BUFSIZE_INSECT)

typedef BOOLEAN (*CARDLOG_READ) (uint32 start, uint32 scts, void *buf);
typedef BOOLEAN (*CARDLOG_WRITE) (uint32 start, uint32 scts, void *buf);
typedef BOOLEAN (*CARDLOG_ERASE) (uint32 start, uint32 scts);
typedef uint32 (*CARDLOG_GETCAPACITY) (void);

typedef struct
{
    CARDLOG_READ cardLog_read;
    CARDLOG_WRITE cardLog_write;
    CARDLOG_ERASE cardLog_erase;
    CARDLOG_GETCAPACITY cardLog_getCapacity;
} CARDLOG_PAL_FUNC_T;

typedef struct
{
    CARDLOG_READ cardLog_read;
    CARDLOG_WRITE cardLog_write;
    CARDLOG_ERASE cardLog_erase;
    CARDLOG_GETCAPACITY cardLog_getCapacity;

    uint32  segmentStartPos;
    uint32  segmentSize;
    uint32  segmentCount;

    uint32  curSegId;
    uint32  GroupId;
    uint32  curSN;

    uint32  dataStartPos;
    uint32  dataSize;

    uint8   logBuf[CARDLOG_BUFSIZE];
    uint32  bufPos;
    uint32  curBufSectId;
} CARDLOG_PAL_T;

LOCAL CARDLOG_PAL_T s_cardlogDev;

typedef PACK struct DISKINFO_Tag //lint !e140 confirmed by xuepeng
{
    //该段只在格式化时写入固定信息，之后不会改变
    uint8   OEMname[16];        //eg."SPREADTRUM" ，该标志也用于判断磁盘当前是否被用作存储器
    uint32  segmentStartPos;    //第一个段的首sect位置，该位置之前的扇区保留，之后的区域被划分成一个个的段区域
    uint32  segmentSize;        //磁盘存储的段大小，该大小必须足够存下整个assert信息，同时又不能过大，建议64M,单位block
    uint32  segmentCount;       //有效段的总个数
    uint8   reserved[512-28];   //保留字段
} DISKINFO_T;

typedef PACK struct SEGMENTINFO_Tag //lint !e140 confirmed by xuepeng
{
    //sect 1    //该sector只在该段启用时写入一次信息
    uint32  groupFlag;      //0xFFFFFFFF表示无效。磁盘的所有group号必须连续，顺序累加，另类视为无效
    uint32  SN;         //当前段在整个group中的序号，如果整个盘的group是一样的，那么该序号可以用来判断log首位置
    uint32  dataStartPos;       //该段中log数据起始sector位置
    uint8   reserved0[512-12];  //保留字段;
    //sect 2    //该sector用于记录实际数据流长度，可能被多次写入
    uint32  dataSize;       //该段有效数据流长度；如果当前段是assert段类型 ，那么该长度是所有assert小段长度之和,单位字节
    uint8   reserved1[512-4];   //保留字段
} SEGMENTINFO_T;


typedef struct
{
    uint32 DiskSectCount;
    uint32 SecPerSegment;
} DSK_FORMAT_T;

LOCAL DSK_FORMAT_T diskFormatTable[] =
{
    {32*2*1024,0},          // <= 32M invalid capacity
    {128*2*1024,8*2*1024},      // <= 128M 8M/seg
    {256 * 2*1024, 16*2*1024},  // <= 256M 16M/seg
    {512*2*1024 , 32*2*1024},   // <= 512M , 32M/seg
    {1024 *2*1024 , 64*2*1024}, // <= 1024M , 64M/seg
    {32*1024*2*1024,128 *2*1024},   // <= 32G , 128M/seg
    {0xFFFFFFFF,256*2*1024} //other 256M/seg
};

LOCAL void _CardLog_Regist (CARDLOG_PAL_FUNC_T func)
{
    s_cardlogDev.cardLog_read = func.cardLog_read;
    s_cardlogDev.cardLog_write = func.cardLog_write;
    s_cardlogDev.cardLog_erase = func.cardLog_erase;
    s_cardlogDev.cardLog_getCapacity = func.cardLog_getCapacity;
    s_cardlogDev.bufPos = 0;
}

/*LOCAL void _CardLog_UnReg()
{
    s_cardlogDev.cardLog_read = 0;
    s_cardlogDev.cardLog_write = 0;
    s_cardlogDev.cardLog_erase = 0;
    s_cardlogDev.cardLog_getCapacity = 0;
    s_cardlogDev.bufPos = 0;
}*/

#ifndef __BIG_ENDIAN
#define swap32Endian(l) (l)
#else
#define swap32Endian(l) ((((l) & 0xff000000) >> 24) |(((l) & 0x00ff0000) >>  8) |(((l) & 0x0000ff00) <<  8) |(((l) & 0x000000ff) << 24))
#endif

LOCAL void _CardLog_Swap_DiskInfo_Endian (DISKINFO_T *diskInfoPtr)
{
    diskInfoPtr->segmentStartPos = swap32Endian ( (uint32) diskInfoPtr->segmentStartPos);
    diskInfoPtr->segmentSize = swap32Endian ( (uint32) diskInfoPtr->segmentSize);
    diskInfoPtr->segmentCount = swap32Endian ( (uint32) diskInfoPtr->segmentCount);
}

LOCAL void _CardLog_Swap_SegInfo_Endian (SEGMENTINFO_T *segInfoPtr)
{
    segInfoPtr->groupFlag = swap32Endian (segInfoPtr->groupFlag);
    segInfoPtr->SN = swap32Endian (segInfoPtr->SN);
    segInfoPtr->dataStartPos = swap32Endian (segInfoPtr->dataStartPos);

    segInfoPtr->dataSize = swap32Endian (segInfoPtr->dataSize);
}

LOCAL BOOLEAN _CardLog_UpdateSeg_datasize (uint32 datasize)
{
    uint8 buf[512];

    SCI_MEMSET (buf,0,512);
    * ( (uint32 *) buf) = swap32Endian (datasize);

    if (FALSE == s_cardlogDev.cardLog_write ( (s_cardlogDev.curSegId*s_cardlogDev.segmentSize+s_cardlogDev.segmentStartPos) +1,1,buf))
    {
        return FALSE;
    }

    return TRUE;
}
LOCAL BOOLEAN _CardLog_UpdateSeg_BasicInfo (uint32 groupId,uint32 SN,uint32 dataStartPos)
{
    SEGMENTINFO_T segInfo;

    SCI_MEMSET ( ( (uint8 *) &segInfo),0,sizeof (SEGMENTINFO_T));

    segInfo.groupFlag = groupId;
    segInfo.SN = SN;
    segInfo.dataStartPos = dataStartPos;
    segInfo.dataSize = 0;

    _CardLog_Swap_SegInfo_Endian (&segInfo);

    if (FALSE == s_cardlogDev.cardLog_write ( (s_cardlogDev.curSegId*s_cardlogDev.segmentSize+s_cardlogDev.segmentStartPos),2, (uint8 *) &segInfo))
    {
        return FALSE;
    }

    return TRUE;
}

LOCAL BOOLEAN _CardLog_FlushBuf (void)
{
    BOOLEAN result = TRUE;
    uint32 num;

    if (0 != s_cardlogDev.bufPos)
    {
        //---
        if (0 == (s_cardlogDev.bufPos%512))
        {
            num = s_cardlogDev.bufPos/512;
        }
        else
        {
            num = (s_cardlogDev.bufPos/512) +1;
        }

        //---
        if (FALSE == s_cardlogDev.cardLog_write (s_cardlogDev.curBufSectId, num, s_cardlogDev.logBuf))
        {
            result = FALSE;
        }

        if (FALSE == _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize))
        {
            result = FALSE;
        }
    }

    return result;
}


LOCAL BOOLEAN _CardLog_SDOUT (uint8 *buf,uint32 size)
{
    BOOLEAN result = TRUE;
    uint32 num;

    if (s_cardlogDev.dataSize + size > ((s_cardlogDev.segmentSize - s_cardlogDev.dataStartPos) * 512))
    {
        //---
        if (0 == (s_cardlogDev.bufPos%512))
        {
            num = s_cardlogDev.bufPos/512;
        }
        else
        {
            num = (s_cardlogDev.bufPos/512) +1;
        }

        //---
        if (FALSE == s_cardlogDev.cardLog_write (s_cardlogDev.curBufSectId, num, s_cardlogDev.logBuf))
        {
            result = FALSE;
        }

        if (FALSE == _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize))
        {
            result = FALSE;
        }

        s_cardlogDev.dataSize = 0;
        s_cardlogDev.curSN++;
        s_cardlogDev.curSegId++;

        if (s_cardlogDev.curSegId == s_cardlogDev.segmentCount)
        {
            s_cardlogDev.curSegId = 0;
        }

        if (FALSE == _CardLog_UpdateSeg_BasicInfo (s_cardlogDev.GroupId,s_cardlogDev.curSN,s_cardlogDev.dataStartPos))
        {
            result = FALSE;
        }

        s_cardlogDev.curBufSectId = s_cardlogDev.curSegId * s_cardlogDev.segmentSize +s_cardlogDev.segmentStartPos+s_cardlogDev.dataStartPos;
        s_cardlogDev.bufPos = 0;

    }

    //---
    if ( (s_cardlogDev.bufPos+size) >= CARDLOG_BUFSIZE)
    {
        if (0 != s_cardlogDev.bufPos)
        {
            SCI_MEMCPY (& (s_cardlogDev.logBuf[s_cardlogDev.bufPos]),buf,CARDLOG_BUFSIZE-s_cardlogDev.bufPos);

            if (FALSE == s_cardlogDev.cardLog_write (s_cardlogDev.curBufSectId,CARDLOG_BUFSIZE_INSECT,s_cardlogDev.logBuf))
            {
                result = FALSE;
            }

            s_cardlogDev.curBufSectId += CARDLOG_BUFSIZE_INSECT;

            s_cardlogDev.dataSize += CARDLOG_BUFSIZE-s_cardlogDev.bufPos;
            buf += (CARDLOG_BUFSIZE-s_cardlogDev.bufPos);
            size -= (CARDLOG_BUFSIZE-s_cardlogDev.bufPos);
            s_cardlogDev.bufPos = 0;

            //---for test---
            if (FALSE == _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize))
            {
                result = FALSE;
            }

            //------------
        }

        while (size >= CARDLOG_BUFSIZE)
        {
            if (FALSE == s_cardlogDev.cardLog_write (s_cardlogDev.curBufSectId,CARDLOG_BUFSIZE_INSECT,buf))
            {
                result = FALSE;
            }

            s_cardlogDev.curBufSectId += CARDLOG_BUFSIZE_INSECT;
            s_cardlogDev.dataSize += CARDLOG_BUFSIZE;
            buf += CARDLOG_BUFSIZE;
            size -= CARDLOG_BUFSIZE;

            //---for test---
            if (FALSE == _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize))
            {
                result = FALSE;
            }

            //------------
        }

        if (0 != size)
        {
            SCI_MEMCPY (& (s_cardlogDev.logBuf[0]),buf,size);
            s_cardlogDev.bufPos = size;
            s_cardlogDev.dataSize += size;
        }
    }
    else // if((s_cardlogDev.bufPos+size) < CARDLOG_BUFSIZE)
    {
        SCI_MEMCPY (& (s_cardlogDev.logBuf[s_cardlogDev.bufPos]),buf,size);
        s_cardlogDev.bufPos += size;
        s_cardlogDev.dataSize += size;
    }

    return result;
    //---
}



#define SEGMENT_START 0x10
#define SEGMENT_DATA_START 0x10

LOCAL BOOLEAN _CardLog_DSK_Format (void)
{
    DISKINFO_T diskInfoSect;
    SEGMENTINFO_T segInfo;
    uint8 oemName[] = {'S','P','R','E','A','D','T','R','U','M',0,0,0,0,0,0};
    uint32 disksize;
    uint32 i;

    /*Erase All disk*/
    SCI_MEMSET ( ( (uint8 *) &diskInfoSect),0,sizeof (DISKINFO_T));

    if (FALSE == s_cardlogDev.cardLog_write (0,1, (uint8 *) &diskInfoSect))
    {
        return FALSE;
    }

    if (FALSE == s_cardlogDev.cardLog_erase (0,s_cardlogDev.cardLog_getCapacity()))
    {
        return FALSE;
    }

    //

    /*Build DSK info Sect*/
    SCI_MEMSET ( ( (uint8 *) &diskInfoSect),0,sizeof (DISKINFO_T));
    SCI_MEMCPY ( (uint8 *) &diskInfoSect,oemName,16);
    diskInfoSect.segmentStartPos = SEGMENT_START;

    disksize = s_cardlogDev.cardLog_getCapacity() - SEGMENT_START;

    for (i = 0; i < sizeof (diskFormatTable) /sizeof (DSK_FORMAT_T); i ++)
    {
        if (disksize <= diskFormatTable[i].DiskSectCount)
        {
            diskInfoSect.segmentSize = diskFormatTable[i].SecPerSegment;
            break;
        }
    }

    if (0 == diskInfoSect.segmentSize)
    {
        return FALSE;
    }

    diskInfoSect.segmentCount = disksize/ ( (uint32) (diskInfoSect.segmentSize));
    s_cardlogDev.segmentStartPos = SEGMENT_START;
    s_cardlogDev.segmentSize = diskInfoSect.segmentSize;
    s_cardlogDev.segmentCount = diskInfoSect.segmentCount;
    _CardLog_Swap_DiskInfo_Endian (&diskInfoSect);

    /*Build segment sect*/
    SCI_MEMSET ( ( (uint8 *) &segInfo),0,sizeof (SEGMENTINFO_T));
    segInfo.groupFlag = 0xFFFFFFFF;
    segInfo.SN = 0;
    segInfo.dataStartPos = SEGMENT_DATA_START;
    segInfo.dataSize = 0;

    _CardLog_Swap_SegInfo_Endian (&segInfo);

    /*write seg info*/
    for (i = 0; i < s_cardlogDev.segmentCount; i++)
    {
        if (FALSE == s_cardlogDev.cardLog_write (SEGMENT_START + i * (s_cardlogDev.segmentSize),2, (uint8 *) &segInfo))
        {
            return FALSE;
        }
    }

    /*write dsk info*/
    if (FALSE == s_cardlogDev.cardLog_write (0,1, (uint8 *) &diskInfoSect))
    {
        return FALSE;
    }

    /*init s_cardlogDev control block*/

    s_cardlogDev.curSegId = 0;
    s_cardlogDev.GroupId = 0;
    s_cardlogDev.curSN = 0;

    s_cardlogDev.dataStartPos = SEGMENT_DATA_START;
    s_cardlogDev.dataSize = 0;

    s_cardlogDev.bufPos = 0;
    s_cardlogDev.curBufSectId = SEGMENT_START + SEGMENT_DATA_START;

    return TRUE;
}

LOCAL BOOLEAN _CardLog_DSK_Init (void)
{
    DISKINFO_T diskInfoSect;
    SEGMENTINFO_T segInfo;
    uint8 oemName[] = {'S','P','R','E','A','D','T','R','U','M',0,0,0,0,0,0};
    uint8 tmpoemName[16];
    uint32 i;
    uint32 groupId;
    uint32 SN;
    /**/
    s_cardlogDev.cardLog_read (0,1, (uint8 *) &diskInfoSect);

    _CardLog_Swap_DiskInfo_Endian (&diskInfoSect);

    SCI_MEMCPY (tmpoemName,(uint8*)diskInfoSect.OEMname,16);

    if (0 != memcmp (oemName,tmpoemName,16))
    {
        return FALSE;
    }

    s_cardlogDev.segmentStartPos = diskInfoSect.segmentStartPos;
    s_cardlogDev.segmentSize = diskInfoSect.segmentSize;
    s_cardlogDev.segmentCount = diskInfoSect.segmentCount;

    /**/
    if (FALSE == s_cardlogDev.cardLog_read (s_cardlogDev.segmentStartPos,2, (uint8 *) &segInfo))
    {
        return FALSE;
    }

    _CardLog_Swap_SegInfo_Endian (&segInfo);

    if (0xFFFFFFFF == segInfo.groupFlag)
    {
        s_cardlogDev.curSegId = 0;
        s_cardlogDev.GroupId = 0;
        s_cardlogDev.curSN = 0;

        s_cardlogDev.dataStartPos = segInfo.dataStartPos;
        s_cardlogDev.dataSize = 0;

        s_cardlogDev.curBufSectId = s_cardlogDev.segmentStartPos+s_cardlogDev.dataStartPos;
        s_cardlogDev.bufPos = 0;

        _CardLog_UpdateSeg_BasicInfo (s_cardlogDev.GroupId,s_cardlogDev.curSN,s_cardlogDev.dataStartPos);
        _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize);

        return TRUE;
    }

    /**/
    s_cardlogDev.dataStartPos = segInfo.dataStartPos;

    groupId = segInfo.groupFlag;
    SN = segInfo.SN;

    for (i = 1; i < s_cardlogDev.segmentCount; i++)
    {
        if (FALSE == s_cardlogDev.cardLog_read ( (s_cardlogDev.segmentStartPos) + i * (s_cardlogDev.segmentSize),2, (uint8 *) &segInfo))
        {
            return FALSE;
        }

        _CardLog_Swap_SegInfo_Endian (&segInfo);

        if (0xFFFFFFFF != segInfo.groupFlag)
        {
            if (groupId > segInfo.groupFlag)
            {
                groupId++;
                s_cardlogDev.curSegId = i;
                s_cardlogDev.GroupId = groupId;
                s_cardlogDev.curSN = 0;

                s_cardlogDev.dataStartPos = segInfo.dataStartPos;
                s_cardlogDev.dataSize = 0;

                s_cardlogDev.curBufSectId = s_cardlogDev.curSegId * s_cardlogDev.segmentSize +s_cardlogDev.segmentStartPos+s_cardlogDev.dataStartPos;;
                s_cardlogDev.bufPos = 0;

                _CardLog_UpdateSeg_BasicInfo (s_cardlogDev.GroupId,s_cardlogDev.curSN,s_cardlogDev.dataStartPos);
                _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize);

                return TRUE;
            }
            else if ( (SN >= segInfo.SN) && (groupId == segInfo.groupFlag))
            {
                groupId++;
                s_cardlogDev.curSegId = i;
                s_cardlogDev.GroupId = groupId;
                s_cardlogDev.curSN = 0;

                s_cardlogDev.dataStartPos = segInfo.dataStartPos;
                s_cardlogDev.dataSize = 0;

                s_cardlogDev.curBufSectId = s_cardlogDev.curSegId * s_cardlogDev.segmentSize +s_cardlogDev.segmentStartPos+s_cardlogDev.dataStartPos;;
                s_cardlogDev.bufPos = 0;

                _CardLog_UpdateSeg_BasicInfo (s_cardlogDev.GroupId,s_cardlogDev.curSN,s_cardlogDev.dataStartPos);
                _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize);

                return TRUE;
            }
            else
            {
                groupId = segInfo.groupFlag;
                SN = segInfo.SN;
            }
        }
        else
        {
            //do nothing
            groupId++;
            s_cardlogDev.curSegId = i;
            s_cardlogDev.GroupId = groupId;
            s_cardlogDev.curSN = 0;

            s_cardlogDev.dataStartPos = segInfo.dataStartPos;
            s_cardlogDev.dataSize = 0;

            s_cardlogDev.curBufSectId = s_cardlogDev.curSegId * s_cardlogDev.segmentSize +s_cardlogDev.segmentStartPos+s_cardlogDev.dataStartPos;;
            s_cardlogDev.bufPos = 0;

            _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize);
            _CardLog_UpdateSeg_BasicInfo (s_cardlogDev.GroupId,s_cardlogDev.curSN,s_cardlogDev.dataStartPos);

            return TRUE;
        }
    }

    if (s_cardlogDev.segmentCount == i)
    {
        groupId++;
        s_cardlogDev.curSegId = 0;
        s_cardlogDev.GroupId = groupId;
        s_cardlogDev.curSN = 0;

        s_cardlogDev.dataSize = 0;

        s_cardlogDev.curBufSectId = s_cardlogDev.curSegId * s_cardlogDev.segmentSize +s_cardlogDev.segmentStartPos+s_cardlogDev.dataStartPos;;
        s_cardlogDev.bufPos = 0;

        _CardLog_UpdateSeg_datasize (s_cardlogDev.dataSize);
        _CardLog_UpdateSeg_BasicInfo (s_cardlogDev.GroupId,s_cardlogDev.curSN,s_cardlogDev.dataStartPos);

        return TRUE;
    }

    /**/
    return TRUE;


}

//===========================


LOCAL SCI_EVENT_GROUP_PTR s_cardlog_event = 0;
LOCAL const uint8 *s_cardlog_dataptr;
LOCAL uint32 s_cardlog_dataLen;
LOCAL UartCallback s_cardlog_callback;

typedef enum
{
    CARDLOG_RUNING_PRE  = 0,
    CARDLOG_RUNING          = 1,
    CARDLOG_DELETE_PRE  = 2,
    CARDLOG_DELETED     = 3
} CARDLOG_STATE_ENUM;
LOCAL CARDLOG_STATE_ENUM CARDLOG_ServiceFlag;

LOCAL  void CARDLOG_Service (uint32 argc,void *argv)
{
    uint32 requested_flags = 0x01;
    uint32 actual_flags_ptr;
    CARDLOG_STATE_ENUM state;

    for (;;)
    {
        SCI_DisableIRQ();
        state = CARDLOG_ServiceFlag;
        SCI_RestoreIRQ();

        if (CARDLOG_RUNING_PRE == state)
        {
            SCI_SLEEP (10);
        }
        else
        {
            break;
        }
    }

    for (;;)
    {
        SCI_DisableIRQ();
        state = CARDLOG_ServiceFlag;
        SCI_RestoreIRQ();

        if (CARDLOG_DELETE_PRE == state)
        {
            SCI_DisableIRQ();
            CARDLOG_ServiceFlag = CARDLOG_DELETED;
            SCI_RestoreIRQ();

            for (;;)
            {
                SCI_SLEEP (1000);
            }
        }

        if (SCI_SUCCESS == SCI_GetEvent (s_cardlog_event,requested_flags,SCI_OR_CLEAR,&actual_flags_ptr,2000))
        {
            _CardLog_SDOUT ( (uint8 *) s_cardlog_dataptr,s_cardlog_dataLen);
            s_cardlog_callback (EVENT_WRITE_COMPLETE);
        }
    }
}
LOCAL uint32 s_cardlog_taskid = SCI_INVALID_BLOCK_ID;
#define CARDLOG_TASK_STACK_SIZE 8096
#define CARDLOG_TASK_QUEUE_NUM  1
#define CARDLOG_TASK_PRIO           PRI_CARDLOG_TASK_PRIO  //30

LOCAL BOOLEAN _CARDLOG_CreateTask (void)
{
    if (SCI_INVALID_BLOCK_ID != s_cardlog_taskid)
    {
        return FALSE;
    }

    CARDLOG_ServiceFlag = CARDLOG_RUNING_PRE;
    s_cardlog_taskid = SCI_CreateThread (      // If successful, returns the thread ID
                           // of the new created thread,
                           // else returns SCI_INVALID_BLOCK_ID
                           "T_LOG",           // Name string of the thread
                           "Q_LOG",            // Queue name string of the thread
                           CARDLOG_Service,    // Entry function of the thread
                           0,                       // First parameter for entry function,
                           0,                      // Second parameter for entry function,
                           CARDLOG_TASK_STACK_SIZE,                 // Size of the thread stack in bytes
                           CARDLOG_TASK_QUEUE_NUM,          // Number of messages which can be enqueued
                           CARDLOG_TASK_PRIO,           // Prority of the thread.
                           SCI_PREEMPT,            // Indicates if the thread is preemptable.
                           SCI_AUTO_START          // Specifies whether the thread starts
                           // immediately or stays in a pure suspended
                           // state. Legal options are SCI_AUTO_START
                           // and SCI_DONT_START.
                       );

    if (SCI_INVALID_BLOCK_ID == s_cardlog_taskid)
    {
        return FALSE;
    }

    if (0 == s_cardlog_event)
    {
        s_cardlog_event = SCI_CreateEvent ("CARDLOG_EVENT");
    }

    SCI_DisableIRQ();
    CARDLOG_ServiceFlag = CARDLOG_RUNING;
    SCI_RestoreIRQ();

    return TRUE;
}

LOCAL BOOLEAN _CARDLOG_DeleteTask (void)
{
    BOOLEAN result = TRUE;
    CARDLOG_STATE_ENUM state;

    SCI_DisableIRQ();
    CARDLOG_ServiceFlag = CARDLOG_DELETE_PRE;
    SCI_RestoreIRQ();

    for (;;)
    {
        SCI_DisableIRQ();
        state = CARDLOG_ServiceFlag;
        SCI_RestoreIRQ();

        if (CARDLOG_DELETED != state)
        {
            SCI_SLEEP (10);
        }
        else
        {
            break;
        }
    }

    if (SCI_INVALID_BLOCK_ID != s_cardlog_taskid)
    {
        if (SCI_ERROR == SCI_DeleteThread (s_cardlog_taskid))
        {
            result = FALSE;
        }

        s_cardlog_taskid = SCI_INVALID_BLOCK_ID;
    }

    if (NULL != s_cardlog_event)
    {
        if (SCI_ERROR == SCI_DeleteEvent (s_cardlog_event))
        {
            result = FALSE;
        }

        s_cardlog_event = 0;
    }

    return result;
}


/*
    return  0 succses
            -1 fail
*/
PUBLIC  uint32 CARDLOG_Init (uint32 port_num, UART_INIT_PARA_T *config,UartCallback callback)
{
    CARDLOG_PAL_FUNC_T cardFunc = {0};

    cardFunc.cardLog_read = Cardlog_Pal_r;
    cardFunc.cardLog_write = Cardlog_Pal_w;
    cardFunc.cardLog_erase = Cardlog_Pal_e;
    cardFunc.cardLog_getCapacity = Cardlog_Pal_g;

    _CardLog_Regist (cardFunc);
    s_cardlog_callback = callback;

    if (TRUE != Cardlog_Pal_init())
    {
        return (uint32) (-1);
    }

    if (FALSE == _CardLog_DSK_Init())
    {
        if (TRUE != _CardLog_DSK_Format())
        {
            return (uint32) (-1);
        }

        if (TRUE != _CardLog_DSK_Init())
        {
            return (uint32) (-1);
        }
    }

    if (FALSE == _CARDLOG_CreateTask())
    {
        SCI_ASSERT (0);/*assert to do*/
        return (uint32) (-1);/*lint !e527 comfirmed by xuepeng*/
    }

    return 0;

}

/*
    return  0 succses
            -1 fail
*/
PUBLIC uint32 CARDLOG_Close (uint32 port_num)
{
    uint32 result = 0;

    if (FALSE == _CARDLOG_DeleteTask())
    {
        result = (uint32) (-1);
    }

    if (TRUE != _CardLog_FlushBuf())
    {
        result = (uint32) (-1);
    }

    //   _CardLog_UnReg();
    Cardlog_Pal_uninit();
    return result;
}

PUBLIC uint32 CARDLOG_GetFifoCnt (uint32 phy_port)
{
    return CARDLOG_BUFSIZE_INSECT;
}

PUBLIC uint32 CARDLOG_Read (uint32 port_num,uint8 *buffer,uint32 length)
{
    SCI_ASSERT (0);/*assert to do*/
    return 0; /*lint !e527 comfirmed by xuepeng*/
}

PUBLIC uint32  CARDLOG_Write (uint32 port_num,const uint8 *buffer,uint32 length)
{
    uint32 requested_flags = 0x01;

    s_cardlog_dataptr = buffer;
    s_cardlog_dataLen = length;

    SCI_SetEvent (s_cardlog_event,
                  requested_flags,
                  SCI_OR
                 );
    return length;
}

PUBLIC int32 CARDLOG_DumpGetChar (uint32 phy_port)
{
    SCI_ASSERT (0);/*assert to do*/
    return -1; /*lint !e527 comfirmed by xuepeng*/
}

PUBLIC void CARDLOG_DumpPutChar (uint32 phy_port, uint8 nchar)
{
    _CardLog_SDOUT (&nchar, 1);
}

PUBLIC void CARDLOG_Tx_Enable (uint32 phy_port,BOOLEAN is_enable)
{
    SCI_ASSERT (0);/*assert to do*/
}

PUBLIC void CARDLOG_Rx_Enable (uint32 phy_port,BOOLEAN is_enable)
{
    SCI_ASSERT (0);/*assert to do*/
}

PUBLIC void CARDLOG_Flush (uint32 phy_port)
{
    _CardLog_FlushBuf();
}
LOCAL const sio_operation_st	ucom_operation = {
	CARDLOG_Init,        
       CARDLOG_Close,
       CARDLOG_GetFifoCnt,
       CARDLOG_Read,
       CARDLOG_Write,
       CARDLOG_DumpGetChar,
       CARDLOG_DumpPutChar,
       CARDLOG_Tx_Enable,
       CARDLOG_Rx_Enable,
       CARDLOG_Flush,	
       NULL,  /*purchars*/
       NULL,
};
PUBLIC const sio_operation_st *CARDLOG_get_operation(void)
{
	return &ucom_operation;
}
#endif

