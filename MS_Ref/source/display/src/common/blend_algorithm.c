#include "sci_types.h"
#include "os_api.h"

#ifdef __cplusplus
extern   "C"
    {
#endif
/*lint -e685 -e568*/

#define BLEND_RGB565TORGB888(value_565,value_888) do{ \
       value_888 = (( value_565&0xf800) | ( ( value_565 & 0x800 ) ? 0x700 : 0 ))<<8 \
                   |((value_565&0x7e0 ) | ( ( value_565&0x20 ) ? 0x18 : 0 ))<<5 \
	               |((( value_565&0x1f )<<3) | ( ( value_565 & 0x1 ) ? 7 : 0 )); \
}while(0)


/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC void DISPLAY_Blend_ARGB888ToARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 colorkey,
                                          uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
										  uint32 layer_alpha
                                          )
{
    uint32 vcount = 0;
    uint32 hcount = 0;
    uint32 src_value = 0;
    uint32 dst_value = 0;
    uint8  alpha = 0;
    uint8  h_r = 0,l_r = 0;
    uint8  h_g = 0,l_g = 0;
    uint8  h_b = 0,l_b = 0;
    uint8 r = 0,g = 0,b = 0;    


    BLEND_RGB565TORGB888(colorkey, colorkey);                      
    
    vcount = refresh_height;      
    if(!is_colorkey_en)
    {
    	while(vcount)
    	{
    	    hcount = refresh_width;
    	    while(hcount)
    	    {
    	        src_value = *u32_src_buf++;
    		    alpha = (src_value>>24);                   
                
                if(alpha==0)
                {
                    dst_buf++;
                }
                else if ((alpha==255) && (layer_alpha==255))
                {
                    *dst_buf++ = src_value; 
                }
                else
                {
                    alpha = (alpha * layer_alpha) >> 8;
                    h_r = ((src_value&0x00ff0000)>>16);
                    h_g = ((src_value&0x0000ff00)>>8);
                    h_b = src_value&0x000000ff;
                    
        	        dst_value = *dst_buf;
                    l_r = ((dst_value&0x00ff0000)>>16);
                    l_g = ((dst_value&0x0000ff00)>>8);
                    l_b = dst_value&0x000000ff;	
                    
                    r = ((l_r<<8)+alpha*(h_r-l_r)+128)>>8;
                    g = ((l_g<<8)+alpha*(h_g-l_g)+128)>>8;
                    b = ((l_b<<8)+alpha*(h_b-l_b)+128)>>8;
                    *dst_buf++ = (r<<16 | g<< 8 | b);
                }                
    	        hcount--;
    	    }
    	    u32_src_buf += srcjumpcount;
    	    dst_buf += dstjumpcount;
    	    vcount--;
    	}
    }
    else
    {
    	while(vcount)
    	{
    	    hcount = refresh_width;
    	    while(hcount)
    	    {
    	        src_value = *u32_src_buf++;    		    
              
                if(colorkey == (src_value&0x00ffffff))
                {
                    dst_buf++;                       
                }
                else
                {
                    alpha = (src_value>>24);
                    
                    if(alpha==0)
                    {
                        dst_buf++;
                    }
                    else if ((alpha==255) && (layer_alpha==255))
                    {
                        *dst_buf++ = src_value; 
                    }
                    else
                    {
                        alpha = (alpha * layer_alpha) >> 8;
                        h_r = ((src_value&0x00ff0000)>>16);
                        h_g = ((src_value&0x0000ff00)>>8);
                        h_b = src_value&0x000000ff;       
                
                        dst_value = *dst_buf;
                        l_r = ((dst_value&0x00ff0000)>>16);
                        l_g = ((dst_value&0x0000ff00)>>8);
                        l_b = dst_value&0x000000ff;	
                        
                        r = ((l_r<<8)+alpha*(h_r-l_r)+128)>>8;
                        g = ((l_g<<8)+alpha*(h_g-l_g)+128)>>8;
                        b = ((l_b<<8)+alpha*(h_b-l_b)+128)>>8;
                        *dst_buf++ = (r<<16 | g<< 8 | b);
                    }
                }
    	        hcount--;
    	    }
    	    u32_src_buf += srcjumpcount;
    	    dst_buf += dstjumpcount;
    	    vcount--;
    	}
        
    }
    

}


