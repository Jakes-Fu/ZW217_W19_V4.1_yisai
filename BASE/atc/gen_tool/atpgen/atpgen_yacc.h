#ifndef ATPGEN_TAB_H
#define ATPGEN_TAB_H

#include "atp_atc_info.h"

typedef union { int                         val;
         ATC_PARAM_STRING_T          str_t;
         ATC_PARAM_NUMERIC_ARRAY_T * num_array_t;
         ATC_PARAM_STRING_ARRAY_T  * str_array_t;
         ATC_PARAM_INFO_T          * param_info_t;
         ATC_INFO_T                * info_t;
       } ATPSTYPE;
#define	ATC_DEF_NAME	257
#define	ATC_DEF_BEGIN	258
#define	ATC_DEF_END	259
#define	ATC_DEF_STRING	260
#define	ATC_DEF_NUMBER	261
#define	ATC_DEF_INFINITY	262


extern ATPSTYPE atplval;

#endif