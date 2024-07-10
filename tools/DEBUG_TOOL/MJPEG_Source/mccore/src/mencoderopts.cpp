/*******************************************************************
* MediaCoder - The universal audio/video transcoder
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*******************************************************************/

#include "config.h"
#include <windows.h>
#include <stdio.h>
#include "datatypes.h"
#include "transcode.h"
#include "preference.h"

extern MC_STATE* pinst;

void GenMplayerOptions(CString& str)
{
	TCHAR *p;
	if (pref.GetValueInt(PI_CACHE_SIZE)) {
		str += " -cache ";
		str += pref.GetValueStr(PI_CACHE_SIZE);
	}
	if (*(p = pref.GetValueStr(PI_MP_OPTS))) {
		str += ' ';
		str += pref.GetValueStr(PI_MP_OPTS);
	}
}

void GetOptionFPS(CString& str)
{
	if (!pref.GetValueInt(PI_FPS)) return;
	TCHAR *v = pref.GetValueEnum(PI_FPS);
	if (!strcmp(v, "23.976"))
		v = "24000/1001";
	else if (!strcmp(v, "29.97"))
		v = "30000/1001";
	str += v;
}

void GenAudioFilterOptions(CString& str, int srate)
{
	CString opts;
	int i;
	if (!srate) {
		srate = pref.GetValueInt(PI_RESAMPLE_FREQ);
	}
	if (srate) {
		opts.AppendFormat("resample=%d:%d:%d,",
			srate,
			pref.GetValueInt(PI_RESAMPLE_HI),
			pref.GetValueInt(PI_RESAMPLE_METHOD));
	}
	if (pref.GetValueBool(PI_EQ)) {
		opts += "equalizer=";
		for (i=PI_EQ_B0;i<=PI_EQ_B9;i++) {
			opts.AppendFormat("%d:",pref.GetValueInt(i));
		}
		opts.SetAt(opts.GetLength() - 1, ',');
	}
	if (pref.GetValueBool(PI_CHANNELS)) {
		int nr=0;
		for (i=0;i<5;i++) {
			if (pref.GetValueInt(PI_CHANNELS_CH0+i)>0) nr++;
		}
		opts.AppendFormat("channels=%d:%d:",pref.GetValueInt(PI_CHANNELS_NUM),nr);
		for (i=0;i<5;i++) {
			if (pref.GetValueInt(PI_CHANNELS_CH0+i)>0)
				opts.AppendFormat("%d:%d:",pref.GetValueInt(PI_CHANNELS_CH0+i)-1,i);
		}
		opts += ',';
	}
	if (pref.GetValueBool(PI_SURROUND)) {
		opts.AppendFormat("surround=%.2f,", pref.GetValueFloat(PI_SURROUND));
	}
	if (pref.GetValueBool(PI_DELAY)) {
		opts += "delay=";
		for (i=PI_DELAY_CH0;i<PI_DELAY_CH5;i++) {
			opts.AppendFormat("%.2f:", pref.GetValueFloat(i));
		}
		opts.SetAt(opts.GetLength() - 1, ',');
	}
	if (pref.GetValueBool(PI_XSTEREO)) {
		opts.AppendFormat("extrastereo=%.2f,", pref.GetValueFloat(PI_XSTEREO_CO));
	}

	float gain = pref.GetValueFloat(PI_VOL_GAIN);
	// workaround for AMR sound level too low issue
	if (mcinst.item->target.audioformat == AC_AMR) {
		gain += 6;
	}
	if (pref.GetValueBool(PI_VOLNORM)) {
		opts += "volnorm=2,";
	} else if (gain) {
		opts.AppendFormat("volume=%.2f:%d,",
			gain,
			pref.GetValueInt(PI_VOL_SOFTCLIP));
	}

	if (pref.GetValueBool(PI_LADSPA)) {
		opts.AppendFormat("ladspa=%s:%s:%.4f,",
			pref.GetValueStr(PI_LADSPA_FILE),
			pref.GetValueStr(PI_LADSPA_LABEL),
			pref.GetValueFloat(PI_LADSPA_CTRL));
	}
	if (pref.GetValueBool(PI_COMP)) {
		opts += "comp,";
	}
	if (!opts.IsEmpty()) {
		opts.Delete(opts.GetLength() - 1, 1);
		str += " -af ";
		str += opts;
	}
}

