#ifndef _GUIRICHTEXT_H_
#define _GUIRICHTEXT_H_

/*---------------------------------------------------------------------------
                          Include Files                                    
 --------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guifont.h"
#include "ctrlrichtext_export.h"

/*------------------------------------------------------------------------------
    Compiler Flag
------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*------------------------------------------------------------------------------
    FUNCTION DEFINITION
------------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief GUIRICHTEXT_AddItem
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@param item_ptr [in] item数据
@param index_ptr [in/out]返回当前item的index，可根据该id操作item
@return 是否增加成功 
@note 当增加一个图片item, 并且图片是buf数据时, GUIRICHTEXT_IMAGE_T结构中的buf_ptr是由窗口申请内存的, 控件不负责申请和释放
@note 焦点类型的设置，请参看GUIRICHTEXT_TAG_TYPE_E定义处的详细说明
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_ModifyItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
#define GUIRICHTEXT_AddItem CTRLRICHTEXT_AddItem

/***************************************************************************//*!
@brief GUIRICHTEXT_DeleteItem
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@param item_index [in]item的index，在add item的时候可以得到
@return 操作是否成功
@note delete之后，如果需要刷新显示，请调用GUIRICHTEXT_Update
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_ModifyItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
#define GUIRICHTEXT_DeleteItem CTRLRICHTEXT_DeleteItem

/***************************************************************************//*!
@brief GUIRICHTEXT_ModifyItem
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@param item_index [in] item的index，在add item的时候可以得到
@param item_ptr [in] item数据
@return 操作是否成功
@note modify之后，如果需要刷新显示，请调用GUIRICHTEXT_Update
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
#define GUIRICHTEXT_ModifyItem CTRLRICHTEXT_ModifyItem

/***************************************************************************//*!
@brief 删除所有的item
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@return 操作是否成功
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_ModifyItem
*******************************************************************************/
#define GUIRICHTEXT_DeleteAllItems CTRLRICHTEXT_DeleteAllItems

/***************************************************************************//*!
@brief 显示控件
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@return 操作是否成功
@note 在刷新之前，会更新控件的数据
@sa GUIRICHTEXT_UpdateData
*******************************************************************************/
#define GUIRICHTEXT_Update CTRLRICHTEXT_Update

/***************************************************************************//*!
@brief 仅更新控件的数据，而不刷新
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@return 操作是否成功
@note 有很多数据是通过计算得到的，而计算只是在需要的时候才进行，通常是在显示的时
候。如果需要即时计算控件的数据，则需要通过更新数据来完成。
@sa GUIRICHTEXT_Update
*******************************************************************************/
#define GUIRICHTEXT_UpdateData CTRLRICHTEXT_UpdateData

/***************************************************************************//*!
@brief 获取指定index的item
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param item_index [in]item的index，在add item的时候可以得到
@param item_ptr [in]item数据指针
@return 操作是否成功
@note 该item的内容并非原始内容，可能会因为显示需要而修改过。比如hyper parse给出
显示的字符，格式丢了，同样text id格式的文字也被构造成了buffer形式的
@sa GUIRICHTEXT_AddItem
*******************************************************************************/
#define GUIRICHTEXT_GetItem CTRLRICHTEXT_GetItem

/***************************************************************************//*!
@brief 获取焦点元素
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param element [in]焦点数据的指针
@return 操作是否成功
@note 
- 即当前焦点所在元素的信息
- 焦点类型，请参看GUIRICHTEXT_TAG_TYPE_E定义处的详细说明
*******************************************************************************/
#define GUIRICHTEXT_GetFocusElement CTRLRICHTEXT_GetFocusElement

