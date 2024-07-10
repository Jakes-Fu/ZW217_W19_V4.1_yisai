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
	IMG_BMP = 0,  	// BMP 图片, ".bmp"为扩展名
    IMG_GIF,      	// GIF 图片, ".gif"为扩展名
    IMG_JPG,      	// JPEG图片, ".jpg" 或者".jpeg" 为扩展名
    IMG_PNG,		// GIF 图片, ".png"为扩展名
	IMG_USR,

    IMG_ANY = 0xFF,      // 非以上文件,".any"为扩展名，它的压缩类型暂时只能为“IMG_CMP_RAW”
} X_IMG_TYPE_E;

typedef enum _IMG_CMP_TYPE_E
{
	IMG_CMP_BMP_16_565_RAW = 0, 	// 输入16、24位BMP,输出16位565格式，未压缩
	IMG_CMP_BMP_16_565_RLE,     	// 输入16、24位BMP,输出16位565格式，行压缩(展讯格式)

    //[[not support
	IMG_CMP_BMP_OSD,    			// OSD压缩(展讯格式）
    //]]

	IMG_CMP_BMP_JPG,     			// 输入16、24位BMP，BMP转JPEG压缩
    IMG_CMP_BMP_SJPG,     		    // 输入16、24位BMP，BMP转JPEG压缩去头(展讯格式)
	
	//[[not support
	IMG_CMP_BMP_32_RAW,     		// 输入32位BMP，输出32位，未压缩
	//]]
    
	IMG_CMP_BMP_32_SCI,     		// 输入32位BMP，输出颜色为565格式，展讯格式
	
	//[[not support
 	IMG_CMP_GIF_RAW,              	// 输出GIF，未压缩
    //]]
	
	IMG_CMP_GIF_SCI,              	// 输出GIF，展讯格式，hansen可能会优化GIF
    IMG_CMP_JPG_RAW,				// 输出JPEG，未压缩
	IMG_CMP_JPG_SCI,				// 输出JPEG压缩去头(展讯格式)
    
	//[[not support
	IMG_CMP_PNG_RAW,				// 输出PNG，未压缩
    //]]
    
	IMG_CMP_PNG_ARGB,			    // 输出PNG转ARGB(展讯格式)
	IMG_CMP_JPG_STD,				// 输出标准JPEG
	
	//[[not support
	IMG_CMP_BMP_8_RAW,				// 输入8位BMP，输出8位BMP，不作任何处理
	//]]
	
	IMG_CMP_BMP_8_SCI,				// 输入8位BMP，输出8位BMP(展讯格式)

	IMG_CMP_PNG_SABM,               // 输入PNG, 输出SABM格式(展讯格式)

	IMG_CMP_BMP_SABM,               // 输入16,24,32位BMP,输出SABM格式(展讯格式)

	IMG_CMP_BMP_CK,                 // 输入1,4,8等带调色板的BMP,输出CK格式(展讯格式)

	IMG_CMP_BMP_OPT,

	IMG_CMP_BMP_32_ARGB888,         // 输入32位BMP，输出颜色为888格式，展讯格式

	IMG_CMP_PNG_ARGB888,		    // 输入带透明度的PNG，输出颜色为888格式，展讯格式

	IMG_CMP_PNG_ARGB_RLE,           // min size of ARGB565 and ARGB565_RLE
	IMG_CMP_BMP_32_ARGB_RLE,        // min size of ARGB565 and ARGB565_RLE
	
	IMG_CMP_PNG_ARGB_RLE_AQ,        // IMG_CMP_PNG_ARGB_RLE with alpha quantify compress
	IMG_CMP_BMP_32_ARGB_RLE_AQ,     // IMG_CMP_BMP_32_ARGB_RLE with alpha quantify compress

	IMG_CMP_PNG_SABM_RLE,           // 输入PNG, 输出SABM格式(展讯格式)	
	IMG_CMP_BMP_SABM_RLE,           // 输入16,24,32位BMP,输出SABM格式(展讯格式)

	IMG_CMP_BMP_32_ARGB888_RLE,     // 输入BMP32, 输出argb888_rle格式(展讯格式)	
	IMG_CMP_PNG_ARGB888_RLE,        // 输入PNG, 输出argb888_rle格式(展讯格式)	

	IMG_CMP_PNG_SABM888_RLE,        // 输入PNG, 输出SABM格式(展讯格式)	
	IMG_CMP_BMP_SABM888_RLE,        // 输入16,24,32位BMP,输出SABM格式(展讯格式)


	IMG_CMP_USER,                   // no input file

	IMG_CMP_RAW = 0xFF              //  表示对输入的图片文件，不作任何处理
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



#define SABM_TILE 		    (0x1)    //平铺
#define SABM_HOR_STRETCH  	(0x2)    //水平拉伸
#define SABM_VER_STRETCH  	(0x4)    //垂直拉伸
#define SABM_CEN_STRETCH  	(0x8)    //中间拉伸
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif // __IMG_DEF_H__