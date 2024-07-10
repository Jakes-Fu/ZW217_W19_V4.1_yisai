#ifdef WRE_SUPPORT
/******************************************************************************
 ** File Name:      adaptor_layer.c                                          *
 ** Author:         RiancyZhang                                              *
 ** DATE:           03/21/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file wraps multi-layer for mocor os				  *
 **																              *
 ******************************************************************************/
#include "mmk_type.h"
#include "mmi_module.h"
#include "guiblock.h"
#include "ui_layer.h"
#include "mmiwre_other.h"
#include "mmiwre_gdi.h"
#include "mmiwre_layer.h"
////////////////////////////////////////////////////////////////////////////////
#define LWA_COLORKEY	0x00000001
#define LWA_ALPHA		0x00000002
//#define MAX_LAYER_NUM		UILAYER_TOTAL_BLT_COUNT //支持最大层数
#define MAIN_LAYER	GUI_BLOCK_0
typedef struct _LAYERLINK
{
	//uint8    isVisible;		//可视标志,只有在可视情况下才能进行合并
	uint32   dwflag;   //模式
	union {
		
		uint8  alpha; //混合值
		uint32 keycolor;//透明颜色
	}ak;
	//uint32    bgcolor;
	GUI_LCD_DEV_INFO hlayer;
	//GUI_RECT_T		 bltrc;
	struct _LAYERLINK *next;	//下一个
	
}LayerLink,*PLayerLink;
GUI_LCD_DEV_INFO main_hlayer = {GUI_LCD_0, GUI_BLOCK_0};
PLayerLink  affix_hlayer=NULL; 
/*=============================================================
 函数名称 : AddLayerLink
 功    能 : 把层添加到链中
 参    数 :  uint32 lcdid[IN] --- lcd 索引
			 uint32 blockid[IN] --- 层索引
 返   回  : 1  成功
            0  失败
   
=============================================================*/
LOCAL int AddLayerLink(uint32 lcdid,uint32 blockid)
{
	struct _LAYERLINK *p1,*p2=NULL,*pnew;

	if(blockid == MAIN_LAYER || lcdid != GUI_LCD_0)
	{
		return 0;
	}
	
	//分配节点空间，存储数据
	pnew=(struct _LAYERLINK*)SCI_ALLOCA(sizeof(struct _LAYERLINK));
	if(pnew == NULL) return 0;
	memset(pnew,0x00,sizeof(struct _LAYERLINK));
	pnew->next=NULL;
	//pnew->isVisible = TRUE;
	pnew->hlayer.lcd_id=lcdid;
	pnew->hlayer.block_id = blockid;
	
	p1=affix_hlayer;
	if(!affix_hlayer) 
	{
		affix_hlayer=pnew;
		return 1;
	}
	while(p1 != NULL)
	{
		/*
		if(p1->hlayer.block_id == blockid)
		{
			free(pnew);
			return;
		}
		*/
		p2=p1;
		p1=p1->next;
	}

	if(p2)	// changed by yifei
		p2->next=pnew;
	
	return 1;
}
/*=============================================================
 函数名称 : RemoveLayerLink
 功    能 : 把层从链中移除
 参    数 :  uint32 lcdid[IN] --- lcd 索引
			 uint32 blockid[IN] --- 层索引
 返   回  : 1  成功
            0  失败
   
=============================================================*/
LOCAL  int RemoveLayerLink(uint32 lcdid,uint32 blockid)
{
	struct _LAYERLINK *p1,*p2=NULL;
    int nFlag=0;

	p1 = affix_hlayer;
	if(!p1 || blockid == MAIN_LAYER || lcdid != GUI_LCD_0) 
	{
		return 0;
	}

	while (p1!= NULL) /*p1指向的节点不是所要查找的，并且它不是最后一个节点，就继续往下找*/
	{
		if(p1->hlayer.block_id == blockid && p1->hlayer.lcd_id == lcdid)
		{
			nFlag=1;
			break;
		}

		p2 = p1; /*保存当前节点的地址*/
		p1 = p1->next; /*后移一个节点*/
	} 
	
	if (nFlag && p1) /*找到了*/
	{
		if (p1 == affix_hlayer) /*如果要删除的节点是第一个节点*/
		{
			affix_hlayer = p1->next; /*头指针指向第一个节点的后一个节点，也就是第二个节点。这样第一个节点就不在链表中，即删除。*/   
		}
		else /*如果是其它节点，则让原来指向当前节点的指针，指向它的下一个节点，完成删除*/
		{
			if(p2)	// changed by yifei
				p2->next = p1->next;/*lint !e716*/
		}
		
		SCI_FREE(p1); /*释放当前节点*/
		p1 = NULL;

		return 1;
	}

	return 0;
}
/*=============================================================
 函数名称 : GetLayerLinkNode
 功    能 : 获取指定层链中节点
 参    数 :  uint32 lcdid[IN] --- lcd 索引
			 uint32 blockid[IN] --- 层索引
 返   回  :  !NULL 成功
             NULL  失败
   
=============================================================*/
struct _LAYERLINK *GetLayerLinkNode(uint32 lcdid,uint32 blockid)
{
	struct _LAYERLINK *p,*pal;
	int nFlag=0;

