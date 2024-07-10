
#include "chip_drv_trc.h"
#include "math.h"
#include "lcd_cfg.h"
#include "dal_lcd.h"
#include "lcm_drvapi.h"
#include "tpc_algorithm.h"
#include "tp_cfg.h"

#define __LOG_OPEN
#ifdef __LOG_OPEN
#define __LOG    SCI_TraceLow
#else
#define __LOG 
#endif

char log_buffer[128]={0};
/**---------------------------------------------------------------------------*
 **                         Macro Define                                       *
 **---------------------------------------------------------------------------*/
#define POINT_INVALID   0
#define POINT_SINGLE    1
#define POINT_DOUBLE    2

LOCAL TP_DP_CFG_PTR s_tp_dp_cfg_ptr;
FLOAT_TYPE g_watch_data[WATCH_ARRAY_SIZE] = {0};

/*****************************************************************************/
/*Function: 开根号处理                                                       */
/*入口参数：被开方数，长整型                                                 */
/*出口参数：开方结果，整型                                                   */
/*****************************************************************************/
uint32 _sqrt16(unsigned long M)
{
  unsigned int N;
  unsigned char  i;
  unsigned long tmp, ttp;   // 结果、循环计数
  if (M == 0)               // 被开方数，开方结果也为0
      return 0;
  N = 0;
  tmp = (M >> 30);          // 获取最高位：B[m-1]
  M <<= 2;
  if (tmp > 1)              // 最高位为1
  {
      N ++;                 // 结果当前位为1，否则为默认的0
      tmp -= N;
  }
  for (i=15; i>0; i--)      // 求剩余的15位
  {
      N <<= 1;              // 左移一位
      tmp <<= 2;
      tmp += (M >> 30);     // 假设
      ttp = N;
      ttp = (ttp<<1)+1;
      M <<= 2;
      if (tmp >= ttp)       // 假设成立
      {
          tmp -= ttp;
          N ++;
      }
  }
  return N;
}

/*****************************************************************************/
//  Description:  Change pixels to length in LCM panel. 
//  Author     :  lichd
//	Note       :  Example pixels-->mm
/*****************************************************************************/ 
FLOAT_TYPE pixel2len(FLOAT_TYPE pixels, FLOAT_TYPE pixel_max, FLOAT_TYPE len_max)
{
    return ((pixels/pixel_max)*len_max);
}

/*****************************************************************************/
//  Description:  Change length to pixels in LCM panel. 
//  Author     :  lichd
//	Note       :  Example mm-->pexels
/*****************************************************************************/ 
uint32     len2pixel(FLOAT_TYPE len, FLOAT_TYPE pixel_max, FLOAT_TYPE len_max)
{
    return (uint32)((len/len_max)*pixel_max);
}

/*****************************************************************************/
//  Description:  Change adc data to ohm ,
//                according to reference voltage and reference resistance 
//  Author     :  lichd
//	Note       :  
/*****************************************************************************/
FLOAT_TYPE adc2ohm(FLOAT_TYPE adc_data, FLOAT_TYPE adc_ref, FLOAT_TYPE r_ref)
{
    //U_adc/Ux_adc = (r+r_ref)/r_ref
    return ((adc_ref/adc_data-1)*r_ref);
}

/*****************************************************************************/
//  Description:  Calculate the parallel of R1 and R2 
//  Author     :  lichd
//	Note       :  
/*****************************************************************************/ 
FLOAT_TYPE ParallelResistor(FLOAT_TYPE R1,FLOAT_TYPE R2)
{
    return ((R1*R2)/(R1+R2));
}

/*****************************************************************************/
//  Description:  Calculate delta_x 
//  Author     :  lichd
//	Note       :  
/*****************************************************************************/ 
FLOAT_TYPE F1_delta_x(FLOAT_TYPE delta_Rx, 
                      FLOAT_TYPE Kx, 
                      FLOAT_TYPE Ky, 
                      FLOAT_TYPE Rtouch)
{
  //          (delta_Rx*(Kx+Ky)+sqrt((delta_Rx*(Kx+Ky))^2) + 8Rtouch*delta_Rx*Kx^2)
  //delta_x = ---------------------------------------------------------------------
  //                                            (2*Kx^2)
 
  FLOAT_TYPE temp0 = delta_Rx*(Kx+Ky);
  FLOAT_TYPE temp1 = Kx*Kx;   //Kx^2  
  
  FLOAT_TYPE numerator  =  temp0 + _sqrtf(temp0*temp0+8*Rtouch*delta_Rx*temp1);
  FLOAT_TYPE denominator=  2*temp1;
  
  return ((numerator)/(denominator));

}

