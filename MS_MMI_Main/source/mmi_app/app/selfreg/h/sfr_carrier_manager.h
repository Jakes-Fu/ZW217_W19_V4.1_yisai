#ifndef __SFR_CARRIER_M_H__
#define __SFR_CARRIER_M_H__

typedef void * (*CONSTRUCT_FUNC) (void);

typedef void (*CLEAN_FUNC) (void *ctx);

typedef char * (*GET_STRING_FUNC) (void *ctx, int *len);

typedef void (*SAVE_REG_FUNC) (void *ctx);

typedef BOOLEAN (*PARSE_STRING_FUNC) (void *ctx, char *string, int *len);

typedef int (*GET_LOOP_FUNC) (void);

typedef struct _SFR_CARRIER
{
    char *carr_name;
    GET_LOOP_FUNC carr_reg_loop;
    void *carr_context;
    CONSTRUCT_FUNC carrier_constructer;
    SAVE_REG_FUNC carrier_saver;
    CLEAN_FUNC carier_free;
    GET_STRING_FUNC carrier_get_url;
    GET_STRING_FUNC carrier_get_accept;
    GET_STRING_FUNC carrier_get_agent;
    GET_STRING_FUNC carrier_get_bodytype;
    GET_STRING_FUNC carrier_get_package;
    PARSE_STRING_FUNC carrier_return_parser;
} SFR_CARRIER;

SFR_CARRIER *SFRM_GetFirstRegCarrier(void);
SFR_CARRIER *SFRM_GetNextRegCarrier(void);

#endif //__SFR_CARRIER_M_H__
