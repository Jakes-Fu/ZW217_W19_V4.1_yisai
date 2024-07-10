/******************************************************************************
 ** File Name:      production_test.c                                         *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/28/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 09/28/2004      Xueliang.Wang   Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "production_test.h"
#include "nvitem.h"
#include "flash.h"
#include "efs.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define PRODUCTION_TEST_DEBUG
#ifdef PRODUCTION_TEST_DEBUG
	#define PRODUCTION_PRINT    SCI_TRACE_LOW
#else
	#define PRODUCTION_PRINT	
#endif	// PRODUCTION_TEST_DEBUG
/**---------------------------------------------------------------------------*
 **                            Data structure                                 *
 **---------------------------------------------------------------------------*/
#ifdef NANDBOOT_SUPPORT
#define	NVITEM_UpdateProductInfo_Protect	NVITEM_UpdateProductInfo
#define	NVITEM_GetProductInfo_Protect		NVITEM_GetProductInfo
#endif
/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define TRACK_INFO_BASE_ADDR    (0x1000000 - 0x2000)
#ifdef __BIG_ENDIAN
#define EndianSwap32(l)  ((((l) & 0xff000000) >> 24) |(((l) & 0x00ff0000) >>  8) |(((l) & 0x0000ff00) <<  8) |(((l) & 0x000000ff) << 24))
#define EndianSwap16(s) ((((s) >> 8) & 0x00ff) | (((s) << 8) & 0xff00))
#else
#define EndianSwap32(l)   (l)
#define EndianSwap16(s)  (s)
#endif
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to get production test result 
//                  information.
//  Author:         Xueliang.Wang
//	Note:           This function does nothing if it is called when playing
//                  GenericTone or Ring.
/*****************************************************************************/
PUBLIC uint32 SCI_GetProductionTestInfo(// If succeed, return SCI_SUCCESS,
                                    	// else return error value
    PRODUCTION_TEST_INFO_T  *info_ptr   // Used to save test result.
    )
{
    TEST_TRACK_HEADER_T *test_header_ptr = NULL;
    TEST_DATA_INFO_T* prod_info_ptr;
    uint32  i;
    uint32  item_num; 
    uint16  len;
    uint16  station_state;
    uint16  station_flag;
    char    test_pass[5] = {'P','A','S','S', '\0'};
    char    test_undo[5]   = {'U','N','D','O', '\0'};
    char    test_fail[5]   = {'F','A','I','L', '\0'};
    char    test_info_sn=0;

	// Check input parameter    
	if (info_ptr == PNULL)
	{
		return SCI_ERROR;
	}
	prod_info_ptr = (TEST_DATA_INFO_T *)SCI_ALLOC_APP(sizeof(TEST_DATA_INFO_T) + 4);
	SCI_ASSERT(prod_info_ptr);
	NVITEM_GetProductInfo_Protect((uint8*)prod_info_ptr,sizeof(TEST_DATA_INFO_T));
	len = sizeof (TEST_DATA_INFO_T);
   
	SCI_MEMSET(info_ptr, 0, sizeof(PRODUCTION_TEST_INFO_T));/*lint !e718 !e746 -e718 -e746*/

	test_header_ptr = (TEST_TRACK_HEADER_T *)(prod_info_ptr);
	test_info_sn=test_header_ptr->SN[0];
	if((test_info_sn>47&&test_info_sn<58)||(test_info_sn>64&&test_info_sn<91)||(test_info_sn>96&&test_info_sn<123))
	{
		/* Get SerialNum */
	SCI_MEMCPY(info_ptr->serial_num,test_header_ptr->SN,SN_LEN); /*lint !e613*/
	}
	else
	{
		SCI_MEMCPY(info_ptr->serial_num,"none",5); /*lint !e613*/
	}
	
	/* Get total test item number, and check if it is valid */
	item_num = EndianSwap32(test_header_ptr->StationNum); /*lint !e613*/
	if (item_num >= PRODUCTION_TEST_ITEM_MAX_NUM)
	{ 
		/*
		* no test info is available
		*/ 
		item_num = 0;
	}

	info_ptr->total_item_num = item_num;

	/* Get every item test information.*/
	for (i = 0; i < item_num; i++)
	{
		SCI_MEMCPY(info_ptr->item_info[i].item_name, (char*)&test_header_ptr->StationName[i], MAX_STATION_NAME_LEN); /*lint !e613*/
		station_state  = EndianSwap16(test_header_ptr->iItem) & (0x0001<< i); /*lint !e613*/
		station_flag    = EndianSwap16(test_header_ptr->iTestSign) & (0x0001<< i); /*lint !e613*/
		if(test_header_ptr->SignFlag == 0x0) /*lint !e613*/
		{
			if(station_state != 0x0)
			{
				SCI_MEMCPY(info_ptr->item_info[i].test_state,   test_pass, 5);
			}
			else
			{
				SCI_MEMCPY(info_ptr->item_info[i].test_state,   test_undo, 5);
			}
		}
		else
		{
			if(station_state == 0x0)
			{
				SCI_MEMCPY(info_ptr->item_info[i].test_state,   test_pass, 5);
			}
			else
			{
				if(station_flag == 0x0)
				{
					SCI_MEMCPY(info_ptr->item_info[i].test_state,   test_fail, 5);
				}
				else
				{
					SCI_MEMCPY(info_ptr->item_info[i].test_state,   test_undo, 5);
				}
			}
		}
	}
	SCI_FREE(prod_info_ptr);
	return SCI_SUCCESS;
 
}
/******************************************************************************
 *  DUMMY API to fix link errors
 ******************************************************************************/

#if (defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)) && !defined(BLUETOOTH_SUPPORT)
typedef void (*BT_LAYER1_CALLBACK)(BOOLEAN bt_on_off);
void BT_LAYER1_Reg_Callback(BT_LAYER1_CALLBACK callback)
{

}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