void GenVideoFilterOptions(CString& str)
{
	CString opts;
	int i;
	if (pref.GetValueBool(PI_DEINTERLACE)) {
		opts += "ivtc=1,";
	}
	if (pref.GetValueBool(PI_CROP)) {
		int x = pref.GetValueInt(PI_CROP_LEFT);
		int y = pref.GetValueInt(PI_CROP_TOP);
		int w = pref.GetValueInt(PI_CROP_WIDTH);
		int h = pref.GetValueInt(PI_CROP_HEIGHT);
		if (x | y | w | h) {
			opts += "crop=";
			if (w) opts.AppendFormat("%d", w);
			opts += ':';
			if (h) opts.AppendFormat("%d", h);
			opts += ':';
			if (x) opts.AppendFormat("%d", x);
			opts += ':';
			if (y) opts.AppendFormat("%d", y);
			opts += ',';
		}
	}
	if (pref.GetValueBool(PI_SCALE)) {
		static const char* alg[] = {":", "0.00:0.75", "0.00:0.50", "0.33:0.33", "1.00:0.00"};
		int i = pref.GetValueInt(PI_SCALE_PRESET);
		opts += "scale=";
		//@ Liu Kai
		/*
		if (!i) {
			opts.AppendFormat("%d:%d:%d:%d:%s,",
				pref.GetValueInt(PI_SCALE_WIDTH),
				pref.GetValueInt(PI_SCALE_HEIGHT),
				pref.GetValueInt(PI_SCALE_IS),
				pref.GetValueInt(PI_SCALE_CHROMA),
				alg[pref.GetValueInt(PI_SCALE_ALG)]
				);
		} else {
			CString str(pref.GetValueEnum(PI_SCALE_PRESET));
			str.Replace('x', ':');
			opts.AppendFormat("%s:%d:%d:%s,", 
				str.GetBuffer(),
				pref.GetValueInt(PI_SCALE_IS),
				pref.GetValueInt(PI_SCALE_CHROMA),
				alg[pref.GetValueInt(PI_SCALE_ALG)]);
		}
		*/
		CString str( pref.GetValueEnum( PI_SCALE_PRESET ) );
		if (!str.CompareNoCase( "original" ) ){
			opts.AppendFormat("%d:%d:%d:%d:%s,",
				pref.GetValueInt(PI_SCALE_WIDTH),
				pref.GetValueInt(PI_SCALE_HEIGHT),
				pref.GetValueInt(PI_SCALE_IS),
				pref.GetValueInt(PI_SCALE_CHROMA),
				alg[pref.GetValueInt(PI_SCALE_ALG)]
				);
		} else {
            //@liukai 
            int nIndex = mcGetPrefValueInt(PI_SCALE_PRESET);
            if( nIndex == -1 )
            {
                // Customize width and height
                int w = mcGetPrefValueInt(PI_SCALE_WIDTH);
                int h = mcGetPrefValueInt(PI_SCALE_HEIGHT);
                opts.AppendFormat("%d:%d:%d:%d:%s,", 
                    w,h,
                    pref.GetValueInt(PI_SCALE_IS),
                    pref.GetValueInt(PI_SCALE_CHROMA),
                    alg[pref.GetValueInt(PI_SCALE_ALG)]);
            }
            else
            {
                CString str(pref.GetValueEnum(PI_SCALE_PRESET));
                str.Replace('x', ':');
                opts.AppendFormat("%s:%d:%d:%s,", 
                    str.GetBuffer(),
                    pref.GetValueInt(PI_SCALE_IS),
                    pref.GetValueInt(PI_SCALE_CHROMA),
                    alg[pref.GetValueInt(PI_SCALE_ALG)]);
            }
		}
	}
	if (pref.GetValueBool(PI_EXPAND)) {
		static const TCHAR* aspects[] = {"3/4", "9/16", "1:2"};
		opts += "expand=";
		if (pref.GetValueInt(PI_EXPAND_W)) opts.AppendFormat("%d", pref.GetValueInt(PI_EXPAND_W));
		opts += ':';
		if (pref.GetValueInt(PI_EXPAND_H)) opts.AppendFormat("%d", pref.GetValueInt(PI_EXPAND_H));
		opts += ':';
		if (pref.GetValueInt(PI_EXPAND_X)) opts.AppendFormat("%d", pref.GetValueInt(PI_EXPAND_X));
		opts += ':';
		if (pref.GetValueInt(PI_EXPAND_Y)) opts.AppendFormat("%d", pref.GetValueInt(PI_EXPAND_Y));
		opts.AppendFormat("%d", pref.GetValueInt(PI_EXPAND_SUB));
		if (pref.GetValueInt(PI_EXPAND_ASPECT)) {
			opts += ':';
			opts += aspects[pref.GetValueInt(PI_EXPAND_ASPECT) - 1] + 1;
		}
		opts += ',';
	}
	if (pref.GetValueBool(PI_CROPDETECT)) {
		opts.AppendFormat("cropdetect=%d:%d,", pref.GetValueInt(PI_LIMIT), pref.GetValueInt(PI_ROUND));
	}
	if (pref.GetValueInt(PI_PP)) {
		CString pp;
		static TCHAR* ppq[]={":c:n", ":y", ":a"};
		TCHAR *q = "";
		switch (pref.GetValueInt(PI_PP)) {
		case 1:
			pp += "de/";
			if (pref.GetValueBool(PI_PP_LB)) pp += ",lb";
			if (pref.GetValueBool(PI_PP_TN)) pp += ",tn";
			break;
		case 2:
			pp += "fa/";
			break;
		case 3:
		case 4:
		case 5:
			q = ppq[pref.GetValueInt(PI_PP) - 3];
		case 6:
			if (pref.GetValueBool(PI_PP_DB)) pp.AppendFormat("hb%s/vb%s/", q, q);
			if (pref.GetValueBool(PI_PP_DR)) pp.AppendFormat("dr%s/", q);
			if (pref.GetValueBool(PI_PP_AL)) pp.AppendFormat("al%s/", q);
			if (pref.GetValueBool(PI_PP_LB)) pp.AppendFormat("lb%s/", q);
			if (pref.GetValueBool(PI_PP_TN)) pp += "tn:1:2:3/";
			break;
		}
		if (!pp.IsEmpty()) {
			pp.Delete(pp.GetLength() - 1, 1);
			opts += "pp=" + pp + ',';
		}
	}
	if ((i = pref.GetValueInt(PI_ROTATE))) {
		opts.AppendFormat("rotate=%d,", i - 1);
	}
	if (pref.GetValueInt(PI_DEN)) {
		opts.AppendFormat("%s=%d:%d:%d,",
			pref.GetValueInt(PI_DEN)==1 ? "denoise3d" : "hqdn3d",
			pref.GetValueInt(PI_DEN_LUMA),
			pref.GetValueInt(PI_DEN_CHROMA),
			pref.GetValueInt(PI_DEN_TEMPORAL));
	}
	if (mcinst.item->target.fps && (pref.GetValueBool(PI_CONSTANT_FPS) || mcinst.item->target.container == CF_MP4)) {
		opts += "harddup,";
	}
	if (pref.GetValueBool(PI_SCREENSHOT)) {
		opts += "screenshot,";
	}
	if (*pref.GetValueStr(PI_VF_EXTRA)) {
		opts += pref.GetValueStr(PI_VF_EXTRA);
		opts += ',';
	}
	if (!opts.IsEmpty()) {
		opts.Delete(opts.GetLength() - 1, 1);
		str += " -vf " + opts;
	}
}

