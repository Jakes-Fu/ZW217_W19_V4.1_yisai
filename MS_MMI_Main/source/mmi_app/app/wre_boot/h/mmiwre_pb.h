#ifndef     _ADAPT_PB_H_
#define     _ADAPT_PB_H_

#include "mmipb_adapt.h"

typedef struct _tagPB_OPC
{
    uint32 handle;
    uint32 entry_id;
    uint8 storage_id;
    uint8 dwAction;
    uint32 dwFlag;
    void *pReserve;
}PB_OPC, *LPPB_OPC;

#define STORAGE_NAME_LEN      15

typedef struct _tagWREPB_STORAGE_TYPE
{
    uint8    storage_id;            //�洢���ʵ�ID��Ψһ�ı�־�洢����
    wchar  storage_name[STORAGE_NAME_LEN+1];     //�洢���ʵ�����
}WREPB_STORAGE_TYPE, *LPWREPB_STORAGE_TYPE;

typedef struct _tagWREPB_STORAGE_ATTRI
{
    WREPB_STORAGE_TYPE storageType; //�洢��������
    DWORD  dwMaxSize;           //������
    DWORD  dwUsedSize;          //�Ѵ洢����
    DWORD  dwAvailSize;          //�ɴ洢����
}WREPB_STORAGE_ATTRI, *LPWREPB_STORAGE_ATTRI;

typedef struct _tagWREPB_FIND_DATA_T
{
    uint8   storage_id;
    uint32  entry_id;
    void    *pReserved;
}WREPB_FIND_DATA, *LPWREPB_FIND_DATA;

typedef struct _tagWREPB_HANDLE
{
    uint32 handle;
    uint32 curIndex;
    void    *pReserve;
}WREPB_FIND_HANDLE, *LPWREPB_FIND_HANDLE;

#define PB_OPC_ADD        0x0001
#define PB_OPC_DEL        0x0002
#define PB_OPC_UPDATE     0x0003
#define PB_OPC_READ       0x0004

#define WREPB_HANDLE uint32

typedef enum
{
    WREPB_STORAGE_SIM1,
#ifndef MMI_MULTI_SIM_SYS_SINGLE        
    WREPB_STORAGE_SIM2,
    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    WREPB_STORAGE_SIM3,
    #endif
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
    WREPB_STORAGE_SIM4,
    #endif
#endif    
    WREPB_STORAGE_PHONE
}WREPB_STORAGE;

typedef enum
{
    WREPB_ERROR_SUCCESS,          //�ɹ�
    WREPB_ERROR_ERROR         =1 ,//ʧ��
    WREPB_ERROR_PENDING       =2, //������
    WREPB_ERROR_BUSY          =3,//��һ��������û�н���
    WREPB_ERROR_FILE_NOT_EXIST=4,//�ļ�������
    WREPB_ERROR_INVALID_PARAM =5,//��������

    WREPB_ERROR_INVALID_FORMAT=6,//��ʽ����
    WREPB_ERROR_PART_SUCCESS  =7,//���ֳɹ�
    WREPB_ERROR_NO_INITIAL    =8,//û�г�ʼ��
    WREPB_ERROR_NO_ENTRY      =9,//û�м�¼
    WREPB_ERROR_NO_NUMBER     =10,//û�к���
    WREPB_ERROR_NO_MSISDN     =11,//û�б�������
    WREPB_ERROR_BLACKLIST_FULL=12,//��������
    WREPB_ERROR_NUMBER_ALERDY_EXIST=13,//�����Ѿ�����
    WREPB_ERROR_MAIL_LEN=14,//�ʼ����ȴ���
    WREPB_ERROR_SIM_ERROR= 15,//sim������
    WREPB_ERROR_FDN_ENABLE = 16,//sim������
    WREPB_ERROR_UDISK_ISRUN = 17,//U�̿�����
    WREPB_ERROR_NO_DEV = 18, //���̲�����   

    WREPB_ERROR_SFS_ERROR = 19,//�ļ�ϵͳ����
    WREPB_ERROR_SMS_TOO_LARGE = 20,
    WREPB_ERROR_INVALID_NUMBER = 21,//��Ч����
    WREPB_ERROR_NO_SPACE      =22,//�ռ䲻�� 
    WREPB_ERROR_PHONE_NO_SPACE =23, 
    WREPB_ERROR_SIM1_NO_SPACE=24,
    WREPB_ERROR_SIM2_NO_SPACE = 25,
    WREPB_ERROR_SIM3_NO_SPACE = 26,
    WREPB_ERROR_SIM4_NO_SPACE = 27,
    WREPB_ERROR_FMM_NO_SPACE=28,
    WREPB_ERROR_CANCEL=29,
    WREPB_ERROR_MAX
} WREPB_ERROR_E;
    
