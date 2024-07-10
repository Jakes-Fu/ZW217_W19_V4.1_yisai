/****************************************************************************
** File Name:      mmiebook_bookshelf.h                              
** Author:         ying.xu                                                        
** Date:           2011/21/12 
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.       
** Description:    This file is used to read/write text file
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 12/21/2011         ying.xu          Create
** 
****************************************************************************/

#ifndef _MMIEBOOK_BOOKSHELF_H_
#define _MMIEBOOK_BOOKSHELF_H_
#ifdef EBOOK_SUPPORT
#include "mmigrid.h"


#ifdef __cplusplus
extern   "C"
{
#endif


        
//#define MMIEBOOK_BOOKSHELF_LAYER_V              34
#define MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_V  3
    
#define MMIEBOOK_BOOKSHELF_LAYER_PER_SCREEN_V 3

//#define MMIEBOOK_BOOKSHELF_LAYER_H    26
#define MMIEBOOK_BOOKSHELF_BLOCK_PER_LAYER_H  4
    
#define MMIEBOOK_BOOKSHELF_LAYER_PER_SCREEN_H 2
    
//  Description : book shelf struct defination
typedef struct
{
    GUI_RECT_T   rect;  //�����LCD
    int32       book_num;
    int32       focused_book_index; //����focused״̬��book index   
    int32       layer_num ;
    MMIGRID_T        grid;
    //��2����grid�еĶ������ظ�����Ҫ��ʹ�÷�Χ�Ĳ�ͬ
    MMI_MESSAGE_ID_E    tp_state ;             //tp down/up��
    GUI_POINT_T         prevous_tp_point ;      //��һ��TP��Ϣ������  
}MMIEBOOK_BOOKSHELF_T;

PUBLIC MMI_HANDLE_T MMIEBOOK_OpenBookListWin(ADD_DATA add_data_ptr);
    
#ifdef __cplusplus
    extern   "C"
    {
#endif
#endif       
#endif
        