void GenCommonOptions(CString& str, T_ITEM* item)
{
	TCHAR *p;
	if (item && item->info.discroot) {
		str.AppendFormat(" -dvd-device \"%s\"", item->info.discroot);
	}
	if (item && item->info.chapter) {
		str.AppendFormat(" -chapter %d", item->info.chapter);
	}
	if (*(p = pref.GetValueStr(PI_SLANG))) {
		str.AppendFormat(" -slang %s", p);
	}
	if (*(p = pref.GetValueStr(PI_ALANG))) {
		str.AppendFormat(" -alang %s", p);
	}
	if (pref.GetValueInt(PI_AID)) {
		str.AppendFormat(" -aid %d", pref.GetValueInt(PI_AID));
	}
	if (pref.GetValueInt(PI_VID)) {
		str.AppendFormat(" -vid %d", pref.GetValueInt(PI_VID));
	}
	if (pref.GetValueInt(PI_CHANNEL)) {
		str.AppendFormat(" -channels %d",pref.GetValueInt(PI_CHANNEL)*2);
	}
	if (*(p = pref.GetValueStr(PI_AUDIO_FILE))) {
		str.AppendFormat(" -audiofile %s", p);
	}
	if (pref.GetValueBool(PI_FORCE_IDX)) {
		str += " -forceidx";
	}
	if (*(p = pref.GetValueStr(PI_ME_OPTS))) {
		str.AppendFormat(" %s", p);
	}
}
