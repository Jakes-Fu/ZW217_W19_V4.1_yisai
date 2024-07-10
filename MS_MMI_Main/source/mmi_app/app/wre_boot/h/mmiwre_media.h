#ifndef _MMIWRE_MEDIA_H_
#define _MMIWRE_MEDIA_H_

//2.3.3	֪ͨ��
#define MEDIA_NOTIFY_FINISHED				0x1001
#define MEDIA_NOTIFY_REQUESTDATA			0x1002
#define MEDIA_NOTIFY_REPONSEDATA			0x1003
/////////////////////////////////////////////////////////////////////
//2.3.1	�����루֪ͨ�����룩
#define MEDIA_ERROR_NOERROR			0x00000000	//û�д���
#define MEDIA_ERROR_UNKNOWNERROR	0x800A0001	//δ֪����
#define MEDIA_ERROR_BADFORMAT		0x800A0002	//ý���ʽ��֧��
#define MEDIA_ERROR_NOTENABLED		0x800A0003	//�豸��ʧ��
#define MEDIA_ERROR_INVALHANDLE		0x800A0004	//�豸�����Ч
#define MEDIA_ERROR_NOMEM			0x800A0005	//�ڴ����ʧ��
#define MEDIA_ERROR_NOTSUPPORTED	0x800A0006	//���ܲ�֧��
#define MEDIA_ERROR_INVALID			0x800A0007	//��Ч��ID
#define MEDIA_ERROR_INVALFLAG		0x800A0008	//��Ч�ı�־
#define MEDIA_ERROR_INVALPARAM		0x800A0009	//��Ч�Ĳ���
#define MEDIA_ERROR_FILEOPENERR		0x800A000A	//�ļ��򿪳���
#define MEDIA_ERROR_OUTOFRANGE		0x800A000B	//�������
#define MEDIA_ERROR_GETSTREAM_WAIT  0x800A000C	//�����ݻ�ȡ�ȴ�
#define MEDIA_ERROR_GETSTREAM_ASY   0x800A000D	//�������첽������
#define MEDIA_ERROR_CONFLICT		0x800A000E	//�豸����ռ
/////////////////////////////////////////////////////////////////////
//2.3.2	ý���ʽ֧��
#define MEDIA_FORMAT_MIDI			L"audio/mid"
#define MEDIA_FORMAT_WAV			L"audio/wav"
#define MEDIA_FORMAT_MP3			L"audio/mp3"
#define MEDIA_FORMAT_AAC			L"audio/aac"
#define MEDIA_FORMAT_M4A			L"audio/m4a"
#define MEDIA_FORMAT_AMR			L"audio/amr"
#define MEDIA_FORMAT_TONE			L"audio/xts"
#define MEDIA_FORMAT_3GP			L"video/3gp"
#define MEDIA_FORMAT_MP4			L"video/mp4"
/////////////////////////////////////////////////////////////////////
typedef unsigned char BOOLEAN;
typedef unsigned long  HMEDIAHANDLE;  //�豸���
typedef long HRESULT;
/////////////////////////////////////////////////////////////////////
//ý���������
typedef enum  
{ 
	MEDIA_TYPE_UNKNOWN=-1, //δ֪
	MEDIA_TYPE_FILE,//�ļ��ķ�ʽ����/¼��
	MEDIA_TYPE_BUFFER,//buffer�ķ�ʽ����/¼��
	MEDIA_TYPE_STREAM,//���ķ�ʽ���в���/¼��  
	MEDIA_TYPE_TONE  //Tone������
		
}eMediaType;

//ָý��seek������
typedef enum 
{ 
	MEDIA_SEEK_TYPE_POS, //���ֽڵķ�ʽ����    ===�ݲ�֧��
	MEDIA_SEEK_TYPE_TIME //��ʱ��ķ�ʽ����
		
}eMMSeekType;

//ý�嵱ǰ״̬
typedef enum 
{ 
	MEDIA_STATE_UNKNOWN=-1, //δ֪״̬
	MEDIA_STATE_PLAYING,    //���ڽ��в���
	MEDIA_STATE_PAUSE,      //��ͣ״̬
	MEDIA_STATE_STOP,       //ֹͣ״̬
	MEDIA_STATE_RECORDING   //���ڽ���¼��
		
}eMMState;


