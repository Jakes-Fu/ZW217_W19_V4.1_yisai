/**************************************************************************
 iekieImSocket.c
 
 Description: iekie ���뷨 ��������

**************************************************************************/

#include "IIMESocket.h"

#if 0
typedef struct tagAmharicProvider
{
    DECLARE_PROVIDER_INTERFACE()

    iIME_INT          m_idMethod;
    const void*     pdbMisc;

    STATUS          m_iStatus;

    iIME_WCHAR        m_cUnderLine;
    iIME_WCHAR        m_cCommit;
    iIME_WCHAR        m_preKey;
    int             m_nCount;

} AmharicProvider;
#endif


/*�������״̬*/
IIME_BOOL iekie_imsocket_network_check(void);

/*���ݾ������뷨��Ҫ��ldb�ļ�����ldb���ص��ڴ�*/
IIME_BOOL iekie_ldb_load(int ldb_num);


IIME_BOOL iekie_imsocket_network_check(void)
{
   IIME_BOOL net_stat = 0;
   if (IEKIE_CHECK_NET_STAT()==1)
   {
   	net_stat = 1;
   }

   return net_stat;
}

IIME_BOOL iekie_ldb_load(int ldb_num)
{
   return 1;
}


