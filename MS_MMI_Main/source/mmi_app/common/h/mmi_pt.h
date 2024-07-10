/******************************************************************************
 ** File Name:      mmi_pt.h		                                          *
 ** Author:         panda luo                                                 *
 ** DATE:           01/28/2003                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    declare the interface for AT                               *
 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/28/2003     panda luo        Create.                                   *
 ******************************************************************************/
#ifndef _MMI_PT_H_
#define _MMI_PT_H_

//#include "mmi_struct.h"
#include "mmk_type.h"

#define MMIPT_MAX_CELL_NUM              20


#define MMIPT_MAX_LAC_NUM               40  //@steven cr7852 2004 03 12 , 40 lac
#define VALID_CELL		    0xFFFF


#define MMIPT_MAX_DIRECT_NUMBER_NUM     20
#define MMIPT_MAX_DIRECT_NUMBER_LEN     12

#define MMIPT_STRING_LENGTH             25

#define MMIPT_MAX_PIN_NUM               2//modify by figo.feng
#define MMIPT_MIN_PIN_LEN               4
#define MMIPT_MAX_PIN_LEN               8

#define MMIPT_MAX_MNS_NUM               4//modify by figo.feng
#define MMIPT_MAX_MNS_LEN               10

#define MMIPT_MAX_SVR_NUM               3//modify by figo.feng
#define MMIPT_MAX_SVR_LEN               10

#define MMIPT_MAX_CTRL_NUM              30//modify by figo.feng
#define MMIPT_MAX_CTRL_LEN              6

#define MMIPT_MAX_PROC_NUM              400//modify by figo.feng
#define MMIPT_MAX_PROC_LEN              7

#define MMIPT_MAX_IMSI_LEN              8
#define MMIPT_MAX_PSW_LEN               7
#define DEFAULT_PSW_VALUE          "123456"          

#define MMIPT_MAX_IP_LEN                8
#define MMIPT_MAX_IP_NUM                10

#define MMIPT_MAX_IPRELATION_NUM        30
#define MMIPT_MAX_IPRELATION_LEN        12

#define MMIPT_MAX_ROC_NUM        20      //the number of the restricted outing call
#define MMIPT_MAX_ROC_LEN        12     //the length of the restricted outingcall

#define MMIPT_MAX_RIC_NUM        20      //the number of the restricted incoming call
#define MMIPT_MAX_RIC_LEN        12     //the length of the restricted incoming call

#define MMIPT_MAX_LMN_NUM        400    //the number of the local mobile number 
#define MMIPT_MAX_LMN_LEN        12     //the length of the local mobile number 

#define MMI_KEY_PWD_LEN          4
#define MMI_KEY_PWD_NUM          3

#define MMIPT_DEFAULT_PIN          1
#define MMIPT_SPECIFY_PIN          2
//#define MMIPT_RANDOM_PIN         3
#define DEFAULT_PIN_VALUE          "1234"          
#define DEFAULT_PIN_VALUE_LEN      4   

#define MMIPT_MAX_FORBINUM_NUM     100
#define MMIPT_MAX_FORBINUM_LEN     6

#define MMIPT_MAX_LOCALNUM_NUM     500 //@steven  cr5761 2003 11 21
#define MMIPT_MAX_LOCALNUM_LEN     7

#define MMIPT_MAX_IPVALUE_LEN      6
#define MMIPT_MAX_USERLOCK_PSW_LEN 7
#define DEFAULT_USERLOCK_PSW_VALUE      "123456" 

// @Great.Tian, cr4484, 2003-09-05
#define MMIPT_MAX_LOCKKEYBOARD_PSW_LEN       5
#define DEFAULT_LOCKKEYBOARD_PSW_VALUE      "0000"
// End; Great.Tian, cr4484, 2003-09-05    
#define MMIPT_IP_LONCALL_PARAM_LEN       2 /*@jassmine MS00005931 2003-12-03*/     

#define MMIPT_RIGHT_MOVE_1         1
#define MMIPT_RIGHT_MOVE_2         2
#define MMIPT_RIGHT_MOVE_3         3
#define MMIPT_RIGHT_MOVE_4         4
#define MMIPT_RIGHT_MOVE_5         5
#define MMIPT_RIGHT_MOVE_6         6
#define MMIPT_RIGHT_MOVE_7         7
#define MMIPT_RIGHT_MOVE_8         8
#define MMIPT_RIGHT_MOVE_9         9
#define MMIPT_RIGHT_MOVE_10        10
#define MMIPT_RIGHT_MOVE_11        11

// @Great.Tian, cr4484, 2003-09-05
// �û������̵ı�ʾ
#define MMIPT_RIGHT_MOVE_12        12
#define MMIPT_RIGHT_MOVE_13        13
// End; Great.Tian, cr4484, 2003-09-05

// @ Roy MS00007523 
// �Ƿ�����������λ 
#define MMIPT_RIGHT_MOVE_14		   14
// end : Roy MS00007523


#define GET_FUNCFLAG_LOWEST_BIT                0x0001

// @Great.Tian, cr4484, 2003-09-05
#define SET_FUNCFLAG_ALLOW_LOCKKEYBOARD        0x2000
#define SET_FUNCFLAG_NOT_ALLOW_LOCKKEYBOARD    0xDFFF
#define SET_FUNCFLAG_LOCK_KEYBOARD             0x1000
#define SET_FUNCFLAG_NOT_LOCK_KEYBOARD         0xEFFF
// End; Great.Tian, cr4484, 2003-09-05

