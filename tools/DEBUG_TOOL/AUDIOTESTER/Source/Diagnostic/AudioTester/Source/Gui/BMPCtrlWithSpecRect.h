// BMPCtrlWithSpecRect.h: interface for the CBMPCtrlWithSpecRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPCTRLWITHSPECRECT_H__98209940_EEEE_4F6E_8F45_3BD0F8236E5F__INCLUDED_)
#define AFX_BMPCTRLWITHSPECRECT_H__98209940_EEEE_4F6E_8F45_3BD0F8236E5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning( disable: 4786 )
#pragma  warning( push, 3 ) 
#include <vector>
#pragma  warning( pop )
#include "BitmapCtrl.h"
//#include "NVValueFile.h"
//#include "sci_types.h"

#include "Typedef.h"

class SpecialRect
{
public:
    enum AUDIO_PARA_TYPE
    {
        volume_ctrl,
        dl_digital_gain,
        ul_digital_gain,
        digital_sidetone_gain,
        dl_eq,
        ul_eq,
        agc_switch,
        pop,
        ng,
        aec,
        dl_fir,
        dl_fir_eq_coeff,
        ul_fir_eq_coeff,
        st_fir_eq_coeff,
        dl_settings,
        ul_settings,
		st_ng,
		volume_settings,
		arm_volume_setting,
        max_type = -1
    };

    typedef std::vector<SpecialRect*> AREA_OF_MAP;
public: 
    /* NOTE: use public here is only because they will be used for paint.
     just for performance reasons.
    */
    //rect of specific area,dynamic changed by mouse scroll
    CRect m_rectArea;
    
    //show values
    CString m_strOtherValue;
    
    // area name
    CString m_strName;

    //value unit
    CString m_strUnit;
    
    //default rect of specific area. without any change
    CRect m_rectDefaultArea;//

//	CString m_strOriFile; 
    
    //rect will be used to show values and value unit, will dynamic changed by mouse scroll
    CRect m_rectValueArea;

    // the type of audio parameter
    AUDIO_PARA_TYPE m_enumParaType;

    //whether or not write to phone immediately after the value is changed
    static BOOL m_bImmediateWrite;
public:
    SpecialRect(){ m_enumParaType = max_type;};
    virtual ~SpecialRect() {};
public:
    /* read and write file. */
	void WriteToFile ( CArchive& file )const;
	void ReadFromFile( CArchive& file );
    void FetchValue(AUDIO_MODE& _value)const;

    virtual void SetValue( AUDIO_STRUCT& value );

    // formate tool tips string
    virtual CString GetToolTips()const;

    //restore to default value,especially set the m_rectArea as m_rectDefaultArea
    //cause when we scroll the window, we will change the value of m_rectArea
    virtual void SetToDefault();

    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRect( *this ); }

    //pop up a dialog to modify related value 
    virtual int ClickAction(CWnd* pParent);

    //export value into NV editor format
//    virtual void ExportToNVEditor(std::vector<NVVALUE_TABLE_ENTRY>& vData )const;

    // update the rect position
    void SynchronizePosition(const SpecialRect& newPosition )
    {
        m_rectArea = newPosition.m_rectArea;
        m_rectDefaultArea = newPosition.m_rectDefaultArea;
        m_rectValueArea = newPosition.m_rectValueArea;
    }

protected:
	virtual void  Write( CArchive& file )const;
	virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;   
};

class SpecialRectOfGain : public SpecialRect
{
public:
    int m_iValue;
    SpecialRectOfGain();
    
public:
    //pop up a dialog to modify related value ,such as m_iValue
    virtual int ClickAction(CWnd* /*pParent*/){ return -1;};  
    
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfGain( *this ); }
    
    // format tool tips string
    virtual CString GetToolTips()const{ CString str;return str;};
    
    //export value into NV editor format
    //virtual void ExportToNVEditor(std::vector<NVVALUE_TABLE_ENTRY>& vData )const;
    
    //get DA/AD gain value string
    void FormatValueString( std::vector<CString>&vValue )const;
    
    //whenever user modify the m_iValue, re-caculate all related value, such as m_strOtherValue
    void ReCaculate();
    
    virtual void SetValue( AUDIO_STRUCT& value );
    