	if(blockid == MAIN_LAYER || lcdid != GUI_LCD_0) return NULL;
	
	pal = affix_hlayer;
	//
	while(pal)
	{
		if(pal->hlayer.block_id == blockid && pal->hlayer.lcd_id == lcdid)
		{
			nFlag=1;
			break;
		}
		p=pal->next;
		pal=p;
	}
	
	return (nFlag)?pal:NULL;
}
////////////////////////////////////////////////////////////////////////////////
/*=============================================================
 函数名称 : GetLayer
 功    能 : 获取指定层的缓冲区地址
 参    数 :  int nLayer[IN] --- 层索引
 
 返   回  : !NULL : 成功
		     NULL : 失败
   
=============================================================*/
unsigned short* GetLayer(int nLayer)
{
    unsigned short   *lcdbuf = NULL;
	struct _LAYERLINK *pll=NULL;

	if(!UILAYER_IsMultiLayerEnable())
	{
		return (unsigned short*)GUIBLOCK_GetBuf(&main_hlayer);
	}

	//主层永远不会释放
	if(nLayer == MAIN_LAYER)
	{
		if (UILAYER_IsLayerActive(&main_hlayer))
		{
			//lcdbuf = (unsigned short*)GUIBLOCK_GetBuf(&main_hlayer);
			lcdbuf = (unsigned short*)UILAYER_GetLayerBufferPtr(&main_hlayer);
		}
	}
	else
	{
		pll = GetLayerLinkNode(GUI_LCD_0,nLayer);
		if(pll && UILAYER_IsLayerActive(&pll->hlayer))
		{
			//lcdbuf = (unsigned short*)GUIBLOCK_GetBuf(&pll->hlayer);
			lcdbuf = (unsigned short*)UILAYER_GetLayerBufferPtr(&pll->hlayer);
		}

	}
    return lcdbuf;
}
/*=============================================================
 函数名称 : CreateLayer
 功    能 : 创建层,返回层id
 参    数 : int nx [IN] --- 横向坐标
            int ny [IN] --- 纵向坐标
			int nw [IN] --- 层宽
            int ny [IN] --- 层高
 返   回  :  !=0xffff : 成功
		     0xffff : 失败
   
=============================================================*/
int  CreateLayer(int nx,int ny,int nw,int nh)
{
	/*
	struct _LAYERLINK *pll=NULL;
	*/
	unsigned short w,h;
	UILAYER_RESULT_E uret;
	UILAYER_CREATE_T uil={0};
	GUI_LCD_DEV_INFO  hlayer={GUI_LCD_0,0};
	UILAYER_APPEND_BLT_T    append_layer = {0};
	if(!UILAYER_IsMultiLayerEnable())
	{
		return GUI_BLOCK_0;
	}

	GetLcdWHMocor(GUI_LCD_0,&w,&h);
	uil.lcd_id=GUI_MAIN_LCD_ID;
	uil.owner_handle=GetMwinMocorWinHandle();
	uil.offset_x= (nx < 0 || nx > w)?0:nx;
	uil.offset_y= (ny < 0 || ny > h)?0:ny;
	uil.width   = (nw <= 0 || nw > w)?w:nw;
	uil.height  = (nh <= 0 || nh > h)?h:nh;
    uil.format = UILAYER_MEM_IMMUTABLE;
	uil.is_bg_layer=TRUE;
	uil.is_static_layer=FALSE;

	uret=UILAYER_CreateLayer(&uil,&hlayer);
	if(UILAYER_RESULT_SUCCESS == uret)
	{
		//添加层到链
		if(!AddLayerLink(hlayer.lcd_id,hlayer.block_id))
		{
			UILAYER_ReleaseLayer(&hlayer);
			return UILAYER_NULL_HANDLE;
		}
		//blt
        append_layer.lcd_dev_info = hlayer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
		//清屏
		UILAYER_SetLayerColorKey(&hlayer, TRUE, 0x8080);
		
		if (UILAYER_IsLayerActive(&hlayer))
		{
			UILAYER_Clear(&hlayer);
		}
		/*
		pll=GetLayerLinkNode(hlayer.lcd_id,hlayer.block_id);
		pll->bltrc.top = uil.offset_y;
		pll->bltrc.left = uil.offset_x;
		pll->bltrc.right = uil.offset_x+uil.width;
		pll->bltrc.bottom = uil.offset_y+uil.height;
		*/
	}
	return hlayer.block_id;
}
/*=============================================================
 函数名称 : DestoryLayer
 功    能 : 摧毁指定层
 参    数 :  int nLayer[IN] --- 层索引
 
 返   回  : 1 : 成功
            0 : 失败
   
=============================================================*/
int DestoryLayer(int nLayer)
{
	struct _LAYERLINK *pll=NULL;
    GUI_LCD_DEV_INFO hlayer={0x00};
	if(!UILAYER_IsMultiLayerEnable())
	{
		return 0;
	}

	//主层,禁止释放
	if(nLayer == MAIN_LAYER || nLayer == 0xFFFF)
	{
		return 1;
	}

	pll = GetLayerLinkNode(GUI_LCD_0,nLayer);
	if(pll)
	{
        hlayer.block_id=pll->hlayer.block_id;
        hlayer.lcd_id=pll->hlayer.lcd_id;
		if(RemoveLayerLink(GUI_LCD_0,nLayer))
		{
			UILAYER_ReleaseLayer(&hlayer);
		}
		
	}

	return 1;
}
/*=============================================================
 函数名称 : DestoryAllLayers
 功    能 : 摧毁所有层
 参    数 : void
 
 返   回  : void
   
=============================================================*/
void DestoryAllLayers(void)
{
	struct _LAYERLINK *pll;
	
	if(!UILAYER_IsMultiLayerEnable())
	{
		return;
	}

	pll = affix_hlayer;
	//
	while(pll)
	{
		UILAYER_ReleaseLayer(&pll->hlayer);
		RemoveLayerLink(GUI_LCD_0,pll->hlayer.block_id);
		pll=pll->next;
	}

	affix_hlayer=NULL;
}
/*=============================================================
 函数名称 : SetLayerAttributes
 功    能 : 设置层属性
 参    数 : int nLayer[IN]   --- 层索引
            uint32 crKey[IN] --- 透明颜色值
            uint8 bAlpha[IN] --- 颜色透明度
            uint32 dwFlags[IN] --- 类型标志
返   回  : 1 : 成功
           0 : 失败
   
=============================================================*/
int SetLayerAttributes(int nLayer,unsigned int crKey,unsigned char bAlpha,unsigned int dwFlags)
{
	GUI_LCD_DEV_INFO  hlayer={0};
	struct _LAYERLINK *pll=NULL;

	if(!UILAYER_IsMultiLayerEnable())
	{
		return 0;
	}

	//主层
	if(nLayer == MAIN_LAYER)
	{
		return 0;
	}

	pll = GetLayerLinkNode(GUI_LCD_0,nLayer);
	if(pll)
	{
		hlayer.lcd_id = pll->hlayer.lcd_id;
		hlayer.block_id=pll->hlayer.block_id;
		
	}
	else
	{
		return 0;
	}
	
	if(dwFlags == LWA_COLORKEY)
	{
		UILAYER_SetLayerColorKey(&hlayer,TRUE,crKey);
		if (UILAYER_IsLayerActive(&hlayer))
		{
			UILAYER_Clear(&hlayer);
		}
		pll->ak.keycolor=crKey;
	}
	else if(dwFlags == LWA_ALPHA)
	{
		UILAYER_SetLayerAlpha(&hlayer,bAlpha);
		pll->ak.alpha=bAlpha;
	}
	else 
	{
		return 0;
	}

	pll->dwflag = dwFlags;
	//清层
	UILAYER_Clear(&hlayer);

	return 1;
}
/*=============================================================
 函数名称 : UniteLayer
 功    能 : 合并层
 参    数 : int nDstLayer[IN] --- 目的层索引
			int dx,dy[IN]     --- 目的层左上角坐标
			int w,h[IN]       --- 层宽高
            int nSrcLayer[IN] --- 源层索引
            int sx,sy[IN]     --- 源层左上角坐标
			uint8 isBlend[IN] --- 混合/覆盖标志
返   回  : 1 : 成功
           0 : 失败
   
=============================================================*/
int UniteLayer( int nDstLayer,int dx,int dy,int w,int h,int nSrcLayer,int sx,int sy,unsigned char isBlend)
{
	/*
	GUI_RECT_T   blt_rect = {0};
	GUI_LCD_DEV_INFO  hDstlayer={0},hSrclayer={0};
	struct _LAYERLINK *pll=NULL;

	if(!UILAYER_IsMultiLayerEnable())
	{
		return 0;
	}


	//不能是同一层
	if(nDstLayer == nSrcLayer || nSrcLayer == MAIN_LAYER)
	{
		return 0;
	}
	//目的层
	if(nDstLayer == MAIN_LAYER)
	{
		hDstlayer.lcd_id = main_hlayer.lcd_id;
		hDstlayer.block_id=main_hlayer.block_id;
	}
	else
	{
		pll = GetLayerLinkNode(GUI_LCD_0,nDstLayer);
		if(pll)
		{
			hDstlayer.lcd_id = pll->hlayer.lcd_id;
			hDstlayer.block_id=pll->hlayer.block_id;
			
		}
		else
		{
			return 0;
		}
	}
	//源层
	pll = GetLayerLinkNode(GUI_LCD_0,nSrcLayer);
	if(pll)
	{
		hSrclayer.lcd_id = pll->hlayer.lcd_id;
		hSrclayer.block_id=pll->hlayer.block_id;
		
	}
	else
	{
		return 0;
	}

	//区域
	blt_rect.left = dx;
	blt_rect.top = dy;
	blt_rect.right = dx+w;
	blt_rect.bottom = dy+h;
	if (UILAYER_IsLayerActive(&hDstlayer)&&UILAYER_IsLayerActive(&hSrclayer))
	{
#if 0
		//移动层的位置
		//UILAYER_SetLayerPosition(&hDstlayer, blt_rect.left, blt_rect.top);
		UILAYER_SetLayerPosition(&hSrclayer, blt_rect.left, blt_rect.top);
		//合并
		UILAYER_BltLayerToLayer(&hDstlayer,&hSrclayer,&blt_rect,isBlend); 
#endif
#ifdef WIN32
		AlphaBlendLayer(MAIN_LAYER,dx,dy,w,h,nSrcLayer,dx,dy,w,h,pll);
#endif
	}
	*/
	return 1;
}
/*=============================================================
 函数名称 : SetLayerVisible
 功    能 : 设置层显示/隐藏,返回老的标志
 参    数 : int nLayer[IN]          --- 层索引
			uint8 isVisible[IN]     --- 显示标志
			
 返   回  : 1 - 可视
            0 - 不可视
=============================================================*/
int SetLayerVisible(int nLayer,unsigned char isVisible)
{
	uint8 oldflag=1;
#if 0

	GUI_LCD_DEV_INFO  hlayer={0};
	struct _LAYERLINK *pll=NULL;


	if(!UILAYER_IsMultiLayerEnable())
	{
		return 0;
	}
	//主层 始终是可视的
	if(nLayer == MAIN_LAYER)
	{
		return 1;
	}
	//其它可以设置
	pll = GetLayerLinkNode(GUI_LCD_0,nLayer);
	if(!pll)
	{
		return 1;
	}
	oldflag = pll->isVisible;
	pll->isVisible = isVisible;
#endif
	return oldflag;
}
/*=============================================================
 函数名称 : UniteAllLayers
 功    能 : 追加所有层到序列
 参    数 : void
返   回  : void
   
=============================================================*/
void UniteAllLayers(void)
{
	struct _LAYERLINK *pll=affix_hlayer;

	UILAYER_APPEND_BLT_T    append_layer = {0};

	if(!UILAYER_IsMultiLayerEnable())
	{
		return;
	}
	while(pll)
	{
        append_layer.lcd_dev_info = pll->hlayer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

		if (UILAYER_IsLayerActive(&pll->hlayer))
		{
			UILAYER_Clear(&pll->hlayer);
		}


		pll=pll->next;
	}
}
/*=============================================================
 函数名称 : IsSupportLayer
 功    能 : 层支持校验
 参    数 : void
返   回  : 1 - 支持
           0 - 不支持
=============================================================*/
int IsSupportLayer(void)
{
	BOOLEAN bRet;
	bRet =UILAYER_IsMultiLayerEnable();

	return (bRet)?1:0;
}
/*=============================================================
 函数名称 : MoveLayer
 功    能 : 把层移动到指定层的后面
 参    数 : int nLayer[IN] --- 待移动层索引
			int nLayerAfter[IN]     --- 指定层索引
返   回  : 1 - 成功
           0 - 不成功
=============================================================*/
int MoveLayer(int nLayer, int nLayerAfter)
{
	struct _LAYERLINK *pll=affix_hlayer;
	struct _LAYERLINK *psr=NULL,*paf=NULL;
	struct _LAYERLINK *psrp=NULL,*pal=NULL;

	while(pll)
	{
        if(pll->hlayer.block_id == (uint32)nLayer && pll->hlayer.lcd_id == GUI_LCD_0)
		{
			psrp=pal;
			psr=pll;
		}
		else if(pll->hlayer.block_id == (uint32)nLayerAfter && pll->hlayer.lcd_id == GUI_LCD_0)
		{
			paf=pll;
		}
		
        if(psr && paf && psrp)
		{
			UILAYER_MoveLayer(&psr->hlayer,&paf->hlayer);

            psrp->next=psr->next;
			psr->next=paf->next;
			paf->next=psr;

			return 1;
		}

		pal=pll;
		pll=pll->next;
	}

	return 0;
}

#endif
