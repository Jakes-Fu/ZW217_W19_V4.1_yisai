%{
#include <stdio.h>
#include <stdlib.h>

#include "OptYacc.h"

%}

%option noyywrap

MICRO_BEGIN      ("#ifndef"|"#ifdef"|"#if")

%%

"#line"          { return KEY_PREPROCESSOR_LINE;      }
"YYDEBUG"        { return KEY_YYDEBUG;                }
{MICRO_BEGIN}    { return KEY_MICRO_BEGIN;            }
"#endif"         { return KEY_ENDIF;                  }
"#ifdef YYPURE"  { return KEY_DEF_YYPURE;             }
"#ifdef YYLSP_NEEDED"   { return KEY_YYLSP_NEEDED;           }
"yyssp >= yyss"  { return KEY_YYSSP_YYSS;             }
"! yyerrstatus"  { return KEY_YYERRSTATUS;            }
"#define YYACCEPT" { return KEY_DEFINE_YYACCEPT;      }
"bison-1.28"     { return KEY_BISON;                  }
"/*  YYMAXDEPTH" { return KEY_YYMAXDEPTH;             }
"yyacceptlab:"   { return KEY_YYACCEPTLAB;            }
"yynerrs"        { return KEY_YYNERRS;                }
"yyerrstatus"    { return KEY_YYERRSTATUS1;           }
"int yylen;"     { return KEY_INT_YYLEN;              }

.                { }

%%