public:
    double m_dGain;
    double m_dDpAtt;
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
};

class SpecialRectOfVolumeCtrl : public SpecialRect
{
#define VOLUME_COUNT 9
#define BASE_SIDE_TONE_GAIN_COUNT 9
    
public:
    int m_iValue;
	int m_nLevel;
    SpecialRectOfVolumeCtrl();

public:
    //pop up a dialog to modify related value ,such as m_iValue
    virtual int ClickAction(CWnd* pParent);  
    
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfVolumeCtrl( *this ); }
    
    // format tool tips string
    virtual CString GetToolTips()const;
    
    //export value into NV editor format
    //virtual void ExportToNVEditor(std::vector<NVVALUE_TABLE_ENTRY>& vData )const;
    
    //get DA/AD gain value string
    void FormatValueString( std::vector<CString>&vValue )const;
    
    //whenever user modify the m_iValue, re-caculate all related value, such as m_strOtherValue
    void ReCaculate();

    virtual void SetValue( AUDIO_STRUCT& value );
    
private:
    //volume value
    static int m_iArrayVolume[VOLUME_COUNT];
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
};


class SpecialRectOfEQ : public SpecialRect
{
    
public:
    SpecialRectOfEQ();
    static double m_dMinXValue;
    static double m_dMaxXValue;

#define X_SCALE_COUNT 9
    
    typedef std::pair<double,double>FreqItem;
    //all frequence curve x value and y value
    std::vector<FreqItem>m_vAllFreqItem;
    std::vector<FreqItem>m_vAllBassFreqItem;
    std::vector<FreqItem>m_vAllTrebleFreqItem;
    std::vector<FreqItem>m_vAllMidFreqItem;
    
    // x axis value
    static double m_dXScale[X_SCALE_COUNT];

     // get the max y axis value 
//     double GetMaxYValue()const { return m_dMaxYValue;     }

public:
    //pop up a dialog to modify related value ,such as m_iValue
    virtual int ClickAction(CWnd* /*pParent*/){return 0;};  
    
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfEQ( *this ); }
    
    // format tool tips string
    //virtual CString GetToolTips()const;
    
    void CalculateFreqGraphyData( int nXScaleCount = 400 );
    
    //get all frequence curve x value and y value
    const std::vector<FreqItem>& GetFreqItem()const { return m_vAllFreqItem;  }
    const std::vector<FreqItem>& GetBassFreqItem()const { return m_vAllBassFreqItem;  }
    const std::vector<FreqItem>& GetTrebleFreqItem()const { return m_vAllTrebleFreqItem;  }
    const std::vector<FreqItem>& GetMidFreqItem()const { return m_vAllMidFreqItem;  }

    virtual void SetValue( AUDIO_STRUCT& value );

public:
    int* m_pSampleRate;
    
public:
    short	m_wBassAlpha;
    short	m_wBassBeta;
    short	m_wBassGain;
    short	m_wBassGama;
    double	m_dDbBass;
    double	m_dDbTreble;
    double	m_dDfBass;
    double	m_dDfTreble;
    double	m_dF0Bass;
    double	m_dF0Treble;
    double	m_dGainBass;
    double	m_dGainTreble;
    short	m_wTrebleAlpha;
    short	m_wTrebleBeta;
    short	m_wTrebleGain;
    short	m_wTrebleGama;
    short	m_wMidAlpha;
    short	m_wMidBeta;
    short	m_wMidGain;
    short	m_wMidGama;
    double	m_dDbMid;
    double	m_dDfMid;
    double	m_dF0Mid;
    double	m_dGainMid;
    
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
};

class SpecialRectOfAgc : public SpecialRect
{
public:
    SpecialRectOfAgc();
    virtual void SetValue( AUDIO_STRUCT& value );
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
    
public:
    int m_iValue;
	BOOL m_bAudio4;
	BOOL m_bAudio41;
	short m_nALCDPthreshold;
	short m_nALCcprsThreshold;
	short m_nALCcprsLimit;
	short m_nALCexpdThreshold;
	short m_nALCexpdLimit;

