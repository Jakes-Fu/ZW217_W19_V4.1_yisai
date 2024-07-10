#include "stdafx.h"

#include "MSSimIniFile.h"

#ifdef MSCODE_MOD
#include "dal_keypad.h"
#include "keypad.h"
#else
#include "tb_dal.h"
#endif

// Mask color configuration
const CString CMSSimIniFile::SECTION_COLOR      = "COLOR";
const CString CMSSimIniFile::COLOR_ENTRY_MASK   = "mask";

// LCD configuration
const CString CMSSimIniFile::SECTION_LCD        = "LCD";
const CString CMSSimIniFile::SECTION_SUBLCD     = "SUB-LCD";
const CString CMSSimIniFile::LCD_ENTRY_RECT     = "rect";

//@ Liu Kai 2004-7-1 CR10261
const CString CMSSimIniFile::LCD_ENTRY_R_BITMASK            = "R_BitMask";
const CString CMSSimIniFile::LCD_ENTRY_G_BITMASK            = "G_BitMask";
const CString CMSSimIniFile::LCD_ENTRY_B_BITMASK            = "B_BitMask";
const CString CMSSimIniFile::LCD_ENTRY_BITS_PER_PIXEL       = "Bits_per_pixel";
const CString CMSSimIniFile::LCD_ENTRY_CONTRAST_MIN         = "Contrast_Min";
const CString CMSSimIniFile::LCD_ENTRY_CONTRAST_MAX         = "Contrast_Max";
const CString CMSSimIniFile::LCD_ENTRY_CONTRAST_DEFAULT     = "Contrast_Default";

const UINT CMSSimIniFile::DEFAULT_LCD_R_BITMASK            = 0xf800;
const UINT CMSSimIniFile::DEFAULT_LCD_G_BITMASK            = 0x07e0;
const UINT CMSSimIniFile::DEFAULT_LCD_B_BITMASK            = 0x001f;
const UINT CMSSimIniFile::DEFAULT_LCD_BITS_PER_PIXEL       = 16;
const UINT CMSSimIniFile::DEFAULT_LCD_CONTRAST_MIN         = 0;
const UINT CMSSimIniFile::DEFAULT_LCD_CONTRAST_MAX         = 0;
const UINT CMSSimIniFile::DEFAULT_LCD_CONTRAST_DEFAULT     = 0;

//@ Liu Kai 2004-7-8 CR10625
// Change default sub lcd settings
const UINT CMSSimIniFile::DEFAULT_SUBLCD_R_BITMASK             = 0xf00;
const UINT CMSSimIniFile::DEFAULT_SUBLCD_G_BITMASK             = 0xf0;
const UINT CMSSimIniFile::DEFAULT_SUBLCD_B_BITMASK             = 0xf;
const UINT CMSSimIniFile::DEFAULT_SUBLCD_BITS_PER_PIXEL        = 16;

// Keypad layout configuration
const CString CMSSimIniFile::SECTION_KEYPAD     = "KEYPAD";
const CString CMSSimIniFile::KEYPAD_ENTRY_KEYS  = "keys";

// Key configuration
// Its sections is determined by [keypad layout] above
const CString CMSSimIniFile::KEY_ENTRY_NAME     = "name";
const CString CMSSimIniFile::KEY_ENTRY_CODE     = "code";
const CString CMSSimIniFile::KEY_ENTRY_RECT     = "rect";
const CString CMSSimIniFile::KEY_ENTRY_GPIO     = "gpio";

const CString CMSSimIniFile::SECTION_SLIDE  = "SLIDE";  
const CString CMSSimIniFile::X_OFFSET_KEY   = "x_offset";
const CString CMSSimIniFile::Y_OFFSET_KEY   = "y_offset";

// Virtual key code
// [
typedef struct KEY_CODE_MAP_tag
{
    char *pszKeyCode;
    int   nKeyCode;
	char *pszHotKey;
}
KEY_CODE_MAP;

#ifndef SCI_VK_TOUCHPANEL
#define SCI_VK_TOUCHPANEL	0
#endif

