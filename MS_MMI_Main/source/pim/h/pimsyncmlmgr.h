/******************************************************************************
                                   ����WCDMA  ����ҵ��ģ��
        ����3G  ������𽥳��죬������ҵ�������Ϊ������
        �ƶ��ն˵����㣬�𽥳�Ϊ���ɻ�ȱ��һ���֡���
        ģ����ƻ���OMA  ��������Ļ���syncml Э������ݺ���
        �˹���ʵ���ڶ�ƽ̨�������ϵͳ�е�PIMҵ��Ŀ��
        ��Ҫ�ﵽ����ƽ̨�Ŀ���ֲ�����õĹ����ȶ���ģ�顣
        ��ģ���ʵ�ַ�Ϊ��������:
        1.ʵ��Data sync����
        2.ʵ��Device Management����
        3.ʵ�ֶ�ƽ̨��ƽ����ֲ
        ģ�鿪���ο�OMA��Ա��ͬ������SRT_c��
        Ŀǰģ��ƻ�����֧��OMA syncML Э��1.1


        ��Ȩ����:�������Ȩ���ں���ͨ�ţ�����һ�з���Ȩ��
        ������Ա:����
        ��������:2005��4��11�ա�
******************************************************************************/


/******************************************************************************
�ļ�����:PIMsyncmlmgr.h
�ļ�����:syncml manager,syncml����ģ��ͷ�ļ���
�ļ�˵��:syncml����ģ�飬��Ҫ������smlXX��syncml�ں�ģ�齻��
������Ա:����
�״ο�������:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            �޸ļ�¼
  ����                   ����                �޸�ԭ��
  -----------------------------------------------------------------------------
  2004-4-11              ����                ��һ�ο������롣
  -----------------------------------------------------------------------------
  *****************************************************************************/
/*�����FEATURE��ֹͷ�ļ��ظ�����*/
  #ifndef   HEADER_FILE_SYNCMLMGR
  #define  HEADER_FILE_SYNCMLMGR
  
  /*  ����ͷ�ļ�*/
  #include "PIMsmlcore.h"
  #include "mmipim.h"//wangzhefeng add 20090911

  /*  ���ݽṹ����*/
  
  /*  ȫ�ֱ�������*/

  /*  ���ܺ�������*/
   
  short hs_syncml_init(void);

  short hs_syncml_initance(short * pID, char* workspacename);

  short hs_syncml_startmessage(short id);

  short hs_syncml_StartSync(short id);

  short hs_syncml_AlertCmd(short id,char* cmd);

  short hs_syncml_PutCmd(short id);

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
  short hs_syncml_PutVCALCmd(short id);
#endif /* HS_PIM_VCAL */

  short hs_syncml_GetCmd(short id);
	
  short hs_syncml_AddCmd(short id); // chenxiang 20071124

  short hs_syncml_DeleteCmd(short id);

  short hs_syncml_StatusCmd(short id);

  short hs_syncml_MapCmd(short id);

  short hs_syncml_ReplaceCmd(short id); // chenxiang 20071124

  short hs_syncml_EndSync(short id);

  short hs_syncml_EndMessage(short id);

  short hs_syncml_EndMessageWithoutFinal(short id);//add by wangxiaolin 2007.11.10

  short hs_syncml_ReceiveData(short id);

  short hs_syncml_TerminateInstance(short id);

  short hs_syncml_TerminateAllExitInstance(void);

  short hs_syncml_Terminate(void);

  short hs_syncml_fresh_for_slow_sync(short id);

  short hs_syncml_modify_for_fast_sync(short id);
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
void hs_syncml_set_large_object_proc_flag(BOOLEAN  proc_flag);
#endif

  #endif