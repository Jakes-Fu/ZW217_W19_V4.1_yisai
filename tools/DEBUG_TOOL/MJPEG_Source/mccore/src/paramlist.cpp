/*******************************************************************
* Parameter List Implementation
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <stdio.h>
#include "atlstr.h"
#include "resource.h"
#include "windowhandle.h"
#include "paramlist.h"
#ifdef PARAM_XML
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#endif

extern PARAM_DATA prefParams[];

DWORD hex2dw(TCHAR *p)
{
	register TCHAR c;
	register DWORD i=0;

	if (!p) return 0;
	while (*p && (*p==' ' || *p=='\t')) p++;
	for(c=*p;;){
		if (c>='A' && c<='F')
			c-=7;
		else if (c>='a' && c<='f')
			c-=39;
		else if (c<'0' || c>'9')
			break;
		i=(i<<4)|(c&0xF);
		c=*(++p);
	}
	return i;
}

void InitParams(PARAM_DATA* params)
{
	int i;

	if (!params) return;
	for (i=0;params[i].type;i++) {
		switch (params[i].type) {
		case PT_FILE:
		case PT_DIR:
			params[i].min=0;
			params[i].max=MAX_PATH - 1;
		case PT_STRING: {
			if (!params[i].max) break;
			params[i].value = malloc(params[i].max+1);
			if (!params[i].valueDefault) {
				params[i].valueDefault="";
				*(TCHAR*)params[i].value=0;
			} else {
				if (strlen((TCHAR*)params[i].valueDefault) <= (unsigned int)params[i].max)
					strcpy((TCHAR*)params[i].value,(TCHAR*)params[i].valueDefault);
			}
			} break;
		case PT_CATALOG:
			break;
		default:
			params[i].value=params[i].valueDefault;
		}
	}
}

void DestroyParams(PARAM_DATA* params)
{
	int i;
	if (!params) return;
	for (i=0;params[i].type;i++) {
		switch (params[i].type&0xf) {
		case PT_FILE:
		case PT_DIR:
		case PT_STRING:
			if (params[i].value) free(params[i].value);
			break;
		}
	}
}

DWORD GetChecksum(TCHAR* str)
{
	DWORD sum = 0;
	int off = 0;
	for (; str[off]; sum += (str[off]<<(off%32)), off++);
	return sum;
}

int IsParamModified(PARAM_DATA* params)
{
	int i;
	for (i=0;params[i].type;i++) {
		if (params[i].flags & PF_MODIFIED) {
			return 1;
		}
	}
	return 0;
}

#ifdef PARAM_XML
TCHAR* StripChars(TCHAR* deststr, TCHAR* srcstr)
{
	TCHAR *s = srcstr, *d = deststr;
	int skipped = 1;
	while (*s) {
		if ((*s >='a' && *s <= 'z') || (*s >='A' && *s <= 'Z') || (*s >='0' && *s <= '9')) {
			if (skipped) {
				*d = toupper(*s);
				skipped = 0;
			} else {
				*d = *s;
			}
			d++;
			s++;
		} else {
			s++;
			skipped++;
		}
	}
	*d = 0;
	return deststr;
}

#define ROOT_PATH "MediaCoder"

int LoadParamNames(PARAM_DATA* params, TCHAR* filename)
{
	xmlChar *value;
	xmlDocPtr doc = xmlParseFile(filename);
	TCHAR* catname = "";
	TCHAR* nodename = 0;
	CString path;
	CString optpath;
	CString optstr;

	for (int i = 0; params[i].type; i++) {
		xmlXPathContextPtr context;
		if (!(context = xmlXPathNewContext(doc))) break;
		if (params[i].type == PT_CATALOG) {
			catname = params[i].pchKeyName;
			nodename = 0;
			path.Format("/Pref/%s/Node", catname);
		} else {
			path.Format("/Pref/%s/", catname);
			if (params[i].flags & PF_NODE) {
				nodename = params[i].pchKeyName;
				path.AppendFormat("%s/Node", nodename);
			} else if (nodename) {
				path.AppendFormat("%s/%s", nodename, params[i].pchKeyName);
			} else {
				path += params[i].pchKeyName;
			}
		}
		optpath = path + "/Name";
		xmlXPathObjectPtr result = xmlXPathEvalExpression((xmlChar*)optpath.GetBuffer(), context);
		if (result != NULL) {
			if (!xmlXPathNodeSetIsEmpty(result->nodesetval)) {
				value = xmlNodeListGetString(doc, result->nodesetval->nodeTab[0]->xmlChildrenNode, 1);
				CString val = value;
				val = val.SpanExcluding("\n\r\t");
				if (!val.IsEmpty()) {
					params[i].pchParamName = strdup(val.GetBuffer());
				} else {
					params[i].pchParamName = strdup(params[i].pchParamName);
				}
				xmlFree(value);
			}
			xmlXPathFreeObject(result);
		}

		optpath = path + "/Options";
		result = xmlXPathEvalExpression((xmlChar*)optpath.GetBuffer(), context);
		if (result != NULL) {
			if (!xmlXPathNodeSetIsEmpty(result->nodesetval)) {
				value = xmlNodeListGetString(doc, result->nodesetval->nodeTab[0]->xmlChildrenNode, 1);
				if (value && *value) {
					params[i].pchEnums = (char*)malloc(strlen((const char*)value) + 2);
					char *s = (char*)value;
					char *d = params[i].pchEnums;
					do {
						*(d++) = (*s == '|') ? 0 : *s;
					} while (*(s++) != 0);
					*d = 0;
				}
				xmlFree(value);
			}
			xmlXPathFreeObject(result);
		}
		xmlXPathFreeContext(context);

	}	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return 0;
}

int SaveParams(PARAM_DATA* params, TCHAR* filename)
{
	int i;
	xmlDocPtr doc;
	xmlNodePtr cur;
	int depth = 0;
	char* catkey = 0;
	char* nodekey = 0;

	if (!params) return -1;

	doc = xmlNewDoc((xmlChar*)"1.0");
	cur = xmlNewDocNode(doc, 0, (xmlChar*)ROOT_PATH, (xmlChar*)"");
	xmlDocSetRootElement(doc, cur);

	//write parameters
	for (i=0; params[i].type; i++) {
		char valbuf[32];
		int skip=0;

		if (params[i].type == PT_CATALOG) {
			catkey = params[i].pchKeyName;
			nodekey = 0;
		} else if (params[i].flags & PF_NODE) {
			nodekey = params[i].pchKeyName;
		}

		if (params[i].flags & PF_NOSAVE) continue;
		switch (params[i].type) {
		case PT_FILE:
		case PT_DIR:
		case PT_STRING:
			if (!strcmp((TCHAR*)params[i].value,(TCHAR*)params[i].valueDefault)) skip=1;
			break;
		case PT_SEPERATOR:
		case PT_CATALOG:
			skip=1;
			break;
		default:
			if (params[i].value==params[i].valueDefault) skip=1;
			break;
		}
		if (skip) continue;

		if (catkey) {
			// to add catalog name
			if (depth == 2) {
				cur = cur->parent;
				depth--;
			}
			if (depth == 1) {
				cur = cur->parent;
				depth--;
			}
			cur = xmlNewTextChild(cur, NULL, (const xmlChar*)catkey, (const xmlChar*)0);
			catkey = 0;
			depth++;
		}
		if (nodekey) {
			if (depth == 2) {
				cur = cur->parent;
				depth--;
			}
			cur = xmlNewTextChild(cur, NULL, (const xmlChar*)nodekey, (const xmlChar*)0);
			nodekey = 0;
			depth++;
		}
		switch (params[i].type) {
		case PT_FILE:
		case PT_DIR:
		case PT_STRING:
			if (*(char*)params[i].value)
				xmlNewTextChild(cur, NULL, (const xmlChar*)params[i].pchKeyName, (const xmlChar*)params[i].value);
			break;
		case PT_BOOL:
			xmlNewTextChild(cur, NULL, (const xmlChar*)params[i].pchKeyName, (const xmlChar*)(params[i].value ? "true" : "false"));
			break;
		case PT_DEFAULT:
		case PT_SEPERATOR:
		case PT_CATALOG:
			break;
		default:
			sprintf(valbuf, "%d", params[i].value);
			xmlNewTextChild(cur, NULL, (const xmlChar*)params[i].pchKeyName, (const xmlChar*)valbuf);
		}
	}
	xmlSaveFormatFile(filename, doc, 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return 0;
}

int LoadParams(PARAM_DATA* params, TCHAR* filename)
{
	xmlChar *value;
	xmlDocPtr doc = xmlParseFile(filename);
	TCHAR* catname = "";
	TCHAR* nodename = 0;
	CString path;

	for (int i = 0; params[i].type; i++) {
		xmlXPathContextPtr context;
		if (!params[i].pchKeyName) continue;
		if (params[i].type == PT_CATALOG) {
			catname = params[i].pchKeyName;
			nodename = 0;
			continue;
		}
		if ( params[i].pchKeyName && !strcmp(params[i].pchKeyName, "PostProcessing")) {
			int x = 0;
		}
		path.Format("/%s/%s/", ROOT_PATH, catname);
		if (params[i].flags & PF_NODE) {
			nodename = params[i].pchKeyName;
			path.AppendFormat("%s/%s", nodename, nodename);
		} else if (nodename) {
			path.AppendFormat("%s/%s", nodename, params[i].pchKeyName);
		} else {
			path += params[i].pchKeyName;
		}
		if (!(context = xmlXPathNewContext(doc))) break;
		xmlXPathObjectPtr result = xmlXPathEvalExpression((xmlChar*)path.GetBuffer(), context);
		if (result != NULL) {
			if (!xmlXPathNodeSetIsEmpty(result->nodesetval)) {
				value = xmlNodeListGetString(doc, result->nodesetval->nodeTab[0]->xmlChildrenNode, 1);
				if (value) {
					switch (params[i].type) {
					case PT_FILE:
					case PT_DIR:
					case PT_STRING:
						strncpy((char*)params[i].value, (const char*)value, params[i].max);
						*((char*)params[i].value + params[i].max) = 0;
						break;
					case PT_BOOL:
						params[i].value = (!stricmp((const char*)value, "true")) ? (void*)1 : 0;
						break;
					case PT_DEFAULT:
					case PT_SEPERATOR:
						break;
					default:
						params[i].value = (void*)atoi((const char*)value);
					}
					xmlFree(value);
				}
			}
			xmlXPathFreeObject(result);
		}
		xmlXPathFreeContext(context);
	}	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return 0;
}

#else

int SaveParams(PARAM_DATA* params, TCHAR* filename)
{
	FILE *fp = 0;
	int i;

	if (!params) return -1;
	fp = fopen(filename, "wb");
	if (!fp) return -1;
	//write parameters
	for (i=0;params[i].type;i++) {
		DWORD checksum;
		int skip=0;
		if (params[i].flags & PF_NOSAVE) continue;
		switch (params[i].type) {
		case PT_FILE:
		case PT_DIR:
		case PT_STRING:
			if (!strcmp((TCHAR*)params[i].value,(TCHAR*)params[i].valueDefault)) skip=1;
			break;
		case PT_SEPERATOR:
		case PT_CATALOG:
			skip=1;
			break;
		default:
			if (params[i].value==params[i].valueDefault) skip=1;
			break;
		}
		if (skip) continue;
		checksum = GetChecksum((TCHAR*)params[i].pchParamName);
		fwrite(&checksum, sizeof(checksum), 1, fp);	
		fputc(params[i].type,fp);
		switch (params[i].type) {
		case PT_FILE:
		case PT_DIR:
		case PT_STRING:
            fwrite(params[i].value,strlen((const TCHAR*)params[i].value)+1,1,fp);
			break;
		case PT_BOOL:
			fputc(params[i].value?1:0,fp);
			break;
		case PT_DEFAULT:
		case PT_SEPERATOR:
		case PT_CATALOG:
			break;
		default:
			fwrite((void*)&params[i].value,sizeof(void*),1,fp);
		}
	}
	if (fp) fclose(fp);
	return 0;
}

int LoadParamNames(PARAM_DATA* params, TCHAR* filename)
{
	return 0;
}

int LoadParams(PARAM_DATA* params, TCHAR* filename)
{
	FILE *fp;
	int i,ich=0;
	TCHAR *p;
	DWORD checksum;

	if (!params) return -1;
	if (!filename) return 0;
	fp = fopen(filename,"rb");
	if (!fp) return -1;
	for (i = 0; (params + i)->type; i++) {
		//verify parameter type
		if (params[i].flags & PF_NOSAVE) continue;
		if (fread(&checksum, 1, sizeof(checksum), fp) != sizeof(checksum)) break;
		if (checksum != GetChecksum((TCHAR*)params[i].pchParamName)) {
			int n;
			for (n = i + 1; (params + n)->type; n++) {
				if (checksum == GetChecksum((TCHAR*)(params+n)->pchParamName)) {
					i = n;
					break;
				}
			}
			if (i != n) {
				for (n = i - 1; n>=0 && (params + n)->type; n--) {
					if (checksum == GetChecksum((TCHAR*)(params+n)->pchParamName)) {
						i = n;
						break;
					}
				}
			}
			if (i != n) continue;
		}
		ich = fgetc(fp); 
		if (ich == PT_DEFAULT || ich != (params[i].type&0xf))
			continue;
		switch (ich) {
		case PT_FILE:
		case PT_DIR:
		case PT_STRING: {
			int cb = 0;
			for (p = (TCHAR*)params[i].value; (ich = fgetc(fp)) > 0 && cb < params[i].max; cb++) {;
				*(p + cb) = (TCHAR)ich;
			}
			*(p + cb) = 0;
			} break;
		case PT_BOOL:
			params[i].value=(void*)fgetc(fp);
			break;
		case PT_SEPERATOR:
		case PT_CATALOG:
			break;
		default:
			fread((void*)&params[i].value,1,sizeof(void*),fp);
		}
	}
	fclose(fp);
	return 0;
}

#endif

bool CParamList::onScroll(int dlgid, int code)
{
	TCHAR buf[16];
	int pos=SendDlgMsg(dlgid,TBM_GETPOS,0,0);
	sprintf(buf,"%d",pos);
	SendDlgMsg(IDC_EDIT,WM_SETTEXT,0,(LPARAM)buf);
	return true;
}

bool CParamList::onCommand(int dlgid, int code)
{
	int ret;
	TCHAR *p;

	switch (code){
	case BN_CLICKED:
		switch (dlgid) {
		case IDC_REVERT:
			revert();
			showValue();
			break;
		case IDC_APPLY:
			saveValue();
			break;
		case IDOK:
			saveValue();
			Close();
			break;
		case IDC_BROWSE:
			switch (params[idx].type) {
			case PT_DIR:
			case PT_FILE:
				p=(TCHAR*)malloc(params[idx].max+1);
				strcpy(p,(TCHAR*)params[idx].value);
				ret=(params[idx].type == PT_DIR) ? browseForPath(p) : browseForFile(p);
				if (ret) SendDlgMsg(IDC_EDIT,WM_SETTEXT,0,(LPARAM)p);
				free(p);
				break;
			case PT_ENUM:
			case PT_BOOL:
				SendDlgMsg(IDC_VALUE,CB_SHOWDROPDOWN,true,0);
				break;
			}
			break;
		}
		break;
	}
	return false;
}

bool CParamList::onNotify(LPNMHDR pnmh)
{
	switch (pnmh->code) {
	case TCN_SELCHANGE:
		if (pnmh->hwndFrom == GetHandle(IDC_TAB)) {
			iTab=TabCtrl_GetCurSel(GetDlgItem(hwnd,IDC_TAB));
			listVariables(-1);
		}
		break;
	case TCN_SELCHANGING:
	case TVN_SELCHANGING:
		saveValue();
		break;
	case TVN_SELCHANGED:
		if (pnmh->hwndFrom == GetHandle(IDC_TREE)) {
			idx=getIndex();
			showValue();
		}
		break;
	}
	return true;
}

void CParamList::revert()
{
	bool fSlider=false;
	switch (params[idx].type) {
	case PT_FLOAT:
	case PT_INT:
		fSlider=(params[idx].max!=0);
	case PT_BOOL:
	case PT_PTR:
	case PT_ENUM:
		if (params[idx].value!=params[idx].valueDefault) {
			params[idx].flags |= PF_MODIFIED;
			params[idx].value=params[idx].valueDefault;
		}
		if (fSlider) {
			if (params[idx].type==PT_FLOAT) {
				float fVal;
				memcpy((void*)&fVal,(void*)&params[idx].value,sizeof(void*));
				SendDlgMsg(IDC_SLIDER,TBM_SETPOS,true,(LPARAM)fVal);
			} else {
				SendDlgMsg(IDC_SLIDER,TBM_SETPOS,true,(LPARAM)params[idx].value);
			}			
		}
		break;
	case PT_FILE:
	case PT_DIR:
	case PT_STRING:
		if (strcmp((const TCHAR*)params[idx].value,(const TCHAR*)params[idx].valueDefault)) {
			params[idx].flags |= PF_MODIFIED;
			strcpy((TCHAR*)params[idx].value,(const TCHAR*)params[idx].valueDefault);
		}
		break;
	}
	if (notify) (*notify)(idx,params[idx].value);
}

void CParamList::saveValue()
{
	HWND hValue=GetDlgItem(hwnd,IDC_VALUE);
	int ret;
	void* valuedef=params[idx].valueDefault;
	void* value=params[idx].value;
	TCHAR buf[256];

	if (params[idx].flags & PF_READONLY) return;
	if (params[idx].type == PT_ENUM || params[idx].type == PT_BOOL) {
		hValue=GetDlgItem(hwnd,IDC_VALUE);
	} else {
		hValue=GetDlgItem(hwnd,IDC_EDIT);
	}
	switch (params[idx].type) {
	case PT_BOOL:
	case PT_ENUM:
		ret=SendMessage(hValue,CB_GETCURSEL,0,0);
		if (ret==-1) return;
		value=(void*)ret;
		break;
	case PT_FLOAT:
		{
		SendMessage(hValue,WM_GETTEXT,15,(LPARAM)buf);
		float fVal=(float)atof(buf);
		if ((params[idx].max && fVal>(float)params[idx].max) ||
			(params[idx].min && fVal<(float)params[idx].min)) {
			sprintf(buf,"The input value of %s is out of the valid range",params[idx].pchParamName);
			MsgBox(buf,0);
			showValue();
			break;
		}
		memcpy((void*)&value,(void*)&fVal,sizeof(value));
		}
		break;
	case PT_INT:
        SendMessage(hValue,WM_GETTEXT,15,(LPARAM)buf);
		ret=atoi(buf);
		if ((params[idx].min && ret<params[idx].min) ||
			(params[idx].max && ret>params[idx].max)) {
			sprintf(buf,"The input value of %s is out of the valid range",params[idx].pchParamName);
			MsgBox(buf,0);
			showValue();
			break;
		}
		value=(void*)atoi(buf);
		break;
	case PT_PTR:
        SendMessage(hValue,WM_GETTEXT,15,(LPARAM)buf);
		value=(void*)hex2dw(buf+2);
		break;
	case PT_FILE:
	case PT_DIR:
	case PT_STRING:
		value=malloc(params[idx].max+1);
		ret=SendMessage(hValue,WM_GETTEXT,params[idx].max,(LPARAM)value);
		if (ret<params[idx].min) {
			MsgBox("Not enough characters",0);
			showValue();
			break;
		}
		if (!strcmp((TCHAR*)value,(TCHAR*)params[idx].value)) {
			free(value);
			return;
		}
		strcpy((TCHAR*)params[idx].value,(TCHAR*)value);
		free(value);
		break;
	default:
		return;
	}
	if (params[idx].type != PT_STRING && params[idx].type != PT_FILE && params[idx].type != PT_DIR) {
		if (params[idx].value==value) return;
		params[idx].value=value;
	}
	params[idx].flags |= PF_MODIFIED;
	if (notify) (*notify)(idx,value);
	return;
}

void CParamList::showValue()
{
	TCHAR *p;
	HWND hValue,hCtrl;
	void *value;
	int paramtype=params[idx].type;
	bool fEnumMode=(paramtype==PT_ENUM || paramtype==PT_BOOL);
	bool fBrowse=(paramtype==PT_DIR || paramtype==PT_FILE);
	bool fSlider=((paramtype==PT_INT || paramtype==PT_FLOAT) && params[idx].max);
	int iWindowWidth;
	int iTextBoxWidth;
	int iSliderVal;

	ShowWindow(GetHandle(IDC_EDIT),SW_HIDE);
	ShowWindow(GetHandle(IDC_BROWSE),SW_HIDE);
	ShowWindow(GetHandle(IDC_VALUE),SW_HIDE);
	ShowWindow(GetHandle(IDC_SLIDER),SW_HIDE);

	GetWindowPos(0,0,&iWindowWidth,0);
	iWindowWidth -= GetSystemMetrics(SM_CXDLGFRAME) * 2;
	iTextBoxWidth=fSlider?50:(iWindowWidth-(fBrowse?63:43));

	if (fEnumMode) {
		hValue=GetHandle(IDC_VALUE);
		SendMessage(hValue,CB_RESETCONTENT,0,0);
		EnableWindow(hValue,!(params[idx].flags & PF_READONLY));
	} else {
		hValue=GetHandle(IDC_EDIT);
		SendMessage(hValue,EM_SETREADONLY,
			(params[idx].flags & PF_READONLY)||(paramtype>=PT_SEPERATOR),0);
	}
	hCtrl=GetHandle(IDC_BROWSE);
	if (fBrowse) ShowWindow(hCtrl,SW_SHOW);

	MoveWindow(hCtrl,iTextBoxWidth+20,iBaseLine,20,21,true);
	MoveWindow(hValue,20,iBaseLine,iTextBoxWidth,21,true);
	ShowWindow(hValue,SW_SHOW);

	if (paramtype==PT_SEPERATOR || paramtype==PT_CATALOG) {
		SendMessage(hValue,WM_SETTEXT,0,0);
		UpdateWindow(hwnd);
		goto exit;
	}

	if (paramtype==PT_ENUM && params[idx].pchEnums) {
		p=(TCHAR*)params[idx].pchEnums;
		do {
			if (*p == '\t') p++;
			SendMessage(hValue,CB_ADDSTRING,0,(LPARAM)p);
			while (*(p++));
		} while (*p);
	}
	value=params[idx].value;
	switch (params[idx].type & 0xf) {
	case PT_ENUM:
		SendMessage(hValue,CB_SETCURSEL,(WPARAM)value,0);
		break;
	case PT_FILE:
	case PT_DIR:
	case PT_STRING:
        SendMessage(hValue,WM_SETTEXT,0,(LPARAM)value);
		break;
	case PT_FLOAT: {
		float fVal;
		TCHAR fmt[8],buf[64];
		sprintf(fmt,"%%.%df",params[idx].pchEnums?(int)params[idx].pchEnums:2);
		memcpy((void*)&fVal,(void*)&value,sizeof(value));
		sprintf(buf,fmt,fVal);
		iSliderVal=(int)fVal;
        SendMessage(hValue,WM_SETTEXT,0,(LPARAM)buf);
		} break;
	case PT_INT: {
		TCHAR buf[16];
		iSliderVal=(int)value;
        sprintf(buf,"%d",iSliderVal);
		SendMessage(hValue,WM_SETTEXT,0,(LPARAM)buf);
		} break;
	case PT_BOOL:
		SendMessage(hValue,CB_ADDSTRING,0,(LPARAM)"No");
		SendMessage(hValue,CB_ADDSTRING,0,(LPARAM)"Yes");
		SendMessage(hValue,CB_SETCURSEL,(WPARAM)value,0);
		break;
	case PT_PTR: {
		TCHAR buf[16];
        sprintf(buf,"0x%x",(int)value);
        SendMessage(hValue,WM_SETTEXT,0,(LPARAM)buf);
		} break;
	default:
		return;
	}

	hCtrl=GetHandle(IDC_SLIDER);
	if (fSlider) {
		SendMessage(hCtrl,TBM_SETPOS,FALSE,0);
		SendMessage(hCtrl,TBM_SETRANGEMIN,0,params[idx].min);
		SendMessage(hCtrl,TBM_SETRANGEMAX,0,params[idx].max);
		SendMessage(hCtrl,TBM_SETPAGESIZE,0,
			params[idx].pchEnums?(LPARAM)params[idx].pchEnums:(params[idx].max-params[idx].min)/16);
		SendMessage(hCtrl,TBM_SETPOS,TRUE,iSliderVal);
		MoveWindow(hCtrl,iTextBoxWidth+20,iBaseLine-2,iWindowWidth-iTextBoxWidth-42,24,false);
		ShowWindow(hCtrl,SW_SHOW);
	}

	// show setting tips
exit:
	if (info) {
		SendDlgMsg(IDC_INFO,WM_SETTEXT,0,0);
		for (int i=0; info[i].desc; i++) {
			if (info[i].id == idx) {
				SendDlgMsg(IDC_INFO,WM_SETTEXT,0,(LPARAM)info[i].desc);
				break;
			}
		}
	}
	UpdateWindow(hwnd);
}

CParamList::~CParamList()
{
	free(pages);
}

void CParamList::refresh()
{
	HWND hwndTab;
	TCITEM tie;
	int count=0;

	hwndTab=GetDlgItem(hwnd,IDC_TAB);
	TabCtrl_DeleteAllItems(hwndTab);
	// Add a tab for each of the three child dialog boxes. 
    tie.mask = TCIF_TEXT | TCIF_IMAGE; 
    tie.iImage = -1; 

	for (int i=0;params[i].type;i++) {
		if ((params[i].type & 0xf)==PT_CATALOG) {
			tie.pszText = (LPSTR) params[i].pchParamName; 
			TabCtrl_InsertItem(hwndTab, i, &tie);
			count++;
		}
	}
	if (pages) free(pages);
	if (count==0) {
		tie.pszText="General";
		TabCtrl_InsertItem(hwndTab, 0, &tie);
		pages=(int*)malloc(sizeof(int));
		pages[0]=0;
		iPageCount=1;
	} else {
		pages=(int*)malloc(count*sizeof(int));
		for (int i=0; iPageCount<count; i++) {
			if ((params[i].type & 0xf)==PT_CATALOG)
				pages[iPageCount++]=i;
		}
	}
}

int CParamList::showVariable(int varidx)
{
	int pg, i;
	for (pg=varidx; pg>=0 && params[pg].type!=PT_CATALOG; pg--);
	for (i=0; i<iPageCount && pages[i]!=pg; i++);
	if (i==iPageCount) return -1;
	iTab=i;
	TabCtrl_SetCurSel(GetDlgItem(hwnd,IDC_TAB),iTab);
	listVariables(varidx);
	return 0;
}

int CParamList::listVariables(int selected)
{
	int i;
	TVINSERTSTRUCT tvis;

	if (!hwnd || !params) return 0;
	hTree=GetHandle(IDC_TREE);
	TreeView_DeleteAllItems(hTree);

	tvis.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_STATE;
	tvis.hParent=TVI_ROOT;
	tvis.hInsertAfter=TVI_LAST;

	for (i=pages[iTab]+1; params[i].type && (params[i].type & 0xf)!=PT_CATALOG; i++) {
		HTREEITEM hitem;
		tvis.item.pszText=params[i].pchParamName?params[i].pchParamName:"";
		tvis.item.lParam=i;
		if (selected==i) {
			tvis.item.state=TVIS_EXPANDED|TVIS_BOLD;
			tvis.item.stateMask=TVIS_EXPANDED|TVIS_BOLD;
		} else {
			tvis.item.stateMask=0;
		}

		if (params[i].flags & PF_NODE) {
			if (tvis.hParent!=TVI_ROOT) {
				tvis.hParent=TreeView_GetParent(hTree,tvis.hParent);
			}
			hitem=TreeView_InsertItem(hTree,(LPARAM)&tvis);
			tvis.hParent=hitem;
		} else {
			hitem=TreeView_InsertItem(hTree,(LPARAM)&tvis);
		}
	}
	idx=pages[iTab];
	showValue();
	return i;
}

int CParamList::getIndex()
{
	TVITEM item;

	item.hItem=TreeView_GetSelection(hTree);
	item.mask=TVIF_PARAM;
	if (!TreeView_GetItem(hTree,&item)) return 0;
	return item.lParam;
}

int CParamList::browseForFile(TCHAR* buffer)
{
	OPENFILENAME ofn;
	int ret;
	TCHAR curdir[MAX_PATH];

	GetCurrentDirectory(sizeof(curdir)-1,curdir);

	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = params[idx].max;
	ofn.nFilterIndex = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lpstrFilter = (LPCSTR)params[idx].pchEnums;
	
	ofn.lpstrTitle = params[idx].pchParamName;
	ofn.Flags = OFN_CREATEPROMPT;
	ret=GetOpenFileName(&ofn);
	SetCurrentDirectory(curdir);
	return ret;
}

int CParamList::browseForPath(TCHAR* buffer)
{
	LPITEMIDLIST idlist;
	BROWSEINFO udtBI;

	ZeroMemory(&udtBI,sizeof(BROWSEINFO));
	udtBI.hwndOwner=0;
	udtBI.lpszTitle=params[idx].pchParamName;
	udtBI.ulFlags= BIF_RETURNONLYFSDIRS;
	idlist=SHBrowseForFolder(&udtBI);
	if (!idlist || !SHGetPathFromIDList(idlist,buffer))
		return 0;
	CoTaskMemFree(idlist);
	while(*(++buffer));
	if (*(buffer-1)!='\\') {
		*buffer='\\';
		*(buffer+1)=0;
	}
	return 1;
}

bool CParamList::onCreate()
{
	RECT rcDialog;
	RECT rc;
	GetWindowRect(hwnd, &rcDialog);
	GetWindowRect(GetHandle(IDC_EDIT), &rc);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	iBaseLine = rc.top - rcDialog.top + 1;

	hTree=GetHandle(IDC_TREE);
	refresh();
	showVariable(idx);
	Show();
	return true;
}

bool CParamList::onClose()
{
	if (notify) (*notify)(-1,0);
	EndDialog(hwnd, 0);
	return true;
}

int CParamList::open(HINSTANCE hInst, PARAM_DATA *params, PARAM_NOTIFY notify, PARAM_DESC *info, int varidx, HWND hwndParent)
{
	this->params=params;
	this->notify=notify;
	this->info=info;
	idx = varidx;
	iTab=0;
	iPageCount=0;
	pages=0;
	return ShowDlg(hInst, IDD_PARAM, hwndParent, 0);
}