static const KEY_CODE_MAP g_aKeyMap[] = 
    {
		{ "SCI_VK_0",		SCI_VK_0,		"0" },
		{ "SCI_VK_1",		SCI_VK_1,		"1" },
		{ "SCI_VK_2",		SCI_VK_2,		"2" },
		{ "SCI_VK_3",		SCI_VK_3,		"3" },
		{ "SCI_VK_4",		SCI_VK_4,		"4" },
		{ "SCI_VK_5",		SCI_VK_5,		"5" },
		{ "SCI_VK_6",		SCI_VK_6,		"6" },
		{ "SCI_VK_7",		SCI_VK_7,		"7" },
		{ "SCI_VK_8",		SCI_VK_8,		"8" },
		{ "SCI_VK_9",		SCI_VK_9,		"9" },
		{ "SCI_VK_STAR",	SCI_VK_STAR,	"*" },
		{ "SCI_VK_POUND",	SCI_VK_POUND,	"#" },
		{ "SCI_VK_CANCEL",	SCI_VK_CANCEL,	"" },
		{ "SCI_VK_CALL",	SCI_VK_CALL,	"F7" },
		{ "SCI_VK_POWER",	SCI_VK_POWER,	"F8" },
		{ "SCI_VK_UP",		SCI_VK_UP,		"Up" },
		{ "SCI_VK_DOWN",	SCI_VK_DOWN,	"Down" }, 
		{ "SCI_VK_LEFT",	SCI_VK_LEFT,	"Left" },
		{ "SCI_VK_RIGHT",	SCI_VK_RIGHT,	"Right" },
		{ "SCI_VK_EXT1",	SCI_VK_EXT1,	"F5/Return" },
		{ "SCI_VK_EXT2",	SCI_VK_EXT2,	"F6/Back" },
		{ "SCI_VK_EXT3",	SCI_VK_EXT3,	"Page up" },
		{ "SCI_VK_EXT4",	SCI_VK_EXT4,	"Page down" },
		{ "SCI_VK_EXT5",	SCI_VK_EXT5,	"F4" },
        { "SCI_GPIO_SIG1",	SCI_GPIO_SIG1,	"" },
        { "SCI_GPIO_SIG2",	SCI_GPIO_SIG2,	"" },
        { "SCI_VK_TOUCHPANEL",	SCI_VK_TOUCHPANEL,""},
		{ "SCI_VK_SIDE_UP", SCI_VK_SIDE_UP, "Page up"},
		{ "SCI_VK_SIDE_DOWN", SCI_VK_SIDE_DOWN, "Page down"},
		{ "SCI_VK_Camera", SCI_VK_CAMERA, ""}
};
// ]


CMSSimIniFile::CMSSimIniFile()
{
    m_bDualLCD = FALSE;
	m_nSlideXOffset  = 0;
	m_nSlideYOffset  = 150;

    //@ Liu Kai 2004-7-1 CR10261
    memset( &m_lcdInfo,0,sizeof( LCD_INFO_T ) );
    memset( &m_sublcdInfo,0,sizeof( LCD_INFO_T ) );
}

CMSSimIniFile::~CMSSimIniFile()
{
}

int CMSSimIniFile::Init(LPCTSTR lpszFilePath)
{
    int nResult = MIF_SUCCESS;

    m_strFilePath = lpszFilePath;

    HANDLE hFile = ::CreateFile( lpszFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL );
    if( INVALID_HANDLE_VALUE != hFile )
    {
        CloseHandle( hFile );
    }

    if (INVALID_HANDLE_VALUE == hFile)
    {
        nResult = MIF_FILE_NOT_EXIST;
    }
    else if (!GetRect(SECTION_LCD, LCD_ENTRY_RECT, m_rcLCD) )
    {
        nResult = MIF_INVALID_LCD_INFO;
    }
    else
    {
        if (0 == EnumKeys(m_aKeyInfo))
        {
            nResult = MIF_INVALID_KEY_INFO; 
        }
    }

    if ( GetRect(SECTION_SUBLCD, LCD_ENTRY_RECT, m_rcSubLCD) )
    {
        m_bDualLCD = TRUE;
    }
    else
    {
        m_bDualLCD = FALSE;
    }

    //@ Liu Kai 2004-7-1 CR10261
    LoadLCDInfo();

	m_nSlideXOffset = GetPrivateProfileInt(SECTION_SLIDE,
                                X_OFFSET_KEY,
                                0,
                                lpszFilePath);

	m_nSlideYOffset = GetPrivateProfileInt(SECTION_SLIDE,
                                Y_OFFSET_KEY,
                                150,
                                lpszFilePath);



    return nResult;
}

