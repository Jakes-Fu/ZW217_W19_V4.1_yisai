/****************************************************************************
** File Name:      mmicom_data.h                                            *
** Author:                                                                 *
** Date:           1/4/2008                                                 *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2008         jianma            Create
** 
****************************************************************************/
#ifndef MMICOM_DATA_H
#define MMICOM_DATA_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/


#include "sci_types.h"
#include "caf.h"
#include "mmk_type.h"

/* Bcd Values for Dial Number storage */
#define   DIALBCD_0                 0x0
#define   DIALBCD_1                 0x1
#define   DIALBCD_2                 0x2
#define   DIALBCD_3                 0x3
#define   DIALBCD_4                 0x4
#define   DIALBCD_5                 0x5
#define   DIALBCD_6                 0x6
#define   DIALBCD_7                 0x7
#define   DIALBCD_8                 0x8
#define   DIALBCD_9                 0x9
#define   DIALBCD_STAR              0xA
#define   DIALBCD_HASH              0xB
#define   DIALBCD_PAUSE             0xC
#define   DIALBCD_WILD              0xD
#define   DIALBCD_EXPANSION         0xE
#define   DIALBCD_FILLER            0xF

//#define UNICODE_ELLIPSIS                  "\x00\x2e\x00\x2e\x00\x2e"    //"..."
//#define UNICODE_ELLIPSIS_LEN              6
/*
 * BCD format - LSB first (1234 = 0x21 0x43), MSB first (1234 = 0x12, 0x34)
 * or unpacked (1 digit per byte).
 */
#define PACKED_LSB_FIRST    0  // LSB first (1234 = 0x21, 0x43)
#define PACKED_MSB_FIRST    1  // MSB first (1234 = 0x12, 0x34)
#define UNPACKED            2  // unpacked  (1234 = 0x01, 0x02, 0x03, 0x04)

#define MMI_COM_OFFSET                         16
#define MMI_COM_LOW_WORD                   0x0000ffff
#define MMI_COM_HIGH_WORD                  0xffff0000
/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
#define UTF8_VALID1(p) \
    ( 0 == ((p)[0] & 0x80) )

#define UTF8_VALID2(p) \
    ( 0xc0==( (p)[0]&0xe0 ) &&  (0x80== ( (p)[1]&0xc0) )  )

#define UTF8_VALID3(p) \
    ( (0xe0==( (p)[0]&0xf0) ) &&  (0x80==( (p)[1]&0xc0) ) && (0x80== ( (p)[2]&0xc0) ) )

#define UTF8_VALID4(p) \
    ( (0xf0==( (p)[0]&0xf8) ) &&  (0x80==( (p)[1]&0xc0) ) && (0x80==( (p)[2]&0xc0) ) && (0x80==( (p)[3]&0xc0) ) )
#define MATH_ABS(x) ((x) < 0 ? ((x) * (-1)) : (x))

#define SUB_ABS(x, y)   (((x) > (y)) ? ((x) - (y)) : ((y) - (x)))

#define MMICOM_GET_KBYTE(x)           (x / 1024)
#define MMICOM_GET_PERCENTKBYTE(x)    (((x % 1024) * 1000) / 1024)

#define MMICOM_GET_BE_FROM_LE_UINT32(X)\
            ((X) & 0x000000FF) << 24 |\
            ((X) & 0x0000FF00) << 8 |\
            ((X) & 0x00FF0000) >> 8 |\
            ((X) & 0xFF000000) >> 24;

#define MMICOM_GET_WORD_HB(X)         (uint16)(((X) >> MMI_COM_OFFSET) & MMI_COM_LOW_WORD)
#define MMICOM_GET_WORD_LB(X)         (uint16)((X) & MMI_COM_LOW_WORD)



/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get the length of the bcd code
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetBcdLen(
                       uint8 bcdFmt, // BCD码的类型，目前有三种: PACKED_LSB_FIRST, PACKED_MSB_FIRST, UNPACKED
                       uint8 *pBcd,  // BCD码的头指针
                       uint8 *pLen   // 返回BCD码的长度
                      );

/*****************************************************************************/
//  Description : convert the bcd code to string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BcdToStr(
                      uint8 bcdFmt, // BCD码的类型，目前有三种: PACKED_LSB_FIRST, PACKED_MSB_FIRST, UNPACKED
                      uint8 *pBcd,  // BCD码的头指针
                      uint8 bcdLen, // BCD码的长度
                      char* str     // 转换后String的头指针
                     );

#ifdef _SW_ZDT_PRODUCT_
PUBLIC void MMIAPICOM_BcdICCIDToStr
(
    uint8*    pBcd,    // BCD码的头指针
    uint8    bcdLen,    // BCD码的长度
    char*    str        // 转换后String的头指针
);
#endif

/*****************************************************************************/
//  Description : convert the bcd code to string
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_NetBcdToStr(
                  uint8 bcdFmt, // BCD码的类型，目前有三种: PACKED_LSB_FIRST, PACKED_MSB_FIRST, UNPACKED
                  uint8 *pBcd,  // BCD码的头指针
                  uint8 bcdLen, // BCD码的长度
                  char* str     // 转换后String的头指针
                 );

/*****************************************************************************/
//  Description : convert the bcd code(PACKED_LSB_FIRST) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BcdLfToStr(
                        uint8 *pBcd,  // BCD码的头指针
                        uint8 bcdLen, // BCD码的长度
                        char* str     // 转换后String的头指针
                       );

/*****************************************************************************/
//  Description : convert the bcd code(PACKED_MSB_FIRST) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BcdMfToStr(
                        uint8 *pBcd,  // BCD码的头指针
                        uint8 bcdLen, // BCD码的长度
                        char* str     // 转换后String的头指针
                       );

