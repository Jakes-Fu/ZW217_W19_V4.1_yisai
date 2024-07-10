#!/usr/bin/python
# -*- coding: UTF-8 -*-
import xml.etree.ElementTree as ET
from pickle import FALSE, NONE, TRUE
import xlrd
import os
from xml.dom.minidom import parseString
import dicttoxml
'''
def 
'''
Llang = []
mapUsedlang = {}
m_mapStrTable = {}
'''
function def
'''
def CheckStrTableHdr(sheet,agLangColIndex,nCurRow,nRows,nCols,strTableName,strSheetName,bChangMapUsedLang,langinfo):
    strCell = ''
    szName = langinfo
    #print(szName)
    for i in range(0,nRows):
        aRow = sheet.row_values(i)
        if len(aRow) != 0:
            nCurRow = i + 1
            break
   # print(aRow)
   # print(i)
   # if i >=nRows:
    #    return 1

    #for i in range(0,len(aRow)):
       # strCell = ''.join(aRow[i])
        #if len(strCell) != 0:
           # break
    #print(i)
   # if i>len(aRow):
       # return 1
    #print(nCols)
    #check
    for i in range(0,nCols):
        strCell = aRow[i]
        #print(strCell)
        if len(strCell) == 0:
            #print('len(strCell) == 0')
            continue
        if i == 0:
            if(strCell != 'ID'):
                print('ID error in sheet %s',strSheetName)
        else:
            for j in range(0,len(szName)):
                strValue = ''
                strKey = szName[j]
                if strCell.lower() == strKey.lower():
                    dwIndex = i | (j << 16)
                    strValue = mapUsedlang.get(strKey)
                    if (strValue == None) and bChangMapUsedLang:
                        agLangColIndex.append(dwIndex)
                        mapUsedlang[strCell] = strCell
                    if (strValue != None) and (bChangMapUsedLang == FALSE):
                        agLangColIndex.append(dwIndex)
                    break
            if len(agLangColIndex) == len(szName):
                break
    return 0

def loadAllString(path,langinfo):
    if os.path.exists(path) == False:
        print('error:no find %s file', path)
    #open excel
    book  = xlrd.open_workbook(path)
    #get sheet name
    agSheets = book.sheet_names()
    #print(agSheets)
    nSheetName = 1 ## 应该是在ini文件中配置，目前只有default,history sheet 页
    bHistory = FALSE
    bFirst = TRUE
    for k in range(0,nSheetName):
        if k==0:
            strSheetName = 'default'
        if k==1:
            strSheetName = 'history'
        if strSheetName == 'history':
            bHistory = TRUE
        else:
            bHistory = FALSE
        sheet = book.sheet_by_name(strSheetName)
        nCols = sheet.ncols
        nRows = sheet.nrows
        nCurRow = 0
        #check row and col count
        if nRows <= 1:
            continue
        if nCols <=1:
            continue
        agLangColIndex = []
        ret = CheckStrTableHdr(sheet,agLangColIndex,nCurRow,nRows,nCols,path,strSheetName,bFirst,langinfo)
        print('CheckStrTableHdr res is',ret)
        #print(nCurRow)
        if ret == 1:
            continue
        bFirst = FALSE
        #print('agLangColIndex is %d', len(agLangColIndex))
        if len(agLangColIndex) == 0:
            continue
        pStrMap = {}
        if m_mapStrTable.get("DEFAULT") != None:
            pStrMap = m_mapStrTable["DEFAULT"]
        else:
            m_mapStrTable["DEFAULT"] = pStrMap
        mapStrId = {}
        for i in range(1,nRows):
            aRow = sheet.row_values(i)
            if len(aRow) == 0:
                continue
            strID = ''
            strID = aRow[0]
            if len(strID) == 0:
                continue
            strTmp = ''
            if mapStrId.get(strID) == strTmp:
                print('error str id [%s] is exist')
                continue
            else:
                mapStrId[strID] = strTmp
            pagstr = []
            if pStrMap.get(strID) == None:
                for j in range(0,len(langinfo)):
                    pagstr.append('')
            else:
                pagstr = pStrMap.get(strID)
            for j in range(0,len(agLangColIndex)):
                nIdx = agLangColIndex[j]
                low = nIdx & 0x00ff
                high = (nIdx>>16) & 0x00ff
                if aRow[low] != '':
                    if isinstance(aRow[low],float) and aRow[low] == int(aRow[low]):
                       aRow[low] = int(aRow[low])
                    pagstr[high] = aRow[low]
            pStrMap[strID] = pagstr
        mapStrId.clear()
#dict to xml
def dict2xml(mapTable,root):
    beforexml = dicttoxml.dicttoxml(mapTable,custom_root=root)
    dom = parseString(beforexml)
    prexml = dom.toprettyxml(indent=' ')
    return prexml
#save xml file
def saveXMLfile(data,path):
    f = open(path,'wb+')
    f.write(data.encode('utf-8'))
    f.close()