//ý���ʼ�����ƽṹ
typedef struct  tagMEDIADATA 
{
    unsigned char           isPlay;     //�豸ʹ�����ͣ�TRUE -��ʾ�������ţ�FALSE-��ʾ����¼��
    eMediaType              eType;      //ý���������
    unsigned short*         lpData;     //ý�����ݣ������ݿ�������,������ý�壬ָ��ý��ͷ���ݻ�����
    unsigned long           dwDataLen;  //ý�����ݳ��ȣ���������������ļ�����ô��ֵ�������ο������򽫻���Ϊ���ݳ���
    const unsigned short*   lpFormat; //ý�崦��ĸ�ʽ
    void                    (*hOwner)(unsigned int wParam,unsigned int lParam,unsigned int unReserved); //ý�岥����Ҫ������Ϣ������callback��
																								//���Ϊ�գ��������н���
    unsigned long           dwReserved; //������Ԫ
    //����ض���
    unsigned long           uiSampleRate;//��������  ��:8000/16000/32000
    unsigned long           uiBitPerSample;//������С ��:8/16/32
    unsigned long           uiChannelNum;//ͨ����:0-stereo/1-joint stereo/2-dual channel/3-single channel
    int			            (*GetStreamData)(unsigned int unHWho,unsigned char** ppucDataAddr,unsigned int* puiDataLength); //��ý�岥����Ҫ�������ݻ�ȡ

}MEDIAData,*PMEDIAData;


/*********************************************************************
 *  mmOpen
 *
 * ���첢��ʼ��һ��ý���豸
 *
 * ����
 * hParent
 *   [in] ָ��һ���豸�����������������,����ΪNULL��
 * lpmmd
 *   [in] ָ��һ��MMData �ṹ������Ϊ��������֮ǰ�����밴�պ��ʵ������������������ṹ��
 * ����ֵ
 *  ����ɹ�������ֵ��һ��32λ��HMMDev��
 *  ���ʧ�ܣ�����ֵ���㡣

 * ˵��
 *   ����tone��,Ŀǰֻ��ý���������á�
 *   Ŀǰ�������Ӹ�������MP3,�ӱ�����WAV,����ֻ֧������;
*/
HMEDIAHANDLE  OpenMedia(MEDIAData *lpmmd);
HMEDIAHANDLE  _OpenMediaEx(HMEDIAHANDLE hParent,MEDIAData *lpmmd);
/*********************************************************************
 *  mmClose
 *
 * �رղ�����һ��ý���豸
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
*/
HRESULT  CloseMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmPlay
 *
 * ����ý�岥��
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��

 * ˵��
 *   ����tone��,����ʧ�ܡ�
*/
HRESULT  PlayMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmPause
 *
 * ��ͣý�岥��/¼��
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��

 * ˵��
 *   ����tone��,����ʧ�ܡ�
*/
HRESULT  PauseMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmResume
 *
 * ��������/¼����ͣ��ý��
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��

 *˵��
 *   ����tone��,����ʧ�ܡ�
*/
HRESULT  ResumeMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmStop
 *
 * ֹͣ����/¼�Ƶ�ý��
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��

 *˵��
 *   ����tone��,����ʧ�ܡ�
*/
HRESULT  StopMedia(HMEDIAHANDLE  hwho);


/*********************************************************************
 *  mmSeek
 *
 * ý�岥�Ŷ�λ
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * esType
 *   [in] ָ��λ�Ѳ����ͣ��꿴2.1.2
 * lValue
 *   [in] ָ��λ��ֵ�����ֽڻ��Ǻ��룬��������ȷ��
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
 * ˵��
 *   ����tone��,����ʧ�ܡ�
*/
HRESULT  SeekMedia(HMEDIAHANDLE  hwho,     // �豸���
				eMMSeekType  esType,
				long     lValue);

/*********************************************************************
 *  mmSetVolume
 *
 * �����豸����
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * dwValue
 *   [in] ָ����ֵ
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
*/
HRESULT  SetMediaVolume(
				HMEDIAHANDLE  hwho,     // �豸���
				unsigned long dwValue);

