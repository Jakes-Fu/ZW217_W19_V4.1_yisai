// DiagPackage.cpp: implementation of the CDiagPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMPackage.h"
#include "Global.h"



#define BM_MIN_PAC_SIZE (6)  // cmd_type(2) + len(2) + crc(2)
#define CRC_LEN         (2)
#define BM_LOG_TYPE     (0xFF) 
#define HDLC_ESCAPE      0x7D
#define CRC_16_L_OK      0x0


unsigned short crc_16_l_calc(unsigned short crc, char* buf_ptr,unsigned int len)
{
    unsigned int i;
    
    while (len--!=0)
    {
        for(i = 0x80; i !=0 ; i = i>>1)
        {
            if((crc & 0x8000) !=0 )
            {
                crc = (unsigned short)(crc << 1) ;
                crc = (unsigned short)(crc ^ 0x1021);
            }
            else
            {
                crc = (unsigned short)(crc << 1) ;
            } 
            
            if((*buf_ptr & i) != 0 )
            {
                crc = (unsigned short)(crc ^ 0x1021);
            }
        }
        buf_ptr++;
    }
	
	return (crc);	
}
/*
unsigned short frm_chk( unsigned short *src, int len,int nEndian )
{
    unsigned int sum = 0;
    unsigned short SourceValue, DestValue;
    unsigned short lowSourceValue, hiSourceValue;
	
    // Get sum value of the source.
    while (len > 1)
    {     
        SourceValue = *src++;
        if( nEndian == 1 )
		{
			// Big endian
			DestValue   = 0;      
			lowSourceValue = (unsigned short)(( SourceValue & 0xFF00 ) >> 8);
			hiSourceValue = (unsigned short)(( SourceValue & 0x00FF ) << 8);
			DestValue = (unsigned short)(lowSourceValue | hiSourceValue);
		}
		else
		{
			// Little endian
			DestValue = SourceValue;
		}
        sum += DestValue;	
        len -= 2;
    }
	
    if (len == 1)
    {
        sum += *( (unsigned char *) src );
    }
	
    sum = (sum >> 16) + (sum & 0x0FFFF);
    sum += (sum >> 16);
	
    return (unsigned short)(~sum);   
}
*/

unsigned int frm_chk(unsigned int crc, unsigned short *src, int len,int nEndian, bool bLast )
{
	//unsigned short DestValue=0;
	UNUSED_ALWAYS( nEndian );
	unsigned char * pSrc = (unsigned char *)src;

	/* Get sum value of the source.*/
	while (len > 1)
	{     
		//SourceValue = *src++;
		//if( nEndian == 1 )
		//{
			// Big endian
			//DestValue   = 0;      
			//lowSourceValue = (unsigned short)(( SourceValue & 0xFF00 ) >> 8);
			//hiSourceValue = (unsigned short)(( SourceValue & 0x00FF ) << 8);
			//DestValue = (unsigned short)(lowSourceValue | hiSourceValue);
			
			//crc += (unsigned short)( ((*(pSrc)) << 8) | (*(pSrc+1)));
			crc += (unsigned short)((*(pSrc)) << 8) ;
			crc += (unsigned short)(*(pSrc+1));
			pSrc += 2;
			//src++;

		//}
		//else
		//{
			// Little endian
		//	crc += *src++;
		//}
		//crc += DestValue;	
		len -= 2;
	}

	if(bLast)
	{
		if (len == 1)
		{
			//crc += *( (unsigned char *) src );
			crc += *pSrc;
		}

		crc = (crc >> 16) + (crc & 0x0FFFF);
		crc += (crc >> 16);

		return (unsigned short)(~crc); 
	}
	else
	{
		return crc;
	}  
}


