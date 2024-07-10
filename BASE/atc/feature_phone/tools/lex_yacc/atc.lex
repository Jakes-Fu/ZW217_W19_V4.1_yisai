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
###"E"             { yylval.cmd_index = AT_CMD_E; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+CPIN"         { yylval.cmd_index = AT_CMD_CPIN; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+CPWD"         { yylval.cmd_index = AT_CMD_CPWD; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+CLCK"         { yylval.cmd_index = AT_CMD_CLCK; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+VTS"          { yylval.cmd_index = AT_CMD_VTS; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+CGCMOD"       { yylval.cmd_index = AT_CMD_CGCMOD; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SPBTTEST"            { yylval.cmd_index = AT_CMD_SPBTTEST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SPFMTEST"            { yylval.cmd_index = AT_CMD_SPFMTEST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SPBTCTRL"            { yylval.cmd_index = AT_CMD_SPBTCTRL; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SPBLETEST"           { yylval.cmd_index = AT_CMD_SPBLETEST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+WIFI"           { yylval.cmd_index = AT_CMD_WIFI; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+NETIF"         { yylval.cmd_index = AT_CMD_NETIF; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+MTU"          { yylval.cmd_index = AT_CMD_MTU; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+DAPSIPERF"            { yylval.cmd_index = AT_CMD_DAPSIPERF; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+TRAFFIC"          { yylval.cmd_index = AT_CMD_TRAFFIC; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+MQTTSUBUNSUB"         { yylval.cmd_index = AT_CMD_MQTTSUBUNSUB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MQTTCONN"         { yylval.cmd_index = AT_CMD_MQTTCONN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MQTTPUB"         { yylval.cmd_index = AT_CMD_MQTTPUB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MQTTDISCONN"         { yylval.cmd_index = AT_CMD_MQTTDISCONN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COAP_GET"         { yylval.cmd_index = AT_CMD_COAP_GET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COAP_POST"         { yylval.cmd_index = AT_CMD_COAP_POST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COAP_PUT"         { yylval.cmd_index = AT_CMD_COAP_PUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COAP_DELETE"         { yylval.cmd_index = AT_CMD_COAP_DELETE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+TESTLWM2M"           { yylval.cmd_index = AT_CMD_TESTLWM2M; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSTEST"           { yylval.cmd_index = AT_CMD_SPGPSTEST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+CGNSSST"           { yylval.cmd_index = AT_CMD_CGNSSST; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+SOCKET"           { yylval.cmd_index = AT_CMD_SOCKET; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }
"+PING"           { yylval.cmd_index = AT_CMD_PING; BEGIN(AT_DTMF_CMD); return AT_BASIC_CMD; }


