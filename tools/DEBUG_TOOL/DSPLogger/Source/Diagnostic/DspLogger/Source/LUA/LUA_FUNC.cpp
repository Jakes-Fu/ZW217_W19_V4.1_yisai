#include "stdafx.h"
#include "LUA_FUNC.h"

/* the Lua interpreter */
BOOL g_bInitLud = FALSE;
lua_State* L1;
lua_State* L2;
lua_State* L3;

void lua_init()
{
	g_bInitLud = FALSE;
	//dsp_description.lua should be in the same folder as DspLogger.exe
    char szPath[_MAX_PATH];
    CWinApp* pApp = AfxGetApp();
    ::GetModuleFileName(pApp->m_hInstance,szPath,_MAX_PATH);
    char* p = strrchr(szPath,'\\');
    p++;
	CString	sErrMsg;

	/* initialize Lua */
    L1 = lua_open();
    L2 = lua_open();
	L3 = lua_open();

    /* load Lua base libraries */
	luaL_openlibs(L1);
	luaL_openlibs(L2);
	luaL_openlibs(L3);

	/* load the script */
	strcpy(p, "dsp_keyinfo.lua");
	if (luaL_dofile(L3, szPath))
	{
		sErrMsg.Format("LUA ERROR: %s",lua_tostring(L3,-1));
		AfxMessageBox(sErrMsg);
		lua_pop(L3, 1);
		return;
	}

    /* load the script */
    strcpy(p,"dsp_data.lua");
	if (luaL_dofile(L2, szPath))
	{
		sErrMsg.Format("LUA ERROR: %s",lua_tostring(L2,-1));
		AfxMessageBox(sErrMsg);
		lua_pop(L2, 1);
		return;
	}

    strcpy(p,"dsp_description.lua");
    /* load the script */
	if (luaL_dofile(L1, szPath))
	{
		sErrMsg.Format("LUA ERROR: %s",lua_tostring(L1,-1));
		AfxMessageBox(sErrMsg);
		lua_pop(L1, 1);
		return;
	}

	/* the function name */
	lua_getglobal(L1, "init_func_list");
	lua_getglobal(L2, "init_func_list");
	lua_getglobal(L3, "init_func_list");

	/* call the function */
	if (lua_pcall(L1, 0, 0, 0))
	{
		sErrMsg.Format("LUA ERROR: %s",lua_tostring(L1,-1));
		AfxMessageBox(sErrMsg);
		lua_pop(L1, 1);
		return;
	}

	if (lua_pcall(L2, 0, 0, 0))
	{
		sErrMsg.Format("LUA ERROR: %s",lua_tostring(L2,-1));
		AfxMessageBox(sErrMsg);
		lua_pop(L2, 1);
		return;
	}

	if (lua_pcall(L3, 0, 0, 0))
	{
		sErrMsg.Format("LUA ERROR: %s",lua_tostring(L3,-1));
		AfxMessageBox(sErrMsg);
		lua_pop(L3, 1);
		return;
	}
	g_bInitLud = TRUE;

}

int lua_description_ex( int address, int data, char* str_desc,int nMaxLen)
{
	char * tmp_str=0;
	
	/* the function name */
	lua_getglobal(L1, "dsp_desc");
	
	/* the first argument */
	lua_pushnumber(L1, address);
	
	/* the second argument */
	lua_pushnumber(L1, data);
	
	/* call the function with 2
	arguments, return 1 result */
	lua_pcall(L1, 2, 1, 0);
	
	/* get the result */
	tmp_str=(char *)lua_tostring(L1,-1);
	
	//sprintf(*str_desc,"%s",tmp_str);
	if (tmp_str)
	{
		strncpy(str_desc,tmp_str,nMaxLen);
	}
	else
	{
		strcpy(str_desc,"(null)");
	}
	
	lua_pop(L1, 1);
	
	return 0;
}

int lua_description ( int address, int data, char** str_desc)
{
	char * tmp_str=0;
	
	/* the function name */
	lua_getglobal(L1, "dsp_desc");
	
	/* the first argument */
	lua_pushnumber(L1, address);
	
	/* the second argument */
	lua_pushnumber(L1, data);
	
	/* call the function with 2
	arguments, return 1 result */
	lua_pcall(L1, 2, 1, 0);
	
	/* get the result */
	tmp_str=(char *)lua_tostring(L1,-1);
	
	sprintf(*str_desc,"%s",tmp_str);
	
	lua_pop(L1, 1);
	
	return 0;
}

