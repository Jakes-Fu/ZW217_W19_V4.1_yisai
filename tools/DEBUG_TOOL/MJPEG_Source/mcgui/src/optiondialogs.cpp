/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#include "config.h"
#include <commctrl.h>
#include "atlstr.h"
#include "resource.h"
#include "menc.h"
#include "optiondialogs.h"
#include "lang.h"

void RefreshGui();
int ParamNotify(int idx, void* value);
void ListEnums(int idx, HWND hCtrl, UINT msg);
T_ITEM* GetSelectedItem();

extern COptionDialog* optionDialog[];
extern CMenc menc;

CDlgVideoOption::CDlgVideoOption(HWND hwndParent)
{
	CreateDlg(0, IDD_VIDEO, hwndParent);
	ListEnums(PI_VCODEC,GetHandle(IDC_CODECS),CB_ADDSTRING);
//@ Liu Kai 
//	ListEnums(PI_XVID_PROFILE,GetHandle(IDC_CODECS),CB_ADDSTRING);
	ListEnums(PI_SCALE_PRESET,GetHandle(IDC_SCALE_PRESET),CB_ADDSTRING);
	ListEnums(PI_FPS,GetHandle(IDC_FPS),CB_ADDSTRING);
	ListEnums(PI_ROTATE,GetHandle(IDC_ROTATE),CB_ADDSTRING);
}

bool CDlgVideoOption::onClick(int dlgid)
{
	switch (dlgid) {
	case IDC_OPT_FIXRATE:
		mcSetPrefValue(PI_VMODE,0);
		break;
	case IDC_OPT_TWOPASS:
		mcSetPrefValue(PI_VMODE,(void*)2);
		break;
	case IDC_OPT_THREEPASS:
		mcSetPrefValue(PI_VMODE,(void*)3);
		break;
	case IDC_OPT_FIXQUANT:
		mcSetPrefValue(PI_VMODE,(void*)1);
		break;
	case IDC_DO_SCALE:
		mcSetPrefValueInt(PI_SCALE, SendDlgMsg(IDC_DO_SCALE,BM_GETCHECK,0,0) == BST_CHECKED ? 1 : 0);
		break;
	case IDC_DO_CROP:
		mcSetPrefValueInt(PI_CROP, SendDlgMsg(IDC_DO_CROP,BM_GETCHECK,0,0) == BST_CHECKED ? 1 : 0);
		break;
	case IDC_EASY_CROP:
        {
            T_ITEM* item = GetSelectedItem();
            if (item && cropper.Open(this, item)) {
                MsgBox(IDS_UNABLE_CROP, 0);
            }
        }
		break;
    case IDC_SP:
        mcSetPrefValueInt(PI_CONTAINER, SendDlgMsg(IDC_SP,BM_GETCHECK,0,0) == BST_CHECKED ? CF_SPMP4 : CF_MP4);
        break;
	}
	Refresh();
	RefreshGui();
	return true;
}

bool CDlgVideoOption::onSelChange(int dlgid)
{
	int i;
	switch (dlgid) {
	case IDC_CODECS:
		i = SendDlgMsg(IDC_CODECS,CB_GETCURSEL,0,0);
		if (i >= 3) {
			mcSetPrefValueInt(PI_XVID_PROFILE, i - 1);
			i = 1;
			mcSetPrefValueInt(PI_VCODEC, 1);
		} else {
			mcSetPrefValueInt(PI_XVID_PROFILE, 0);
			mcSetPrefValueInt(PI_VCODEC, i);
		}
		break;
	case IDC_FPS:
		mcSetPrefValueInt(PI_FPS, SendDlgMsg(IDC_FPS,CB_GETCURSEL,0,0));
		break;
	case IDC_SCALE_PRESET:
		mcSetPrefValueInt(PI_SCALE_PRESET, SendDlgMsg(IDC_SCALE_PRESET,CB_GETCURSEL,0,0));
		break;
	case IDC_ROTATE:
		mcSetPrefValueInt(PI_ROTATE, SendDlgMsg(IDC_ROTATE,CB_GETCURSEL,0,0));
		break;
	}
	Refresh();
	RefreshGui();
	return true;
}

