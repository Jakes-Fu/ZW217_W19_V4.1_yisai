/******************************************************************************
** File Name:       config_file.c                                                   
** Author:          Yifeng.Wang                              
** Date:            2007/12/10                                               
** Copyright:       2007 Spreadtrum, Incorporated. All Rights Reserved.      
** Description:     This file defines configure file read & write interface
** Note:            Amend from INIFileOP.C v1.0 of Yuewang.Liao      
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2007/12/10       Yifeng.Wang         Create
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "stdafx.h"
#include "config_file.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define CURRENT_FILE        "config_file.cpp"


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/* configuration file format
   [ SECTION_1 ]
   # comment
   KEY_1 = VALUE_1

   [ SECTION_2 ]
   # comment
   KEY_2 = +
   VALUE_2
*/

/* format symbol -- you can change them for special use! */
static char s_cfg_ssl = '[', s_cfg_ssr = ']';
static char s_cfg_nts = '#';        /* comment char */

static int  s_cfg_section_line_no = 0;
static int  s_cfg_key_line_no = 0;
static int  s_cfg_key_lines = 0; 


/*---------------------------------------------------------------------------*
/*                          LOCAL FUNCTION DEFINITION                        *
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: clear space chars at the left of the string
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL char* strtrimr( char* buf )
{
    int     len,i;
    char*   tmp = NULL;

    if( NULL == buf )
    {
        return NULL;
    }

    /* allocate a temp memory */
    len = strlen(buf);
    tmp = new char[len];
    if( NULL == tmp )
    {
        TRACE("%s(%d) : malloc memory failed!\n", CURRENT_FILE, __LINE__);
        return NULL;
    }

    /* find the index of first non-space char */
    memset(tmp, '\0', len);
    for( i = 0 ; i < len ; i++ )
    {
        if( buf[i] != ' ' )
            break;
    }

    /* copy the string after the space */
    if ( i < len )
    {
        strncpy(tmp, (buf+i), (len-i));
    }
    strncpy(buf, tmp, len);
    delete[] tmp;
    return buf;
}

/*****************************************************************************/
// Description: clear space chars at the right of the string
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL char* strtriml( char* buf )
{
    int     len, i; 
    char*   tmp = NULL;
    
    if( NULL == buf )
    {
        return NULL;
    }

    /* allocate a temp memory */
    len = strlen(buf);
    tmp = new char[len];
    if( NULL == tmp )
    {
        TRACE("%s(%d) : malloc memory failed!\n", CURRENT_FILE, __LINE__);
        return NULL;
    }

    /* find the index of last non-space char */
    memset(tmp, 0x00, len);
    for( i = 0 ; i < len ; i++ )
    {
        if( buf[len-i-1] !=' ' )
            break;
    }

    /* copy the string before the space */
    if ( i < len )
    {
        strncpy(tmp, buf, len-i);
    }
    strncpy(buf, tmp, len);
    delete[] tmp;
    return buf;
}

/*****************************************************************************/
// Description: read a line from file
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL int FileGetLine(FILE *fp, char *buffer, int max_buff_size )
{
    int  i, j;
    char ch1;
    
    for( i= 0, j= 0; i < max_buff_size-1; j++ )
    {
        /* read a byte from file */
        if( fread(&ch1, sizeof(ch1), 1, fp) != 1 )
        {
            /* the end of the file */
            if( feof(fp) != 0 )
            {
                if( j == 0 )
                {
                    return -1;
                }
                else
                    break;
            }

            /* file read error */
            if( 0 != ferror(fp) )
            {
                return -2;
            }
            return -2; 
        } 
        else    /* check the read byte */ 
        {
            /* check if char is "LINE-FEED" or NULL(string end) */
            if (ch1 == '\n' || ch1 == 0x00)
            {
                break;
            }

            /* check if char is "FORM-FEED" or "Ctrl+Z"(EOF) */
            if (ch1 == '\f' || ch1 == 0x1A)
            {
                buffer[i++] = ch1;
                break;
            }

            /* ignore "CARRIAGE-RETURN" */
            if (ch1 != '\r')
            {
                buffer[i++] = ch1;
            }
        }
    }// end for
    
    ASSERT( i <= max_buff_size );
    buffer[i] = '\0';
    return i;
} 

