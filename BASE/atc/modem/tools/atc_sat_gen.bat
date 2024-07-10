.\gen_tool\atpgen .\template\atc_sat.atd .\template\atc_sat_info.c.tpl .\template\atc_sat.lex.tpl .\template\atc_sat_def.h.tpl

attrib -r ..\source\c\atc_sat_info.c
attrib -r ..\source\c\atc_sat_lex.c
attrib -r ..\source\c\atc_yacc.c
attrib -r ..\source\h\atc_sat_def.h
attrib -r .\lex_yacc\atc_sat.lex

move .\template\atc_sat_def.h .\..\Source\h
move .\template\atc_sat_info.c .\..\Source\c
move .\template\atc_sat.lex .\lex_yacc 

@rem 切换当前目录到..\ms_code\atc\tools\lex_yacc\bison_flex
cd .\lex_yacc\bison_flex

@rem flex命令用于词法解析
flex -Li ..\atc_sat.lex 

@rem bison命令用于语法解析
set BISON_SIMPLE=.\bison.simple
bison -d ..\atc.y
move .\lex.yy.c .\..

@rem 切换当前目录到..\ms_code\atc\tools
cd .\..\..

.\gen_tool\OptLex .\lex_yacc\lex.yy.c atc_sat_lex.c 
.\gen_tool\OptYacc .\lex_yacc\atc.tab.c atc_yacc.c

move atc_sat_lex.c .\..\Source\c
move atc_yacc.c .\..\Source\c

del .\lex_yacc\lex.yy.c
del .\lex_yacc\atc.tab.c
del .\lex_yacc\atc.tab.h
del atc_lex.tmp.c
del atc_yacc.tmp.c

@rem pause
@rem exit