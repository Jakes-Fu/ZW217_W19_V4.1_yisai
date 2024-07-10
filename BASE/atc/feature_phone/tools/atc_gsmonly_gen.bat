.\gen_tool\atpgen .\template\atc_gsmonly.atd .\template\atc_gsmonly_info.c.tpl .\template\atc_gsmonly.lex.tpl .\template\atc_gsmonly_def.h.tpl

attrib -r ..\source\c\atc_gsmonly_info.c
attrib -r ..\source\c\atc_gsmonly_lex.c
attrib -r ..\source\c\atc_yacc.c
attrib -r ..\source\h\atc_gsmonly_def.h
attrib -r .\lex_yacc\atc_gsmonly.lex

move .\template\atc_gsmonly_def.h .\..\Source\h
move .\template\atc_gsmonly_info.c .\..\Source\c
move .\template\atc_gsmonly.lex .\lex_yacc 

@rem 切换当前目录到..\ms_code\atc\tools\lex_yacc\bison_flex
cd .\lex_yacc\bison_flex

@rem flex命令用于词法解析
flex -Li ..\atc_gsmonly.lex 

@rem bison命令用于语法解析
set BISON_SIMPLE=.\bison.simple
bison -d ..\atc.y
move .\lex.yy.c .\..

@rem 切换当前目录到..\ms_code\atc\tools
cd .\..\..

.\gen_tool\OptLex .\lex_yacc\lex.yy.c atc_gsmonly_lex.c 
@rem .\gen_tool\OptYacc .\lex_yacc\atc.tab.c atc_yacc.c

move atc_gsmonly_lex.c .\..\Source\c
@rem move atc_yacc.c .\..\Source\c

del .\lex_yacc\lex.yy.c
del .\lex_yacc\atc.tab.c
del .\lex_yacc\atc.tab.h
del atc_lex.tmp.c
@rem del atc_yacc.tmp.c

@rem pause
@rem exit