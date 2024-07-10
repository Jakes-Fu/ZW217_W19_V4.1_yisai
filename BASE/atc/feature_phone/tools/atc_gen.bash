#!/bin/bash

echo "linux_atc_gen"
./gen_tool/atpgen.exe ./template/atc.atd ./template/atc_info.c.tpl ./template/atc.lex.tpl ./template/atc_def.h.tpl

chmod -R +w ../source/c/atc_info.c
chmod -R +w ../source/c/atc_lex.c
chmod -R +w ../source/c/atc_yacc.c
chmod -R +w ../source/h/atc_def.h
chmod -R +w ./lex_yacc/atc.lex

mv ./template/atc_def.h ./../source/h
mv ./template/atc_info.c ./../source/c
mv ./template/atc.lex ./lex_yacc 

#rem 切换当前目录到../ms_code/atc/tools/lex_yacc/bison_flex
cd ./lex_yacc/bison_flex

#rem flex命令用于词法解析 生成lex.yy.c
$(./flex.exe -Li ../atc.lex)
stty echo  ##开启回显 
#rem bison命令用于语法解析 atc.tab.c atc.tab.h
set BISON_SIMPLE=./bison.simple
bison -d ../atc_linux.y
mv ./lex.yy.c ./..

#rem 切换当前目录到../ms_code/atc/tools
cd ./../..
#atc_lex.tmp.c
./gen_tool/OptLex.exe ./lex_yacc/lex.yy.c atc_lex.c 
#rem ./gen_tool/OptYacc ./lex_yacc/atc_linux.tab.c atc_yacc.c

mv atc_lex.c ./../source/c
#rem move atc_yacc.c ./../source/c

rm ./lex_yacc/lex.yy.c
rm ./lex_yacc/bison_flex/atc_linux.tab.c
rm ./lex_yacc/bison_flex/atc_linux.tab.h
rm atc_lex.tmp.c
#rem del atc_yacc.tmp.c

#rem pause
#rem exit
