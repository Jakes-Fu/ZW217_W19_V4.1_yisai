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
//#define MAX_LAYER_NUM		UILAYER_TOTAL_BLT_COUNT //֧��������
#define MAIN_LAYER	GUI_BLOCK_0
typedef struct _LAYERLINK
{
	//uint8    isVisible;		//���ӱ�־,ֻ���ڿ�������²��ܽ��кϲ�
	uint32   dwflag;   //ģʽ
	union {
		
		uint8  alpha; //���ֵ
		uint32 keycolor;//͸����ɫ
	}ak;
	//uint32    bgcolor;
	GUI_LCD_DEV_INFO hlayer;
	//GUI_RECT_T		 bltrc;
	struct _LAYERLINK *next;	//��һ��
	
}LayerLink,*PLayerLink;
GUI_LCD_DEV_INFO main_hlayer = {GUI_LCD_0, GUI_BLOCK_0};
PLayerLink  affix_hlayer=NULL; 
/*=============================================================
 �������� : AddLayerLink
 ��    �� : �Ѳ���ӵ�����
 ��    �� :  uint32 lcdid[IN] --- lcd ����
			 uint32 blockid[IN] --- ������
 ��   ��  : 1  �ɹ�
            0  ʧ��
   
=============================================================*/
LOCAL int AddLayerLink(uint32 lcdid,uint32 blockid)
{
	struct _LAYERLINK *p1,*p2=NULL,*pnew;

	if(blockid == MAIN_LAYER || lcdid != GUI_LCD_0)
	{
		return 0;
	}
	
	//����ڵ�ռ䣬�洢����
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
 �������� : RemoveLayerLink
 ��    �� : �Ѳ�������Ƴ�
 ��    �� :  uint32 lcdid[IN] --- lcd ����
			 uint32 blockid[IN] --- ������
 ��   ��  : 1  �ɹ�
            0  ʧ��
   
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

	while (p1!= NULL) /*p1ָ��Ľڵ㲻����Ҫ���ҵģ��������������һ���ڵ㣬�ͼ���������*/
	{
		if(p1->hlayer.block_id == blockid && p1->hlayer.lcd_id == lcdid)
		{
			nFlag=1;
			break;
		}

		p2 = p1; /*���浱ǰ�ڵ�ĵ�ַ*/
		p1 = p1->next; /*����һ���ڵ�*/
	} 
	
	if (nFlag && p1) /*�ҵ���*/
	{
		if (p1 == affix_hlayer) /*���Ҫɾ���Ľڵ��ǵ�һ���ڵ�*/
		{
			affix_hlayer = p1->next; /*ͷָ��ָ���һ���ڵ�ĺ�һ���ڵ㣬Ҳ���ǵڶ����ڵ㡣������һ���ڵ�Ͳ��������У���ɾ����*/   
		}
		else /*����������ڵ㣬����ԭ��ָ��ǰ�ڵ��ָ�룬ָ��������һ���ڵ㣬���ɾ��*/
		{
			if(p2)	// changed by yifei
				p2->next = p1->next;/*lint !e716*/
		}
		
		SCI_FREE(p1); /*�ͷŵ�ǰ�ڵ�*/
		p1 = NULL;

		return 1;
	}

	return 0;
}
/*=============================================================
 �������� : GetLayerLinkNode
 ��    �� : ��ȡָ�������нڵ�
 ��    �� :  uint32 lcdid[IN] --- lcd ����
			 uint32 blockid[IN] --- ������
 ��   ��  :  !NULL �ɹ�
             NULL  ʧ��
   
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
 �������� : GetLayer
 ��    �� : ��ȡָ����Ļ�������ַ
 ��    �� :  int nLayer[IN] --- ������
 
 ��   ��  : !NULL : �ɹ�
		     NULL : ʧ��
   
=============================================================*/
unsigned short* GetLayer(int nLayer)
{
    unsigned short   *lcdbuf = NULL;
	struct _LAYERLINK *pll=NULL;

	if(!UILAYER_IsMultiLayerEnable())
	{
		return (unsigned short*)GUIBLOCK_GetBuf(&main_hlayer);
	}

	//������Զ�����ͷ�
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
 �������� : CreateLayer
 ��    �� : ������,���ز�id
 ��    �� : int nx [IN] --- ��������
            int ny [IN] --- ��������
			int nw [IN] --- ���
            int ny [IN] --- ���
 ��   ��  :  !=0xffff : �ɹ�
		     0xffff : ʧ��
   
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
		//��Ӳ㵽��
		if(!AddLayerLink(hlayer.lcd_id,hlayer.block_id))
		{
			UILAYER_ReleaseLayer(&hlayer);
			return UILAYER_NULL_HANDLE;
		}
		//blt
        append_layer.lcd_dev_info = hlayer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
		//����
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
 �������� : DestoryLayer
 ��    �� : �ݻ�ָ����
 ��    �� :  int nLayer[IN] --- ������
 
 ��   ��  : 1 : �ɹ�
            0 : ʧ��
   
=============================================================*/
int DestoryLayer(int nLayer)
{
	struct _LAYERLINK *pll=NULL;
    GUI_LCD_DEV_INFO hlayer={0x00};
	if(!UILAYER_IsMultiLayerEnable())
	{
		return 0;
	}

	//����,��ֹ�ͷ�
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
 �������� : DestoryAllLayers
 ��    �� : �ݻ����в�
 ��    �� : void
 
 ��   ��  : void
   
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
 �������� : SetLayerAttributes
 ��    �� : ���ò�����
 ��    �� : int nLayer[IN]   --- ������
            uint32 crKey[IN] --- ͸����ɫֵ
            uint8 bAlpha[IN] --- ��ɫ͸����
            uint32 dwFlags[IN] --- ���ͱ�־
��   ��  : 1 : �ɹ�
           0 : ʧ��
   
=============================================================*/
int SetLayerAttributes(int nLayer,unsigned int crKey,unsigned char bAlpha,unsigned int dwFlags)
{
	GUI_LCD_DEV_INFO  hlayer={0};
	struct _LAYERLINK *pll=NULL;

	if(!UILAYER_IsMultiLayerEnable())
	{
		return 0;
	}

	//����
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
	//���
	UILAYER_Clear(&hlayer);

	return 1;
}
/*=============================================================
 �������� : UniteLayer
 ��    �� : �ϲ���
 ��    �� : int nDstLayer[IN] --- Ŀ�Ĳ�����
			int dx,dy[IN]     --- Ŀ�Ĳ����Ͻ�����
			int w,h[IN]       --- ����
            int nSrcLayer[IN] --- Դ������
            int sx,sy[IN]     --- Դ�����Ͻ�����
			uint8 isBlend[IN] --- ���/���Ǳ�־
��   ��  : 1 : �ɹ�
           0 : ʧ��
   
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


	//������ͬһ��
	if(nDstLayer == nSrcLayer || nSrcLayer == MAIN_LAYER)
	{
		return 0;
	}
	//Ŀ�Ĳ�
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
	//Դ��
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

	//����
	blt_rect.left = dx;
	blt_rect.top = dy;
	blt_rect.right = dx+w;
	blt_rect.bottom = dy+h;
	if (UILAYER_IsLayerActive(&hDstlayer)&&UILAYER_IsLayerActive(&hSrclayer))
	{
#if 0
		//�ƶ����λ��
		//UILAYER_SetLayerPosition(&hDstlayer, blt_rect.left, blt_rect.top);
		UILAYER_SetLayerPosition(&hSrclayer, blt_rect.left, blt_rect.top);
		//�ϲ�
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
 �������� : SetLayerVisible
 ��    �� : ���ò���ʾ/����,�����ϵı�־
 ��    �� : int nLayer[IN]          --- ������
			uint8 isVisible[IN]     --- ��ʾ��־
			
 ��   ��  : 1 - ����
            0 - ������
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
	//���� ʼ���ǿ��ӵ�
	if(nLayer == MAIN_LAYER)
	{
		return 1;
	}
	//������������
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
 �������� : UniteAllLayers
 ��    �� : ׷�����в㵽����
 ��    �� : void
��   ��  : void
   
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
 �������� : IsSupportLayer
 ��    �� : ��֧��У��
 ��    �� : void
��   ��  : 1 - ֧��
           0 - ��֧��
=============================================================*/
int IsSupportLayer(void)
{
	BOOLEAN bRet;
	bRet =UILAYER_IsMultiLayerEnable();

	return (bRet)?1:0;
}
/*=============================================================
 �������� : MoveLayer
 ��    �� : �Ѳ��ƶ���ָ����ĺ���
 ��    �� : int nLayer[IN] --- ���ƶ�������
			int nLayerAfter[IN]     --- ָ��������
��   ��  : 1 - �ɹ�
           0 - ���ɹ�
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
