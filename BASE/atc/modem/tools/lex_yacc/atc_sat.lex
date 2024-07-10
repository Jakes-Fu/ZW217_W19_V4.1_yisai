%{
#include "sci_types.h"
//#include "sdl_api.h"

#include "atc_common.h"
#include "atc_sat_def.h"

#include "atc_yacc.h"

/******************************************************************************/
// Description : This function is used to reset lex state while do some unexpect
//               parase.
// Global resource dependence : None
// Author : Winnie.zhu
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


"Q"     { yylval.cmd_index = AT_CMD_Q; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"V"     { yylval.cmd_index = AT_CMD_V; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"A"     { yylval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"H"     { yylval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"Z"     { yylval.cmd_index = AT_CMD_Z; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"E"     { yylval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"and_F"     { yylval.cmd_index = AT_CMD_and_F; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"and_D"     { yylval.cmd_index = AT_CMD_and_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"and_C"     { yylval.cmd_index = AT_CMD_and_C; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"and_W"     { yylval.cmd_index = AT_CMD_and_W; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"W"     { yylval.cmd_index = AT_CMD_W; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"THREE_PERCENT"     { yylval.cmd_index = AT_CMD_THREE_PERCENT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"S0"     { yylval.cmd_index = AT_CMD_S0; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S2"     { yylval.cmd_index = AT_CMD_S2; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S3"     { yylval.cmd_index = AT_CMD_S3; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S4"     { yylval.cmd_index = AT_CMD_S4; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S5"     { yylval.cmd_index = AT_CMD_S5; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S6"     { yylval.cmd_index = AT_CMD_S6; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S7"     { yylval.cmd_index = AT_CMD_S7; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S8"     { yylval.cmd_index = AT_CMD_S8; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"S10"     { yylval.cmd_index = AT_CMD_S10; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPAS"     { yylval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCFC"     { yylval.cmd_index = AT_CMD_CCFC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCWA"     { yylval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CREG"     { yylval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRC"     { yylval.cmd_index = AT_CMD_CRC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHLD"     { yylval.cmd_index = AT_CMD_CHLD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLCC"     { yylval.cmd_index = AT_CMD_CLCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COPS"     { yylval.cmd_index = AT_CMD_COPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSSN"     { yylval.cmd_index = AT_CMD_CSSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
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
"+CGDCONT"     { yylval.cmd_index = AT_CMD_CGDCONT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGPCO"     { yylval.cmd_index = AT_CMD_CGPCO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGQREQ"     { yylval.cmd_index = AT_CMD_CGQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGQMIN"     { yylval.cmd_index = AT_CMD_CGQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGATT"     { yylval.cmd_index = AT_CMD_CGATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGACT"     { yylval.cmd_index = AT_CMD_CGACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGPADDR"     { yylval.cmd_index = AT_CMD_CGPADDR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGDATA"     { yylval.cmd_index = AT_CMD_CGDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGAUTO"     { yylval.cmd_index = AT_CMD_CGAUTO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEREP"     { yylval.cmd_index = AT_CMD_CGEREP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGREG"     { yylval.cmd_index = AT_CMD_CGREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
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
"+CNMI"     { yylval.cmd_index = AT_CMD_CNMI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBR"     { yylval.cmd_index = AT_CMD_CPBR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBF"     { yylval.cmd_index = AT_CMD_CPBF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBW"     { yylval.cmd_index = AT_CMD_CPBW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBP"     { yylval.cmd_index = AT_CMD_CPBP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBN"     { yylval.cmd_index = AT_CMD_CPBN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CNUM"     { yylval.cmd_index = AT_CMD_CNUM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSVM"     { yylval.cmd_index = AT_CMD_CSVM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPBS"     { yylval.cmd_index = AT_CMD_CPBS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPUC"     { yylval.cmd_index = AT_CMD_CPUC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CUSD"     { yylval.cmd_index = AT_CMD_CUSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CACM"     { yylval.cmd_index = AT_CMD_CACM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
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
"+SSEA"     { yylval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SBAND"     { yylval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDTMF"     { yylval.cmd_index = AT_CMD_SDTMF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ECHO"     { yylval.cmd_index = AT_CMD_ECHO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSAM"     { yylval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSAP"     { yylval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SADM"     { yylval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPLO"     { yylval.cmd_index = AT_CMD_SPLO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CICB"     { yylval.cmd_index = AT_CMD_CICB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CFUN"     { yylval.cmd_index = AT_CMD_CFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPR"     { yylval.cmd_index = AT_CMD_IPR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIND"     { yylval.cmd_index = AT_CMD_CIND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCED"     { yylval.cmd_index = AT_CMD_CCED; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+STONE"     { yylval.cmd_index = AT_CMD_STONE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SAC"     { yylval.cmd_index = AT_CMD_SAC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SATT"     { yylval.cmd_index = AT_CMD_SATT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CEER"     { yylval.cmd_index = AT_CMD_CEER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CR"     { yylval.cmd_index = AT_CMD_CR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDCP"     { yylval.cmd_index = AT_CMD_SDCP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SNVM"     { yylval.cmd_index = AT_CMD_SNVM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SBOOT"     { yylval.cmd_index = AT_CMD_SBOOT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDMUT"     { yylval.cmd_index = AT_CMD_SDMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SLOG"     { yylval.cmd_index = AT_CMD_SLOG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SUPS"     { yylval.cmd_index = AT_CMD_SUPS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSYS"     { yylval.cmd_index = AT_CMD_SSYS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCPFRE"     { yylval.cmd_index = AT_CMD_SCPFRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEQREQ"     { yylval.cmd_index = AT_CMD_CGEQREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEQMIN"     { yylval.cmd_index = AT_CMD_CGEQMIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRSM"     { yylval.cmd_index = AT_CMD_CRSM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCINF"     { yylval.cmd_index = AT_CMD_SCINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPEQPARA"     { yylval.cmd_index = AT_CMD_SPEQPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPREF"     { yylval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMUX"     { yylval.cmd_index = AT_CMD_CMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SIME"     { yylval.cmd_index = AT_CMD_SIME; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCMGS"     { yylval.cmd_index = AT_CMD_SCMGS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCMGW"     { yylval.cmd_index = AT_CMD_SCMGW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+PSORGMOD"     { yylval.cmd_index = AT_CMD_PSORGMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCREG"     { yylval.cmd_index = AT_CMD_SCREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CIEV"     { yylval.cmd_index = AT_CMD_CIEV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+NETAPN"     { yylval.cmd_index = AT_CMD_NETAPN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SOCKET"     { yylval.cmd_index = AT_CMD_SOCKET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPOPEN"     { yylval.cmd_index = AT_CMD_IPOPEN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPSEND"     { yylval.cmd_index = AT_CMD_IPSEND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPCLOSE"     { yylval.cmd_index = AT_CMD_IPCLOSE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IPDEACT"     { yylval.cmd_index = AT_CMD_IPDEACT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GSMSTATUS"     { yylval.cmd_index = AT_CMD_GSMSTATUS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FIRBOOT"     { yylval.cmd_index = AT_CMD_FIRBOOT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+VGSMSW"     { yylval.cmd_index = AT_CMD_VGSMSW; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+POSI"     { yylval.cmd_index = AT_CMD_POSI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SMMSWAP"     { yylval.cmd_index = AT_CMD_SMMSWAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SADMDSP"     { yylval.cmd_index = AT_CMD_SADMDSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPADLVVE"     { yylval.cmd_index = AT_CMD_SPADLVVE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPAUDVOLTYPE"     { yylval.cmd_index = AT_CMD_SPAUDVOLTYPE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUSIMTEST"     { yylval.cmd_index = AT_CMD_SPUSIMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }


<AT_CMD>{NUMBER}        { return NUMBER;  }
 
<AT_CMD>{STRING}        { return STRING; }

<AT_DIAL_CMD>{DIALSTRING} { BEGIN(INITIAL); return DIALSTRING; }
<AT_SHARP_CMD>{SHARPSTRING} { BEGIN(INITIAL); return SHARPSTRING; }
<AT_DTMF_CMD>{DTMFSTRING} { BEGIN(INITIAL); return DTMFSTRING; }

<<EOF>>         {return 0;}

{SPACE}         { }

.               { return yytext[0]; }
%%

