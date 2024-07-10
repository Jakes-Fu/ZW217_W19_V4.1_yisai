#ifndef __HERO_TYPES_H_
#define __HERO_TYPES_H_

#ifdef HERO_ENGINE_TTS_SUPPORT

typedef int   			   HERO_BOOL  ;      /*BOOLEAN : unsigned char(1/0)        prefix: b     */

typedef char	           HERO_CHAR  ;      /*CHAR    : char                      prefix: c     */
typedef char	           HERO_SCHAR ;      /*SCHAR   : signed char               prefix: sc    */     
typedef unsigned char      HERO_UCHAR ;      /*UCHAR   : unsigned char             prefix: uc    */
typedef unsigned short     HERO_WCHAR;

typedef signed char	           HERO_INT8  ;      /*INT8    : 8 bit  integer            prefix: c     */   
typedef signed char               HERO_SINT8 ;      /*SINT8   : 8 bit  signed integer     prefix: sc    */
typedef unsigned char      HERO_UINT8 ;      /*UINT8   : 8 bit  unsigned integer   prefix: uc    */  

typedef short              HERO_INT16 ;      /*INT16   : 16 bit integer            prefix: s     */
typedef short              HERO_SINT16;      /*SINT16  : 16 bit signed integer     prefix: ss    */    
typedef unsigned short 	   HERO_UINT16;      /*UINT16  : 16 bit unsigned integer   prefix: us    */ 

typedef int                HERO_INT32   ;      /*INT     : general integer           prefix: i     */
typedef signed int         HERO_SINT32  ;      /*SINT    : general signed integer    prefix: si    */
typedef unsigned int       HERO_UINT32  ;      /*UINT    : general unsigned integer  prefix: ui    */


typedef long	           HERO_LONG  ;      /*LONG    : long                      prefix: l     */
typedef signed long        HERO_SLONG ;      /*SLONG   : signed long               prefix: sl    */     
typedef unsigned long	   HERO_ULONG ;      /*ULONG   : unsigned long             prefix: ul    */ 


typedef float	           HERO_FLOAT ;      /*FLOAT   : float                     prefix: f     */               
typedef double	           HERO_DOUBLE;      /*DOUBLE  : double float              prefix: d     */  
typedef void               HERO_VOID  ;      /*VOID    : void                      prefix: v     */       
typedef void *             HERO_HANDLE;      /*HANDLE  : for some object's pointer prefix: h     */

typedef int                HFILE; //file handle
typedef int                HTIME;//timer handle
typedef unsigned int size_t;


#ifndef NULL
#define NULL (void*)0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef HR_SUCCESS
#define HR_SUCCESS  0    //³É¹¦
#endif

#ifndef HR_FAILED
#define HR_FAILED   -1    //Ê§°Ü
#endif

#endif

#endif
