#ifndef _OD_GAME_BILLING_CONFIG_H_
#define _OD_GAME_BILLING_CONFIG_H_
/*
   ����ļ�����һЩ�ƷѲ���,�����벻Ҫ�޸�������Ϣ
*/
//���Կ���������ر�
//only for debug version
//#define OD_GAME_DEBUG_SUPPORT
//���ղ��ԼƷѶ��ź���
#define OD_GAME_TEST_PHONE_NUMBER ""
//����
#define OD_GAME_MODEL_STRING "Spreadtrum Generic"
//ƽ̨
#define OD_GAME_OS_STRING "NAT"

#ifdef OD_DOODLE_JUMP_SUPPORT
#define OD_GAME_DOODLE_JUMP_PROJECT_ID (125)
#define OD_GAME_DOODLE_JUMP_CONTENT_NAME "DJMP"
#endif

#ifdef OD_CROSSY_ROAD_SUPPORT
#define OD_GAME_CROSSY_ROAD_PROJECT_ID (126)
#define OD_GAME_CROSSY_ROAD_CONTENT_NAME "CROA"
#endif

#ifdef OD_STICK_HERO_SUPPORT
#define OD_GAME_STICK_HERO_PROJECT_ID (127)
#define OD_GAME_STICK_HERO_CONTENT_NAME "SHER"
#endif

#ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
#define OD_GAME_BUBBLE_PARTY_MIX_PROJECT_ID (128)
#define OD_GAME_BUBBLE_PARTY_MIX_CONTENT_NAME "BPMI"
#endif        

#ifdef OD_SUPER_PET_SUPPORT
#define OD_GAME_SUPER_PET_PROJECT_ID (128)
#define OD_GAME_SUPER_PET_CONTENT_NAME "BPMI"
#endif        
#endif
