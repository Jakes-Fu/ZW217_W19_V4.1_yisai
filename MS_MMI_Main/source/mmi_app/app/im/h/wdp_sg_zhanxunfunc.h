#ifndef WDP_SG_ZHANXUNFUNC_H
#define WDP_SG_ZHANXUNFUNC_H

#include "wdp_SG_Interface_net.h"

typedef void(*FuncPtr)(void);

/******************************************************************************/
// Description: 搜狗输入法确认回调函数
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
void mmi_sogou_ime_confirm(SG_UINT16* sg_text,
						   FuncPtr yes_callback, 
						   FuncPtr no_callback);
#endif //WDP_SG_ZHANXUNFUNC_H
