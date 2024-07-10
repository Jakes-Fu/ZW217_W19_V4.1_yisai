#ifndef __MMIMRAPP_MOTION_H__
#define __MMIMRAPP_MOTION_H__


#define MOTION_MAX_ACC     (150)
#define MOTION_FREQUENCY   (50)


typedef enum
{
	T_MOTION_PWROFF,
	T_MOTION_PWERON,
	T_MOTION_IDLE,
	T_MOTION_LISTENING
}T_MOTION_STATE;


typedef enum
{
	MR_MOTION_SENSITIVE_LOW,
	MR_MOTION_SENSITIVE_NORMAL,
	MR_MOTION_SENSITIVE_HIGH,
	MR_MOTION_SENSITIVE_MAX
} MR_MOTION_SENSITIVE_ENUM;


typedef enum
{
	MR_MOTION_EVENT_SHAKE,
	MR_MOTION_EVENT_TILT
} MR_MOTION_EVENT_ENUM;


typedef struct
{
	int32 x;
	int32 y;
	int32 z;
}T_MOTION_ACC;


/**
 * \brief motion ����
 *
 * \return �ɹ�����MR_SUCCESS, ʧ�ܷ���MR_FAILED
 */
int32 mr_motion_power_on(int32 param);


/**
 * \brief motion �ر�
 *
 * \return �ɹ�����MR_SUCCESS, ʧ�ܷ���MR_FAILED
 */
int32 mr_motion_power_off(int32 param);


/**
 * \brief ����shake
 */
int32 mr_motion_listen_shake(int32 param);


/**
 * \brief ����tilt
 */
int32 mr_motion_listen_tilt(int32 param);


/**
 * \brief ֹͣ����
 */
int32 mr_motion_stop_listen(int32 param);


/**
 * \brief ��ȡ max acc
 */
int32 mr_motion_get_max_acc(int32 param);


#endif
