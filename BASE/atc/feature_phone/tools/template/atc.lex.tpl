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

###atcbegin
"##atcname"     { yylval.cmd_index = AT_CMD_##atcnamenoplus; BEGIN(AT_CMD); return AT_EXTENSION_CMD; }###atcend


<AT_CMD>{NUMBER}        { return NUMBER;  }
 
<AT_CMD>{STRING}        { return STRING; }

<AT_DIAL_CMD>{DIALSTRING} { BEGIN(INITIAL); return DIALSTRING; }
<AT_SHARP_CMD>{SHARPSTRING} { BEGIN(INITIAL); return SHARPSTRING; }
<AT_DTMF_CMD>{DTMFSTRING} { BEGIN(INITIAL); return DTMFSTRING; }

<<EOF>>         {return 0;}

{SPACE}         { }

.               { return yytext[0]; }
%%

