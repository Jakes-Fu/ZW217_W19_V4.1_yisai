#if !defined(AFX_CONVERTFUNC_H__7E67F528_8DD9_4996_B40A_40D5480C3F79__INCLUDED_)
#define AFX_CONVERTFUNC_H__7E67F528_8DD9_4996_B40A_40D5480C3F79__INCLUDED_

/*
void SetSampleRate( double sample_rate );
double GetSampleRate();
*/

#include "Typedef.h"


void df2alpha( double& df,short& alpha,double sample_rate );
void alpha2df( double& df,short& alpha,double sample_rate );
void f02beta( double& f0,short& beta,double sample_rate );
void beta2f0( double& f0,short& beta,double sample_rate );
void db2gama( double& db,short& gama );
void gama2db( double& db,short& gama );
void gain2rgain( double& gain,short& rgain );
void rgain2gain( double& gain,short& rgain );
void CheckGamaAndGain( double& db,double&gain,short& gama,short& rgain );

void EqConvert( double f0,double df,double db,double gain,short& alpha,short& beta,short& gama,short& rgain );
void EqReverseConvert( short alpha,short beta,short gama,short rgain,double& f0,double& df,double& db,double& gain );
/*
double CalcFrequencyResponse( 
                              double trenble_alpha,
                              double trenble_beta,
                              double trenble_gama,
                              double trenble_gain,
                              double bass_alpha,
                              double bass_beta,
                              double bass_gama,
                              double bass_gain,
                              double dFreq,
                              double& treble,
                              double& bass
                            );
*/
double CalcFrequencyResponse( 
                            double alpha,
                            double beta,
                            double gama,
                            double gain,
                            double dFreq,
                            double sample_rate
                          );

double CalcFrequencyResponse( 
                             double bass,
                             double treble,
                             double mid = 0
                             );

//teana hu 2010.06.11
double CalcFrequencyResponse( 
                            short A1,
                            short A2,
                            short A3,
                            short B1,
                            short B2,
                            short B3,
							double dFreq
                          );
//

//teana hu 2011.03.30
double CalcFrequencyResponse( 
							 short A1,
							 short A2,
							 short A3,
							 short B1,
							 short B2,
							 short B3,
							 double dFreq,
							 short sGain,
							 int nValue = 4096
							 );
//

void CalcDigGain( double gain,unsigned short& dgain );
void CalcDigGainRev( double& gain,unsigned short dgain, int nStep = 0 , int nCoeff = 0);
void CalcAgcSwitch( int nSwitch,unsigned short& agc_switch );
void CalcAgcSwitchRev( int& nSwitch,unsigned short agc_switch );
unsigned short CalcSampleRate( int nSampleRate );
int CalcSampleRateRev( unsigned short nSampleRate );

void VolumeConv( AUDIO_STRUCT* lpAudio );

typedef struct 
{
    short   B0;
    short   B1;
    short   B2;

    short   A0;
    short   A1;
    short   A2;
}IIR_FILTER_PARA_T; 

#define SAMPLE_FREQUENCY 44100

/*****************************************************************************/
//! \brief  Description:  This function is to caculate eq paras.
//! \author Author:  cherry.liu 
/*****************************************************************************/
BOOL Filter_CalcEqCoeffs(
	short  fo,                       //central freq in Hz      , nv
	short  q,                        //fo/df *512 , nv
	short  boostdB,                 //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
    short  basegaindB,              //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
	int  fs,                      //samplerate in Hz
	IIR_FILTER_PARA_T* filterPara  //out ptr,filter paras a,b
);

/*****************************************************************************/
//! \brief  Description:  This function is to caculate f1f1 filter paras.
//! \author Author:  cherry.liu 
/*****************************************************************************/
BOOL Filter_CalcF1f1Coeffs(
		short  f1_g0_dB,                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
		short  f1_g1_dB,                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
		short  f1_fp,                   // in Hz
		short  f2_g0_dB,                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
		short  f2_g1_dB,                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
		short  f2_fp,                   // in Hz
		int  fs,                      //samplerate in Hz
		IIR_FILTER_PARA_T* filterPara   //out ptr,filter paras a,b
		
);

//////////////////////////////////////////////////////////////////////////
//8800G///////////////////////////////////////////////////////////////////s

typedef enum//  3  para  types
{   
    FILTER_EQPARA_NORMAL_EQ = 0,
    FILTER_EQPARA_LOW_SHELVE ,
    FILTER_EQPARA_HIGH_SHELVE ,
    FILTER_EQPARA_MAX
}FILTER_EQPARA_TYPE_E;

union Filter_eq_input_para_union
{
    short q;          //FOR noraml eq
    short fo_next;   //for LS          
    short fo_last;   //for HS   
};

typedef struct 
{
    BOOL   isFilterOn;
    FILTER_EQPARA_TYPE_E eEqParaType;	
	short  fo;                     //central freq in Hz      , nv
	union Filter_eq_input_para_union unEqPara; //q OR fo_next OR  fo_last
	short  boostdB;                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
    short  basegaindB;             //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
}FILTER_EQ_CALC_PARA_T;


typedef enum
{
	FILTER_LCFPARA_F1F1 = 0,            
	FILTER_LCFPARA_BUTTERWORTH,                
	FILTER_LCFPARA_CHEBYSHEV,    
	FILTER_LCFPARA_BESSEL	,         
	FILTER_LCFPARA_ELLIPTICAL,                  
	FILTER_LCFPARA_MAX
}FILTER_LCFPARA_TYPE_E;


