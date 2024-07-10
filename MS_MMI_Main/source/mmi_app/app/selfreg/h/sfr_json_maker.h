#ifndef _SFR_JSON_MAKER_H_
#define _SFR_JSON_MAKER_H_

#define JSON_START '{'
#define JSON_END '}'
#define JSON_SPLIT ','

void sfr_json_buff_init(void);

void sfr_json_buff_term(void);

BOOLEAN sfr_add_json_item(const char *_key, const char *_value);

void sfr_add_json_char(char ch);

void sfr_remove_last_split(void);

char *sfr_get_json_string(void);

#endif