# rect table to rect xml 
def loadAllRect(rectMap,path):
    #open excel
    book  = xlrd.open_workbook(path)
    #get sheet name
    agSheets = book.sheet_names()
    strSheetName = 'default'
    sheet = book.sheet_by_name(strSheetName)
    nCols = sheet.ncols
    nRows = sheet.nrows
    nCurRow = 0
    #check row and col count
    if nRows <= 1:
        return 0
    if nCols <5:
        return 0
    #get first valid row data
    for i in range(0,nRows):
        aRow = sheet.row_values(i)
        if len(aRow) != 0:
            nCurRow = i + 1
            break
    #check
    if aRow[0] != 'ID' or aRow[1] != 'left' or aRow[2] != 'top' or aRow[3] != 'right' or aRow[4] != 'bottom':
        print('color_table.xls the header is not correct.')
        return 0
    # to string
    for i in range(0,len(aRow)):
        strCell = ''.join(aRow[i])
        if len(strCell) != 0:
            break
    pRecMap = {}
    rectMap[strSheetName.upper()] = pRecMap
    #遍历每一行数据
    for i in range(1,nRows):
        aRow = sheet.row_values(i)
        if len(aRow) == 0:
            continue
        strID = ''
        strID = aRow[0]
        if len(strID) == 0:
            continue
        strTmp = ''
        if aRow[1] == '' or aRow[2] == '' or aRow[3] == '' or aRow[4] == '':
            print('error rect_table.xls  [%s]  id is  [%s]  there exist emptu value',strSheetName,strID)
            continue
        mapStrId = {}
        strID.upper()
        if mapStrId.get(strID) == strTmp:
            print('error rect id [%s] is exist',strID)
            continue
        else:
            mapStrId[strID] = strTmp
        pagrect = []
        pagrect.append(round(aRow[1]))
        pagrect.append(round(aRow[2]))
        pagrect.append(round(aRow[3]))
        pagrect.append(round(aRow[4]))
        pRecMap[strID] = pagrect
    mapStrId.clear()

def loadAllColor(colorMap,path):
    #open excel
    book  = xlrd.open_workbook(path)
    #get sheet name
    agSheets = book.sheet_names()
    strSheetName = 'default'
    sheet = book.sheet_by_name(strSheetName)
    nCols = sheet.ncols
    nRows = sheet.nrows
    nCurRow = 0
    #check row and col count
    if nRows <= 1:
        return 0
    if nCols <=1:
        return 0
    #get first valid row data
    for i in range(0,nRows):
        aRow = sheet.row_values(i)
        if len(aRow) != 0:
            nCurRow = i + 1
            break
    #check
    if aRow[0] != 'ID' or aRow[1] != 'color':
        print('color_table.xls the header is not correct.')
        return 0
    # to string
    for i in range(0,len(aRow)):
        strCell = ''.join(aRow[i])
        if len(strCell) != 0:
            break
    pColMap = {}
    colorMap[strSheetName.upper()] = pColMap
    #遍历每一行数据
    for i in range(1,nRows):
        aRow = sheet.row_values(i)
        if len(aRow) == 0:
            continue
        strID = ''
        strID = aRow[0]
        if len(strID) == 0:
            continue
        strTmp = ''
        if aRow[1] == '':
            print('error color_table.xls  [%s]  id is  [%s]  there exist emptu value',strSheetName,strID)
            continue
        mapStrId = {}
        strID.upper()
        if mapStrId.get(strID) == strTmp:
            print('error color id [%s] is exist',strID)
            continue
        else:
            mapStrId[strID] = strTmp

        pColMap[strID] = aRow[1]
    mapStrId.clear()  
        


def excel2xml(strResDir,strComPath,langinfo):
    xmlpath = []
    #1.str
    Llang = langinfo
    strTableFile = strResDir + '/str_table.xls'
    strTableFile1 = strResDir + '/str_table1.xls'
    if os.path.exists(strTableFile) == False and strComPath != '':
        strTableFile = strComPath + '/str_table.xls'
        strTableFile1 = strComPath + '/str_table1.xls'
    #load string
    print('str table file path is',strTableFile)
    loadAllString(strTableFile,langinfo)
    if len(mapUsedlang) != len(langinfo):
        loadAllString(strTableFile1,langinfo)

    strdata = dict2xml(m_mapStrTable,'str_table')
    strxmlpath = strTableFile[0:strTableFile.rfind('/')+1] + 'str_table.xml'
    saveXMLfile(strdata,strxmlpath)
    xmlpath.append(strxmlpath)
    #2.rect
    rectTableFile = strResDir + '/rect_table.xls'
    print('rect table file path is',rectTableFile)
    rectMap={}
    loadAllRect(rectMap,rectTableFile)
    rectdata = dict2xml(rectMap,'rect_table')
    rectxmlpath = rectTableFile[0:rectTableFile.rfind('/')+1] + 'rect_table.xml'
    saveXMLfile(rectdata,rectxmlpath)
    xmlpath.append(rectxmlpath)
	#3.color
    colorTableFile = strResDir + '/color_table.xls'
    colorMap={}
    print('color table file path is',colorTableFile)
    loadAllColor(colorMap,colorTableFile)
    colordata = dict2xml(colorMap,'color_table')
    colorxmlpath = colorTableFile[0:colorTableFile.rfind('/')+1] + 'color_table.xml'
    saveXMLfile(colordata,colorxmlpath)
    xmlpath.append(colorxmlpath)

    return xmlpath


    
    
