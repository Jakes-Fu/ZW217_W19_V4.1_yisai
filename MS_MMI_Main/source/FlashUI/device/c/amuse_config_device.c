/*
 * amuse_config_device.c
 * Copyright (C) 2011 Microrapid Inc
 * Author: gentoo <gentoo@Microrapid.com>
 * 
 * This file contains device related configuration settings. for SPRD
 */
#include "amuse.h"
#include "amuse_config_device.h"
#include "amuse_display.h"

void 
amuse_config_setDefault(int width, int height)
{
	amuse_screen_width = (short)width;	
	amuse_screen_height = (short)height;
	need_screen_rotate = 0;//0 is false ;1 is true, rotate 90 

	heap_expand_size = 0;

	max_heaps_total_size = AMUSE_MEMORY_SIZE;
	
	max_jpeg_data_totalsize = 0xA00000;//0xA00000;//0x200000=2M 0x500000=5M 0xA00000=10M 0xF00000=15M
	max_jpeg_data_size = 0x100000;//0x100000; //0x19000=100K,  0x100000=1M, 0xC800=50k, 0x32000 = 200k     
	min_jpeg_data_size = 0x100000;//30k
	bitmap_ratio_bit = 0;//bitmap_ratio=2
	max_jpeg_number	=40000;//400;	
	amuse_jpeg_dither_mode = 2;//JDITHER_NONE=0,JDITHER_FS=2

	amuseUseJpegLib = 1;
	use_hardware_decode_jpeg = 1;
	use_hardware_decode_video = 0;

	loader_buffer_size = 0x200000;

	amuse_max_swfsize_onlyuse_memory = 0x400000; //0x1400000 =20M 
	use_amuse_write_file = 0;

	remove_temp_file_when_exit = 0;

	amuse_show_fps = 1;

	amuse_set_screen_size(amuse_screen_width, amuse_screen_height);
}
