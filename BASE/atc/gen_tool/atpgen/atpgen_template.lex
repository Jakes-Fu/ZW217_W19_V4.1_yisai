%{
#include <stdio.h>
#include <stdlib.h>

#include "atpgen_from_template.h"

%}

%option noyywrap
%s TPL

%%

"###atcbegin"            { BEGIN(TPL); return TPL_BEGIN; }
"###atcend"              { BEGIN(INITIAL); return TPL_END; }
<TPL>"##atcname"         { return TPL_ATCNAME; }
<TPL>"##atcnamenoplus"   { return TPL_ATCNAMENOPLUS; }
<TPL>"##atcindex"        { return TPL_ATCINDEX; }
<TPL>"##atctype"         { return TPL_ATCTYPE; }
<TPL>"##atcparamnum"     { return TPL_ATCPARAMNUM; }
"\n"					 { return 10; }
.                        { return yytext[0]; }

%%
