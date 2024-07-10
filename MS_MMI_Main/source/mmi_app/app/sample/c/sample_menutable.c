

#define MENUTABLE_C
#include "std_header.h"

#ifdef MMI_MOCOR_SAMPLE
#include "sci_types.h"
#include "sample_text.h"
#include "sample_image.h"
#include "mmi_image.h"
#include "guimenu.h"
#include "sample_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmibt_text.h"

#define TIP_NULL 0

// menu table
const GUIMENU_ITEM_T menu_hello_table[] =
{
    {ID_HELLO_TEST1,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST1,  IMAGE_SECMENU_ICON_SET_CALL,    0,0,0},
    {ID_HELLO_TEST2,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST2,  IMAGE_SECMENU_ICON_SET_PHONE,    0,0,0},
    {ID_HELLO_TEST3,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST3,  IMAGE_SECMENU_ICON_SET_DISPLAY,    0,0,0},
};

// menu table
const GUIMENU_ITEM_T menu_test_popupmenu_table[] =
{
    {ID_POPUP_MENU_1,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST1,  0,    0,0,0},
    {ID_POPUP_MENU_2,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST2,  0,    0,0,0},
    {ID_POPUP_MENU_3,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST3,  0,    0,0,MENU_TEST_SUBMENU_TABLE},
};

// menu table
const GUIMENU_ITEM_T menu_test_submenu_table[] =
{
    {ID_POPUP_MENU_SUB1,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST1,  0,    0,0,0},
    {ID_POPUP_MENU_SUB2,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST2,  0,    0,0,0},
    {ID_POPUP_MENU_SUB3,      TIP_NULL,{TXT_COMMON_OK,TXT_NULL,STXT_RETURN},TXT_HELLO_TEST3,  0,    0,0,0},
};


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T hello_menu_table[] = 
{
    {0},
    #include "sample_menutable.def"
};

/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIHELLO_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_SAMPLE, hello_menu_table);
}

#undef MENU_DEF

#endif
