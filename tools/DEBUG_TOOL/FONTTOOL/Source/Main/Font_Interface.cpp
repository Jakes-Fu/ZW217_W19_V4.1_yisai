// Font_Interface.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Font_Interface.h"

#define BIT_SHIFT 4

#define RLE_TRANS255 0x0c
#define RLE_TRANS 0x0d
#define RLE_NO_RELAY 0x0f
#define RLE_NO_TRANS 0x0e

#define SIZE_MAX    0xFF*0xFF

#define INIT_BIT {\
	still_count = 0;\
	output = 0;\
k=0;}

#define OUTPUT_BIT {\
	output = (output<<BIT_SHIFT)|input ;\
	still_count = still_count + BIT_SHIFT;\
	while(still_count >= 8)\
{\
	*font_des_ptr++ = (unsigned char)(output&0xff);\
	output = output >> 8;\
	k++; \
	still_count = still_count - 8;\
}\
}

#define FLUSH_BIT  *font_des_ptr = (uint8)(output<<BIT_SHIFT)

const int g_actalpha[] = {0,16,32,48,64,96,128,160,192,224,240,255};	
const int g_alphapoint[] = {0,8,24,40,56,80,112,144,176,200,232,248,256};


/*功能描述：用于降阶，将256阶降为12阶
参数：
font_src_ptr: 降阶前字体源指针
font_des_ptr：降阶后字体目标指针
font_width：  字体宽度
font_height ：字体高度
idxflag:      降价后是alpha的索引，还是实际值
返回值：  无
*/
void Font_Half_Sample(unsigned char *font_src_ptr, unsigned char *font_des_ptr, int font_width, int font_height,bool idxflag)
{	
	int i, j, temp=0;
	unsigned char *alpha_ptr = font_src_ptr;
	unsigned char *loc_font_ptr = font_des_ptr;	
	for(j=0; j<font_height ; j++)
	{
		for(i=0; i<font_width; i++)
		{
			for(temp=1; temp<13; temp++)
			{
				if(*(alpha_ptr+i)<g_alphapoint[temp])
				{	
					if(idxflag==1)
					{
						*loc_font_ptr++ = (unsigned char)(temp-1);
					}
					else
					{
						*loc_font_ptr++ = (unsigned char)g_actalpha[temp-1];
					}
					break;
				}
			}
		}		
		alpha_ptr += font_width;			
	}	
}
/*---------------------------------------------------------*/
//Description: vector font compress interface
//	Global resource dependence: 
//  Author: Jialian.bai
//	Note:
//	input:
//			font_src_ptr -  input font data address
//			font_width - input font width
//          font_height - input font height
//	output:
//          font_des_ptr - output font data address
//          output_size - output font data size after compress
//	return: 
//			1 - compress success
//          0 - compress fail
/*----------------------------------------------------------*/
bool Font_Rle_Compress(unsigned char *font_src_ptr, unsigned char *font_des_ptr,int font_width, int font_height, int *output_size)
{
	uint8 diff_font[SIZE_MAX] ={0};
	uint8 half_data[SIZE_MAX] ={0}; 
	uint8 *loc_font_ptr;	

	int i,j,k;
	int font_size =0;
	
	uint32 still_count = 0;
	long output = 0;
	long input = 0;
	int same_count = 0;
	uint8 cur = 0;
	uint8 next = 0;
	int k_diff = 0;
	int diff_i = 0;
	
	font_size = font_width * font_height;
	Font_Half_Sample(font_src_ptr,half_data,font_width, font_height,1);
	
    loc_font_ptr = font_des_ptr;
	
	i=j=k=0;

	
	INIT_BIT;
	
	while(i < font_size)
	{
		
		same_count = 0;
		cur = half_data[i];
		
		next = cur;                      
		while(cur == next && j<font_size )     //查找相同数据
		{
			j++;
			next =  half_data[j];
			
			same_count++;
			
			if(same_count == 15)
			{
				break;
			}
		}
		
		if(same_count > 2 || j >=font_size)   //2个以上连续相同的数据
		{
			i = i + same_count;
			
			if(k_diff > 0)                   //对不连续数据进行编码
			{
				input = RLE_NO_RELAY;
				OUTPUT_BIT; 
				
				for(diff_i = 0; diff_i<k_diff; diff_i++)  
				{					
					input  = diff_font[diff_i];	
					OUTPUT_BIT;
				}
				
				k_diff = 0;
				memset(diff_font, 0, SIZE_MAX);
			}
			
			if(cur == 0)                    //连续2个0以上 
			{
				input = RLE_TRANS;
				OUTPUT_BIT; 
				input = same_count;
				OUTPUT_BIT; 
				
			}
			else if(cur ==11)           //连续3个及以上的255
			{
				input = RLE_TRANS255;
				OUTPUT_BIT; 
				input = same_count;
				OUTPUT_BIT; 
			}
			else                         //连续2个相同非零数据
			{
				input = RLE_NO_TRANS;
				OUTPUT_BIT;
				input = same_count;
				OUTPUT_BIT; 
				input = cur;
				OUTPUT_BIT; 				
			}
		}
		
		else
		{
			for(diff_i = i; diff_i<j; diff_i++)
			{
				diff_font[k_diff] =  half_data[i];        //统计不连续的数据
				k_diff++;
			}
			
			i = i + same_count;
		}
	}
	
	FLUSH_BIT;
	*output_size = k+1;
	
	return true;	
}


