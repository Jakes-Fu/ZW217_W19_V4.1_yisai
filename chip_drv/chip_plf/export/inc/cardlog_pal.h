#ifndef _CARDLOG_PAL_H_
#define _CARDLOG_PAL_H_

PUBLIC BOOLEAN Cardlog_Pal_init (void);
PUBLIC void Cardlog_Pal_uninit (void);
PUBLIC BOOLEAN Cardlog_Pal_r (uint32 start, uint32 scts, void *buf);
PUBLIC BOOLEAN Cardlog_Pal_w (uint32 start, uint32 scts, void *buf);
PUBLIC BOOLEAN Cardlog_Pal_e (uint32 start, uint32 scts);
PUBLIC uint32  Cardlog_Pal_g (void);


#endif
