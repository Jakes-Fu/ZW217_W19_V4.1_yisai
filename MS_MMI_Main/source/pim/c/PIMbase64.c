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
文件名称:PIMbase64.c
文件描述:base64 codec functions.
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

  /*  包含头文件*/
 #ifdef WIN32
  #include "std_header.h"
 #endif
  #define _PIMBASE64_C_
  
  #include "../h/PIMLibmem.h"
  #include "../h/PIMbase64.h"

  /*  数据结构声明*/
  #define MAX_COLUMNS 45
  /*  全局变量声明*/

  /*  功能函数*/
  

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

int hs_nextBase64Char (unsigned char**ppbData, unsigned long *pcbDataLength)
   {
   int r = -1;
   unsigned char* pbData = *ppbData;
   unsigned long cbDataLength = *pcbDataLength;
   static char *pszSkipChars = "\t\r\n ";
   char ch;

   if (cbDataLength == 0) return r;

   do {
      ch = *pbData;
      if ((ch>='0')&&(ch<='9'))      r = (int)ch+4;  //  |
      else if ((ch>='A')&&(ch<='Z')) r = (int)ch-65; //  |
      else if ((ch>='a')&&(ch<='z')) r = (int)ch-71; //  | Valid characters
      else if (ch == '/')            r = 63;         //  |
      else if (ch == '+')            r = 62;         //  |
      else if (ch == '=')            r = 64;         // padding character
      else if (!xppStrchr (pszSkipChars, ch))  r = -2; // invalid character
      cbDataLength --;
      pbData ++;
      } while ((r == -1) && (cbDataLength > 0));

   /***************************************************/
   /* Pass the updated parameter values to the caller */
   /***************************************************/
   if (r != -1)
      {
      *ppbData = pbData;
      *pcbDataLength = cbDataLength;
      }



   
   return r;
   }

  
unsigned long hs_base64GetSize (unsigned long cbRealDataSize)
   {
   unsigned long iMod = cbRealDataSize % 3;
   /* The encoded data size ... */
   unsigned long cbEncodedSize = ((cbRealDataSize - iMod) / 3 ) * 4;/*lint !e737*/
   if (iMod != 0) cbEncodedSize += 4;
   /* ... and the # of CRLF characters */
   cbEncodedSize += ((cbEncodedSize-1) / ((MAX_COLUMNS*4)/3)) * 2;
   return cbEncodedSize;
   }


/*****************************************************************/
/* Function: pre-compute the size of the base64 encoded document */
/****************************************************************/

unsigned long hs_base64Encode (unsigned char* pbTarget,       // o: target buffer
                     unsigned long cbTargetSize,   // i: target buffer size
                     unsigned char* pbData,         // i: Data buffer
                     unsigned long *pcbDataLength, // i/o: Data buffer size
                     unsigned long *pcbOffset,     // i/o: absolute # of bytes encoded so far
                     unsigned int bLast,          // i: 0=first block, 1= next block, 2=last block
                     unsigned char *pbSaveBytes)  // i/o: last incomplete data block
   {
   unsigned char* pbSourceData = pbData;
   unsigned long cbCopySize = 0;
   unsigned long cbDataProcessed = *pcbDataLength;
   unsigned int i0, i1, i2, i3;
   unsigned int byt;
   int iSave = 1;
   static char t [] =   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"    // 26
                        "abcdefghijklmnopqrstuvwxyz"    // 26
                        "0123456789+/"                  // 12
                        "=";                            // 1
   i0=i1=i2=i3=0;
   // Check for NULL data buffer,
   if (pbData == NULL ) {
       // See if last block and there is any "saved" data that needs to go now.
       if ( bLast && ( pbSaveBytes && pbSaveBytes [0] )) {
          /**************************************/
          /* Check if it is time to send a CRLF */
          /**************************************/
          if ((*pcbOffset) > 0 && ((*pcbOffset) % MAX_COLUMNS == 0))
             {
             //if (cbTargetSize < 6)        // there is not enough space in the target buffer:
             //   break;                    // return to the caller.
             *pbTarget = '\r';
             *(pbTarget+1) = '\n';
             cbCopySize += 2;
             cbTargetSize -= 2;
             pbTarget += 2;
             }

          byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --;

          i0 =  byt >> 2;
          i1 =  (byt & 0x0003) << 4;

          (*pcbOffset) ++;

#if 0//del by wangxiaolin   2009.12.20  for del warning
          if (pbSaveBytes && pbSaveBytes [0]) /*lint !e774*/
#endif
             {
              byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --;
              i1 += (byt >> 4);
              i2 =  (byt & 0x000F) << 2;
             }
#if 0//del by wangxiaolin   2009.12.20 for del warning
             else
             {
                 i2 = i3 = 64;  // index to the padding char '=';
             }
#endif

          pbTarget [0] = t[i0];
          pbTarget [1] = t[i1];
          pbTarget [2] = t[i2];
          pbTarget [3] = t[i3];

          cbCopySize += 4;
          cbTargetSize -= 4;
          pbTarget += 4;
       }

   } else {
       while ((cbTargetSize >= 4) &&
              ( ((cbDataProcessed >= 3) && (bLast == 0)) ||
                ((cbDataProcessed >  0) && (bLast == 1)) ))
          {
          /**************************************/
          /* Check if it is time to send a CRLF */
          /**************************************/
          if ((*pcbOffset) > 0 && ((*pcbOffset) % MAX_COLUMNS == 0))
             {
             if (cbTargetSize < 6)        // there is not enough space in the target buffer:
                break;                    // return to the caller.
             *pbTarget = '\r';
             *(pbTarget+1) = '\n';
             cbCopySize += 2;
             cbTargetSize -= 2;
             pbTarget += 2;
             }

          if (pbSaveBytes && pbSaveBytes [0])
             { byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --; }
          else
             { byt = (unsigned int) *pbSourceData; pbSourceData ++; cbDataProcessed --;}

          i0 =  byt >> 2;
          i1 =  (byt & 0x0003) << 4;

          (*pcbOffset) ++;

          if (cbDataProcessed > 0)
             {
             if (pbSaveBytes && pbSaveBytes [0])
                { byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --; }
             else
                { byt = (unsigned int) *pbSourceData; pbSourceData ++; cbDataProcessed --;}

             i1 += (byt >> 4);
             i2 =  (byt & 0x000F) << 2;

             (*pcbOffset) ++;

             if (cbDataProcessed > 0)
                {
                if (pbSaveBytes && pbSaveBytes [0])
                   { byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --; }
                else
                   { byt = (unsigned int) *pbSourceData; pbSourceData ++; cbDataProcessed --;}

                i2 += (byt & 0x00C0) >> 6;
                i3 =  byt & 0x003F;
                (*pcbOffset) ++;
                }
             else
                i3 = 64;  // index to the padding char '=';
             }
          else
             i2 = i3 = 64;  // index to the padding char '=';
          pbTarget [0] = t[i0];
          pbTarget [1] = t[i1];
          pbTarget [2] = t[i2];
          pbTarget [3] = t[i3];

          cbCopySize += 4;
          cbTargetSize -= 4;
          pbTarget += 4;
       }
   }



   /*************************************************************/
   /* Save the bytes that must be processed in the following    */
   /* call (max. 2 Bytes).                                      */
   /*************************************************************/
   if ((bLast == 0) && (cbDataProcessed <= 2) && (pbSaveBytes != NULL))
      {
      pbSaveBytes[0] = (unsigned char)cbDataProcessed;
      while (cbDataProcessed)
         {
         *(++pbSaveBytes) = pbSourceData[0];
         cbDataProcessed --; pbSourceData ++;
         }
      }

   /*****************************************************************/
   /* Shift all non-processed data to the start of the input buffer */
   /*****************************************************************/

   if (cbDataProcessed > 0)
      {
         if(NULL!=pbData)  //add by wangxiaolin 2009.12.20 for del warning
         {
      xppMemmove (pbData, pbSourceData, cbDataProcessed);/*lint !e668*/
         }
      }
   *pcbDataLength = cbDataProcessed;

   return cbCopySize;
   }


