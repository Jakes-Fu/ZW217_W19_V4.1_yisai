#ifndef _DELTA_SSL_H
#define _DELTA_SSL_H


#define SSL_V2 2
#define SSL_V3 3
#define TLS_V1 4

#define RSA_WITH_DES_CBC_SHA   9
#define RSA_WITH_DES_CBC3_SHA  10

#define MyClientNum 10   //SSL认可的最大用户数

#define CERTF  "client.crt"  /*客户端的证书(需经CA签名)*/
#define KEYF  "client.key"   /*客户端的私钥(建议加密存储)*/
#define CACERT "ca.crt"      /*CA 的证书*/

struct CertDesc{
	char Version[10];       //证书的版本号
	char * SerialNumber;    //证书的序列号
	char SignAlgorithm[30]; //证书的签名算法
	char * Issure;          //证书的颁发者
	char NotAfter[24];      //证书的有效起始日期
	char NotBefore[24];     //证书的有效终止日期
	char * subject;         //证书的主题（拥有者）
	char * PublicKey;       //证书的公钥
};

struct ErrInfo{
	unsigned char ErrInd;
/*用位表示出错类型，当前使用低三位
ErrInd && 0x01=0x01 :  该服务器证书与您试图查看的网页名次不匹配
ErrInd && 0x02=0x01 :  该服务器证书已过期
ErrInd && 0x04=0x01 :  该服务器证书由您没有选定的信任的公司颁发
*/
	struct CertDesc * ServerCert;
	struct CertDesc * CACert;
};

struct UserList
{
	char * name [MyClientNum];     //字符串指针数组
	short length;   //字符串的数目
	char pass[9];       // 回填密码
	char index;   //回填字符串索引
};
struct SSLParam {
	short  ProtocolVersion;
	short  CipherSuite;
	int (*callback)(void *err,int flag);
/*
	浏览器提供的回调函数，用于显示SSL连接的提示信息并与用户进行交互
返回值定义：
1：用户选择'NO'
2：用户选择'YES'
3：用户选择安装CA证书并选择'YES'
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