COLORREF CMSSimIniFile::GetMaskColor()
{
    // Format of mask color:
    // mask = 0x00XXXXXX
    //            R G B

    return GetPrivateProfileInt(SECTION_COLOR,
                                COLOR_ENTRY_MASK,
                                0x00FFFFFF,
                                m_strFilePath);
}

BOOL CMSSimIniFile::IsDualLCD()
{
    return m_bDualLCD;
}

BOOL CMSSimIniFile::GetRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect &rect)
{
    // Format of rectangle information:
    // rect = left, top, width, height

    const int  MAX_RECT    = 100;
    const char SEPARATOR[] = " ,";

    char szLCDInfo[MAX_RECT] = {0};
    char *pszLeft;
    char *pszTop;
    char *pszWidth;
    char *pszHeight;

    GetPrivateProfileString(lpszSection,
                            lpszEntry,
                            "",
                            szLCDInfo,
                            MAX_RECT,
                            m_strFilePath);

    pszLeft   = strtok(szLCDInfo, SEPARATOR);
    pszTop    = strtok(NULL, SEPARATOR);
    pszWidth  = strtok(NULL, SEPARATOR);
    pszHeight = strtok(NULL, SEPARATOR);

    if ((NULL == pszLeft) || (NULL == pszTop) || (NULL == pszWidth) || (NULL == pszHeight))
    {
        return FALSE;
    }

    rect.left   = atoi(pszLeft);
    rect.top    = atoi(pszTop);
    rect.right  = rect.left + atoi(pszWidth);
    rect.bottom = rect.top + atoi(pszHeight);
    return TRUE;
}

CRect CMSSimIniFile::GetLCDRect()
{
    return m_rcLCD;
}

CRect CMSSimIniFile::GetSubLCDRect()
{
    return m_rcSubLCD;
}

int CMSSimIniFile::EnumKeys(KEYARRAY &aKeyInfo)
{
    const int  MAX_KEYS    = 4*1024;
    const char SEPARATOR[] = " ,";

    char szKeys[MAX_KEYS] = {0};
    char *pszKey = NULL;
    int  nKeyCount = 0;

    KEY_INFO keyInfo;
    CString  strError;

    // Clear up the aKeyInfo first
    aKeyInfo.RemoveAll();
	m_aTest.RemoveAll();

    GetPrivateProfileString(SECTION_KEYPAD,
                            KEYPAD_ENTRY_KEYS,
                            "",
                            szKeys,
                            MAX_KEYS,
                            m_strFilePath);

    CStringArray aKeys;
    pszKey = strtok(szKeys, SEPARATOR);
    while (pszKey != NULL)
    {
        aKeys.Add(pszKey);
        pszKey = strtok(NULL, SEPARATOR);
    }


    for (int i = 0; i < aKeys.GetSize(); i++)
    {
        if (GetKey(aKeys[i], keyInfo)) // Note: strtok() inside !
        {
            aKeyInfo.Add(keyInfo);
            nKeyCount++;
			if(keyInfo.strKeyName.Find(_T("TEST_"))== 0)
			{
				CString strTest = keyInfo.strKeyName;
				strTest.Delete(0,5);
				m_aTest.Add(strTest);
			}
        }
        else // Invalid keys
        {
            strError.Format("Invalid key information: %s", aKeys[i]);
            AfxMessageBox(strError);
        }
    }

    return nKeyCount; // Valid key count
}

KEYARRAY * CMSSimIniFile::GetKeys()
{
    return &m_aKeyInfo;
}

int CMSSimIniFile::GetVirtualKeyCode(LPCTSTR lpszKeyCode)
{
    int nKeyCount = sizeof(g_aKeyMap) / sizeof(KEY_CODE_MAP);
    
    for ( int i = 0; i < nKeyCount; i++ )
    {
        if ( 0 == strcmp(g_aKeyMap[i].pszKeyCode, lpszKeyCode) )
        {
            return g_aKeyMap[i].nKeyCode;
        }
    }

    return -1; // -1 indicates invalid key code
}

