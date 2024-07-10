%{
#include <stdio.h>
#include <stdlib.h>

#include "OptLex.h"
%}

%option noyywrap

%%

"Special action"           { return KEY_SPECIAL;            }
"Macros after"             { return KEY_MACROS;             }
"/* yy_get_next"           { return KEY_YY_GET_NEXT;        }
"*yy_cp = '"               { return KEY_YY_CP;              }
"YY_RULE_SETUP"            { return KEY_YY_RULE_SETUP;      }
"YY_BREAK"                 { return KEY_YY_BREAK;           }
"case YY_END_OF_BUFFER:"   { return KEY_YY_END_OF_BUFFER;   }
"YY_FATAL_ERROR("          { return KEY_YY_FATAL_ERROR;     }
"/* Flag"                  { return KEY_FLAG;               }
"if ( yy_init )"           { return KEY_YY_INIT;            }
"ECHO"                     { return KEY_ECHO;               }
"yyterminate()"            { return KEY_YYTERMINATE;        }
"yy_hold_char"             { return KEY_YY_HOLD_CHAR;       }
"char *yy_c_buf_p"         { return KEY_YY_C_BUF_P;         }
"int yy_start"             { return KEY_YY_START;           }
"yy_match:"                { return KEY_YY_MATCH;           }
"do_action:"               { return KEY_DO_ACTION;          }
"lexical"                  { return KEY_LEXICAL;            }
"Translate"                { return KEY_TRANSLATE;          }
"Support of "              { return KEY_SUPPORT_OF;         }
"int yy_init"              { return KEY_INT_YY_INIT;        }
"yytext_ptr"               { return KEY_YYTEXT_PTR;         }
"intent"                   { return KEY_INTENT;             }

.               { }
%%