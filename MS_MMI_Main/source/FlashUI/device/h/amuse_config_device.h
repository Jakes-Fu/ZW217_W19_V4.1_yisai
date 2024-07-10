/*
 * amuse_config_device.h
 * Copyright (C) 2011 Microrapid Inc
 * Author: Tim <tim@Microrapid.com>
 * 
 * This file contains configuration interface.
 */

#ifndef _AMUSE_CONFIG_DEVICE_H_
#define _AMUSE_CONFIG_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

//#ifndef  IMPORT_C
#if WIN32
#define IMPORT_C  extern
#define EXPORT_C
//#define IMPORT_C	__declspec(dllimport)
//#define EXPORT_C   __declspec(dllexport)
#else
#define IMPORT_C extern
#define EXPORT_C
#endif
//#endif

#define AMUSE_MEMORY_SIZE		(6 * 1024 * 1024)

//IMPORT_C unsigned int amuse_memory_size;
IMPORT_C unsigned int heap_expand_size;
IMPORT_C unsigned int max_heaps_total_size;

IMPORT_C short amuse_screen_width;

IMPORT_C short amuse_screen_height;
IMPORT_C short need_screen_rotate; 

IMPORT_C int max_jpeg_data_totalsize;
IMPORT_C int max_jpeg_data_size;
IMPORT_C int min_jpeg_data_size;
IMPORT_C short bitmap_ratio_bit;
IMPORT_C int max_jpeg_number;
IMPORT_C int amuse_jpeg_dither_mode;

IMPORT_C short amuseUseJpegLib;
IMPORT_C short use_hardware_decode_jpeg;
IMPORT_C short use_hardware_decode_image;
IMPORT_C short use_hardware_decode_video;

IMPORT_C int loader_buffer_size;
IMPORT_C short use_amuse_write_file;
IMPORT_C int amuse_max_swfsize_onlyuse_memory;
IMPORT_C int remove_temp_file_when_exit;

IMPORT_C int amuse_network_retry_num;
IMPORT_C int amuse_network_connect_timeout_ms;

IMPORT_C char back_run_enable;
IMPORT_C char picture_zoom_enable;
IMPORT_C char enable_allow_scale;

IMPORT_C short amuse_show_fps;
IMPORT_C char sol_file_path[260];

EXPORT_C void amuse_config_default();
EXPORT_C void amuse_config_setDefault(int width, int height);
EXPORT_C int amuse_config_readFromFile(const char *filename);

#ifdef __cplusplus
}
#endif

#endif

