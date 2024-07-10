#!/bin/bash

echo "linux_atc_gen"
./gen_tool/atpgen ./template/atc_gsmonly.atd ./template/atc_gsmonly_info.c.tpl ./template/atc_gsmonly.lex.tpl ./template/atc_gsmonly_def.h.tpl

chmod -R +w../source/c/atc_gsmonly_info.c
chmod -R +w../source/c/atc_gsmonly_lex.c
chmod -R +w../source/c/atc_yacc.c
chmod -R +w../source/h/atc_gsmonly_def.h
chmod -R +w./lex_yacc/atc_gsmonly.lex

mv ./template/atc_gsmonly_def.h ./../Source/h
mv ./template/atc_gsmonly_info.c ./../Source/c
mv ./template/atc_gsmonly.lex ./lex_yacc 

#rem 切换当前目录到../ms_code/atc/tools/lex_yacc/bison_flex
cd ./lex_yacc/bison_flex

#rem flex命令用于词法解析
$(./flex.exe -Li ../atc_gsmonly.lex) 
stty echo ##开启回显

#rem bison命令用于语法解析
set BISON_SIMPLE=./bison.simple
bison -d ../atc_linux.y
mv ./lex.yy.c ./..

#rem 切换当前目录到../ms_code/atc/tools
cd ./../..

./gen_tool/OptLex ./lex_yacc/lex.yy.c atc_gsmonly_lex.c 
#rem ./gen_tool/OptYacc ./lex_yacc/atc_linux.tab.c atc_yacc.c

mv atc_gsmonly_lex.c ./../Source/c
#rem mv atc_yacc.c ./../Source/c

rm  ./lex_yacc/lex.yy.c
rm  ./lex_yacc/bison_flex/atc.tab.c
rm  ./lex_yacc/bison_flex/atc.tab.h
rm  atc_lex.tmp.c
#rem rm  atc_yacc.tmp.c

#rem pause
#rem exit