BOOL CMSSimIniFile::GetKey(LPCTSTR lpszKey, KEY_INFO &keyInfo)
{
    BOOL bResult = FALSE;

    const int MAX_KEY_NAME = 20;
    const int MAX_KEY_CODE = 20;

    int   nKeyCode;
	int   nGPIO;
    CRect rcKey;
    char  szKeyName[MAX_KEY_NAME] = {0};
    char  szKeyCode[MAX_KEY_CODE] = {0};

    // Get the key name (used for tooltip, can be empty)
    GetPrivateProfileString(lpszKey,
                            KEY_ENTRY_NAME,
                            "",
                            szKeyName,
                            MAX_KEY_NAME,
                            m_strFilePath);

    // Get the virtual key code (can't be empty or invalid)
    GetPrivateProfileString(lpszKey,
                            KEY_ENTRY_CODE,
                            "",
                            szKeyCode,
                            MAX_KEY_CODE,
                            m_strFilePath);

	nGPIO = GetPrivateProfileInt(lpszKey,
							KEY_ENTRY_GPIO,
							0,
							m_strFilePath);
    nKeyCode = GetVirtualKeyCode(szKeyCode);

	// Get hot key name
	CString strHotKey;
	GetHotkey( szKeyCode,strHotKey );
	if( strHotKey.IsEmpty() )
	{
		strHotKey = szKeyCode;
	}
	else
	{
		CString strKey = strHotKey;
		strHotKey.Format( "%s Hotkey:%s",szKeyCode,strKey );
	}
    
    //@ LiuKai 2005-3-16 CR19708
    if (nKeyCode == -1)
    {
        // Can not find the key in key map,maybe a 
        // user-defined key
        nKeyCode = atoi( szKeyCode );
    }
    
    // Get the key rectange in the handset image (can't be empty or invalid)
    if (GetRect(lpszKey, KEY_ENTRY_RECT, rcKey))
    {
        keyInfo.nKeyCode  = nKeyCode;
        keyInfo.rcKeyRect = rcKey;
        keyInfo.strTip    = strHotKey;
		keyInfo.strKeyName= szKeyName;
		keyInfo.nGPIO = nGPIO;
        
        bResult = TRUE;
    }
    
    return bResult;
}

int CMSSimIniFile::HitTest(POINT point, CRect *pRect/*=NULL*/)
{
    int nKeyCount = m_aKeyInfo.GetSize();
    for (int i = 0; i < nKeyCount; i++)
    {
        if (m_aKeyInfo[i].rcKeyRect.PtInRect(point))
        {
			if(pRect != NULL)
			{
				pRect->CopyRect(&m_aKeyInfo[i].rcKeyRect);
			}
            return m_aKeyInfo[i].nKeyCode;
        }
    }

    return -1; // -1 indicates invalid key
}

BOOL CMSSimIniFile::GetKeyRectByCode(int nKeyCode,CRect *pRect)
{
    int nKeyCount = m_aKeyInfo.GetSize();
    for (int i = 0; i < nKeyCount; i++)
    {
        if (m_aKeyInfo[i].nKeyCode == nKeyCode)
        {
			if(pRect != NULL)
			{
				pRect->CopyRect(&m_aKeyInfo[i].rcKeyRect);
			}
            return TRUE;
        }
    }
	return FALSE;
}

