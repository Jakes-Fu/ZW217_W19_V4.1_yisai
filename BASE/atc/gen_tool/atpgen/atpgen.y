%{
#include <stdio.h>
#include <stdlib.h>

#include "atp_atc_info.h"
%}

%no_lines

%token ATC_DEF_NAME ATC_DEF_BEGIN ATC_DEF_END
%token ATC_DEF_STRING ATC_DEF_NUMBER ATC_DEF_INFINITY

%union { int                         val;
         ATC_PARAM_STRING_T          str_t;
         ATC_PARAM_NUMERIC_ARRAY_T * num_array_t;
         ATC_PARAM_STRING_ARRAY_T  * str_array_t;
         ATC_PARAM_INFO_T          * param_info_t;
         ATC_INFO_T                * info_t;
       }

%type <val> ATC_DEF_NUMBER
%type <str_t> ATC_DEF_STRING ATC_DEF_NAME
%type <num_array_t> atc_range_number atc_range_numbers
%type <str_array_t> atc_range_string atc_range_strings
%type <param_info_t> atc_range_param atc_range_params atc_range
%type <info_t> atc_def atc_defs

%start atc_def_file

%%

atc_def_file      :   atc_defs
                      {
                        atp_store_all_defs($1);
                        YYACCEPT;
                      }
                  ;  

atc_defs          :   atc_defs atc_def
                      {
                        $$ = atp_link_info_defs($1, $2);
                      }
                  |   atc_def
                      {
                        $$ = atp_link_info_defs($1, NULL);
                      }
                  ;

atc_def           :   ATC_DEF_NAME ATC_DEF_BEGIN ATC_DEF_NUMBER ',' atc_range ATC_DEF_END
                      {
                        $$ = atp_store_one_info_def($1, $3, $5);
                      }
                  ;

atc_range         :   '\"' atc_range_params '\"'
                      {
                        $$ = $2;
                      }
                  |   '\"' '\"'
                      {
                        $$ = NULL;
                      }
                  ;

atc_range_params  :   atc_range_params ',' atc_range_param
                      {
                        $$ = atp_link_param_ranges($1, $3);
                      }
                  |   atc_range_param
                      {
                        $$ = atp_link_param_ranges($1, NULL);
                      }
                  ;

atc_range_param   :   '(' atc_range_numbers ')'
                      {
                        $$ = atp_store_numeric_param_range($2);
                      }
                  |   '(' atc_range_strings ')'
                      {
                        $$ = atp_store_string_param_range($2);
                      }
                  |   '(' ')'
                      {
                        $$ = atp_store_unknown_param_range();
                      }
                  ;

atc_range_numbers :   atc_range_numbers ',' atc_range_number
                      {
                        $$ = atp_link_numeric_param_ranges($1, $3);
                      }
                  |   atc_range_number
                      {
                        $$ = atp_link_numeric_param_ranges($1, NULL);
                      }
                  ;

atc_range_number  :   ATC_DEF_NUMBER ATC_DEF_NUMBER
                      {
                        if ($2 < 0)
                          $$ = atp_store_one_numeric_param_range($1, -$2);
                      }
                  |   ATC_DEF_NUMBER '-' ATC_DEF_NUMBER
                      {
                        $$ = atp_store_one_numeric_param_range($1, $3);
                      }
                  |   ATC_DEF_NUMBER
                      {
                        $$ = atp_store_one_numeric_param_range($1, $1);
                      }
                  ;

atc_range_strings :   atc_range_strings ',' atc_range_string
                      {
                        $$ = atp_link_string_param_ranges($1, $3);
                      }
                  |   atc_range_string
                      {
                        $$ = atp_link_string_param_ranges($1, NULL);
                      }
                  ;

atc_range_string  :   ATC_DEF_STRING
                      {
                        $$ = atp_store_one_string_param_range($1);
                      }
                  ;

%%
int yyerror( char * s, ... )
{
    printf("%s", s);
    return 0;
}
