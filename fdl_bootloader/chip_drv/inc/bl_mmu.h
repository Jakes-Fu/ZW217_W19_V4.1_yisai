#ifndef _BL_MMU_H_
#define _BL_MMU_H_

extern void MMU_Init (void);
extern void MMU_Init_ext(void);
extern void MMU_DisableIDCM(void);
extern void MMU_EnableIDCM(void);
extern void MMU_InvalideICACHEALL(void);
extern void MMU_InvalideDCACHEALL(void);
extern void Dcache_InvalRegion(void);

#endif