#define READ_DATA(still_count,rle_font, read_data)    \
                {\
				if((still_count&0x1)==0)   \
                        {\
						read_data=((*rle_font)>>BIT_SHIFT)&0x0f; \
} \
	else \
                        { \
						read_data=((*rle_font++)&0x0f);  \
}\
	still_count++; \
}

/*---------------------------------------------------------*/
//Description: vector font decompress interface
//	Global resource dependence: 
//  Author: Jialian.bai
//	Note:
//	input:
//			rle_font_ptr -  input compressed font data address
//			font_size -     input compressed font data size
//	output:
//          unrle_font_ptr - output decomressed font data address
//	return: 
/*----------------------------------------------------------*/
void Font_Rle_Decompress(uint8 *rle_font_ptr, uint8 *unrle_font_ptr, uint32 font_size,int font_width, int font_height)
{
	int i = 0;
	int j = 0;
	uint8 *font_ptr = rle_font_ptr;	
	uint8 * end_font_ptr = font_ptr + font_size;
	uint8 * end_unrle_font_ptr = unrle_font_ptr + font_width*font_height;	
    uint32 read_data = 0;
    uint32 still_count = 0;
    uint32 same_count = 0;
	uint8 * loc_font_ptr = unrle_font_ptr;		
    do
    {		
		READ_DATA(still_count, font_ptr, read_data);		
		if(RLE_NO_RELAY==read_data)
		{				
			do
			{				
				READ_DATA(still_count, font_ptr, read_data);
				if(read_data<RLE_TRANS255)
				{
					*unrle_font_ptr++ = (uint8)read_data;
				}                     
				else
				{
					break;
				}
				
			}while( font_ptr<end_font_ptr);

			if(font_ptr>=end_font_ptr)
			{
				break;
			}			
			if((still_count&0x1)==0)
			{
				font_ptr--;
			}
			still_count--;
		}
		else if(read_data != 0)
		{  	
			
			READ_DATA(still_count, font_ptr, same_count);
			
			if(RLE_TRANS==read_data)              
			{
				while(same_count--)
				{              	
					*unrle_font_ptr++ = 0; 			
				}
			}			
			else if(RLE_TRANS255 == read_data)
			{
				while(same_count--)
				{
					*unrle_font_ptr++ = 11; 			
				}
			}			
			else if( RLE_NO_TRANS == read_data )
			{				
				READ_DATA(still_count, font_ptr, read_data);
				
				while(same_count--)
				{
					*unrle_font_ptr++ = (uint8)read_data;					
				} 
			}
			else
			{
				break;
			}			
		}
		else
		{
			break;
		}
		
    }while(font_ptr<end_font_ptr);

	while(unrle_font_ptr<end_unrle_font_ptr)
	{
		*unrle_font_ptr ++ = 0;
	}
	
	for(i=0; i<font_height; i++)       
	{	
		for(j=0; j<font_width; j++)  
		{
			*loc_font_ptr = (uint8)*(g_actalpha + (*loc_font_ptr)); 	
			loc_font_ptr ++;
		}
	}
	
    return;
}