/*****************************************************************************/
//  Description:  low layer blends with RGB565
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC void DISPLAY_Blend_RGB565ToARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 colorkey,
                                          uint32 *dst_buf,
                                          uint16 *src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
										  uint32 layer_alpha
                                          )
{
    uint32 vcount = 0;
    uint32 hcount = 0;
    uint32 src_u32_value = 0;
    uint32 dst_value = 0;
    uint8  alpha = layer_alpha;
    uint8  h_r = 0,l_r = 0;
    uint8  h_g = 0,l_g = 0;
    uint8  h_b = 0,l_b = 0;
    uint8 r = 0,g = 0,b = 0;   
   
    
    vcount = refresh_height;    
    if(!is_colorkey_en)
    {   
        if(layer_alpha==0)
        {
            return;
        }
        else if(layer_alpha==255)
        {
        	while(vcount)
        	{
        	    hcount = refresh_width;
        	    while(hcount)
        	    {
        	        src_u32_value = *src_buf++;                  
                    BLEND_RGB565TORGB888(src_u32_value, src_u32_value);                      
                    *dst_buf++ = src_u32_value;                                               
        	        hcount--;
        	    }
        	    src_buf += srcjumpcount;
        	    dst_buf += dstjumpcount;
        	    vcount--;
        	}
        }
        else
        {
        	while(vcount)
        	{
        	    hcount = refresh_width;
        	    while(hcount)
        	    {
        	        src_u32_value = *src_buf++;                  
                    BLEND_RGB565TORGB888(src_u32_value, src_u32_value);                  
        		    
                    h_r = ((src_u32_value&0x00ff0000)>>16);
                    h_g = ((src_u32_value&0x0000ff00)>>8);
                    h_b = src_u32_value&0x000000ff;
                    
        	        dst_value = *dst_buf;
                    l_r = ((dst_value&0x00ff0000)>>16);
                    l_g = ((dst_value&0x0000ff00)>>8);
                    l_b = dst_value&0x000000ff;	
                   
                    r = ((l_r<<8)+alpha*(h_r-l_r)+128)>>8;
                    g = ((l_g<<8)+alpha*(h_g-l_g)+128)>>8;
                    b = ((l_b<<8)+alpha*(h_b-l_b)+128)>>8;
                    *dst_buf++ = (r<<16 | g<< 8 | b);
                                  
        	        hcount--;
        	    }
        	    src_buf += srcjumpcount;
        	    dst_buf += dstjumpcount;
        	    vcount--;
        	}

        }
    }
    else
    {
        uint16 src_u16_value = 0;
        if(layer_alpha==0)
        {
            return;
        }
        else if(layer_alpha==255)
        {
        	while(vcount)
        	{
        	    hcount = refresh_width;
        	    while(hcount)
        	    {
        	        src_u16_value = *src_buf++;  			                    
			 BLEND_RGB565TORGB888(src_u16_value, src_u32_value);    
                   
			if(colorkey == (src_u16_value&0xffff))
                    {
                         dst_buf++;                        
                    }
                    else
                    {                                                
                        *dst_buf++ = src_u32_value;                         
                    }
        	        hcount--;
        	    }
        	    src_buf += srcjumpcount;
        	    dst_buf += dstjumpcount;
        	    vcount--;
        	}
        }
        else
        {
        	while(vcount)
        	{
        	    hcount = refresh_width;
        	    while(hcount)
        	    {        	       
			 src_u16_value = *src_buf++;  			             
			BLEND_RGB565TORGB888(src_u16_value, src_u32_value);    
        	                                                
			if(colorkey == (src_u16_value&0xffff))
                    {
                         dst_buf++;                        
                    }
                    else
                    {            
                        h_r = ((src_u32_value&0x00ff0000)>>16);
                        h_g = ((src_u32_value&0x0000ff00)>>8);
                        h_b = src_u32_value&0x000000ff;             
                        dst_value = *dst_buf;
                        l_r = ((dst_value&0x00ff0000)>>16);
                        l_g = ((dst_value&0x0000ff00)>>8);
                        l_b = dst_value&0x000000ff;	
                        r = ((l_r<<8)+alpha*(h_r-l_r)+128)>>8;
                        g = ((l_g<<8)+alpha*(h_g-l_g)+128)>>8;
                        b = ((l_b<<8)+alpha*(h_b-l_b)+128)>>8;
                        *dst_buf++ = (r<<16 | g<< 8 | b);
                    }
        	        hcount--;
        	    }
        	    src_buf += srcjumpcount;
        	    dst_buf += dstjumpcount;
        	    vcount--;
        	}

        }

    }
}