bool CDlgVideoOption::onCommand(int dlgid, int code)
{
	switch (dlgid) {
	case IDC_SCALE_PRESET:
		if (code == CBN_EDITCHANGE) {
			TCHAR buf[16], *p;
			SendDlgMsg(IDC_SCALE_PRESET, WM_GETTEXT, sizeof(buf) - 1, (LPARAM)buf);
			p = strchr(buf, 'x');
			if (!p) p = strchr(buf, 'X');
			if (!p) p = strchr(buf, ':');
			if (p && *(p + 1)) {
				*p = 0;
                //@Liukai 
				//mcSetPrefValueInt(PI_SCALE_PRESET, 0);
                // Customize width and height
                mcSetPrefValueInt(PI_SCALE_PRESET, -1);
				mcSetPrefValueInt(PI_SCALE_WIDTH, atoi(buf));
				mcSetPrefValueInt(PI_SCALE_HEIGHT, atoi(p + 1));
			}
		}
		break;
	case IDC_CROP_PARAM: {
		TCHAR buf[64];
		TCHAR *p;
		SendDlgMsg(IDC_CROP_PARAM, WM_GETTEXT, sizeof(buf), (LPARAM)buf);
		if (!(p = strtok(buf, ":"))) break;
		mcSetPrefValueInt(PI_CROP_LEFT, atoi(p));
		if (!(p = strtok(0, ":"))) break;
		mcSetPrefValueInt(PI_CROP_TOP, atoi(p));
		if (!(p = strtok(0, ":"))) break;
		mcSetPrefValueInt(PI_CROP_WIDTH, atoi(p));
		if (!(p = strtok(0, ":"))) break;
		mcSetPrefValueInt(PI_CROP_HEIGHT, atoi(p));
		} break;
	}
	return true;
}

bool CDlgVideoOption::onScroll(int dlgid, int code)
{
	TCHAR buf[16];
	int pos=SendDlgMsg(dlgid,TBM_GETPOS,0,0);
	if (mcGetPrefValueInt(PI_VMODE)==1) {
		mcSetPrefValue(PI_VQUALITY, (void*)pos);
		sprintf(buf,"%d",pos);
	} else {
		mcSetPrefValue(PI_VBITRATE, (void*)pos);
		sprintf(buf,"%d Kbps",pos);
	}
	SendDlgMsg(IDC_VIDEO_BITRATE,WM_SETTEXT,0,(LPARAM)buf);
	if (code == SB_THUMBPOSITION)
		RefreshGui();
	return true;
}

