%{
#include "sci_types.h"
//#include "sdl_api.h"

#include "atc_common.h"
#include "atc_def.h"

#include "atc_yacc.h"

/******************************************************************************/
// Description : This function is used to reset lex state while do some unexpect
//               parase.
// Global resource dependence : None
// Author : Shijun.cui
// Note : add for reset lex state during at command parase.
/******************************************************************************/
void zerolex(void)
{
    atc_start = 1;
    atc_last_accepting_state = 0;
    atc_last_accepting_cpos = NULL;
}

const uint8 * atcparse_at(const uint8 * atcbuf, uint32 len)
{
	if(atcbuf && len)
	{
		while(len)
		{
			if((atcbuf[0] == 'a' || atcbuf[0] == 'A') && (atcbuf[1] == 't' || atcbuf[1] == 'T'))
			{
				return atcbuf;
			}

			atcbuf++;
			len--;
		}
	}

	return atcbuf;
}
%}

%option noyywrap
%s AT_CMD
%x AT_DIAL_CMD
%x AT_SHARP_CMD
%x AT_DTMF_CMD

CHARACTER   [0-9A-Za-z]|(" "|"!"|"#"|"$"|"%"|"&"|"'"|"("|")"|"*"|"+"|","|"-"|"."|"/"|":"|";"|"<"|"="|">"|"?"|"@"|"["|"]"|"^"|"_"|"`"|"{"|"|"|"}"|"~"|"\\"|"")
STRING      "\""{CHARACTER}*"\""
DIGIT       [0-9]
HDIGIT      [0-9A-Fa-f]
NUMBER      ("+"|"-")?([1-9]{DIGIT}*|[0])
HNUMBER     ("+"|"-")?{HDIGIT}+
SPACE       ([ \t]+)
QUOTATION   (\")
DTMFCHAR   [0-9A-Za-z]|(" "|"!"|"#"|"$"|"%"|"&"|"'"|"("|")"|"*"|"+"|","|"-"|"."|"/"|":"|";"|"<"|"="|">"|"?"|"@"|"["|"]"|"^"|"_"|"`"|"{"|"|"|"}"|"~"|""|"\"")

DIALCHAR    [0-9a-diA-DI]|(" "|","|">"|"*"|"#"|"+")
DIALSTRING  {DIALCHAR}+
SHARPSTRING {CHARACTER}+
DTMFSTRING    {DTMFCHAR}+
%%

"AT"            { return AT_CMD_PREFIX; }
";"             { BEGIN(INITIAL); return AT_CMD_DIVIDE;}
"\r"            { BEGIN(INITIAL); return AT_CMD_END; }

"D"             { yylval.cmd_index = AT_CMD_D; BEGIN(AT_DIAL_CMD); return AT_BASIC_CMD; }
"##"            { yylval.cmd_index = AT_CMD_SHARP_SHARP; BEGIN(AT_SHARP_CMD); return AT_BASIC_CMD; }
"+CPIN"            { yylval.cmd_index = AT_CMD_CPIN; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+CPWD"            { yylval.cmd_index = AT_CMD_CPWD; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+CLCK"            { yylval.cmd_index = AT_CMD_CLCK; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+VTS"             { yylval.cmd_index = AT_CMD_VTS; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+EVTS"             { yylval.cmd_index = AT_CMD_EVTS; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+ECPIN2"            { yylval.cmd_index = AT_CMD_ECPIN2; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"X"            { yylval.cmd_index = AT_CMD_X; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SPBTTEST"            { yylval.cmd_index = AT_CMD_SPBTTEST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SPBTCTRL"            { yylval.cmd_index = AT_CMD_SPBTCTRL; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SPBLETEST"           { yylval.cmd_index = AT_CMD_SPBLETEST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }


"Q"     { yylval.cmd_index = AT_CMD_Q; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"V"     { yylval.cmd_index = AT_CMD_V; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"A"     { yylval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"H"     { yylval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"Z"     { yylval.cmd_index = AT_CMD_Z; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"E"     { yylval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&F"     { yylval.cmd_index = AT_CMD_and_F; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&D"     { yylval.cmd_index = AT_CMD_and_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&C"     { yylval.cmd_index = AT_CMD_and_C; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&W"     { yylval.cmd_index = AT_CMD_and_W; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"W"     { yylval.cmd_index = AT_CMD_W; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&K"     { yylval.cmd_index = AT_CMD_and_K; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"S95"     { yylval.cmd_index = AT_CMD_S95; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"%%%"     { yylval.cmd_index = AT_CMD_THREE_PERCENT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"O"     { yylval.cmd_index = AT_CMD_O; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPAS"     { yylval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CBC"     { yylval.cmd_index = AT_CMD_CBC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLIP"     { yylval.cmd_index = AT_CMD_CLIP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLIR"     { yylval.cmd_index = AT_CMD_CLIR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCFC"     { yylval.cmd_index = AT_CMD_CCFC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCWA"     { yylval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CREG"     { yylval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRC"     { yylval.cmd_index = AT_CMD_CRC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHLD"     { yylval.cmd_index = AT_CMD_CHLD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLCC"     { yylval.cmd_index = AT_CMD_CLCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COPS"     { yylval.cmd_index = AT_CMD_COPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSSN"     { yylval.cmd_index = AT_CMD_CSSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SIND"     { yylval.cmd_index = AT_CMD_SIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SBCM"     { yylval.cmd_index = AT_CMD_SBCM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSQ"     { yylval.cmd_index = AT_CMD_CSQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIMI"     { yylval.cmd_index = AT_CMD_CIMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGMI"     { yylval.cmd_index = AT_CMD_CGMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGMM"     { yylval.cmd_index = AT_CMD_CGMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGSN"     { yylval.cmd_index = AT_CMD_CGSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGMR"     { yylval.cmd_index = AT_CMD_CGMR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMEE"     { yylval.cmd_index = AT_CMD_CMEE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSCS"     { yylval.cmd_index = AT_CMD_CSCS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SMUX"     { yylval.cmd_index = AT_CMD_SMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPOF"     { yylval.cmd_index = AT_CMD_CPOF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S0"     { yylval.cmd_index = AT_CMD_S0; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S2"     { yylval.cmd_index = AT_CMD_S2; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S3"     { yylval.cmd_index = AT_CMD_S3; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S4"     { yylval.cmd_index = AT_CMD_S4; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S5"     { yylval.cmd_index = AT_CMD_S5; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S6"     { yylval.cmd_index = AT_CMD_S6; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S7"     { yylval.cmd_index = AT_CMD_S7; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S8"     { yylval.cmd_index = AT_CMD_S8; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S10"     { yylval.cmd_index = AT_CMD_S10; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I1"     { yylval.cmd_index = AT_CMD_I1; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I2"     { yylval.cmd_index = AT_CMD_I2; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I3"     { yylval.cmd_index = AT_CMD_I3; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I4"     { yylval.cmd_index = AT_CMD_I4; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I5"     { yylval.cmd_index = AT_CMD_I5; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I6"     { yylval.cmd_index = AT_CMD_I6; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I7"     { yylval.cmd_index = AT_CMD_I7; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGDCONT"     { yylval.cmd_index = AT_CMD_CGDCONT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGPCO"     { yylval.cmd_index = AT_CMD_CGPCO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGQREQ"     { yylval.cmd_index = AT_CMD_CGQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGQMIN"     { yylval.cmd_index = AT_CMD_CGQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGATT"     { yylval.cmd_index = AT_CMD_CGATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGACT"     { yylval.cmd_index = AT_CMD_CGACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGPADDR"     { yylval.cmd_index = AT_CMD_CGPADDR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGDATA"     { yylval.cmd_index = AT_CMD_CGDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGAUTO"     { yylval.cmd_index = AT_CMD_CGAUTO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGANS"     { yylval.cmd_index = AT_CMD_CGANS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGCLASS"     { yylval.cmd_index = AT_CMD_CGCLASS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEREP"     { yylval.cmd_index = AT_CMD_CGEREP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGREG"     { yylval.cmd_index = AT_CMD_CGREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGSMS"     { yylval.cmd_index = AT_CMD_CGSMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSMS"     { yylval.cmd_index = AT_CMD_CSMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSDH"     { yylval.cmd_index = AT_CMD_CSDH; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRES"     { yylval.cmd_index = AT_CMD_CRES; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPMS"     { yylval.cmd_index = AT_CMD_CPMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMGF"     { yylval.cmd_index = AT_CMD_CMGF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMGR"     { yylval.cmd_index = AT_CMD_CMGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMGL"     { yylval.cmd_index = AT_CMD_CMGL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMGD"     { yylval.cmd_index = AT_CMD_CMGD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMGS"     { yylval.cmd_index = AT_CMD_CMGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMSS"     { yylval.cmd_index = AT_CMD_CMSS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSMP"     { yylval.cmd_index = AT_CMD_CSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMGC"     { yylval.cmd_index = AT_CMD_CMGC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMGW"     { yylval.cmd_index = AT_CMD_CMGW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSCA"     { yylval.cmd_index = AT_CMD_CSCA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSAS"     { yylval.cmd_index = AT_CMD_CSAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SMSC"     { yylval.cmd_index = AT_CMD_SMSC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SUSS"     { yylval.cmd_index = AT_CMD_SUSS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSCB"     { yylval.cmd_index = AT_CMD_CSCB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CNMI"     { yylval.cmd_index = AT_CMD_CNMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBR"     { yylval.cmd_index = AT_CMD_CPBR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBF"     { yylval.cmd_index = AT_CMD_CPBF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBW"     { yylval.cmd_index = AT_CMD_CPBW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBP"     { yylval.cmd_index = AT_CMD_CPBP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBN"     { yylval.cmd_index = AT_CMD_CPBN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CNUM"     { yylval.cmd_index = AT_CMD_CNUM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSMP"     { yylval.cmd_index = AT_CMD_SSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSVM"     { yylval.cmd_index = AT_CMD_CSVM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBS"     { yylval.cmd_index = AT_CMD_CPBS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COLP"     { yylval.cmd_index = AT_CMD_COLP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COLR"     { yylval.cmd_index = AT_CMD_COLR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPUC"     { yylval.cmd_index = AT_CMD_CPUC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CUSD"     { yylval.cmd_index = AT_CMD_CUSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAOC"     { yylval.cmd_index = AT_CMD_CAOC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CACM"     { yylval.cmd_index = AT_CMD_CACM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^STSF"     { yylval.cmd_index = AT_CMD_xor_STSF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^STIN"     { yylval.cmd_index = AT_CMD_xor_STIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^STGI"     { yylval.cmd_index = AT_CMD_xor_STGI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^STGR"     { yylval.cmd_index = AT_CMD_xor_STGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAMM"     { yylval.cmd_index = AT_CMD_CAMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCLK"     { yylval.cmd_index = AT_CMD_CCLK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+XX"     { yylval.cmd_index = AT_CMD_XX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+VTD"     { yylval.cmd_index = AT_CMD_VTD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCID"     { yylval.cmd_index = AT_CMD_CCID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+VGR"     { yylval.cmd_index = AT_CMD_VGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRSL"     { yylval.cmd_index = AT_CMD_CRSL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMUT"     { yylval.cmd_index = AT_CMD_CMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+VGT"     { yylval.cmd_index = AT_CMD_VGT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPEAKER"     { yylval.cmd_index = AT_CMD_SPEAKER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SEQT"     { yylval.cmd_index = AT_CMD_SEQT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSEA"     { yylval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SBAND"     { yylval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDTMF"     { yylval.cmd_index = AT_CMD_SDTMF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ECHO"     { yylval.cmd_index = AT_CMD_ECHO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSAM"     { yylval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSAP"     { yylval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SADM"     { yylval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPCM"     { yylval.cmd_index = AT_CMD_SPCM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SL1MON"     { yylval.cmd_index = AT_CMD_SL1MON; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SADC"     { yylval.cmd_index = AT_CMD_SADC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SFUN"     { yylval.cmd_index = AT_CMD_SFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CICB"     { yylval.cmd_index = AT_CMD_CICB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CFUN"     { yylval.cmd_index = AT_CMD_CFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPR"     { yylval.cmd_index = AT_CMD_IPR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIND"     { yylval.cmd_index = AT_CMD_CIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCED"     { yylval.cmd_index = AT_CMD_CCED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+STONE"     { yylval.cmd_index = AT_CMD_STONE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SAC"     { yylval.cmd_index = AT_CMD_SAC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ASSERT"     { yylval.cmd_index = AT_CMD_ASSERT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSST"     { yylval.cmd_index = AT_CMD_SSST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SATT"     { yylval.cmd_index = AT_CMD_SATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+S32K"     { yylval.cmd_index = AT_CMD_S32K; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CEER"     { yylval.cmd_index = AT_CMD_CEER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CR"     { yylval.cmd_index = AT_CMD_CR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SAUTOATT"     { yylval.cmd_index = AT_CMD_SAUTOATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SGPRSDATA"     { yylval.cmd_index = AT_CMD_SGPRSDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDCP"     { yylval.cmd_index = AT_CMD_SDCP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SRWD"     { yylval.cmd_index = AT_CMD_SRWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SNVM"     { yylval.cmd_index = AT_CMD_SNVM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SBRA"     { yylval.cmd_index = AT_CMD_SBRA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDMUT"     { yylval.cmd_index = AT_CMD_SDMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SLOG"     { yylval.cmd_index = AT_CMD_SLOG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SUPS"     { yylval.cmd_index = AT_CMD_SUPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSGF"     { yylval.cmd_index = AT_CMD_SSGF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSGS"     { yylval.cmd_index = AT_CMD_SSGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPLS"     { yylval.cmd_index = AT_CMD_CPLS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPOL"     { yylval.cmd_index = AT_CMD_CPOL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPPSRATE"     { yylval.cmd_index = AT_CMD_SPPSRATE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SYSINFO"     { yylval.cmd_index = AT_CMD_xor_SYSINFO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CBST"     { yylval.cmd_index = AT_CMD_CBST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMOD"     { yylval.cmd_index = AT_CMD_CMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"DL"     { yylval.cmd_index = AT_CMD_DL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHUP"     { yylval.cmd_index = AT_CMD_CHUP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGDSCONT"     { yylval.cmd_index = AT_CMD_CGDSCONT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEQREQ"     { yylval.cmd_index = AT_CMD_CGEQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEQMIN"     { yylval.cmd_index = AT_CMD_CGEQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGTFT"     { yylval.cmd_index = AT_CMD_CGTFT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPCOMDEBUG"     { yylval.cmd_index = AT_CMD_SPCOMDEBUG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMUX"     { yylval.cmd_index = AT_CMD_CMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SPN"     { yylval.cmd_index = AT_CMD_xor_SPN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSTA"     { yylval.cmd_index = AT_CMD_CSTA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SCPBR"     { yylval.cmd_index = AT_CMD_xor_SCPBR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SCPBW"     { yylval.cmd_index = AT_CMD_xor_SCPBW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ARMLOG"     { yylval.cmd_index = AT_CMD_ARMLOG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+UDISK"     { yylval.cmd_index = AT_CMD_UDISK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRSM"     { yylval.cmd_index = AT_CMD_CRSM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CNMA"     { yylval.cmd_index = AT_CMD_CNMA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+STED"     { yylval.cmd_index = AT_CMD_STED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHUPVT"     { yylval.cmd_index = AT_CMD_CHUPVT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+TRACE"     { yylval.cmd_index = AT_CMD_TRACE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+OFF"     { yylval.cmd_index = AT_CMD_OFF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+RESET"     { yylval.cmd_index = AT_CMD_RESET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEQNEG"     { yylval.cmd_index = AT_CMD_CGEQNEG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPAUTO"     { yylval.cmd_index = AT_CMD_SPAUTO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ECMMB"     { yylval.cmd_index = AT_CMD_ECMMB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ECMMBSTS"     { yylval.cmd_index = AT_CMD_ECMMBSTS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ECMMBEER"     { yylval.cmd_index = AT_CMD_ECMMBEER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+EUICC"     { yylval.cmd_index = AT_CMD_EUICC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUSATCAPREQ"     { yylval.cmd_index = AT_CMD_SPUSATCAPREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ESQOPT"     { yylval.cmd_index = AT_CMD_ESQOPT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMMS"     { yylval.cmd_index = AT_CMD_CMMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ECIND"     { yylval.cmd_index = AT_CMD_ECIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCWE"     { yylval.cmd_index = AT_CMD_CCWE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUSATENVECMD"     { yylval.cmd_index = AT_CMD_SPUSATENVECMD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUSATPROFILE"     { yylval.cmd_index = AT_CMD_SPUSATPROFILE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUSATTERMINAL"     { yylval.cmd_index = AT_CMD_SPUSATTERMINAL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUSATCALLSETUP"     { yylval.cmd_index = AT_CMD_SPUSATCALLSETUP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLVL"     { yylval.cmd_index = AT_CMD_CLVL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMER"     { yylval.cmd_index = AT_CMD_CMER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"*USBCT"     { yylval.cmd_index = AT_CMD_star_USBCT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"*PSNTRG"     { yylval.cmd_index = AT_CMD_star_PSNTRG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GMM"     { yylval.cmd_index = AT_CMD_GMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FCLASS"     { yylval.cmd_index = AT_CMD_FCLASS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GCI"     { yylval.cmd_index = AT_CMD_GCI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"#CLS"     { yylval.cmd_index = AT_CMD_sharp_CLS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^DSCI"     { yylval.cmd_index = AT_CMD_xor_DSCI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+PSRABR"     { yylval.cmd_index = AT_CMD_PSRABR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ERGA"     { yylval.cmd_index = AT_CMD_ERGA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ERTCA"     { yylval.cmd_index = AT_CMD_ERTCA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"M"     { yylval.cmd_index = AT_CMD_M; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPDUMMYKPD"     { yylval.cmd_index = AT_CMD_SPDUMMYKPD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPDUMMYKPDRSP"     { yylval.cmd_index = AT_CMD_SPDUMMYKPDRSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPCEER"     { yylval.cmd_index = AT_CMD_SPCEER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPSMSFULL"     { yylval.cmd_index = AT_CMD_SPSMSFULL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPCHIPTYPE"     { yylval.cmd_index = AT_CMD_SPCHIPTYPE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPEQPARA"     { yylval.cmd_index = AT_CMD_SPEQPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPREF"     { yylval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+NVOPERATE"     { yylval.cmd_index = AT_CMD_NVOPERATE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCPFRE"     { yylval.cmd_index = AT_CMD_SCPFRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPSTPARAM"     { yylval.cmd_index = AT_CMD_SPSTPARAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPSTACTION"     { yylval.cmd_index = AT_CMD_SPSTACTION; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SIPCONFIG"     { yylval.cmd_index = AT_CMD_SIPCONFIG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSYS"     { yylval.cmd_index = AT_CMD_SSYS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^MBCELLID"     { yylval.cmd_index = AT_CMD_xor_MBCELLID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^MBAU"     { yylval.cmd_index = AT_CMD_xor_MBAU; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPHAR"     { yylval.cmd_index = AT_CMD_SPHAR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CTZR"     { yylval.cmd_index = AT_CMD_CTZR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CTZU"     { yylval.cmd_index = AT_CMD_CTZU; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCMUX"     { yylval.cmd_index = AT_CMD_SCMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SMMSWAP"     { yylval.cmd_index = AT_CMD_SMMSWAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPSETSAVESMS"     { yylval.cmd_index = AT_CMD_SPSETSAVESMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SGFD"     { yylval.cmd_index = AT_CMD_SGFD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SGSIMG"     { yylval.cmd_index = AT_CMD_SGSIMG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SGMR"     { yylval.cmd_index = AT_CMD_SGMR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDRMOD"     { yylval.cmd_index = AT_CMD_SDRMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SFPL"     { yylval.cmd_index = AT_CMD_SFPL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SEPL"     { yylval.cmd_index = AT_CMD_SEPL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGSMFRQ"     { yylval.cmd_index = AT_CMD_SPGSMFRQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCDSLEEP"     { yylval.cmd_index = AT_CMD_SCDSLEEP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPENHA"     { yylval.cmd_index = AT_CMD_SPENHA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^CURC"     { yylval.cmd_index = AT_CMD_xor_CURC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCCBS"     { yylval.cmd_index = AT_CMD_SCCBS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSRVT"     { yylval.cmd_index = AT_CMD_SSRVT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSCR"     { yylval.cmd_index = AT_CMD_SSCR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SLOOPTEST"     { yylval.cmd_index = AT_CMD_SLOOPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SUSB"     { yylval.cmd_index = AT_CMD_SUSB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+STPTEST"     { yylval.cmd_index = AT_CMD_STPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSWAPCOM"     { yylval.cmd_index = AT_CMD_SSWAPCOM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SASYNC"     { yylval.cmd_index = AT_CMD_SASYNC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPTEST"     { yylval.cmd_index = AT_CMD_SPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUPLMN"     { yylval.cmd_index = AT_CMD_SPUPLMN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^CARDMODE"     { yylval.cmd_index = AT_CMD_xor_CARDMODE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SYSCONFIG"     { yylval.cmd_index = AT_CMD_xor_SYSCONFIG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPDSP"     { yylval.cmd_index = AT_CMD_SPDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPFRQ"     { yylval.cmd_index = AT_CMD_SPFRQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPID"     { yylval.cmd_index = AT_CMD_SPID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPDIAG"     { yylval.cmd_index = AT_CMD_SPDIAG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SADMDSP"     { yylval.cmd_index = AT_CMD_SADMDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPADLVVE"     { yylval.cmd_index = AT_CMD_SPADLVVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPAUDVOLTYPE"     { yylval.cmd_index = AT_CMD_SPAUDVOLTYPE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SGETIQ"     { yylval.cmd_index = AT_CMD_SGETIQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+NETAPN"     { yylval.cmd_index = AT_CMD_NETAPN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SOCKET"     { yylval.cmd_index = AT_CMD_SOCKET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPOPEN"     { yylval.cmd_index = AT_CMD_IPOPEN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPSEND"     { yylval.cmd_index = AT_CMD_IPSEND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPCLOSE"     { yylval.cmd_index = AT_CMD_IPCLOSE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPDEACT"     { yylval.cmd_index = AT_CMD_IPDEACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSIMDETPIN"     { yylval.cmd_index = AT_CMD_SSIMDETPIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+XLEMA"     { yylval.cmd_index = AT_CMD_XLEMA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+PRIO"     { yylval.cmd_index = AT_CMD_PRIO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGAUTH"     { yylval.cmd_index = AT_CMD_CGAUTH; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MAXPWRBF"     { yylval.cmd_index = AT_CMD_MAXPWRBF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^FCHAN"     { yylval.cmd_index = AT_CMD_xor_FCHAN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^FRXON"     { yylval.cmd_index = AT_CMD_xor_FRXON; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^FLNA"     { yylval.cmd_index = AT_CMD_xor_FLNA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^FRSSI"     { yylval.cmd_index = AT_CMD_xor_FRSSI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^FWAVE"     { yylval.cmd_index = AT_CMD_xor_FWAVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^FTXON"     { yylval.cmd_index = AT_CMD_xor_FTXON; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^FLOWCONTROL"     { yylval.cmd_index = AT_CMD_xor_FLOWCONTROL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"\\Q"     { yylval.cmd_index = AT_CMD_backslash_Q; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^MONI"     { yylval.cmd_index = AT_CMD_xor_MONI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^MONP"     { yylval.cmd_index = AT_CMD_xor_MONP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SMONG"     { yylval.cmd_index = AT_CMD_xor_SMONG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+WIFICTRL"     { yylval.cmd_index = AT_CMD_WIFICTRL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSIM"     { yylval.cmd_index = AT_CMD_CSIM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUSIMTEST"     { yylval.cmd_index = AT_CMD_SPUSIMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ETSRESET"     { yylval.cmd_index = AT_CMD_ETSRESET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSPWR"     { yylval.cmd_index = AT_CMD_SPGPSPWR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSRST"     { yylval.cmd_index = AT_CMD_SPGPSRST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSSTATUS"     { yylval.cmd_index = AT_CMD_SPGPSSTATUS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSINF"     { yylval.cmd_index = AT_CMD_SPGPSINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSOUT"     { yylval.cmd_index = AT_CMD_SPGPSOUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSFIX"     { yylval.cmd_index = AT_CMD_SPGPSFIX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSPDP"     { yylval.cmd_index = AT_CMD_SPGPSPDP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+QGNSSC"     { yylval.cmd_index = AT_CMD_QGNSSC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+QGNSSRD"     { yylval.cmd_index = AT_CMD_QGNSSRD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+QGNSSCMD"     { yylval.cmd_index = AT_CMD_QGNSSCMD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSTTFF"     { yylval.cmd_index = AT_CMD_SPGPSTTFF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSTT"     { yylval.cmd_index = AT_CMD_CSTT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIICR"     { yylval.cmd_index = AT_CMD_CIICR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIPSHUT"     { yylval.cmd_index = AT_CMD_CIPSHUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIFSR"     { yylval.cmd_index = AT_CMD_CIFSR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIPMUX"     { yylval.cmd_index = AT_CMD_CIPMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIPSTART"     { yylval.cmd_index = AT_CMD_CIPSTART; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GSN"     { yylval.cmd_index = AT_CMD_GSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GMR"     { yylval.cmd_index = AT_CMD_GMR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGNSPWR"     { yylval.cmd_index = AT_CMD_CGNSPWR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGNSAID"     { yylval.cmd_index = AT_CMD_CGNSAID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGNSCMD"     { yylval.cmd_index = AT_CMD_CGNSCMD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGNSINF"     { yylval.cmd_index = AT_CMD_CGNSINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FOTA"     { yylval.cmd_index = AT_CMD_FOTA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUART1"     { yylval.cmd_index = AT_CMD_SPUART1; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCWTEST"     { yylval.cmd_index = AT_CMD_SCWTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SCFG"     { yylval.cmd_index = AT_CMD_xor_SCFG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSASSERT"     { yylval.cmd_index = AT_CMD_SPGPSASSERT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSLTEOPEN"     { yylval.cmd_index = AT_CMD_SPGPSLTEOPEN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+RWRFREG"     { yylval.cmd_index = AT_CMD_RWRFREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }


<AT_CMD>{NUMBER}        { return NUMBER;  }
 
<AT_CMD>{STRING}        { return STRING; }

<AT_DIAL_CMD>{DIALSTRING} { BEGIN(INITIAL); return DIALSTRING; }
<AT_SHARP_CMD>{SHARPSTRING} { BEGIN(INITIAL); return SHARPSTRING; }
<AT_DTMF_CMD>{DTMFSTRING} { BEGIN(INITIAL); return DTMFSTRING; }

<<EOF>>         {return 0;}

{SPACE}         { }

.               { return yytext[0]; }
%%