#define SET_FUNCFLAG_UNLOCK_PHONE_FLAG         0x0800
#define SET_FUNCFLAG_NOT_UNLOCK_PHONE_FLAG     0xF7FF
#define SET_FUNCFLAG_UNLOCK_SIM_FLAG           0x0400
#define SET_FUNCFLAG_NOT_UNLOCK_SIM_FLAG       0xFBFF
#define SET_FUNCFLAG_USERLOCK_DISTANCE         0x0200
#define SET_FUNCFLAG_NOT_USERLOCK_DISTANCE     0xFDFF
#define SET_FUNCFLAG_LOCK_DISTANCE             0x0100
#define SET_FUNCFLAG_NOT_LOCK_DISTANCE         0xFEFF
#define SET_FUNCFLAG_NUMBER_PROCESS            0x0080
#define SET_FUNCFLAG_NOT_NUMBER_PROCESS        0xFF7F
#define SET_FUNCFLAG_LOCK_NETWORK              0x0040
#define SET_FUNCFLAG_NOT_LOCK_NETWORK          0xFFBF
#define SET_FUNCFLAG_LOCK_LAC                  0x0020
#define SET_FUNCFLAG_NOT_LOCK_LAC              0xFFDF
#define SET_FUNCFLAG_LOCK_CELL                 0x0010
#define SET_FUNCFLAG_NOT_LOCK_CELL             0xFFEF
#define SET_FUNCFLAG_LOCK_PHONE                0x0008
#define SET_FUNCFLAG_NOT_LOCK_PHONE            0xFFF7
#define SET_FUNCFLAG_LOCK_SIM				   0x0004
#define SET_FUNCFLAG_NOT_LOCK_SIM			   0xFFFB
#define SET_FUNCFLAG_RANDOM_PIN				   0x0002
#define SET_FUNCFLAG_NOT_RANDOM_PIN            0xFFFD
#define SET_FUNCFLAG_SHOW_DIALNUM			   0x0001
#define SET_FUNCFLAG_SHOW_PROCNUM			   0xFFFE

// @ Roy MS00007523
#define SET_FUNCFLAG_Clear_LOCK_SIM_FLAG           0x4000
#define SET_FUNCFLAG_NOT_Clear_LOCK_SIM_FLAG           0xBFFF
// end : Roy MS00007523

#define CTRL_VALUE_THIBIT              0x20
#define CTRL_VALUE_FOUBIT              0x10

#define AREA_CODE_FLAG                 1
#define AREA_CODE_ID				   1

#define TELE_TEMP_NUMBER_LEN		   50//@figo.feng 2003.10.09 cr_5091

#define   BCD_F                    0xF


#define TRANSFORM_ID(_id) \
        if((_id)<=0)\
		{\
			return MMIPT_ERR_PARAM;\
		}\
        _id--


typedef enum 
{
    MMIPT_ERR_SUCCESS,
    MMIPT_ERR_FAILURE,
    MMIPT_ERR_PARAM,        //The param is error
    MMIPT_ERR_WRITENV,      //WriteNV error.
    MMIPT_ERR_READNV,
	MMIPT_ERR_SAME,
	MMIPT_ERR_LENGTH,//add by figo.feng
    MMIPT_ERR_NOSETIP,/*@jassmine MS00005930 2003-12-03*/
    MMIPT_ERR_NONUM,/*@jassmine MS00006936 2004-01-17*/
    MMIPT_ERR_LISTISFULL,
    MMIPT_ERR_LISTISEMPTY
}MMIPT_RESULT_E;

//add by figo.feng
typedef enum 
{
    MMIPT_LOCKPHONE_SUCCESS,
    MMIPT_LOCKPHONE_CONTINUE_STARTUP,
    MMIPT_UNMATCHED_IMSI       
}MMIPT_LOCKPHONE_RESULT_E;

typedef enum 
{
    MMIPT_USER_INPUT,
    //MMIPT_HAVELOCKEDSIM_UNLOCK,
    MMIPT_VERIFY_PIN       
}MMIPT_HAVELOCKEDSIM_RESULT_E;

typedef enum 
{
    MMIPT_NOT_UNLOCK,
    MMIPT_UNLOCK_PHONE,
    MMIPT_UNLOCK_SIM       
}MMIPT_UNLOCK_TYPE_E;

typedef enum 
{
	MMIPT_ERROR,
	MMIPT_DIAL_FORBIDDEN,
    MMIPT_SHOW_DIALED_NUM,
    MMIPT_SHOW_PROCESSED_NUM
	//MMIPT_DIAL       
}MMIPT_PROCESS_RESULT_E;

typedef enum 
{
    MMIPT_NO_LOCK,
    MMIPT_LOCK_PHONE,
    MMIPT_LOCK_SIM_SPECIFY,
    MMIPT_LOCK_SIM_RANDOM,
    MMIPT_LOCK_BOTH_SPECIFY,
    MMIPT_LOCK_BOTH_RANDOM
}MMIPT_LOCK_PHONE_SIM_E;

typedef enum 
{
    MMIPT_LOCK_CELLID,
    MMIPT_LOCK_LAC,
    MMIPT_LOCK_SIMCARD,
    MMIPT_LOCK_OPERATER,
    MMIPT_LOCK_IP,
    MMIPT_LOCK_IVR,
    MMIPT_LOCK_ADD_ZERO_BEFORE_NUMBER
}MMIPT_LOCK_TYPE_E;