/***************************************************************************//*!
@brief 获取焦点Item的索引号
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param index_ptr [in]索引号指针
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_GetFocusItemIndex CTRLRICHTEXT_GetFocusItemIndex

/***************************************************************************//*!
@brief 设置整个控件的默认字体
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param font [in]字体
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetFont CTRLRICHTEXT_SetFont

/***************************************************************************//*!
@brief 设置整个控件的默认字体颜色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in]颜色
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetFontColor CTRLRICHTEXT_SetFontColor

/***************************************************************************//*!
@brief 设置显示区域
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param rect [in]显示区域
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetRect CTRLRICHTEXT_SetRect

/***************************************************************************//*!
@brief 设置字间距
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]字间距
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetCharSpace CTRLRICHTEXT_SetCharSpace

/***************************************************************************//*!
@brief 设置文本的行间距
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]行间距
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetLineSpace CTRLRICHTEXT_SetLineSpace

/***************************************************************************//*!
@brief 设置图片和文本的距离
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]文本和图片之间的距离
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetImgTxtSpace CTRLRICHTEXT_SetImgTxtSpace

/***************************************************************************//*!
@brief 设置item之间的间距
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]item之间的间距
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetItemSpace CTRLRICHTEXT_SetItemSpace

/***************************************************************************//*!
@brief 设置边界距，即item和控件边缘的距离
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]item和控件边缘的距离
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetBorderSpace CTRLRICHTEXT_SetBorderSpace

/***************************************************************************//*!
@brief 设置边界距，item四周的留白
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param space [in]item四周的留白
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetItemBorderSpace CTRLRICHTEXT_SetItemBorderSpace

/***************************************************************************//*!
@brief 设置文字到边框的空白
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param margin [in]设置文字到边框的空白
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetTxtMargin CTRLRICHTEXT_SetTxtMargin

/***************************************************************************//*!
@brief 设置切换焦点的方式
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param focus_mode [in]焦点模式
@return 操作是否成功
@sa _GUIRICHTEXT_FOCUS_SHIFT_E
@sa GUIRICHTEXT_TAG_TYPE_E
*******************************************************************************/
#define GUIRICHTEXT_SetFocusMode CTRLRICHTEXT_SetFocusMode

/***************************************************************************//*!
@brief 设置焦点字符的下划线颜色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in] 颜色
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetFocusLineColor CTRLRICHTEXT_SetFocusLineColor

/***************************************************************************//*!
@brief 设置焦点区域选择时的背景色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in]颜色
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetFocusBgColor CTRLRICHTEXT_SetFocusBgColor

/***************************************************************************//*!
@brief 设置控件的背景色
@author Lianxiang.Zhou
@param ctrl_id [in] 控件id
@param color [in] 颜色
@return 操作是否成功
@note 当没有背景图片或者背景图片失效的时候，背景色才启动
*******************************************************************************/
#define GUIRICHTEXT_SetBgColor CTRLRICHTEXT_SetBgColor

/***************************************************************************//*!
@brief 设置控件的背景图片
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param bg_img_id [in]背景图片id
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetBgImage CTRLRICHTEXT_SetBgImage

/***************************************************************************//*!
@brief 设置边框,默认为GUIRICHTEXT_FRAME_NONE
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param border_type [in]边框类型
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetBorder CTRLRICHTEXT_SetBorder

/***************************************************************************//*!
@brief 设置边框的颜色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in]颜色
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetBorderColor CTRLRICHTEXT_SetBorderColor

/***************************************************************************//*!
@brief 设置焦点元素的边框颜色
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param color [in]颜色
@return 操作是否成功
@note 焦点item在显示边框的时候，会有不同的颜色
*******************************************************************************/
#define GUIRICHTEXT_SetBorderFocusColor CTRLRICHTEXT_SetBorderFocusColor

/***************************************************************************//*!
@brief 设置在图片解析过程中显示的图片
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param img_id [in]图片的id
@return 操作是否成功
@note 有些图片的显示会要等待，在等待过程中，会显示一个等待图片
*******************************************************************************/
#define GUIRICHTEXT_SetAnimParsingIcon CTRLRICHTEXT_SetAnimParsingIcon

/***************************************************************************//*!
@brief 设置在图片解析错误时显示的图片
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param img_id [in]图片的id
@return 操作是否成功
@note 在图片不能显示的时候，显示指定图片
*******************************************************************************/
#define GUIRICHTEXT_SetAnimErrorIcon CTRLRICHTEXT_SetAnimErrorIcon

