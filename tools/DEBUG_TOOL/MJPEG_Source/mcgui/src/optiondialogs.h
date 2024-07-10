/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#ifndef VIDEO_OPTIONS_H
#define VIDEO_OPTIONS_H

typedef enum {
	OD_SUMMARY = 0,
	OD_AUDIO,
	OD_VIDEO,
	OD_PREVIEW,
	OD_PREVIEW2,
	OD_COUNT,
	OD_CONTAINER,
	OD_PICTURE,
} OPT_DIALOGS;

class COptionDialog:public CWindow
{
public:
	virtual TCHAR* GetName() = 0;
	virtual void Refresh() {}
};

class CCropper:public CWindow
{
public:
	int Open(CWindow* wndParent, T_ITEM *item);
private:
	bool onClose();
	bool onScroll(int dlgid, int code);
	bool onClick(int dlgid);
	bool onSelChange(int dlgid);
	bool onCommand(int dlgid, int code);
	bool onCreate();
	void Adjust();
	HWND hVideo;
	CWindow wndVideo;
	int vw;
	int vh;
	int x1;
	int y1;
	int x2;
	int y2;
	int round;
	T_ITEM *item;
};

class CDlgVideoOption:public COptionDialog
{
public:
	CDlgVideoOption(HWND hwndParent);
	TCHAR* GetName() {return "Video";}
	void Refresh();
private:
	bool onScroll(int dlgid, int code);
	bool onClick(int dlgid);
	bool onSelChange(int dlgid);
	bool onCommand(int dlgid, int code);
	CCropper cropper;
};

class CDlgAudio:public COptionDialog
{
public:
	CDlgAudio(HWND hwndParent);
	TCHAR* GetName() {return "Audio";}
	void Refresh();
private:
	bool onClick(int dlgid);
	bool onSelChange(int dlgid);
	bool onScroll(int dlgid, int code);
};

class CDlgSummary:public COptionDialog
{
public:
	CDlgSummary(HWND hwndParent);
	TCHAR* GetName() {return "Summury";}
	void Refresh();
private:
};

class CDlgPreview:public COptionDialog
{
public:
	CDlgPreview(HWND hwndParent);
	TCHAR* GetName() {return "Preview";}
	void SetOsd(int level);
	int mode;
private:
	bool onClick(int dlgid);
	bool onScroll(int dlgid, int code);
	bool onTimer(int id);
	void SetControlState(int mode);
	int sliderMax;
	int step;
	int osd;
};

#endif