typedef  enum
{
    MMIPT_ROC_FREE ,//= 0,
    MMIPT_ROC_ALL_RESTRICTED,// = 1,
    MMIPT_ROC_TABLE_RESTRICTED ,//= 2 ,
    MMIPT_ROC_TABLE_ALLOW_ONLY ,//= 4,
    MMIPT_ROC_INTER_FREE ,//= 8,
    MMIPT_ROC_INTER_RESTRICTED ,//=16,
    MMIPT_ROC_MAX = MMIPT_ROC_INTER_RESTRICTED
}MMIPT_ROC_MODE_E;

typedef  enum
{
    MMIPT_RIC_FREE,// = 0,
    MMIPT_RIC_ALL_RESTRICTED,// =1,
    MMIPT_RIC_TABLE_RESTRICTED,// = 2,
    MMIPT_RIC_TABLE_ALLOW_ONLY,// =4,
    MMIPT_RIC_INTER_FREE ,//=8,
    MMIPT_RIC_INTER_RESTRICTED,// =16,
    MMIPT_RIC_MAX = MMIPT_RIC_INTER_RESTRICTED
}MMIPT_RIC_MODE_E;

/*@jassmine MS00005930 2003-12-03*/
typedef enum
{
    MMIPT_IPVALUE_NOACT,
    MMIPT_IPVALUE_DAILZERO_DELZERO,
    MMIPT_IPVALUE_DELZERO,
    MMIPT_IPVALUE_ADDZERO
}MMIPT_IPVALUE_ZERO_E;

typedef struct
{
    uint32 length;
    uint8   value[MMIPT_STRING_LENGTH];
}MMIPT_STRING_T;


typedef struct
{
    uint16  mcc;
    uint16  mnc;
}MMIPT_OPERATOR_T;

typedef struct
{
    uint16  ip_id;          //the telephone number corresponding IP ID
    uint16  tele_len;       //the length of telephone number 
    uint8   tele_value[MMIPT_STRING_LENGTH];//the value of telephone number 
}MMIPT_RELATION_TELENUMER_T;

/*@jassmine MS00005930 2003-12-03*/
typedef struct
{
    BOOLEAN  ip_zero_turn_on;
    uint8    ip_zero_value;   
}MMIPT_IPVALUE_ZERO_T;
/*@End jassmine MS00005930 2003-12-03*/

/*@jassmine MS00005931 2003-12-03*/
typedef struct
{
    BOOLEAN  ip_internal;//���ڳ�;�Ƿ��Զ���IP
    BOOLEAN  ip_interntional;//���ʳ�;�Ƿ��Զ���IP
    BOOLEAN  ip_diffplace;//��������ֻ��Ƿ��IP
}MMINV_IP_LONGCALL_T;
/*@End jassmine MS00005931 2003-12-03*/

MMIPT_RESULT_E Str2Bcd(
                        MMIPT_STRING_T string,  //the value of the param
                        char* bcd_value         //the value of the param
                        ) ;


//����FUNC_FLAG
/*****************************************************************************/
// 	Description : set func_flag
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetFuncFlag(uint16 func_flag);

//�õ�FUNC_FLAG
/*****************************************************************************/
// 	Description : get func_flag
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetFuncFlag(uint16 *func_flag);

//���ý�������־
/*****************************************************************************/
// 	Description : set unlock phone flag
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetUnlockPhoneFlag(BOOLEAN unlock);

//�õ���������־
/*****************************************************************************/
// 	Description : get unlock phone flag
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetUnlockPhoneFlag(BOOLEAN *unlock);

//���ý�������־
/*****************************************************************************/
// 	Description : set unlock sim flag
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetUnlockSimFlag(BOOLEAN unlock);

//�õ���������־
/*****************************************************************************/
// 	Description : get unlock sim flag
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetUnlockSimFlag(BOOLEAN *unlock);

//�����û�����;
/*****************************************************************************/
// 	Description : set locking long distance by user
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetUserLongDistanceLock(BOOLEAN lock);

//�õ��Ƿ��û�����;
/*****************************************************************************/
// 	Description : get locking long distance by user
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetUserLongDistanceLock(BOOLEAN *lock);

//�¼ӣ��õ��Ƿ��û�����; 
/*****************************************************************************/
// 	Description : get locking long distance by user (new)
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
BOOLEAN MMIPT_GetUserLDLock(void);

// @Great.Tian, cr4484, 2003-09-05
//�����Ƿ������û�����
/*****************************************************************************/
// 	Description : set allow locking keyboard by user
//	Global resource dependence : 
//  Author:	Great.Tian
//	Note: 
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetAllowKeyboardLock(
                                 BOOLEAN lock
                                 );


//�õ��Ƿ������û����� 
/*****************************************************************************/
// 	Description : get allow locking keyboard by user (new)
//	Global resource dependence : 
//  Author:	Great.Tian
//	Note: 
/*****************************************************************************/
BOOLEAN MMIPT_GetAllowKeyboardLock(void);

//�����û�����
/*****************************************************************************/
// 	Description : set locking keyboard by user
//	Global resource dependence : 
//  Author:	Great.Tian
//	Note: 
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetKeyboardLock(
                                 BOOLEAN lock
                                 );

