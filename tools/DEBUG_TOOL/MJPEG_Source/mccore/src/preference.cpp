/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#include "config.h"
#include <windows.h>
#include <stdio.h>
#include "atlstr.h"
#include "preference.h"
#include "resource.h"
#include "paramlistdef.h"

CPrefHelper pref;
extern PARAM_DATA prefParams[];

void CPrefHelper::GetValue(int id, int* pvalue)
{
	*pvalue = (int)prefParams[id].value;
}

void CPrefHelper::GetValue(int id, char** pvalue)
{
	*pvalue = (char*)prefParams[id].value;
}

void CPrefHelper::GetValue(int id, bool* pvalue)
{
	*pvalue = (prefParams[id].value != 0);
}

void CPrefHelper::GetValue(int id, float* pvalue)
{
	memcpy((void*)pvalue, (void*)&prefParams[id].value, 4);
}

int CPrefHelper::GetIdByKey(char* key)
{
	bool fCatMatched = false, fNodeMatched = false;
	char* catname = "";
	int catlen = 0;
	char* nodename = "";
	int nodelen = 0;

	for (int i = 0; prefParams[i].type; i++) {
		if (prefParams[i].type == PT_CATALOG) {
			catname = prefParams[i].pchKeyName;
			catlen = strlen(catname);
			if (!strncmp(key, catname, catlen)) {
				if (key[catlen] == 0) return i;
				fCatMatched = true;
			} else {
				fCatMatched = false;
			}
		}
		if (!fCatMatched) continue;
		if (prefParams[i].flags & PF_NODE) {
			nodename = prefParams[i].pchKeyName;
			nodelen = strlen(nodename);
			if (!strncmp(key + catlen + 1, nodename, nodelen)) {
				if (key[catlen + nodelen + 1] == 0) return i;
				fNodeMatched = true;
			} else {
				fNodeMatched = false;
			}
		}
		if (!fNodeMatched) continue;
		if (!strcmp(key + catlen + nodelen + 2, prefParams[i].pchKeyName)) return i;
	}
	return -1;
}

int CPrefHelper::GetValueInt(char* key)
{
	int id = GetIdByKey(key);
	if (id < 0) return 0;
	return GetValueInt(id);
}

char* CPrefHelper::GetValueStr(char* key)
{
	int id = GetIdByKey(key);
	if (id < 0) return 0;
	return GetValueStr(id);
}

bool CPrefHelper::GetValueBool(char* key)
{
	int id = GetIdByKey(key);
	if (id < 0) return false;
	return GetValueBool(id);
}

char* CPrefHelper::GetValueEnum(char* key)
{
	int id = GetIdByKey(key);
	if (id < 0) return 0;
	return GetValueEnum(id);
}

float CPrefHelper::GetValueFloat(char* key)
{
	int id = GetIdByKey(key);
	if (id < 0) return 0;
	return GetValueFloat(id);
}

void* CPrefHelper::GetValue(char* key)
{
	int id = GetIdByKey(key);
	if (id < 0) return 0;
	return GetValue(id);
}

int CPrefHelper::GetValueInt(int id)
{
	return (int)prefParams[id].value;
}

char* CPrefHelper::GetValueStr(int id)
{
	static char buf[12];

	switch (prefParams[id].type) {
	case PT_BOOL:
	case PT_ENUM:
	case PT_INT:
		sprintf(buf, "%d", prefParams[id].value);
		return buf;
	case PT_FLOAT:
		float v;
		GetValue(id, &v);
		_snprintf(buf, sizeof(buf), "%f", v);
		return buf;
	default:
		return (char*)prefParams[id].value;
	}
}

bool CPrefHelper::GetValueBool(int id)
{
	return (prefParams[id].value != 0);
}

char* CPrefHelper::GetValueEnum(int id)
{
	char *p=(char*)prefParams[id].pchEnums;
	int i=0;
	do {
		if (i++==(int)prefParams[id].value) {
			if (*p == '\t') p++;
			return p;
		}
		while (*(p++));
	} while (*p);
	return 0;
}

float CPrefHelper::GetValueFloat(int id)
{
	float val;
	memcpy((void*)&val, (void*)&prefParams[id].value, 4);
	return val;
}

void* CPrefHelper::GetValue(int id)
{
	return prefParams[id].value;
}

int CPrefHelper::SetValue(int id, int value)
{
	prefParams[id].value = (void*)value;
	return value;
}

char* CPrefHelper::SetValue(int id, char* value)
{
	prefParams[id].value = (void*)value;
	return value;
}

char* CPrefHelper::SetValue(int id, CString& value)
{
	return (char*)(prefParams[id].value = (void*)value.GetBuffer());
}

void* CPrefHelper::SetValue(int id, void* value)
{
	prefParams[id].value = value;
	return value;
}

bool CPrefHelper::SetValue(int id, bool value)
{
	prefParams[id].value = (void*)(value);
	return value;
}

float CPrefHelper::SetValue(int id, float value)
{
	memcpy(&prefParams[id].value, &value, sizeof(value));
	return value;
}

bool CPrefHelper::IsDefaultValue(int id)
{
	return ((prefParams[id].value == prefParams[id].valueDefault));
}

void CPrefHelper::SetDefaultValue(int id, int value)
{
	prefParams[id].valueDefault = (void*)value;
}

void CPrefHelper::SetDefaultValue(int id, char* value)
{
	prefParams[id].valueDefault = (void*)strdup(value);
	if (!*(char*)prefParams[id].value) strcpy((char*)prefParams[id].value, value);
}

void CPrefHelper::SetDefaultValue(int id, bool value)
{
	prefParams[id].valueDefault = (void*)(value);
}

void CPrefHelper::SetDefaultValue(int id, float value)
{
	memcpy(&prefParams[id].valueDefault, &value, sizeof(value));
}

char* CPrefHelper::GetKeyById(int id)
{
	int i;
	char* catkey = 0;
	char* nodekey = 0;

	if (key) {
		free(key);
		key = 0;
	}
	for (i = 0; prefParams[i].type; i++) {
		if (i == id) {
			if (prefParams[i].type == PT_CATALOG) {
				key = strdup(prefParams[i].pchKeyName);
			} else if (prefParams[i].flags & PF_NODE) {
				if (catkey) {
					key = (char*)malloc(strlen(catkey)+ strlen(prefParams[i].pchKeyName) + 2);
					sprintf(key, "%s/%s", catkey, prefParams[i].pchKeyName);
				}
			} else if (catkey && nodekey) {
				key = (char*)malloc(strlen(catkey) + strlen(nodekey) + strlen(prefParams[i].pchKeyName) + 3);
				sprintf(key, "%s/%s/%s", catkey, nodekey, prefParams[i].pchKeyName);
			}
			return key;
		} else if (prefParams[i].type == PT_CATALOG) {
			catkey = prefParams[i].pchKeyName;
		} else if (prefParams[i].flags & PF_NODE) {
			nodekey = prefParams[i].pchKeyName;
		}
	}
	return 0;
}

PARAM_DATA* CPrefHelper::GetEntry(int id)
{
	return &prefParams[id];
}
