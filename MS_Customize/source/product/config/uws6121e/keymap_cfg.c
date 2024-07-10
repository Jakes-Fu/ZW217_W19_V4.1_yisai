/******************************************************************************
 ** File Name:      keymap_sp9800.c                                          *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           30/05/2007                                                *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 30/05/2007     Benjamin.Wang    Create.                                   *
 ******************************************************************************/
#include "tb_dal.h" 

#ifdef ZDT_PCBA_ZW201_SUPPORT
LOCAL const uint16 keymap[] = {
       /*			KEYIN0					KEYIN1					KEYIN2					KEYIN3					KEYIN4					KEYIN5					KEYIN6					KEYIN7					   */
	/*				|						|						|						|						|						|						|						|                                     */
	/*KEYOUT0------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o--------------------  */
	/*				|						|						|						|						|						|						|						|				           */
					SCI_VK_SHORTCUT,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,  	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                    */
	/*KEYOUT1------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------- o----------------------o----------------------*/
	/*				|						|						|						|						|						|						|						|                                    */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                     */
	/*KEYOUT2------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o---------------------*/
	/*				|						|						|						|						|						|						|						|                                     */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                      */
	/*KEYOUT3------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o---------------------*/
	/*				|						|						|						|						|						|						|						|                                     */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                      */
	/*				|						|						|						|						|						|						|						|                                     */

};
#else
#ifdef HW_6121_pre_w217
// For uws6121e openphone
LOCAL const uint16 keymap[] = {
       /*			KEYIN0					KEYIN1					KEYIN2					KEYIN3					KEYIN4					KEYIN5					KEYIN6					KEYIN7					   */
	/*				|						|						|						|						|						|						|						|                                     */
	/*KEYOUT0------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o--------------------  */
	/*				|						|						|						|						|						|						|						|				           */
					SCI_VK_1,	               SCI_VK_2,			       SCI_VK_3,                          SCI_VK_LEFT,			       SCI_VK_WEB,  	              SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                    */
	/*KEYOUT1------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------- o----------------------o----------------------*/
	/*				|						|						|						|						|						|						|						|                                    */
					SCI_VK_4,			SCI_VK_5,				SCI_VK_6,				SCI_VK_RIGHT,			SCI_VK_MENU_CANCEL,       SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                     */
	/*KEYOUT2------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o---------------------*/
	/*				|						|						|						|						|						|						|						|                                     */
					SCI_VK_7,			SCI_VK_8,				SCI_VK_9,				SCI_VK_UP,			       SCI_VK_MENU_SELECT,       SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                      */
	/*KEYOUT3------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o---------------------*/
	/*				|						|						|						|						|						|						|						|                                     */
					SCI_VK_STAR,		SCI_VK_0,				SCI_VK_POUND,			SCI_VK_DOWN,			SCI_VK_INVALID_KEY,        SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                      */
	/*				|						|						|						|						|						|						|						|                                     */
	/*KEYOUT4------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o---------------------*/
	/*				|						|						|						|						|						|						|						|                                      */
					SCI_VK_CALL,		SCI_VK_INVALID_KEY,	      SCI_VK_INVALID_KEY,	       SCI_VK_INVALID_KEY,	        SCI_VK_INVALID_KEY,	      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|	                            	|						|                                      */
	/*				|						|						|						|						|						|	                     		|						|                                     */
	/*KEYOUT5------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o--------------------- */
	/*				|						|						|						|						|						|		                     	|						|                                      */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	      SCI_VK_INVALID_KEY,	       SCI_VK_INVALID_KEY,	        SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY
	/*				|						|						|						|						|						|		                     	|						|                                      */
	/*				|						|						|						|						|						|		                     	|						|                                      */

};
#else
// For uws6121e watch
LOCAL const uint16 keymap[] = {
       /*			KEYIN0					KEYIN1					KEYIN2					KEYIN3					KEYIN4					KEYIN5					KEYIN6					KEYIN7					   */
	/*				|						|						|						|						|						|						|						|                                     */
	/*KEYOUT0------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o--------------------  */
	/*				|						|						|						|						|						|						|						|				           */
					SCI_VK_SHORTCUT,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,  	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                    */
	/*KEYOUT1------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------- o----------------------o----------------------*/
	/*				|						|						|						|						|						|						|						|                                    */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                     */
	/*KEYOUT2------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o---------------------*/
	/*				|						|						|						|						|						|						|						|                                     */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                     */
	/*				|						|						|						|						|						|						|						|                                      */
	/*KEYOUT3------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o---------------------*/
	/*				|						|						|						|						|						|						|						|                                     */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|						|						|                                      */
	/*				|						|						|						|						|						|						|						|                                     */
	/*KEYOUT4------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o---------------------*/
	/*				|						|						|						|						|						|						|						|                                      */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,
	/*				|						|						|						|						|						|	                            	|						|                                      */
	/*				|						|						|						|						|						|	                     		|						|                                     */
	/*KEYOUT5------o-----------------------o-----------------------o-----------------------o-----------------------o-----------------------o----------------------  o-----------------------o--------------------- */
	/*				|						|						|						|						|						|		                     	|						|                                      */
					SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY,      SCI_VK_INVALID_KEY
	/*				|						|						|						|						|						|		                     	|						|                                      */
	/*				|						|						|						|						|						|		                     	|						|                                      */

};
#endif
#endif

PUBLIC uint16* KPD_GetKeyMap( uint32* keymap_size)
{
    *keymap_size=0;
    *keymap_size = sizeof(keymap)/sizeof(keymap[0]);
    return (uint16*)keymap;
}


