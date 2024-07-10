#!/bin/bash

Svn=/usr/bin/svn

up_to_low_name(){
	count1=0
	find $1 -type f -name "*.[h,H]" | while read line
	do			
			filename=`basename $line`			
			if [ "$filename" != "`echo $filename|tr [A-Z] [a-z] `" ]
			then				
				echo $line
				cd `dirname $line`
				echo "$PWD"
				$Svn ren $filename `echo $filename|tr [A-Z] [a-z] `
				#let count1+1
				#mv $line `echo $line|tr [a-z] [A-Z] `    ##小写转大写 for debug
			fi
			cd $rootDir
			
	done
	#echo "总计修改$count1处"
 }
 
up_to_low_content(){
	count=0            ##总计修改
	find $1 -name "*.[c,h]" |while read line 
		do 
			echo $line
			#sed -i '/^#include.*\.h\"$/{s/[A-Z]/\l&/g}' $line  ##匹配以#include开头 以.h“结尾的文件
			for NUM in `sed -n '/^#include.*\.h\"$/{=;}' $line`
				do
					echo "$NUM"
					filename=`sed -n "$NUM"p $line|awk -F '\' '{print $NF}'|awk -F '"' '{print $1}'`
					#echo "filename=$filename"
					filelname2=`echo $filename|awk -F '.' '{print $1}'|tr [A-Z] [a-z]`.h
					#echo "filename2=$filelname2"
					sed -i $NUM{s/"$filename"/"$filelname2"/g}  $line	
					let count++
			done
			#sed -i '/^#include/{s/[a-z]/\u&/g}' $line  ##小写转大写 for debug
	done
	#echo "总计修改$count处"
}



char_change(){
	find $1 -name "*.[c,h]" |while read line 
		do 
			echo $line
			sed -i '/^#include.*\.h\"$/{s:\\\\:\/:g}' $line  ##完全匹配以#include开头 以.h"结尾 匹配\\转换成/
			sed -i '/^#include.*\.h\"$/{s:\\:\/:g}' $line  ##完全匹配以#include开头 以.h"结尾 匹配\转换成/ 
			#sed -i '/^#include/{s:\/:\\:g}' $line  ##反转 for debug
	done
}

num_up_low_name=1
num_up_low_content=2
num_char_change=3

if [ $# != 1 ] ; then
	echo " e.g.: $0 dirpath"
	exit 1;
fi

if [ -d "$1" ];then
	
	echo "文件夹存在，请输入功能选项"
	echo "1.修改当前目录头文件为小写"
	echo "2.修改当前目录.c .h的include的头文件为小写"
	echo "3.修改当前目录.c和.h文件中，以#include 开头，以.h结尾的所在行中的\的替换成/ "
	Flagnum=1
	while [ $Flagnum -gt 0 ]
	do
		read  num
		if [[ $num =~ ^-?[0-9]+$ ]]; then		
				if [ $num -lt 4 ]
					then
						if [ $num -gt 0 ]
							then
							echo "正在执行选项${num}"
							let Flagnum--
						fi
				else
					echo "选项输入错误，请重新输入" 
				fi	
		else
			echo "选项输入错误，请重新输入"
		fi
	done
else
	echo "文件夹不存在"
	exit 1;
fi

cd $1

rootDir=$PWD

if [ $num -eq $num_up_low_name ]   ##功能1 文件名修改
	then
		up_to_low_name		
	
elif [ $num -eq $num_up_low_content ] ##功能2 文件内大小写修改
	then
		up_to_low_content	
	
elif [ $num -eq $num_char_change ] ##功能2 字符修改
	then
		char_change
fi

echo "选项$num执行完成"
	