/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//	Global resource dependence:
//  Author: Jianping.wang
//  param: top layer: PMARGB888
//              bottom layer: ARGB888: image, not OSD. alpha is ignored.
//	Note:  do not support color key and layer_alpha
/*****************************************************************************/

PUBLIC void DISPLAY_Blend_PMARGB888ToARGB888(BOOLEAN is_colorkey_en, 
                                          uint32 colorkey,
										  uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          )
{
    uint32 vcount = 0;
    uint32 hcount = 0;
    uint32 src_value = 0;
    uint32 dst_value = 0;
    uint32 dst_alpha = 0;
    uint8  alpha = 0;
    uint8  h_r = 0,l_r = 0;
    uint8  h_g = 0,l_g = 0;
    uint8  h_b = 0,l_b = 0;
    uint8 r = 0,g = 0,b = 0;   
    uint8 a = 0xff;
    
    vcount = refresh_height;  
    while(vcount)
    {
    	hcount = refresh_width;
    	while(hcount)
    	{
    		src_value = *u32_src_buf++;
    		alpha = (src_value>>24);
    		
    		if(255 != layer_alpha)
    		{
    			return;//do not support layer_alpha;
    		}
    		if(0 == alpha)
    		{
    			*dst_buf++ = src_value;
    		}
    		else if(255 == alpha)
    		{
    			dst_buf++;
    		}
    		else//normal alpha : 1~254
			{
				dst_value = *dst_buf;
				dst_alpha = (dst_value>>24);
				
				h_r = ((src_value&0x00ff0000)>>16);
				h_g = ((src_value&0x0000ff00)>>8);
				h_b = src_value&0x000000ff;
				
				l_r = ((dst_value&0x00ff0000)>>16);
				l_g = ((dst_value&0x0000ff00)>>8);
				l_b = dst_value&0x000000ff;
				
				l_r = (l_r * (alpha))>>8;
				l_g = (l_g * (alpha))>>8;
				l_b = (l_b * (alpha))>>8;
				
				r = l_r + h_r;
				g = l_g + h_g;
				b = l_b + h_b;
				*dst_buf++ =((a<<24) |(r<<16 | g<< 8 | b));
			}
			hcount--;
		}
		u32_src_buf += srcjumpcount;
		dst_buf += dstjumpcount;
		vcount--;
	}
}

/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//	Global resource dependence:
//  Author: Jianping.wang
//  param: top layer: ARGB888
//              bottom layer: PMARGB888
//	Note:
/*****************************************************************************/


