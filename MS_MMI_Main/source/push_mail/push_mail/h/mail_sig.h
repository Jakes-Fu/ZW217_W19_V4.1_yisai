/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_sig.h
 *	Author		: sunguochen    
 *	Description	: �ֻ�����-�¼��������
 *	Vesion		: 1.0        
 *	Date			: 2010-4-28  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-28      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#ifndef _MAIL_SIG_H_
#define _MAIL_SIG_H_

#include "mail_cfg.h"




typedef enum
{
    E_FSM_NOTUSED,      
    E_FSM_MAIL_MAIN,      
    E_FSM_POP3,         
    E_FSM_SMTP,         
    E_FSM_IMAP4,        
    E_FSM_SEND_MAIL,      
    E_FSM_NET,          
    E_FSM_RETRIEVE,        
    E_FSM_FLDR_MANAGER, 
        
    E_FSM_IMAP4_IDLE,
    
    E_FSM_MAX_REGS

} EStateMachine;





#define MAIL_MAIN_SIGNAL_BASE        0
#define MAIL_POP3_SIGNAL_BASE        100
#define MAIL_IMAP4_SIGNAL_BASE       200
#define MAIL_SMTP_SIGNAL_BASE        300
#define MAIL_SEND_SIGNAL_BASE        400
#define MAIL_MSGBOX_SIGNAL_BASE      500
#define MAIL_NET_SIGNAL_BASE         600
#define MAIL_RETRIEVE_SIGNAL_BASE    700
#define MAIL_ASYNC_SIGNAL_BASE       900
#define MAIL_FLRD_SIGNAL_BASE        1000
#define MAIL_MIME_SIGNAL_BASE        1100

#define MAIL_PH_SIGNAL_BASE          1200

#define MAIL_MSF_SIGNAL_BASE         1300 
#define MAIL_COMMON_SIGNAL_BASE      1400 

#define MAIL_EMN_SIGNAL_BASE     1500 

#define MAIL_MLST_SIGNAL_BASE        1600
#define MAIL_EVNT_SIGNAL_BASE        1700

#define MAIL_MDN_SIGNAL_BASE         1800

#define MAIL_PEXEC_SIGNAL_BASE       1900



typedef enum
{
    E_SIG_CMMN_TIMER_EXPIRED  = MAIL_COMMON_SIGNAL_BASE  

} ESignalId;


#define E_SIGNAL_SENDTO(d, sig) \
            mail_SignalSendTo ((d), (sig), 0L, 0L, 0L, 0L, NULL)

#define E_SIGNAL_SENDTO_I(d, sig, i) \
            mail_SignalSendTo ((d), (sig), i, 0L, 0L, 0L, NULL)

#define E_SIGNAL_SENDTO_II(d, sig, i, i2) \
            mail_SignalSendTo ((d), (sig), i, i2, 0L, 0L, NULL)

#define E_SIGNAL_SENDTO_U(d, sig, u) \
            mail_SignalSendTo ((d), (sig), 0L, 0L, (u), 0L, NULL)

#define E_SIGNAL_SENDTO_IU(d, sig, i, u) \
            mail_SignalSendTo ((d), (sig), (i), 0L, (u), 0L, NULL)

#define E_SIGNAL_SENDTO_UU(d, sig, u1, u2) \
            mail_SignalSendTo ((d), (sig), 0L, 0L, (u1), (u2), NULL)

#define E_SIGNAL_SENDTO_UP(d, sig, u1, p) \
            mail_SignalSendTo ((d), (sig), 0L, 0L, (u1), 0L, (p))

#define E_SIGNAL_SENDTO_IUU(d, sig, i, u1, u2) \
            mail_SignalSendTo ((d), (sig), (i), 0L, (u1), (u2), NULL)

#define E_SIGNAL_SENDTO_IIU(d, sig, i, i2, u1) \
            mail_SignalSendTo ((d), (sig), (i), (i2), (u1), 0L, NULL)

#define E_SIGNAL_SENDTO_P(d, sig, p) \
            mail_SignalSendTo ((d), (sig), 0L, 0L, 0L, 0L, (p))

#define E_SIGNAL_SENDTO_IP(d, sig, i, p) \
            mail_SignalSendTo ((d), (sig), (i), 0L, 0L, 0L, (p))

#define E_SIGNAL_SENDTO_IUP(d, sig, i, u, p) \
            mail_SignalSendTo ((d), (sig), (i), 0L, (u), 0L, (p))

#define E_SIGNAL_SENDTO_IUUP(d, sig, i, u1, u2, p) \
            mail_SignalSendTo ((d), (sig), (i), 0L, (u1), (u2), (p))

#define E_SIGNAL_SENDTO_IIUUP(d, sig, i, i2, u1, u2, p) \
            mail_SignalSendTo ((d), (sig), (i), (i2), (u1), (u2), (p))








typedef struct ESignalStruct
{
    struct ESignalStruct *next;       
    EStateMachine         dst;        
    int                         type;       
    int                  i_param;    
    int                  i_param2;   
    unsigned int     u_param1;   
    unsigned int     u_param2;   
    void                       *p_param;    

}ESignal;






typedef void ESignalFunction (ESignal *sig);








/*==========================================================
 * Function 	: mail_SignalInit
 * Author		: sunguochen      
 * Description : �źŴ����ʼ��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_SignalInit (void);



/*==========================================================
 * Function 	: mail_SignalTerminate
 * Author		: sunguochen      
 * Description : �����źŴ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_SignalTerminate (void);



/*==========================================================
 * Function 	: mail_SignalProcess
 * Author		: sunguochen      
 * Description : �źŴ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_SignalProcess (EStateMachine machine);



/*==========================================================
 * Function 	: mail_SignalQueueIsEmpty
 * Author		: sunguochen      
 * Description : �źŶ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
int  mail_SignalQueueIsEmpty (void);



/*==========================================================
 * Function 	: mail_SignalDelete
 * Author		: sunguochen      
 * Description : ɾ���ź�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_SignalDelete (ESignal *sig);



/*==========================================================
 * Function 	: mail_SignalRegisterDst
 * Author		: sunguochen      
 * Description : ע���źŴ�����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_SignalDeregister (EStateMachine dst);



/*==========================================================
 * Function 	: mail_SignalDeregister
 * Author		: sunguochen      
 * Description : ȡ��ע���źŴ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_SignalRegisterDst (EStateMachine dst, ESignalFunction *f);



/*==========================================================
 * Function 	: mail_SignalSendTo
 * Author		: sunguochen      
 * Description : �źŷ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
void mail_SignalSendTo(EStateMachine dst,
                        int sig,
                        int i_param,
                        int i_param2,
                        unsigned int u_param1,
                        unsigned int u_param2,
                        void *p_param);



/*==========================================================
 * Function 	: mail_fsmNum2str
 * Author		: sunguochen      
 * Description : ��fsmת��Ϊ����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/28/2010 
 ==========================================================*/
const char* mail_fsmNum2str(int fsmNum);



#endif
