#ifndef __MSSIM_INI_FILE__
#define __MSSIM_INI_FILE__

#include <afxtempl.h>

#ifdef MSCODE_MOD
#include "dal_lcd.h"
#else
#include "tb_dal.h"
#endif

typedef struct KEY_INFO_tag
{
    int     nKeyCode;   // virtual key code
	int     nGPIO;      // send key down up seperately
    CRect   rcKeyRect;  // key rect
    CString strTip;     // tooltip for this key
	CString strKeyName;
}
KEY_INFO;

typedef CArray<KEY_INFO, KEY_INFO> KEYARRAY;


class CMSSimIniFile
{
public:
    int       Init(LPCTSTR lpszFilePath);
    COLORREF  GetMaskColor();
    BOOL      IsDualLCD();
    CRect     GetLCDRect();
    CRect     GetSubLCDRect();
    KEYARRAY *GetKeys();
    int       HitTest(POINT point, CRect *pRect=NULL);
	BOOL      GetKeyRectByCode(int nKeyCode,CRect *pRect);

	int       GetKeyCodeByName(LPCTSTR lpszKeyName,BOOL *bGPIO);
	int       GetTestNames(CStringArray &aTest);


    //@ Liu Kai 2004-7-1 CR10261
    LCD_INFO_T* GetLCDInfo( LCD_ID_E );

	void      Rotate(int nAngle,int nRcWidth, int nRcHeight);
	void      GetSlideOffset(int *pXOffset,int *pYOffset);

public:
    CMSSimIniFile();
    ~CMSSimIniFile();

    enum { MIF_SUCCESS, MIF_FILE_NOT_EXIST, MIF_INVALID_KEY_INFO, MIF_INVALID_LCD_INFO };
    enum { MIF_BTN_HANG, MIF_BTN_HANDFREE, MIF_BTN_POWER };

private:
    int   EnumKeys(KEYARRAY &aKeyInfo);
    BOOL  GetRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect &rect);
    BOOL  GetKey(LPCTSTR lpszKey, KEY_INFO &keyInfo);
    int   GetVirtualKeyCode(LPCTSTR lpszKeyCode);
	void  GetHotkey( LPCTSTR lpszKeyCode,CString& strHotKey );

    //@ Liu Kai 2004-7-1 CR10261
    void LoadLCDInfo();
	void RotateCoordinate(LONG &x,LONG &y,int nAngle,int nRcWidth, int nRcHeight);
	void RotateWidthHeight(int &w,int &h,int nAngle);
	void RotateRect(CRect& rect,int nAngle,int nRcWidth, int nRcHeight);


private:
    CString  m_strFilePath;
    BOOL     m_bDualLCD;
    CRect    m_rcLCD;
    CRect    m_rcSubLCD;
    KEYARRAY m_aKeyInfo;

	CStringArray m_aTest;
	int          m_nSlideXOffset;
	int          m_nSlideYOffset;

    //@ Liu Kai 2004-7-1 CR10261
    LCD_INFO_T m_lcdInfo;
    LCD_INFO_T m_sublcdInfo;

    // Mask color configuration
    static const CString SECTION_COLOR;
    static const CString COLOR_ENTRY_MASK;

    // LCD configuration
    static const CString SECTION_LCD;
    static const CString SECTION_SUBLCD;
    static const CString LCD_ENTRY_RECT;

    //@ Liu Kai 2004-7-1 CR10261
    static const CString LCD_ENTRY_R_BITMASK;
    static const CString LCD_ENTRY_G_BITMASK;
    static const CString LCD_ENTRY_B_BITMASK;
    static const CString LCD_ENTRY_BITS_PER_PIXEL;
    static const CString LCD_ENTRY_CONTRAST_MIN;
    static const CString LCD_ENTRY_CONTRAST_MAX;
    static const CString LCD_ENTRY_CONTRAST_DEFAULT;

    static const UINT DEFAULT_LCD_R_BITMASK;
    static const UINT DEFAULT_LCD_G_BITMASK;
    static const UINT DEFAULT_LCD_B_BITMASK;
    static const UINT DEFAULT_LCD_BITS_PER_PIXEL;
    static const UINT DEFAULT_LCD_CONTRAST_MIN;
    static const UINT DEFAULT_LCD_CONTRAST_MAX;
    static const UINT DEFAULT_LCD_CONTRAST_DEFAULT;

    //@ Liu Kai 2004-7-8 CR10625
    // Change default sub lcd settings
    static const UINT DEFAULT_SUBLCD_R_BITMASK;
    static const UINT DEFAULT_SUBLCD_G_BITMASK;
    static const UINT DEFAULT_SUBLCD_B_BITMASK;
    static const UINT DEFAULT_SUBLCD_BITS_PER_PIXEL;
    

    // Keypad layout configuration
    static const CString SECTION_KEYPAD;  
    static const CString KEYPAD_ENTRY_KEYS;

    // Key configuration
    static const CString KEY_ENTRY_NAME;
    static const CString KEY_ENTRY_CODE;  
    static const CString KEY_ENTRY_RECT;  
	static const CString KEY_ENTRY_GPIO;  

	// Slide configuration
    static const CString SECTION_SLIDE;  
    static const CString X_OFFSET_KEY;
	static const CString Y_OFFSET_KEY;
};

#endif // __MSSIM_INI_FILE__
