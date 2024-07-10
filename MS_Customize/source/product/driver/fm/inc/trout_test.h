#ifndef _TROUT_TEST_H_
#define _TROUT_TEST_H_

#include <string.h>
#include <stdlib.h>
#include "sci_types.h"
//#include "..\inc\trout_main.h"

#define gets  	Uart_GetString

struct main_menu_handler
{
    const char 	*menu;
    void (*handler)();
    uint32	flag;
};

#define SCI_MEMSET(_DEST_PTR, _VALUE, _SIZE) \
        do\
        {\
            SCI_ASSERT(PNULL != _DEST_PTR); \
            if (_SIZE>0) { memset(_DEST_PTR, _VALUE, _SIZE);  }	\
        }while(0);


#define GET_USER_INPUT_QUIT_SIGNAL() \
do\
{\
	char input = UartGetChNoWait();\
	if (input EQ 'q' OR input EQ 'Q')\
	{\
		SCI_TraceLow("get user quit signal!!!\r\n");\
		return;\
	}\
}while(0)


#define GET_USER_INPUT_BREAK_SIGNAL() \
{\
	char input = UartGetChNoWait();\
	if (input EQ 'q' OR input EQ 'Q')\
	{\
		break;\
	}\
}

#define GET_USER_INPUT_BEGIN() \
do{\
    char buffer[256];

#define GET_USER_INPUT_PARAM_H(_value, _string) \
	SCI_MEMSET(buffer, 0, sizeof(buffer));\
	SCI_TraceLow("(HEX)%s", _string);\
	gets(buffer);\
	_value = str_to_hex(buffer);

#define GET_USER_INPUT_PARAM GET_USER_INPUT_PARAM_H

#define GET_USER_INPUT_PARAM_D(_value, _string) \
	SCI_MEMSET(buffer, 0, sizeof(buffer));\
	SCI_TraceLow("(DEC)%s", _string);\
	gets(buffer);\
	_value = atoi(buffer);

#define GET_USER_INPUT_CONFIRM(_format,...) \
	SCI_MEMSET(buffer, 0, sizeof(buffer));\
	SCI_TraceLow(_format, __VA_ARGS__);\
	SCI_TraceLow("are you sure [Y/N]:");\
	gets(buffer);\
	SCI_TraceLow("");\
	if (buffer[0] == 'Y' || buffer[0] == 'y')\
		break;\
}while(1)


#define CHECK_DATA_VALUE(_data, _min, _max) \
do\
{\
	SCI_ASSERT(_min<_max);\
	if ((_data)>(_max))_data=_max;\
	if ((_data)<(_min))_data=_min;\
}while(0)


typedef struct wifi_mac_reg_cfg
{
    uint32 reg_addr;
    uint32 reg_data;
} wifi_mac_reg_cfg_t;


typedef struct
{
    const uint32 start_addr;
    const uint32 base_addr;
    const uint32 offset;
    const uint32 access;
    const uint32 mask;
    const uint8 *mark;
    uint32 write_data;
    uint32 read_data;
} trout_test_reg_des_t;


typedef struct wifi_mac_reg_cfg wifi_phy_reg_cfg_t;
typedef struct wifi_mac_reg_cfg wifi_phy_agc_lut_cfg_t;

typedef BOOLEAN (*test_func)(void *);


void trout_print_menu_split_tag(char *menu);
uint32 str_to_hex(char *string);
uint32 trout_print_test_menu(struct main_menu_handler *menu, uint32 menu_size);
void main_test_func(struct main_menu_handler *menu_handler, uint32 menu_size);
#endif
