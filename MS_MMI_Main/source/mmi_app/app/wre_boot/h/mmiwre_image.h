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
 �������� : GetImageInfo
 ���� : ��ȡͨ��Ӳ�����������.
 ����:
		unsigned long dwType[IN]---ͼ������
		BOOL bIsFile[IN] --- �ļ���־
		                     TRUE: �ļ�
							 FALSE: ����
		LPTSTR  lpData[IN] --- ����/�ļ�ȫ·��
		DWORD dwDataLen[IN] --- ���ݳ���
        int* pnw[IN/OUT] --- �޶����/ʵ�ʽ����Ŀ��.
		int* pnh[IN/OUT] --- �޶��߶�/ʵ�ʽ����ĸ߶�.
 ���� : >0: ����ɹ�
        0: ����ʧ��
=============================================================*/
unsigned long GetImageInfo(unsigned long dwType,unsigned char bIsFile,unsigned short* lpData,unsigned long dwDataLen,int* pnw,int* pnh);

/*=============================================================
 �������� : GetImageData
 ���� : ��ȡͨ��Ӳ�����������.
 ����:
		unsigned long dwType[IN] --- ͼ������ PT_JPG/PT_WPNG/PT_PNG
		unsigned long handle[IN] --- ͼ�ξ��
		unsigned char* pszRGBData[IN/OUT] --- ����ռ�
		unsigned char* pszAlphaData[IN/OUT] --- alpha ��
		unsigned long dwDataLen[IN] --- �ռ��С
        int* pnw[IN/OUT] --- �޶����/ʵ�ʽ����Ŀ��.
		int* pnh[IN/OUT] --- �޶��߶�/ʵ�ʽ����ĸ߶�.
 ���� : >0: ����ɹ�,2:��ʾû��alpha��3��ʾ��alpha��
        0: ����ʧ��
=============================================================*/

unsigned char GetImageData(unsigned long dwType,unsigned long handle,unsigned char* pszRGBData,unsigned char* pszAlphaData,unsigned long dwDataLen,int* pnw,int* pnh);
/*=============================================================
 �������� : _ScratchImage
 ���� : ��ͼƬ���ո����Ŀ������
 ����:
		unsigned short* lpszName[IN] --- ͼ���ļ�ȫ·��
		int cxDesired[IN] --- Ҫ�����ŵĿ�
		int cyDesired[IN] --- Ҫ�����ŵĸ�
		unsigned char* lpszData[OUT]--- �����ͼ���ļ�����
		unsigned long *unDataLen[IN/OUT] --- �����ͼ���ļ����ݴ�С
		int nPriority [IN]          --- ͼƬѹ������
 ���� : 1: �ɹ�
        0: ʧ��
=============================================================*/
unsigned char _ScratchImage(unsigned short* lpszName,int cxDesired,int cyDesired,unsigned char* lpszData, unsigned long* unDataLen,int nPriority);









#endif //_ADAPTOR_IMAGE_