//�õ��Ƿ��û����� 
/*****************************************************************************/
// 	Description : get locking keyboard by user (new)
//	Global resource dependence : 
//  Author:	Great.Tian
//	Note: 
/*****************************************************************************/
BOOLEAN MMIPT_GetKeyboardLock(void);

//�����û���������
/*****************************************************************************/
// 	Description : set password of user locking long distance
//	Global resource dependence : 
//  Author:	Great.Tian
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetLockKeyboardPsw(MMIPT_STRING_T lockkeyboard_value);

//�õ��û���������
/*****************************************************************************/
// 	Description : get password of user locking long distance
//	Global resource dependence : 
//  Author:	Great.Tian
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetLockKeyboardPsw(MMIPT_STRING_T *lockkeyboard_value);

// End; Great.Tian, cr4484, 2003-09-05

//��������;
/*****************************************************************************/
// 	Description : set locking long distance 
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetLongDistanceLock(BOOLEAN lock);

//�õ��Ƿ�����;
/*****************************************************************************/
// 	Description : get locking long distance 
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetLongDistanceLock(BOOLEAN *lock);

//�����Ƿ���Ҫ���봦��
/*****************************************************************************/
// 	Description : set if phone number need process
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetIfNumberProcess(BOOLEAN lock);

//�õ��Ƿ���Ҫ���봦��
/*****************************************************************************/
// 	Description : get if phone number need process
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetIfNumberProcess(BOOLEAN *lock);

//��������
/*****************************************************************************/
// 	Description : set lock operator
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetOperatorLock(BOOLEAN lock);

//�õ�����
/*****************************************************************************/
// 	Description : get lock operator
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetOperatorLock(BOOLEAN *lock);

//������LAC
/*****************************************************************************/
// 	Description : set lock lac
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetLacLock(BOOLEAN lock);

//�õ��Ƿ���LAC
/*****************************************************************************/
// 	Description : get lock lac
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetLacLock(BOOLEAN *lock);

//������С��
/*****************************************************************************/
// 	Description : set lock cell
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetCellLock(BOOLEAN lock);

//�õ��Ƿ���С��
/*****************************************************************************/
// 	Description : get lock cell
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetCellLock(BOOLEAN *lock);

//��������
/*****************************************************************************/
// 	Description : set lock phone
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetPhoneLock(BOOLEAN lock);

//�õ��Ƿ�����
/*****************************************************************************/
// 	Description : get lock phone
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetPhoneLock(BOOLEAN *lock);

//������SIM CARD
/*****************************************************************************/
// 	Description : set lock sim
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetSimCardLock(BOOLEAN lock);

//�õ��Ƿ���SIM CARD
/*****************************************************************************/
// 	Description : get lock sim
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetSimCardLock(BOOLEAN *lock);

//��������ʱPIN�Ƿ����
/*****************************************************************************/
// 	Description : set pin type when lock sim
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetPinType(BOOLEAN lock);

//�õ�����ʱPIN�Ƿ����
/*****************************************************************************/
// 	Description : get pin type when lock sim
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetPinType(BOOLEAN *lock);

//������������������Ϣ��������־λ��NV�е�IMSI�Լ�NV��Sim�е�PIN��ֵ
/*****************************************************************************/
// 	Description : clear lock of phone and sim 
//	Global resource dependence : 
//  Author:	Jassmine.Meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_ClearLockPhoneAndSim(void);

//���ò�����ʾ��1Ϊ��ʾ�û��������룬0Ϊ��ʾ�����ĺ���
/*****************************************************************************/
// 	Description : set the method of show number
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetDialDisp(BOOLEAN lock);

//�õ�������ʾ��1Ϊ��ʾ�û��������룬0Ϊ��ʾ�����ĺ���
/*****************************************************************************/
// 	Description : get the method of show number
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetDialDisp(BOOLEAN *lock);


//����PIN
/*****************************************************************************/
// 	Description : set the pin
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetFixedPin(
                                 uint16 pin_id,
                                 MMIPT_STRING_T pin_value
                                 ) ;

//�õ�PIN
/*****************************************************************************/
// 	Description : get the pin
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetFixedPin(
                                 uint16 pin_id,
                                 MMIPT_STRING_T *pin_value
                                 ) ;


//����IMSI
/*****************************************************************************/
// 	Description : set the imsi
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetIMSI(MMIPT_STRING_T imsi_value);

//�õ�IMSI
/*****************************************************************************/
// 	Description : get the imsi
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetIMSI(MMIPT_STRING_T *imsi_value);

//����MCC��MNC
/*****************************************************************************/
// 	Description : set mcc and mnc
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetOperator(
                                uint16  mcc,
                                uint16  mnc
                                //MMIPT_OPERATOR_T operator_value
                                ) ;

//�õ�MCC��MNC
/*****************************************************************************/
// 	Description : get mcc and mnc
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetOperator(
                                uint16  *mcc,
                                uint16  *mnc
                                 ) ;


//���ÿ���LAC����
/*****************************************************************************/
// 	Description : set number of lac
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetLacNum(uint16 lac_num);

//�õ�����LAC����
/*****************************************************************************/
// 	Description : get number of lac
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetLacNum(uint16 *lac_num);

//���ÿ���CELL ID����
/*****************************************************************************/
// 	Description : set number of cell
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetCellNum(uint16 cell_num);

//�õ�����CELL ID����
/*****************************************************************************/
// 	Description : get number of cell
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetCellNum(uint16* cell_num) ;

