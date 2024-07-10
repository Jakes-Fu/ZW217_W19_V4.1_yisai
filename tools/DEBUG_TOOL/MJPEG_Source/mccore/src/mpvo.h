typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef struct vo_info_s
{
        /* driver name ("Matrox Millennium G200/G400" */
        const char *name;
        /* short name (for config strings) ("mga") */
        const char *short_name;
        /* author ("Aaron Holtzman <aholtzma@ess.engr.uvic.ca>") */
        const char *author;
        /* any additional comments */
        const char *comment;
} vo_info_t;

typedef struct vo_functions_s
{
	vo_info_t *info;
	/*
	 * Preinitializes driver (real INITIALIZATION)
	 *   arg - currently it's vo_subdevice
	 *   returns: zero on successful initialization, non-zero on error.
	 */
	int (*preinit)(const char *arg);
        /*
         * Initialize (means CONFIGURE) the display driver.
	 * params:
         *   width,height: image source size
	 *   d_width,d_height: size of the requested window size, just a hint
	 *   fullscreen: flag, 0=windowd 1=fullscreen, just a hint
	 *   title: window title, if available
	 *   format: fourcc of pixel format
         * returns : zero on successful initialization, non-zero on error.
         */
        int (*config)(uint32_t width, uint32_t height, uint32_t d_width,
			 uint32_t d_height, uint32_t fullscreen, char *title,
			 uint32_t format);

	/*
	 * Control interface
	 */
	int (*control)(uint32_t request, void *data, ...);

        /*
         * Display a new RGB/BGR frame of the video to the screen.
         * params:
	 *   src[0] - pointer to the image
         */
        int (*draw_frame)(uint8_t *src[]);

        /*
         * Draw a planar YUV slice to the buffer:
	 * params:
	 *   src[3] = source image planes (Y,U,V)
         *   stride[3] = source image planes line widths (in bytes)
	 *   w,h = width*height of area to be copied (in Y pixels)
         *   x,y = position at the destination image (in Y pixels)
         */
        int (*draw_slice)(uint8_t *src[], int stride[], int w,int h, int x,int y);

   	/*
         * Draws OSD to the screen buffer
         */
        void (*draw_osd)(void);

        /*
         * Blit/Flip buffer to the screen. Must be called after each frame!
         */
        void (*flip_page)(void);

        /*
         * This func is called after every frames to handle keyboard and
	 * other events. It's called in PAUSE mode too!
         */
        void (*check_events)(void);

        /*
         * Closes driver. Should restore the original state of the system.
         */
        void (*uninit)(void);

} vo_functions_t;

typedef struct {
	int* adapter_num;
	int* dbpp;
	int* directrendering;
	int* doublebuffering;
	int* dx;
	int* dy;
	int* fs;
	int* keepaspect;
	float* panscan;
	int* refresh_rate;
	int* screenheight;
	int* screenwidth;
	int* vidmode;
} VO_INFO;

#define LIBVO_EXTERN(x) vo_functions_t video_out_##x =\
{\
	&info,\
	preinit,\
	config,\
	control,\
	draw_frame,\
	draw_slice,\
     	draw_osd,\
	flip_page,\
	check_events,\
	uninit\
};
