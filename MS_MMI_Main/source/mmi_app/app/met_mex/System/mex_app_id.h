/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
*
* �ļ����ƣ� // mex_app_id.h
* �ļ���ʶ��
* ����ժҪ�� // ����Ӧ�ó���ID
*
************************************************************************/

#ifndef __MEX_APP_ID_H__
#define __MEX_APP_ID_H__

//ƽ̨����汾ID��Ҳ����Ӧ�ó���ID
typedef enum
{
    E_MSVT_Normal       = 0,                //�����汾
    E_MSVT_QQ           = 11111,            //qq�汾
    E_MSVT_MSN          = 11110,            //msn�汾
    E_MSVT_TB           = 10804,            //tb�汾
    E_MSVT_SMS          = E_MSVT_Normal,    //���Ű汾 (��ʱδʹ��)
    E_MSVT_FETION       = 11109,            //���Ű汾
    E_MSVT_WOCHAT       = 11113,            //����
    E_MSVT_LWSG         = 11072,            //��������
    E_MSVT_ZT           = 11114,            //��;
    E_MSVT_FARM         = 11107,            //ũ��
    E_MSVT_ZWDZJS       = 11070,            //ֲ���ս��ʬ
    E_MSVT_DDZ          = 11008,            //������
    E_MSVT_FKCS         = 11064,            //CS
    E_MSVT_JSCB         = 11104,            //��ɽ�ʰ�
    E_MSVT_CSJ          = 11108,            //������
    E_MSVT_ZFT          = 11117,            //�ҷ�
    E_MSVT_YMDS         = 11074,            //��Ĭ��ʦ
    E_MSVT_PLAYER       = 11115,            //������
    E_MSVT_HDHL         = 11116,            //�춹����
    E_MSVT_HXZ          = 11082,            //˵�������
    E_MSVT_DMC          = 11162,            //���ӱ���
    E_MSVT_FNDXN        = 11178,            //��ŭ��С��
    E_MSVT_WEATHER      = 11197,            //����Ԥ��
    E_MSVT_WEIBO        = 11189,            //����΢��
    E_MSVT_WOMIVIDEO    = 11113,            //������Ƶ
    E_MSVT_TOMCAT       = 11156,            //Tom è
    E_MSVT_FAUI			= 11204,            //FAUI
    E_MSVT_FRUITSLICE   = 11157,            //��ˮ��
    E_MSVT_AQ           = 11181,            //������ʿ
    E_MSVT_CQZ          = 11155,            //��ȹ��
	E_MSVT_BULLETSFLY   = 11235,            //���ӵ���
	E_MSVT_MLSZ         = 11284,            //ħ������
	E_MSVT_MVBOL        = 11282,            //����ƽ����
	E_MSVT_HDCQZ        = 11261,            //ȹ�Ƿ���
	E_MSVT_HDTQYB       = 11299,            //HD����
	E_MSVT_HTC_TQ       = 11323,            //HTC����
	E_MSVT_I_TQ         = 11324,            //i����
	E_MSVT_LIGHTER      = 11240,            //ħ�ô���
	E_MSVT_LIGHTERHD    = 11317,            //HDħ�ô���
    E_MSVT_DZDP         = 11273, 	        //���ڵ���
    E_MSVT_FNDXN_RIO    = 11321, 	        //��ŭ��С��:��Լ��ð��
    E_MSVT_SETTIME      = 11175, 	        //�Զ���ʱ
    E_MSVT_JJFB         = 11005, 	        //���׷籩
    E_MSVT_JPFCKB       = 11289, 	        //��Ʒ�ɳ�
    E_MSVT_GANJILIFE    = 11283, 	        //�ϼ�����
    E_MSVT_MEXSQAPP     = 11321, 	        //ͨ������
    E_MSVT_SHSL         = 11234,            //�����
    E_MSVT_CSJJS         = 11267,            //cs�ѻ���
    E_MSVT_BLFC         = 11275,            //�����ɳ�
    E_MSVT_MXSQY        = 11017,            //������ȱһ
    E_MSVT_HDCAT        = 11331,            //HDè
    E_MSVT_DZT        = 11266,            //��ֽ�� 
	
    E_MSVT_Max
} E_Mex_Special_Version_Type;



#endif // __MEX_APP_ID_H__