void CDlgVideoOption::Refresh()
{
	int checked;
	CString text;
	int i;
	SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETPOS,0,0);
	if (mcGetPrefValueInt(PI_VMODE)==1) {
		// fixed quant
		SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETRANGE,0,MAKELPARAM(1,100));
		SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETPAGESIZE,0,10);
		SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETPOS,TRUE,mcGetPrefValueInt(PI_VQUALITY));
	} else {
		//SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETRANGE,0,MAKELPARAM(0,2048)); @richardguan
		//@ Liu Kai 
		//SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETRANGE,0,MAKELPARAM(0,5120));
		SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETRANGE,0,MAKELPARAM(0,768));

		SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETPAGESIZE,0,256);
		SendDlgMsg(IDC_SLIDER_VIDEO,TBM_SETPOS,TRUE,mcGetPrefValueInt(PI_VBITRATE));
	}
	int vcodec = mcGetPrefValueInt(PI_VCODEC);
	switch (vcodec) {
	case VC_NONE:
		SendDlgMsg(IDC_CODECS,CB_SETCURSEL,0,0);
		SendDlgMsg(IDC_VIDEO_BITRATE,WM_SETTEXT,0,0);
		break;
	case VC_XVID:
		i = mcGetPrefValueInt(PI_XVID_PROFILE);
		if (i == 0) {
			SendDlgMsg(IDC_CODECS,CB_SETCURSEL,1,0);
		} else {
			SendDlgMsg(IDC_CODECS,CB_SETCURSEL,i + 2,0);
		}
		EnableWindow(GetHandle(IDC_SLIDER_VIDEO), TRUE);
		onScroll(IDC_SLIDER_VIDEO,0);
		break;
	case VC_MJPEG:
		SendDlgMsg(IDC_CODECS,CB_SETCURSEL,2,0);
		EnableWindow(GetHandle(IDC_SLIDER_VIDEO), FALSE);
		SendDlgMsg(IDC_VIDEO_BITRATE,WM_SETTEXT,0,0);
		break;
	}
	{
		int i,items[]={IDC_OPT_FIXRATE,IDC_OPT_FIXQUANT,IDC_OPT_TWOPASS,IDC_OPT_THREEPASS};
		for (i=0;i<4;i++) SendDlgMsg(items[i],BM_SETCHECK,BST_UNCHECKED,0);
		SendDlgMsg(items[mcGetPrefValueInt(PI_VMODE)],BM_SETCHECK,BST_CHECKED,0);
	}
	i = mcGetPrefValueInt(PI_SCALE_PRESET);
	if (i > 0) {
		SendDlgMsg(IDC_SCALE_PRESET,CB_SETCURSEL,i,0);
    } else if( i == 0 ){
        //@ Liukai 2007-7-27
#ifndef LANG_ZH
        CString strOriginal = "Original";
#else
        CString strOriginal = "²»¸Ä±ä";
#endif
        TCHAR szText[50];
        SendDlgMsg( IDC_SCALE_PRESET,CB_GETLBTEXT,i,(LPARAM)szText );
        if( strOriginal.Compare( szText ) )
        {
            SendDlgMsg(IDC_SCALE_PRESET,CB_SETCURSEL,i,0);
        }
        else
        {
            int w = mcGetPrefValueInt(PI_SCALE_WIDTH);
            int h = mcGetPrefValueInt(PI_SCALE_HEIGHT);
            if (!w || !h) {
                SendDlgMsg(IDC_SCALE_PRESET,CB_SETCURSEL,0,0);
            } else {
                text.Format("%dx%d", mcGetPrefValueInt(PI_SCALE_WIDTH), mcGetPrefValueInt(PI_SCALE_HEIGHT));
                SendDlgMsg(IDC_SCALE_PRESET,WM_SETTEXT,0,(LPARAM)text.GetBuffer());
            }
        }
	}
    else
    {
        //Customize height and width
        int w = mcGetPrefValueInt(PI_SCALE_WIDTH);
        int h = mcGetPrefValueInt(PI_SCALE_HEIGHT);
        if (!w || !h) {
            SendDlgMsg(IDC_SCALE_PRESET,CB_SETCURSEL,0,0);
        } else {
            text.Format("%dx%d", w,h);
            SendDlgMsg(IDC_SCALE_PRESET,WM_SETTEXT,0,(LPARAM)text.GetBuffer());
        }
    }
	SendDlgMsg(IDC_FPS,CB_SETCURSEL,(WPARAM)mcGetPrefValueInt(PI_FPS),0);
	SendDlgMsg(IDC_ROTATE,CB_SETCURSEL,(WPARAM)mcGetPrefValueInt(PI_ROTATE),0);
	checked = mcGetPrefValueInt(PI_SCALE);
	SendDlgMsg(IDC_DO_SCALE,BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED,0);
	EnableWindow(GetHandle(IDC_SCALE_PRESET), checked);
	checked = mcGetPrefValueInt(PI_CROP);
	SendDlgMsg(IDC_DO_CROP,BM_SETCHECK,checked ? BST_CHECKED : BST_UNCHECKED,0);
	EnableWindow(GetHandle(IDC_CROP_PARAM), checked);
	text.Format("%d:%d:%d:%d",
		mcGetPrefValueInt(PI_CROP_LEFT),
		mcGetPrefValueInt(PI_CROP_TOP),
		mcGetPrefValueInt(PI_CROP_WIDTH),
		mcGetPrefValueInt(PI_CROP_HEIGHT));
	SendDlgMsg(IDC_CROP_PARAM, WM_SETTEXT, 0, (LPARAM)text.GetBuffer());
    SendDlgMsg(IDC_SP,BM_SETCHECK, mcGetPrefValueInt(PI_CONTAINER) != CF_MP4 ? BST_CHECKED : BST_UNCHECKED,0);
}

static int srate[] = {8000, 11025, 16000, 22050, 32000, 44100, 48000, 0};
static int volgain[] = {-200, -60, -40, -20, -10, -6, -3, -1, 0, 1, 3, 6, 10, 20, 40, 60};
#define GAIN_OPT_COUNT 15

