/******************************************************************************
 ** File Name:      atp_atc_info.h                                            *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/15/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains MICROS and struct types that are of use*
 **                 to module parsing AT commands infomation definition file. *
 **                 We use a multi-level link to store the infomation of ATC. *
 **                                                                           *
 ** 				atc_info --- param_info --- num_param_info                *
 **		    		|              |            |                             *
 **		    		|              |            V next                        *
 **		    		|              |         num_param_info                   *
 **		    		|              V next                                     *
 **		    		|         param_info --- str_param_info                   *
 **		    		|              |            |                             *
 **		    		|              |            V next                        *
 **		    		|              .         str_param_info                   *
 **		    		|              .                                          *
 **		    		|              .                                          *
 **		    		|              V next                                     *
 **		    		|         param_info                                      *
 **		    		V next                                                    *
 **		 		    atc_info                                                  *
 **		    		|                                                         *
 **				    .                                                         *
 **		    		.                                                         *
 **		    		.                                                         *
 **		    		|                                                         *
 **		    		V next                                                    *
 **		 		    atc_info                                                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/15/2002     Steven.Yao       Create.                                   *
 ******************************************************************************/

#ifndef ATP_ATC_INFO_H
#define ATP_ATC_INFO_H

#define MAX_ATC_PARAM_COUNT    20

#define ATC_PARAM_TYPE_NUMERIC     1
#define ATC_PARAM_TYPE_STRING      2
#define ATC_PARAM_TYPE_NONE        3

typedef struct atc_param_numeric_struct {
    int     min_val; /* Minimum value of numeric parameter */
    int     max_val; /* Maximum value of numeric parameter */
}ATC_PARAM_NUMERIC_T;

typedef struct atc_param_string_struct {
    int     len;     /* Length of string parameter */
    char  * str;     /* Point to the string parameter, it is also 
                        zero-terminated */
}ATC_PARAM_STRING_T;

typedef struct atc_param_string_array_struct {
    ATC_PARAM_STRING_T      str;

    struct atc_param_string_array_struct *  next;
}ATC_PARAM_STRING_ARRAY_T;

typedef struct atc_param_numeric_array_struct {
    ATC_PARAM_NUMERIC_T     num;

    struct atc_param_numeric_array_struct * next;
}ATC_PARAM_NUMERIC_ARRAY_T;

typedef struct atc_param_info_struct {
    int     param_type; /* Parameter type: 1 - numeric
                                           2 - string    
                                           3 - none of them */
    int     param_range_num;

    union {
        ATC_PARAM_STRING_ARRAY_T  * str_head;
        ATC_PARAM_NUMERIC_ARRAY_T * num_head;
    };

    union {
        ATC_PARAM_STRING_ARRAY_T  * str_tail;
        ATC_PARAM_NUMERIC_ARRAY_T * num_tail;
    };

    struct atc_param_info_struct  * next;
}ATC_PARAM_INFO_T;

typedef struct atc_info_struct {
    int     cmd_index;
    int     cmd_type;
    ATC_PARAM_STRING_T  cmd_name;
    ATC_PARAM_STRING_T  cmd_name_no_plus;

    int     param_info_num;

    ATC_PARAM_INFO_T        * param_head;
    ATC_PARAM_INFO_T        * param_tail;

    struct atc_info_struct  * next;
}ATC_INFO_T;

ATC_INFO_T * atp_parse_atc_info_file( const char * );

/* function used in flex */
void atp_store_numeric_param(void);

void atp_store_string_param(void);

/* functions used in bison */
ATC_PARAM_NUMERIC_ARRAY_T * atp_store_one_numeric_param_range( 
        int, 
        int 
        );

ATC_PARAM_STRING_ARRAY_T * atp_store_one_string_param_range( 
        ATC_PARAM_STRING_T 
        );

ATC_PARAM_STRING_ARRAY_T * atp_link_string_param_ranges( 
        ATC_PARAM_STRING_ARRAY_T *, 
        ATC_PARAM_STRING_ARRAY_T * 
        );

ATC_PARAM_NUMERIC_ARRAY_T * atp_link_numeric_param_ranges( 
        ATC_PARAM_NUMERIC_ARRAY_T *, 
        ATC_PARAM_NUMERIC_ARRAY_T * 
        );

ATC_PARAM_INFO_T * atp_store_numeric_param_range( 
        ATC_PARAM_NUMERIC_ARRAY_T * 
        );

ATC_PARAM_INFO_T * atp_store_string_param_range( 
        ATC_PARAM_STRING_ARRAY_T * 
        );

ATC_PARAM_INFO_T * atp_store_unknown_param_range(void);

ATC_PARAM_INFO_T * atp_link_param_ranges( 
        ATC_PARAM_INFO_T *, 
        ATC_PARAM_INFO_T *
        );

ATC_INFO_T * atp_store_one_info_def( 
        ATC_PARAM_STRING_T, 
        int, 
        ATC_PARAM_INFO_T *
        );

ATC_INFO_T * atp_link_info_defs( 
        ATC_INFO_T *, 
        ATC_INFO_T * 
        );

void atp_store_all_defs( 
        ATC_INFO_T * 
        );

int atperror( char * s, ... );

int atplex(void);

unsigned int  Get_cmd_type(ATC_PARAM_STRING_T cmd_name);

#endif /* End of #ifndef ATP_ATC_INFO_H */