/*****************************************************************************/
// Description: file copy
// Global resource : none
// Author: yifeng.wang  
/*****************************************************************************/
LOCAL int FileCopy( void *source_file, void *dest_file )
{ 
    FILE *fp1, *fp2;
    char buf[1024+1];
    int  ret;

    /* open source file */
    fp1 = fopen( (char*)source_file, "r" );
    if ( NULL == fp1 )
    {
        return COPYF_ERR_OPEN_FILE;
    }
    ret = COPYF_ERR_CREATE_FILE;

    /* open destination file */
    fp2 = fopen((char *)dest_file, "w");
    if ( NULL == fp2 )
    {
        goto copy_end;
    }

    /* file copy */
    while(1)
    {
        /* copy data from source file */
        ret = COPYF_ERR_READ_FILE;
        memset(buf, 0x00, 1024+1);
        if( fgets((char *)buf, 1024, fp1) == NULL )
        {
            if( strlen(buf) == 0 )
            { 
                if( ferror(fp1) != 0 )
                {
                    goto copy_end;
                }
                break;
            }
        }

        /* copy data to dest file */
        ret = COPYF_ERR_WRITE_FILE;
        if( fputs((char *)buf, fp2) == EOF )
        {
            goto copy_end;
        }
    } 
    ret = COPYF_OK;

copy_end:
    if(fp2 != NULL) fclose(fp2);
    if(fp1 != NULL) fclose(fp1);
    return ret;
}

/*****************************************************************************/
// Description: split key & value
// Global resource : none
// Author: yifeng.wang 
// Note:    key    =   value
//          (k1)  (k2)  (i)  
// Return: 1 - ok; 0 - blank line; -1 - no key ("= val"); -2 - only key, no '=' 
/*****************************************************************************/
LOCAL int SplitKeyValue(char *buf, char **key, char **val)
{
    int  i, k1, k2, len;

    ASSERT( (NULL!=buf ) && (NULL!=key) && (NULL!=val) );
    
    len = strlen((char*)buf);
    if ( len < 1 ) 
        return 0; 

    /* ignore SPACE & TAB at the front */
    for ( i = 0; i < len; i++ )
    {
        if ( (buf[i] != ' ') && (buf[i] != '\t') ) 
            break;
    }

    /* no char left except SPACE & TAB */
    if ( i >= len )
        return 0;

    /* first char is '=', means no "KEY" */
    if ( buf[i] == '=' )
        return -1;

    /* find the end of "KEY" */
    k1 = i;
    for ( i++; i < len; i++ )
    {
        if ( buf[i] == '=' )
            break;
    }

    /* no '=' found */
    if ( i >= len ) 
        return -2; 

    /* find the head of "VALUE" */
    k2 = i; 
    for ( i++; i < len; i++ )
    {
        if ( (buf[i] != ' ') && (buf[i] != '\t') ) 
            break;
    }

    buf[k2] = '\0'; 
    *key = buf + k1; 
    *val = buf + i; 
    return 1; 
} 

/*---------------------------------------------------------------------------*
/*                               FUNCTIONS                                   *
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: check if section name in file 
// Global resource: none
// Author: yifeng.wang
// Note: 0 - OK ; not 0 - error
/*****************************************************************************/
PUBLIC int CONFIGFILE_IsSectionExist( char* file_path, void *section )
{
    FILE *fp = NULL; 
    char buf[MAX_CFG_BUF]; 
    int  line_no, n, ret; 

    line_no = 0;

    /* open file */
    fp = fopen((char *)file_path, "rb");
    if ( NULL == fp )
    {
        return CFG_ERR_OPEN_FILE; 
    }

    /* search for matched SECTION NAME */
    while (1)
    { 
        ret = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp, buf, MAX_CFG_BUF); 
        if ( n < -1 ) 
            goto r_cfg_end; 
        ret = CFG_SECTION_NOT_FOUND; 
        if ( n < 0 ) 
            goto r_cfg_end;
        
        line_no++; 
        n = strlen(strtriml(strtrimr(buf)));

        /* check if a null line ( clear all spaces ) or comment line */
        if (n == 0 || buf[0] == s_cfg_nts)
            continue;

        /* check SECTION format : [ ... ] */
        ret = CFG_ERR_FILE_FORMAT;
        if (n > 2 && (buf[0] != s_cfg_ssl || buf[n-1] != s_cfg_ssr))
            continue;

        if ( s_cfg_ssl == buf[0] )
        {
            buf[n-1] = 0x00;
            n = strlen(strtriml(strtrimr(buf+1)));
            /* check if section name matched */
            if( n > 0 && 0 == strcmp(buf+1, (char*)section) )
                break;
        }
    }
    ret = CFG_OK;

