
#ifndef _MSENSOR_MEYE_YANGXUN_
#define _MSENSOR_MEYE_YANGXUN_

#include "sci_types.h"


typedef uint32		BOOL;

//����Ĭ�ϵ�������
#define MET_SENSITIVE_VALUE 1

//��������ͷȡ�����ڵĿ��
//��������ͷȡ�����ڵĸ߶�
#define MET_IMAGE_WIDTH 	128
#define MET_IMAGE_HEIGHT 	128




//����ħ�ۿ��ش򿪵ı�־
//����ħ�ۿ��عرյı�־
#define MET_CAMERA_CLOSE 0
#define MET_CAMERA_OPEN 1


// add by zhanglei ��������ʱ��Ĭ������
#define MET_CAMERA_CHBG_DEFAULT_SOUND 0 



//�����޲εĺ���ָ������
typedef void (*MET_CAM_FUNC_PARAM0)(void);

//����Ӧ�����͵�ö��
typedef enum _MET_CAMERA_TYPE_APP
{
	MET_CAMERA_TYPE_APP_SHAKE_MUSIC,						//ҡ���и�
	MET_CAMERA_TYPE_APP_SHAKE_BACKGROUD,					//ҡ���б���
    MET_CAMERA_TYPE_APP_SHAKE_ALARM,                        //ҡ�����徲��   
    MET_CAMERA_TYPE_APP_SHAKE_INCOMINGCALL,                 //ҡ�����羲��
	MET_CAMERA_TYPE_APP_MAX
}MET_CAMERA_TYPE_APP;


//����Ӧ�õĽṹ
typedef struct _MET_CAMERA_APP
{
	int32					selected;							//��ѡ���Ӧ��
	int32 					available;							//ѡ�����ĸ�Ӧ��
	MET_CAM_FUNC_PARAM0 	init;								//��ʼ���ĺ���ָ��                        
}MET_CAMERA_APP;

//��������ͷ�������û���������
typedef struct _MET_CAMERA_CONTEXT
{
	BOOL					is_osd_horizontal;							//����ͷ�Ƿ���
	uint16   				preview_width;								//ȡ����ͷ���ݵĿ��(���������н�ȡ�Ŀ��)
	uint16   				preview_height;								//ȡ����ͷ���ݵĸ߶�(���������н�ȡ�ĸ߶�)
	uint16					target_width;								//����ͷ��ȡ���ݵ�Ŀ����
	uint16 					target_height;								//����ͷ��ȡ���ݵ�Ŀ��߶�
	uint  					addr_intmem;								//����ͷ�����ڲ�ʹ�õĻ������׵�ַ(�����ڴ˴���ʾ�����ⲿָ���ڴ�λ��)
	uint8 *  				ptr_buffer;									//����ͷ�����ⲿʹ�õĻ������׵�ַ
	int32  					delay_time;									//��¼����ͷ�򿪵�׼��֮����ӳ���Ϣ
	BOOL  					cmotion_is_run;								//ħ���Ƿ���������
	int32					cmotion_flag;								//ħ�۵�ǰ���еı�־λ�����궨��
	MET_CAMERA_APP			apps[MET_CAMERA_TYPE_APP_MAX];				//ħ��Ӧ�õĽṹ������
	int32 last_capp;
	
}MET_CAMERA_CONTEXT;

//��������ͷϵͳ���ýṹ��
typedef struct _MET_CAMERA_SYSTEM
{
	int32 	eviron;					//�������ñ���(Ĭ��Ϊ2 ����ʾ��ͨ)
	int32 	open;					//�������ñ���(Ĭ��Ϊ1����ʾ����)	?
	int32  	sound_type;            //��������ʱ���ŵ�����(Ĭ��ֵΪ0)   	
	int32  	selected_eviron;		//��ѡ��Ļ������ñ���
	int32  	selected_open;			//��ѡ��Ŀ������ñ���
	int32  	selected_sound_type;    //��ѡ�����ʾ�����ñ���
	BOOL 	sleep_now;				//�Ƿ�Ϊ��Ļ����
	BOOL 	initialized;			//�Ƿ��ʼ����
	BOOL 	cut_initialized;		//��Ƭģ���Ƿ��ʼ����
	 
}MET_CAMERA_SYSTEM;

//�����ⲿ���ӵĺ���������
extern MET_CAMERA_CONTEXT met_camera_context;
extern MET_CAMERA_SYSTEM  met_camera_system;

extern void * met_cam_alloc_ext_mem(uint32 size);
extern void met_cam_free_ext_mem(void ** pointer);


#endif

//#endif


