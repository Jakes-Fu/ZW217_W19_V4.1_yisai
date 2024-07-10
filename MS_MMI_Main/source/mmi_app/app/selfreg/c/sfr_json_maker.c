#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_app.h"
#include "mmiphone_export.h"

#include "sfr_json_maker.h"



static int json_cursor;
static int json_buff_size;
static char *json_buff;

void sfr_json_buff_init(void)
{
    json_buff_size = 256;
    json_cursor = 0;
    json_buff = SCI_ALLOC_APPZ(json_buff_size);
}

void sfr_json_buff_term(void)
{
    json_buff_size = 0;
    json_cursor = 0;
    SCI_Free(json_buff);
}

static void json_buff_resize(void)
{
    char *_n;
    json_buff_size <<= 1;
    _n = SCI_ALLOC_APPZ(json_buff_size);

    memcpy(_n, json_buff, json_cursor + 1);
    SCI_FREE(json_buff);
    json_buff = _n;
    SCI_TraceLow("[Self Reg] json_buff_resize [%d]", json_buff_size);
}

BOOLEAN sfr_add_json_item(const char *_key, const char *_value)
{
    int l_key, l_val;

    if (_key == NULL || _value == NULL)
        return FALSE;

    l_key = strlen(_key);
    l_val = strlen(_value);

    // [string] + " + [string] + " + '\0'
    if (json_cursor + l_key + l_val + 5 + 1 >= json_buff_size)
    {
        json_buff_resize();
    }

    sprintf(&json_buff[json_cursor], "\"%s\":\"%s\"", _key, _value);

    json_cursor += (l_key + l_val + 5);
    return TRUE;
}

void sfr_add_json_char(char ch)
{
    // [string] + [char] + '\0'
    if (json_cursor + 1 + 1>= json_buff_size)
    {
        json_buff_resize();
    }

    json_buff[json_cursor] = ch;

    json_cursor ++;
}

void sfr_add_json_key(const char *_key)
{
    int l_key;

    if (_key == NULL)
        return;

    l_key = strlen(_key);

    // [string] + "  + '\0'
    if (json_cursor + l_key + 3 + 1 >= json_buff_size)
    {
        json_buff_resize();
    }

    sprintf(&json_buff[json_cursor], "\"%s\":", _key);

    json_cursor += (l_key + 3);
}

BOOLEAN sfr_add_json_array_start(const char *_key)
{
    int l_key;

    if (_key == NULL)
        return FALSE;

    l_key = strlen(_key);

    // [string] + : + [ + '\0'
    if (json_cursor + l_key + 4 + 1 >= json_buff_size)
    {
        json_buff_resize();
    }

    sprintf(&json_buff[json_cursor], "\"%s\":[", _key);

    json_cursor += (l_key + 4);
}

void sfr_add_json_array_end(void)
{
    sfr_add_json_char(']');
}

void sfr_remove_last_split(void)
{
    if (json_buff[json_cursor - 1] == JSON_SPLIT)
    {
        json_buff[json_cursor - 1] = '\0';
        json_cursor--;
    }
}

char *sfr_get_json_string(void)
{
    return json_buff;
}