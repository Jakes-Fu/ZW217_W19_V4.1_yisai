#ifndef __MMI_HERO_ENGINE_MOTION_H__
#define __MMI_HERO_ENGINE_MOTION_H__


#define HERO_MOTION_MAX_ACC     (150)
#define HERO_MOTION_FREQUENCY   (50)


typedef enum
{
	T_MOTION_PWROFF,
	T_MOTION_PWERON,
	T_MOTION_IDLE,
	T_MOTION_LISTENING
}T_MOTION_STATE;


typedef enum
{
	HR_MOTION_SENSITIVE_LOW,
	HR_MOTION_SENSITIVE_NORMAL,
	HR_MOTION_SENSITIVE_HIGH,
	HR_MOTION_SENSITIVE_MAX
} HR_MOTION_SENSITIVE_ENUM;


typedef enum
{
	HR_MOTION_EVENT_SHAKE,
	HR_MOTION_EVENT_TILT
} HR_MOTION_EVENT_ENUM;


typedef struct
{
	int32 x;
	int32 y;
	int32 z;
}T_MOTION_ACC;


/**
 * \brief motion ����
 *
 * \return �ɹ�����HR_SUCCESS, ʧ�ܷ���HR_FAILED
 */
int32 hr_motion_power_on(int32 param);


/**
 * \brief motion �ر�
 *
 * \return �ɹ�����HR_SUCCESS, ʧ�ܷ���HR_FAILED
 */
int32 hr_motion_power_off(int32 param);


/**
 * \brief ����shake
 */
int32 hr_motion_listen_shake(int32 param);


/**
 * \brief ����tilt
 */
int32 hr_motion_listen_tilt(int32 param);


/**
 * \brief ֹͣ����
 */
int32 hr_motion_stop_listen(int32 param);


/**
 * \brief ��ȡ max acc
 */
int32 hr_motion_get_max_acc(int32 param);


#endif