CDlgAudio::CDlgAudio(HWND hwndParent)
{
	CreateDlg(0, IDD_AUDIO, hwndParent);
	ListEnums(PI_AENC, GetHandle(IDC_ENCODER), CB_ADDSTRING);
	SendDlgMsg(IDC_RESAMPLE, CB_ADDSTRING, 0, (LPARAM)lngOriginal);
	for (int i = 0; srate[i]; i++) {
		char buf[16];
		sprintf(buf, "%d Hz", srate[i]);
		SendDlgMsg(IDC_RESAMPLE, CB_ADDSTRING, 0, (LPARAM)buf);
	}
	ListEnums(PI_DOWNMIX, GetHandle(IDC_DOWNMIX2), CB_ADDSTRING);
	for (i = 0; i < GAIN_OPT_COUNT; i++) {
		TCHAR buf[8];
		sprintf(buf, "%d dB", volgain[i]);
		SendDlgMsg(IDC_VOLUME, CB_ADDSTRING, 0, (LPARAM)buf);
	}
}

bool CDlgAudio::onClick(int dlgid)
{
	switch (dlgid) {
	case IDC_DECOPTS:
		mcShowConfigDialog(CD_AUDIO_SRC, ParamNotify);
		break;
	case IDC_ENCOPTS:
		mcShowConfigDialog(CD_AUDIO_ENC, ParamNotify);
		break;
	}
	Refresh();
	return true;
}

bool CDlgAudio::onSelChange(int dlgid)
{
	int i;
	switch (dlgid) {
	case IDC_ENCODER:
		i = SendDlgMsg(IDC_ENCODER,CB_GETCURSEL,0,0);
		mcSetPrefValue(PI_AENC, (void*)i);
		menc.ShowAudioDialog(mcGetPrefValueInt(PI_AENC));
		break;
	case IDC_RESAMPLE:
		i = SendDlgMsg(IDC_RESAMPLE,CB_GETCURSEL,0,0);
		mcSetPrefValue(PI_RESAMPLE_FREQ, (void*)(i == 0 ? 0 : srate[i - 1]));
		break;
	case IDC_DOWNMIX2:
		mcSetPrefValue(PI_DOWNMIX, (void*)SendDlgMsg(IDC_DOWNMIX2,CB_GETCURSEL,0,0));
		break;
	case IDC_VOLUME:
		mcSetPrefValueFloat(PI_VOL_GAIN, (float)volgain[SendDlgMsg(IDC_VOLUME,CB_GETCURSEL,0,0)]);
		break;
	default:
		return false;
	}
	Refresh();
	RefreshGui();
	return true;
}

void CDlgAudio::Refresh()
{
	SendDlgMsg(IDC_DOWNMIX2,CB_SETCURSEL,mcGetPrefValueInt(PI_DOWNMIX),0);
	SendDlgMsg(IDC_ENCODER,CB_SETCURSEL,mcGetPrefValueInt(PI_AENC),0);
	switch (mcGetPrefValueInt(PI_AENC)) {
	case AC_FAAC:
		SendDlgMsg(IDC_BITRATE_SLIDER,TBM_SETRANGE,0,MAKELPARAM(16,256));
		SendDlgMsg(IDC_BITRATE_SLIDER,TBM_SETPAGESIZE,0,16);
		SendDlgMsg(IDC_BITRATE_SLIDER,TBM_SETPOS,TRUE,mcGetPrefValueInt(PI_FAAC_BITRATE));
		EnableWindow(GetHandle(IDC_BITRATE_SLIDER), TRUE);
		break;
	case AC_AMR:
		SendDlgMsg(IDC_BITRATE_SLIDER,TBM_SETPOS,FALSE,0);
		SendDlgMsg(IDC_BITRATE_SLIDER,TBM_SETRANGE,0,MAKELPARAM(0,7));
		SendDlgMsg(IDC_BITRATE_SLIDER,TBM_SETPAGESIZE,0,1);
		SendDlgMsg(IDC_BITRATE_SLIDER,TBM_SETPOS,TRUE,mcGetPrefValueInt(PI_AMR_MODE));
		EnableWindow(GetHandle(IDC_BITRATE_SLIDER), TRUE);
		break;
	default:
		EnableWindow(GetHandle(IDC_BITRATE_SLIDER), FALSE);
	}
	int n = mcGetPrefValueInt(PI_RESAMPLE_FREQ);
	if (n) {
		int i;
		for (i = 0; srate[i] && srate[i] < n; i++);
		if (srate[i]) SendDlgMsg(IDC_RESAMPLE,CB_SETCURSEL,i + 1,0);	
	} else {
		SendDlgMsg(IDC_RESAMPLE,CB_SETCURSEL,0,0);
	}
	onScroll(IDC_BITRATE_SLIDER, 0);

	float gain = mcGetPrefValueFloat(PI_VOL_GAIN);
	SendDlgMsg(IDC_VOLUME, CB_SETCURSEL, 0, 0);
	for (int i = GAIN_OPT_COUNT - 1; i > 0; i--) {
		if ((volgain[i - 1] < gain && gain <= volgain[i])) {
			SendDlgMsg(IDC_VOLUME, CB_SETCURSEL, i, 0);
			break;
		}
	}
	EnableWindow(GetHandle(IDC_VOLUME), !mcGetPrefValueInt(PI_VOLNORM));
}

