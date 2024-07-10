#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "OptYacc.h"

/* struct for the delete program part */
extern char *atc_c_buf_p;
int atclex(void);
#define SCI_ASSERT(_bool)  assert(_bool)

/* struct for the replace program part */
extern FILE *yyin;
extern char *yytext;
int          yylex();

void OptYacc_Delete( 
		const char * in_file_name,
		const char * out_file_name
		);

void OptYacc_Replace( 
		const char * in_file_name,
		const char * out_file_name
		);

int main(int argc, char* argv[])
{
	int     r_pattern = 0;

    if ( 3 != argc ) {
        printf("Error! No input file");
        return 1;
    }

	OptYacc_Delete( argv[1], "atc_yacc.tmp.c" );

	OptYacc_Replace( "atc_yacc.tmp.c", argv[2] );
    return 0;
}

void OptYacc_Delete( 
		const char * in_file_name,
		const char * out_file_name
		)
{
    int     match_pattern = 0;
    int     last_pattern = 0;

	int     d_lines[20] = 
			{ 0, 0, 0, 0, 0, 0, 0, 88, 140, 23 ,
			  83, 87, 22, 0, 0, 0, 0, 0, 0, 0 }; 

    int     tmp_char = 0;
    int     index = 0;
    int     endif_num = 1;

    char    line_buf[1024];

    FILE    * fin_ptr = NULL;
    FILE    * fout_ptr = NULL;

    int     j = 0;

	/*
	to delete the following code for clearing pclint warning:
        ATC_STATUS  parse_status = S_ATC_SUCCESS;


        typedef union          {
                                uint                    cmd_index;
                                ATC_CUR_PARAM_INFO_LIST_T * cur_param_ptr;
                                ATC_CUR_INFO_LIST_T   * atc_cur_info_ptr;
                               } ATCSTYPE;
        #include <stdio.h>

        #ifndef __cplusplus
        #ifndef __STDC__
        #define const
        #endif
        #endif
	*/
    unsigned char skim_status = 0; //0 checking target; 1 under skimming process; 2 skimemd target
	unsigned char skim_num = 0; // need to skim 13 lines continusely
	char          target_line[]= "typedef union          {";

    SCI_ASSERT( NULL != in_file_name );
    SCI_ASSERT( NULL != out_file_name );

    fin_ptr = fopen( in_file_name, "r" );
    if ( NULL == fin_ptr )
    {
        printf( "Error! Can not open input file.\n" );
        exit(1);
    }

    fout_ptr = fopen( out_file_name, "w+" );
    if ( NULL == fout_ptr )
    {
        printf( "Error! Can not create output file.\n" );
        fclose( fin_ptr );
        exit(1);
    }

    index = 0;
    tmp_char = fgetc( fin_ptr );
    while ( EOF != tmp_char )
    {
        line_buf[index] = tmp_char;
        while ( '\n' != line_buf[index] )
        {
            line_buf[++index] = fgetc( fin_ptr );
        } // End of while ( '\n' != line_buf[index] )

        /* Add two NULL-teminate characters for lexical analyzer */
        line_buf[++index] = '\0'; line_buf[++index] = '\0';

        switch (skim_status)
        {
        case 0:
            //checking the targeted line
            if (!strncmp(line_buf, target_line, strlen(target_line)))
            {
                skim_num++; 
                skim_status = 1; // change status into 1
                goto search_match;
            }
            break;

        case 1:
            if (skim_num <= 13)
            {
                skim_num++; 
                goto search_match; // skimming targeted line continusely
            }
            else
            {
                skim_status = 2; // change status into 2
            }
            break;

        case 2:  // stopping checking and skimming current line
        default:
            break;
        }
        
        atc_c_buf_p = line_buf;
        match_pattern = atclex();

        while ( 0 != match_pattern )
        {
            switch ( match_pattern )
            {
            case KEY_YYDEBUG:
                last_pattern = KEY_YYDEBUG;
                endif_num = 1;
                goto search_match;
                break;

            case KEY_DEF_YYPURE:
                last_pattern = KEY_DEF_YYPURE;
                endif_num = 1;
                goto search_match;
                break;

            case KEY_YYLSP_NEEDED:
                if ( KEY_DEF_YYPURE == last_pattern )
                {
                    endif_num++;
                    goto search_match;
                }
                last_pattern = KEY_YYLSP_NEEDED;
                endif_num = 1;
                goto search_match;
                break;

            case KEY_MICRO_BEGIN:
                if ( ( KEY_YYDEBUG == last_pattern ) ||
                     ( KEY_DEF_YYPURE == last_pattern ) ||
                     ( KEY_YYLSP_NEEDED == last_pattern ) )
                {
                    endif_num++;
                    goto search_match;
                }
                break;

            case KEY_ENDIF:
                if ( ( KEY_YYDEBUG == last_pattern ) ||
                     ( KEY_DEF_YYPURE == last_pattern ) ||
                     ( KEY_YYLSP_NEEDED == last_pattern ) )
                {
                    endif_num--;
                    if ( 0 == endif_num )
                    {
                        last_pattern = 0;
                    }

                    goto search_match;
                }
                break;

            case KEY_YYERRSTATUS:
                for(j=0;j<d_lines[match_pattern];j++)
                {
                    while ( '\n' != fgetc( fin_ptr ) );
                }
				fwrite("    return ATCY_ParseError(*info_ptr_ptr);\n", 1, 43, fout_ptr);
                last_pattern = 0;
                goto search_match;
                break;

            case KEY_YYMAXDEPTH:
                for(j=0;j<d_lines[match_pattern];j++)
                {
                    while ( '\n' != fgetc( fin_ptr ) );
                }
                fwrite("ATC_STATUS yyparse(uint8 * yybuf, ATC_CUR_INFO_LIST_T ** info_ptr_ptr)\n", 1, 72, fout_ptr );
                last_pattern = 0;
                goto search_match;
                break;
                
			case KEY_INT_YYLEN:
				fwrite(line_buf, 1, (index-1), fout_ptr );
				fwrite("    yy_c_buf_p = yybuf;\n", 1, 24, fout_ptr );
                last_pattern = 0;
                goto search_match;
				break;

            default:
                if ( (KEY_YYDEBUG == last_pattern) ||
                     (KEY_DEF_YYPURE == last_pattern) ||
                     (KEY_YYLSP_NEEDED == last_pattern) )
                {
                    goto search_match;
                }
                for(j=0;j<d_lines[match_pattern];j++)
                {
                    while ( '\n' != fgetc( fin_ptr ) );
                }
                last_pattern = 0;
                goto search_match;
                break;
            }

            /* Find the next match */
            match_pattern = atclex();
        } // End of while ( 0 != match_pattern )

        switch ( last_pattern ) {
        case KEY_YYDEBUG:
            break;

        case KEY_DEF_YYPURE:
            break;

        case KEY_YYLSP_NEEDED:
            break;

        default:
            fwrite( line_buf, 1, (index - 1), fout_ptr );
            break;
        }

search_match:
        tmp_char = fgetc( fin_ptr );
        index = 0;
    } // End of while ( EOF != tmp_char )

    fclose( fin_ptr );
    fclose( fout_ptr );

    return;
}

