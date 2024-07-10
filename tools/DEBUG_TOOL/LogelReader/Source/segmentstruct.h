//�����׽ṹ��ռ��1��sect��С����512�ֽ�

//���ڱ�ʾ���̵�ǰ������log��¼����ʹ�ã����ڵ�һ���������������ڴ��̸�ʽ��ʱ�ᱻɾ��

//little endian


typedef struct	//�ö�ֻ�ڸ�ʽ��ʱд��̶���Ϣ��֮�󲻻�ı�
{
	BYTE	OEMname[16];		//eg."SPREADTRUM" ���ñ�־Ҳ�����жϴ��̵�ǰ�Ƿ������洢��
	DWORD	segmentStartPos;	//��һ���ε���sectλ�ã���λ��֮ǰ������������֮������򱻻��ֳ�һ�����Ķ�����
	DWORD	segmentSize;		//���̴洢�Ķδ�С���ô�С�����㹻��������assert��Ϣ��ͬʱ�ֲ��ܹ��󣬽���64M
	DWORD	segmentCount;		//��Ч�ε��ܸ���
	BYTE	reserved[512-28];	//�����ֶ�
}DISKINFO_T;

typedef struct
{
//sect 1	//��sectorֻ�ڸö�����ʱд��һ����Ϣ
	DWORD	groupFlag;		//0xFFFFFFFF��ʾ��Ч�����̵�����group�ű���������˳���ۼӣ�������Ϊ��Ч
	DWORD	SN;			//��ǰ��������group�е���ţ���������̵�group��һ���ģ���ô����ſ��������ж�log��λ��
	DWORD	dataStartPos;		//�ö���log������ʼsectorλ��
	BYTE	reserved1[512-12];	//�����ֶ�;
//sect 2	//��sector���ڼ�¼ʵ�����������ȣ����ܱ����д��
	DWORD	dataSize;		//�ö���Ч���������ȣ������ǰ����assert������ ����ô�ó���������assertС�γ���֮��
	BYTE	reserved2[512-4];	//�����ֶ�
}SEGMENTINFO_T;

//--------------------------------