bool CDlgAudio::onScroll(int dlgid, int code)
{
	TCHAR buf[16];
	int pos=SendDlgMsg(dlgid,TBM_GETPOS,0,0);
	switch (mcGetPrefValueInt(PI_AENC)) {
	case AC_FAAC:
		mcSetPrefValue(PI_FAAC_BITRATE, (void*)pos);
		sprintf(buf,"%d Kbps",pos);
		break;
	case AC_AMR:
		mcSetPrefValue(PI_AMR_MODE, (void*)pos);
		sprintf(buf,"%d Kbps",pos <= 6 ? pos + 4 : pos + 5);
		break;
	default:
		buf[0] = 0;
	}
	SendDlgMsg(IDC_BITRATE,WM_SETTEXT,0,(LPARAM)buf);
	return true;
}

CDlgSummary::CDlgSummary(HWND hwndParent)
{
	CreateDlg(0, IDD_SUMMARY, hwndParent);
}

void CDlgSummary::Refresh()
{
	CString text;
	int video = mcGetPrefValueInt(PI_VCODEC);
	int audio = mcGetPrefValueInt(PI_AENC);
	T_ITEM* item = GetSelectedItem();
	TVINSERTSTRUCT tvis;
	TCHAR buf[256];

	HWND hTree=GetHandle(IDC_TREE);
	ShowWindow(hTree, SW_HIDE);
	TreeView_DeleteAllItems(hTree);
	memset(&tvis, 0, sizeof(tvis));
	tvis.item.mask=TVIF_TEXT|TVIF_STATE;
	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.pszText = buf;
	tvis.item.state = TVIS_EXPANDED;
	tvis.item.stateMask = TVIS_EXPANDED;

	if (item) {
		if (item->info.container) {
			_snprintf(buf, sizeof(buf), "%s: %s", lngContainer, item->info.container);
			tvis.hParent = TreeView_InsertItem(hTree,(LPARAM)&tvis);
			if (item->info.duration) {
				_snprintf(buf, sizeof(buf), "%s: %d.%d sec(s)", lngDuration, item->info.duration / 1000, (item->info.duration / 100) % 10);
				TreeView_InsertItem(hTree,(LPARAM)&tvis);
			}
			tvis.hParent = TreeView_GetParent(hTree, tvis.hParent);
		}
		if (item->info.video.width && item->info.video.height) {
			_snprintf(buf, sizeof(buf), "%s: %s", lngVideo, item->info.video.codec ? item->info.video.codec : (item->info.container ? item->info.container : ""));
			tvis.hParent = TreeView_InsertItem(hTree,(LPARAM)&tvis);
			if (item->info.video.bitrate) {
				_snprintf(buf, sizeof(buf), "%s: %d Kbps", lngBitrate, item->info.video.bitrate / 1000);
				TreeView_InsertItem(hTree,(LPARAM)&tvis);
			}
			if (item->info.video.width && item->info.video.height) {
				_snprintf(buf, sizeof(buf), "%s: %dx%d", lngRes, item->info.video.width, item->info.video.height);
				TreeView_InsertItem(hTree,(LPARAM)&tvis);
			}
			if (item->info.video.fps) {
				_snprintf(buf, sizeof(buf), "%s: %.2f fps", lngFPS, item->info.video.fps);
				TreeView_InsertItem(hTree,(LPARAM)&tvis);
			}
			tvis.hParent = TreeView_GetParent(hTree, tvis.hParent);
		}
		if (item->info.audio.codec) {
			_snprintf(buf, sizeof(buf), "%s: %s", lngAudio, item->info.audio.codec);
			tvis.hParent = TreeView_InsertItem(hTree,(LPARAM)&tvis);
			if (item->info.audio.bitrate) {
				_snprintf(buf, sizeof(buf), "%s: %d Kbps", lngBitrate, item->info.audio.bitrate / 1000);
				TreeView_InsertItem(hTree,(LPARAM)&tvis);
			}
			if (item->info.audio.bitratemode) {
				_snprintf(buf, sizeof(buf), "%s: %s", lngBitrateMode, item->info.audio.bitrate == 1 ? "CBR" : "VBR");
				TreeView_InsertItem(hTree,(LPARAM)&tvis);
			}
			if (item->info.audio.samplerate) {
				_snprintf(buf, sizeof(buf), "%s: %d Hz", lngSRate, item->info.audio.samplerate);
				TreeView_InsertItem(hTree,(LPARAM)&tvis);
			}
			if (item->info.audio.channels) {
				_snprintf(buf, sizeof(buf), "%s: %d", lngChannel, item->info.audio.channels);
				TreeView_InsertItem(hTree,(LPARAM)&tvis);
			}
		}
	}
	tvis.hParent = TVI_ROOT;

	ShowWindow(hTree, SW_SHOW);
}

