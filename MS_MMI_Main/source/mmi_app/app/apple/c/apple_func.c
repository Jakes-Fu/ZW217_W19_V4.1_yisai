
#define _APPLE_FUNC_C_  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "std_header.h"
#include "window_parse.h"
#include "guilistbox.h"
#include "guimsgbox.h"
// #include "mmiset.h"
// #include "mmiacc_winmain.h"
#include "mmidisplay_data.h" 
#include "guilcd.h"
#include "mmi_nv.h"
//#include "apple_menutable.h"
#include "apple_nv.h"
#include "apple_id.h"

#include "apple_export.h"

PUBLIC  BOOLEAN  Apple_Init(void)
{
//     uint16 nv_return = 0;
//     uint16 high_score[3];
//     uint8 lever[6];
//     
//     MMINV_READ(MMINV_GMBOXMAN_LEVER_SET, (uint16*)lever, nv_return);
//     if (MN_RETURN_SUCCESS != nv_return)
//     {
//         SCI_MEMSET(&lever[0], 0, 6);
//         MMINV_WRITE(MMINV_GMBOXMAN_LEVER_SET, &lever[0]);
//     }
//     
//     
//     MMINV_READ(MMINV_GMBOXMAN_HIGH_SCORE, ( int16*)high_score, nv_return);   
//     if (MN_RETURN_SUCCESS != nv_return)
//     {
//         SCI_MEMSET(&high_score[0], 0, 6);
//         MMINV_WRITE(MMINV_GMBOXMAN_HIGH_SCORE, &high_score[0]);
//     }
    
    return TRUE;
    
}



/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPPLE_InitModule(void)
{
//    Apple_RegMenuGroup();  //初始化im menu
 //   Apple_RegNv();   //初始化im nv
    Apple_RegWinIdNameArr();
}