//���ð�����Ҫ������
/*****************************************************************************/
// 	Description : set password of cryptogram
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetPsw(MMIPT_STRING_T psw_value);

//�õ�������Ҫ������
/*****************************************************************************/
// 	Description : get password of cryptogram
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetPsw(MMIPT_STRING_T *psw_value);

//����LAC��ֵ
/*****************************************************************************/
// 	Description : set value of lac
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetLacValue(
                                 uint16 lac_id,
                                 uint16 lac_value
                                ) ;

//�õ�LAC��ֵ
/*****************************************************************************/
// 	Description : get value of lac
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetLacValue(
                                 uint16 lac_id,
								 uint16 *lac_value
                                ) ;

//����CELL ID��ֵ
/*****************************************************************************/
// 	Description : set value of cell
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetCellValue(
                                  uint16 cell_id,
                                  uint16 cell_value
                                  ) ;

//�õ�CELL ID��ֵ
/*****************************************************************************/
// 	Description : get value of cell
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetCellValue(
                              uint16 cell_id,
                              uint16 *cell_value
                              ) ;

//���ý��������
/*****************************************************************************/
// 	Description : set forbidden number
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetForbiddenNumber(
								  uint16          forbidden_id,
                                  uint16          forbidden_value_num,
								  MMIPT_STRING_T *forbidden_value
								  );

//�õ����������
/*****************************************************************************/
// 	Description : get forbidden number
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetForbiddenNumber(
								  uint16 forbidden_id,
								  MMIPT_STRING_T *forbidden_value
								  );

//���ñ��غ����
/*****************************************************************************/
// 	Description : set local number
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetLocalNumber(
								  uint16          local_id,
                                  uint16          local_value_num,
								  MMIPT_STRING_T *local_value
								  );
//�õ����غ����
/*****************************************************************************/
// 	Description : get local number
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetLocalNumber(
								  uint16 local_id,
								  MMIPT_STRING_T *local_value
								  );
/*@jassmine MS00006667 2004-01-05*/
//ɾ�����غ�����еĹ̶��绰����
/*****************************************************************************/
// 	Description : delete local number
//	Global resource dependence : 
//  Author:	Jassmine.Meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_DelLocalNumber(
                                    uint16          local_special_num,
                                    MMIPT_STRING_T *local_value
                                    );

//���ӱ��غ�����еĹ̶��绰����
/*****************************************************************************/
// 	Description : add local number
//	Global resource dependence : 
//  Author:	Jassmine.Meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_AddLocalNumber(
                                    uint16          local_special_num,
                                    MMIPT_STRING_T *local_value
                                    );
//ɾ������������еĹ̶��绰����
/*****************************************************************************/
// 	Description : delete forbidden number
//	Global resource dependence : 
//  Author:	Jassmine.Meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_DelForbiddenNumber(
                                        uint16          forbidden_value_num,
                                        MMIPT_STRING_T *forbidden_value
                                        );

//���ӽ���������еĹ̶��绰����
/*****************************************************************************/
// 	Description : add forbidden number
//	Global resource dependence : 
//  Author:	Jassmine.Meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_AddForbiddenNumber(
                                        uint16          forbidden_value_num,
                                        MMIPT_STRING_T *forbidden_value
                                        );
/*@jassmine MS00006667 2004-01-05*/
//����Ԥ��IP��
/*****************************************************************************/
// 	Description : set IP 
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetIP(MMIPT_STRING_T ip_value);

//�õ�Ԥ��IP��
/*****************************************************************************/
// 	Description : get IP
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetIP(MMIPT_STRING_T *ip_value);

/*@jassmine MS00005930 2003-12-03*/
//�Ƿ��Ѿ�����IP��
/*****************************************************************************/
// 	Description : whether already set IP
//	Global resource dependence : 
//  Author:	jassmine.meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_IfSetIP(void);

//�Զ���IP������£��Զ�ʵ�ֳ�0��0����
/*****************************************************************************/
// 	Description : please refer cr5930
//	Global resource dependence : 
//  Author:	jassmine.meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetIPAboutZero(uint8 esp_ip);

//���Զ���IP���������£��õ��Ƿ��Զ�ʵ�ֳ�0��0����
/*****************************************************************************/
// 	Description : get if eat and add 0 
//	Global resource dependence : 
//  Author:	jassmine.meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetIPAboutZero(MMIPT_IPVALUE_ZERO_T *ip_value_zero);
/*@End jassmine MS00005930 2003-12-03*/

/*@jassmine MS00005931 2003-12-03*/
//���Զ���IP���������£����ù��ڳ�;�����ʳ�;����������Ƿ��IP
/*****************************************************************************/
// 	Description : set or not set IP with international call,
//                    internal call and internal different place call 
//	Global resource dependence : 
//  Author:	jassmine.meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetIPAboutLongCall(MMIPT_STRING_T *ip_longcall_value);

//���Զ���IP���������£��õ����ڳ�;�����ʳ�;�͹�������ֻ���IP���ÿ���
/*****************************************************************************/
// 	Description : get if set IP with international call,
//                    internal call and internal different place call 
//	Global resource dependence : 
//  Author:	jassmine.meng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetIPAboutLongCall(MMINV_IP_LONGCALL_T *ip_longcall_value);

//������������
/*****************************************************************************/
// 	Description : clear forbidden number
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_ClearForbiddenNumber(void);