typedef  void (*WREPB_ACCESS_CALLBACK_FUNC) (WREPB_ERROR_E error, uint32 handle, LPPB_OPC lpPbOpc);


/*=============================================================
 �������� : PBInit
 ���� : ��ʼ���绰��
 ����:��
 ���� : PB_ERROR_SUCCESS: ��ʼ���ɹ�
               ����:ʧ��
=============================================================*/
WREPB_ERROR_E ADAPT_PBInit();

/*=============================================================
 �������� : PBUninit
 ���� : ��ʼ���绰��
 ����:��
 ���� : PB_ERROR_SUCCESS: ��ʼ���ɹ�
               ����:ʧ��
=============================================================*/
WREPB_ERROR_E ADAPT_PBUninit();

/*=============================================================
 �������� : PBEnumStorage
 ���� : ö�ٵ绰���洢�豸
 ����:l
             pPBStorageAttri [out]   �洢�豸������
             nPBStorageNum [out]   �洢�豸�ĸ���

 ���� : PB_ERROR_SUCCESS ��ʾ�ɹ�
               ����: ʧ��
=============================================================*/
WREPB_ERROR_E ADAPT_PBEnumStorage(WREPB_STORAGE_TYPE *lpPBStorageAttri, uint8 *nPBStorageNum);

/*=============================================================
 �������� : PBGetStorageAttri
 ���� : ��ȡ�绰���洢�豸������
 ����:
              storage_id  [in]                �洢�豸ID
              lpPBStorageAttri [out]      �洢�豸������
              
 ���� : PB_ERROR_SUCCESS ��ʾ�ɹ�
               ����:ʧ��
=============================================================*/
WREPB_ERROR_E ADAPT_PBGetStorageAttri(uint8 storage_id, WREPB_STORAGE_ATTRI *lpPBStorageAttri);

/*=============================================================
 �������� : PBGetFreeContactId
 ����: ��ȡ�ض��绰���洢�豸����ϵ�˿���ID
 ����: 
              storage_id [in]   �洢�豸��ID

 ����: �洢�豸�п��õ���ϵ��ID
              
=============================================================*/
DWORD ADAPT_PBGetFreeContactId(uint8 storage_id);


/*=============================================================
 �������� : PBGet
 ����:  ��ȡһ���绰����¼
 ����: 
               entry_id [in]                      Ҫ��ȡ�ļ�¼���ڴ洢�豸�е����id
               storage_id  [in]                  Ҫ��ȡ�ļ�¼���ڵĴ洢�豸id
               dwMaxSize  [in]                  ��������С
               lpvCard [in][out]                ָ��һ��vCard��¼��ָ��
               dwvCardSize [in][out]         ����vCard��¼�Ĵ�С               
   
 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��

=============================================================*/
WREPB_ERROR_E ADAPT_PBRead(uint32 entry_id, uint8 storage_id, uint32 dwMaxSize, char *lpvCard, uint32 *dwvCardSize);


/*=============================================================
 �������� : PBAdd
 ����:  ���һ���绰����¼
 ����: 
              lpvCard [in]             ָ��һ��vCard��¼��ָ��
              dwvCardSize [int]     ����vCard��¼�Ĵ�С
   
 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��

=============================================================*/
WREPB_ERROR_E ADAPT_PBAdd(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize);


/*=============================================================
 �������� : PBAddAsync
 ����: �첽���һ���绰����¼
 ����: 
               lpvCard [in]                ָ��һ��vCard��¼��ָ��
               dwvCardSize [in]        ����vCard��¼�Ĵ�С
               lpAccessCallBack [int   �ص��������ڼ�¼�����ɹ���ʧ�ܺ����
  
 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��
 

=============================================================*/
WREPB_ERROR_E ADAPT_PBAddAsync(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle);

/*=============================================================
 �������� : PBDelete
 ����:  ɾ��һ���绰����¼
 ����:
               entry_id [in]          Ҫɾ���ļ�¼���ڴ洢�豸�е����id
               storage_id  [in]      Ҫɾ���ļ�¼���ڵĴ洢�豸id

 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��


=============================================================*/
WREPB_ERROR_E ADAPT_PBDelete(uint32 entry_id, uint8 storage_id);

/*=============================================================
 �������� : PBDeleteAsync
 ����: �첽ɾ��һ���绰����¼
 ����:
              entry_id [in]                     Ҫɾ���ļ�¼���ڴ洢�豸�е����id
              storage_id  [in]                 Ҫɾ���ļ�¼���ڵĴ洢�豸id
              lpAccessCallBack [in]        �ص��������ڼ�¼�����ɹ���ʧ�ܺ����

 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��
                  

=============================================================*/
WREPB_ERROR_E ADAPT_PBDeleteAsync(uint32 entry_id, uint8 storage_id, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle);

