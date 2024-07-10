#ifndef __WE_MSG_H__
#define __WE_MSG_H__

#include "we_def.h"

typedef struct IWEMsg         IWEMsg;
typedef struct IWEMsgValue    IWEMsgValue;

typedef WE_INT32    WEMsgOptId;
typedef WE_INT32    WEMsgOptType;
typedef WE_VOID     WEMsgOptValue;
typedef WE_INT32    WEMsgOptSize;

/*interface*/
#define WE_VTBL(name)                           name##Vtbl
#define WE_GETVTBL(p, name)                     (*(WE_VTBL(name)**)((WE_VOID*)p))
#define WE_INTERFACE(name) \
   typedef struct WE_VTBL(name) WE_VTBL(name); \
struct WE_VTBL(name)

#define WE_BASEFUN(name) \
   WE_UINT32 (*AddRef) (name *p); \
WE_UINT32 (*Release)(name *p)

/*break check*/
#define WE_BREAKIF(ret, des)                    if(SUCCESS!=ret){if(NULL!=des){EME_UTILS_LOG_ERROR(("Error: %s",des));}break;}
#define WE_BREAKNULL(p,ret,err,des)             if(NULL==p){ret=err;if(NULL!=des){EME_UTILS_LOG_ERROR(("Error: %s",des));}break;}

#define WE_SUCCESS            (0)
#define WE_FAILED             (1)
#define WE_BADPARM            (2)
#define WE_NOMEMORY           (3)
#define WE_UNSUPPORT          (4)
#define WE_BADCLASS           (5)
#define WE_BADFILE            (6)
#define WE_NOTFOUND           (7)

typedef struct WEMsgOpt WEMsgOpt;
struct WEMsgOpt
{
   WEMsgOptId     iId;
   WEMsgOptType   uiType;
   WEMsgOptSize   uiSize;
   WEMsgOptValue  *pVal;
};

#define WEMSG_OPT_RESERVED          0x00000000
#define WEMSG_OPT_RESERVED_LAST     0x0000ffff

#define WEMSG_OPT_END               0x00000000
#define WEMSG_OPT_ANY               0x00000001
#define WEMSG_OPT_REMOVED           0x00000002

#define WEMSG_OPT_SZ                0x00010000
#define WEMSG_OPT_SZ_LAST           0x0001ffff

#define WEMSG_OPT_WSZ               0x00020000
#define WEMSG_OPT_WSZ_LAST          0x0002ffff

#define WEMSG_OPT_VARBUFFER         0x00030000
#define WEMSG_OPT_VARBUFFER_LAST    0x0003ffff

#define WEMSG_OPT_BINARY            0x00040000
#define WEMSG_OPT_BINARY_LAST       0x0004ffff

#define WEMSG_OPT_32BIT             0x00050000
#define WEMSG_OPT_32BIT_LAST        0x0005ffff

#define WEMSG_END_POSTION           (-1)

WE_INTERFACE(IWEMsg)
{
   WE_BASEFUN(IWEMsg);
   WE_INT (*DelHdrValue)   (IWEMsg *p);
   WE_INT (*AddHdrValue)   (IWEMsg *p, IWEMsgValue *pIHeader);
   WE_INT (*GetHdrValue)   (IWEMsg *p, IWEMsgValue **ppIHeader);
   WE_INT (*AddPartValue)  (IWEMsg *p, IWEMsgValue *pIPart,   WE_INT32 *pnPos);
   WE_INT (*DelPartValue)  (IWEMsg *p, WE_INT32 nPos);
   WE_INT (*GetPartValue)  (IWEMsg *p, IWEMsgValue **ppIPart, WE_INT32 nPos);
   WE_INT (*GetPartCount)  (IWEMsg *p, WE_UINT32 *pcount);

   WE_INT (*EncodeMsg)     (IWEMsg *p, WE_UCHAR **ppBuf, WE_UINT32 *puiBufSize);
   WE_INT (*DecodeMsg)     (IWEMsg *p, WE_UCHAR *pBuf, WE_UINT32 uiBufSize);
   WE_UINT32 (*GetCrc)     (IWEMsg *p);
};

