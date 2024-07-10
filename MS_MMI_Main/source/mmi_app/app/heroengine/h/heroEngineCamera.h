#ifdef HERO_ENGINE_SUPPORT
#ifdef __HERO_CFG_CAMERA_SUPPORT__ 

#define DC_RIGHT_TRUNK(x, y)    (((x)+(y)-1)/(y)*(y))

typedef enum
{
	HERO_CAMERA_UNINIT,
	HERO_CAMERA_INITED,
	HERO_CAMERA_PREVIEW,
	HERO_CAMERA_CAPTURED,
	HERO_CAMERA_PAUSED
}hr_camera_state_enum;


typedef struct
{
	uint32 preview_x;
	uint32 preview_y;
	uint32 preview_w;
	uint32 preview_h;
	uint32 img_w;
	uint32 img_h;
	uint32 img_qt;
}hr_camera_launch_info_t;

typedef struct
{
	hr_camera_launch_info_t pre_req;
	int32 osd_layer;
	int32 preview_layer;
	int32 base_layer;
	int32 is_from_jsky;
	void *preview_layer_buffer;
	void *osd_layer_buffer;
	void *pImg;
	int32 ImgSize;
	int32 state;
	int32 last_error;
	int32 camera_handler; /*��������*/
	int32 continue_capture;
	uint16 zoom_max;
	uint16 zoom_steps;
	uint16 zoom_index;
	uint16 tmpName[32];
	uint8 *pTmp;
#if defined(__CAMERA_OSD_HORIZONTAL__) 
	int32 req_x;
    int32 req_y;
	int32 req_w;
	int32 req_h;
#endif	
}hr_camera_ctrl_t;


/**
 * \brief ��ʼ��CAMERA
 *
 * \return �ɹ�����HR_SUCCESS, ʧ�ܷ���HR_FAILED
 */
int32 hr_camera_init(int32 param);


/**
 * \brief ����CAMERA
 */
int32 hr_camera_create(void);


/**
 * \brief ����CAMERA
 */
int32 hr_camera_destroy(int32 param);


/**
 * \brief ��ͣCAMERA
 */
int32 hr_camera_pause(int32 param);


/**
 * \brief ��ȡ�������ű���
 *
 * \return the zoom max value
 */
int32 hr_camera_get_zoom_max(int32 param);


/**
 * \brief ��С
 *
 * \return �ɹ�����HR_SUCCESS, ʧ�ܷ���HR_FAILED
 */
int32 hr_camera_zoom_in(int32 param);


/**
 * \brief �Ŵ�
 *
 * \return �ɹ�����HR_SUCCESS, ʧ�ܷ���HR_FAILED
 */
int32 hr_camera_zoom_out(int32 param);


/**
 * \brief �ظ�CAMERA��չ
 *
 * \return �ɹ�����HR_SUCCESS, ʧ�ܷ���HR_FAILED
 */
int32 hr_camera_resume(int32 param);


/**
 * \brief ��ȡԤ������
 */
int32 hr_camera_get_preview_data(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);


/**
 * \brief ����
 */
int32 hr_camera_capture(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

    
#endif
#endif//HERO_ENGINE_SUPPORT
