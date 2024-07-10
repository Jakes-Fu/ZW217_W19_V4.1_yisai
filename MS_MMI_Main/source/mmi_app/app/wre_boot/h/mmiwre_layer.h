#ifndef _MMIWRE_LAYER_H_
#define _MMIWRE_LAYER_H_

//////////////////////////////////////

///////////////////////////////////
/*=============================================================
 函数名称 : GetLayer
 功    能 : 获取指定层的缓冲区地址
 参    数 :  int nLayer[IN] --- 层索引
 
 返   回  : !NULL : 成功
		     NULL : 失败
   
=============================================================*/
unsigned short* GetLayer(int nLayer);
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
int  CreateLayer(int nx,int ny,int nw,int nh);
/*=============================================================
 函数名称 : DestoryLayer
 功    能 : 摧毁指定层
 参    数 :  int nLayer[IN] --- 层索引
 
 返   回  : 1 : 成功
            0 : 失败
   
=============================================================*/
int DestoryLayer(int nLayer);
/*=============================================================
 函数名称 : DestoryAllLayer
 功    能 : 摧毁指定层
 参    数 : void
 
 返   回  : void
   
=============================================================*/
void DestoryAllLayers(void);
/*=============================================================
 函数名称 : SetLayerAttributes
 功    能 : 设置窗口层属性
 参    数 : int nLayer[IN]   --- 层索引
            unsigned int crKey[IN] --- 透明颜色值
            unsigned char bAlpha[IN] --- 颜色透明度
            unsigned int dwFlags[IN] --- 类型标志
返   回  : 1 : 成功
           0 : 失败
   
=============================================================*/
int SetLayerAttributes(int nLayer,unsigned int crKey,unsigned char bAlpha,unsigned int dwFlags);
/*=============================================================
 函数名称 : UniteLayer
 功    能 : 合并层
 参    数 : int nDstLayer[IN] --- 目的层索引
			int dx,dy[IN]     --- 目的层左上角坐标
			int w,h[IN]       --- 层宽高
            int nSrcLayer[IN] --- 源层索引
            int sx,sy[IN]     --- 源层左上角坐标
			unsigned char isBlend[IN] --- 混合/覆盖标志
返   回  : 1 : 成功
           0 : 失败
   
=============================================================*/
int UniteLayer( int nDstLayer,int dx,int dy,int w,int h,int nSrcLayer,int sx,int sy,unsigned char isBlend);
/*=============================================================
 函数名称 : SetLayerVisible
 功    能 : 设置层显示/隐藏,返回老的标志
 参    数 : int nLayer[IN]          --- 层索引
			uint8 isVisible[IN]     --- 显示标志
			
 返   回  : 1 - 可视
            0 - 不可视
=============================================================*/
int SetLayerVisible(int nLayer,unsigned char isVisible);
/*=============================================================
 函数名称 : UniteAllLayers
 功    能 : 追加所有层到序列
 参    数 : void
返   回  : void
   
=============================================================*/
void UniteAllLayers(void);
/*=============================================================
 函数名称 : IsSupportLayer
 功    能 : 层支持校验
 参    数 : void
返   回  : 1 - 支持
           0 - 不支持
=============================================================*/
int IsSupportLayer(void);
/*=============================================================
 函数名称 : MoveLayer
 功    能 : 把层移动到指定层的后面
 参    数 : int nLayer[IN] --- 待移动层索引
			int nLayerAfter[IN]     --- 指定层索引
返   回  : 1 - 成功
           0 - 不成功
=============================================================*/
int MoveLayer(int nLayer, int nLayerAfter);
#endif //_ADAPTOR_LAYER_