PUBLIC void DISPLAY_Blend_ARGB888ToPMARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
			                                          uint32 colorkey, 
			                                          uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          )
{
    uint32 vcount = 0;
    uint32 hcount = 0;
    uint32 src_value = 0;
    uint32 dst_value = 0;
    uint32 dst_alpha = 0;
    uint8  alpha = 0;
    uint8  h_r = 0,l_r = 0;
    uint8  h_g = 0,l_g = 0;
    uint8  h_b = 0,l_b = 0;
    //uint8 r = 0,g = 0,b = 0;    

    BLEND_RGB565TORGB888(colorkey, colorkey);                      
    
    vcount = refresh_height;      
    {
    	while(vcount)
    	{
    	    hcount = refresh_width;
    	    while(hcount)
    	    {
    	          src_value = *u32_src_buf++;
				  if(is_colorkey_en)
				  {
					  if(colorkey == (src_value&0x00ffffff))
					  {
						  dst_buf++;
                                            hcount--;
						  continue;
					  }
				  }
				  alpha = (src_value>>24);
				  
				  if(255 != layer_alpha)
				  {
					  alpha = (alpha * layer_alpha)>>8;
				  }
				  
				  if(0 == alpha)
				  {
					  dst_buf++;
				  }
				  else if(alpha==255)
				  {
					  *dst_buf++ = (src_value&0x00ffffff);  //  1-alpha is always 0
				  }
				  else  //normal alpha : 1~254
				  {
					  dst_value = *dst_buf;
					  dst_alpha = (dst_value>>24);
					  h_r = ((src_value&0x00ff0000)>>16);
					  h_g = ((src_value&0x0000ff00)>>8);
					  h_b = src_value&0x000000ff;
					  
					  
					  h_r = (h_r*alpha)>>8;
					  h_g = (h_g*alpha)>>8;
					  h_b = (h_b*alpha)>>8;
					  
					  
					  l_r = ((dst_value&0x00ff0000)>>16);
					  l_g = ((dst_value&0x0000ff00)>>8);
					  l_b = dst_value&0x000000ff;
					  
					  
					  l_r = (l_r * (255-alpha))>>8;
					  l_g = (l_g * (255-alpha))>>8;
					  l_b = (l_b * (255-alpha))>>8;
					  
					  l_r = l_r + h_r;
					  l_g = l_g + h_g;
					  l_b = l_b + h_b;
						
					  alpha = ((255-alpha) * dst_alpha)>>8; 
					  
					  *dst_buf++ =((alpha<<24) |(l_r<<16 | l_g<< 8 | l_b));
				  } 
				  hcount--;
			}
    	    u32_src_buf += srcjumpcount;
    	    dst_buf += dstjumpcount;
    	    vcount--;
    	}
    }
    
    

}



/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//	Global resource dependence:
//  Author: Jianping.wang
//  param: top layer: PMARGB888
//              bottom layer: PMARGB888
//	Note:  do not support color key and layer_alpha
/*****************************************************************************/


PUBLIC  void DISPLAY_Blend_PMARGB888ToPMARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 colorkey,
										  										uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          )
{
    uint32 vcount = 0;
    uint32 hcount = 0;
    uint32 src_value = 0;
    uint32 dst_value = 0;
    uint32 dst_alpha = 0;
    uint8  alpha = 0;
    uint8  h_r = 0,l_r = 0;
    uint8  h_g = 0,l_g = 0;
    uint8  h_b = 0,l_b = 0;
    //uint8 r = 0,g = 0,b = 0;    
    
    vcount = refresh_height;      
    {
    	while(vcount)
    	{
    	    hcount = refresh_width;
    	    while(hcount)
    	    {
				src_value = *u32_src_buf++;
				alpha = (src_value>>24);
				
				//if(255 != layer_alpha)
				//{
				//	return;//do not support layer_alpha;
				//	//alpha = (alpha * layer_alpha)>>8
				//}
				
				if(0 == alpha)
				{
					*dst_buf++ = src_value;
				}
				else if(alpha==255)
				{
					dst_buf++;
				}
                else  //normal alpha : 1~254
				{
					dst_value = *dst_buf;
					dst_alpha = (dst_value>>24);
					
					h_r = ((src_value&0x00ff0000)>>16);
					h_g = ((src_value&0x0000ff00)>>8);
					h_b = src_value&0x000000ff;
					
					l_r = ((dst_value&0x00ff0000)>>16);
					l_g = ((dst_value&0x0000ff00)>>8);
					l_b = dst_value&0x000000ff;
				
					l_r = (l_r * (alpha))>>8;
					l_g = (l_g * (alpha))>>8;
					l_b = (l_b * (alpha))>>8;
					
					l_r = l_r + h_r;
					l_g = l_g + h_g;
					l_b = l_b + h_b;
					
					if(255 != dst_alpha)
					{
						alpha = ((alpha) * dst_alpha)>>8;
					}
					
					*dst_buf++ =((alpha<<24) |(l_r<<16 | l_g<< 8 | l_b));
				
				}                
    	        hcount--;
    	    }
    	    u32_src_buf += srcjumpcount;
    	    dst_buf += dstjumpcount;
    	    vcount--;
    	}
    }
       

}


