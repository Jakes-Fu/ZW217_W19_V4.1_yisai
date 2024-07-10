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

CHARACTER   [0-9A-Za-z]|(" "|"!"|"#"|"$"|"%"|"&"|"'"|"("|")"|"*"|"+"|","|"-"|"."|"/"|":"|";"|"<"|"="|">"|"?"|"@"|"["|"]"|"^"|"_"|"`"|"{"|"|"|"}"|"~"|"")
STRING      "\""{CHARACTER}*"\""
DIGIT       [0-9]
HDIGIT      [0-9A-Fa-f]
NUMBER      ("+"|"-")?{DIGIT}+
HNUMBER     ("+"|"-")?{HDIGIT}+
SPACE       ([ \t]+)
QUOTATION   (\")
DTMFCHAR   [0-9A-Za-z]|(" "|"!"|"#"|"$"|"%"|"&"|"'"|"("|")"|"*"|"+"|","|"-"|"."|"/"|":"|";"|"<"|"="|">"|"?"|"@"|"["|"]"|"^"|"_"|"`"|"{"|"|"|"}"|"~"|""|"\"")

DIALCHAR    [0-9a-dA-D]|(" "|","|">"|"*"|"#")
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
"+IMEITEST"        { yylval.cmd_index = AT_CMD_IMEITEST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+MSLSECUR"        { yylval.cmd_index = AT_CMD_MSLSECUR; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SERIALNO"        { yylval.cmd_index = AT_CMD_SERIALNO; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SETTESTNV"       { yylval.cmd_index = AT_CMD_SETTESTNV; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+DETALOCK"        { yylval.cmd_index = AT_CMD_DETALOCK; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+CGCMOD"          { yylval.cmd_index = AT_CMD_CGCMOD; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SETFDATA"          { yylval.cmd_index = AT_CMD_SETFDATA; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SETFULLFDATA"          { yylval.cmd_index = AT_CMD_SETFULLFDATA; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }


"AT"     { yylval.cmd_index = AT_CMD_AT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"##"     { yylval.cmd_index = AT_CMD_SHARP_SHARP; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"D"     { yylval.cmd_index = AT_CMD_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"Q"     { yylval.cmd_index = AT_CMD_Q; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"V"     { yylval.cmd_index = AT_CMD_V; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"A"     { yylval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"H"     { yylval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"Z"     { yylval.cmd_index = AT_CMD_Z; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"E"     { yylval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&F"     { yylval.cmd_index = AT_CMD_and_F; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&D"     { yylval.cmd_index = AT_CMD_and_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&C"     { yylval.cmd_index = AT_CMD_and_C; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"+CPAS"     { yylval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPIN"     { yylval.cmd_index = AT_CMD_CPIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLIP"     { yylval.cmd_index = AT_CMD_CLIP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLIR"     { yylval.cmd_index = AT_CMD_CLIR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCFC"     { yylval.cmd_index = AT_CMD_CCFC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCWA"     { yylval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CREG"     { yylval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+VTS"     { yylval.cmd_index = AT_CMD_VTS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHLD"     { yylval.cmd_index = AT_CMD_CHLD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLCC"     { yylval.cmd_index = AT_CMD_CLCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COPS"     { yylval.cmd_index = AT_CMD_COPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SIND"     { yylval.cmd_index = AT_CMD_SIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSQ"     { yylval.cmd_index = AT_CMD_CSQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIMI"     { yylval.cmd_index = AT_CMD_CIMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGMI"     { yylval.cmd_index = AT_CMD_CGMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGMM"     { yylval.cmd_index = AT_CMD_CGMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGSN"     { yylval.cmd_index = AT_CMD_CGSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGMR"     { yylval.cmd_index = AT_CMD_CGMR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMEE"     { yylval.cmd_index = AT_CMD_CMEE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSCS"     { yylval.cmd_index = AT_CMD_CSCS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPOF"     { yylval.cmd_index = AT_CMD_CPOF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPOL"     { yylval.cmd_index = AT_CMD_CPOL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S0"     { yylval.cmd_index = AT_CMD_S0; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGDCONT"     { yylval.cmd_index = AT_CMD_CGDCONT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGPCO"     { yylval.cmd_index = AT_CMD_CGPCO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGQREQ"     { yylval.cmd_index = AT_CMD_CGQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGQMIN"     { yylval.cmd_index = AT_CMD_CGQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGATT"     { yylval.cmd_index = AT_CMD_CGATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGACT"     { yylval.cmd_index = AT_CMD_CGACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGCMOD"     { yylval.cmd_index = AT_CMD_CGCMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGPADDR"     { yylval.cmd_index = AT_CMD_CGPADDR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGDATA"     { yylval.cmd_index = AT_CMD_CGDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
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
"+CSCB"     { yylval.cmd_index = AT_CMD_CSCB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CNMI"     { yylval.cmd_index = AT_CMD_CNMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBR"     { yylval.cmd_index = AT_CMD_CPBR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBF"     { yylval.cmd_index = AT_CMD_CPBF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBW"     { yylval.cmd_index = AT_CMD_CPBW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBP"     { yylval.cmd_index = AT_CMD_CPBP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBN"     { yylval.cmd_index = AT_CMD_CPBN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CNUM"     { yylval.cmd_index = AT_CMD_CNUM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSMP"     { yylval.cmd_index = AT_CMD_SSMP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBS"     { yylval.cmd_index = AT_CMD_CPBS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCUG"     { yylval.cmd_index = AT_CMD_CCUG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CUSD"     { yylval.cmd_index = AT_CMD_CUSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAOC"     { yylval.cmd_index = AT_CMD_CAOC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCLK"     { yylval.cmd_index = AT_CMD_CCLK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+XX"     { yylval.cmd_index = AT_CMD_XX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCID"     { yylval.cmd_index = AT_CMD_CCID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+VGR"     { yylval.cmd_index = AT_CMD_VGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRSL"     { yylval.cmd_index = AT_CMD_CRSL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMUT"     { yylval.cmd_index = AT_CMD_CMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSEA"     { yylval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SBAND"     { yylval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDTMF"     { yylval.cmd_index = AT_CMD_SDTMF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLCK"     { yylval.cmd_index = AT_CMD_CLCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPWD"     { yylval.cmd_index = AT_CMD_CPWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSAM"     { yylval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSAP"     { yylval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SADM"     { yylval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SADMDSP"     { yylval.cmd_index = AT_CMD_SADMDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPLO"     { yylval.cmd_index = AT_CMD_SPLO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CFUN"     { yylval.cmd_index = AT_CMD_CFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIND"     { yylval.cmd_index = AT_CMD_CIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCED"     { yylval.cmd_index = AT_CMD_CCED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+STONE"     { yylval.cmd_index = AT_CMD_STONE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSST"     { yylval.cmd_index = AT_CMD_SSST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SATT"     { yylval.cmd_index = AT_CMD_SATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SAUTOATT"     { yylval.cmd_index = AT_CMD_SAUTOATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SGPRSDATA"     { yylval.cmd_index = AT_CMD_SGPRSDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDMUT"     { yylval.cmd_index = AT_CMD_SDMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SUPS"     { yylval.cmd_index = AT_CMD_SUPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSYS"     { yylval.cmd_index = AT_CMD_SSYS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCPFRE"     { yylval.cmd_index = AT_CMD_SCPFRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEQREQ"     { yylval.cmd_index = AT_CMD_CGEQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEQMIN"     { yylval.cmd_index = AT_CMD_CGEQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCINF"     { yylval.cmd_index = AT_CMD_SCINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SFSMSM"     { yylval.cmd_index = AT_CMD_SFSMSM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPEQPARA"     { yylval.cmd_index = AT_CMD_SPEQPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPREF"     { yylval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SYTIN"     { yylval.cmd_index = AT_CMD_SYTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SFTIN"     { yylval.cmd_index = AT_CMD_SFTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMUX"     { yylval.cmd_index = AT_CMD_CMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SIME"     { yylval.cmd_index = AT_CMD_SIME; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCMUX"     { yylval.cmd_index = AT_CMD_SCMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+PSORGMOD"     { yylval.cmd_index = AT_CMD_PSORGMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPENHA"     { yylval.cmd_index = AT_CMD_SPENHA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPADLVVE"     { yylval.cmd_index = AT_CMD_SPADLVVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMMS"     { yylval.cmd_index = AT_CMD_CMMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMER"     { yylval.cmd_index = AT_CMD_CMER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMOD"     { yylval.cmd_index = AT_CMD_CMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHUP"     { yylval.cmd_index = AT_CMD_CHUP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+HEADINFO"     { yylval.cmd_index = AT_CMD_HEADINFO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SYSSLEEP"     { yylval.cmd_index = AT_CMD_SYSSLEEP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MAXPOWER"     { yylval.cmd_index = AT_CMD_MAXPOWER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FCEPTEST"     { yylval.cmd_index = AT_CMD_FCEPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPKSTEST"     { yylval.cmd_index = AT_CMD_SPKSTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+KEYSHORT"     { yylval.cmd_index = AT_CMD_KEYSHORT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+READRSSI"     { yylval.cmd_index = AT_CMD_READRSSI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GETTESTNV"     { yylval.cmd_index = AT_CMD_GETTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SETTESTNV"     { yylval.cmd_index = AT_CMD_SETTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GETFULLHISTNV"     { yylval.cmd_index = AT_CMD_GETFULLHISTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+LOGERASE"     { yylval.cmd_index = AT_CMD_LOGERASE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GETFULLTESTNV"     { yylval.cmd_index = AT_CMD_GETFULLTESTNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+TEMPTEST"     { yylval.cmd_index = AT_CMD_TEMPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+BATTTEST"     { yylval.cmd_index = AT_CMD_BATTTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IMEITEST"     { yylval.cmd_index = AT_CMD_IMEITEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+LVOFLOCK"     { yylval.cmd_index = AT_CMD_LVOFLOCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+DETALOCK"     { yylval.cmd_index = AT_CMD_DETALOCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+RSTVERIF"     { yylval.cmd_index = AT_CMD_RSTVERIF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+LOOPTEST"     { yylval.cmd_index = AT_CMD_LOOPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+LOCKREAD"     { yylval.cmd_index = AT_CMD_LOCKREAD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CVHU"     { yylval.cmd_index = AT_CMD_CVHU; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MSLSECUR"     { yylval.cmd_index = AT_CMD_MSLSECUR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SERIALNO"     { yylval.cmd_index = AT_CMD_SERIALNO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FACTORST"     { yylval.cmd_index = AT_CMD_FACTORST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+KSTRINGB"     { yylval.cmd_index = AT_CMD_KSTRINGB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+AKSEEDNO"     { yylval.cmd_index = AT_CMD_AKSEEDNO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CALIDATE"     { yylval.cmd_index = AT_CMD_CALIDATE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SIMDETEC"     { yylval.cmd_index = AT_CMD_SIMDETEC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SMS"     { yylval.cmd_index = AT_CMD_SMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MISCD"     { yylval.cmd_index = AT_CMD_MISCD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+EMEMTEST"     { yylval.cmd_index = AT_CMD_EMEMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FCMPTEST"     { yylval.cmd_index = AT_CMD_FCMPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FCBTTEST"     { yylval.cmd_index = AT_CMD_FCBTTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SETFDATA"     { yylval.cmd_index = AT_CMD_SETFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SETFULLFDATA"     { yylval.cmd_index = AT_CMD_SETFULLFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GETFDATA"     { yylval.cmd_index = AT_CMD_GETFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GETFULLFDATA"     { yylval.cmd_index = AT_CMD_GETFULLFDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPAUDVOLTYPE"     { yylval.cmd_index = AT_CMD_SPAUDVOLTYPE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }


<AT_CMD>{NUMBER}        { return NUMBER;  }
 
<AT_CMD>{STRING}        { return STRING; }

<AT_DIAL_CMD>{DIALSTRING} { BEGIN(INITIAL); return DIALSTRING; }
<AT_SHARP_CMD>{SHARPSTRING} { BEGIN(INITIAL); return SHARPSTRING; }
<AT_DTMF_CMD>{DTMFSTRING} { BEGIN(INITIAL); return DTMFSTRING; }

<<EOF>>         {return 0;}

{SPACE}         { }

.               { return yytext[0]; }
%%

