/****************************************************************************
** File Name:      mmi_event_manager.c                                              *
** Author:         gang.tong                                              *
** Date:           03/11/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the event database manager.  *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2009        gang.tong        Create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#ifdef WIN32
//#include "std_header.h"
//#endif

#include "mmi_event_manager.h"
#include "mmi_event_api.h"
#include "mmi_link.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct
{
	uint32  					event_type;
	uint32						num;
	MMI_LINK_NODE_T *			notify_handle_list;
}EVENT_LIST_NODE_T;

typedef struct
{
	void*						callback;
}EVENT_HANDLE_NODE_T;


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMI_LINK_NODE_T * s_register_database_ptr = PNULL;
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : search event list
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL BOOLEAN SearchEventList(
							  MMI_LINK_NODE_T const* node_ptr,
							  uint32 condition1, 
							  uint32 condition2
							  )
{
	if(condition1 == ((EVENT_LIST_NODE_T*)node_ptr->data)->event_type)
	{
		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************/
//  Description : search event list
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL BOOLEAN SearchEventHandle(
								MMI_LINK_NODE_T const* node_ptr,
								uint32 condition1,
								uint32 condition2
								)
{
	if(condition1 == (uint32)((EVENT_HANDLE_NODE_T*)node_ptr->data)->callback)
	{
		return TRUE;
	}
	return FALSE;
}										 

/*****************************************************************************/
//  Description : add event事件到database中
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL BOOLEAN AddEventToDatabase( 
								 uint32  event_type, 
								 void*	callback
								 )
{
	MMI_LINK_NODE_T *event_list_ptr = PNULL;
	MMI_LINK_NODE_T *event_node_ptr = PNULL;
	MMI_LINK_NODE_T	*link_node_ptr = PNULL;
	
	if (PNULL == callback)
	{
		return FALSE;
	}
	
	if(PNULL != s_register_database_ptr)
	{
		event_list_ptr = MMILINK_SearchNode(s_register_database_ptr, TRUE, SearchEventList, event_type, 0);
	}
	
	if(PNULL != event_list_ptr)
	{
		/* search callback in this list */
		if (PNULL != ((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list)
		{
			event_node_ptr = MMILINK_SearchNode(((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list,\
				TRUE, SearchEventHandle, (uint32)callback, 0);
		}
		
		if(PNULL != event_node_ptr)
		{
			/* node exist, return. */
			return TRUE;
		}
		else
		{
			EVENT_HANDLE_NODE_T * node_ptr = PNULL;
			
			node_ptr = SCI_ALLOCA(sizeof(EVENT_HANDLE_NODE_T));
			if (PNULL == node_ptr)
			{
				return FALSE;
			}
			node_ptr->callback = callback;
			
			/* add event handle */
			if(PNULL == ((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list)
			{
				link_node_ptr = MMILINK_CreateHead((uint32)node_ptr);
				if (PNULL == link_node_ptr)
				{
					SCI_FREE(node_ptr);
					return FALSE;
				}
				
				((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list = link_node_ptr;
				((EVENT_LIST_NODE_T*)event_list_ptr->data)->num = 1;
			}
			else
			{
				link_node_ptr = MMILINK_CreateNode((uint32)node_ptr);
				if (PNULL == link_node_ptr)
				{
					SCI_FREE(node_ptr);
					return FALSE;
				}
				MMILINK_AddNodeBeforeBaseNode(((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list,\
					link_node_ptr);
				((EVENT_LIST_NODE_T*)event_list_ptr->data)->num ++;
			}
		}
	}
	else
	{
		/* add event list and handle */
		EVENT_LIST_NODE_T * list_ptr = PNULL;        			
		EVENT_HANDLE_NODE_T * node_ptr = PNULL;        			
		
		/* add event list */			
		list_ptr = SCI_ALLOCA(sizeof(EVENT_LIST_NODE_T));
		if (PNULL == list_ptr)
		{
			return FALSE;
		}
		
		list_ptr->event_type = event_type;
		list_ptr->num = 0;
		list_ptr->notify_handle_list = PNULL;		
		if(PNULL != s_register_database_ptr)
		{
			link_node_ptr = MMILINK_CreateNode((uint32)list_ptr);
			if (PNULL == link_node_ptr)
			{
				SCI_FREE(list_ptr);
				return FALSE;
			}
			MMILINK_AddNodeBeforeBaseNode(s_register_database_ptr, link_node_ptr);		
		}
		else
		{
			link_node_ptr = MMILINK_CreateHead((uint32)list_ptr);
			if (PNULL == link_node_ptr)
			{
				SCI_FREE(list_ptr);
				return FALSE;
			}
			s_register_database_ptr = link_node_ptr;
		}
		
		/* add event handle */
		node_ptr = SCI_ALLOCA(sizeof(EVENT_HANDLE_NODE_T)); 
		if (PNULL == node_ptr)
		{
			return FALSE;
		}
		
		node_ptr->callback = callback;
		link_node_ptr = MMILINK_CreateHead((uint32)node_ptr);
		if (PNULL == link_node_ptr)
		{
			SCI_FREE(node_ptr);
			return FALSE;
		}
		
		list_ptr->notify_handle_list = link_node_ptr;	
		list_ptr->num = 1;
	}
	
	return (PNULL != link_node_ptr);
}

/*******************************************************************/
//  Interface:		DestroyEventNode
//  Description : 	DestroyEventNode
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void DestroyEventNode(MMI_LINK_NODE_T const * node_ptr)
{
	EVENT_HANDLE_NODE_T *handle_node_ptr = NULL;
	
	if (PNULL != node_ptr)
	{
		handle_node_ptr = (EVENT_HANDLE_NODE_T*)node_ptr->data;
		SCI_FREE(handle_node_ptr);
	}
}

/*****************************************************************************/
//  Description : remove event事件从database中
//  Global resource dependence : 
//  Author: cheng.luo
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL BOOLEAN RemoveEventToDatabase( 
									uint32  event_type, 
									void*	callback
									)
{
	MMI_LINK_NODE_T *event_list_ptr = PNULL;
	MMI_LINK_NODE_T *event_node_ptr = PNULL;
	MMI_LINK_NODE_T	*link_node_ptr = PNULL;
	
	if (PNULL == s_register_database_ptr
		|| PNULL == callback)
	{
		return TRUE;
	}
	
	if (PNULL != s_register_database_ptr)
	{
		event_list_ptr = MMILINK_SearchNode(s_register_database_ptr, TRUE, SearchEventList, event_type, 0);
	}	
	if (PNULL != event_list_ptr)
	{
		if (PNULL != ((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list)
		{
			event_node_ptr = MMILINK_SearchNode(((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list,\
				TRUE, SearchEventHandle, (uint32)callback, 0);
		}
		if(PNULL != event_node_ptr)
		{
			link_node_ptr = MMILINK_DestroyNode(event_node_ptr, ((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list,\
				DestroyEventNode);
			((EVENT_LIST_NODE_T*)event_list_ptr->data)->num--;
			((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list = link_node_ptr;
		}
	}
	
	return TRUE;
}

/*****************************************************************************/
//  Description : 从database中获取event事件
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T * GetEventListFromDatabase(uint32 event_type)
{
	MMI_LINK_NODE_T *event_list_ptr = PNULL;
	
	if (PNULL != s_register_database_ptr)
	{
		event_list_ptr = MMILINK_SearchNode(s_register_database_ptr, TRUE, SearchEventList, event_type, 0);
	}
	
	if(PNULL != event_list_ptr)
	{  		
		return ((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list;
	}
	
	return PNULL;
}

//////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description : 该event事件和处理是否已在database中
//  Global resource dependence : 
//  Author: cheng.luo
//  Note: TRUE exsit, FALSE not exsit
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_EventIsRegister( 
									 uint32  event_type, 
									 void*	 callback
									 )
{
	BOOLEAN			bret = FALSE;
	MMI_LINK_NODE_T *event_list_ptr = PNULL;
	MMI_LINK_NODE_T *event_node_ptr = PNULL;
	
	if (PNULL == callback)
	{
		return FALSE;
	}
	
	if (PNULL != s_register_database_ptr)
	{
		event_list_ptr = MMILINK_SearchNode(s_register_database_ptr, TRUE, SearchEventList, event_type, 0);
	}
	if (PNULL != event_list_ptr)
	{
		if (PNULL != ((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list)
		{
			event_node_ptr = MMILINK_SearchNode(((EVENT_LIST_NODE_T*)event_list_ptr->data)->notify_handle_list,\
				TRUE, SearchEventHandle, (uint32)callback, 0);
		}
		if(PNULL != event_node_ptr)
		{
			bret = TRUE;
		}
	}
	
	return bret;
}	

/*****************************************************************************/
//  Description : 注册event事件
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_RegisterEvent(
								   uint32  event_type,
								   void*	  call_back
								   )
{
	return AddEventToDatabase(event_type, call_back);
}

/*****************************************************************************/
//  Description : 去注册event事件
//  Global resource dependence : 
//  Author: cheng.luo
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_UnregisterEvent(
									 uint32  event_type,
									 void*	call_back
									 )
{
	return RemoveEventToDatabase(event_type, call_back);
}

/*****************************************************************************/
//  Description : 触发event事件
//  Global resource dependence : 
//  Author: gang.tong
//  Note: TRUE success, FALSE fail
/*****************************************************************************/
PUBLIC BOOLEAN MMIFW_PumpEvent( 
							   uint32 event_type,
							   uint32 argv[],
							   uint32 argc)
{
	MMI_LINK_NODE_T *event_list_ptr = PNULL;	
	MMI_LINK_NODE_T *event_move_ptr = PNULL;	
	EVENT_HANDLE_NODE_T * node_ptr = PNULL;
	
	event_list_ptr = GetEventListFromDatabase(event_type);	
	if(PNULL != event_list_ptr)
	{
		event_move_ptr = event_list_ptr;
		do
		{
			node_ptr = (EVENT_HANDLE_NODE_T*)event_move_ptr->data;
			MMIFW_EventCallBack(event_type, node_ptr->callback, argc, argv);
			event_move_ptr = event_move_ptr->next_ptr;
		}
		while(event_move_ptr != event_list_ptr && PNULL != event_move_ptr);
	}
	
	return TRUE;
}