//������غ����
/*****************************************************************************/
// 	Description : clear local number
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_ClearLocalNumber(void);

//���IP
/*****************************************************************************/
// 	Description : clear IP
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_ClearIP(void);

//�����û�����;����
/*****************************************************************************/
// 	Description : set password of user locking long distance
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetUserLockPsw(MMIPT_STRING_T userlock_value);

//�õ��û�����;����
/*****************************************************************************/
// 	Description : get password of user locking long distance
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetUserLockPsw(MMIPT_STRING_T *userlock_value);

//����������
/*****************************************************************************/
// 	Description : set lock type
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetLockType(MMIPT_LOCK_PHONE_SIM_E lock_type);

//�õ�������
/*****************************************************************************/
// 	Description : get lock type
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetLockType(MMIPT_LOCK_PHONE_SIM_E* lock_type);

//�õ���������
/*****************************************************************************/
// 	Description : get unlock type
//	Global resource dependence : 
//  Author:	figo.feng
//	Note: 
/*****************************************************************************/
//MMIPT_UNLOCK_TYPE_E MMIPT_GetUnLockType(void);

//will delete

//���õ绰��������,�绰�����Һ�,������������
MMIPT_RESULT_E MMIPT_SetMeetingSvrNum(
									  uint16 svr_id,                            
									  MMIPT_STRING_T  svr_value
									  );


//�õ��绰��������,�绰�����Һ�,������������
MMIPT_RESULT_E MMIPT_GetMeetingSvrNum(
									  uint16 svr_id,
									  MMIPT_STRING_T  *svr_value
									  );

//�������ܺ�
MMIPT_RESULT_E MMIPT_SetMNS(
                            uint16 mns_id,                            
							MMIPT_STRING_T  mns_value
                            );

//�õ����ܺ�
MMIPT_RESULT_E MMIPT_GetMNS(
                            uint16 mns_id,
                            MMIPT_STRING_T  *mns_value
                            );

//����λ�ñ����������Ϣ
MMIPT_RESULT_E MMIPT_SetRepFlag(uint8  rep_flag);

//�õ�λ�ñ����������Ϣ
MMIPT_RESULT_E MMIPT_GetRepFlag(uint8  *rep_flag);

//���ö�ʱ�����ʱ����
MMIPT_RESULT_E MMIPT_SetRepTime(uint8  rep_time);

//�õ���ʱ�����ʱ����
MMIPT_RESULT_E MMIPT_GetRepTime(uint8  *rep_time);

//���ÿͻ�����
MMIPT_RESULT_E MMIPT_SetClientNum(uint8 client_num);

//�õ��ͻ�����
MMIPT_RESULT_E MMIPT_GetClientNum(uint8 *client_num);

//����ҵ�����
MMIPT_RESULT_E MMIPT_SetPrdNum(uint8 prd_num);

//�õ�ҵ�����
MMIPT_RESULT_E MMIPT_GetPrdNum(uint8 *prd_num);

//���ÿ��ƴ����
MMIPT_RESULT_E MMIPT_SetCtrlTable(
								  MMI_CTRL_ID_T ctrl_id,
								  MMIPT_STRING_T ctrl_value
								  );

//�õ����ƴ����
MMIPT_RESULT_E MMIPT_GetCtrlTable(
								  MMI_CTRL_ID_T ctrl_id,
								  MMIPT_STRING_T *ctrl_value
								  );

//���ú��봦���
MMIPT_RESULT_E MMIPT_SetNumProcTable(
								  uint16 proc_id,
								  MMIPT_STRING_T proc_value
								  );

//�õ����봦���
MMIPT_RESULT_E MMIPT_GetNumProcTable(
								  uint16 proc_id,
								  MMIPT_STRING_T *proc_value
								  );

/*****************************************************************************/
// 	Description : Set the status of the lock of the type
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetLock(MMIPT_LOCK_TYPE_E type,BOOLEAN lock) ;

/*****************************************************************************/
// 	Description : set the telephone number that can be directly dialed.
//	Global resource dependence : none
//  Author: panda.luo
//	Note: top layer turn in string format,ex:0x31 is express '0x1'
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetDirectPhoneValue(
                                         uint16 phone_id,
                                         MMIPT_STRING_T phone_value
                                         ) ;

/*****************************************************************************/
// 	Description : Get the telephone that can be directly dialed.
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetDirectPhoneValue(
                                         uint16 phone_id,
                                         MMIPT_STRING_T *phone_num
                                         ) ;

MMIPT_RESULT_E MMIPT_SetIPSwitch(
                                 BOOLEAN turn_on
                                ) ;

MMIPT_RESULT_E MMIPT_GetIPSwitch(
                                 BOOLEAN *turn_on
                                ) ;

/*****************************************************************************/
// 	Description : Set the IP telephone number
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/
// Set IP's value

MMIPT_RESULT_E MMIPT_SetIPValue(
                                uint16 ip_id,
                                MMIPT_STRING_T ip_value
                                ) ;

/*****************************************************************************/
// 	Description : Get the IP telephone number
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetIPValue(
                                uint16 ip_id,
                                MMIPT_STRING_T *ip_value
                                ) ;

/*****************************************************************************/
// 	Description : Set the number type and its coresponding IP number
//	Global resource dependence : none
//  Author:panda
//	Note:the data saved to NV:
//         the first byte: ip number 
//         the second byte -- the last byte: number type in BCD format
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetIPRelation(
                                   uint16 record_id,
                                   MMIPT_RELATION_TELENUMER_T relation
                                   ) ;

