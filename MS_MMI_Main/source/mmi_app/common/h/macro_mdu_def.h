//在定义某种资源的时候取出该资源的id，把其他资源后定义忽略掉
#define MACRO_MDU_NONE      0
#define MACRO_MDU_IMAGE     1
#define MACRO_MDU_ANIM      2
#define MACRO_MDU_TEXT      3
#define MACRO_MDU_DATA      4
#define MACRO_MDU_COLOR     5
#define MACRO_MDU_RECT      6
#define MACRO_MDU_RING		7
#define MACRO_MDU_QBTHEME	8
#define MACRO_MDU_OUTSIDE 9

#undef RES_ADD_IMG
#undef RES_ADD_IMG2
#undef RES_ADD_ANIM
#undef RES_ADD_STRING
#undef RES_ADD_COLOR
#undef RES_ADD_RECT
#undef RES_ADD_RING
#undef RES_ADD_DATA

#undef RES_ADD_IMG_TAG
#undef RES_ADD_ANIM_TAG
#undef RES_ADD_DATA_TAG
#undef RES_ADD_COLOR_TAG
#undef RES_ADD_RECT_TAG
#undef RES_ADD_IMG_RANGE
#undef RES_ADD_RING_TAG
#undef RES_ADD_IMG_QBTHEME_MAP
#undef MMI_RES_ADD_IMG_OUTSIDE_TAG
#undef RES_ADD_DATA_TAG

/*****************************************************************************/
//imageid:  图片ID
//param1:   图片相对路径
//param2:   图片压缩类型
//param3:   图片压缩参数(JPEG压缩率参见IMG_JPG_QULITY_E)
/*****************************************************************************/
#define RES_ADD_IMG(IMAGE_ID, IMAGE_PATH, IMAGE_COMPRESS_TYPE, IMAGE_QULITY_TYPE) 

#define RES_ADD_IMG2(IMAGE_ID, IMAGE_PATH, IMAGE_FOLDER, IMAGE_COMPRESS_TYPE, IMAGE_QULITY_TYPE) 

/*****************************************************************************/
//animid:   动画ID
//param1:   动画相对路径
//param2:   动画压缩类型
//param3:   动画压缩参数(JPEG压缩率参见IMG_JPG_QULITY_E)
//param4:   动画的关键色
/*****************************************************************************/
#define RES_ADD_ANIM(ANIM_ID, ANIM_PATH, ANIM_COMPRESS, ANIM_QULITY_TYPE, ANIM_COLOR_KEY)

/*****************************************************************************/
//textid:           字符串ID
//default_string:   字符串默认内容
/*****************************************************************************/
#define RES_ADD_STRING(TXT_ID, DEFAULT_STRING)

#define RES_ADD_COLOR(COLOR_ID, DEFAULT_COLOR)

#define RES_ADD_RECT(RECT_ID, DEFAULT_LEFT, DEFAULT_TOP, DEFAULT_RIGHT, DEFAULT_BOTTOM)

#define RES_ADD_RING(RING_ID,ring_path) 

#define RES_ADD_IMG_QBTHEME_MAP(ID, RANGE, PATH, OFFSET)

#define MMI_RES_ADD_IMG_OUTSIDE_TAG(ID, PATH, OFFSET)

#define RES_ADD_DATA(ID, PATH)

/*****************************************************************************/
//resource id:    
//resource description: max length 36, only latin string   
/*****************************************************************************/
#define RES_ADD_IMG_TAG(IMAGE_ID, DESCRIPTION)
#define RES_ADD_ANIM_TAG(ANIM_ID, DESCRIPTION)
#define RES_ADD_DATA_TAG(DATA_ID, DESCRIPTION)
#define RES_ADD_COLOR_TAG(COLOR_ID, DESCRIPTION)
#define RES_ADD_RECT_TAG(RECT_ID, DESCRIPTION)
#define RES_ADD_IMG_RANGE(IMAGE_ID, IMAGE_NUM, IMAGE_PATH, IMAGE_COMPRESS_TYPE, IMAGE_PARAM)
#define RES_ADD_IMG_TAG_RANGE(ID,NUM,DESCRIPTION)
#define RES_ADD_RING_TAG(RING_ID, DESCRIPTION)
#define RES_ADD_DATA_TAG(DATA_ID, DESCRIPTION)


//IMAGE_MP3_MAIN_BG
#if (MACRO_MDU_TYPE == MACRO_MDU_IMAGE)
    #undef RES_ADD_IMG
    #define RES_ADD_IMG(IMAGE_ID, param1, param2, param3)   IMAGE_ID,
    #undef RES_ADD_IMG_RANGE
    #define RES_ADD_IMG_RANGE(IMAGE_ID, IMAGE_NUM, IMAGE_PATH, IMAGE_COMPRESS_TYPE, IMAGE_PARAM) \
            IMAGE_ID##_1,\
            IMAGE_ID##_##IMAGE_NUM = IMAGE_ID##_1 + IMAGE_NUM - 1,
		#undef RES_ADD_IMG2
		#define RES_ADD_IMG2(IMAGE_ID, param1, param2, param3, param4)   IMAGE_ID,
#elif (MACRO_MDU_TYPE == MACRO_MDU_ANIM)
    #undef RES_ADD_ANIM
    #define RES_ADD_ANIM(ANIM_ID, param1, param2, param3, param4)   ANIM_ID,
#elif (MACRO_MDU_TYPE == MACRO_MDU_TEXT)
    #undef RES_ADD_STRING
    #define RES_ADD_STRING(TXT_ID, default_string)  TXT_ID,
#elif (MACRO_MDU_TYPE == MACRO_MDU_DATA)
    #undef RES_ADD_DATA
    #define RES_ADD_DATA(DATA_ID,data_path)  DATA_ID,
#elif (MACRO_MDU_TYPE == MACRO_MDU_COLOR)
    #undef RES_ADD_COLOR
    #define RES_ADD_COLOR(COLOR_ID, default_color)  COLOR_ID,
#elif (MACRO_MDU_TYPE == MACRO_MDU_RECT)
    #undef RES_ADD_RECT
    #define RES_ADD_RECT(RECT_ID, default_left, default_top, default_right, default_bottom)  RECT_ID,
#elif (MACRO_MDU_TYPE == MACRO_MDU_RING)
    #undef RES_ADD_RING
    #define RES_ADD_RING(RING_ID,ring_path)         RING_ID,	
#elif (MACRO_MDU_TYPE == MACRO_MDU_QBTHEME)
    #undef RES_ADD_IMG_QBTHEME_MAP
    #define RES_ADD_IMG_QBTHEME_MAP(ID, RANGE, PATH, OFFSET)				{ID, RANGE, PATH, OFFSET}, 
#elif (MACRO_MDU_TYPE == MACRO_MDU_OUTSIDE)
    #undef MMI_RES_ADD_IMG_OUTSIDE_TAG
    #define MMI_RES_ADD_IMG_OUTSIDE_TAG(ID, PATH, OFFSET)				{ID, PATH, OFFSET},	
#elif (MACRO_MDU_TYPE == 0)
    #error "Undefined MACRO_MDU_TYPE!"
#else
    #error "Unknown MACRO_MDU_TYPE!"
#endif