/*****************************************************************************/
//  Description:  Calculate delta_y 
//  Author     :  lichd
//	Note       :  
/*****************************************************************************/ 
FLOAT_TYPE F1_delta_y(FLOAT_TYPE delta_Ry, 
                      FLOAT_TYPE Kx, 
                      FLOAT_TYPE Ky, 
                      FLOAT_TYPE Rtouch)
{
  //          (delta_Ry*(Kx+Ky)+sqrt((delta_Ry*(Kx+Ky))^2) + 8Rtouch*delta_Ry*Ky^2)
  //delta_y = ---------------------------------------------------------------------
  //                                            (2*Ky^2)
  
  FLOAT_TYPE temp0 = delta_Ry*(Kx+Ky);
  FLOAT_TYPE temp1 = Ky*Ky;   //Ky^2  
  
  FLOAT_TYPE numerator  =  temp0 + _sqrtf(temp0*temp0+8*Rtouch*delta_Ry*temp1);
  FLOAT_TYPE denominator=  2*temp1;

  return ((numerator)/(denominator));
}

/*****************************************************************************/
//  Description:  Calculate Rs 
//  Author     :  lichd
//	Note       :  
/*****************************************************************************/
FLOAT_TYPE F2(FLOAT_TYPE X, FLOAT_TYPE Y,
              FLOAT_TYPE Kx, FLOAT_TYPE Ky)
{
    FLOAT_TYPE Rs;
    FLOAT_TYPE Rt;
    FLOAT_TYPE R1,R2,R3,R4,Ra,Rb,Rc;
    FLOAT_TYPE Rx0,Ry0, Rtouch;
    CALIBRATED_DATA_T* calibrated_data_ptr= TPDRV_GetCalibratedData();

    Rx0 = calibrated_data_ptr->Rx0;
    Ry0 = calibrated_data_ptr->Ry0;
    Rtouch=calibrated_data_ptr->Rt0;
    
//step2:
    R1 = Kx*X;
    R2 = Rx0-R1;
    R3 = Ky*Y;
    R4 = Ry0-R3;
//step3:    
    Rt = R1*R2 + (R1+R2)*Rtouch;
    Ra = Rt/R2;
    Rb = Rt/R1;
    Rc = Rt/Rtouch;
//step4:
    Rs = ParallelResistor(Rc , (ParallelResistor(R3,Ra) + ParallelResistor(R4,Rb)));

    return (Rs);
}


FLOAT_TYPE TPC_GetRtouch(TPC_SEQ_DATA_PTR tpc_seq_ptr) 
{
    FLOAT_TYPE TP_WIDTH = s_tp_dp_cfg_ptr->tp_width;
    FLOAT_TYPE TP_HEIGHT= s_tp_dp_cfg_ptr->tp_height;
    FLOAT_TYPE X0 = tpc_seq_ptr[TPC_SEQ_X].data;
    FLOAT_TYPE Y0 = tpc_seq_ptr[TPC_SEQ_Y].data;
    FLOAT_TYPE Rx0= tpc_seq_ptr[TPC_SEQ_RX0].data;
    FLOAT_TYPE Ry0= tpc_seq_ptr[TPC_SEQ_RY0].data;
    FLOAT_TYPE Rt_x= tpc_seq_ptr[TPC_SEQ_RT_X].data;
    FLOAT_TYPE Rt_y= tpc_seq_ptr[TPC_SEQ_RT_Y].data;
    
    FLOAT_TYPE Kx , Ky; 
    FLOAT_TYPE Rtouch_x,Rtouch_y;

    Kx = Rx0/TP_WIDTH;
    Ky = Ry0/TP_HEIGHT; 
    
    //calculate Rtouch_x
    Rtouch_x = Rt_x-Kx*X0-(Ry0-Ky*Y0);
    //calculate Rtouch_y  
    Rtouch_y = Rt_y-Ky*Y0-(Rx0-Kx*X0);

    return ((Rtouch_x+Rtouch_y)/2);
}

