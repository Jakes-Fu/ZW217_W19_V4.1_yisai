#include "fake.h"

// io
/*******************************************************************/
//  Interface:		SPRD_printf
//  Description : 	SPRD_printf
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_printf( const char *format,... )
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_fprintf
//  Description : 	SPRD_fprintf
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_fprintf( FILE *stream, const char *format,...)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD__fprintf
//  Description : 	SPRD__fprintf
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC__fprintf( FILE *stream, const char *format,...)
{
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_getc
//  Description : 	SPRD_getc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_getc(FILE * stream)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_getchar
//  Description : 	SPRD_getchar
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_getchar(void)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_fputc
//  Description : 	SPRD_fputc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_fputc(int c,FILE * stream)
{
	//SCI_ASSERT(FALSE);
	return c;
}

/*******************************************************************/
//  Interface:		SPRD_fputs
//  Description : 	SPRD_fputs
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_fputs(const char * s,FILE * stream)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_fgetc
//  Description : 	SPRD_fgetc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_fgetc(FILE * stream)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_fgets
//  Description : 	SPRD_fgets
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
char * SPLIBC_fgets(char * s,int size,FILE * stream)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_gets
//  Description : 	SPRD_gets
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
char * SPLIBC_gets(char *s)
{
	//SCI_ASSERT(FALSE);
	return 0;
}

/*******************************************************************/
//  Interface:		SPRD_putc
//  Description : 	SPRD_putc
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int SPLIBC_putc(int c,FILE * stream)
{
	//SCI_ASSERT(FALSE);
	return c;
}
