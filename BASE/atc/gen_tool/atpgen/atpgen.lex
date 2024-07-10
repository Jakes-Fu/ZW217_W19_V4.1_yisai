%{
#include <stdio.h>
#include <stdlib.h>

#include "atpgen_yacc.h"
#include "atpgen.h"

int last_start;
%}

%option noyywrap
%s ATC_DEF
%s ATC_COMMENT

CHARACTER  [0-9A-Za-z]|("!"|"#"|"$"|"%"|"&"|"'"|"*"|"+"|","|"-"|"."|"/"|":"|";"|"<"|"="|">"|"?"|"@"|"["|"]"|"^"|"_"|"`"|"|"|"~"|"\\")
ATCNAME  ("+"|"$"|"^"|"#"|"*"|"_"|"%")?{CHARACTER}+
STRING   "\""{CHARACTER}*"\""
DIGIT    [0-9]
NUMBER   ("+"|"-")?{DIGIT}+
NOUSECHAR   ([ \t\n]+)
%%

<INITIAL>{ATCNAME} { BEGIN(ATC_DEF); atp_store_string_param(); return ATC_DEF_NAME; }
<ATC_DEF>"{"       { return ATC_DEF_BEGIN; }
<ATC_DEF>"};"      { BEGIN(INITIAL); return ATC_DEF_END; }
<ATC_DEF>{STRING}  { atp_store_string_param(); return ATC_DEF_STRING; }
<ATC_DEF>{NUMBER}  { atp_store_numeric_param(); return ATC_DEF_NUMBER; }
<ATC_DEF>"..."     { return ATC_DEF_INFINITY; }
{NOUSECHAR}        { }
.                  { return yytext[0]; }

"/*"               { last_start = yy_start; BEGIN(ATC_COMMENT); }
<ATC_COMMENT>"*/"  { yy_start = last_start; }
<ATC_COMMENT>.     { }

%%