//@ Liu Kai 2004-7-1 CR10261
void CMSSimIniFile::LoadLCDInfo()
{
    // Get main lcd info
    m_lcdInfo.r_bitmask = 
        GetPrivateProfileInt( 
                             SECTION_LCD,
                             LCD_ENTRY_R_BITMASK,
                             DEFAULT_LCD_R_BITMASK,
                             m_strFilePath 
                            );

    
    m_lcdInfo.g_bitmask = 
        GetPrivateProfileInt( 
                             SECTION_LCD,
                             LCD_ENTRY_G_BITMASK,
                             DEFAULT_LCD_G_BITMASK,
                             m_strFilePath 
                            );

    
    m_lcdInfo.b_bitmask = 
        GetPrivateProfileInt( 
                             SECTION_LCD,
                             LCD_ENTRY_B_BITMASK,
                             DEFAULT_LCD_B_BITMASK,
                             m_strFilePath 
                            );


    m_lcdInfo.bits_per_pixel = 
        GetPrivateProfileInt( 
                             SECTION_LCD,
                             LCD_ENTRY_BITS_PER_PIXEL,
                             DEFAULT_LCD_BITS_PER_PIXEL,
                             m_strFilePath 
                            );


    m_lcdInfo.contrast_min = 
        GetPrivateProfileInt( 
                             SECTION_LCD,
                             LCD_ENTRY_CONTRAST_MIN,
                             DEFAULT_LCD_CONTRAST_MIN,
                             m_strFilePath 
                            );


    m_lcdInfo.contrast_max = 
        GetPrivateProfileInt( 
                             SECTION_LCD,
                             LCD_ENTRY_CONTRAST_MAX,
                             DEFAULT_LCD_CONTRAST_MAX,
                             m_strFilePath 
                            );


    m_lcdInfo.contrast_defaut = 
        GetPrivateProfileInt( 
                             SECTION_LCD,
                             LCD_ENTRY_CONTRAST_DEFAULT,
                             DEFAULT_LCD_CONTRAST_DEFAULT,
                             m_strFilePath 
                            );

    m_lcdInfo.lcd_width = m_rcLCD.Width();
    m_lcdInfo.lcd_height = m_rcLCD.Height();

    // Get sub lcd info
    m_sublcdInfo.r_bitmask = 
        GetPrivateProfileInt( 
                             SECTION_SUBLCD,
                             LCD_ENTRY_R_BITMASK,
                             DEFAULT_SUBLCD_R_BITMASK,
                             m_strFilePath 
                            );

    
    m_sublcdInfo.g_bitmask = 
        GetPrivateProfileInt( 
                             SECTION_SUBLCD,
                             LCD_ENTRY_G_BITMASK,
                             DEFAULT_SUBLCD_G_BITMASK,
                             m_strFilePath 
                            );

    
    m_sublcdInfo.b_bitmask = 
        GetPrivateProfileInt( 
                             SECTION_SUBLCD,
                             LCD_ENTRY_B_BITMASK,
                             DEFAULT_SUBLCD_B_BITMASK,
                             m_strFilePath 
                            );


    m_sublcdInfo.bits_per_pixel = 
        GetPrivateProfileInt( 
                             SECTION_SUBLCD,
                             LCD_ENTRY_BITS_PER_PIXEL,
                             DEFAULT_SUBLCD_BITS_PER_PIXEL,
                             m_strFilePath 
                            );


    m_sublcdInfo.contrast_min = 
        GetPrivateProfileInt( 
                             SECTION_SUBLCD,
                             LCD_ENTRY_CONTRAST_MIN,
                             DEFAULT_LCD_CONTRAST_MIN,
                             m_strFilePath 
                            );


    m_sublcdInfo.contrast_max = 
        GetPrivateProfileInt( 
                             SECTION_SUBLCD,
                             LCD_ENTRY_CONTRAST_MAX,
                             DEFAULT_LCD_CONTRAST_MAX,
                             m_strFilePath 
                            );


    m_sublcdInfo.contrast_defaut = 
        GetPrivateProfileInt( 
                             SECTION_SUBLCD,
                             LCD_ENTRY_CONTRAST_DEFAULT,
                             DEFAULT_LCD_CONTRAST_DEFAULT,
                             m_strFilePath 
                            );

    m_sublcdInfo.lcd_width = m_rcSubLCD.Width();
    m_sublcdInfo.lcd_height = m_rcSubLCD.Height();
}

//@ Liu Kai 2004-7-1 CR10261
LCD_INFO_T* CMSSimIniFile::GetLCDInfo( LCD_ID_E lcdid)
{
    if( MAIN_LCD_ID == lcdid )
    {
        return &m_lcdInfo;
    }
    else if( SUB_LCD_ID == lcdid )
    {
        return &m_sublcdInfo;
    }
    else
    {
        return NULL;
    }
}

void  CMSSimIniFile::GetHotkey( LPCTSTR lpszKeyCode,CString& strHotKey )
{
    int nKeyCount = sizeof(g_aKeyMap) / sizeof(KEY_CODE_MAP);
	strHotKey.Empty();
    
    for ( int i = 0; i < nKeyCount; i++ )
    {
        if ( 0 == strcmp(g_aKeyMap[i].pszKeyCode, lpszKeyCode) )
        {
            
			strHotKey = g_aKeyMap[i].pszHotKey;
			return;
        }
    }
}