/***************************************************************/
/* Function: decode a base64- encoded block of data.           */
/* The function returns the count of data that are decoded, or */
/* 0 in case of a data error, or if cbTargetSize < 4           */
/***************************************************************/

unsigned long hs_base64Decode (unsigned char* pbTarget,       // o: target buffer
                     unsigned long cbTargetSize,   // i: target buffer size
                     unsigned char* pbData,         // i: data buffer
                     unsigned long *pcbDataLength) // i/o: Data buffer size
   {
   unsigned char* pbSource = pbData;
   unsigned long cbDataCopied = 0L;
   unsigned long cbRemaining = *pcbDataLength; // remaining source data
   int i0 = 0, i1 = 0, i2 = 0, i3 = 0;

   while (cbTargetSize > 0)
      {
      unsigned long cbNext = cbRemaining;
      unsigned char* pbNext = pbSource;

      i0 = hs_nextBase64Char (&pbNext, &cbNext);
      i1 = hs_nextBase64Char (&pbNext, &cbNext);
      i2 = hs_nextBase64Char (&pbNext, &cbNext);
      i3 = hs_nextBase64Char (&pbNext, &cbNext);
      if ((i0 < 0) || (i1 < 0) || (i2 < 0) || (i3 < 0))
         break; // end-of-block, or data error.

      else if ( ((cbTargetSize <= 2) && (i3 != 64)) ||
                ((cbTargetSize <= 1) && (i2 != 64)) )
         break; // end of transmission.

      else
         {
         pbSource = pbNext;
         cbRemaining = cbNext;
         /************************/
         /* decode the quadruple */
         /************************/
         *pbTarget =(unsigned char)((i0 << 2) + (i1 >> 4));
         pbTarget ++; cbDataCopied ++; cbTargetSize --;
         if (i2 != 64)
            {
            *pbTarget = (unsigned char)(((i1 & 0x000f) << 4) + (i2 >> 2));
            pbTarget ++; cbDataCopied ++; cbTargetSize --;
            if (i3 != 64)
               {
               *pbTarget = (unsigned char)(((i2 & 0x0003) << 6) + i3);
               pbTarget ++; cbDataCopied ++; cbTargetSize --;
               }
            }
         }
      }

   /*******************************/
   /* Handle invalid data errors! */
   /*******************************/

   if ((i0 == -2) || (i1 == -2) || (i2 == -2) || (i3 == -2))
      cbDataCopied = 0;

   /*****************************************************************/
   /* Shift all non-processed data to the start of the input buffer */
   /*****************************************************************/
   if (cbRemaining > 0)
      xppMemmove (pbData, pbSource, cbRemaining);
   *pcbDataLength = cbRemaining;
   return cbDataCopied;
   }
