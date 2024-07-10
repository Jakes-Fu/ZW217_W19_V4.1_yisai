/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*************************************************************************/

#include "config.h"
#include <windows.h>
#include "processpil.h"
#include "datatypes.h"
#include "transcode.h"
#include "preference.h"
#include "containers.h"
#include "common.h"

TCHAR* CMp4box::UpdateSettings(TCHAR* destfile, CONTAINER_TRACKS tracks[])
{
	int i, count = 0;
	static const TCHAR* argnames[] = {"packed", "mpeg4", "keepsys", "isma", "3gp", 0};
	static const int argindex[] = {PI_MP4_PACKED, PI_MP4_MPEG4, PI_MP4_KEEPSYS, PI_MP4_ISMA, PI_MP4_3GP};

	this->destfile = destfile;

	cmd.Format("\"%s\" -keepsys", pref.GetValueStr(PI_MP4BOX_PATH));
	if (mcinst.item->target.fps) {
		cmd.AppendFormat(" -fps %5.3f", mcinst.item->target.fps);
	} else if (mcinst.item->info.video.fps) {
		cmd.AppendFormat(" -fps %5.3f", mcinst.item->info.video.fps);
	}
	if (mcGetPrefValueInt(PI_VCODEC) == 2) {
		// MJPEG
		for (i = 0; i < MAX_TRACKS; i++) {
			switch (tracks[i].type) {
			case CT_VIDEO:
				cmd.AppendFormat(" \"%s\"", tracks[i].filename);
				videofile = tracks[i].filename;
				break;
			case CT_AUDIO:
				cmd.AppendFormat(" -add \"%s\"#audio", tracks[i].filename);
				break;
			default:
				continue;
			}
			count++;
		}
	} else {
		for (i = 0; i < MAX_TRACKS; i++) {
			if (!tracks[i].filename) continue;
			if (tracks[i].type == CT_AUDIO) {
				if (pref.GetValueBool(PI_MP4_SBRX))
					cmd += " -sbrx";
				else if (pref.GetValueBool(PI_MP4_SBR) || tracks[i].flags & FLAG_SBR_AAC)
					cmd += " -sbr";
				cmd.AppendFormat(" -add \"%s\"#audio", tracks[i].filename);
			} else if (tracks[i].type == CT_VIDEO) {
				cmd.AppendFormat(" -add \"%s\"#video", tracks[i].filename);
			} else {
				cmd.AppendFormat(" -add \"%s\"", tracks[i].filename);
			}
			if (tracks[i].delay) {
				cmd.AppendFormat(":delay=%d", tracks[i].delay);
			}
			count++;
		}
		for (i = 0; argnames[i]; i++) {
			if (pref.GetValueBool(argindex[i])) {
				cmd += " -";
				cmd += argnames[i];
			}
		}
		if (*pref.GetValueStr(PI_MP4BOX_OPTS)) {
			cmd += ' ';
			cmd += pref.GetValueStr(PI_MP4BOX_OPTS);
		}
		cmd.AppendFormat(" \"%s\"", destfile);
	}
	return count == 0 ? 0 : cmd.GetBuffer();
}

TCHAR* CMp4box::GetVersionInfo()
{
	return GetEncoderVersionInfo(pref.GetValueStr(PI_MP4BOX_PATH), "-version", "\n");
}

TCHAR* CMp4box::GetOutExt()
{
	return (mcinst.fAudioEncoding) ? "m4a" : "mp4";
}

int CMp4box::Mux()
{
	memset(&sp ,0, sizeof(sp));
	sp.pchCommandLine = cmd.GetBuffer();
	int ret = ShellExec(&sp);

	if (mcGetPrefValueInt(PI_VCODEC) == 2) {
		// MJPEG
		ShellWait(&sp, -1);
		MoveFile(videofile, destfile);
	}
	return ret;
}

int CMp4box::GetProgress()
{ 
	ShellWait(&sp, -1);
	ShellClean(&sp);
	return -1;
}

int CMp4box::ExtractTrack(TCHAR* srcfile, CString& destfile, int trackno)
{
	CString cmd;
	cmd.Format("\"%s\" \"%s\" -out \"%s\" -raw %d", 
		pref.GetValueStr(PI_MP4BOX_PATH),
		srcfile,
		destfile,
		trackno);
	memset(&sp ,0, sizeof(sp));
	sp.pchCommandLine = cmd.GetBuffer();
	ShellExec(&sp);
	ShellWait(&sp, -1);
	ShellClean(&sp);
	destfile.AppendFormat("_track%d.*", trackno);
	WIN32_FIND_DATA finddata;
	HANDLE hFind = FindFirstFile(destfile.GetBuffer(), &finddata);
	if (hFind != INVALID_HANDLE_VALUE) {
		destfile.Delete(destfile.GetLength() - 2, 2);
		destfile += strrchr(finddata.cFileName, '.');
		FindClose(hFind);
		return 0;
	} else {
		return -1;
	}
}

int CSpMp4::Mux()
{
	CString outpath;
	int i, j;
    CString outfile = destfile;

	i = outfile.ReverseFind('\\');
	j = outfile.ReverseFind('.');
	if (i >= 0) outpath = outfile.Left(i + 1);
	memset(&sp ,0, sizeof(sp));
	sp.pchCommandLine = cmd.GetBuffer();
	if (ShellExec(&sp)) return -1;
	ShellWait(&sp, -1);
	ShellClean(&sp);

	int w = pref.GetValueInt(PI_SCALE_WIDTH);
	int h = pref.GetValueInt(PI_SCALE_HEIGHT);
	if (pref.GetValueInt(PI_SCALE_PRESET)) {
		char *p = pref.GetValueEnum(PI_SCALE_PRESET);
		w = atoi(p);
		h = atoi(strchr(p, 'x') + 1);
	}

	memset(&sp ,0, sizeof(sp));
	cmd.Format(".\\tools\\GPConverter.exe -i %s -o %s -p %d*%d -s",
        outfile.GetBuffer(), outpath.GetBuffer(), w, h);
    sp.pchCommandLine = cmd.GetBuffer();
	pfnEvent(MC_MSG_CMD, sp.pchCommandLine);
	if (ShellExec(&sp)) return -1;
	ShellWait(&sp, -1);
	ShellClean(&sp);
	DeleteFile(outfile.GetBuffer());
	outpath += outfile.Mid(i, j - i);
	outpath += "_new";
	outpath += outfile.Mid(j);
	if (!FileExist(outpath.GetBuffer()) || 
		!MoveFile(outpath.GetBuffer(), outfile.GetBuffer())
		) return -1;
	return 0;
}

int CSpMp4::GetProgress()
{ 
	return -1;
}