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
�ļ�����:PIMsmlcore.h
�ļ�����:����ģ�����
�ļ�˵��:����ģ��ĺ������ݣ��ؼ�������������һ������
                       ������ʵ�ֺ�����ģ����ĵ�ͷ�ļ���
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
  #ifndef   HEADER_FILE_SMLCORE
  #define  HEADER_FILE_SMLCORE
  
  /*  ����ͷ�ļ�*/
  #include "PIMsmldef.h"
  #include "PIMwsm.h"

  /*  ���ݽṹ����*/

  /** 
 * structure describing the current status of the global syncml module
 * (holds all global variables within SyncML)
 **/
typedef struct syncml_info_s {
  InstanceInfoPtr_t        instanceListAnchor;// Anchor of the global list of known SyncML instances
  SmlOptionsPtr_t          syncmlOptions;     // Options valid for this SyncML Process
  WsmGlobalsPtr_t          wsmGlobals;        // Workspace global variables
  TokenInfoPtr_t           tokTbl;
} *SyncMLInfoPtr_t, SyncMLInfo_t;

  /*  ȫ�ֱ�������*/
  extern  SyncMLInfoPtr_t hs_pGlobalAnchor;

  /*  ���ܺ�������*/
  short hs_smlInit(SmlOptionsPtr_t pOptions);
  short hs_smlInitInstance(SmlCallbacksPtr_t callbacks, SmlInstanceOptionsPtr_t pOptions, void* pUserData, short *pInstanceID);

  short hs_smlStartMessageExt(short id, SmlSyncHdrPtr_t pContent, SmlVersion_t vers);
  short hs_smlStartSync(short id, SmlSyncPtr_t pContent);
  #ifdef HS_ADD_SEND
  short hs_smlAddCmd(short id, SmlAddPtr_t pContent);
  #endif
  short hs_smlDeleteCmd(short id, SmlDeletePtr_t pContent);
  short hs_smlReplaceCmd(short id, SmlReplacePtr_t pContent);
  short hs_smlAlertCmd(short id, SmlAlertPtr_t pContent);
  short hs_smlPutCmd(short id, SmlPutPtr_t pContent);
  short hs_smlGetCmd(short id, SmlGetPtr_t pContent);
  short hs_smlStatusCmd(short id, SmlStatusPtr_t pContent);
  short hs_smlMapCmd(short id, SmlMapPtr_t pContent);
  short hs_smlEndSync(short id);
  short hs_smlSetUserData(short id, void* pUserData);
  short hs_smlProcessData(short id, SmlProcessMode_t mode);
  short hs_smlEndMessage(short id, unsigned char final);
  short hs_smlTerminateInstance (short id);
  short hs_smlTerminate(void);
 
  short hs_smlLockWriteBuffer(short id, unsigned char **pWritePosition, long *freeSize);
  short hs_smlUnlockWriteBuffer(short id, long writtenBytes);
  short hs_smlLockReadBuffer(short id, unsigned char* *pReadPosition, long *usedSize);
  short hs_smlUnlockReadBuffer(short id, long processedBytes);

  short hs_mgrProcessStartMessage(short id, InstanceInfoPtr_t pInstanceInfo);
  short hs_mgrResetWorkspace (short id) ;
  short hs_mgrProcessNextCommand(short id, InstanceInfoPtr_t pInstanceInfo);

#endif