/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//	Global resource dependence:
//  Author: Jianping.wang
//  param: top layer: rgb565
//              bottom layer: PMARGB888
//	Note:  do not support color key and layer_alpha
/*****************************************************************************/


PUBLIC  void DISPLAY_Blend_RGB565ToPMARGB888(BOOLEAN is_colorkey_en, //DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 colorkey,
										  uint32 *dst_buf,
                                          uint16 *src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height,
                                          uint32 layer_alpha
                                          )
{
    uint32 vcount = 0;
    uint32 hcount = 0;
    uint32 src_value = 0;
    uint8  alpha = layer_alpha;

    uint8 r = 0,g = 0,b = 0;   

    
    
    vcount = refresh_height;      
    if(is_colorkey_en)
    {
    	while(vcount)
    	{
    	        hcount = refresh_width;

                while(hcount)
        	  {
        	       src_value = *src_buf++;
        	                    
                    if(colorkey == src_value)
                    {
                         dst_buf++;                   
                    }
                    else
                    {

                        r =  (src_value <<8)&0x00f80000; 
                        g =  (src_value <<5)&0x0000fc00; 
                        b =  (src_value <<3)&0x000000ff; 
                        if(255 != alpha)
                        {
                              r = (r*alpha)>>8;
                              g = (g*alpha)>>8;
                              b = (b*alpha)>>8;
                        }
                       
                        *dst_buf++ = (r|g|b);
                    }
                    hcount--;
                  
        	    }
                   
    	    src_buf += srcjumpcount;
    	    dst_buf += dstjumpcount;
    	    vcount--;
           }   
    	
    }
    else
    {
    	while(vcount)
    	{
    	        hcount = refresh_width;

                while(hcount)
        	  {
        	       src_value = *src_buf++;
                    {

                        r =  (src_value <<8)&0x00f80000; 
                        g =  (src_value <<5)&0x0000fc00; 
                        b =  (src_value <<3)&0x000000ff; 

                        if(255 != alpha)
                        {
                              r = (r*alpha)>>8;
                              g = (g*alpha)>>8;
                              b = (b*alpha)>>8;
                        }

                        *dst_buf++ = (r|g|b);
                    }
                    hcount--;
                  
        	    }
                   
    	    src_buf += srcjumpcount;
    	    dst_buf += dstjumpcount;
    	    vcount--;
           }   
    	
    }
}

PUBLIC void  DISPLAY_RGB565ToPMARGB888(uint32 *dst_ptr,
							 uint16 *src_ptr,
							 uint32 dst_width,
							 uint32 src_width,
							 uint32 roi_width,
							 uint32 roi_height,
							 uint32 colorkey,
							 uint32 colorkey_en)
{

    uint32 *tmp_dst_ptr;
    uint16 *tmp_src_ptr;

    //uint32 width = roi_width;
    uint32 i,j;

    uint32 dst32;
    

    if(colorkey_en)
    {
        uint32 key = colorkey;
        uint32 r,g,b;
              
        for( i=0 ; i<roi_height ; i++)
        {
            tmp_dst_ptr = dst_ptr + i*dst_width;
            tmp_src_ptr = src_ptr + i*src_width;
                                
            for(j=0; j<roi_width;j++)
            {
                dst32 = *tmp_src_ptr++;
                if(dst32 != key)
                {
                    r =  (dst32 <<8)&0x00f80000; 
                    g =  (dst32 <<5)&0x0000fc00; 
                    b =  (dst32 <<3)&0x000000ff; 
                    *tmp_dst_ptr++ = (r|g|b);
                }
                else
                {
                    *tmp_dst_ptr++ = 0xff000000;
                }
            }
        }     
    }
    else
     {

        uint32 r,g,b;
              
        for( i=0 ; i<roi_height ; i++)
        {
            tmp_dst_ptr = dst_ptr + i*dst_width;
            tmp_src_ptr = src_ptr + i*src_width;
                                
            for(j=0; j<roi_width;j++)
            {
                dst32 = *tmp_src_ptr++;
                r =  (dst32 <<8)&0x00f80000; 
                g =  (dst32 <<5)&0x0000fc00; 
                b =  (dst32 <<3)&0x000000ff; 
                *tmp_dst_ptr++ = (r|g|b);
             }
        }     
    }

    return;


}



