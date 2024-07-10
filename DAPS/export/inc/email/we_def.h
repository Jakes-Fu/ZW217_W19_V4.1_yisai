/*=====================================================================================

    HEADER NAME : we_def.h
    MODULE NAME : WE Framework Base Type Define

    PRE-INCLUDE FILES DESCRIPTION

    GENERAL DESCRIPTION
        This file can be included by all WE Programmer.In this file some we's base datatype
    is defined.
        These Info is  aptotic and is not changed by switching Project.

    TECHSOFT Confidential Proprietary
    (c) Copyright 2006 by TechSoft. All Rights Reserved.
=======================================================================================
    Revision History
   
    Modification                 Tracking
    Date           Author         Number        Description of changes
    ----------   ------------    ---------   --------------------------------------------
    2006-05-28   HiroWang				       create this file

    Self-documenting Code
    Describe/explain low-level design, especially things in header files, of this module and/or group 
    of funtions and/or specific funtion that are hard to understand by reading code and thus requires 
    detail description.
    Free format !

=====================================================================================*/
/*--------------------------START-----------------------------*/


/*******************************************************************************
*   Multi-Include-Prevent Section
*******************************************************************************/
#ifndef WE_DEF_H
#define WE_DEF_H

/*******************************************************************************
*   Include File Section
*******************************************************************************/
/*Include System head file*/

/*Include Program Global head file*/

/*Include Module head file*/

/*******************************************************************************
*   Macro Define Section
*******************************************************************************/
/*Conditional Compilation Directives start*/
/* Define this if the platform has support for floating-point numbers. */
//#define  CFG_WE_HAS_FLOAT
/*Conditional Compilation Directives end*/

/*Define Constant Macro start*/
#ifndef NULL
#define NULL      (void*)0       /* NULL  :  Null pointer    */
#endif

#ifndef TRUE
#define TRUE      1       /* TRUE  :  Integer value 1 */
#endif

#ifndef FALSE
#define FALSE     0       /* FALSE :  Integer value 0 */
#endif

#ifndef SUCCESS
#define SUCCESS   0
#endif

/*Define Constant Macro end*/


/*Define Macro Function start*/
#define WE_UNUSED_PARAMETER(x)         ((x) = (x))
/*Define Macro Function end*/

/*******************************************************************************
*   Type Define Section
*******************************************************************************/

/*Define base type start*/

/**********************from Brew Base Type******************************************************/
typedef unsigned char      WE_BOOL  ;      /*BOOLEAN : unsigned char(1/0)        prefix: b     */

typedef char               WE_CHAR  ;      /*CHAR    : char                      prefix: c     */
typedef signed char        WE_SCHAR ;      /*SCHAR   : signed char               prefix: sc    */     
typedef unsigned char      WE_UCHAR ;      /*UCHAR   : unsigned char             prefix: uc    */

typedef char               WE_INT8  ;      /*INT8    : 8 bit  integer            prefix: c     */   
typedef signed char        WE_SINT8 ;      /*SINT8   : 8 bit  signed integer     prefix: sc    */
typedef unsigned char      WE_UINT8 ;      /*UINT8   : 8 bit  unsigned integer   prefix: uc    */  

typedef short              WE_INT16 ;      /*INT16   : 16 bit integer            prefix: s     */
typedef signed short       WE_SINT16;      /*SINT16  : 16 bit signed integer     prefix: ss    */    
typedef unsigned short     WE_UINT16;      /*UINT16  : 16 bit unsigned integer   prefix: us    */ 

typedef int                WE_INT   ;      /*INT     : general integer           prefix: i     */
typedef signed int         WE_SINT  ;      /*SINT    : general signed integer    prefix: si    */
typedef unsigned int       WE_UINT  ;      /*UINT    : general unsigned integer  prefix: ui    */

typedef int                WE_INT32 ;      /*INT32   : 32 bit integer            prefix: i     */ 
typedef signed int         WE_SINT32;      /*SINT32  : 32 bit signed integer     prefix: si    */ 
typedef unsigned int  	   WE_UINT32;      /*UINT32  : 32 bit unsigned integer   prefix: ui    */ 

typedef long	            WE_LONG  ;      /*LONG    : long                      prefix: l     */
typedef signed long        WE_SLONG ;      /*SLONG   : signed long               prefix: sl    */     
typedef unsigned long	   WE_ULONG ;      /*ULONG   : unsigned long             prefix: ul    */ 

typedef float	            WE_FLOAT ;      /*FLOAT   : float                     prefix: f     */               
typedef double	            WE_DOUBLE;      /*DOUBLE  : double float              prefix: d     */  
typedef void               WE_VOID  ;      /*VOID    : void                      prefix: v     */       
typedef void*              WE_HANDLE;      /*HANDLE  : for some object's pointer prefix: h     */
/***********************************************************************************************/

/**********************from Standard C Base Type******************************************************
typedef unsigned char      	   WE_BOOL  ;     BOOLEAN : unsigned char(1/0)        prefix: b     
          
typedef char	               WE_CHAR  ;     CHAR    : char                      prefix: c     
typedef signed   char	       WE_SCHAR ;     SCHAR   : signed char               prefix: sc         
typedef unsigned char	       WE_UCHAR ;     UCHAR   : unsigned char             prefix: uc    

typedef char	               WE_INT8  ;     INT8    : 8 bit  integer            prefix: c        
typedef signed   char          WE_SINT8 ;     SINT8   : 8 bit  signed integer     prefix: sc    
typedef unsigned char	       WE_UINT8 ;     UINT8   : 8 bit  unsigned integer   prefix: uc      

typedef short                  WE_INT16 ;     INT16   : 16 bit integer            prefix: s     
typedef signed   short         WE_SINT16;     SINT16  : 16 bit signed integer     prefix: ss        
typedef unsigned short         WE_UINT16;     UINT16  : 16 bit unsigned integer   prefix: us     

typedef int                    WE_INT   ;     INT     : general integer           prefix: i     
typedef signed   int           WE_SINT  ;     SINT    : general signed integer    prefix: si    
typedef unsigned int           WE_UINT  ;     UINT    : general unsigned integer  prefix: ui    

typedef long     int           WE_INT32 ;     INT32   : 32 bit integer            prefix: i      
typedef signed   long int      WE_SINT32;     SINT32  : 32 bit signed integer     prefix: si     
typedef unsigned long int  	   WE_UINT32;     UINT32  : 32 bit unsigned integer   prefix: ui     
 
typedef long	               WE_LONG  ;     LONG    : long                      prefix: l     
typedef signed long            WE_SLONG ;     SLONG   : signed long               prefix: sl         
typedef unsigned long	       WE_ULONG ;     ULONG   : unsigned long             prefix: ul     

    
typedef float	               WE_FLOAT ;     FLOAT   : float                     prefix: f                    
typedef double	               WE_DOUBLE;     DOUBLE  : double float              prefix: d       
typedef void                   WE_VOID  ;     VOID    : void                      prefix: v            
typedef void *                 WE_HANDLE;     HANDLE  : for some object's pointer prefix: h     
*******************************************************************************************************/

typedef unsigned short uint16;

/*Define base type end*/

/*Define Enum type start*/
/*Define Enum type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
/*Define struct &  union type end*/

/*******************************************************************************
*   Prototype Declare Section
*******************************************************************************/


#endif/*endif WE_DEF_H*/



/*--------------------------END-----------------------------*/


