//在定义某种资源的时候取出该资源的id，把其他资源后定义忽略掉
#define MACRO_RES_PRJ_NONE          0
#define MACRO_RES_PRJ_LANGUAGE      1
#define MACRO_RES_PRJ_RING          2
#define MACRO_RES_PRJ_FONT          3
#define MACRO_RES_PRJ_MODULE        4
#define MACRO_RES_PRJ_DATA          5
#define MACRO_RES_PRJ_TEST_MOD  6
#undef RES_ADD_LANGUAGE
#define RES_ADD_LANGUAGE(language_id, language_string)       //none
#define RES_SET_VERSION(version_string)     //none
#define RES_SET_MAX_MODULE_NUM(num)         //none
#define RES_SET_IMG_DEFAULT_COMPRESS_TYPE(type, percent)    //none
#define RES_SET_ANIM_DEFAULT_COMPRESS_TYPE(type, percent)   //none
#define RES_SET_IMG_GLOBAL_COMPRESS_TYPE(type, percent)     //none
#define RES_SET_ANIM_GLOBAL_COMPRESS_TYPE(type, percent)    //none
#define RES_SET_ANIM_GLOBAL_KEY_COLOR(color)        //none
#define RES_SET_GLOBAL_JPEG_COMPRESS_RATE(percent)  //none
#define RES_ADD_SKIN(skin_name, version_string, index)  //none
#define RES_SET_MAX_RING_SIZE(size) //none
#undef RES_ADD_MODULE
#define RES_ADD_MODULE(module_id, module_path)          //none
//#define RES_ADD_RING(RING_ID, param1,ring_path)         RING_ID,
#undef RES_ADD_RING
#define RES_ADD_RING(RING_ID,ring_path)         //none
#undef RES_ADD_FONT
#define RES_ADD_FONT(FONT_ID, font_path, describe_string)   //none
#undef RES_ADD_DATA
#define RES_ADD_DATA(DATA_ID,data_path)   //none

#undef RES_ADD_LOGO
#define RES_ADD_LOGO(IMAGE_ID, IMAGE_PATH, IMAGE_COMPRESS_TYPE, IMAGE_QULITY_TYPE) 
#undef RES_ADD_RING_TAG
#define RES_ADD_RING_TAG(RING_ID, DESCRIPTION)
#undef RES_ADD_FONT_TAG
#define RES_ADD_FONT_TAG(FONT_ID, DESCRIPTION)
#undef RES_ADD_LOGO_TAG
#define RES_ADD_LOGO_TAG(IMAGE_ID, IMAGE_NAME) 
#undef RES_ADD_RING_RANGE
#define RES_ADD_RING_RANGE(RING_ID, RING_NUM, RING_PATH)

#undef RES_SET_IMG_CMP_DEFAULT
#define RES_SET_IMG_CMP_DEFAULT(img_type, img_cmp_type, img_cmp_param)

//IMAGE_MP3_MAIN_BG
#if (MACRO_RES_PRJ_TYPE == MACRO_RES_PRJ_LANGUAGE)
    #undef RES_ADD_LANGUAGE
    #define RES_ADD_LANGUAGE(language_id, language_string)         language_id,
#elif (MACRO_RES_PRJ_TYPE == MACRO_RES_PRJ_RING)
    #undef RES_ADD_RING
    #define RES_ADD_RING(RING_ID,ring_path)         RING_ID,
    #undef RES_ADD_RING_RANGE
    #define RES_ADD_RING_RANGE(RING_ID, RING_NUM, RING_PATH) \
            RING_ID##_1,\
            RING_ID##_##RING_NUM = RING_ID##_1 + RING_NUM - 1,
#elif (MACRO_RES_PRJ_TYPE == MACRO_RES_PRJ_FONT)
    #undef RES_ADD_FONT
    #define RES_ADD_FONT(FONT_ID, font_path, describe_string)         FONT_ID,
#elif (MACRO_RES_PRJ_TYPE == MACRO_RES_PRJ_MODULE)
    #undef RES_ADD_MODULE
    #define RES_ADD_MODULE(module_id, module_path)         module_id, //none
#elif (MACRO_RES_PRJ_TYPE == MACRO_RES_PRJ_TEST_MOD)
    #undef RES_ADD_MODULE
#ifdef MMI_SAVE_RUNING_NV_INFO
    #define RES_ADD_MODULE(module_id, module_path)         {module_id, module_path}, //none    
#endif
#elif (MACRO_RES_PRJ_TYPE == MACRO_RES_PRJ_DATA)
    #undef RES_ADD_DATA
    #define RES_ADD_DATA(DATA_ID,data_path)         DATA_ID,
#elif (MACRO_RES_PRJ_TYPE == 0)
    #error "Undefined MACRO_RES_PRJ_TYPE!"
#else
    #error "Unknown MACRO_RES_PRJ_TYPE!"
#endif
