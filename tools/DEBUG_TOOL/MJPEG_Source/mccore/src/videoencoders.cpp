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
#include "audioencoders.h"
#include "videoencoders.h"
#include "preference.h"

extern MC_STATE* pinst;
extern CAudioEncoder* AudioEncoders[];

int CMencoder::OpenEncoder(TCHAR* sourcefile, TCHAR* destfile)
{
	if (!fLoadDll) {
		sp = (SHELL_PARAM*)malloc(sizeof(SHELL_PARAM));
		memset(sp, 0, sizeof(SHELL_PARAM));
		if (!pref.GetValueBool(PI_SHOW_CONSOLE)) {
			sp->flags = SHELL_REDIRECT_STDOUT;
		}
		stats = (MP_STATS*)malloc(sizeof(MP_STATS));
	} else {
		sp = 0;
		dll = new CDllHelper;
		if (!dll->LoadDLL(pref.GetValueStr(PI_ME_PATH), ME_DLL)) return -1;
		stats = (MP_STATS*)GetProcAddress(dll->hDll, "mestats");
		if (!stats) {
			dll->Unload();
			return -1;
		}
	}
	memset(stats, 0, sizeof(MP_STATS));
	return 0;
}

void CMencoder::GetAudioOptions(CString& str)
{
	switch (mcinst.item->target.audioformat) {
	case AC_FAAC:
		str.AppendFormat(" -oac faac -faacopts %s=%d:mpeg=%c",
			pref.GetValueInt(PI_FAAC_MODE)?"br":"quality",
			pref.GetValueInt(pref.GetValueInt(PI_FAAC_MODE)?PI_FAAC_BITRATE:PI_FAAC_QUALITY),
			pref.GetValueInt(PI_FAAC_MPEGVER)?'4':'2');
		if (pref.GetValueBool(PI_FAAC_TNS)) str += ":tns";
		break;
	case AC_PCM:
		str += " -oac pcm";
		break;
	case AC_COPY:
		str += " -oac copy";
		break;
	default:
		str += " -nosound";
		return;
	}
	GenAudioFilterOptions(str, 0);
}

TCHAR* CMencoder::GetVersionInfo()
{
	CString path(pref.GetValueStr(PI_ME_PATH));
	path += "mencoder.exe";
	return GetEncoderVersionInfo(path.GetBuffer(), 0, "(C)");
}

int CMencoder::Encode(CString& cmd)
{
	if (!fLoadDll) {
		sp->pchCommandLine = cmd.GetBuffer();
		pfnEvent(MC_MSG_CMD, sp->pchCommandLine);
		if (ShellExec(sp)) return -1;
		SetProcessPriorityBySetting(sp->piProcInfo.hProcess);
	} else {
		if (dll->StartMain(cmd.GetBuffer())) return -1;
		SetThreadPriorityBySetting(dll->hThread);
	}
	return 0;
}

int CMencoder::CloseEncoder()
{
	if (!fLoadDll) {
		ShellTerminate(sp);
		ShellClean(sp);
		free(sp);
		stats = 0;
		free(stats);
		return 0;
	} else {
		dll->Stop();
		stats = 0;
		dll->Unload();
		delete dll;
		dll = 0;
	}
	return 0;
}

void CMencoder::ParseConsole(char* consoleText, int consoleBytes)
{
	TCHAR *ql = 0, *qr = 0, *p = consoleText + consoleBytes;
	TCHAR *s = 0;

	while (p>consoleText && *p!='\n') {
		if (*p=='(')
			ql = p;
		else if (*p==')')
			qr = p;
		else if (*(DWORD*)p == ':soP') {
			s = p;
			*(s - 1) = 0;
		}
		p--;
	}
	while (--p>=consoleText) {
		if (*p=='(')
			ql = p;
		else if (*p==')')
			qr = p;
		else if (*(DWORD*)p == ':soP') {
			s = p;
			break;
		}
	}
	if (!qr || !ql || !s) return;

	pfnEvent(MC_WRITE_CONSOLE, s);

	inst.vstats->percent = (float)atof(ql+1);

	do {
		p = strtok(s,":");
		if (!(p = strtok(0," "))) continue;
		inst.vstats->pos = atoi(p);
		if (!(p = strtok(0," "))) continue;
		inst.vstats->frame = atoi(p);
		if (!(p = strtok(qr + 2," "))) continue;
		inst.vstats->fps = (float)atof(p);
		p=strtok(0,"  ");
		if (!(p = strtok(0,"  "))) continue;
		inst.vstats->remaintime = atoi(p);
		if (!(p = strtok(0,"  "))) continue;
		inst.vstats->estsize = atoi(p) << 10;
		p=strtok(0,":");
		p=strtok(0," ");
		if (!(p = strtok(0,":"))) continue;
		p++;
		inst.vstats->vbitrate = atoi(p);
		if (!(p = strtok(0,"]"))) continue;
		inst.vstats->abitrate = atoi(p);
	} while (0);
}