/*
void Free_PRT_BUFF( void* lpBuff )
{
	if( NULL != lpBuff )
	{
		PRT_BUFF* lpData = (PRT_BUFF*)lpBuff;
		if( 0 == InterlockedDecrement( (long*)&lpData->ref_count ) )
		{
			free( lpData );
		}
	}
}

PRT_BUFF* Alloc_PRT_BUFF( unsigned int size )
{
	PRT_BUFF* lpBuff = (PRT_BUFF*)malloc( sizeof( PRT_BUFF ) + size );
	if( NULL != lpBuff )
	{
		lpBuff->type = 0;
		lpBuff->ref_count = 1;
		lpBuff->size = size;
		lpBuff->lpData = (unsigned char*)(lpBuff + 1);
		lpBuff->free_prt = Free_PRT_BUFF;
	}
	
	return lpBuff;
}
*/



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBMPkg::CBMPkg()
{
	m_bBigEndian = TRUE;

	m_nDataSize = 0;
	m_Status = BPS_NoValidData;
	m_nDataLen = 0;


	InitializeCriticalSection( &m_Section );

	m_PackageInfo.nTotalPackages = 0;
	m_PackageInfo.nValidPackages = 0;
	m_PackageInfo.bInvalidLen = 0;
	m_PackageInfo.bNoTail = 0;
	m_PackageInfo.bCrcError = 0;
	m_PackageInfo.Reserved = 0;

	m_lpPkg = NULL;

	m_nMinPackageSize = BM_MIN_PAC_SIZE;
	
	for(int i =0; i< MAX_PACKET_NUM; i++)
	{
		m_Packages[i] = new BYTE[MAX_BM_PKG_SIZE+4];
	}

	m_nPacketCount = 0;
	m_nCurPacketIndex = 0;
	m_bCrc = TRUE;
	m_bDisableTransCode = FALSE;

	m_bEnableCheckBaudCrc = FALSE;
	m_nCheckBaudCrc = -1;
    ZeroMemory(m_szFDLVer,MAX_PATH);
}

CBMPkg::~CBMPkg()
{
	DeleteCriticalSection( &m_Section );
	for(int i =0; i< MAX_PACKET_NUM; i++)
	{
		SAFE_DELETE_ARRAY(m_Packages[i]);
	}
}
void CBMPkg::RsvDataByTransCode(void* lpBuffer,unsigned int uSize)
{
	unsigned char* lpBuff = (unsigned char*)lpBuffer;
	unsigned char* lpBuffEnd = lpBuff + uSize;
	while( lpBuff < lpBuffEnd )
	{
		switch( m_Status )
		{
		case BPS_NoValidData:
			{
				// No invalidate data now,find 0x7E in the lpbuffer
				while( lpBuff < lpBuffEnd )
				{
					if( HDLC_HEADER == *lpBuff++ )
					{
						m_Status = BPS_Header;
						break;
					}
				}			
				break;
			}
		case BPS_Header:
			{
				// ignore continuous 0x7e
				while( lpBuff < lpBuffEnd )
				{
					if( HDLC_HEADER != *lpBuff )
					{
						m_Status = BPS_Unpacking;
						break;
					}
					lpBuff++;
				}
				break;
			}
		case BPS_Unpacking:
			{
				// Check data one by one till another 0x7e is found
				while( lpBuff < lpBuffEnd )
				{
					if( *lpBuff == HDLC_ESCAPE )
					{
						lpBuff++;
						// the escape char
						if( lpBuff == lpBuffEnd )
						{
							// The last character in buffer
							// Record the status and quit
							m_Status = BPS_Escape;
							break;
						}
						else
						{
							m_RingBuf[m_nDataSize++] = (unsigned char)(*lpBuff ^ 0x20);
							lpBuff++;
						}
					}
					else if( *lpBuff == HDLC_HEADER )
					{
						// Check the package
						// If the data size is less than size of the BM_HEADER,
						// discard this package
                        if (m_nDataSize >= m_nMinPackageSize )
                        {
							// If this is not a valid package,remain the last 0x7E
							// in the buffer						
							if( AddPackage() )
							{
								lpBuff++;
								
								// remain the last 0x7E always
								m_nDataSize = 0;
								m_Status = BPS_Header;									
								break;
							
							}
							else
							{
								//The following code is fix code by liang.zhao 2013-8-8
								m_nDataSize = 0;
								m_Status = BPS_Header;
								lpBuff++;
								break;
								// end
							}
						}  	   
						// Reset internal status
						m_nDataSize = 0;
						m_Status = BPS_NoValidData;
						break;	   
					}
					else
					{
						m_RingBuf[m_nDataSize++] = *lpBuff;
						lpBuff++;
					}
					if( m_nDataSize == MAX_BM_PKG_SIZE )
					{
						// The packet is too long
						m_nDataSize = 0;
						m_Status = BPS_NoValidData;
						m_PackageInfo.bNoTail = 1;
						break;
					}
				}
				break;
			}
		case BPS_Escape:
			{
				m_RingBuf[m_nDataSize++] = (unsigned char)(*lpBuff ^ 0x20);
				lpBuff++;

				if( m_nDataSize == MAX_BM_PKG_SIZE )
				{
					// The packet is too long
					m_nDataSize = 0;
					m_Status = BPS_NoValidData;
					m_PackageInfo.bNoTail = 1;
				}
				else
				{
					m_Status = BPS_Unpacking;
				}
				break;
			}
		}
	}
}