r_cfg_end:
    if(fp != NULL)
        fclose(fp);
    return ret;
}

/*****************************************************************************/
// Description: get key's value under a given section 
// Global resource: none
// Author: yifeng.wang
// Note: 0 - OK ; not 0 - error
/*****************************************************************************/
PUBLIC int CONFIGFILE_GetKeyValue( char* file_path, void *section, void *key, void *buf)
{ 
    FILE *fp; 
    char buf1[MAX_CFG_BUF], buf2[MAX_CFG_BUF]; 
    char *key_ptr, *val_ptr; 
    int  line_no, n, ret; 
    
    line_no = 0; 
    s_cfg_section_line_no = 0; 
    s_cfg_key_line_no = 0; 
    s_cfg_key_lines = 0; 
    
    /* open file */
    fp = fopen((char *)file_path, "rb");
    if ( NULL == fp ) 
        return CFG_ERR_OPEN_FILE; 

    /* search for matched SECTION NAME */
    while (1)
    { 
        ret = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp, buf1, MAX_CFG_BUF); 
        if ( n < -1) 
            goto r_cfg_end; 
        ret = CFG_SECTION_NOT_FOUND; 
        if ( n < 0 ) 
            goto r_cfg_end;
        
        line_no++; 
        n = strlen(strtriml(strtrimr(buf1)));

        /* check if a null line ( clear all spaces ) or comment line */
        if (n == 0 || buf1[0] == s_cfg_nts)
            continue;

        /* check SECTION format : [ ... ] */
        ret = CFG_ERR_FILE_FORMAT;
        if (n > 2 && (buf1[0] != s_cfg_ssl || buf1[n-1] != s_cfg_ssr))
            continue;

        buf1[n-1] = 0x00;
        n = strlen(strtriml(strtrimr(buf1+1)));
        /* check if section name matched */
        if( n > 0 && 0 == strcmp(buf1+1, (char*)section) )
            break;
    }
    s_cfg_section_line_no = line_no;

    /* search for matched KEY NAME and get its value */
    while(1)
    { 
        ret = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp, buf1, MAX_CFG_BUF); 
        if (n < -1) 
            goto r_cfg_end; 
        ret = CFG_KEY_NOT_FOUND; 
        if (n < 0) 
            goto r_cfg_end;
        
        line_no++;
        s_cfg_key_line_no = line_no;
        s_cfg_key_lines = 1;
        n = strlen(strtriml(strtrimr(buf1)));

        /* check if a null line ( clear all spaces ) or comment line */
        if (n == 0 || buf1[0] == s_cfg_nts)
            continue;
        
        ret = CFG_KEY_NOT_FOUND; 
        if (buf1[0] == s_cfg_ssl) 
            goto r_cfg_end;

        /* '+' means the string connect to next line */
        if (buf1[n-1] == '+')
        {
            buf1[n-1] = 0x00;
            while(1)
            {
                ret = CFG_ERR_READ_FILE;
                n = FileGetLine(fp, buf2, MAX_CFG_BUF);
                if(n < -1)
                    goto r_cfg_end;
                if(n < 0)
                    break;
                
                line_no++;
                s_cfg_key_lines++;
                n = strlen(strtrimr(buf2));
                
                ret = CFG_ERR_EXCEED_BUF_SIZE;
                /* '+' means string connect to next line */
                if(n > 0 && buf2[n-1] == '+')
                {
                    buf2[n-1] = 0x00;
                    if(strlen(buf1) + strlen(buf2) > MAX_CFG_BUF)
                        goto r_cfg_end;
                    strcat(buf1, buf2);
                    continue;
                }

                /* check if buffer is full */
                if(strlen(buf1) + strlen(buf2) > MAX_CFG_BUF)
                    goto r_cfg_end;

                strcat(buf1, buf2);
                break;
            }
        }
        ret = CFG_ERR_FILE_FORMAT;

        /* get KEY and VAULE */
        if(SplitKeyValue(buf1, &key_ptr, &val_ptr) != 1) 
            goto r_cfg_end;
        strtriml(strtrimr(key_ptr));

        /* check if KEY NAME matched */
        if(strcmp(key_ptr, (char*)key) != 0)
            continue;

        /* get VALUE of the matched KEY NAME */
        strcpy((char*)buf, val_ptr);
        break;
    }

    ret = CFG_OK;