PUBLIC void  DISPLAY_ARGB888ToPMARGB888(uint32 *dst_ptr, 
							  uint32 *src_ptr, 
							  uint32 dst_width,
							  uint32 src_width, 
							  uint32 roi_width,
							  uint32 roi_height, 
							  uint32 colorkey,
							  uint32 colorkey_en)
{

    uint32 *tmp_dst_ptr;
    uint32 *tmp_src_ptr;

    //uint32 width = roi_width;
    uint32 i,j;

    uint32 dst32;    

    {
        uint32 rb,g;
        uint32 alpha = 0;      
        for( i=0 ; i<roi_height ; i++)
        {
            tmp_dst_ptr = dst_ptr + i*dst_width;
            tmp_src_ptr = src_ptr + i*src_width;
                                
            for(j=0; j<roi_width;j++)
            {
                dst32 = *tmp_src_ptr++;

                alpha = (dst32>>24);
                if(0 == alpha)
                 {
                      *tmp_dst_ptr++ = 0xff000000;
                 }
                 else if(255 == alpha)
                 {
                      *tmp_dst_ptr++ = (dst32&0x00ffffff);
                 }
                 else
                 {
                       g = dst32&0x0000ff00;
                       rb = dst32&0x00ff00ff;
                      
                       g = ((g*alpha)>>8)&0x0000ff00;
                       rb =((rb*alpha)>>8)&0x00ff00ff;
                       
                       alpha= ((255 - alpha) <<24);
                       dst32= (g+rb);
                        *tmp_dst_ptr++ = (alpha|dst32) ;
                       
                 }
               
            }
        }     
    }

    return;


}





extern uint32 DMA_HAL_Memcpy (void *des, void *src, uint32 count);
extern uint32 DMA_HAL_Memset (void *des, void *value, uint32 count, uint32);

PUBLIC void DISPLAY_MEMCPY(void *des, void *src, uint32 count)
{
#ifndef WIN32
    DMA_HAL_Memcpy (des, src, count);
#else
    SCI_MEMCPY(des,src,count);
#endif
}


//value_size; 1: byte(uint8); 2: halfword(uint16); 4: word(uint32)
//count; pixel;
PUBLIC void DISPLAY_MEMSET(void *des, uint32 value, uint32 count, uint32 value_size)
{
#ifndef WIN32
    uint32 size = 0;
    uint32 byte_count = 0;
    
    if(1 == value_size)
    {
        size = 0;   
        byte_count = count;
    }
    
    if(2 == value_size)
    {
        size = 1;  
        byte_count = count * 2;
    }
    else if(4 == value_size)
    {
        size = 2;
        byte_count = count * 4;
    }
    else
    {
        return; //do nothing
    }
    //value will be used by IRAM space later.
    DMA_HAL_Memset (des, &value, byte_count, size);
#else

    if(1 == value_size)
    {   
        SCI_MEMSET(des,value,count);
    }
    else if(2 == value_size)
    {
        uint32 i;
		uint16 *dst_ptr = (uint16 *)des;
		uint16 value16 = (uint16)value;
		for(i=0; i<count;i++)
		{
			*dst_ptr++ = value16;
		}
    }
    else if(4 == value_size)
    {
        uint32 i;
		uint32 *dst_ptr = (uint32 *)des;
		uint32 value32 = (uint32)value;
		for(i=0; i<count;i++)
		{
			*dst_ptr++ = value32;
		}
    }
    else
    {
        ;
    }
    
#endif
}