/*****************************************************************************/
//  Description:  double point algorithm 
//  Author     :  lichd
//	Note       :  
/*****************************************************************************/
uint32 tpc_algorithm (TPC_SEQ_DATA_PTR tpc_seq_ptr,TPC_DATA_U *data0, uint32 size) 
{
    uint32 interator_count=0;
    uint32 LCM_WIDTH,LCM_HEIGHT;	
	
    uint32     result =POINT_INVALID;

    uint16     Xd1=0,Yd1=0, Xd2=0,Yd2=0;	
    FLOAT_TYPE Xf1=0,Yf1=0, Xf2=0,Yf2=0;
	
    FLOAT_TYPE x =tpc_seq_ptr[TPC_SEQ_X].data;
    FLOAT_TYPE x1=tpc_seq_ptr[TPC_SEQ_X1].data;
    FLOAT_TYPE y =tpc_seq_ptr[TPC_SEQ_Y].data; 
    FLOAT_TYPE y1=tpc_seq_ptr[TPC_SEQ_Y1].data;//lint !e578
    FLOAT_TYPE Rx=tpc_seq_ptr[TPC_SEQ_RX].data;
    FLOAT_TYPE Ry=tpc_seq_ptr[TPC_SEQ_RY].data; 
    FLOAT_TYPE Rt_x=tpc_seq_ptr[TPC_SEQ_RT].data;
    //FLOAT_TYPE Rt_y=tpc_seq_ptr[TPC_SEQ_RT].data;  //avoid pclint error

    FLOAT_TYPE delta_Rx=0,delta_Ry=0;
    FLOAT_TYPE delta_x=0, delta_y=0;

    FLOAT_TYPE X1=0,Y1=0, X2=0,Y2=0;
    FLOAT_TYPE X3=0,Y3=0, X4=0,Y4=0;    

    FLOAT_TYPE delta_X=TP_ABS(x-x1);
    FLOAT_TYPE delta_Y=TP_ABS(y-y1);
    FLOAT_TYPE X0 = (x+x1)/2;
    FLOAT_TYPE Y0 = (y+y1)/2;

    FLOAT_TYPE Kx0=0,Ky0=0;
    FLOAT_TYPE TP_WIDTH,TP_HEIGHT, DPOINT_X_LIMIT, DPOINT_Y_LIMIT;
    FLOAT_TYPE DPOINT_RT_MAX,DPOINT_RT_MIN, SPOINT_RT_MAX;
    FLOAT_TYPE Rx0,Ry0,Rs0;  //Calibrated data
    static FLOAT_TYPE Rtouch=0;
    CALIBRATED_DATA_T* calibrated_data_ptr= TPDRV_GetCalibratedData();
	TP_CFG_INFO_PTR tp_cfg_ptr = TP_GetCfgInfo();
    LCM_SIZE_T lcm_size;
    
	s_tp_dp_cfg_ptr = tp_cfg_ptr->dpoint_cfg_ptr;

    LCM_GetSize(0, &lcm_size);
    LCM_WIDTH = lcm_size.width;
    LCM_HEIGHT= lcm_size.height;

    Rx0 = calibrated_data_ptr->Rx0;
    Ry0 = calibrated_data_ptr->Ry0;
    Rs0 = calibrated_data_ptr->Rs0;
    Rtouch=calibrated_data_ptr->Rt0;	       

    TP_WIDTH       = s_tp_dp_cfg_ptr->tp_width;
    TP_HEIGHT      = s_tp_dp_cfg_ptr->tp_height;
	DPOINT_X_LIMIT = s_tp_dp_cfg_ptr->dpoint_x_limit;
	DPOINT_Y_LIMIT = s_tp_dp_cfg_ptr->dpoint_y_limit;

    SPOINT_RT_MAX= s_tp_dp_cfg_ptr->spoint_rt_max;	    
    DPOINT_RT_MAX= s_tp_dp_cfg_ptr->dpoint_rt_max;
    DPOINT_RT_MIN= s_tp_dp_cfg_ptr->dpoint_rt_min;
        
	Kx0   =Rx0/TP_WIDTH;
    Ky0   =Ry0/TP_HEIGHT; 
	
    sprintf(log_buffer, "\r\nTPDRV_algorithm: y=%f y1=%f x=%f x1=%f Ry=%f Rx=%f Rt_x=%f",\
                                                              y,\
                                                              y1,\
                                                              x,\
                                                              x1,\
                                                              Ry,\
                                                              Rx,\
                                                              Rt_x);
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_263_112_2_17_23_7_27_1339,(uint8*)"s", log_buffer);
    sprintf(log_buffer, "TPDRV_algorithm: Rx0=%f Ry0=%f \r\n", Rx0,Ry0);
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_265_112_2_17_23_7_27_1340,(uint8*)"s", log_buffer);
	
//step0:
    delta_Rx=(Rx0>Rx)?(Rx0-Rx):(0);
    delta_Ry=(Ry0>Ry)?(Ry0-Ry):(0);
	