/*****************************************************************************/
// 	Description : Get the number type and its coresponding IP number
//	Global resource dependence : none
//  Author:panda
//	Note:the data saved to NV:
//         the first byte: ip num 
//         the second byte -- the last byte: num type in BCD format
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetIPRelation(
                                   uint16 record_id,
                                   MMIPT_RELATION_TELENUMER_T *relation
                                   ) ;

/*****************************************************************************/
// 	Description : Set the telephone of the network center
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetNetworkCenter(
                                MMIPT_STRING_T NetworkCenter_number
                                ) ;

/*****************************************************************************/
// 	Description : Get the telephone of the network center
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetNetworkCenter(
                                    MMIPT_STRING_T *NetworkCenter_number
                                    ) ;

/*****************************************************************************/
// 	Description : Set the mode of the restricted outing call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetROCMode(
                                MMIPT_ROC_MODE_E mode
                                ) ;

/*****************************************************************************/
// 	Description : Get the mode of the restricted outing call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetROCMode(
                                MMIPT_ROC_MODE_E  *mode
                                ) ;

/*****************************************************************************/
// 	Description : Set the telephone number of the restricted outing call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetROCNumber(
                                  uint16 roc_id,
                                  MMIPT_STRING_T call_num
                                  ) ;

/*****************************************************************************/
// 	Description : Get the telephone number of the restricted outing call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetROCNumber(
                                  uint16    roc_id,
                                  MMIPT_STRING_T *call_num
                                  ) ;

/*****************************************************************************/
// 	Description : Set the mode of the restricted incoming call
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetRICMode(
                                MMIPT_RIC_MODE_E mode
                                ) ;

/*****************************************************************************/
// 	Description : Get the mode of the restricted incoming call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetRICMode(
                                 MMIPT_RIC_MODE_E *mode
                                 ) ;

/*****************************************************************************/
// 	Description : Set the telephone number of the restricted incoming call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_SetRICNumber(
                                  uint16 ric_id,
                                  MMIPT_STRING_T call_num
                                  ) ;

/*****************************************************************************/
// 	Description : Get the telephone number of the restricted incoming call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetRICNumber(
                                  uint16 ric_id,
                                  MMIPT_STRING_T *call_num
                                  ) ;

MMIPT_RESULT_E MMIPT_SetIVRSwitch(BOOLEAN turn_on) ;
MMIPT_RESULT_E MMIPT_GetIVRSwitch(BOOLEAN *turn_on) ;

/*****************************************************************************/
// 	Description : Set the IVR center number
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetIVRCenterNumber(MMIPT_STRING_T call_num) ;
/*****************************************************************************/
// 	Description : Get the IVR center number
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetIVRCenterNumber(MMIPT_STRING_T call_num) ;
/*****************************************************************************/
// 	Description : Set the IVR number
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetIVRNumber(uint16 ivr_id,MMIPT_STRING_T call_num) ;
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_GetIVRNumber(uint16 ivr_id,MMIPT_STRING_T *ivr_number) ;

/*****************************************************************************/
// 	Description : Set the IVR interval time
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/
MMIPT_RESULT_E MMIPT_SetIVRInterval(uint16 ivr_id,uint16 second) ;
/*****************************************************************************/
// 	Description : get IVR interval
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetIVRInterval(
                            uint16 ivr_id,
                            uint16 *ivr_interval
                            ) ;

/*****************************************************************************/
// 	Description : Set the local mobile number
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/
/*
MMIPT_RESULT_E MMIPT_SetLocalMNumber(
                                     uint16 number_id,
                                     MMIPT_STRING_T call_num
                                     ) ;
*/
/*****************************************************************************/
// 	Description : Read local mobile number 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_GetLocalMNumber(
                                     uint16 number_id,
                                     MMIPT_STRING_T *call_num
                                     ) ;

/*****************************************************************************/
// 	Description : clear the telephone number that can be directly dialed.
//	Global resource dependence : none
//  Author: panda.luo
//	Note: top layer turn in string format,ex:0x31 is express '0x1'
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_ClearDirectPhoneValue(
                                         uint16 phone_id
                                         );

/*****************************************************************************/
// 	Description : Set the telephone number of the restricted outing call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_ClearROCNumber(
                                  uint16 roc_id
                                  );

/*****************************************************************************/
// 	Description : Clear the number type and its coresponding IP number
//	Global resource dependence : none
//  Author:panda
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_ClearIPRelation(
                                   uint16 record_id
                                   );

/*****************************************************************************/
// 	Description : Set the telephone number of the restricted outing call 
//	Global resource dependence : none
//  Author:panda
//	Note:
/*****************************************************************************/

MMIPT_RESULT_E MMIPT_ClearRICNumber(
                                  uint16 ric_id
                                  );

MMIPT_RESULT_E MMIPT_SetAddZeroSwitch(BOOLEAN turn_on) ;
MMIPT_RESULT_E MMIPT_GetAddZeroSwitch(BOOLEAN *turn_on) ;
/*****************************************************************************/
// 	Description : ����Cellʱ��ǿ�ƻ���������С��(Tcl����)
//	Global resource dependence : none
//  Author:alexye
//	Note:prd490
/*****************************************************************************/
BOOLEAN MMI_IsValidCell(
                       uint16  cell_id
                       );

