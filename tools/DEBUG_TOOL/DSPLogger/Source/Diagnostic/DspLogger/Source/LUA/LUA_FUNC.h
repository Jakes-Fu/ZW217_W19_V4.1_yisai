extern "C" 
{
	#include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#pragma warning(push,3)
#include <vector>
#pragma warning(pop)
#pragma warning( disable : 4018)
using namespace std;

int lua_description ( int address, int data, char** str_desc);
int lua_description_ex( int address, int data, char* str_desc,int nMaxLen = MAX_LUA_DESCTRIPTION_LEN);

int lua_data( int address, int data, int* new_data);

int lua_table(char *ptable, std::vector<LPSTR>*pkeyinfo, std::vector<WORD>*pvecAddr);

int lua_item(char *pItem, std::vector<LPSTR>*ptable);

int lua_chart(char *pItem, int* new_data);

int lua_keyinfo(int address, int data, char* type, char** new_str);

void lua_init();

void lua_clean();