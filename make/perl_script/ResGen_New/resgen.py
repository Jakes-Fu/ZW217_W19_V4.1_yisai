#!/usr/bin/python
# -*- coding: UTF-8 -*-
import excelToXml as Ex
import xml.etree.ElementTree as ET
from xml.dom.minidom import parseString
import sys
import os


def showErrorMsg():
    print("Please input parameter: -xrp <\"xxx.xrp\"> -bin <\"xxx.bin\"> [-max [number]] [-rep [true|false]] [-langsplit [true|false]] [-alu [true|false]] [-crs [true|false]]\n\n")
    print("-xrp <\"xxx.xrp\">      --xrp file path\n")
    print("-bin <\"xxx.bin\">      --bin file path\n")
    print("-max [number]           --set max module number\n")
    print("-rep [true|false]       --true, replace the resource not existing with default;false, not generate bin\n")
    print("-langsplit [true|false] --true, split the each lang string to a theme; false, not split, default is false\n")
    print("-alu [true|false]         --true, all strings are stored by unicode; false, by ascii and unicode, default is true\n")
    print("-crs [true|false]         --true, remove repeat string cross lang; false, not remove, default is false\n")
    print("-gencsc [true|false]      --true, generated csc bin; false, just generate res bin, default is false\n")
    print("-font2csc [true|false]    --true, split font res to csc bin; false, not split, default is true\n")
    print("--------------------------if langsplit is true, this param is not used, it will be as default false\n")
    print("\r\n")

def callResView():
    strXrpPath = ''
    strBinPath = ''
    argc = len(sys.argv)
    for i in range(1,argc):
        if sys.argv[i] == "-xrp":
            if (i+1) < argc:
                strXrpPath = sys.argv[i+1]
            else:
                print("parameters error -xrp.\n\n")
                showErrorMsg()
                return 1
            i = i+1
        elif sys.argv[i] == "-bin":
            if (i+1) < argc:
                strBinPath = sys.argv[i+1]
            else:
                print("parameters error -bin.\n\n")
                showErrorMsg()
                return 1
            i = i+1
        elif sys.argv[i] == "-max":
            if (i+1) < argc:
                nMaxNum = sys.argv[i+1]
                if nMaxNum <=0:
                    print("parameters error -max [%s].\n\n",sys.argv[i+1])
                    showErrorMsg()
                    return 1
                i = i+1
            else:
                print("parameters error -max.\n\n")
                showErrorMsg()
                return 1;
        elif sys.argv[i] == "-rep":
            if (i+1) < argc:
                if sys.argv[i+1] == "TRUE":
                    bRep = 'TRUE'
                elif sys.argv[i+1] == "FALSE":
                    bRep = 'FALSE'
                else:
                    print("parameters error -rep [%s].\n\n",sys.argv[i+1])
                    showErrorMsg()
                    return 1
                i = i+1
            else:
                print("parameters error -rep.\n\n")
                showErrorMsg()
                return 1
        elif sys.argv[i] == "-langsplit":
            if (i+1) < argc:
                if sys.argv[i+1] == "true":
                    bSplit = 'TRUE'
                elif sys.argv[i+1] == "false":
                    bSplit = 'FALSE'
                else:
                    print("parameters error -langsplit [%s].\n\n",sys.argv[i+1])
                    showErrorMsg()
                    return 1
                i = i+1
            else:
                print("parameters error -langsplit.\n\n")
                showErrorMsg()
                return 1
        elif sys.argv[i] == "-alu":
            if (i+1) < argc:
                if sys.argv[i+1] == "TRUE":
                    bAllUnicode = 'TRUE'
                elif sys.argv[i+1] == "FALSE":
                    bAllUnicode = 'FALSE'
                else:
                    print("parameters error -alu [%s].\n\n",sys.argv[i+1])
                    showErrorMsg()
                    return 1
                i = i+1
            else:
                print("parameters error -alu.\n\n")
                showErrorMsg()
                return 1
        elif sys.argv[i] == "-crs":
            if (i+1) < argc:
                if sys.argv[i+1] == "TRUE":
                    bCrossRmString = 'TRUE'
                elif sys.argv[i+1] == "FALSE":
                    bCrossRmString = 'FALSE'
                else:
                    print("parameters error -crs [%s].\n\n",sys.argv[i+1])
                    showErrorMsg()
                    return 1
                i = i+1
            else:
                print("parameters error -crs.\n\n")
                showErrorMsg();
                return 1
        elif sys.argv[i] == "-gencsc":
            if (i+1) < argc:
                if sys.argv[i+1] == "true":
                    bGenCsc = 'TRUE'
                elif sys.argv[i+1] == "false":
                    bGenCsc = 'FALSE'
                else:
                    print("parameters error -gencsc [%s].\n\n",sys.argv[i+1])
                    showErrorMsg()
                    return 1
                i = i+1
            else:
                print("parameters error -crs.\n\n")
                showErrorMsg()
                return 1
        elif sys.argv[i] == "-font2csc":
            if (i+1) < argc:
                if sys.argv[i+1] == "true":
                    bGenFont2Csc = 'TRUE'
                elif sys.argv[i+1] == "false":
                    bGenFont2Csc = 'FALSE'
                else:
                    print("parameters error -font2csc [%s].\n\n",sys.argv[i+1])
                    showErrorMsg()
                    return 1
                i = i+1
            else:
                print("parameters error -crs.\n\n")
                showErrorMsg()
                return 1

    if strXrpPath == '':
        print("xrp file path is empty\n")
        showErrorMsg()
        return 1
    if strBinPath == '':
        print("bin file path is empty\n")
        showErrorMsg()
        return 1
    strXrpPath = strXrpPath.replace('/','\\\\')
    strBinPath = strBinPath.replace('/','\\\\')
    command = "./make/perl_script/ResGen_New/ResView.exe"
    if sys.platform == 'win32' or sys.platform == 'win64':
        command = command.replace('/','\\')
    result = os.system("%s -xrp \"%s\" -bin \"%s\" -rep %s -langsplit %s -alu %s -crs  %s -gencsc %s -font2csc %s" % (command, strXrpPath,strBinPath,bRep,bSplit,bAllUnicode,bCrossRmString,bGenCsc,bGenFont2Csc))
    print('call resview.exe return is',result)
    print(command)
    print(sys.platform)
    return result

def deleteFile(delDir):
    #delDir = "./make/perl_script/ResGen_New"
    Lfile = os.listdir(delDir)
    for f in Lfile:
        filePath = os.path.join(delDir,f)
        #print(filePath)
        if os.path.isfile(filePath):
            handname,ext = os.path.splitext(filePath)
            if ext == '.pyc':
                os.remove(filePath)
        elif os.path.isdir(filePath):
            if f == '__pycache__':
                deleteFile(filePath)
                os.rmdir(filePath)
            elif f == 'xlrd':
                deleteFile(filePath)

if __name__ == "__main__":
    #1.parase xrp file get excel file path and lang info
    tree = ET.ElementTree(file=sys.argv[2])
    root = tree.getroot()
    strResDir = ''
    strComPath = ''
    for elem in tree.iterfind('RESDIR'):
        strResDir = elem.get('url')
        strResDir = strResDir.replace('\\','/')
    strComPath = strResDir[0:strResDir.rfind('/')+1] + 'Common'
    Llang = []
    for lang in tree.iterfind('LANG/LangItem'):
        Llang.append(lang.get('name'))
    #2.create xml file
    xmlpath = Ex.excel2xml(strResDir,strComPath,Llang)
    #3.call resview.exe
    ret =  callResView()
    #4.delete xml file
    for path in xmlpath:
        os.remove(path)
    #5.delete temp file
    deleteFile('./make/perl_script/ResGen_New')
    #5.exit py
    sys.exit(ret)