#define RGB_TO_RGB565(r, g, b)  ((((r) << 8) & 0xf800) |(((b) >> 3) & 0x1f) |  ( ( ((g)>>0x02)&0x3f)<<5) ) 

#define RGB_TO_ARGB888(r, g, b) (0xff000000|((r) << 16) |(b)  |   (g<<8) ) 

//r = y + 1.402 * (v - 128);
#define YUV_TO_R(_y, _v) ((int32)(_y) + ((359 * ((int32)(_v) - 128)) >> 8))
//g =y - 0.34414 * (u - 128) - 0.71414 * (v - 128);
#define YUV_TO_G(_y, _u, _v) ((int32)(_y) - ((88 * ((int32)(_u) - 128) + 183 * ((int32)(_v) - 128)) >> 8))
//b = y + 1.772 * (u - 128);
#define YUV_TO_B(_y, _u) ((int32)(_y) + ((454 * ((int32)(_u) - 128)) >> 8))

#define CLIP(x, bottom, top) ((x) < bottom ? bottom : (x) > top ? top :(x))

//Y = 0.299 * r + 0.587 * g + 0.114 * b;
#define RGB_TO_Y(_r, _g, _b)    (uint8)((77 * (_r) + 150 * (_g) + 29 * (_b)) >> 8)
//U = 128 - 0.1687 * r - 0.3313 * g + 0.5 * b 
#define RGB_TO_U(_r, _g, _b)    (uint8)(128 + ((128 * (_b) - 43 * (_r) - 85 * (_g)) >> 8))
//V = 128 + 0.5 * r - 0.4187 * g - 0.0813 * b 
#define RGB_TO_V(_r, _g, _b)    (uint8)(128  + ((128 * (_r) - 107 * (_g) - 21 * (_b)) >> 8))

#define RGB565_TO_RGB(A,R,G,B) \
	(B) = (((A)&0x1f)<<3); \
	(G) = (((A)>>3)&0xfc); \
	(R) = (((A)<<3)&0xF8);

#define RGB888_TO_RGB(A,R,G,B) \
	(B) = (((A)&0xff)); \
	(G) = (((A)>>8)&0xff); \
	(R) = (((A)>>16)&0xff);



PUBLIC void DISPLAY_YUV2RGB565(uint16 *rgb565_ptr, uint8 * y_ptr, uint8 * uv_ptr, uint32 w,uint32 h)
{
    uint32 i;
    uint32 r=0x00;
    uint32 g=0x00;
    uint32 b=0x00;
    uint32 y, u, v;

    uint32 rgb565;
    for(i=0; i<w*h; i=i+2)
    {

        y = y_ptr[i];
        u = uv_ptr[i];
        v = uv_ptr[i+1];
        
        r=YUV_TO_R(y, v);
        r=CLIP(r, 0x00, 0xff);
        g=YUV_TO_G(y, u, v);
        g=CLIP(g, 0x00, 0xff);
        b=YUV_TO_B(y, u);
        b=CLIP(b, 0x00, 0xff);
        

        rgb565 = RGB_TO_RGB565(r,g,b);

        *rgb565_ptr++ = rgb565;

        y = y_ptr[i+1];
        
        r=YUV_TO_R(y, v);
        r=CLIP(r, 0x00, 0xff);
        g=YUV_TO_G(y, u, v);
        g=CLIP(g, 0x00, 0xff);
        b=YUV_TO_B(y, u);
        b=CLIP(b, 0x00, 0xff);
        

        rgb565 = RGB_TO_RGB565(r,g,b);

        *rgb565_ptr++ = rgb565;
        

   }

   return;
}