void CBMPkg::RsvDataByNotTransCode(void* lpBuffer,unsigned int uSize)
{
	unsigned char* lpBuff = (unsigned char*)lpBuffer;
	unsigned char* lpBuffEnd = lpBuff + uSize;

	while( lpBuff < lpBuffEnd )
	{
		switch( m_Status )
		{
		case BPS_NoValidData:
			{
				// No invalidate data now,find 0x7E in the lpbuffer
				while( lpBuff < lpBuffEnd )
				{
					if( HDLC_HEADER == *lpBuff++ )
					{
						m_Status = BPS_Header;
						break;
					}
				}			
				break;
			}
		case BPS_Header:
			{
				// ignore continuous 0x7e
				m_nDataLen = 0;
				while( lpBuff < lpBuffEnd )
				{
					if( HDLC_HEADER != *lpBuff )
					{
						if(lpBuff+4 < lpBuffEnd)
						{
							memcpy(m_RingBuf+m_nDataSize,lpBuff,4);
							m_nDataSize += 4;
							lpBuff += 4;
							m_nDataLen = *(unsigned short*)(m_RingBuf+m_nDataSize-2);
							if( m_bBigEndian )
							{
								CONVERT_SHORT( m_nDataLen,m_nDataLen );
							}
							if (m_nDataLen)
							{
								m_Status = BPS_RSVDATA;
							}
							else
							{
								m_Status = BPS_LOCRC;
							}
						}
						else
						{
							m_Status = BPS_LOTYPE;
						}
						
						break;
					}
					++lpBuff;
				}
				
				break;
			}
		case BPS_LOTYPE:
			{
				m_RingBuf[m_nDataSize++] = *lpBuff;
				++lpBuff;
				m_Status = BPS_HITYPE;
				break;
			}
		case BPS_HITYPE:
			{
				m_RingBuf[m_nDataSize++] = *lpBuff;
				++lpBuff;
				m_Status = BPS_LOLEN;
				break;
			}
		case BPS_LOLEN:
			{
				m_RingBuf[m_nDataSize++] = *lpBuff;
				++lpBuff;
				m_Status = BPS_HILEN;
				break;
			}
		case BPS_HILEN:
			{
				m_RingBuf[m_nDataSize++] = *lpBuff;
				++lpBuff;
				m_nDataLen = *(unsigned short*)(m_RingBuf+m_nDataSize-2);
				
				if( m_bBigEndian )
				{
					CONVERT_SHORT( m_nDataLen,m_nDataLen );
				}
				if (m_nDataLen)
				{
					m_Status = BPS_RSVDATA;
				}
				else
				{
					m_Status = BPS_LOCRC;
				}
				break;
			}
		case BPS_RSVDATA:
			{
				if (lpBuff+m_nDataLen > lpBuffEnd)
				{
					int nLeft = lpBuffEnd - lpBuff;
					memcpy(m_RingBuf+m_nDataSize,lpBuff,nLeft);
					m_nDataSize += nLeft;
					lpBuff += nLeft;
					m_Status = BPS_RSVDATA;
					m_nDataLen -= (unsigned short)nLeft;
				}
				else
				{
					memcpy(m_RingBuf+m_nDataSize,lpBuff,m_nDataLen);
					m_nDataSize += m_nDataLen;
					lpBuff += m_nDataLen;
					m_Status = BPS_LOCRC;
					m_nDataLen = 0;
				}
				break;
			}
		case BPS_LOCRC:
			{
				m_RingBuf[m_nDataSize++] = *lpBuff;
				++lpBuff;
				m_Status = BPS_HICRC;
				break;
			}
		case BPS_HICRC:
			{
				m_RingBuf[m_nDataSize++] = *lpBuff;
				++lpBuff;
				if (m_nDataSize >= m_nMinPackageSize )
				{
					AddPackage();
					lpBuff++;
					m_nDataSize = 0;
					m_Status = BPS_Header;									
					break;
				}  	   
				// Reset internal status
				m_nDataSize = 0;
				m_Status = BPS_NoValidData;
				break;
			}
		}
	}
}
unsigned int CBMPkg::Append( void* lpBuffer,unsigned int uSize )
{
	if( NULL == lpBuffer || 0 == uSize )
	{
		return m_nPacketCount;
	}

	memset( &m_PackageInfo,0,sizeof( m_PackageInfo ) );
	
	LockPakcageList( true );
	
	if (m_bDisableTransCode)//Not trans code
	{
		RsvDataByNotTransCode(lpBuffer,uSize);
	}
	else
	{
		RsvDataByTransCode(lpBuffer,uSize);
	}
	LockPakcageList( false );
	if( m_nDataSize )
	{
		// If there are some data in the internal buffer, it means a incomplete package received
		// We just save the situation here, the caller will decide it is a error or not
		m_PackageInfo.bNoTail = 1;
	}
	return m_nPacketCount;
}