	//teana hu 2011.05.23
	short m_nALCcprsRatio;
	short m_nALCexpdRatio;
	//

};

//teana hu 2010.06.11
class SpecialRectOfEQMode : public SpecialRect
{
    
public:
    SpecialRectOfEQMode();
    static double m_dMinXValue;
    static double m_dMaxXValue;

#define X_SCALE_MODE_COUNT 14
    
    typedef std::pair<double,double>FreqItem;
    //all frequence curve x value and y value
    std::vector<FreqItem>m_vAllFreqItem;
    std::vector<FreqItem>m_vAllBassFreqItem;
    std::vector<FreqItem>m_vAllTrebleFreqItem;
    std::vector<FreqItem>m_vAllLcfFreqItem;
    
    // x axis value
    static double m_dXScale[X_SCALE_MODE_COUNT];

     // get the max y axis value 
//     double GetMaxYValue()const { return m_dMaxYValue;     }

public:
    //pop up a dialog to modify related value ,such as m_iValue
    virtual int ClickAction(CWnd* /*pParent*/){return 0;};  
    
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfEQMode( *this ); }
    
    // format tool tips string
    //virtual CString GetToolTips()const;
    
    void CalculateFreqGraphyData( int nXScaleCount = 22049 );
    
    //get all frequence curve x value and y value
    const std::vector<FreqItem>& GetFreqItem()const { return m_vAllFreqItem;  }
    const std::vector<FreqItem>& GetBassFreqItem()const { return m_vAllBassFreqItem;  }
    const std::vector<FreqItem>& GetTrebleFreqItem()const { return m_vAllTrebleFreqItem;  }
    const std::vector<FreqItem>& GetLcfFreqItem()const { return m_vAllLcfFreqItem;  }

    virtual void SetValue( EQ_MODE_NV_PARAM_T& _value );

public:
    int* m_pSampleRate;
    
public:
    short	m_wBassf0;
    short	m_wBassq;
    short	m_wBassboost;
    short	m_wBassgain;
	short	m_wTreblef0;
    short	m_wTrebleq;
    short	m_wTrebleboost;
    short	m_wTreblegain;
	short	m_wf1_g0;
    short	m_wf1_g1;
	short	m_wf1_fp;
    short	m_wf2_g0;
    short	m_wf2_g1;
    short	m_wf2_fp;
	float	m_fEQGain;
    
protected:
    virtual void Fetch(EQ_MODE_NV_PARAM_T& _value)const;
};
//

class CBMPCtrlWithSpecRect : public CBitmapCtrl  
{
#define MAX_TOOL_TIPS 2048
public:
    CBMPCtrlWithSpecRect();
    virtual ~CBMPCtrlWithSpecRect();
    void SetSpecialRects( std::vector<SpecialRect*>* pAreas ) { m_pvSpecialRects = pAreas;}
    void ResetState()
    {
        m_iIdxOfCurrentArea = -1;
        m_bIsHandCur = FALSE;
    }
protected:
        //{{AFX_MSG(CBitmapCtrl)            
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
protected: 
        virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

        //paint specific value , unit of every rect
        virtual void PaintSpecialValue(CDC* pDC) ;
        //BOOL OnToolTipNeedText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
        
        virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
        
        //test to see if the test point is in any rect, if yes, return its idx ,otherwise return -1
        int PointInWhichRect(const CPoint& ptTest )const;
protected:
        
        // all specific rects
        std::vector<SpecialRect*>* m_pvSpecialRects;
        //CButton m_edit;

        // the "hot" area when mouse move into it
        int m_iIdxOfCurrentArea;

        BOOL m_bIsHandCur;
        //the tool tips buffer
        char* m_pChToolTips;
};

