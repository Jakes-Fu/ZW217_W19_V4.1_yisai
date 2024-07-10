/******************************************************************************
                                   海信WCDMA  自主业务模块
        由于3G  网络的逐渐成熟，大量的业务需求成为第三代
        移动终端的亮点，逐渐成为不可或缺的一部分。本
        模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现在多平台、多操作系统中的PIM业务。目标
        是要达到任意平台的可移植性良好的功能稳定的模块。
        本模块的实现分为三个步骤:
        1.实现Data sync功能
        2.实现Device Management功能
        3.实现多平台的平滑移植
        模块开发参考OMA成员共同开发的SRT_c。
        目前模块计划开发支持OMA syncML 协议1.1


        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:刘刚
        开发日期:2005年4月11日。
******************************************************************************/


/******************************************************************************
文件名称:PIMsyncmlmgr.h
文件描述:syncml manager,syncml管理模块头文件。
文件说明:syncml管理模块，主要负责与smlXX等syncml内核模块交互
开发人员:刘刚
首次开发日期:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-4-11              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/
/*加入此FEATURE防止头文件重复包含*/
  #ifndef   HEADER_FILE_SYNCMLMGR
  #define  HEADER_FILE_SYNCMLMGR
  
  /*  包含头文件*/
  #include "PIMsmlcore.h"
  #include "mmipim.h"//wangzhefeng add 20090911

  /*  数据结构声明*/
  
  /*  全局变量声明*/

  /*  功能函数声明*/
   
  short hs_syncml_init(void);

  short hs_syncml_initance(short * pID, char* workspacename);

  short hs_syncml_startmessage(short id);

  short hs_syncml_StartSync(short id);

  short hs_syncml_AlertCmd(short id,char* cmd);

  short hs_syncml_PutCmd(short id);

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
  short hs_syncml_PutVCALCmd(short id);
#endif /* HS_PIM_VCAL */

  short hs_syncml_GetCmd(short id);
	
  short hs_syncml_AddCmd(short id); // chenxiang 20071124

  short hs_syncml_DeleteCmd(short id);

  short hs_syncml_StatusCmd(short id);

  short hs_syncml_MapCmd(short id);

  short hs_syncml_ReplaceCmd(short id); // chenxiang 20071124

  short hs_syncml_EndSync(short id);

  short hs_syncml_EndMessage(short id);

  short hs_syncml_EndMessageWithoutFinal(short id);//add by wangxiaolin 2007.11.10

  short hs_syncml_ReceiveData(short id);

  short hs_syncml_TerminateInstance(short id);

  short hs_syncml_TerminateAllExitInstance(void);

  short hs_syncml_Terminate(void);

  short hs_syncml_fresh_for_slow_sync(short id);

  short hs_syncml_modify_for_fast_sync(short id);
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for 大对象
void hs_syncml_set_large_object_proc_flag(BOOLEAN  proc_flag);
#endif

  #endif