void CBMPkg::Clear()
{
	LockPakcageList( true );
	// Remove all packages
	m_nPacketCount = 0;
	m_nCurPacketIndex = 0;

	// Remove all internal data
	m_nDataSize = 0;
	m_nDataLen  = 0;
	m_Status = BPS_NoValidData;
	m_PackageInfo.nTotalPackages = 0;
	m_PackageInfo.nValidPackages = 0;
	m_PackageInfo.bInvalidLen = 0;
	m_PackageInfo.bNoTail = 0;
	m_PackageInfo.bCrcError = 0;
	LockPakcageList( false );

}

void * CBMPkg::GetPackages()
{
	void *pPacket = NULL;
	LockPakcageList( true );
	if(m_nPacketCount > 0)
	{
		int nCur = 0;
		if(m_nCurPacketIndex == 0)
		{
			nCur = MAX_PACKET_NUM - 1;
		}
		else
		{
			nCur = m_nCurPacketIndex - 1;
		}

		pPacket = (void*)m_Packages[nCur];
		m_nPacketCount --;
	}
	LockPakcageList( false );

	return pPacket;
}


void CBMPkg::LockPakcageList( bool bLock )
{
	if( bLock )
	{
		EnterCriticalSection( &m_Section );
	}
	else
	{
		LeaveCriticalSection( &m_Section );
	}
}