extern WE_INT wemsg_new(IWEMsg **ppObj);
#define IWEMSG_New(pp)                    wemsg_new(pp)
#define IWEMSG_AddRef(p)                  WE_GETVTBL(p,IWEMsg)->AddRef(p)
#define IWEMSG_Release(p)                 WE_GETVTBL(p,IWEMsg)->Release(p)
#define IWEMSG_AddHdrValue(p,pi)          WE_GETVTBL(p,IWEMsg)->AddHdrValue(p,pi)
#define IWEMSG_RemoveHdrValue(p)          WE_GETVTBL(p,IWEMsg)->DelHdrValue(p)
#define IWEMSG_GetHdrValue(p,ppi)         WE_GETVTBL(p,IWEMsg)->GetHdrValue(p,ppi)
#define IWEMSG_AddPartValue(p,pi,ppos)    WE_GETVTBL(p,IWEMsg)->AddPartValue(p,pi,(ppos))
#define IWEMSG_RemovePartValue(p,pos)     WE_GETVTBL(p,IWEMsg)->DelPartValue(p,(pos))
#define IWEMSG_GetPartValue(p,ppi,pos)    WE_GETVTBL(p,IWEMsg)->GetPartValue(p,ppi,(pos))
#define IWEMSG_GetPartCount(p,pcount)     WE_GETVTBL(p,IWEMsg)->GetPartCount(p,(pcount))
#define IWEMSG_EncodeMsg(p,pbuff,psize)   WE_GETVTBL(p,IWEMsg)->EncodeMsg(p,pbuff,psize)
#define IWEMSG_DecodeMsg(p,buff,size)     WE_GETVTBL(p,IWEMsg)->DecodeMsg(p,buff,size)
#define IWEMSG_GetCrc(p)                  WE_GETVTBL(p,IWEMsg)->GetCrc(p)


WE_INTERFACE(IWEMsgValue)
{
   WE_BASEFUN(IWEMsgValue);
   WE_INT (*AddOpt)   (IWEMsgValue *p, WEMsgOpt *pstOpts);
   WE_INT (*GetOpt)   (IWEMsgValue *p, WEMsgOptId iOptId, WE_INT32 nPos, WEMsgOpt *pstOpt);
   WE_INT (*RemoveOpt)(IWEMsgValue *p, WEMsgOptId iOptId, WE_INT32 nPos);
   WE_INT (*UpdateOpt)(IWEMsgValue *p, WE_INT32 nPos, WEMsgOpt *pstOpt);
   WE_INT (*GetOptNum)(IWEMsgValue *p, WE_UINT32 *puiNum);
   WE_INT (*GetIWEMsg)(IWEMsgValue *p, IWEMsg **ppIMsg);
};

extern WE_INT wemsgvalue_new(IWEMsgValue **ppObj);
#define IWEMSGVALUE_New(pp)                             wemsgvalue_new(pp)
#define IWEMSGVALUE_AddRef(p)                           WE_GETVTBL(p,IWEMsgValue)->AddRef(p)
#define IWEMSGVALUE_Release(p)                          WE_GETVTBL(p,IWEMsgValue)->Release(p)
#define IWEMSGVALUE_AddOpt(p,opt)                       WE_GETVTBL(p,IWEMsgValue)->AddOpt(p,(opt))
#define IWEMSGVALUE_GetOpt(p,id,opt)                    WE_GETVTBL(p,IWEMsgValue)->GetOpt(p,(id),(0),(opt))
#define IWEMSGVALUE_GetOptByIndex(p,id,index,opt)       WE_GETVTBL(p,IWEMsgValue)->GetOpt(p,(id),(index),(opt))
#define IWEMSGVALUE_RemoveOpt(p,id,index)               WE_GETVTBL(p,IWEMsgValue)->RemoveOpt(p,(id),(index))
#define IWEMSGVALUE_UpdateOptByIndex(p,index,opt)       WE_GETVTBL(p,IWEMsgValue)->UpdateOpt(p,(index),(opt))
#define IWEMSGVALUE_UpdateOpt(p,opt)                    WE_GETVTBL(p,IWEMsgValue)->UpdateOpt(p,(0),(opt))
#define IWEMSGVALUE_GetOptNum(p,pnum)                   WE_GETVTBL(p,IWEMsgValue)->GetOptNum(p,(pnum))
#define IWEMSGVALUE_GetIWEMsg(p,ppmsg)                  WE_GETVTBL(p,IWEMsgValue)->GetIWEMsg(p,(ppmsg))

#endif