"AT"     { yylval.cmd_index = AT_CMD_AT; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"##"     { yylval.cmd_index = AT_CMD_SHARP_SHARP; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"D"     { yylval.cmd_index = AT_CMD_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"Q"     { yylval.cmd_index = AT_CMD_Q; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"V"     { yylval.cmd_index = AT_CMD_V; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"A"     { yylval.cmd_index = AT_CMD_A; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"H"     { yylval.cmd_index = AT_CMD_H; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"Z"     { yylval.cmd_index = AT_CMD_Z; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"E"     { yylval.cmd_index = AT_CMD_E; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&C"     { yylval.cmd_index = AT_CMD_and_C; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&D"     { yylval.cmd_index = AT_CMD_and_D; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"&F"     { yylval.cmd_index = AT_CMD_and_F; BEGIN(AT_CMD); return AT_BASIC_CMD; }
"S7"     { yylval.cmd_index = AT_CMD_S7; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPIN"     { yylval.cmd_index = AT_CMD_CPIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCFC"     { yylval.cmd_index = AT_CMD_CCFC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCWA"     { yylval.cmd_index = AT_CMD_CCWA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CREG"     { yylval.cmd_index = AT_CMD_CREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+VTS"     { yylval.cmd_index = AT_CMD_VTS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHLD"     { yylval.cmd_index = AT_CMD_CHLD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLCC"     { yylval.cmd_index = AT_CMD_CLCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPAS"     { yylval.cmd_index = AT_CMD_CPAS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
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
"+CGDATA"     { yylval.cmd_index = AT_CMD_CGDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
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
"+CAOC"     { yylval.cmd_index = AT_CMD_CAOC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCLK"     { yylval.cmd_index = AT_CMD_CCLK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+XX"     { yylval.cmd_index = AT_CMD_XX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCID"     { yylval.cmd_index = AT_CMD_CCID; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+VGR"     { yylval.cmd_index = AT_CMD_VGR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMUT"     { yylval.cmd_index = AT_CMD_CMUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSEA"     { yylval.cmd_index = AT_CMD_SSEA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SBAND"     { yylval.cmd_index = AT_CMD_SBAND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDTMF"     { yylval.cmd_index = AT_CMD_SDTMF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLCK"     { yylval.cmd_index = AT_CMD_CLCK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CPWD"     { yylval.cmd_index = AT_CMD_CPWD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+NVGV"     { yylval.cmd_index = AT_CMD_NVGV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+NVPC"     { yylval.cmd_index = AT_CMD_NVPC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SSIT"     { yylval.cmd_index = AT_CMD_xor_SSIT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSAM"     { yylval.cmd_index = AT_CMD_SSAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SSAP"     { yylval.cmd_index = AT_CMD_SSAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SADM"     { yylval.cmd_index = AT_CMD_SADM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
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
"+SPREF"     { yylval.cmd_index = AT_CMD_SPREF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SYTIN"     { yylval.cmd_index = AT_CMD_SYTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SFTIN"     { yylval.cmd_index = AT_CMD_SFTIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMUX"     { yylval.cmd_index = AT_CMD_CMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SIME"     { yylval.cmd_index = AT_CMD_SIME; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCMUX"     { yylval.cmd_index = AT_CMD_SCMUX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+PSORGMOD"     { yylval.cmd_index = AT_CMD_PSORGMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPENHA"     { yylval.cmd_index = AT_CMD_SPENHA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"X"     { yylval.cmd_index = AT_CMD_X; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPAUDVOLTYPE"     { yylval.cmd_index = AT_CMD_SPAUDVOLTYPE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPDIAG"     { yylval.cmd_index = AT_CMD_SPDIAG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPBTTEST"     { yylval.cmd_index = AT_CMD_SPBTTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPBTCTRL"     { yylval.cmd_index = AT_CMD_SPBTCTRL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I1"     { yylval.cmd_index = AT_CMD_I1; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I2"     { yylval.cmd_index = AT_CMD_I2; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I3"     { yylval.cmd_index = AT_CMD_I3; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I4"     { yylval.cmd_index = AT_CMD_I4; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I5"     { yylval.cmd_index = AT_CMD_I5; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I6"     { yylval.cmd_index = AT_CMD_I6; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"I7"     { yylval.cmd_index = AT_CMD_I7; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GMM"     { yylval.cmd_index = AT_CMD_GMM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FCLASS"     { yylval.cmd_index = AT_CMD_FCLASS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GCI"     { yylval.cmd_index = AT_CMD_GCI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"#CLS"     { yylval.cmd_index = AT_CMD_SHARP_CLS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPDUMMYKPD"     { yylval.cmd_index = AT_CMD_SPDUMMYKPD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPDUMMYKPDRSP"     { yylval.cmd_index = AT_CMD_SPDUMMYKPDRSP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPAUTOPOWER"     { yylval.cmd_index = AT_CMD_SPAUTOPOWER; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+L2CAP"     { yylval.cmd_index = AT_CMD_L2CAP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPSMSFULL"     { yylval.cmd_index = AT_CMD_SPSMSFULL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CUSD"     { yylval.cmd_index = AT_CMD_CUSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPFMMODE"     { yylval.cmd_index = AT_CMD_SPFMMODE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPFMTUNE"     { yylval.cmd_index = AT_CMD_SPFMTUNE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+RWRFREG"     { yylval.cmd_index = AT_CMD_RWRFREG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPFMTEST"     { yylval.cmd_index = AT_CMD_SPFMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPUSIMTEST"     { yylval.cmd_index = AT_CMD_SPUSIMTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ETSRESET"     { yylval.cmd_index = AT_CMD_ETSRESET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SPDATAENCRYPT"     { yylval.cmd_index = AT_CMD_xor_SPDATAENCRYPT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SPSIMLOCKDATAWRITE"     { yylval.cmd_index = AT_CMD_xor_SPSIMLOCKDATAWRITE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SPSIMLOCKIMEI"     { yylval.cmd_index = AT_CMD_xor_SPSIMLOCKIMEI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SPHUK"     { yylval.cmd_index = AT_CMD_xor_SPHUK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SPFACAUTHPUBKEY"     { yylval.cmd_index = AT_CMD_xor_SPFACAUTHPUBKEY; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SPIDENTIFYSTART"     { yylval.cmd_index = AT_CMD_xor_SPIDENTIFYSTART; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SPIDENTIFYEND"     { yylval.cmd_index = AT_CMD_xor_SPIDENTIFYEND; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSPWR"     { yylval.cmd_index = AT_CMD_SPGPSPWR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSRST"     { yylval.cmd_index = AT_CMD_SPGPSRST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSSTATUS"     { yylval.cmd_index = AT_CMD_SPGPSSTATUS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSINF"     { yylval.cmd_index = AT_CMD_SPGPSINF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSOUT"     { yylval.cmd_index = AT_CMD_SPGPSOUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSFIX"     { yylval.cmd_index = AT_CMD_SPGPSFIX; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSPDP"     { yylval.cmd_index = AT_CMD_SPGPSPDP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPBLETEST"     { yylval.cmd_index = AT_CMD_SPBLETEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+QGNSSC"     { yylval.cmd_index = AT_CMD_QGNSSC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+QGNSSRD"     { yylval.cmd_index = AT_CMD_QGNSSRD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+QGNSSCMD"     { yylval.cmd_index = AT_CMD_QGNSSCMD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSTTFF"     { yylval.cmd_index = AT_CMD_SPGPSTTFF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FOTA"     { yylval.cmd_index = AT_CMD_FOTA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SCWTEST"     { yylval.cmd_index = AT_CMD_SCWTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SCFG"     { yylval.cmd_index = AT_CMD_xor_SCFG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSASSERT"     { yylval.cmd_index = AT_CMD_SPGPSASSERT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSLTEOPEN"     { yylval.cmd_index = AT_CMD_SPGPSLTEOPEN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CACCP"     { yylval.cmd_index = AT_CMD_CACCP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAVQE"     { yylval.cmd_index = AT_CMD_CAVQE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAPRE"     { yylval.cmd_index = AT_CMD_CAPRE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAPOST"     { yylval.cmd_index = AT_CMD_CAPOST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAWTF"     { yylval.cmd_index = AT_CMD_CAWTF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAIET"     { yylval.cmd_index = AT_CMD_CAIET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CADTF"     { yylval.cmd_index = AT_CMD_CADTF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAVCT"     { yylval.cmd_index = AT_CMD_CAVCT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CAVIMS"     { yylval.cmd_index = AT_CMD_CAVIMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IMSEN"     { yylval.cmd_index = AT_CMD_IMSEN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPSELMODE"     { yylval.cmd_index = AT_CMD_SPSELMODE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPIMEI"     { yylval.cmd_index = AT_CMD_SPIMEI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SFUN"     { yylval.cmd_index = AT_CMD_SFUN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPTEST"     { yylval.cmd_index = AT_CMD_SPTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPTESTMODEM"     { yylval.cmd_index = AT_CMD_SPTESTMODEM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPDATA"     { yylval.cmd_index = AT_CMD_SPDATA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CEMODE"     { yylval.cmd_index = AT_CMD_CEMODE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GLPDNS"     { yylval.cmd_index = AT_CMD_GLPDNS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGDSCONT"     { yylval.cmd_index = AT_CMD_CGDSCONT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CANXP"     { yylval.cmd_index = AT_CMD_CANXP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLIP"     { yylval.cmd_index = AT_CMD_CLIP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CLIR"     { yylval.cmd_index = AT_CMD_CLIR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COLP"     { yylval.cmd_index = AT_CMD_COLP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COLR"     { yylval.cmd_index = AT_CMD_COLR; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPACTCARD"     { yylval.cmd_index = AT_CMD_SPACTCARD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"^SYSCONFIG"     { yylval.cmd_index = AT_CMD_xor_SYSCONFIG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPLTEDUMMYPARA"     { yylval.cmd_index = AT_CMD_SPLTEDUMMYPARA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CCFCU"     { yylval.cmd_index = AT_CMD_CCFCU; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+RRTMPARAM"     { yylval.cmd_index = AT_CMD_RRTMPARAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+GRRLTEFREQ"     { yylval.cmd_index = AT_CMD_GRRLTEFREQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+L1PARAM"     { yylval.cmd_index = AT_CMD_L1PARAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+RRDMPARAM"     { yylval.cmd_index = AT_CMD_RRDMPARAM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPCLEANINFO"     { yylval.cmd_index = AT_CMD_SPCLEANINFO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGTFT"     { yylval.cmd_index = AT_CMD_CGTFT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGEQOS"     { yylval.cmd_index = AT_CMD_CGEQOS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CACM"     { yylval.cmd_index = AT_CMD_CACM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+XCAPRTURI"     { yylval.cmd_index = AT_CMD_XCAPRTURI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+BSF"     { yylval.cmd_index = AT_CMD_BSF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CMMS"     { yylval.cmd_index = AT_CMD_CMMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHUP"     { yylval.cmd_index = AT_CMD_CHUP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRC"     { yylval.cmd_index = AT_CMD_CRC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CRSM"     { yylval.cmd_index = AT_CMD_CRSM; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CISRVCC"     { yylval.cmd_index = AT_CMD_CISRVCC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPCHKSD"     { yylval.cmd_index = AT_CMD_SPCHKSD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CSSN"     { yylval.cmd_index = AT_CMD_CSSN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+EMMCDDRSIZE"     { yylval.cmd_index = AT_CMD_EMMCDDRSIZE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHANGERAT"     { yylval.cmd_index = AT_CMD_CHANGERAT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGSMS"     { yylval.cmd_index = AT_CMD_CGSMS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+EVTS"     { yylval.cmd_index = AT_CMD_EVTS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FACNETINFO"     { yylval.cmd_index = AT_CMD_FACNETINFO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+FACNETINFOSTA"     { yylval.cmd_index = AT_CMD_FACNETINFOSTA; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPRNDIS"     { yylval.cmd_index = AT_CMD_SPRNDIS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CHECKOPERAIN"     { yylval.cmd_index = AT_CMD_CHECKOPERAIN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+AUDCH"     { yylval.cmd_index = AT_CMD_AUDCH; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPWIQ"     { yylval.cmd_index = AT_CMD_SPWIQ; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+IMSNV"     { yylval.cmd_index = AT_CMD_IMSNV; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+RRCREL"     { yylval.cmd_index = AT_CMD_RRCREL; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPSIMLOCKCHECK"     { yylval.cmd_index = AT_CMD_SPSIMLOCKCHECK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPADCVS"     { yylval.cmd_index = AT_CMD_SPADCVS; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+PEINFO"     { yylval.cmd_index = AT_CMD_PEINFO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+WIFI"     { yylval.cmd_index = AT_CMD_WIFI; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+PING"     { yylval.cmd_index = AT_CMD_PING; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SACK"     { yylval.cmd_index = AT_CMD_SACK; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SOCKET"     { yylval.cmd_index = AT_CMD_SOCKET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+HTTP"     { yylval.cmd_index = AT_CMD_HTTP; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+NETIF"     { yylval.cmd_index = AT_CMD_NETIF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MTU"     { yylval.cmd_index = AT_CMD_MTU; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+DAPSIPERF"     { yylval.cmd_index = AT_CMD_DAPSIPERF; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+TRAFFIC"     { yylval.cmd_index = AT_CMD_TRAFFIC; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MQTTSUBUNSUB"     { yylval.cmd_index = AT_CMD_MQTTSUBUNSUB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MQTTCONN"     { yylval.cmd_index = AT_CMD_MQTTCONN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MQTTPUB"     { yylval.cmd_index = AT_CMD_MQTTPUB; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+MQTTDISCONN"     { yylval.cmd_index = AT_CMD_MQTTDISCONN; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COAP_GET"     { yylval.cmd_index = AT_CMD_COAP_GET; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COAP_POST"     { yylval.cmd_index = AT_CMD_COAP_POST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COAP_PUT"     { yylval.cmd_index = AT_CMD_COAP_PUT; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+COAP_DELETE"     { yylval.cmd_index = AT_CMD_COAP_DELETE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+ARMLOG"     { yylval.cmd_index = AT_CMD_ARMLOG; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+TESTLWM2M"     { yylval.cmd_index = AT_CMD_TESTLWM2M; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+PMMODE"     { yylval.cmd_index = AT_CMD_PMMODE; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SPGPSTEST"     { yylval.cmd_index = AT_CMD_SPGPSTEST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CGNSSST"     { yylval.cmd_index = AT_CMD_CGNSSST; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+SDRMOD"     { yylval.cmd_index = AT_CMD_SDRMOD; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }
"+CALIBINFO"     { yylval.cmd_index = AT_CMD_CALIBINFO; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }


<AT_CMD>{NUMBER}        { return NUMBER;  }
 
<AT_CMD>{STRING}        { return STRING; }

<AT_DIAL_CMD>{DIALSTRING} { BEGIN(INITIAL); return DIALSTRING; }
<AT_SHARP_CMD>{SHARPSTRING} { BEGIN(INITIAL); return SHARPSTRING; }
<AT_DTMF_CMD>{DTMFSTRING} { BEGIN(INITIAL); return DTMFSTRING; }

<<EOF>>         {return 0;}

{SPACE}         { }

.               { return yytext[0]; }
%%