void OptYacc_Replace( 
		const char * in_file_name,
		const char * out_file_name
		)
{
	int		r_pattern = 0;

	FILE	* fin_ptr = NULL;
	FILE	* fout_ptr = NULL;

	char  * replace_str[MAX_R_NUM] = {
		"",
		"return 0",
		"atc",
		"ATC",
		"    ",
		"\n"
	};

    SCI_ASSERT( NULL != in_file_name );
    SCI_ASSERT( NULL != out_file_name );

    fin_ptr = fopen( in_file_name, "r" );
    if ( NULL == fin_ptr )
    {
        printf( "Error! Can not open input file.\n" );
        exit(1);
    }

    fout_ptr = fopen( out_file_name, "w+" );
    if ( NULL == fout_ptr )
    {
        printf( "Error! Can not create output file.\n" );
        fclose( fin_ptr );
        exit(1);
    }

    yyin = fin_ptr;

    fprintf(fout_ptr, "%s\n", "/*lint -e766*/ ");
    fprintf(fout_ptr, "%s\n", "/*lint -e529*/ ");
    fprintf(fout_ptr, "%s\n", "/*lint -e64*/ ");
    fprintf(fout_ptr, "%s\n", "/*lint -e18*/ ");

    r_pattern = yylex();
    while ( 0 != r_pattern ) {
        if ( R_OTHER_NOT_CHANGE != r_pattern ) {
            fwrite(replace_str[r_pattern], 1, strlen(replace_str[r_pattern]), fout_ptr );
        } else {
            fwrite(yytext, 1, strlen(yytext), fout_ptr );
        }
        r_pattern = yylex();
    }

    fprintf(fout_ptr, "%s\n", "/*lint +e18*/ ");
    fprintf(fout_ptr, "%s\n", "/*lint +e64*/ ");
    fprintf(fout_ptr, "%s\n", "/*lint +e529*/ ");
    fprintf(fout_ptr, "%s\n", "/*lint +e766*/ ");

    fclose( fin_ptr );
    fclose( fout_ptr );

	return;
}