bool CCropper::onCreate()
{
	SendDlgMsg(IDC_SLIDER_X1,TBM_SETRANGE,TRUE,MAKELPARAM(0,vw));
	SendDlgMsg(IDC_SLIDER_Y1,TBM_SETRANGE,TRUE,MAKELPARAM(0,vh));
	SendDlgMsg(IDC_SLIDER_X2,TBM_SETRANGE,FALSE,MAKELPARAM(0,vw));
	SendDlgMsg(IDC_SLIDER_X2,TBM_SETPOS,TRUE,vw);
	SendDlgMsg(IDC_SLIDER_Y2,TBM_SETRANGE,FALSE,MAKELPARAM(0,vh));
	SendDlgMsg(IDC_SLIDER_Y2,TBM_SETPOS,TRUE,vh);
	MoveWindow(GetHandle(IDC_SLIDER_X1), 22, 30, vw + 20, 20, TRUE);
	MoveWindow(GetHandle(IDC_SLIDER_Y1), 0, 50, 20, vh + 20, TRUE);
	MoveWindow(GetHandle(IDC_SLIDER_X2), 22, vh + 70, vw + 20, 20, TRUE);
	MoveWindow(GetHandle(IDC_SLIDER_Y2), vw + 38, 50, 20, vh + 20, TRUE);

	RECT rc = {0, 0, vw < 280 ? 345 : vw + 65, vh + 125};
	AdjustWindowRect(&rc, WS_DLGFRAME , FALSE);
	Move(0, 0, rc.right - rc.left, rc.bottom - rc.top);

	for (int i = 1; i <= 32; i <<= 1) {
		char buf[4];
		sprintf(buf, "%d", i);
		SendDlgMsg(IDC_ROUND_VALUE, CB_ADDSTRING, 0, (LPARAM)buf);
	}
	SendDlgMsg(IDC_ROUND_VALUE, CB_SETCURSEL, 2, 0);
	round = 4;
	return true;
}

bool CCropper::onCommand(int dlgid, int code)
{
	return false;
}

bool CCropper::onSelChange(int dlgid)
{
	TCHAR buf[4];
	int i = SendDlgMsg(IDC_ROUND_VALUE, CB_GETCURSEL, 0, 0);
	SendDlgMsg(IDC_ROUND_VALUE, CB_GETLBTEXT, i, (LPARAM)buf);
	round = atoi(buf);
	Adjust();
	return true;
}

bool CCropper::onScroll(int dlgid, int code)
{
	if (code == SB_THUMBPOSITION) return false;
	int pos = SendDlgMsg(dlgid,TBM_GETPOS,0,0);
	switch (dlgid) {
	case IDC_SLIDER_X1:
		x1 = pos;
		Adjust();
		SendDlgMsg(IDC_SLIDER_X1,TBM_SETPOS,TRUE,x1);
		SendDlgMsg(IDC_SLIDER_X2,TBM_SETPOS,TRUE,vw - x2);
		break;
	case IDC_SLIDER_X2:
		x2 = vw - pos;
		Adjust();
		SendDlgMsg(IDC_SLIDER_X1,TBM_SETPOS,TRUE,x1);
		SendDlgMsg(IDC_SLIDER_X2,TBM_SETPOS,TRUE,vw - x2);
		break;
	case IDC_SLIDER_Y1:
		y1 = pos;
		Adjust();
		SendDlgMsg(IDC_SLIDER_Y1,TBM_SETPOS,TRUE,y1);
		SendDlgMsg(IDC_SLIDER_Y2,TBM_SETPOS,TRUE,vh - y2);
		break;
	case IDC_SLIDER_Y2:
		y2 = vh - pos;
		Adjust();
		SendDlgMsg(IDC_SLIDER_Y1,TBM_SETPOS,TRUE,y1);
		SendDlgMsg(IDC_SLIDER_Y2,TBM_SETPOS,TRUE,vh - y2);
		break;
	default:
		return false;
	}
	return true;
}

