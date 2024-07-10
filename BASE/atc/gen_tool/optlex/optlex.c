#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <assert.h>
//#include "sci_api.h"

#include "OptLex.h"

/* struct yy_buffer_state * yy_scan_buffer( char *base, int size ); */
/* struct for the delete program part */
extern char *atc_c_buf_p;
int		atclex();

/* struct for the replace program part */
extern FILE *yyin;
extern char *yytext;
int		yylex();

void OptLex_Delete(
		const char * in_file_name,
		const char * out_file_name
		);

void OptLex_Replace(
		const char * in_file_name,
		const char * out_file_name
		);

int		delete_line_num[PATTERN_NUM] = 
{ 0, 124, 143, 751, 0, 0, 0, 121, 1, 57, 23, 0, 0, 0, 0, 0, 0, 0, 71, 6,
  2, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int		add_line_num[PATTERN_NUM] = 
{ 0, 0, 1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 3, 0, 
  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

char *  flex_header_str[3] = 
{
	"#include \"sci_types.h\"\n",
//	"#include \"sdl_api.h\"\n",
	"#include \"atc_malloc.h\"\n",
	"\n"
};

char *   break_str[1] = 
{
	"    break;\n"
};

char *   end_of_buffer_str[2] =
{
	"	case YY_END_OF_BUFFER:\n",
    "        return 0;\n"
};

char *   yylex_str[1] =
{
	"int yylex(void)\n"
};

char *   fatal_error_str[1] = 
{
	"        return -1;"
};

char *	 terminate_str[1] =
{
	"    return 0;\n"
};

char *	 yy_c_buf_str[1] =
{
	"char *yy_c_buf_p = NULL;\n"
};

char *	 yy_start_str[1] =
{
	"static int yy_start = 1;\n"
};

char *	 yytext_str[1] =
{
	"    yytext = yy_bp; \\\n"
};

char  ** add_lines[PATTERN_NUM] = 
{ NULL, NULL, yylex_str, NULL, NULL, NULL, break_str, end_of_buffer_str, fatal_error_str, NULL, 
  NULL, NULL, terminate_str, NULL, yy_c_buf_str, yy_start_str, NULL, NULL, flex_header_str, NULL, 
  NULL, NULL, yytext_str, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };


char  *  replace_str[MAX_R_NUM] = {
	"",
	"const",
	"atc",
	"ATC",
	"uint8",
	"uint",
	"uint16",
	"uint",
	"    ",
	"\n"
};


int main(int argc, char* argv[])
{
	if ( 3 != argc ) { 
		printf("Error! No input commands file");
		return 1;
	}

	OptLex_Delete( argv[1], "atc_lex.tmp.c" );

	OptLex_Replace( "atc_lex.tmp.c", argv[2] );

	return 0;
}

void OptLex_Delete(
		const char * in_file_name,
		const char * out_file_name
		)
{
	char	line_buf[1024];
	char	former_line_buf[1024];
	int		index = 0;
	char	tmp_char = 0;

	int		lex_match_pattern = 0;

	int		j = 0;

	FILE	* fin_ptr = NULL;
	FILE	* fout_ptr = NULL;

	assert( NULL != in_file_name );
	assert( NULL != out_file_name );

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

	tmp_char = fgetc(fin_ptr);
	while ( EOF != tmp_char )
	{
		line_buf[index] = tmp_char;
		while ( '\n' != line_buf[index] )
		{
			line_buf[++index] = fgetc(fin_ptr);
		}
		line_buf[++index] = 0; line_buf[++index] = 0;
 
		atc_c_buf_p = line_buf;

		lex_match_pattern = atclex();
		if ( 0 != lex_match_pattern ) 
		{
			for ( j = 0;j < delete_line_num[lex_match_pattern];j++) {
				while ( '\n' != fgetc(fin_ptr) ) ;
			}

			for ( j = 0; j < add_line_num[lex_match_pattern]; j++ )
			{
                if (0 == strcmp((char *)(add_lines[lex_match_pattern][j]), (char *)break_str[0]))
                {
                    if (NULL != strstr(former_line_buf, "return"))
                    {
                        break;
                    }
                }
				fwrite(
					add_lines[lex_match_pattern][j], 
					1, 
					strlen(add_lines[lex_match_pattern][j]), 
					fout_ptr); 
			}
		}
		else
		{
			fwrite(line_buf, 1, (index-1), fout_ptr);
		}
        strcpy(former_line_buf, line_buf);
		tmp_char = fgetc(fin_ptr);
		index = 0;
	}

	fclose( fin_ptr);
	fclose( fout_ptr);

}

void OptLex_Replace(
		const char * in_file_name,
		const char * out_file_name
		)
{
	int  r_pattern = 0;    /* replace pattern */

	FILE	* fin_ptr = NULL;
	FILE	* fout_ptr = NULL;

	assert( NULL != in_file_name );
	assert( NULL != out_file_name );

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

    fprintf(fout_ptr, "%s\n", "/*lint -e18*/ ");
    fprintf(fout_ptr, "%s\n", "/*lint -e716*/ ");

	r_pattern = yylex();
	while ( 0 != r_pattern ) {
		if ( R_OTHER_NOT_CHANGE != r_pattern ) {
			fwrite(replace_str[r_pattern], 1, strlen(replace_str[r_pattern]), fout_ptr);
		} else {
			fwrite(yytext, 1, strlen(yytext), fout_ptr);
		}
		r_pattern = yylex();
	}

    fprintf(fout_ptr, "%s\n", "/*lint +e716*/ ");
    fprintf(fout_ptr, "%s\n", "/*lint +e18*/ ");

	fclose(fin_ptr);
	fclose(fout_ptr);

	return;
}