int CBMPkg::Package( BM_PACKAGE* lpPkg,void* lpOutBuf,int* lpOutSize, BOOL bTransCode/* = TRUE*/)
{
	if( NULL == lpOutBuf || NULL == lpOutSize )
	{
		return -1;
	}

	if( NULL == lpPkg )
	{
		*lpOutSize = 0;
		return 0;
	}

	int nSize = 0;
	unsigned char* lpData = (unsigned char *)lpOutBuf;

	if( lpPkg->header.type == HDLC_HEADER )
	{
		// CheckBaud command is special
		nSize = lpPkg->header.len;	
		memset( lpData,HDLC_HEADER,nSize );
	}
	else
	{
		BOOL bOdd = FALSE;
		if( lpPkg->header.len % 2 )
		{
			bOdd = TRUE;
		}

		unsigned short type = lpPkg->header.type;
		unsigned short len = (unsigned short)(lpPkg->header.len + bOdd);

		if( m_bBigEndian )
		{
			CONVERT_SHORT( type,type );
			CONVERT_SHORT( len,len );
		}

		// crc
		unsigned short crc = 0;		
		char cZero[2]  = {0};
		if(m_bCrc)
		{
			crc = crc_16_l_calc(crc,(char*)&type,2);
			crc = crc_16_l_calc(crc,(char*)&len,2);	
			if(lpPkg->header.len > 0 )
			{
				crc = crc_16_l_calc(crc,(char*)lpPkg->data,lpPkg->header.len);
				if(bOdd)
				{
					crc = crc_16_l_calc(crc,cZero,1);
				}
			}		
		}
		else
		{
			unsigned int nCrc = 0;
			nCrc = frm_chk(nCrc,&type,2,m_bBigEndian,false);			
			if(lpPkg->header.len == 0)
			{
				nCrc = frm_chk(nCrc,&len,2,m_bBigEndian,true);
			}
			else
			{
				nCrc = frm_chk(nCrc,&len,2,m_bBigEndian,false);

				if(bOdd)
				{
					nCrc = frm_chk(nCrc,(unsigned short*)lpPkg->data,lpPkg->header.len-1,m_bBigEndian,false);
					cZero[0] = ((char*)lpPkg->data)[lpPkg->header.len-1];
					nCrc = frm_chk(nCrc,(unsigned short*)cZero,2,m_bBigEndian,true);
				}
				else
				{
					nCrc = frm_chk(nCrc,(unsigned short*)lpPkg->data,lpPkg->header.len,m_bBigEndian,true);
				}
			}
			crc = (unsigned short)nCrc;
		}

		if(m_bBigEndian)
		{
			CONVERT_SHORT( crc,crc );
		}


		nSize = 0;
		
		// Package header
		lpData[nSize++] = HDLC_HEADER;
		
		// type
		Copy( lpData,nSize,(unsigned char*)&type,2,bTransCode );

		// length
		Copy( lpData,nSize,(unsigned char*)&len,2,bTransCode );

		// data
		if(lpPkg->header.len > 0)
		{
			if (bTransCode)
			{
				Copy( lpData,nSize,(unsigned char*)lpPkg->data,lpPkg->header.len );
			}
			else
			{
				memcpy(lpData+nSize,lpPkg->data,lpPkg->header.len);
				nSize += lpPkg->header.len;
			}
			// odd
			if(bOdd)
			{
				lpData[nSize++] = 0;
			}
		}

		// crc
		Copy( lpData,nSize,(unsigned char*)&crc,sizeof(crc),bTransCode );

		// Packet tail
		lpData[nSize++] = HDLC_HEADER;
	}

	*lpOutSize = nSize;
	return 0;
}

bool CBMPkg::AddPackage()
{
	// Check package length and convert endian if needed
	bool bRet = CheckLength();
	if( bRet )
	{
		// The length is valid
		BM_HEADER * pHDR = (BM_HEADER *)m_RingBuf;
		if(pHDR->type != BM_LOG_TYPE) // remove log packet
		{			
			memcpy(m_Packages[m_nCurPacketIndex],m_RingBuf,m_nDataSize);

			m_nCurPacketIndex = (m_nCurPacketIndex+1) % MAX_PACKET_NUM;

			m_nPacketCount ++;
			if( m_nPacketCount > MAX_PACKET_NUM )
			{
				m_nPacketCount = MAX_PACKET_NUM;
			}

			m_PackageInfo.nValidPackages++;
		}
	}
	m_PackageInfo.nTotalPackages++;

	return bRet;
}