bool CCropper::onClick(int dlgid)
{
	switch (dlgid) {
	case IDC_PLAY:
		EnableWindow(GetHandle(IDC_PLAY), FALSE);
		mcPlayerStop();
		Sleep(1000);
		mcPlayerStart(item, wndVideo.hwnd, &hVideo, TRUE ,"-nokeepaspect");
		Adjust();
		Sleep(1000);
		EnableWindow(GetHandle(IDC_PLAY), TRUE);
		break;
	case IDOK:
		mcSetPrefValueInt(PI_CROP_LEFT, x1);
		mcSetPrefValueInt(PI_CROP_TOP, y1);
		mcSetPrefValueInt(PI_CROP_WIDTH, vw - x2 - x1);
		mcSetPrefValueInt(PI_CROP_HEIGHT, vh - y2 -y1);
		RefreshGui();
		Close();
		break;
	}
	return true;
}

int CCropper::Open(CWindow* wndParent, T_ITEM *item)
{
	vw = item->info.video.width;
	vh = item->info.video.height;
	if (vw == 0 && vh == 0) return -1;
	x1 = mcGetPrefValueInt(PI_CROP_LEFT);
	y1 = mcGetPrefValueInt(PI_CROP_TOP);
	x2 = 0;
	y2 = 0;
	int i;
	if (i = mcGetPrefValueInt(PI_CROP_WIDTH))
		x2 = vw - x1 - i;
	if (i = mcGetPrefValueInt(PI_CROP_HEIGHT))
		y2 = vh - y1 - i;
	this->item = item;

	CreateDlg(0, IDD_CROP, wndParent);
	if (!hwnd) return 0;
	EnableWindow(wndParent->hwnd, FALSE);

	WNDCLASS   wc;
	HBRUSH colorbrush = CreateSolidBrush(0);
	memset(&wc, 0, sizeof(wc));
	wc.style         =  CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.hbrBackground =  colorbrush;
	wc.lpszClassName =  "CropWindow";
	wndVideo.Create(WS_CHILD|WS_VISIBLE, 0, hwnd, wc);

	if (mcPlayerStart(item, wndVideo.hwnd, &hVideo, TRUE, "-nokeepaspect")) return -1;
	Adjust();
	return 0;
}

void CCropper::Adjust()
{
	if (x2 < 0) x2 = 0;
	if (y2 < 0) y2 = 0;
	int cw = vw - x1 - x2;
	int ch = vh - y1 - y2;
	if (round > 1) {
		cw = (cw + round - 1) & (unsigned int)(-round);
		ch = (ch + round - 1) & (unsigned int)(-round);
	}
	if (cw < 0) cw = round;
	if (ch < 0) ch = round;
	x2 = vw - x1 - cw;
	if (x2 < 0) {
		x2 = 0;
		x1 = vw - cw;
	}
	y2 = vh - y1 - ch;
	if (y2 < 0) {
		y2 = 0;
		y1 = vh - ch;
	}
	if (vw - x1 - x2 <= 0) {
		if (x1 >= round)
			x1 -= round;
		else
			x2 -= round;
	}
	if (vh - y1 - y2 <= 0) {
		if (y1 >= round)
			y1 -=round;
		else
			y2 -=round;
	}
	CString param;
	param.Format("%d:%d:%d:%d", x1, y1, vw - x2 - x1, vh - y2 - y1);
	SendDlgMsg(IDC_CROP_PARAM, WM_SETTEXT, 0, (LPARAM)param.GetBuffer());
	MoveWindow(hVideo, -x1, -y1, vw, vh, 1);
	wndVideo.Move(30 + x1, 60 + y1, vw - x2 - x1, vh - y2 - y1);
	SendDlgMsg(IDC_SLIDER_X1,TBM_SETPOS,TRUE,x1);
	SendDlgMsg(IDC_SLIDER_X2,TBM_SETPOS,TRUE,vw - x2);
	SendDlgMsg(IDC_SLIDER_Y1,TBM_SETPOS,TRUE,y1);
	SendDlgMsg(IDC_SLIDER_Y2,TBM_SETPOS,TRUE,vh - y2);
}