class SpecialRectOfPop : public SpecialRect
{
public:
    SpecialRectOfPop();
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfPop( *this ); }
    
    //pop up a dialog to modify related value 
    virtual int ClickAction(CWnd* pParent);

    void SetValue( AUDIO_STRUCT& value );
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
    
public:
    BOOL m_bPopEnable;
    short m_nDaLimit;    
};


class SpecialRectOfNg : public SpecialRect
{
public:
    /*lint -save -e1401 */
    // all members will be init in SetValue function
    SpecialRectOfNg(){};
    /*lint -restore */

     virtual ~SpecialRectOfNg() {};
public:

    // virtual construct
     virtual SpecialRect* Clone()const { return new SpecialRectOfNg( *this ); }
    void SetValue( AUDIO_STRUCT& value );

     short	m_nNgSelect;
     
     short	m_nAlphaDistor;
     short	m_nBetaDistor;

     short	m_nUlNgClkAtt;
     short	m_nUlNgClkHoldc;
     short	m_nUlNgClkWpyyA;
     short	m_nUlNgClkWpyyN;
     short	m_nUlNgPlkAtt;
     short	m_nUlNgPlkHoldc;
     short	m_nUlNgPlkWpyyA;
     short	m_nUlNgPlkWpyyN;
     
     short	m_nDlNgClkAtt;
     short	m_nDlNgClkHoldc;
     short	m_nDlNgClkWpyyA;
     short	m_nDlNgClkWpyyN;
     short	m_nDlNgPlkAtt;
     short	m_nDlNgPlkHoldc;
     short	m_nDlNgPlkWpyyA;
     short	m_nDlNgPlkWpyyN;
     
protected:
	virtual void Read(CArchive &file);
	virtual void Write( CArchive& file )const;
    virtual void Fetch(AUDIO_MODE& _value)const;
};

class SpecialRectOfAec : public SpecialRect
{
public:
    SpecialRectOfAec();
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfAec( *this ); }
    
    void SetValue( AUDIO_STRUCT& value );
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
    
public:
    short m_nUlFirHpf;
    short m_nAecEnable;
    short m_nPdelay;
    short m_nDlRefHpf;
    short m_nDecorFilter;
    short m_nFirTaps;
    short m_nCoeffFrozen;
    short m_nAecFrozen;
    short m_nDtDectThreshold;
    short m_nDtNoiseFloor;
    short m_nStepSize;
    short m_nCoeffNormShift;
    short m_nSaCtrl;
    short m_nSendAttenuInDt;
    short m_nSendAttenuInRv;
    short m_nSendThreshold;
    short m_nR2dtThreshold;
    short m_nS2dtThreshold;
    short m_nRecvThreshold;
    short m_nBn40;

    short m_nSignalDelay;
    short m_nSaDelay;
	
	//teana hu 2011.02.14
	short m_nCGNGain;
	BOOL m_bAudio4;
};

class SpecialRectOfCoefficient : public SpecialRect
{
#define ROW_COUNT 26   
#define F1K_INDEX 7
#define COLUMN_COUNT 33
#define POWERDATA_WEIGHT_COUNT 17
    
#define X_SCALE_COUNT_FIR 26
    typedef std::pair<double,double>FreqItem;
    typedef struct POWER_DATA
    {
        double m_vPoValue[ROW_COUNT];
        int m_vPoSign[ROW_COUNT];
        POWER_DATA()
        {
            ZeroMemory( m_vPoValue, sizeof(double)*ROW_COUNT);
            ZeroMemory( m_vPoSign, sizeof(int)*ROW_COUNT);
        }
        
    };
    
public:
    static const double PI ;
    static const double m_dMinXValue;
    static const double m_dMaxXValue;
	BOOL m_bModified;
    POWER_DATA m_vPo;
    
    
    //out put, 17 阶FIR滤波器系数    
    double m_vCoefficients[ COLUMN_COUNT ];

    WORD m_dl_fir_eq_gain;
    
    // x axis value
    static int m_iXScale[X_SCALE_COUNT_FIR];