typedef struct 
{
    short f1_g0;  
    short f1_g1;  
    short f1_fp;  
    
    short f2_g0;  
    short f2_g1;    
    short f2_fp;  
}FILTER_LCF_F1F1_PARAM_T; 

union Filter_lcf_input_para_union
{
    short fp;        
    FILTER_LCF_F1F1_PARAM_T lcfPara;
};

typedef struct 
{
    BOOL   isFilterOn;
    FILTER_LCFPARA_TYPE_E eLcfParaType;	
	union Filter_lcf_input_para_union unlcfPara;
}FILTER_LCF_CALC_PARA_T;

/*****************************************************************************/
//! \brief  Description:  This function is to caculate eq paras.
//! \author Author:  cherry.liu 
/*****************************************************************************/
BOOL Filter_CalcEqCoeffsG(
	short  fo,                       //central freq in Hz      , nv
	short  q,                        //fo/df *512 , nv
	short  boostdB,                 //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
    short  basegaindB,              //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
	int  fs,                      //samplerate in Hz
	IIR_FILTER_PARA_T* filterPara  //out ptr,filter paras a,b
);

/*****************************************************************************/
//! \brief  Description:  This function is to caculate f1f1 filter paras.
//! \author Author:  cherry.liu 
/*****************************************************************************/
BOOL Filter_CalcF1f1CoeffsG(
		short  f1_g0_dB,                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
		short  f1_g1_dB,                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
		short  f1_fp,                   // in Hz
		short  f2_g0_dB,                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
		short  f2_g1_dB,                //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
		short  f2_fp,                   // in Hz
		int  fs,                      //samplerate in Hz
		IIR_FILTER_PARA_T* filterPara   //out ptr,filter paras a,b
		
);

/*****************************************************************************/
//! \brief  Description:  This function is to caculate eq paras and output hpf para set.
//! \author Author:  cherry.liu 
/*****************************************************************************/
BOOL Filter_CalcEQ(
    FILTER_EQ_CALC_PARA_T* eq_input_para_ptr,//IN
	int  fs,  //IN
    IIR_FILTER_PARA_T* filterPara ,//OUT
    short*   sGain//OUT
);

/*****************************************************************************/
//! \brief  Description:  This function is to caculate eq paras and output hpf para set.
//! \author Author:  cherry.liu 
//! this interface is for tool layer only
/*****************************************************************************/
BOOL Filter_CalcEQ_Para(
    BOOL  band_filter_on,
	short    band_fo,                
    short    band_q, 
	short    band_boostdB,  
    short    band_basegaindB, 
	int    fs,              
    IIR_FILTER_PARA_T* filterPara ,//OUT
    short*   sGain//OUT
);
/*****************************************************************************/
//! \brief  Description:  This function is to caculate f1f1 filter paras and output hpf para set.
//! \author Author:  cherry.liu 
//! this interface is for tool layer only
/*****************************************************************************/
BOOL Filter_CalcLCF_Para(
    FILTER_LCF_CALC_PARA_T* lcf_input_para_ptr,//IN
	int  fs,  //IN
    IIR_FILTER_PARA_T* filterPara ,//OUT
    short*   sGain//OUT
);

/*****************************************************************************/
//! \brief  Description:  This function is to caculate f1f1 filter paras and output hpf para set.
//! \author Author:  cherry.liu 
/*****************************************************************************/
BOOLEAN Filter_CalcLCF_Ex(
					   FILTER_LCF_CALC_PARA_T* lcf_input_para_ptr,//IN
					   short  lcf_gain,//IN ,scaled by 16384
					   int  fs,  //IN
					   IIR_FILTER_PARA_T* filterPara ,//OUT
					   short*   sGain//OUT
);

/*****************************************************************************/
//! \brief  Description:  This function is to caculate eq a/b paras.
//!    it is an enhancement or extention function for Filter_CalcEqCoeffs
//! \author Author:  cherry.liu 
/*****************************************************************************/
BOOLEAN Filter_CalcEqCoeffs_Ex(
							   short  fo,                    //central freq in Hz  
							   short  df,                    //band width in Hz  
							   short  boostdB,               //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
							   short  basegaindB,            //range -840:1:60;  (-84dB ~ 6dB ,step:0.1dB)
							   int  Fs,                    //samplerate in Hz
							   IIR_FILTER_PARA_T* filterPara,  //out ptr,filter paras a,b
							   short*   sGain//OUT
);

/*****************************************************************************/
//! \brief  Description:  This function is to caculate eq a/b paras.
//!    This function is used for record eq.
//! \author Author:  cherry.liu 
/*****************************************************************************/
BOOLEAN Filter_CalcRecordEq(
							BOOL  bandSW,                //SWITCH of current band
							short  fo,                      //central freq in Hz  
							short  df,                      //band width in Hz  
							short  boostdB,                 //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
							short  basegaindB,              //range -720:1:180;  (-72dB ~ 18dB ,step:0.1dB)
							int  Fs,                      //samplerate in Hz
							IIR_FILTER_PARA_T* filterPara,  //out ptr,filter paras a,b
							short*   sGain                  //OUT  scaled by 4096
);

#endif