/*=============================================================
 �������� : PBModify
 ����:   �޸�һ���绰����¼
 ����:
                entry_id [in]                 Ҫ�޸ĵļ�¼���ڴ洢�豸�е����Id
                storage_id  [in]             Ҫ�޸ĵļ�¼���ڵĴ洢�豸Id
                lpvCard  [in]                 Ҫ�޸ĵ�vCard��¼������

 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��

=============================================================*/
WREPB_ERROR_E ADAPT_PBModify(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize);

/*=============================================================
 �������� : PBModifyAsync
 ����: �첽�޸�һ���绰����¼
 ����:
                entry_id   [in]                         Ҫ�޸ĵļ�¼���ڴ洢�豸�е����Id
                storage_id  [in]                       Ҫ�޸ĵļ�¼���ڵĴ洢�豸Id
                lpvCard  [in]                           Ҫ�޸ĵ�vCard��¼������
                lpAccessCallBack [in]               �ص��������ڼ�¼�����ɹ���ʧ�ܺ����

 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��
=============================================================*/
WREPB_ERROR_E ADAPT_PBModifyAsync(uint32 entry_id, uint8 storage_id , char *lpvCard, uint32 dwvCardSize, WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack, DWORD dwHandle);

/*=============================================================
 �������� : PBCopy
 ����:����һ���绰����¼
 ����:
                entry_id  [in]                            Ҫ�����ļ�¼���ڴ洢�豸�е����Id
                storage_id_to [in]                     Ҫ�����ļ�¼��Ŀ�Ĵ洢�豸Id
                storage_id _from [in]                Ҫ�����ļ�¼��Դ�洢�豸Id

 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��

=============================================================*/
WREPB_ERROR_E ADAPT_PBCopy(uint32 entry_id, uint8 storage_id_to, uint8 storage_id_from);

/*=============================================================
 �������� : PBCopyAsync
 ����:  �첽����һ���绰��¼
 ����:
                entry_id [int]                   Ҫ�����ļ�¼���ڴ洢�豸�е����Id
                storage_id_to [in]            Ҫ�����ļ�¼��Ŀ�Ĵ洢�豸Id
                storage_id _from [in]        Ҫ�����ļ�¼��Դ�洢�豸Id
                lpAccessCallBack[in]         �ص��������ڼ�¼�����ɹ���ʧ�ܺ����

 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��

=============================================================*/
WREPB_ERROR_E ADAPT_PBCopyAsync(uint32 entry_id, uint8 storage_id_to, uint8 storage_id_from,  WREPB_ACCESS_CALLBACK_FUNC lpAccessCallBack);

/*=============================================================
 �������� : PBClear
 ����: ����ض��洢���ĵ绰����¼
 ����:
                storage_id [in]         Ҫ��յ绰����¼�Ĵ洢�豸��id
                
 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��

=============================================================*/
WREPB_ERROR_E ADAPT_PBClear(uint8 storage_id);

/*=============================================================
 �������� : PBFindFirst
 ����: �����绰����¼
 ����:
                search_str [in]     Ҫ�������������ߵ绰����
                storage_id [in]     Ҫ�����Ĵ洢�豸
                lpFindData [in]     ���ҵ��Ľ����

 ����: ������������ڱ����������

=============================================================*/
WREPB_HANDLE ADAPT_PBFindFirst(uint16 *search_str, uint8 storage_id, WREPB_FIND_DATA *lpFindData);

/*=============================================================
 �������� : PBFindNext
 ����: �������ҽ��
 ����:
                pbHandle [in]           �������
                lpFindData[out]        �������

 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��

=============================================================*/
WREPB_ERROR_E ADAPT_PBFindNext(WREPB_HANDLE pbHandle, WREPB_FIND_DATA *lpFindData);

/*=============================================================
 �������� : PBFindClose
 ����: ��������
 ����:
                pbHandle [in]  �������
                
 ����:  PB_ERROR_SUCCESS ��ʾ�ɹ�
               ������ʾʧ��

=============================================================*/
WREPB_ERROR_E ADAPT_PBFindClose(WREPB_HANDLE pbHandle);


WREPB_ERROR_E ADAPT_PBContactToVcard(MMIPB_CONTACT_T *contact_ptr,  //[IN] 
                                                                             uint32          data_max_size,//[IN]
                                                                             uint8 *data_ptr,//[OUT]
                                                                             uint32 *data_len_ptr//[OUT]
                                                                            );

WREPB_ERROR_E ADAPT_PBVcardToContact(uint8 *data_ptr,//[IN]
                                                                             uint32  data_len,//[IN]
                                                                             MMIPB_CONTACT_T *contact_ptr//[OUT]
                                                                             );

uint8 ADAPT_StorageIDWRE2Mocor(uint8 storage_id);
uint8 ADAPT_StorageIDMocor2WRE(uint8 storage_id);

WREPB_ERROR_E ADAPT_MocorError2WREError(MMIPB_ERROR_E error);

#endif
