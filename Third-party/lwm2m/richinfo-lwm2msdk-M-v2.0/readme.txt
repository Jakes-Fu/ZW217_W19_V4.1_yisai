0.����
ԭ���� linux_2.6_x86_64
�����ڿ�pthread;

makefile��������:
make all
make clean
make testsdk
make install

makefile����˵��:
-DLWM2M_CLIENT_MODE: ʹ��lwm2m�ͻ���ģʽ
-DLWM2M_SUPPORT_JSON: �ڳ�����֧��json��������
-DLWM2M_LITTLE_ENDIAN: ����ƽ̨���ֽ�˳��ΪС��; //��Ϊ���,��ʹ�ú�-DLWM2M_BIG_ENDIAN
-DLWM2M_WITH_LOGS: ��ʾlwm2m�ײ���־;


1.Ŀ¼
core��lwm2m sources
sdksrc/sdk: sdk sources
sdksrc/testsdk: test use sdk example
sdksrc/shared:  common sources

2.��������˵��
LWM2M_SDK_INIT�������
typedef struct
{
	char szCMEI_IMEI[64]; 	//CMEI/IMEI ;��IMEI2ѡһ����ο��淶�ĵ�
	char szCMEI_IMEI2[64]; 	//CMEI/IMEI2;
	char szIMSI[64];		//IMSI;
	char szDMv[16]; 		//DM�汾��;�̶�v2.0
	char szAppKey[64]; 		//appkey;  ������
	char szPwd[64];			//pwd��Կ; ������
	
	int  nAddressFamily; 	//4��6�� ��ʾipv4��ipv6; �̶���Ϊ4;
	char szSrvIP[32];		//������ip��ַ��host���� shipei.fxltsbl.com �� 127.0.0.1��
    int  nSrvPort;   		//5683; �������˿�
	int  nLifetime;    		//300s ;���ڸ���ע��ʱ�䣻��λ�� 
	int  nLocalPort;       	//���ض˿ڰ�,Ĭ��Ϊ56830
	int  nBootstrap; 		//�Ƿ�booststrap 0,not use; other n,use; ��ǰ�̶���Ϊ0��
	
} Options;

typedef struct
{
    //sdk��Ϣ֪ͨ����--no block
	//OptNotifyParam ֪ͨ���ͺͽ��
	void  (*NotifyMsg) (OptNotifyParam *optNotifyParam);
	
	//�ɼ��豸��Ϣ�Ķ�����--�������ܳ�ʱ����
	//resId  : �豸��Ϣ��Դ�ţ����淶�ĵ�����6602�ȣ�
	//outbuff: resId��Ӧ���豸���ַ���ֵ�� ��Ҫ�����ڲ�malloc�ַ����Ĵ洢�ռ䣻sdk�ڲ�ʹ������ͷţ�
	//return�� 0���ɹ��� ����ʧ��
	int   (*DMReadInfo) (int resId,char **outbuff);
	
} OptFuncs;


//��ʼ��Options:����,optfuncs:�ص�����
int LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs); 

//����sdk��nMode=0,�����ڵ�ǰ�߳����У�nMode!=0,�����߳�������
int LWM2M_SDK_RUN(int nMode);

//�˳�SDK��ѭ����ע��lwm2m����
int LWM2M_SDK_STOP();

//����sdk,������
int LWM2M_SDK_FINI();


//�汾˵��
M180525: 
�Զ���ʵ�ֵ�aes�����㷨(����openssl��sdk����ͬ������); 
������IMEI2�ֶδ���(����������); 
�����ɴ��뱾�ذ󶨶˿�;
����bug�޸�;

M180606: 
�������ֱ���澯��Ϣ

M180906: 
���Ӷ��׶����ֶεĲɼ���DMReadInfo�ص�������
�޸�sha256�ڴ��ϵͳ�ϵ�bug��

M190304: 
�޸���־��ʾ����









