/*
 *  video_out.h
 *
 *      Copyright (C) Aaron Holtzman - Aug 1999
 *	Strongly modified, most parts rewritten: A'rpi/ESP-team - 2000-2001
 *	(C) MPlayer Developers
 *
 */
 
#ifndef __VIDEO_OUT_H
#define __VIDEO_OUT_H 1

#include "mpvo.h"

//#include "font_load.h"
#include "img_format.h"
//#include "vidix/vidix.h"

#define VO_EVENT_EXPOSE 1
#define VO_EVENT_RESIZE 2
#define VO_EVENT_KEYPRESS 4

/* Obsolete: VOCTRL_QUERY_VAA 1 */
/* does the device support the required format */
#define VOCTRL_QUERY_FORMAT 2
/* signal a device reset seek */
#define VOCTRL_RESET 3
/* true if vo driver can use GUI created windows */
#define VOCTRL_GUISUPPORT 4
#define VOCTRL_GUI_NOWINDOW 19
/* used to switch to fullscreen */
#define VOCTRL_FULLSCREEN 5
#define VOCTRL_SCREENSHOT 6
/* signal a device pause */
#define VOCTRL_PAUSE 7
/* start/resume playback */
#define VOCTRL_RESUME 8
/* libmpcodecs direct rendering: */
#define VOCTRL_GET_IMAGE 9
#define VOCTRL_DRAW_IMAGE 13
#define VOCTRL_SET_SPU_PALETTE 14
/* decoding ahead: */
#define VOCTRL_GET_NUM_FRAMES 10
#define VOCTRL_GET_FRAME_NUM  11
#define VOCTRL_SET_FRAME_NUM  12
#define VOCTRL_GET_PANSCAN 15
#define VOCTRL_SET_PANSCAN 16
/* equalizer controls */
#define VOCTRL_SET_EQUALIZER 17
#define VOCTRL_GET_EQUALIZER 18
//#define VOCTRL_GUI_NOWINDOW 19
/* Frame duplication */
#define VOCTRL_DUPLICATE_FRAME 20
// ... 21
#define VOCTRL_START_SLICE 21

#define VOCTRL_ONTOP 25
#define VOCTRL_ROOTWIN 26
#define VOCTRL_BORDER 27

// Vo can be used by xover
#define VOCTRL_XOVERLAY_SUPPORT 22

#define VOCTRL_XOVERLAY_SET_COLORKEY 24
typedef struct {
  uint32_t x11; // The raw x11 color
  uint16_t r,g,b;
} mp_colorkey_t;

#define VOCTRL_XOVERLAY_SET_WIN 23
typedef struct {
  int x,y;
  int w,h;
} mp_win_t;

#define VO_TRUE		1
#define VO_FALSE	0
#define VO_ERROR	-1
#define VO_NOTAVAIL	-2
#define VO_NOTIMPL	-3

#define VOFLAG_FULLSCREEN	0x01
#define VOFLAG_MODESWITCHING	0x02
#define VOFLAG_SWSCALE		0x04
#define VOFLAG_FLIPPING		0x08
#define VOFLAG_XOVERLAY_SUB_VO  0x10000

char *vo_format_name(int format);
int vo_init(void);

vo_functions_t* init_best_video_out(char** vo_list);
void list_video_out(void);

// NULL terminated array of all drivers
extern vo_functions_t* video_out_drivers[];

extern int vo_flags;

extern int vo_config_count;

// correct resolution/bpp on screen:  (should be autodetected by vo_init())
extern int vo_depthonscreen;
extern int vo_screenwidth;
extern int vo_screenheight;

// requested resolution/bpp:  (-x -y -bpp options)
extern int vo_gamma_brightness;
extern int vo_gamma_saturation;
extern int vo_gamma_contrast;
extern int vo_gamma_hue;
extern int vo_gamma_red_intensity;
extern int vo_gamma_green_intensity;
extern int vo_gamma_blue_intensity;

extern int vo_mouse_timer_const;
extern int vo_nomouse_input;

extern int vo_pts;
extern float vo_fps;

extern char *vo_subdevice;

extern int vo_colorkey;

#if defined(HAVE_FBDEV)||defined(HAVE_VESA) 

typedef struct {
        float min;
	float max;
	} range_t;

extern float range_max(range_t *r);
extern int in_range(range_t *r, float f);
extern range_t *str2range(char *s);
extern char *monitor_hfreq_str;
extern char *monitor_vfreq_str;
extern char *monitor_dotclock_str;

#endif 


//--------- codec's requirements (filled by the codec/vf) ---------

//--- buffer content restrictions:
// set if buffer content shouldn't be modified:
#define MP_IMGFLAG_PRESERVE 0x01
// set if buffer content will be READ for next frame's MC: (I/P mpeg frames)
#define MP_IMGFLAG_READABLE 0x02