int lua_data( int address, int data, int* new_data)
{
	/* the function name */
	lua_getglobal(L2, "dsp_data");
	
	/* the first argument */
	lua_pushnumber(L2, address);
	
	/* the second argument */
	lua_pushnumber(L2, data);
	
	/* call the function with 2
	arguments, return 1 result */
	if (lua_pcall(L2, 2, 1, 0))
	{
		*new_data=NULL;
	}
	else
	{
		/* get the result */
		*new_data=(int)lua_tonumber(L2,-1);
	}
	
	lua_pop(L2, 1);
	
	return 0;
}

int lua_keyinfo(int address, int data, char* type, char** new_str)
{
	char * tmp_str=0;
	int nRet = 0;
	/* the function name */
    lua_getglobal(L3, "dsp_keyinfo");
	
	/* the first argument */
	lua_pushnumber(L3, address);
	
	/* the second argument */
	lua_pushnumber(L3, data);

	/* the third argument */
	lua_pushstring(L3, type);
	
	/* call the function with 3
	arguments, return 1 result */
	if (lua_pcall(L3, 3, 1, 0))
	{
		*new_str=NULL;
		nRet = 1;
	}
	else
	{
		/* get the result */

		tmp_str=(char *)lua_tostring(L3,-1);	
		sprintf(*new_str,"%s",tmp_str);
		nRet = 0;
	}
	
	lua_pop(L3, 1);
	
	return nRet;
}

int lua_table(char *ptable, std::vector<LPSTR>*pkeyinfo, std::vector<WORD>*pvecAddr)
{
	char *pKey = NULL;
	char *pValue = NULL;
	int nValue = 0;
	int nIndex = 0;
	if(ptable == NULL)
	{
		return 0;
	}
	
	//get table
	lua_getglobal(L3, ptable);
	lua_pushnil(L3);
	while (0 != lua_next(L3, -2))
	{
		pKey = (char *)lua_tostring(L3,-2);
		if(lua_type(L3, -1) == LUA_TSTRING)
		{
			pValue = (char *)lua_tostring(L3,-1);
		}
		else if(lua_type(L3, -1) == LUA_TNUMBER || lua_type(L3, -1) == LUA_TBOOLEAN)
		{
			nValue = (int)lua_tonumber(L3, -1);
		}
		else if(lua_type(L3, -1) == LUA_TTABLE)
		{
		}
		pkeyinfo->push_back(pKey);
		pvecAddr->push_back((WORD)nValue);
		nIndex++;
		
		lua_pop(L3, 1);
	}

	lua_pop(L3, 1);
	
	return 1;
}

int lua_item(char *pItem, std::vector<LPSTR>*ptable)
{
	char *pValue = NULL;
	int nValue = 0;
	int nIndex = 0;
	if(pItem == NULL)
	{
		return 0;
	}
	
	//get table
	lua_getglobal(L3, pItem);
	lua_pushnil(L3);
	while (0 != lua_next(L3, -2))
	{
		if(lua_type(L3, -1) == LUA_TSTRING)
		{
			pValue = (char *)lua_tostring(L3,-1);
		}
		else if(lua_type(L3, -1) == LUA_TNUMBER || lua_type(L3, -1) == LUA_TBOOLEAN)
		{
			nValue = (int)lua_tonumber(L3, -1);
		}
		else if(lua_type(L3, -1) == LUA_TTABLE)
		{
		}
		ptable->push_back(pValue);
		nIndex++;
		
		lua_pop(L3, 1);
	}

	lua_pop(L3, 1);
	
	return 1;
}

int lua_chart(char *pItem, int* new_data)
{
	char *pKey = NULL;
	char *pValue = NULL;
	int nValue = 0;
	if(pItem == NULL)
	{
		return 0;
	}
	
	//get table
	lua_getglobal(L3, "graph_info");
	lua_pushnil(L3);
	while (0 != lua_next(L3, -2))
	{
		pKey = (char *)lua_tostring(L3,-2);
		if(lua_type(L3, -1) == LUA_TSTRING)
		{
			pValue = (char *)lua_tostring(L3,-1);
		}
		else if(lua_type(L3, -1) == LUA_TNUMBER || lua_type(L3, -1) == LUA_TBOOLEAN)
		{
			nValue = (int)lua_tonumber(L3, -1);
		}
		else if(lua_type(L3, -1) == LUA_TTABLE)
		{
		}
		if(_tcscmp(pKey, pItem) == 0)
		{
			*new_data = nValue;
		}
		
		lua_pop(L3, 1);
	}

	lua_pop(L3, 1);
	
	return 1;
}

void lua_clean()
{
	if(g_bInitLud)
	{
		/* cleanup Lua */
        lua_close(L1);
        lua_close(L2);
		lua_close(L3);
	}
}