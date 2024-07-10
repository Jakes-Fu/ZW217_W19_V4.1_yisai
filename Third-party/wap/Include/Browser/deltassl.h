#ifndef _DELTA_SSL_H
#define _DELTA_SSL_H


#define SSL_V2 2
#define SSL_V3 3
#define TLS_V1 4

#define RSA_WITH_DES_CBC_SHA   9
#define RSA_WITH_DES_CBC3_SHA  10

#define MyClientNum 10   //SSL�Ͽɵ�����û���

#define CERTF  "client.crt"  /*�ͻ��˵�֤��(�辭CAǩ��)*/
#define KEYF  "client.key"   /*�ͻ��˵�˽Կ(������ܴ洢)*/
#define CACERT "ca.crt"      /*CA ��֤��*/

struct CertDesc{
	char Version[10];       //֤��İ汾��
	char * SerialNumber;    //֤������к�
	char SignAlgorithm[30]; //֤���ǩ���㷨
	char * Issure;          //֤��İ䷢��
	char NotAfter[24];      //֤�����Ч��ʼ����
	char NotBefore[24];     //֤�����Ч��ֹ����
	char * subject;         //֤������⣨ӵ���ߣ�
	char * PublicKey;       //֤��Ĺ�Կ
};

struct ErrInfo{
	unsigned char ErrInd;
/*��λ��ʾ�������ͣ���ǰʹ�õ���λ
ErrInd && 0x01=0x01 :  �÷�����֤��������ͼ�鿴����ҳ���β�ƥ��
ErrInd && 0x02=0x01 :  �÷�����֤���ѹ���
ErrInd && 0x04=0x01 :  �÷�����֤������û��ѡ�������εĹ�˾�䷢
*/
	struct CertDesc * ServerCert;
	struct CertDesc * CACert;
};

struct UserList
{
	char * name [MyClientNum];     //�ַ���ָ������
	short length;   //�ַ�������Ŀ
	char pass[9];       // ��������
	char index;   //�����ַ�������
};
struct SSLParam {
	short  ProtocolVersion;
	short  CipherSuite;
	int (*callback)(void *err,int flag);
/*
	������ṩ�Ļص�������������ʾSSL���ӵ���ʾ��Ϣ�����û����н���
����ֵ���壺
1���û�ѡ��'NO'
2���û�ѡ��'YES'
3���û�ѡ��װCA֤�鲢ѡ��'YES'
*/
};

//SSL *  SSLSetParam(struct SSLParam * param);
void SSLSetParam(struct SSLParam * param);
//short HTTPSConnect(int sd , struct sockaddr_in *sa,SSL * ssl,char * SiteName);
short HTTPSConnect(int sd , struct sockaddr_in *sa,char * SiteName);
int deltaSSLWrite(const char *buf,int num);
int deltaSSLRead(char *buf,int num);
void HTTPSExit(void);


// New DSSLAPI
struct DSSLCert{
		char UserName[7];
		char Version[10];
		char *SerialNumber;
		char SignAlgorithm[30];
		char *Issure;
		char NotAfter[24];
		char NotBefore[24];
		char *Subject;
		char *PublicKey;
};

struct DSSLParam{
	int IsVerifyServer;
	int ProtocolVersion;
	int CihperSuite;
	int (*UserCallback)(void *err,int flag);
};

#define RSA_EXPORT_WITH_RC4_40_MD5	3
#define RSA_WITH_RC4_128_MD5	4
#define RSA_WITH_RC4_128_SHA	5
#define RSA_EXPORT_WITH_RC2_CBC_40_MD5	6
#define RSA_WITH_IDEA_CBC_SHA	7
#define RSA_EXPORT_WITH_DES40_CBC_40_SHA	8

int DSSLConnect(int sd,struct sockaddr_in *sa, char *SiteIP);
int DSSLInit(struct DSSLParam *param);
int DSSLWrite(char *buf, int buf_len);
int DSSLRead(char *buf, int buf_len);
int DSSLExit(void);
int DSSLGetCertInfo(int type,struct DSSLCert *cert);
int DSSLStoreCACert(unsigned char *buf, short len);
int DSSLStoreClientCert(unsigned char *buf, short len, char *username, char *passwd);
int DSSLDelCert(int type, char *SubjectName);
void CertInit(void);

#define ENOCERTFOUND	-10
#define EREADCERTERROR	-11
#define EBADCERT		-12
#define EDELFLASHERROR	-13
#define EBADPASS		-14

#endif