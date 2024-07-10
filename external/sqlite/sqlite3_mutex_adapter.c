/*****************************************************************************
** File Name:      sqlite3_mutex_adapter.c                                   *
** Author:                                                                   *
** Date:             04/11/2011                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe file array module           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2011       Cheng.Luo          Create                                 *
******************************************************************************/

#if (SQLITE_MUTEX_APPDEF == 1)
#ifdef SQLITE_MUTEX_ADAPETER

#include "os_api.h"

//////////////////////////////////////////////////////////////////////////

#define ADAPTER_MUTEX_INITIALIZER		SCI_NULL, 0, 0, SCI_INVALID_BLOCK_ID
#define ADAPTER_MUTEX_DYNAMIC_NAME		"ADAPTER_MUTEX_DYNAMIC_NAME"
#define ADAPTER_MUTEX_STATIC_NAME		"ADAPTER_MUTEX_STATIC_NAME"

//////////////////////////////////////////////////////////////////////////

struct sqlite3_mutex {
	SCI_MUTEX_PTR mutex_ptr;
	int		id;   
	int		nRef;
	BLOCK_ID	owner;
};

static sqlite3_mutex staticMutexes[] = 
{
	{ ADAPTER_MUTEX_INITIALIZER},
	{ ADAPTER_MUTEX_INITIALIZER},
	{ ADAPTER_MUTEX_INITIALIZER},
	{ ADAPTER_MUTEX_INITIALIZER},
	{ ADAPTER_MUTEX_INITIALIZER},
	{ ADAPTER_MUTEX_INITIALIZER},
};

static int isInit = 0;

//////////////////////////////////////////////////////////////////////////
#ifdef SQLITE_DEBUG
static int adapterMutexHeld(sqlite3_mutex *p)
{
	return 0!=p && p->nRef!=0 && p->owner==SCI_IdentifyThread();
}

static int adapterMutexNotheld(sqlite3_mutex *p)
{
	return 0!=p && p->nRef==0 || p->owner!=SCI_IdentifyThread();
}
#endif

/*******************************************************************/
//  Interface:		adapterMutexInit
//  Description : 	adapterMutexInit
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static int adapterMutexInit(void)
{
	return SQLITE_OK; 
}

/*******************************************************************/
//  Interface:		adapterMutexEnd
//  Description : 	adapterMutexEnd
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static int adapterMutexEnd(void)
{
	return SQLITE_OK;
}

/*******************************************************************/
//  Interface:		adapterMutexAlloc
//  Description : 	adapterMutexAlloc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static sqlite3_mutex *adapterMutexAlloc(int iType)
{
	sqlite3_mutex *p = PNULL;
	
	switch( iType )
	{
    case SQLITE_MUTEX_FAST:
    case SQLITE_MUTEX_RECURSIVE:
		{
			p = sqlite3MallocZero( sizeof(*p) );
			if( p )
			{
				p->id = iType;
				p->mutex_ptr = SCI_CreateMutex(ADAPTER_MUTEX_DYNAMIC_NAME, SCI_INHERIT);
				if(SCI_NULL == p->mutex_ptr)
				{
					sqlite3_free( p );
					p = NULL;
				}
			}
			break;
		}
    default: 
		{
			if ( !isInit )
			{	
				unsigned int i,j;				
				for( i = 0; i < sizeof(staticMutexes)/sizeof(staticMutexes[0]); i++ )
				{
					staticMutexes[i].mutex_ptr = SCI_CreateMutex(ADAPTER_MUTEX_STATIC_NAME, SCI_INHERIT);
					if (SCI_NULL == staticMutexes[i].mutex_ptr)
					{
						for (j = 0; j < i; j++)
						{
							SCI_DeleteMutex(staticMutexes[j].mutex_ptr);
							staticMutexes[j].mutex_ptr = SCI_NULL;
						}
						return NULL;
					}
				}
				isInit = 1;				
			}
			assert( iType-2 >= 0 );
			assert( iType-2 < sizeof(staticMutexes)/sizeof(staticMutexes[0]) );
			p = &staticMutexes[iType-2];
			p->id = iType;
			break;
		}
	}
	
	return p;
}

/*******************************************************************/
//  Interface:		adapterMutexFree
//  Description : 	adapterMutexFree
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static void adapterMutexFree(sqlite3_mutex *p)
{
    if (PNULL == p->mutex_ptr)
    {
        SCI_TRACE_LOW("@sqlite3, adapterMutexFree,mutex_ptr is PNULL.");
        return;
    }

	assert( p );
	assert( p->nRef==0 );
	assert( p->id==SQLITE_MUTEX_FAST || p->id==SQLITE_MUTEX_RECURSIVE );
	SCI_DeleteMutex(p->mutex_ptr);
	p->mutex_ptr = SCI_NULL;
	sqlite3_free(p);
}

/*******************************************************************/
//  Interface:		adapterMutexEnter
//  Description : 	adapterMutexEnter
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static void adapterMutexEnter(sqlite3_mutex *p)
{
    if (PNULL == p->mutex_ptr)
    {
        SCI_TRACE_LOW("@sqlite3, adapterMutexEnter,mutex_ptr is PNULL.");
        return;
    }
    
    assert( p->id==SQLITE_MUTEX_RECURSIVE || adapterMutexNotheld(p) );
    SCI_GetMutex(p->mutex_ptr, SCI_WAIT_FOREVER);
    p->owner = SCI_IdentifyThread(); 
    p->nRef++;
}

/*******************************************************************/
//  Interface:		adapterMutexTry
//  Description : 	adapterMutexTry
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static int adapterMutexTry(sqlite3_mutex *p)
{
	return SQLITE_BUSY;
}

/*******************************************************************/
//  Interface:		adapterMutexLeave
//  Description : 	adapterMutexLeave
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
static void adapterMutexLeave(sqlite3_mutex *p)
{      
    if (PNULL == p->mutex_ptr)
    {
        SCI_TRACE_LOW("@sqlite3, adapterMutexLeave,mutex_ptr is PNULL.");
        return;
    }

    assert( p->nRef>0 );
    assert( p->owner==SCI_IdentifyThread());   
    p->nRef--;
    assert( p->nRef==0 || p->id==SQLITE_MUTEX_RECURSIVE );
    SCI_PutMutex(p->mutex_ptr);
}

/*******************************************************************/
//  Interface:		sqlite3DefaultMutex
//  Description : 	sqlite3DefaultMutex
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
sqlite3_mutex_methods *sqlite3DefaultMutex(void)
{
	static sqlite3_mutex_methods sMutex = 
	{
		adapterMutexInit,
		adapterMutexEnd,
		adapterMutexAlloc,
		adapterMutexFree,
		adapterMutexEnter,
		adapterMutexTry,
		adapterMutexLeave,
#ifdef SQLITE_DEBUG
		adapterMutexHeld,
		adapterMutexNotheld
#else
		0,
		0
#endif
	};
	
	return &sMutex;
}

#endif /*SQLITE_MUTEX_ADAPETER*/
#endif /*SQLITE_MUTEX_APPDEF*/