PUBLIC void DISPLAY_YUV2ARGB888(uint32 *argb888_ptr, uint8 * y_ptr, uint8 * uv_ptr, uint32 w,uint32 h)
{
    uint32 i;
    uint32 r=0x00;
    uint32 g=0x00;
    uint32 b=0x00;
    uint32 y, u, v;

    uint32 argb888;
    for(i=0; i<w*h; i=i+2)
    {

        y = y_ptr[i];
        u = uv_ptr[i];
        v = uv_ptr[i];
        
        r=YUV_TO_R(y, v);
        r=CLIP(r, 0x00, 0xff);
        g=YUV_TO_G(y, u, v);
        g=CLIP(g, 0x00, 0xff);
        b=YUV_TO_B(y, u);
        b=CLIP(b, 0x00, 0xff);
        

        argb888 = RGB_TO_ARGB888(r,g,b);

        *argb888_ptr++ = argb888;

        y = y_ptr[i+1];
        
        r=YUV_TO_R(y, v);
        r=CLIP(r, 0x00, 0xff);
        g=YUV_TO_G(y, u, v);
        g=CLIP(g, 0x00, 0xff);
        b=YUV_TO_B(y, u);
        b=CLIP(b, 0x00, 0xff);
        

        argb888 = RGB_TO_ARGB888(r,g,b);

        *argb888_ptr++ = argb888;
        

   }

   return;
}


//width must be 2X pixel.
PUBLIC void DISPLAY_RGB5652YUV422( uint8 * y_ptr,uint8 * uv_ptr, uint16 *rgb565_ptr,uint32 w,uint32 h)
{
    uint32 i;
    uint32 r=0x00;
    uint32 g=0x00;
    uint32 b=0x00;
    uint32 u1, v1;
    uint32  u2, v2;

    uint32 rgb565;

    for(i=0; i<w*h; i=i+2)
    {
        rgb565 = *rgb565_ptr++;
		
		RGB565_TO_RGB(rgb565, r, g, b)
        *y_ptr++ = RGB_TO_Y(r, g, b);
        u1 = RGB_TO_U(r, g, b);
		v1 = RGB_TO_V(r, g, b);
			
		rgb565 = *rgb565_ptr++;
		RGB565_TO_RGB(rgb565, r, g, b)			
       *y_ptr++ = RGB_TO_Y(r, g, b);
        u2 = RGB_TO_U(r, g, b);
		v2 = RGB_TO_V(r, g, b);		

		*uv_ptr++ = (u1 + u2) >> 1;
		*uv_ptr++ = (v1 + v2) >> 1;

   }

   return;
}


//width must be 2X pixel.
//argb888_ptr is image, not osd. Alpha will be skipped.
PUBLIC void DISPLAY_ARGB8882YUV422( uint8 * y_ptr,uint8 * uv_ptr, uint16 *argb888_ptr,uint32 w,uint32 h)
{
    uint32 i;
    uint32 r=0x00;
    uint32 g=0x00;
    uint32 b=0x00;
    uint32 u1, v1;
    uint32  u2, v2;

    uint32 rgb888;

    for(i=0; i<w*h; i=i+2)
    {
        rgb888 = *argb888_ptr++;
		
		RGB888_TO_RGB(rgb888, r, g, b)
        *y_ptr++ = RGB_TO_Y(r, g, b);
        u1 = RGB_TO_U(r, g, b);
		v1 = RGB_TO_V(r, g, b);
			
		rgb888 = *argb888_ptr++;
		RGB888_TO_RGB(rgb888, r, g, b)			
       *y_ptr++ = RGB_TO_Y(r, g, b);
        u2 = RGB_TO_U(r, g, b);
		v2 = RGB_TO_V(r, g, b);		

		*uv_ptr++ = (u1 + u2) >> 1;
		*uv_ptr++ = (v1 + v2) >> 1;

   }

   return;
}

/*lint +e685 +e568*/

#ifdef __cplusplus
    }
#endif