int CMencoder::GetProgress()
{
	if (!fLoadDll) {
		if (sp->flags & SHELL_REDIRECT_STDOUT) {
			char buf[1024];
			int bytes;
			int bytessum = 0;
			while ( inst.state == STATE_ENCODING && bytessum < sizeof(buf) - 1) {
				if (!ReadFile(sp->fdRead, buf + bytessum, sizeof(buf)- 1 - bytessum, (LPDWORD)&bytes,0)) {
					return -1;
				}
                bytessum += bytes;
			}
			buf[bytessum] = 0;
			ParseConsole(buf, bytessum);
			return 0;
		} else {
			return (ShellWait(sp, 500)) ? -1 : 0;
		}
	} else {
		if (dll->Wait(500) != WAIT_OBJECT_0) {
			return -1;
		}
		return (int)stats->percent;
	}
}

static const char *lavcvcodecs[]={"mpeg1video","mpeg2video","mpeg4","msmpeg4v2","h261","h263","h263p","snow","msmpeg4","wmv1","wmv2","flv","dvvideo","mjpeg","ljpeg"};

int CMencoder::UpdateSettings()
{
	int i;
	TCHAR *p;
	CString mecmd;

	for (i = 0; i < MAX_PASS_COUNT; i++) cmds[i] = "";

	fLoadDll = pref.GetValueBool(PI_ME_DLL);
	mecmd.Format("\"%s%s\" \"$(SourceFile)\"",
		pref.GetValueStr(PI_ME_PATH),
		fLoadDll ? ME_DLL : ME_EXE);
	if (fLoadDll) {
		mecmd += " -really-quiet";
	}
	GenCommonOptions(mecmd, mcinst.item);
	if (mcinst.item->target.fps) {
		mecmd.AppendFormat(" -ofps %5.3f", mcinst.item->target.fps);
	}
	if (strcmp(GetOutExt(), "avi")) {
		mecmd += " -of rawvideo";
	}
	GenVideoFilterOptions(mecmd);
	if ((i = pref.GetValueInt(PI_STARTPOS)))
		mecmd.AppendFormat(" -ss %d", i);
	if (mcinst.item->info.end)
		mecmd.AppendFormat(" -endpos %d", mcinst.item->info.end / 1000);
	else if ((i = pref.GetValueInt(PI_ENDPOS)))
		mecmd.AppendFormat(" -endpos %d", i);

	if (pref.GetValueInt(PI_VMODE) > 1 || pref.GetValueBool(PI_AUTO_BITRATE)) {
		mecmd.AppendFormat(" -passlogfile \"$(TempDir)mcpass%d.log\"",
			GetCurrentProcessId());
	}
	switch (mcinst.item->target.videoformat) {
	case VC_XVID: {
		static const char* opts[] = {"interlacing", "qpel", "gmc", "trellis", "chroma_me", "chroma_opt", "hq_ac", "packed"};
		static const short int optid[] = {PI_XVID_INTERLACE, PI_XVID_QPEL, PI_XVID_GMC, PI_XVID_TRELLIS, PI_XVID_CM, PI_XVID_CO, PI_XVID_HQAC, PI_XVID_PACK, 0};
		static const char* profiles[] = {"sp0", "sp1", "sp2", "sp3", "asp0", "asp1", "asp2", "asp3", "asp4", "asp5", "dxnhandheld", "dxnportntsc", "dxnportpal", "dxnhtntsc", "dxnhtpal", "dxnhdtv"};
		CString fullargs;
		int profile = pref.GetValueInt(PI_XVID_PROFILE);
		mecmd += " -ovc xvid -xvidencopts ";
		mecmd.AppendFormat("max_key_interval=%d:", (int)mcinst.item->target.fps);
		if (pref.GetValueBool(PI_XVID_GREY)) mecmd += "greyscale:";
		if (pref.GetValueBool(PI_XVID_CARTOON)) mecmd += "cartoon:";
		if (*(p = pref.GetValueStr(PI_XVID_EXTRAOPTS))) {
			mecmd.AppendFormat("%s:", p);
		}
		if ((i = pref.GetValueInt(PI_XVID_THREADS)) > 1) {
			mecmd.AppendFormat("threads=%d:", i);
		}
		if (profile) {
			int paramfps = pref.GetValueInt(PI_FPS);
			int maxfps = 0;
			int scale = 0;
			mecmd.AppendFormat("profile=%s:", profiles[profile - 1]);
			switch (profile) {
			case 1:
			case 2:
				scale = 5;	//scale to 176x144
				maxfps = 6;	//15 fps
				break;
			case 3:
			case 4:
				scale = 1;	//scale to 352x288
				maxfps = 6;	//15 fps
				break;
			case 5:
			case 6:
				scale = 5;	//scale to 176x144
				break;
			case 8:
			case 9:
			case 10:
				scale = 1;	//scale to 352x288
			case 7:
				maxfps = 6;	//15 fps
				break;
			}
			if (maxfps && pref.GetValueBool(PI_AUTO_FPS) && (!paramfps || paramfps > maxfps)) pref.SetValue(PI_FPS, maxfps);
			if (scale && pref.GetValueBool(PI_AUTO_GEOMETRY)) {
				pref.SetValue(PI_SCALE, 1);
				pref.SetValue(PI_SCALE_PRESET, scale);
			}
		} else {
			fullargs.Format("max_bframes=%d:me_quality=%d:quant_type=%s:vhq=%d:",
				pref.GetValueInt(PI_XVID_BFRAME),
				pref.GetValueInt(PI_XVID_MQ),
				pref.GetValueInt(PI_XVID_QUANTTYPE)==0 ? "h263" : "mpeg",
				pref.GetValueInt(PI_XVID_VHQ)
			);
			for (i = 0; optid[i]; i++) {
				if (!pref.GetValueBool(optid[i])) fullargs += "no";
				fullargs += opts[i];
				fullargs += ':';
			}
		}

		switch (pref.GetValueInt(PI_VMODE)) {
		case 0:	// fixed bitrate
			if (pref.GetValueBool(PI_AUTO_BITRATE)) {
				cmds[0] = mecmd + "pass=1";
				cmds[1] = mecmd + fullargs + "bitrate=$(VideoBitrate)";
			} else {
				cmds[0] = mecmd + fullargs + "bitrate=$(VideoBitrate)";
			}
			break;
		case 1:	// fixed quant
			cmds[0] += mecmd + fullargs;
			cmds[0].AppendFormat("fixed_quant=%d", 31 * (101 - pref.GetValueInt(PI_VQUALITY)) / 100);
			break;
		case 2:	// 2-pass
			cmds[0] = mecmd + "pass=1";
			cmds[1] = mecmd + fullargs + "pass=2:bitrate=$(VideoBitrate)";
			break;
		}
		} break;
	case VC_MJPEG:
		cmds[0] = mecmd += " -ovc lavc -lavcopts vcodec=mjpeg  -of lavf -lavfopts i_certify_that_my_video_stream_does_not_use_b_frames:format=mp4";
		break;
	case VC_COPY:
		cmds[0] = mecmd += " -ovc copy";
		break;
	default:
		cmds[0] = mecmd += " -ovc null";
	}

	passcount = 0;
	for (i = 0; i < MAX_PASS_COUNT && cmds[i] != ""; i++);
	if (!i) return 0;
	passcount = i;;
	for (i = 0; i < passcount - 1; i++) {
		cmds[i] += " -nosound -o NUL";
	}
	switch (pref.GetValueInt(PI_CONTAINER)) {
	case CF_DEFAULT:
		GetAudioOptions(cmds[i]);
		break;
	default:
		cmds[i] += " -nosound";
	}
	cmds[i] += " -o \"$(DestFile)\"";
	return passcount;
}

bool CMencoder::CheckAudioSupport(int id)
{
	switch (id) {
	case AC_FAAC:
	case AC_PCM:
	case AC_COPY:
	case AC_NONE:
		return true;
	default:
		return false;
	}
}

bool CMencoder::CheckContainerSupport(int id)
{
	return false;
}

TCHAR* CMencoder::GetOutExt()
{
	if (!mcinst.item) return "m4v";
	switch (mcinst.item->target.videoformat) {
	case VC_XVID:
		return "m4v";
	default:
		return "mp4";
	}
}