//--- buffer width/stride/plane restrictions: (used for direct rendering)
// stride _have_to_ be aligned to MB boundary:  [for DR restrictions]
#define MP_IMGFLAG_ACCEPT_ALIGNED_STRIDE 0x4
// stride should be aligned to MB boundary:     [for buffer allocation]
#define MP_IMGFLAG_PREFER_ALIGNED_STRIDE 0x8
// codec accept any stride (>=width):
#define MP_IMGFLAG_ACCEPT_STRIDE 0x10
// codec accept any width (width*bpp=stride -> stride%bpp==0) (>=width):
#define MP_IMGFLAG_ACCEPT_WIDTH 0x20
//--- for planar formats only:
// uses only stride[0], and stride[1]=stride[2]=stride[0]>>mpi->chroma_x_shift
#define MP_IMGFLAG_COMMON_STRIDE 0x40
// uses only planes[0], and calculates planes[1,2] from width,height,imgfmt
#define MP_IMGFLAG_COMMON_PLANE 0x80

#define MP_IMGFLAGMASK_RESTRICTIONS 0xFF

//--------- color info (filled by mp_image_setfmt() ) -----------
// set if number of planes > 1
#define MP_IMGFLAG_PLANAR 0x100
// set if it's YUV colorspace
#define MP_IMGFLAG_YUV 0x200
// set if it's swapped (BGR or YVU) plane/byteorder
#define MP_IMGFLAG_SWAPPED 0x400
// using palette for RGB data
#define MP_IMGFLAG_RGB_PALETTE 0x800

#define MP_IMGFLAGMASK_COLORS 0xF00

// codec uses drawing/rendering callbacks (draw_slice()-like thing, DR method 2)
// [the codec will set this flag if it supports callbacks, and the vo _may_
//  clear it in get_image() if draw_slice() not implemented]
#define MP_IMGFLAG_DRAW_CALLBACK 0x1000
// set if it's in video buffer/memory: [set by vo/vf's get_image() !!!]
#define MP_IMGFLAG_DIRECT 0x2000
// set if buffer is allocated (used in destination images):
#define MP_IMGFLAG_ALLOCATED 0x4000

// buffer type was printed (do NOT set this flag - it's for INTERNAL USE!!!)
#define MP_IMGFLAG_TYPE_DISPLAYED 0x8000

// codec doesn't support any form of direct rendering - it has own buffer
// allocation. so we just export its buffer pointers:
#define MP_IMGTYPE_EXPORT 0
// codec requires a static WO buffer, but it does only partial updates later:
#define MP_IMGTYPE_STATIC 1
// codec just needs some WO memory, where it writes/copies the whole frame to:
#define MP_IMGTYPE_TEMP 2
// I+P type, requires 2+ independent static R/W buffers
#define MP_IMGTYPE_IP 3
// I+P+B type, requires 2+ independent static R/W and 1+ temp WO buffers
#define MP_IMGTYPE_IPB 4

#define MP_MAX_PLANES	4

#define MP_IMGFIELD_ORDERED 0x01
#define MP_IMGFIELD_TOP_FIRST 0x02
#define MP_IMGFIELD_REPEAT_FIRST 0x04
#define MP_IMGFIELD_TOP 0x08
#define MP_IMGFIELD_BOTTOM 0x10
#define MP_IMGFIELD_INTERLACED 0x20

typedef struct mp_image_s {
    unsigned short flags;
    unsigned char type;
    unsigned char bpp;  // bits/pixel. NOT depth! for RGB it will be n*8
    unsigned int imgfmt;
    int width,height;  // stored dimensions
    int x,y,w,h;  // visible dimensions
    unsigned char* planes[MP_MAX_PLANES];
    int stride[MP_MAX_PLANES];
    char * qscale;
    int qstride;
    int pict_type; // 0->unknown, 1->I, 2->P, 3->B
    int fields;
    int qscale_type; // 0->mpeg1/4/h263, 1->mpeg2
    int num_planes;
    /* these are only used by planar formats Y,U(Cb),V(Cr) */
    int chroma_width;
    int chroma_height;
    int chroma_x_shift; // horizontal
    int chroma_y_shift; // vertical
    /* for private use by filter or vo driver (to store buffer id or dmpi) */
    void* priv;
} mp_image_t;

// VFCAP_* values: they are flags, returned by query_format():

// set, if the given colorspace is supported (with or without conversion)
#define VFCAP_CSP_SUPPORTED 0x1
// set, if the given colorspace is supported _without_ conversion
#define VFCAP_CSP_SUPPORTED_BY_HW 0x2
// set if the driver/filter can draw OSD
#define VFCAP_OSD 0x4
// set if the driver/filter can handle compressed SPU stream
#define VFCAP_SPU 0x8
// scaling up/down by hardware, or software:
#define VFCAP_HWSCALE_UP 0x10
#define VFCAP_HWSCALE_DOWN 0x20
#define VFCAP_SWSCALE 0x40
// driver/filter can do vertical flip (upside-down)
#define VFCAP_FLIP 0x80

// driver/hardware handles timing (blocking)
#define VFCAP_TIMER 0x100
// driver _always_ flip image upside-down (for ve_vfw)
#define VFCAP_FLIPPED 0x200
// vf filter: accepts stride (put_image)
// vo driver: has draw_slice() support for the given csp
#define VFCAP_ACCEPT_STRIDE 0x400
// filter does postprocessing (so you shouldn't scale/filter image before it)
#define VFCAP_POSTPROC 0x800
// filter cannot be reconfigured to different size & format
#define VFCAP_CONSTANT 0x1000

#endif