/*****Local struct*****/
typedef struct
{
    //BOOLEAN operator_locked_need;   //Need to lock operator?
    //BOOLEAN lac_locked_need;        //Need to lock lac?
    //BOOLEAN cell_locked_need;       //Need to lock cell?
    //BOOLEAN operator_locked;        //operator is already locked?
    //BOOLEAN lac_locked;             //lac is already locked?
    //BOOLEAN cell_locked;            //cell is already locked?

    uint16 mcc;
    uint16 mnc;
    //uint16 lac;
    uint16 max_cell_num;
    //uint16 valid_cell_num;
    uint16 cell_id[MMIPT_MAX_CELL_NUM];
	uint16 max_lac_num;
	uint16 lac_id[MMIPT_MAX_LAC_NUM];
}MMIPT_CELL_T;

typedef struct
{
    //BOOLEAN sim_locked_need;
    //@steven  cr5459   2003 11 06
	//char pin1[MMIPT_MAX_PIN_NUM][MMIPT_MAX_PIN_LEN];   //the initial pin1 of the sim.
	char pin1[MMIPT_MAX_PIN_NUM][MMIPT_MAX_PIN_LEN/2];   //the initial pin1 of the sim
}MMIPT_SIM_T;

typedef struct
{
    char imsi[MMIPT_MAX_IMSI_LEN];   
}MMIPT_IMSI_T;

typedef struct
{
    char psw[MMIPT_MAX_PSW_LEN];   
}MMIPT_PSW_T;

typedef struct
{
    char forbi_num[MMIPT_MAX_FORBINUM_NUM][MMIPT_MAX_FORBINUM_LEN];   
}MMIPT_FORBINUM_T;

typedef struct
{
    char local_num[MMIPT_MAX_LOCALNUM_NUM][MMIPT_MAX_LOCALNUM_LEN];   
}MMIPT_LOCALNUM_T;

typedef struct
{
    BOOLEAN ip_turn_on;/*@jassmine MS00005930 2003-12-03*/
    char    ip[MMIPT_MAX_IPVALUE_LEN];   
}MMIPT_IPVALUE_T;

typedef struct
{
    char userlock_psw[MMIPT_MAX_USERLOCK_PSW_LEN];   
}MMIPT_USERLOCK_PSW_T;

// @Great.Tian, cr4484, 2003-09-05
typedef struct
{
    char userlock_psw[MMIPT_MAX_LOCKKEYBOARD_PSW_LEN];   
}MMIPT_LOCKKEYBOARD_PSW_T;
// End; Great.Tian, cr4484, 2003-09-05

// will delete
typedef struct
{
    char tele_num[MMIPT_MAX_DIRECT_NUMBER_NUM][MMIPT_MAX_DIRECT_NUMBER_LEN/2];   //the initial pin1 of the sim.
}MMIPT_DIRECT_NUMBER_T;

typedef struct
{
    BOOLEAN ip_turn_on;
    char tele_num[MMIPT_MAX_IP_NUM][MMIPT_MAX_IP_LEN/2];   //the initial pin1 of the sim.
}MMIPT_IP_INFO_T;

typedef struct
{

    uint16  ip_id;          //the telephone number corresponding IP ID
    uint8   tele_value[MMIPT_MAX_IPRELATION_LEN/2];//the value of telephone number 
}MMIPT_IP_NUM_RELATION_T;

typedef struct 
{ 
    MMIPT_IP_NUM_RELATION_T in_rleation[MMIPT_MAX_IPRELATION_NUM];
}MMIPT_IP_NUM_RELATION_ARR_T;

typedef struct
{
    MMIPT_ROC_MODE_E mode;          //the telephone number corresponding IP ID
    char tele_num[MMIPT_MAX_ROC_NUM][MMIPT_MAX_ROC_LEN];
}MMIPT_ROC_INFO_T;

typedef struct
{
    MMIPT_RIC_MODE_E mode;          //the telephone number corresponding IP ID
    char tele_num[MMIPT_MAX_RIC_NUM][MMIPT_MAX_RIC_LEN];
}MMIPT_RIC_INFO_T;

typedef struct
{
    uint16  valid_num;
    BOOLEAN add_zero_before_tele;
    char tele_num[MMIPT_MAX_LMN_NUM][MMIPT_MAX_LMN_LEN/2];
}MMIPT_LOCAL_NUMBER_T;


typedef struct
{
    uint8 key[MMI_KEY_PWD_NUM][MMI_KEY_PWD_LEN]; 
}MMIPT_KEY_PWD_T;

typedef struct
{
    char mns_num[MMIPT_MAX_MNS_NUM][MMIPT_MAX_MNS_LEN];   //the initial MNS number.

}MMIPT_MNSNUM_T;

typedef struct
{
    char svr_num[MMIPT_MAX_SVR_NUM][MMIPT_MAX_SVR_LEN];   //the initial serve number.

}MMIPT_SVRNUM_T;

typedef struct
{
    char ctrl[MMIPT_MAX_CTRL_NUM][MMIPT_MAX_CTRL_LEN];   //the initial control table
}MMIPT_CTRLTAB_T;

typedef struct
{
    char proc[MMIPT_MAX_PROC_NUM][MMIPT_MAX_PROC_LEN];   //the initial process table 
}MMIPT_PROCTAB_T;

#endif