r_cfg_end:
    if(fp != NULL)
        fclose(fp);
    return ret;
} 

/*****************************************************************************/
// Description: set key value of a given section
// Global resource: none
// Author: yifeng.wang
// Note: 0 - OK ; not 0 - error
/*****************************************************************************/
PUBLIC int CONFIGFILE_SetKeyValue(char* file_path, void *section, void *key, void *buf)
{ 
    FILE *fp1, *fp2;
    char buf1[MAX_CFG_BUF + 1];
    int  line_no, line_no1, n, ret, ret2;
    char *tmpfname;
    
    ret = CONFIGFILE_GetKeyValue(file_path, section, key, buf1);
    if(ret <= CFG_ERR && ret != CFG_ERR_OPEN_FILE)
        return ret;
    
    if(ret == CFG_ERR_OPEN_FILE || ret == CFG_SECTION_NOT_FOUND) 
    {
        if((fp1 = fopen((char *)file_path, "a")) == NULL) 
            return CFG_ERR_CREATE_FILE; 
        
        if(fprintf(fp1, "%c%s%c\n", s_cfg_ssl, section, s_cfg_ssr) == EOF) 
        {
            fclose(fp1);
            return CFG_ERR_WRITE_FILE;
        }
        if(fprintf(fp1, "%s=%s\n", key, buf) == EOF)
        {
            fclose(fp1);
            return CFG_ERR_WRITE_FILE;
        }
        fclose(fp1);
        return CFG_OK;
    } 
    if((tmpfname = _tempnam(NULL, NULL)) == NULL)
        return CFG_ERR_CREATE_FILE;

    if((fp2 = fopen(tmpfname, "w")) == NULL)
        return CFG_ERR_CREATE_FILE;
    
    ret2 = CFG_ERR_OPEN_FILE;
    if((fp1 = fopen((char *)file_path, "rb")) == NULL)
        goto w_cfg_end;
    
    if(ret == CFG_KEY_NOT_FOUND) 
        line_no1 = s_cfg_section_line_no; 
    else /* ret = CFG_OK */ 
        line_no1 = s_cfg_key_line_no - 1;
    
    for(line_no = 0; line_no < line_no1; line_no++) 
    { 
        ret2 = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp1, buf1, MAX_CFG_BUF); 
        if(n < 0) goto w_cfg_end; 
        ret2 = CFG_ERR_WRITE_FILE; 
        if(fprintf(fp2, "%s\n", buf1) == EOF)
            goto w_cfg_end; 
    }
    
    if(ret != CFG_KEY_NOT_FOUND)
    {
        for( ; line_no < line_no1+s_cfg_key_lines; line_no++) 
        { 
            ret2 = CFG_ERR_READ_FILE; 
            n = FileGetLine(fp1, buf1, MAX_CFG_BUF); 
            if(n < 0) goto w_cfg_end; 
        }
    }
    
    ret2 = CFG_ERR_WRITE_FILE; 
    if(fprintf(fp2, "%s=%s\n", key, buf) == EOF)
        goto w_cfg_end;
        
    while(1)
    { 
        ret2 = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp1, buf1, MAX_CFG_BUF); 
        if(n < -1) goto w_cfg_end; 
        if(n < 0) break; 
        ret2 = CFG_ERR_WRITE_FILE; 
        if(fprintf(fp2, "%s\n", buf1) == EOF)
            goto w_cfg_end;
    }
    ret2 = CFG_OK;
    
w_cfg_end:
    if(fp1 != NULL) fclose(fp1); 
    if(fp2 != NULL) fclose(fp2); 
    if(ret2 == CFG_OK) 
    { 
        ret = FileCopy(tmpfname, file_path); 
        if(ret != 0) 
            return CFG_ERR_CREATE_FILE; 
    } 
    remove(tmpfname); 
    return ret2; 
}