/*****************************************************************************/
//  Description : convert the bcd code(UNPACKED) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BcdUpToStr(
                        uint8 *pBcd,  // BCD码的头指针
                        uint8 bcdLen, // BCD码的长度
                        char *str     // 转换后String的头指针
                       );

/*****************************************************************************/
//  Description : convert the bcd code(PACKED_LSB_FIRST) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_NetBcdLfToStr(
                        uint8 *pBcd,  // BCD码的头指针
                        uint8 bcdLen, // BCD码的长度
                        char* str     // 转换后String的头指针
                       );

/*****************************************************************************/
//  Description : convert the bcd code(PACKED_MSB_FIRST) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_NetBcdMfToStr(
                        uint8 *pBcd,  // BCD码的头指针
                        uint8 bcdLen, // BCD码的长度
                        char* str     // 转换后String的头指针
                       );

/*****************************************************************************/
//  Description : convert the bcd code(UNPACKED) to the string
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_NetBcdUpToStr(
                        uint8 *pBcd,  // BCD码的头指针
                        uint8 bcdLen, // BCD码的长度
                        char *str     // 转换后String的头指针
                       );

/*****************************************************************************/
//  Description : check this the sting can be convert to bcd number
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: the pStr can be convert to bcd number
//          FALSE:the pStr include invalid string, can not to be convert to bcd number
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsValidNumberStringEx(MMI_STRING_T *num_string);

/*****************************************************************************/
//  Description : check this the sting can be convert to bcd number
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: the pStr can be convert to bcd number
//          FALSE:the pStr include invalid string, can not to be convert to bcd number
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsValidNumberString(
                      char *pStr,   // String的头指针
                      uint8 strLen  // String的长度
                     );

/*****************************************************************************/
//  Description : convert the string to the bcd code
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrToBcd(
                      uint8 bcdFmt, // BCD码的类型，目前有三种: PACKED_LSB_FIRST, PACKED_MSB_FIRST, UNPACKED
                      char *pStr,   // String的头指针
                      uint8 *pBcd   // 转换后BCD码的头指针
                     );

/*****************************************************************************/
//  Description : convert the string to the bcd code(PACKED_LSB_FIRST)
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrToBcdLf(
                        char *pStr,   // String的头指针
                        uint8 strLen, // String的长度
                        uint8 *pBcd   // 转换后BCD码的头指针
                       );

/*****************************************************************************/
//  Description : convert the string to the bcd code(PACKED_MSB_FIRST)
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrToBcdMf(
                        char *pStr,   // String的头指针
                        uint8 strLen, // String的长度
                        uint8 *pBcd   // 转换后BCD码的头指针
                       );

/*****************************************************************************/
//  Description : convert the string to the bcd code(UNPACKED)
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_StrToBcdUp(
                        char *pStr,   // String的头指针
                        uint8 strLen, // String的长度
                        uint8 *pBcd   // 转换后BCD码的头指针
                       );

/*****************************************************************************/
//  Description : 得到a除以b的商
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Divide(
              uint16 a,
              uint16 b
              ) ;

/*****************************************************************************/
//  Description : 得到a除以b的余数
//  Global resource dependence : 
//  Author: Jibin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Remainder(
                 uint16 a,
                 uint16 b
                 );

/******************************************************************************/
//  Description : draw softkey and clock
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
PUBLIC int MMIAPICOM_SIN_S(int angle);

/******************************************************************************/
//  Description : draw softkey and clock
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
PUBLIC int MMIAPICOM_COS_S(int angle);

/*****************************************************************************/
//  Description : to get quadrantalpha
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIAPICOM_GetQuadrantAlpha(int angle);
#if 0
/*****************************************************************************/
//  Description : 计算电子书名的哈希数
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:用于详情文件中电子书的索引表
//  Return:16位的哈希数
/*****************************************************************************/                      
PUBLIC uint16 MMIAPICOM_MakeTxtNameHash(
                             wchar* name,//IN: 书名
                             uint16 name_len//IN: 书名字节数
                             );
#endif
/*****************************************************************************/
//  Description : to Count the ceiling of a value
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_CeilValue(                       //RETURN:
                      uint16    total_len,  //IN:
                      uint8     split_len   //IN:
                      );

/*****************************************************************************/
//  Description : simulate float divid
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_FloatDivid(
                    uint32  num1, 
                    uint32  num2
                    );

/*****************************************************************************/
//  Description : simulate float multiple
//  Global resource dependence : 
//  Author: zhaohui
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_FloatMultiple(
                      uint32    num1, 
                      uint32   num2
                      );

/*****************************************************************************/
//  Description : This functions trace data,
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_TraceData(unsigned char *buf, int len);

/*****************************************************************************/
//  Description : This functions switch binary and hex char,
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_BinaryToHexChar(uint8* src_ptr, uint16 src_len, uint8* dest_ptr, uint16* dest_len);

/*****************************************************************************/
//  Description : This functions switch Hex char to binary
//  Global resource dependence : none
//  Author:      
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_HexCharToBinary(uint8* src_ptr, int src_len, uint8* dest_ptr, uint16* dest_len);

/*****************************************************************************/
//  Description : This functions switch Hex char to Byte
//  Global resource dependence : none
//  Author:      
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_HexCharToByte(uint8 cValue);
/*****************************************************************************/
//  Description : This functions judge if the char is in the telephone number
//  Global resource dependence : none
//  Author:      
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsTelephoneNum(uint8 cValue);

/*****************************************************************************/
//  Description : align 4
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetAlign4Offset(uint32 x);

/*****************************************************************************/
//  Description : align 8
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICOM_GetAlign8Offset(uint32 x);
#endif 