/***************************************************************************//*!
@brief 指定焦点item
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param item_index [in]item的index，在add item的时候可以得到
@return 操作是否成功
*******************************************************************************/
#define GUIRICHTEXT_SetFocusItem CTRLRICHTEXT_SetFocusItem

/***************************************************************************//*!
@brief 设定图片解码是否同步
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param is_img_syn_decode [in]是否同步解码
@return 操作是否成功
@note 在一些对显示速度有要求的应用，会采用异步解码。
*******************************************************************************/
#define GUIRICHTEXT_SetImgDecodeSyn CTRLRICHTEXT_SetImgDecodeSyn

/***************************************************************************//*!
@brief 重新初始化控件的显示参数
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@note 用于在重新设置控件条目后不需要保持状态的情况
*******************************************************************************/
#define GUIRICHTEXT_ResetState CTRLRICHTEXT_ResetState

/***************************************************************************//*!
@brief 设置显示到焦点元素所在页面
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
*******************************************************************************/
#define GUIRICHTEXT_ScrollToFocus CTRLRICHTEXT_ScrollToFocus

/***************************************************************************//*!
@brief 设置显示指定item所在页面
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@param item_index [in]item的index，在add item的时候可以得到
@return 是否滚动成功
@note 新加入的item必须在update之后才能正确的滚动显示。因为在update的时候计算rect
update在其后的第一个刷新会自动做，也可以调用函数GUIRICHTEXT_Update来完成
*******************************************************************************/
#define GUIRICHTEXT_ScrollToItem CTRLRICHTEXT_ScrollToItem

/***************************************************************************//*!
@brief 启用剪贴板功能
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@return 是否设置成功
@sa GUIRICHTEXT_DisableClipBoard
@note 默认剪切板是开启的
-# 选字，选字有下面几种方式：
    -# 通过触笔拖动选择
    -# 长按触笔，弹出菜单全选
-# 复制，可复制的文字有下面几种：
    -# 选定的文字
    -# 焦点文字
*******************************************************************************/
#define GUIRICHTEXT_EnableClipBoard CTRLRICHTEXT_EnableClipBoard

/***************************************************************************//*!
@brief 禁用剪贴板功能
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@return 是否设置成功
@sa GUIRICHTEXT_EnableClipBoard
*******************************************************************************/
#define GUIRICHTEXT_DisableClipBoard CTRLRICHTEXT_DisableClipBoard

/***************************************************************************//*!
@brief 保存控件的显示中间信息，包括剪切板、滚动位置等
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@return 是否保存成功
@sa GUIRICHTEXT_RestoreInfo
@warning 一定要调用GUIRICHTEXT_RestoreInfo来恢复，否则引起显示混乱
@warning 焦点信息等无法保存，因为焦点信息要依赖项(item)信息，而item的信息会全部
丢失，item的信息又比较复杂，甚至不能保证在操作之后，原来的item还存在，故而，无法
保存。
@details 一些应用会因为简化应用的需要，对控件进行摧毁重建，而在重建的过程中，会
导致一些信息的丢失，比如剪切板信息、滚动位置信息等。这时候，我们就可以采用对其中
的一些信息进行保护。
*******************************************************************************/
#define GUIRICHTEXT_SaveInfo CTRLRICHTEXT_SaveInfo

/***************************************************************************//*!
@brief 恢复控件的显示中间信息，包括剪切板、滚动位置等
@author Lianxiang.Zhou
@param ctrl_id [in]控件id
@return 是否恢复成功
@sa GUIRICHTEXT_SaveInfo
*******************************************************************************/
#define GUIRICHTEXT_RestoreInfo CTRLRICHTEXT_RestoreInfo

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
#define GUIRICHTEXT_GetHighlightPos CTRLRICHTEXT_GetHighlightPos

/*------------------------------------------------------------------------------
    Compiler Flag
------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_GUIRICHTEXT_H_

/*@}*/

