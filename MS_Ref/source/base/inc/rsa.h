#ifndef _RSA_H_
#define _RSA_H_

/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:
//    Note:           p  = p exp (e) MOD m
//    Note:           r2 = 2 exp (k)
/*****************************************************************************/
extern void RSA_ModPower(unsigned int *p, unsigned int *m,
              unsigned int *r2, unsigned int e);

#endif