bool CCropper::onClose()
{
	mcPlayerStop();
	wndVideo.Destroy();
	UnregisterClass("CropWindow", hInstance);
	EnableWindow(wndParent->hwnd, TRUE);
	return false;
}

static bool playing = false;

T_ITEM* GetSelectedItem();

CDlgPreview::CDlgPreview(HWND hwndParent)
{
	CreateDlg(0, IDD_PREVIEW, hwndParent);
	SetControlState(0);
	osd = 0;
}

#define PREVIEW_WIDTH 160
#define PREVIEW_HEIGHT 120

void CDlgPreview::SetControlState(int state)
{
	EnableWindow(GetHandle(IDC_PLAY), !state);
	EnableWindow(GetHandle(IDC_STOP), state);
	EnableWindow(GetHandle(IDC_PAUSE), state);
	EnableWindow(GetHandle(IDC_SLIDER), state);
	if (state == 0) {
		sliderMax = 0;
		step = 0;
		KillTimer(hwnd, 0);
		playing = false;
	} else {
		SendDlgMsg(IDC_SLIDER,TBM_SETRANGE,TRUE,MAKELPARAM(0,sliderMax));
		if (sliderMax) SetTimer(hwnd, 0, 1000, 0);
	}
	SendDlgMsg(IDC_SLIDER, TBM_SETPOS, TRUE, 0);
}

void CDlgPreview::SetOsd(int level)
{
	char buf[8];
	sprintf(buf, "osd %d", level);
	mcPlayerCommand(buf);
	osd = level;
}

bool CDlgPreview::onClick(int dlgid)
{
	switch (dlgid) {
	case IDC_PLAY: {
		if (playing) {
			optionDialog[mode == 0 ? OD_PREVIEW2 : OD_PREVIEW]->onClick(IDC_STOP);
		}
		HWND hVideo = 0;
		T_ITEM* item = GetSelectedItem();
		if (item) {
			if (!mode) {
				if (mcPlayerStart(item, GetHandle(IDC_PREVIEW_BOX), &hVideo, 0, "-nokeepaspect "))
					break;
			} else {
				if (mcPreviewStart(item, GetHandle(IDC_PREVIEW_BOX), &hVideo, "-nokeepaspect"))
					break;
			}
			SetOsd(osd);
			sliderMax = item->info.duration / 1000;
			step = 1;
			SetControlState(1);
		}
		if (hVideo) {
			playing = true;
			MoveWindow(hVideo, 0, 0, PREVIEW_WIDTH, PREVIEW_HEIGHT, 1);
		}
		} break;
	case IDC_STOP:
		mcPlayerStop();
		SetControlState(0);
		Sleep(500);
		break;
	case IDC_PAUSE:
		if (step) {
			SendDlgMsg(IDC_PAUSE, WM_SETTEXT, 0, (LPARAM)lngResume);
            step = 0;
		} else {
			SendDlgMsg(IDC_PAUSE, WM_SETTEXT, 0, (LPARAM)lngPause);
            step = 1;
		}
		mcPlayerCommand("pause");
		break;
	}
	return true;
}


bool CDlgPreview::onScroll(int dlgid, int code)
{
	int pos = SendDlgMsg(IDC_SLIDER,TBM_GETPOS,0,0);
	CString cmd;
	cmd.Format("seek %d 2", pos);
	mcPlayerCommand(cmd.GetBuffer());
	return true;
}

bool CDlgPreview::onTimer(int id)
{
	int pos = SendDlgMsg(IDC_SLIDER,TBM_GETPOS,0,0);
	if (step) {
		pos += step;
		if (mcPlayerCommand("get_time_pos")) {
			SetControlState(0);
		}
	}
	if (sliderMax > 0) {
		if (pos > sliderMax) {
			SetControlState(0);
		} else {
			SendDlgMsg(IDC_SLIDER, TBM_SETPOS, TRUE, pos);
		}
	}
	return true;
}