/*********************************************************************
 *  mmGetVolume
 *
 * ��ȡ�豸����ֵ
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * pdwValue
 *    [out] ָ����ֵ
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
*/
HRESULT  GetMediaVolume(
				HMEDIAHANDLE  hwho,    // �豸���
				unsigned long *pdwValue);

/*********************************************************************
 *  mmGetState
 *
 * ��ȡ��ǰý�����״̬
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * pdwState
 *   [out] ָ״ֵ̬�����ڲ�����˵����ָ�����š���ͣ��ֹͣ��δ֪��
 *                    ����¼����˵��¼�ơ���ͣ��ֹͣ��δ֪�� 
 *
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
*/
HRESULT  GetMediaState(
				HMEDIAHANDLE  hwho,    // �豸���
				unsigned long *pdwState);

/*********************************************************************
 *  mmGetDuration
 *
 * ��ȡý�岥����ʱ��
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * pdwDuration
 *   [out] ָý�岥����ʱ��(����)
 * 
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
*/
HRESULT  GetMediaDuration(
				HMEDIAHANDLE  hwho,     // �豸���
				unsigned long *pdwDuration);

/*********************************************************************
 *  mmGetTime
 *
 * ��ȡý�岥�ŵ�ǰʱ��
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * pdwTime
 *   [out] ָý�岥�ŵ�ǰʱ��(����)
 * 
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
*/
HRESULT  GetMediaTime(
				HMEDIAHANDLE  hwho,     // �豸���
				unsigned long *pdwTime);

/*********************************************************************
 *  mmRecord
 *
 * ý��¼��
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * 
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
 * ˵��
 * ����tone��,����ʧ�ܡ�
 *
*/
HRESULT  RecordMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmSetMute
 *
 * ��������
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * isMute
 *   [in] �������أ�TRUE ������FALSE �رվ���
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
 *
*/
HRESULT  SetMediaMute(
				HMEDIAHANDLE  hwho,     // �豸���
				BOOLEAN    isMute);

/*********************************************************************
 *  mmPlayTone
 *
 * ����tone��
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * dwDuration
 *  [in] ����ʱ��
 * dwFreq
 *  [in] ����Ƶ��
 *
 *
 *����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
 *
*/
HRESULT  PlayTone(
				HMEDIAHANDLE  hwho,     // �豸���
				unsigned long      dwDuration,
				unsigned long   dwFreg);


BOOLEAN Enable_Tp_Key_Tone(int tone_type,BOOLEAN opt);
/*********************************************************************
 *  GetMediaMicDb
 *
 * ��ȡ��ǰ�豸Mic��������
 *
 * ����
 * hwho
 *   [in] ָ��һ���豸�����
 * pdwMicDb
 *    [out] ָMic������
 *
 * ����ֵ
 *  ����ɹ�������ֵ���㡣
 *  ���ʧ�ܣ�����ֵ�� һ�� HRESULT���͵�ֵ��
 *
 *
*/
HRESULT  _GetMediaMicDb(
				HMEDIAHANDLE  hwho,     // �豸���
				unsigned long    *pdwMicDb);



/*********************************************************************
 *  MMIWRE_AdjustVolumeBySideKey_UP
 *
 * the function is called when the side up_key is pressed in wre application 
 *
 * param:
 *
 * return value:
 * If the function successes ,the return value is true,
 * If the function fails ,the return value is false
 *
 *author :chi.llei 20120417
**/
PUBLIC BOOLEAN MMIWRE_AdjustVolumeBySideKey_UP(void);

/*********************************************************************
 *  MMIWRE_AdjustVolumeBySideKey_DOWN
 *
 * the function is called when the side down_key is pressed in wre application 
 * param:
 *
 * return value:
 * If the function successes ,the return value is true,
 * If the function fails ,the return value is false
 *
 *author :chi.llei 20120417
**/
PUBLIC BOOLEAN MMIWRE_AdjustVolumeBySideKey_DOWN(void);

PUBLIC void MMIWRE_InitSideKeyVolume(void);

PUBLIC int MMIWRE_RecordBufferStop(void);
PUBLIC BOOLEAN MMIWRE_StartRecordBuffer(const uint8 * buffer, uint32 buf_len);

#endif //_ADAPTOR_MEDIA_
