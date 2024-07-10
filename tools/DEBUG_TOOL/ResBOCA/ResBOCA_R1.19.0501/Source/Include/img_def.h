#ifndef __IMG_DEF_H__
#define __IMG_DEF_H__

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef enum _X_IMG_TYPE_E
{
	IMG_BMP = 0,  	// BMP ͼƬ, ".bmp"Ϊ��չ��
    IMG_GIF,      	// GIF ͼƬ, ".gif"Ϊ��չ��
    IMG_JPG,      	// JPEGͼƬ, ".jpg" ����".jpeg" Ϊ��չ��
    IMG_PNG,		// GIF ͼƬ, ".png"Ϊ��չ��
	IMG_USR,

    IMG_ANY = 0xFF,      // �������ļ�,".any"Ϊ��չ��������ѹ��������ʱֻ��Ϊ��IMG_CMP_RAW��
} X_IMG_TYPE_E;

typedef enum _IMG_CMP_TYPE_E
{
	IMG_CMP_BMP_16_565_RAW = 0, 	// ����16��24λBMP,���16λ565��ʽ��δѹ��
	IMG_CMP_BMP_16_565_RLE,     	// ����16��24λBMP,���16λ565��ʽ����ѹ��(չѶ��ʽ)

    //[[not support
	IMG_CMP_BMP_OSD,    			// OSDѹ��(չѶ��ʽ��
    //]]

	IMG_CMP_BMP_JPG,     			// ����16��24λBMP��BMPתJPEGѹ��
    IMG_CMP_BMP_SJPG,     		    // ����16��24λBMP��BMPתJPEGѹ��ȥͷ(չѶ��ʽ)
	
	//[[not support
	IMG_CMP_BMP_32_RAW,     		// ����32λBMP�����32λ��δѹ��
	//]]
    
	IMG_CMP_BMP_32_SCI,     		// ����32λBMP�������ɫΪ565��ʽ��չѶ��ʽ
	
	//[[not support
 	IMG_CMP_GIF_RAW,              	// ���GIF��δѹ��
    //]]
	
	IMG_CMP_GIF_SCI,              	// ���GIF��չѶ��ʽ��hansen���ܻ��Ż�GIF
    IMG_CMP_JPG_RAW,				// ���JPEG��δѹ��
	IMG_CMP_JPG_SCI,				// ���JPEGѹ��ȥͷ(չѶ��ʽ)
    
	//[[not support
	IMG_CMP_PNG_RAW,				// ���PNG��δѹ��
    //]]
    
	IMG_CMP_PNG_ARGB,			    // ���PNGתARGB(չѶ��ʽ)
	IMG_CMP_JPG_STD,				// �����׼JPEG
	
	//[[not support
	IMG_CMP_BMP_8_RAW,				// ����8λBMP�����8λBMP�������κδ���
	//]]
	
	IMG_CMP_BMP_8_SCI,				// ����8λBMP�����8λBMP(չѶ��ʽ)

	IMG_CMP_PNG_SABM,               // ����PNG, ���SABM��ʽ(չѶ��ʽ)

	IMG_CMP_BMP_SABM,               // ����16,24,32λBMP,���SABM��ʽ(չѶ��ʽ)

	IMG_CMP_BMP_CK,                 // ����1,4,8�ȴ���ɫ���BMP,���CK��ʽ(չѶ��ʽ)

	IMG_CMP_BMP_OPT,

	IMG_CMP_BMP_32_ARGB888,         // ����32λBMP�������ɫΪ888��ʽ��չѶ��ʽ

	IMG_CMP_PNG_ARGB888,		    // �����͸���ȵ�PNG�������ɫΪ888��ʽ��չѶ��ʽ

	IMG_CMP_PNG_ARGB_RLE,           // min size of ARGB565 and ARGB565_RLE
	IMG_CMP_BMP_32_ARGB_RLE,        // min size of ARGB565 and ARGB565_RLE
	
	IMG_CMP_PNG_ARGB_RLE_AQ,        // IMG_CMP_PNG_ARGB_RLE with alpha quantify compress
	IMG_CMP_BMP_32_ARGB_RLE_AQ,     // IMG_CMP_BMP_32_ARGB_RLE with alpha quantify compress

	IMG_CMP_PNG_SABM_RLE,           // ����PNG, ���SABM��ʽ(չѶ��ʽ)	
	IMG_CMP_BMP_SABM_RLE,           // ����16,24,32λBMP,���SABM��ʽ(չѶ��ʽ)

	IMG_CMP_BMP_32_ARGB888_RLE,     // ����BMP32, ���argb888_rle��ʽ(չѶ��ʽ)	
	IMG_CMP_PNG_ARGB888_RLE,        // ����PNG, ���argb888_rle��ʽ(չѶ��ʽ)	

	IMG_CMP_PNG_SABM888_RLE,        // ����PNG, ���SABM��ʽ(չѶ��ʽ)	
	IMG_CMP_BMP_SABM888_RLE,        // ����16,24,32λBMP,���SABM��ʽ(չѶ��ʽ)


	IMG_CMP_USER,                   // no input file

	IMG_CMP_RAW = 0xFF              //  ��ʾ�������ͼƬ�ļ��������κδ���
} IMG_CMP_TYPE_E;

typedef enum _IMG_JPG_QULITY_E
{
	JPG_Q40 = 0,
    JPG_Q50,
	JPG_Q70,
	JPG_Q85,
	JPG_Q95,
}IMG_JPG_QULITY_E;

typedef enum _IMG_JPG_MCU_E
{
	JPG_YUV420 = 0,
    JPG_YUV411,
	JPG_YUV444,
	JPG_YUV422,
	JPG_YUV400,
}IMG_JPG_MCU_E;


typedef struct _SABM_IMG_HDR_T
{
	uint8 tag[4]; //'s','a','b','m'; 
	uint8 flag;
	uint8 opacity;
	uint8 cmpflag;
	uint8 reserved3;
	uint16 width;
	uint16 height;	
}SABM_IMG_HDR;

typedef struct _USER_IMG_HDR_T
{
	uint8  tag[4]; //'u','s','e','r'; 
	uint8  flag;
	uint8  reserved1;
	uint16 reserved2;
	uint32 reserved3;
}USER_IMG_HDR;



#define SABM_TILE 		    (0x1)    //ƽ��
#define SABM_HOR_STRETCH  	(0x2)    //ˮƽ����
#define SABM_VER_STRETCH  	(0x4)    //��ֱ����
#define SABM_CEN_STRETCH  	(0x8)    //�м�����
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif // __IMG_DEF_H__