step:
if(interator_count)	
{
	/*
	Des:
	Rt=RTc-[Rx0*((1.5*x1*x2/(x1+x2)+abs(x2-x1)/5)) +
	        Ry0*((y1*y2/(y1+y2)-abs(y2-y1)/10))
	        ]
	Rt=Rt*2
	*/
	//x1/x2 is distance to right edge.
	//y1/y2 is the min distance in vertical to each edge.
	FLOAT_TYPE x1_=TP_WIDTH-Xf1,x2_=TP_WIDTH-Xf2, y1_=Yf1,y2_=Yf2;
	FLOAT_TYPE delta_x12=0,delta_y12=0;
	FLOAT_TYPE Rt_=0;

    y1_ = (TP_HEIGHT/2-y1_>=0) ? (y1_):(TP_HEIGHT-y1_);
    y2_ = (TP_HEIGHT/2-y2_>=0) ? (y2_):(TP_HEIGHT-y2_);
    //Normalization	
    x1_ = x1_/TP_WIDTH; 	x2_ = x2_/TP_WIDTH; 
    y1_ = y1_/TP_HEIGHT; 	y2_ = y2_/TP_HEIGHT; 

	if(x1_>=x2_)
		delta_x12 = x1_-x2_;
	else
		delta_x12 = x2_-x1_;
	if(y1_>=y2_)
		delta_y12 = y1_-y2_;
	else
		delta_y12 = y2_-y1_;
	
    sprintf(log_buffer, "TPDRV_algorithm:step2 x1=%f,x2=%f  y1=%f,y2=%f Rx0=%f,Ry0=%f,Rtc=%f",x1_,x2_,y1_,y2_, Rx0,Ry0,Rt_x);	
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_303_112_2_17_23_7_27_1341,(uint8*)"s", log_buffer);
    sprintf(log_buffer, "TPDRV_algorithm:step2 abs(x2-x1)=%f,abs(y2-y1)=%f",delta_x12,delta_y12);		
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_305_112_2_17_23_7_27_1342,(uint8*)"s", log_buffer);
	Rt_ = (FLOAT_TYPE)(Rx0*(1.5*x1_*x2_/(x1_+x2_) + (delta_x12)/5.0) +Ry0*(y1_*y2_/(y1_+y2_)-(delta_y12)/10.0));
    sprintf(log_buffer, "TPDRV_algorithm:step2 ---------new Rt=%f ", Rt_);	
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_308_112_2_17_23_7_27_1343,(uint8*)"s", log_buffer);
	Rt_ = Rt_x-Rt_;
	Rtouch = Rt_*2;	
    sprintf(log_buffer, "TPDRV_algorithm:step2 ---------new Rt=%f ", Rtouch);	
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_312_112_2_17_23_7_27_1344,(uint8*)"s", log_buffer);
}    

//step1:
    sprintf(log_buffer, "TPDRV_algorithm: delta_Rx=%f delta_Ry=%f Kx0=%f Ky0=%f Rtouch=%f", delta_Rx,delta_Ry,Kx0,Ky0,Rtouch);
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_317_112_2_17_23_7_27_1345,(uint8*)"s", log_buffer);
    g_watch_data[WATCH_RT] = Rtouch;
    delta_x = F1_delta_x(delta_Rx, Kx0, Ky0, Rtouch);
    delta_y = F1_delta_y(delta_Ry, Kx0, Ky0, Rtouch);
    g_watch_data[WATCH_DELTA_X] = delta_x;
    g_watch_data[WATCH_DELTA_Y] = delta_y;
    sprintf(log_buffer, "TPDRV_algorithm: delta_x=%f delta_y=%f ", delta_x,delta_y);
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_321_112_2_17_23_7_27_1346,(uint8*)"s", log_buffer);
    
    X1 = X0-delta_x/2;    Y1 = Y0-delta_y/2;
    X2 = X0+delta_x/2;    Y2 = Y0+delta_y/2; 
    X3 = X0+delta_x/2;    Y3 = Y0-delta_y/2;
    X4 = X0-delta_x/2;    Y4 = Y0+delta_y/2; 

    sprintf(log_buffer, "TPDRV_algorithm: delta_x=%f delta_y=%f ", delta_x,delta_y);
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_329_112_2_17_23_7_27_1347,(uint8*)"s", log_buffer);
    sprintf(log_buffer, "TPDRV_algorithm: DPOINT_X_LIMIT=%f DPOINT_Y_LIMIT=%f ", DPOINT_X_LIMIT,DPOINT_Y_LIMIT);
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_331_112_2_17_23_7_27_1348,(uint8*)"s", log_buffer);
    sprintf(log_buffer, "TPDRV_algorithm: SPOINT_RT_MAX=%f DPOINT_RT_(%f,%f) ", SPOINT_RT_MAX,DPOINT_RT_MAX,DPOINT_RT_MIN);
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_333_112_2_17_23_7_27_1349,(uint8*)"s", log_buffer);

    if((delta_x<=DPOINT_X_LIMIT) && (delta_y<=DPOINT_Y_LIMIT)) 
    {
        //if(Rtouch<=SPOINT_RT_MAX) //Single_point Rt_limit
        //----------------------------------------------------------------Single_point Rt_limit
        {
		result = POINT_SINGLE;	
        //__LOG:"TPDRV_algorithm:----single_point---------------------==OK==----"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_343_112_2_17_23_7_27_1350,(uint8*)"");
        }
    }
    else
    {
        if( (interator_count==0) || 
            ((interator_count==1)&&(Rtouch<=DPOINT_RT_MAX)&&(Rtouch>=DPOINT_RT_MIN)) ) //Double_point Rt_limit
        //----------------------------------------------------------------//Double_point Rt_limit        
        {
        //__LOG:"TPDRV_algorithm:----double_point: ----------------==OK==-------"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_357_112_2_17_23_7_27_1352,(uint8*)"");
		result = POINT_DOUBLE;			
        }
        else
        {
        //__LOG:"TPDRV_algorithm:----double_point:-* -*-* -* -* -*invalid-* -* "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_362_112_2_17_23_7_27_1353,(uint8*)"");
        //result =POINT_INVALID;
        result =POINT_SINGLE;
        goto OUT;
        }
    }

    /*
    [(X0-delta_x/2, Y0-delta_y/2), (X0+delta_x/2, Y0-delta_y/2)], 
    [(X0-delta_x/2, Y0+delta_y/2), (X0+delta_x/2, Y0+delta_y/2)]
    <==>
    [(X1,Y1), (X3,Y3)] 
    [(X4,Y4), (X2,Y2)]
    */