	CString m_strOriFile; 
public:
    SpecialRectOfCoefficient();
    virtual ~SpecialRectOfCoefficient() {};
public:
    virtual CString GetToolTips() const;
    virtual int ClickAction(CWnd* pParent);
    // calculate all 33 coefficents
    void CalculateCoefficients();

    void CalculatePowerData();
    
    //restore to default value,especily set the m_rectArea as m_rectDefaultArea
    //cause when we scroll the window, we will change the value of m_rectArea
    virtual void SetToDefault();
    
    //calculate all frequence curve x value and y value
    virtual void CalculateFreqGraphyData(int iXScaleCount = 500 );
	//
	void CalculateFreqGraphyDataEx(double dX[65], int iXScaleCount /*=65*/ );
    
    // get the max y axis value 
    double GetMaxYValue()const { return m_dMaxYValue;     }
    
    //get all frequence curve x value and y value
    const std::vector<FreqItem>& GetFreqItem()const { return m_vAllFreqItem;  }
    
    virtual void SetValue( AUDIO_STRUCT& value );
    
    //set a set of special data into coefficients
    void SetSpecialData();
protected:
    virtual void Read(CArchive &file);
    virtual void Write( CArchive& file )const;
    virtual void Fetch(AUDIO_MODE& _value)const;
    //the max y axis value 
    double m_dMaxYValue;
    
    //the const coeffice used when we caculate 33 coefficient
    static int m_iArrayFreq[ROW_COUNT][COLUMN_COUNT];
    
    static int m_iDy[ROW_COUNT][POWERDATA_WEIGHT_COUNT];
    
    
    //all frequence curve x value and y value
    std::vector<FreqItem>m_vAllFreqItem;    
};

class SpecialRectOfCoefficientEx : public SpecialRect
{
#define ROW_COUNT_EX 65   
#define COLUMN_COUNT_EX 65
#define X_SCALE_COUNT_FIR_EX 65
#define FIR_EQ_COUNT 33

    typedef std::pair<double,double>FreqItem;
    
public:
    static const double PI ;
    static const double m_dMinXValue;
    static const double m_dMaxXValue;
	static const int m_frequency_scale;
	BOOL m_bModified;
    double m_vPoValue[ROW_COUNT_EX];
    
    
    //out put, 65 阶FIR滤波器系数    
    int m_vCoefficients[ COLUMN_COUNT_EX ];
	
    WORD m_dl_fir_eq_gain;
    
    // x axis value
    static int m_iXScale[X_SCALE_COUNT_FIR_EX];
	
	CString m_strOriFile; 
public:
    SpecialRectOfCoefficientEx();
    virtual ~SpecialRectOfCoefficientEx() {};
public:
    virtual CString GetToolTips() const;
    virtual int ClickAction(CWnd* pParent);
    // calculate all 65 coefficents
    void CalculateCoefficients();
	
    void CalculatePowerData();

	void audio_fir_eq_65(
		int * frequency_set,//频率设置
		double * target_eq,//目标频响
		int  frequency_scale,//频率设置尺度
		int * fir_coef//fir_eq系数
		);
	
	void audio_calculate_responese_65(
		int * fir_coef, //fir_eq系数
		int * frequency_set,//频率设置
		int  frequency_scale,//频率设置尺度
		double * fir_gain//输出频响
	  );
    
    //restore to default value,especily set the m_rectArea as m_rectDefaultArea
    //cause when we scroll the window, we will change the value of m_rectArea
    virtual void SetToDefault();
    
    //calculate all frequence curve x value and y value
    virtual void CalculateFreqGraphyData(int iXScaleCount = 500 );
	//
	void CalculateFreqGraphyDataEx(double dX[65], int iXScaleCount /*=65*/ );
    
    // get the max y axis value 
    double GetMaxYValue()const { return m_dMaxYValue;     }
    
    //get all frequence curve x value and y value
    const std::vector<FreqItem>& GetFreqItem()const { return m_vAllFreqItem;  }
    
    virtual void SetValue( AUDIO_STRUCT& value );
    
