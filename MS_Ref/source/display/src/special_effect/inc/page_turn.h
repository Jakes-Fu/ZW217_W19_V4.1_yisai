#ifndef _PAGE_TURN_OFF_H_
#define _PAGE_TURN_OFF_H_

#include "sci_types.h"
#include "img_ref_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	PAGE_TURN_LEFT_TOP = 0,
	PAGE_TURN_RIGHT_TOP,
	PAGE_TURN_LEFT_BOTTOM,				//turn from the left bottom
	PAGE_TURN_RIGHT_BOTTOM,				//turn from the right bottom
}PAGE_TURN_OFF_MODE_E;

typedef enum
{
	PAGE_TURN_BACKFACE_TEXT = 0,		//display text on backface
	PAGE_TURN_BACKFACE_COLOR			//display color on backface
}PAGE_TURN_OFF_BACKFACE_PATTERN_E;


typedef struct
{
	uint16					page_width;				//width of page
	uint16					page_height;			//height of page
	void					*cur_page_ptr;			//data of current page
	void					*next_page_ptr;			//data of next page
	IMGREF_DATA_FORMAT_E	format;					//format of page data, only support RGB565 format
	uint32					backface_color;			//backface color of the page	

	PAGE_TURN_OFF_BACKFACE_PATTERN_E	backface_pattern;	//backface pattern
	
	PAGE_TURN_OFF_MODE_E	mode;					//mode of turn off

	void					*target_buf_ptr;
	uint32					target_buf_size;

	int16					frame_index;			//set to -1 if use hot_spot 	
	int16					hot_spot;				// [1, page_width - 1]

	
}PAGE_TURN_OFF_T;

PUBLIC int32 PAGE_GetFrames();

PUBLIC int32 PAGE_TurnOff(PAGE_TURN_OFF_T *turn_off_ptr);

#ifdef __cplusplus
}
#endif

#endif