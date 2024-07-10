
#ifndef HELLO_MENUTABLE_H
#define HELLO_MENUTABLE_H

#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
//²Ëµ¥ID
typedef enum
{
    ID_HELLO_TEST1,
    ID_HELLO_TEST2,
    ID_HELLO_TEST3,
    
    ID_POPUP_MENU_1,
    ID_POPUP_MENU_2,
    ID_POPUP_MENU_3,
    ID_POPUP_MENU_SUB1,
    ID_POPUP_MENU_SUB2,
    ID_POPUP_MENU_SUB3,

	ID_HELLO_TOTAL
}MMI_HELLO_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
	HELLO_MENU_LABEL_START = (MMI_MODULE_SAMPLE << 16),
    #include "sample_menutable.def"
	MENU_HELLO_TABLE_MAX
} MenuHelloLabelE;

#undef MENU_DEF

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register hello menu group
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIHELLO_RegMenuGroup(void);

/*-------------------------------------------------------------------------*/
/*                         VARIABLES                                       */
/*-------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* HELLO_MENUTABLE_H */
