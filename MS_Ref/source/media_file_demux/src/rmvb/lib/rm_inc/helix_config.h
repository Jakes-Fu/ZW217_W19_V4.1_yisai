


#ifndef HELIX_CONFIG_H
#define HELIX_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */


/********************************************************************
 *
 * This file contains system wide configuration options.  Please look
 * through all the config options below to make sure the SDK is tuned
 * best for your system.
 *
 ********************************************************************
 */

    
/*
 * Endian'ness.
 *
 * This package supports both compile-time and run-time determination
 * of CPU byte ordering.  If ARCH_IS_BIG_ENDIAN is defined as 0, the
 * code will be compiled to run only on little-endian CPUs; if
 * ARCH_IS_BIG_ENDIAN is defined as non-zero, the code will be
 * compiled to run only on big-endian CPUs; if ARCH_IS_BIG_ENDIAN is
 * not defined, the code will be compiled to run on either big- or
 * little-endian CPUs, but will run slightly less efficiently on
 * either one than if ARCH_IS_BIG_ENDIAN is defined.
 *
 * If you run on a BIG engian box uncomment this:
 *
 *    #define ARCH_IS_BIG_ENDIAN 1
 * 
 * If you are on a LITTLE engian box uncomment this:
 *
 *    #define ARCH_IS_BIG_ENDIAN 0
 * 
 * Or you can just leave it undefined and have it determined
 * at runtime.
 *
 */







    
#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef HELIX_CONFIG_H */