int  CMSSimIniFile::GetKeyCodeByName(LPCTSTR lpszKeyName,BOOL *bGPIO)
{
    int nKeyCount = m_aKeyInfo.GetSize();
    for (int i = 0; i < nKeyCount; i++)
    {
        if (m_aKeyInfo[i].strKeyName.CompareNoCase(lpszKeyName)==0)
        {
			if(bGPIO != NULL)
			{
				*bGPIO = m_aKeyInfo[i].nGPIO;
			}
			return m_aKeyInfo[i].nKeyCode;
        }
    }
	return -1;
}

int  CMSSimIniFile::GetTestNames(CStringArray &aTest)
{
	aTest.RemoveAll();
	int nCount = m_aTest.GetSize();
	if(nCount != 0)
	{
		aTest.Copy(m_aTest);
	}
	return nCount;
}

void CMSSimIniFile::Rotate(int nAngle,int nRcWidth, int nRcHeight)
{
	// Main LCD
    RotateRect(m_rcLCD,nAngle,nRcWidth,nRcHeight);
	// Sub LCD
	RotateRect(m_rcSubLCD,nAngle,nRcWidth,nRcHeight);

	// Main LCD Info
	int w = m_lcdInfo.lcd_width;
	int h = m_lcdInfo.lcd_height;
	RotateWidthHeight(w,h,nAngle);
	m_lcdInfo.lcd_width=(uint16)w;
	m_lcdInfo.lcd_height=(uint16)h;

	// Sub LCD Info
	w = m_sublcdInfo.lcd_width;
	h = m_sublcdInfo.lcd_height;
	RotateWidthHeight(w,h,nAngle);
	m_sublcdInfo.lcd_width=(uint16)w;
	m_sublcdInfo.lcd_height=(uint16)h;


	// Key Info
    int nKeyCount = m_aKeyInfo.GetSize();
    for (int i = 0; i < nKeyCount; i++)
    {
		//RotateCoordinate(m_aKeyInfo[i].rcKeyRect.left,m_aKeyInfo[i].rcKeyRect.top,nAngle,nRcWidth,nRcHeight);
		//RotateCoordinate(m_aKeyInfo[i].rcKeyRect.right,m_aKeyInfo[i].rcKeyRect.bottom,nAngle,nRcWidth,nRcHeight);
		RotateRect(m_aKeyInfo[i].rcKeyRect,nAngle,nRcWidth,nRcHeight);

    }
 
}
void CMSSimIniFile::RotateCoordinate(LONG &x,LONG &y,int nAngle,int nRcWidth, int nRcHeight)
{

	int x1=x;
	int y1=y;
	if( nAngle == 90 )
	{
		x = nRcHeight - 1 - y1;
		y = x1;
	}
	else if( nAngle == 180 )
	{
		x = nRcWidth - 1 - x1;
		y = nRcHeight - 1 - y1;
	}
	else if( nAngle == 270 )
	{
		
		x = y1;
		y = nRcWidth - 1 - x1;
	}
}

void CMSSimIniFile::RotateWidthHeight(int &w,int &h,int nAngle)
{
	int w1=w;
	int h1=h;
	if( nAngle == 90 || nAngle == 270 )
	{
		w = h1;
		h = w1;
	}
	else if( nAngle == 180 )
	{
		w = w1;
		h = h1;
	}

}

void CMSSimIniFile::RotateRect(CRect& rect,int nAngle,int nRcWidth, int nRcHeight)
{
	int x1=rect.left;
	int y1=rect.top;
	int x2=rect.right;
	int y2=rect.bottom;
	
	if( nAngle == 90 )
	{
		rect.left = nRcHeight-y2;
		rect.top  = x1;
		rect.right = nRcHeight-y1;
		rect.bottom = x2; 
	}
	else if( nAngle == 180 )
	{
		rect.left = x1;
		rect.top  = nRcHeight-y2;
		rect.right = x2;
		rect.bottom = nRcHeight-y1; 
	}
	else if( nAngle == 270 )
	{		
		rect.left = y1;
		rect.top  = nRcWidth-x2;
		rect.right = y2;
		rect.bottom = nRcWidth-x1; 
	}

}

void  CMSSimIniFile::GetSlideOffset(int *pXOffset,int *pYOffset)
{
	if(pXOffset!=NULL)
	{
		*pXOffset = m_nSlideXOffset;
	}
	if(pYOffset!=NULL)
	{
		*pYOffset = m_nSlideYOffset;
	}
}