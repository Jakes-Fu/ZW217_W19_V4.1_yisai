#ifndef _MMIWRE_IMAGE_H_
#define _MMIWRE_IMAGE_H_

//Picture type
#define     PT_JPG			0x01
#define     PT_PNG			0x02
#define     PT_WPNG			0x03

//alpha flag
#define     HAS_ALPHA_TABLE  3
#define     NO_ALPHA_TABLE   2

//picture's zip flag
#define  PICTURE_PRIORITY_LOWEST			0x00 //poor
#define  PICTURE_PRIORITY_BELOW_NORMAL		0x01 //normal
#define  PICTURE_PRIORITY_NORMAL			0x02 //good
#define  PICTURE_PRIORITY_ABOVE_NORMAL		0x03 //excellent  
#define  PICTURE_PRIORITY_HIGHEST			0x04 //outstanding
///////////////////////////////////

/*=============================================================
 函数名称 : GetImageInfo
 功能 : 获取通过硬件解码的数据.
 参数:
		unsigned long dwType[IN]---图形类型
		BOOL bIsFile[IN] --- 文件标志
		                     TRUE: 文件
							 FALSE: 数据
		LPTSTR  lpData[IN] --- 数据/文件全路径
		DWORD dwDataLen[IN] --- 数据长度
        int* pnw[IN/OUT] --- 限定宽度/实际解码后的宽度.
		int* pnh[IN/OUT] --- 限定高度/实际解码后的高度.
 返回 : >0: 解码成功
        0: 解码失败
=============================================================*/
unsigned long GetImageInfo(unsigned long dwType,unsigned char bIsFile,unsigned short* lpData,unsigned long dwDataLen,int* pnw,int* pnh);

/*=============================================================
 函数名称 : GetImageData
 功能 : 获取通过硬件解码的数据.
 参数:
		unsigned long dwType[IN] --- 图像类型 PT_JPG/PT_WPNG/PT_PNG
		unsigned long handle[IN] --- 图形句柄
		unsigned char* pszRGBData[IN/OUT] --- 解码空间
		unsigned char* pszAlphaData[IN/OUT] --- alpha 表
		unsigned long dwDataLen[IN] --- 空间大小
        int* pnw[IN/OUT] --- 限定宽度/实际解码后的宽度.
		int* pnh[IN/OUT] --- 限定高度/实际解码后的高度.
 返回 : >0: 解码成功,2:表示没有alpha表，3表示有alpha表
        0: 解码失败
=============================================================*/

unsigned char GetImageData(unsigned long dwType,unsigned long handle,unsigned char* pszRGBData,unsigned char* pszAlphaData,unsigned long dwDataLen,int* pnw,int* pnh);
/*=============================================================
 函数名称 : _ScratchImage
 功能 : 把图片按照给定的宽高缩放
 参数:
		unsigned short* lpszName[IN] --- 图像文件全路径
		int cxDesired[IN] --- 要求缩放的宽
		int cyDesired[IN] --- 要求缩放的高
		unsigned char* lpszData[OUT]--- 缩后的图像文件数据
		unsigned long *unDataLen[IN/OUT] --- 缩后的图像文件数据大小
		int nPriority [IN]          --- 图片压缩级别
 返回 : 1: 成功
        0: 失败
=============================================================*/
unsigned char _ScratchImage(unsigned short* lpszName,int cxDesired,int cyDesired,unsigned char* lpszData, unsigned long* unDataLen,int nPriority);









#endif //_ADAPTOR_IMAGE_
