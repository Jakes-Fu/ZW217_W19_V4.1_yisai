// Phone.h: interface for the CPhone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONE_H__786F02C2_342E_4EFF_98CC_7296FD0804D1__INCLUDED_)
#define AFX_PHONE_H__786F02C2_342E_4EFF_98CC_7296FD0804D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct _SMS;
class IPort;
class CPhone  
{
public:
    void SetPort( IPort * pIntfc );

    BOOL IsConnected( void );
    BOOL InitPhone( void );

	int  GetSIMState( void );
	int  GetMMIVersion( void );
	int  GetSIMCount( void );

    BOOL DeleteSMS( _SMS * psms );
public:
	CPhone();
	~CPhone();

protected:
    BOOL ExecuteCmd( LPCSTR pCmd );

private:
    IPort * m_pPort;
	int     m_nSIMState;   
	int     m_nSIMCount;
	int     m_nMMIVersion;

    static const int  MAX_RETRY_COUNT;
	static const int  READ_DELAY_TIME;

	static const char MMI_READY[];
	static const char MMI_READY_OK[];
	static const char MMI_ERROR[];
	

	static const char AT_AT[];
    static const char AT_RES_OK[];
	static const char AT_ERROR[];

	static const char AT_NOPROMPT[];
    static const char AT_NOECHO[] ;

    static const char MMI_DEL_IDXSMS[];
	static const char MMI_SMS_MT;
	static const char MMI_SMS_MO;
	
	static const char MMI_SMS_MS;
	static const char MMI_SMS_SIM;
	
	
	static const char MMI2_SMS_MS;
	static const char MMI2_SMS_SIM1;
	static const char MMI2_SMS_SIM2;
	static const char MMI2_SMS_SIM3;
	static const char MMI2_SMS_SIM4;
	
	static const char AT_REG_INMSG_INDEX[];
};

#endif // !defined(AFX_PHONE_H__786F02C2_342E_4EFF_98CC_7296FD0804D1__INCLUDED_)
