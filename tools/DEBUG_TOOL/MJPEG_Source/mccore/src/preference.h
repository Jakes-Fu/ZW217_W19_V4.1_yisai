/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifndef _PREFERENCE_H
#define _PREFERENCE_H

#include "paramlistdef.h"
#include "mcpref.h"

class CPrefHelper
{
public:
	CPrefHelper():key(0) {}
	~CPrefHelper() { if (key) free(key); }
	void GetValue(int id, int* pvalue);
	void GetValue(int id, char** pvalue);
	void GetValue(int id, bool* pvalue);
	void GetValue(int id, float* pvalue);

	int GetIdByKey(char* key);
	char* GetKeyById(int id);

	void* GetValue(int id);
	int GetValueInt(int id);
	char* GetValueStr(int id);
	bool GetValueBool(int id);
	float GetValueFloat(int id);
	char* GetValueEnum(int id);

	void* GetValue(char* key);
	int GetValueInt(char* key);
	char* GetValueStr(char* key);
	bool GetValueBool(char* key);
	float GetValueFloat(char* key);
	char* GetValueEnum(char* key);

	int SetValue(int id, int value);
	char* SetValue(int id, char* value);
	bool SetValue(int id, bool value);
	float SetValue(int id, float value);
	void* SetValue(int id, void* value);
	char* SetValue(int id, CString& value);
	bool IsDefaultValue(int id);
	void SetDefaultValue(int id, int value);
	void SetDefaultValue(int id, char* value);
	void SetDefaultValue(int id, bool value);
	void SetDefaultValue(int id, float value);
	PARAM_DATA* GetEntry(int id);
private:
	char* key;
};

extern CPrefHelper pref;

#endif
