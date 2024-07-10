/******************************************************************************
                                   海信WCDMA  自主业务模块
        由于3G  网络的逐渐成熟，大量的业务需求成为第三代
        移动终端的亮点，逐渐成为不可或缺的一部分。本
        模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现在多平台、多操作系统中的PIM业务。目标
        是要达到任意平台的可移植性良好的功能稳定的模块。
        本模块的实现分为三个步骤:
        1.实现Data sync功能
        2.实现Device Management功能
        3.实现多平台的平滑移植
        模块开发参考OMA成员共同开发的SRT_c。
        目前模块计划开发支持OMA syncML 协议1.1


        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:刘刚
        开发日期:2005年4月11日。
******************************************************************************/


/******************************************************************************
文件名称:PIMbase64.h
文件描述:base64 codec functions header file.
文件说明:base64 encode/decode functions for syncml authentication and http authentication.
开发人员:刘刚
首次开发日期:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-4-11              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/

/*加入此FEATURE防止头文件重复包含*/
#ifndef   HEADER_FILE_BASE64
#define HEADER_FILE_BASE64
  /*  包含头文件*/

  /*  数据结构声明*/
  
  /*  全局变量声明*/

  /*  功能函数声明*/
/***************************************************************************/
/* The function decodes the next character of the input buffer and updates                        */
/* the pointers to the input buffer. The function skips CRLF characters                               */
/* and whitespace characters.                                                                                        */
/* Returns: 0..63, the logical value of the next valid character in the                                  */
/*                 input buffer                                                                                               */
/*          64,    padding character                                                                                    */
/*          -1,    No more characters to read                                                                      */
/*          -2,    an error occurred: invalid characters in the data stream                              */
/***************************************************************************/

int hs_nextBase64Char (unsigned char**ppbData, unsigned long *pcbDataLength);
/**
 * FUNCTION: hs_base64GetSize
 *
 *  Precalculates the size of an encoded document with the given size
 *
 * PRE-Condition:
 *  The function is called to get the size of the document that
 *  will be encoded with the hs_base64Encode() service.
 *
 * POST-Condition:
 *
 * IN: cbRealDataSize, the size of the non-encoded document.
 *
 * RETURN: BufferSize_t, the size of the encoded document that will be
 *     generated using the hs_base64Encode() service.
 *
 */

unsigned long hs_base64GetSize (unsigned long cbRealDataSize);

/**
 * FUNCTION: hs_base64Encode
 *
 *  Encodes a chunk of data according to the base64 encoding rules
 *
 * PRE-Condition:
 *  A chunk of data os copied to the source data buffer pbData, and the
 *  length of the data chunk is specified in *pcbDataLength;
 *
 * POST-Condition:
 *  A block of encoded data is available in the specified target buffer.
 *  The length of the encoded data is returned by the function.
 *
 *
 * IN: pbTarget, pointer to an allocated chunk of memory that receives the
 *               encoded data block.
 *     cbTargetSize, size of the data buffer above.
 *     bLast, flag that indicates if the specified block is the last
 *            part of the document. If the value is 0, the funciton expects
 *            that other blocks will follow, a value of 1 indicates that
 *            the data block that is presented in the input buffer is the
 *            last data block to be encoded.
 *     pbSaveBytes, pointer to a block of at least 3 Bytes. When this function
 *            is invoked the first time, the first byte of this buffer MUST
 *            be set to 0.
 * IN/OUT:
 *     pbData, pointer to a data block that contains the clear data that
 *             are to be encoded. On return, the remaining piece of the
 *             input data block that could not be encoded is copied to
 *             the memory that pbData points at.
 *     pcbDataLength, pointer to a variable that denotes the length of
 *             the data block that is to be encoded, The function updates
 *             this value with the size of the data block that could not
 *             be processed. If all data were able to be encoded, the
 *             value will be 0.
 *     pcbOffset, pointer to a variable that is internally used by the
 *             function. before the first call of base64encode() for a
 *             certain document is made, the variable that pcbOffset points
 *             at must be set to 0. The variable will be updated by the
 *             function, and should not be modified by the caller.
 * RETURN: BufferSize_t, the size of the data block that are available in
 *     pbTarget.
 *
 */

unsigned long hs_base64Encode (unsigned char* pbTarget,
                     unsigned long cbTargetSize,
                     unsigned char* pbData,
                     unsigned long *pcbDataLength,
                     unsigned long *pcbOffset,
                     unsigned int bLast,
                     unsigned char *pbSavebytes);
/**
 * FUNCTION: hs_base64Decode
 *
 *  Decodes a chunk of data according to the base64 encoding rules
 *
 * PRE-Condition:
 *  A chunk of data os copied to the source data buffer pbData, and the
 *  length of the data chunk is specified in *pcbDataLength;
 *
 * POST-Condition:
 *  A block of decoded data is available in the specified target buffer.
 *  The length of the decoded data is returned by the function.
 *
 *
 * IN: pbTarget, pointer to an allocated chunk of memory that receives the
 *               decoded data block.
 *     cbTargetSize, size of the data buffer above.
 * IN/OUT:
 *     pbData, pointer to a data block that contains the clear data that
 *             are to be decoded. On return, the remaining piece of the
 *             input data block that could not be decoded is copied to
 *             the memory that pbData points at.
 *     pcbDataLength, pointer to a variable that denotes the length of
 *             the data block that is to be decoded, The function updates
 *             this value with the size of the data block that could not
 *             be processed. If all data were able to be decoded, the
 *             value will be 0.
 * RETURN: BufferSize_t, the size of the data block that are available in
 *             pbTarget. If some invalid data were detected in the input
 *             data buffer, or if cbTargetSize is less than 3,
 *             the function returns 0. The caller should treat this as an
 *             error condition.
 *
 */

unsigned long hs_base64Decode (unsigned char* pbTarget,
                     unsigned long cbTargetSize,
                     unsigned char* pbData,
                     unsigned long *pcbDataLength);



#endif