    //set a set of special data into coefficients
    void SetSpecialData();
protected:
    virtual void Read(CArchive &file);
    virtual void Write( CArchive& file )const;
    virtual void Fetch(AUDIO_MODE& _value)const;
    //the max y axis value 
    double m_dMaxYValue;
    
    
    //all frequence curve x value and y value
    std::vector<FreqItem>m_vAllFreqItem;    
};


class SpecialRectOfDlSettings : public SpecialRect
{
public:
    SpecialRectOfDlSettings();
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfDlSettings( *this ); }
    
    //pop up a dialog to modify related value 
    virtual int ClickAction(CWnd* pParent);
    
    void SetValue( AUDIO_STRUCT& value );
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
    
public:
    short	m_dl_no_vad_cnt_thd;
    short m_dl_min_psne;
    short m_dl_max_temp_uamn;
    short m_dl_vad_thd;
    short m_dl_active_thd;
    short m_dl_noise_thd;
    short m_dl_max_psne;
    short m_dl_voise_burst;
    short m_dl_noise_tail;
    short m_dl_rfilter_delay;
    short m_dl_rfilter_tail;
    short m_dl_rfilter;
    short m_dl_dgain;
    short m_dl_sim_M;
    short m_dl_sim_fac;
    short m_dl_dac_limt;
    short m_dl_ns_factor;
    short m_dl_ns_limit;
    short m_dl_dis_snr_thd;
    short m_dl_dis_band_1k;
    short m_dl_dis_limit;
    short m_dl_drc_thd;
    short m_dl_drc_ratio;
    short m_dl_drc_dstep;
    short m_dl_drc_ustep;
    short m_dl_drc_cnt;

	short	m_dl_clarity_switch;

	short m_dl_vad_switch;
    short m_dl_complex_switch;
    short m_dl_noise_est_switch;
    short m_dl_nr_switch;
    
    short m_dl_dis_switch;
    short m_dl_amb_noise_est_switch;
    short m_dl_amb_eq_switch;
    short m_dl_acoustic_eq_switch;
    
    short m_dl_drc_switch;
    short m_dl_dac_limit_switch;
    short m_dl_hpf_set;
    short m_dl_lpf_set;

	short m_dl_ns_up_factor;
};

class SpecialRectOfUlSettings : public SpecialRect
{
public:
    SpecialRectOfUlSettings();
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfUlSettings( *this ); }
    
    //pop up a dialog to modify related value 
    virtual int ClickAction(CWnd* pParent);
    
    void SetValue( AUDIO_STRUCT& value );
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
    
public:
    short		m_ul_no_vad_cnt_thd;
    short m_ul_min_psne;
    short m_ul_max_temp_uamn;
    short m_ul_vad_thd;
    short m_ul_active_thd;
    short m_ul_noise_thd;
    short m_ul_max_psne;
    short m_ul_voise_burst;
    short m_ul_noise_tail;
    short m_ul_rfilter_delay;
    short m_ul_rfilter_tail;
    short m_ul_rfilter;
    short m_ul_dgain;
    short m_ul_sim_M;
    short m_ul_sim_fac;
    short m_ul_ns_factor;
    short m_ul_ns_limit;
    short m_ul_dis_snr_thd;
    short m_ul_dis_band_1k;
    short m_ul_dis_limit;
    short m_ul_drc_thd;
    short m_ul_drc_ratio;
    short m_ul_drc_dstep;
    short m_ul_drc_ustep;
    short m_ul_drc_cnt;

	short m_ul_clarity_switch;
    
    short m_aec_sa_in_select;
    short m_aec_sa_off;
    short m_aec_coeff_frozen;
    
    
    short m_ul_vad_switch;
    short m_ul_complex_switch;
    short m_ul_noise_est_switch;
    short m_ul_nr_switch;
    
    short m_ul_dis_switch;
    short m_ul_amb_noise_est_switch;
    short m_ul_amb_eq_switch;
    
    short m_ul_drc_switch;
    short m_ul_echo_residual_switch;
    short m_ul_hpf_set;
    short m_ul_lpf_set;

	short		m_ul_amb_attack;
	short		m_ul_amb_ndefault;
	short		m_ul_amb_release;
	short		m_ul_dac_limit;
	short		m_ul_echo_ns_limit;
	short		m_ul_ns_up_factor;

};