//Result: 
    if( delta_Y <= delta_X)
    {   
        if(y1<=y)
        {
		Xf1 = (X1); Yf1 = (Y1);
		Xf2 = (X2); Yf2 = (Y2);	
        }
        else
        {    
		Xf1 = (X3); Yf1 = (Y3);
		Xf2 = (X4); Yf2 = (Y4);			
        }
    }
    else
    {
        if(x1<=x)
        {
		Xf1 = (X1); Yf1 = (Y1);
		Xf2 = (X2); Yf2 = (Y2);			
        }
        else
        {
		Xf1 = (X3); Yf1 = (Y3);
		Xf2 = (X4); Yf2 = (Y4);			
        }
    }

    sprintf(log_buffer, "TPDRV_algorithm:---- Xf1=%f Yf1=%f Xf2=%f Yf2=%f", Xf1,Yf1, Xf1,Yf2);
    //__LOG:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_402_112_2_17_23_7_27_1354,(uint8*)"s", log_buffer);
    if((interator_count < 1) && (result == POINT_DOUBLE))
    {
        //__LOG:"TPDRV_algorithm:---------------interator_count=%d---------\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_405_112_2_17_23_7_27_1355,(uint8*)"d", interator_count);
        interator_count++;	
		goto step;
	}
        //__LOG:"TPDRV_algorithm:---------------interator_count=%d---------\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_409_112_2_17_23_7_27_1356,(uint8*)"d", interator_count);
OUT: 	
    Xd1 = len2pixel(Xf1,(FLOAT_TYPE)LCM_WIDTH,TP_WIDTH);
    Yd1 = len2pixel(Yf1,(FLOAT_TYPE)LCM_HEIGHT,TP_HEIGHT);
    Xd2 = len2pixel(Xf2,(FLOAT_TYPE)LCM_WIDTH,TP_WIDTH);
    Yd2 = len2pixel(Yf2,(FLOAT_TYPE)LCM_HEIGHT,TP_HEIGHT);   

    if(result == POINT_SINGLE)
    {
        data0[0].data.data_x = (Xd1+Xd2)/2;	
    	data0[0].data.data_y = (Yd1+Yd2)/2;
        data0[1].data.data_x = 0;	
    	data0[1].data.data_y = 0;    
	}
	else
	{
        data0[0].data.data_x = Xd1;	
    	data0[0].data.data_y = Yd1;
        data0[1].data.data_x = Xd2;	
    	data0[1].data.data_y = Yd2;
	}
	
    //__LOG:"[TPDRV_algorithm:(Xd1,Yd1)=[%d,%d], (Xd2,Yd2)=[%d,%d]rn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_ALGORITHM_433_112_2_17_23_7_27_1357,(uint8*)"dddd",data0[0].data.data_x,data0[0].data.data_y,data0[1].data.data_x,data0[1].data.data_y);
	
    return (result);
} 