bool CBMPkg::CheckLength()
{
	BM_HEADER* lpPkg = (BM_HEADER*)m_RingBuf;
	unsigned short len = lpPkg->len;
	unsigned short type = lpPkg->type;
	unsigned short expect_len = (unsigned short)(m_nDataSize - CRC_LEN - sizeof( lpPkg->type ) - sizeof( lpPkg->len ));

	if( m_bBigEndian )
	{
		CONVERT_SHORT( len,len );
		CONVERT_SHORT( type,type );
	}
	
	if( len != expect_len )
	{
		// Invalid length
		m_PackageInfo.bInvalidLen = 1;
		return false;
	}

	if(m_bEnableCheckBaudCrc && type == BSL_REP_VER)
	{
		do 
		{	
            int nLength = len >MAX_PATH ? MAX_PATH : len;
            memcpy(m_szFDLVer,m_RingBuf+4,nLength);
			unsigned short crc = 0;
			if( CRC_16_L_OK == crc_16_l_calc(crc,(char*)m_RingBuf, m_nDataSize) )
			{
				m_nCheckBaudCrc = TRUE; // CRC
				m_bCrc = TRUE;
				break;
			}

			unsigned int nCrc =0;
			if( 0 == frm_chk(nCrc,(unsigned short *)m_RingBuf, m_nDataSize,m_bBigEndian,true) )
			{
				m_nCheckBaudCrc = FALSE; // CheckSum
				m_bCrc = FALSE;
				break;
			}

		} while(0);

		if(m_nCheckBaudCrc == -1)
		{
			m_PackageInfo.bCrcError = 1;
			return false;
		}

	}
	else
	{
		// Check crc
		if( m_bCrc )
		{
			unsigned short crc = 0;
			if( CRC_16_L_OK != crc_16_l_calc(crc,(char*)m_RingBuf, m_nDataSize) )
			{
				// CRC error
				m_PackageInfo.bCrcError = 1;
				return false;
			}
		}
		else
		{
			unsigned int nCrc =0;
			if( 0 != frm_chk(nCrc,(unsigned short *)m_RingBuf, m_nDataSize,m_bBigEndian,true) )
			{
				// CRC error
				m_PackageInfo.bCrcError = 1;
				return 0;
			}
		}
	}


	m_nDataSize -= CRC_LEN;

	if(m_bBigEndian)
	{
		CONVERT_SHORT( lpPkg->type,lpPkg->type );
		CONVERT_SHORT( lpPkg->len,lpPkg->len );
	}

	return true;
}



void CBMPkg::Copy( unsigned char* lpData,int& nStartPos,const unsigned char* lpCopy,const int nSize,BOOL bTransCode /*= TRUE*/ )
{
	for( int i=0;i<nSize;i++ )
	{
		if (!bTransCode)
		{
			lpData[nStartPos++] = lpCopy[i];
			continue;
		}
		if( HDLC_HEADER == lpCopy[i])
		{
			lpData[nStartPos++] = HDLC_ESCAPE;
			lpData[nStartPos++] = 0x5E;
		}
		else if(HDLC_ESCAPE == lpCopy[i] )
		{
			lpData[nStartPos++] = HDLC_ESCAPE;
			lpData[nStartPos++] = 0x5D;
		}
		else
		{
			lpData[nStartPos++] = lpCopy[i];
		}
	}
}


PACKAGE_INFO & CBMPkg::GetError()
{
	return m_PackageInfo;
}

void CBMPkg::SetCrc(BOOL bCrc)
{
	m_bCrc = bCrc;
}

void CBMPkg::EnableCheckBaudCrc(BOOL bEanble)
{
	m_bEnableCheckBaudCrc = bEanble;
	m_nCheckBaudCrc = -1;
}
int CBMPkg::GetCheckBaudCrcType()
{
	return m_nCheckBaudCrc;
}
BOOL CBMPkg::IsNeed2ndEnumPort()
{
    return stricmp(m_szFDLVer,"SPRD4:AutoD") == 0 ? FALSE : TRUE;// "SPRD4:AutoD" --uboot don't launchFDL
    
}