class SpecialRectOfStng : public SpecialRect
{
public:
    SpecialRectOfStng();
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfStng( *this ); }
    
    //pop up a dialog to modify related value 
    virtual int ClickAction(CWnd* pParent);
    
    void SetValue( AUDIO_STRUCT& value );
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
    
public:
	int m_st_ng_switch;
	int m_st_voice_threshold;
	int m_st_noise_threshold;
	int m_st_holdc;
	int m_st_att;
	int m_st_zcr_threshold;
	int m_st_zcr_ceil;
};



#define VOLUME_LEVEL 50
#define VOLUME_LEVEL_BASE 9
class SpecialRectOfVolume : public SpecialRect
{
public:
    SpecialRectOfVolume();
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfVolume( *this ); }
    
    //pop up a dialog to modify related value 
    virtual int ClickAction(CWnd* pParent);
    
    void SetValue( AUDIO_STRUCT& value );
	
	//teana hu 2012.02.01
	void SetDspCodecValue(AUDIO_NV_DSP_MODE_STRUCT_T& value);
	void FetchDspCodecValue(AUDIO_NV_DSP_MODE_STRUCT_T& _value);
	//
	//teana hu 2012.03.29
	void SetVolumeLevelFor6530(int nLevel6530){m_nLevel6530 = nLevel6530;}
	//
	//teana hu 2012.04.20
	void SetDspCodecInfo(BOOL bDspCodecInfo){m_bDspCodecInfo = bDspCodecInfo;}
	//
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;

    
public:
	int m_dl_st_gain[VOLUME_LEVEL];
	int m_dl_PGA0[VOLUME_LEVEL];
	int m_dl_digital_gain[VOLUME_LEVEL];
	//teana hu 2009.09.03
	int m_ul_digital_gain[VOLUME_LEVEL];
	//
	int m_aec_vol_comp_gain[VOLUME_LEVEL];
	int m_ul_PGA0[VOLUME_LEVEL];
	
	//teana hu 2011.05.04
	int m_nLevel;
	int m_nStep;
	//
	//teana hu 2012.03.29
	int m_nLevel6530;
	//
	
	//teana hu 2012.04.20
	BOOL m_bDspCodecInfo;
	//
	
	//teana hu 2012.06.12
	int m_nInnerPA;
	//
};

class SpecialRectOfArmVolume : public SpecialRect
{
public:
    SpecialRectOfArmVolume();
    // virtual construct
    virtual SpecialRect* Clone()const { return new SpecialRectOfArmVolume( *this ); }
    
    //pop up a dialog to modify related value 
    virtual int ClickAction(CWnd* pParent);
    
    void SetValue( AUDIO_STRUCT& value );
	
	//teana hu 2011.02.16
	void SetVolumeValue( VOLUME_STRUCT& value, int nIndex);
	void FetchVolume(VOLUME_MODE& _value, int nIndex);
	void SetArmVolumeLevel(int nLevel, int nIndex){m_narmVolumeLevel[nIndex] = nLevel;}
	int GetArmVolumeLevel(int nIndex){return m_narmVolumeLevel[nIndex];}
    
protected:
    virtual void  Write( CArchive& file )const;
    virtual void  Read( CArchive& file );
    virtual void Fetch(AUDIO_MODE& _value)const;
    
public:
	int m_narmVolumeLevel[AUDIO_ARM_APP_TYPE_MAX];
	int m_armVolumeMul[AUDIO_ARM_APP_TYPE_MAX][AUDIO_ARM_VOLUME_LEVEL];
	int m_armVolume[ARM_VOLUME_SIZE];
	int m_nCurType;
	BOOL m_bOldVer;
};

#endif // !defined(AFX_BMPCTRLWITHSPECRECT_H__98209940_EEEE_4F6E_8F45_3BD0F8236E5F__INCLUDED_)
