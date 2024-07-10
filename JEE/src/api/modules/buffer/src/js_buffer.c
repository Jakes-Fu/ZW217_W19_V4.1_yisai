/*
 * Licensed under the Apache License, Version 2.0 (the "License")
 * Copyright (c) 2016-2018, Intel Corporation.
 *
 * ported from zephyr.js
 * COPYRIGHT (C) 2018, RT-Thread Development Team
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "js_buffer.h"


#define strequal(a, b) !strcmp(a, b)


#define DECL_FUNC_ARGS(name, ...)                                            \
    jerry_value_t name(const jerry_value_t function_obj_val,                     \
                       const jerry_value_t this_val, const jerry_value_t args_p[], \
                       const jerry_length_t args_cnt, __VA_ARGS__)

static jerry_value_t jerry_buffer_prototype;
js_buffer_t *jerry_buffer_find(const jerry_value_t obj);

static void jerry_buffer_callback_free(void *handle)
{
    js_buffer_t *item = (js_buffer_t *)handle;

	art_free(item->buffer);
	art_free(item);
}

static const jerry_object_native_info_t buffer_type_info =
{
    .free_cb = jerry_buffer_callback_free
};

bool jerry_value_is_buffer(const jerry_value_t value)
{
    if (jerry_value_is_object(value) && jerry_buffer_find(value))
    {
        return true;
    }
    return false;
}

js_buffer_t *jerry_buffer_find(const jerry_value_t obj)
{
    // requires: obj should be the JS object associated with a buffer, created
    //             in jerry_buffer
    //  effects: looks up obj in our list of known buffer objects and returns
    //             the associated list item struct, or NULL if not found
    js_buffer_t *handle;

    if (jerry_get_object_native_pointer(obj, (void **)&handle, &buffer_type_info))
    {
        return handle;
    }

    return NULL;
}

static DECL_FUNC_ARGS(jerry_buffer_read_bytes, int bytes, bool big_endian, bool sign)
{
    // requires: this is a JS buffer object created with jerry_buffer_create,
    //             argv[0] should be an offset into the buffer, but will treat
    //             offset as 0 if not given, as node.js seems to
    //           bytes is the number of bytes to read (expects 1, 2, or 4)
    //           big_endian true reads the bytes in big endian order, false in
    //             little endian order
    //  effects: reads bytes from the buffer associated with this JS object, if
    //             found, at the given offset, if within the bounds of the
    //             buffer; otherwise returns an error

    // args: offset
    // ZJS_VALIDATE_ARGS(Z_OPTIONAL Z_NUMBER);

    JERRY_CHECK_ARGS(1, number);
    uint32_t offset = JERRY_GET_ARG(0, number);

    js_buffer_t *buf = jerry_buffer_find(this_val);
    if (!buf)
        return jerry_create_undefined();

    if (offset + bytes > buf->bufsize)
        return jerry_create_undefined();

    int dir = big_endian ? 1 : -1;
    if (!big_endian)
        offset += bytes - 1;  // start on the big end

    uint32_t value = 0, i;
    for (i = 0; i < bytes; i++)
    {
        value <<= 8;
        value |= buf->buffer[offset];
        offset += dir;
    }

    if (sign)
    {
        if (bytes == 1)
            return jerry_create_number((int8_t)value);
        else if (bytes == 2)
            return jerry_create_number((int16_t)value);
        else if (bytes == 4)
            return jerry_create_number((int32_t)value);
    }

    return jerry_create_number(value);
}

static DECL_FUNC_ARGS(jerry_buffer_write_bytes, int bytes, bool big_endian)
{
    // requires: this is a JS buffer object created with jerry_buffer_create,
    //             argv[0] must be the value to be written, argv[1] should be
    //             an offset into the buffer, but will treat offset as 0 if not
    //             given, as node.js seems to
    //           bytes is the number of bytes to write (expects 1, 2, or 4)
    //           big_endian true writes the bytes in big endian order, false in
    //             little endian order
    //  effects: writes bytes into the buffer associated with this JS object, if
    //             found, at the given offset, if within the bounds of the
    //             buffer and returns the offset just beyond what was written;
    //             otherwise returns an error

    // args: value[, offset]
    // ZJS_VALIDATE_ARGS(Z_NUMBER, Z_OPTIONAL Z_NUMBER);

    JERRY_CHECK_ARGS(1, number);
    JERRY_CHECK_ARG_IF_EXIST(1, number);

    // technically negatives aren't supported but this makes them behave better
    double dval = JERRY_GET_ARG(0, number);
    uint32_t value = (uint32_t)(dval < 0 ? (int32_t)dval : dval);

    uint32_t offset = 0;
    if (args_cnt > 1)
    {
        offset = (uint32_t)JERRY_GET_ARG(1, number);;
    }

    js_buffer_t *buf = jerry_buffer_find(this_val);
    if (!buf)
    {
        return jerry_create_undefined();
    }

    uint32_t beyond = offset + bytes;
    if (beyond > buf->bufsize)
    {
        JS_DEBUG_LOG("bufsize %d, write attempted from %d to %d\n",  (int)buf->bufsize, (int)offset, (int)beyond);
        return jerry_create_undefined();
    }

    int dir = big_endian ? -1 : 1;
    if (big_endian)
        offset += bytes - 1;  // start on the little end

  int i;
    for (i = 0; i < bytes; i++)
    {
        buf->buffer[offset] = value & 0xff;
        value >>= 8;
        offset += dir;
    }

    return jerry_create_number(beyond);
}


DECLARE_HANDLER(readUInt8)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 1, true, false);
}

DECLARE_HANDLER(readInt8)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 1, true, true);
}

DECLARE_HANDLER(readUInt16BE)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 2, true, false);
}

DECLARE_HANDLER(readInt16BE)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 2, true, true);
}

DECLARE_HANDLER(readUInt16LE)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 2, false, false);
}

DECLARE_HANDLER(readInt16LE)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 2, false, true);
}

DECLARE_HANDLER(readUInt32BE)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 4, true, false);
}

DECLARE_HANDLER(readInt32BE)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 4, true, true);
}

DECLARE_HANDLER(readUInt32LE)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 4, false, false);
}

DECLARE_HANDLER(readInt32LE)
{
    return jerry_buffer_read_bytes(function_obj_val, this_val, args_p, args_cnt, 4, false, true);
}

DECLARE_HANDLER(writeUInt8)
{
    return jerry_buffer_write_bytes(function_obj_val, this_val, args_p, args_cnt, 1, true);
}

DECLARE_HANDLER(writeUInt16BE)
{
    return jerry_buffer_write_bytes(function_obj_val, this_val, args_p, args_cnt, 2, true);
}

DECLARE_HANDLER(writeUInt16LE)
{
    return jerry_buffer_write_bytes(function_obj_val, this_val, args_p, args_cnt, 2, false);
}

DECLARE_HANDLER(writeUInt32BE)
{
    return jerry_buffer_write_bytes(function_obj_val, this_val, args_p, args_cnt, 4, true);
}

DECLARE_HANDLER(writeUInt32LE)
{
    return jerry_buffer_write_bytes(function_obj_val, this_val, args_p, args_cnt, 4, false);
}

DECLARE_HANDLER(readFloatBE)
{
    return jerry_create_undefined();
}

DECLARE_HANDLER(readDoubleBE)
{
    return jerry_create_undefined();
}

char jerry_int_to_hex(int value)
{
    if (value < 10)
        return '0' + value;

    return 'A' + value - 10;
}

static int hex2int(const char *str)
{
    int i = 0;
    int r = 0;

    for (i = 0; i < 2; i++)
    {
        if (str[i] >= '0' && str[i] <= '9') r += str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'f') r += str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F') r += str[i] - 'A' + 10;

        if (!i) r *= 16;
    }

    return r;
}

/*
 * toString(encoding[opt], start[opt], end[opt])
 */
DECLARE_HANDLER(toString)
{
    int start = 0, end = 0;
    int optcount = args_cnt;

    JERRY_CHECK_ARG_IF_EXIST(0, string);
    JERRY_CHECK_ARG_IF_EXIST(1, number);
    JERRY_CHECK_ARG_IF_EXIST(2, number);

    js_buffer_t *buf = jerry_buffer_find(this_val);
    if (!buf) return jerry_create_undefined();

    if (args_cnt > 2) start = JERRY_GET_ARG(1, number);
    else start = 0;
    if (args_cnt > 2) end = JERRY_GET_ARG(2, number);
    else end = buf->bufsize;

    int size = 0;
    char *enc = NULL;
    const char *encoding = "utf8";
    if (optcount)
    {
        enc = js_value_to_string(args_p[0]);
        size = strlen(enc);
        if (!size)
        {
			art_free(enc);
            return jerry_create_undefined();
        }

        encoding = enc;
    }

    if (strequal(encoding, "utf8"))
    {
        jerry_char_t *str = (jerry_char_t *)buf->buffer;

        str += start;
		art_free(enc);
        if (jerry_str_is_utf8(str, end - start))
            return jerry_create_string_sz_from_utf8(str, end - start);
        else
            return jerry_create_undefined();
    }
    else if (strequal(encoding, "ascii"))
    {
        char *str = art_malloc(buf->bufsize);
        if (!str)
        {
			art_free(enc);
            return jerry_create_undefined();
        }

        int index;
        for (index = start; index < end; ++index)
        {
            // strip off high bit if present
            str[index - start] = buf->buffer[index] & 0x7f;
            if (!str[index - start])
                break;
        }

        jerry_value_t jstr;
        if (jerry_str_is_utf8(str, index - start))
            jstr = jerry_create_string_sz_from_utf8((jerry_char_t *)str, index - start);
        else
            jstr = jerry_create_undefined();
		art_free(str);
		art_free(enc);
        return jstr;
    }
    else if (strequal(encoding, "hex"))
    {
        if (buf && end - start > 0)
        {
            int size = (end - start);
            char *hexbuf = art_malloc(size * 2 + 1);
            if (hexbuf)
            {
        int i;
                jerry_value_t ret;

                for (i = 0; i < size; i++)
                {
                    int high = (0xf0 & buf->buffer[i + start]) >> 4;
                    int low = 0xf & buf->buffer[i + start];
                    hexbuf[2 * i] = jerry_int_to_hex(high);
                    hexbuf[2 * i + 1] = jerry_int_to_hex(low);
                }
                hexbuf[size * 2] = '\0';

				art_free(enc);

                ret = jerry_create_string((jerry_char_t *)hexbuf);

				art_free(hexbuf);

                return ret;
            }
            else
            {
				art_free(enc);
                return jerry_create_undefined();
            }
        }
    }
    else
    {
		art_free(enc);
        return jerry_create_undefined();
    }

	art_free(enc);
    return jerry_create_undefined();
}

DECLARE_HANDLER(jsonParse)
{
    int start, end;

    js_buffer_t *buf = jerry_buffer_find(this_val);
    if (!buf) return jerry_create_undefined();

    if (args_cnt > 0)
        start = jerry_get_number_value(args_p[0]);
    else
        start = 0;

    if (args_cnt > 1)
        end = jerry_get_number_value(args_p[1]);
    else
        end = buf->bufsize;

    if (end - start > 0)
    {
        jerry_value_t parsed_json;
        jerry_char_t *str = (jerry_char_t *)buf->buffer;

        str += start;

        parsed_json = jerry_json_parse(str, end - start);
        if (jerry_value_is_error(parsed_json))
        {
            jerry_release_value(parsed_json);
            return jerry_create_undefined();
        }
        else
            return parsed_json;
    }

    return jerry_create_undefined();
}

DECLARE_HANDLER(concat)
{
    if (args_cnt == 1)
    {
        uint8_t *ptr;

        js_buffer_t *source = jerry_buffer_find(this_val);
        js_buffer_t *target = jerry_buffer_find(args_p[0]);
        if (!source || !target)
        {
            return jerry_create_undefined();
        }

        if (target->bufsize == 0 || source->bufsize == 0) return this_val;

        ptr = realloc(source->buffer, source->bufsize + target->bufsize);
        if (ptr)
        {
            source->buffer = ptr;

            memcpy(&source->buffer[source->bufsize], target->buffer, target->bufsize);
            source->bufsize += target->bufsize;
            jerry_jval_set_property_jval(this_val, "length", jerry_create_number(source->bufsize));
        }
    }

    return jerry_create_undefined();
}

DECLARE_HANDLER(copy)
{
    // requires: this must be a JS buffer object and args[0] a target buffer
    //             object
    //  effects: copies buffer contents w/ optional offsets considered to the
    //             target buffer (following Node v8.9.4 API)

    // args: target, [targetStart], [sourceStart], [sourceEnd]
    // ZJS_VALIDATE_ARGS_OPTCOUNT(optcount, Z_BUFFER,
    //                           Z_OPTIONAL Z_NUMBER Z_UNDEFINED,
    //                           Z_OPTIONAL Z_NUMBER Z_UNDEFINED,
    //                           Z_OPTIONAL Z_NUMBER Z_UNDEFINED);

    int optcount = args_cnt;

    if (optcount <= 0)
        return jerry_create_undefined();

    js_buffer_t *source = jerry_buffer_find(this_val);
    js_buffer_t *target = jerry_buffer_find(args_p[0]);
    if (!source || !target)
    {
        return jerry_create_undefined();
    }

    int targetStart = 0;
    int sourceStart = 0;
    int sourceEnd = -1;
    if (optcount > 1 && !jerry_value_is_undefined(args_p[1]))
    {
        targetStart = (int)jerry_get_number_value(args_p[1]);
    }
    if (optcount > 2 && !jerry_value_is_undefined(args_p[2]))
    {
        sourceStart = (int)jerry_get_number_value(args_p[2]);
    }
    if (optcount > 3 && !jerry_value_is_undefined(args_p[3]))
    {
        sourceEnd = (int)jerry_get_number_value(args_p[3]);
    }

    if (sourceEnd == -1)
    {
        sourceEnd = source->bufsize;
    }

    if (targetStart < 0 || targetStart >= target->bufsize ||
            sourceStart < 0 || sourceStart >= source->bufsize ||
            sourceEnd < 0 || sourceEnd <= sourceStart ||
            sourceEnd > source->bufsize)
    {
        return jerry_create_undefined();
    }

    if (sourceEnd - sourceStart > target->bufsize - targetStart)
    {
        return jerry_create_undefined();
    }

    int len = sourceEnd - sourceStart;
    memcpy(target->buffer + targetStart, source->buffer + sourceStart, len);
    return jerry_create_number(len);
}

DECLARE_HANDLER(write)
{
    // requires: string - what will be written to buf
    //           offset - where to start writing (Default: 0)
    //           length - how many bytes to write (Default: buf.length -offset)
    //           encoding - the character encoding of string. Currently only
    //             supports the default of utf8
    //  effects: writes string to buf at offset according to the character
    //             encoding in encoding.

    // args: data[, offset[, length[, encoding]]]
    // ZJS_VALIDATE_ARGS(Z_STRING, Z_OPTIONAL Z_NUMBER, Z_OPTIONAL Z_NUMBER,
    //                  Z_OPTIONAL Z_STRING);

    js_buffer_t *buf = jerry_buffer_find(this_val);
    if (!buf)
    {
        return jerry_create_undefined();
    }

    // Check if the encoding string is anything other than utf8
    if (args_cnt > 3)
    {
        char *encoding = js_value_to_string(args_p[3]);
        if (!encoding)
        {
            return jerry_create_undefined();
        }

        // ask for one more char than needed to make sure not just prefix match
        const char *utf8_encoding = "utf8";
        int utf8_len = strlen(utf8_encoding);
        int rval = strncmp(encoding, utf8_encoding, utf8_len + 1);
		art_free(encoding);
        if (rval != 0)
        {
            return jerry_create_undefined();
        }
    }

    char *str = js_value_to_string(args_p[0]);
    if (!str)
    {
        return jerry_create_undefined();
    }
    jerry_size_t size = strlen(str);

    uint32_t offset = 0;
    if (args_cnt > 1)
        offset = (uint32_t)jerry_get_number_value(args_p[1]);

    uint32_t length = buf->bufsize - offset;
    if (args_cnt > 2)
        length = (uint32_t)jerry_get_number_value(args_p[2]);

    if (length > size)
    {
		art_free(str);
        return jerry_create_undefined();
    }

    if (offset + length > buf->bufsize)
    {
		art_free(str);
        return jerry_create_undefined();
    }

    memcpy(buf->buffer + offset, str, length);
	art_free(str);

    return jerry_create_number(length);
}

DECLARE_HANDLER(fill)
{
    // requires: value - what will be written to buf
    //           offset - where to start writing (Default: 0)
    //           end - offset at which to stop writing (Default: buf.length)
    //           encoding - the character encoding of value. Currently only
    //             supports the default of utf8
    //  effects: writes string to buf at offset according to the character
    //             encoding in encoding.

    // args: data[, offset[, length[, encoding]]]
    // ZJS_VALIDATE_ARGS(Z_STRING Z_NUMBER Z_BUFFER, Z_OPTIONAL Z_NUMBER,
    //                  Z_OPTIONAL Z_NUMBER, Z_OPTIONAL Z_STRING);

    // TODO: support encodings other than 'utf8'

    uint32_t num;
    char *source = NULL;
    char *str = NULL;
    uint32_t srclen = 0;

    if (jerry_value_is_number(args_p[0]))
    {
        uint32_t srcnum = (uint32_t)jerry_get_number_value(args_p[0]);

        // convert in case of endian difference
        source = (char *)&num;
        source[0] = (0xff000000 & srcnum) >> 24;
        source[1] = (0x00ff0000 & srcnum) >> 16;
        source[2] = (0x0000ff00 & srcnum) >> 8;
        source[3] = 0x000000ff & srcnum;
        srclen = sizeof(uint32_t);
    }
    else if (jerry_value_is_buffer(args_p[0]))
    {
        js_buffer_t *srcbuf = jerry_buffer_find(args_p[0]);
        source = (char *)srcbuf->buffer;
        srclen = srcbuf->bufsize;
    }

    js_buffer_t *buf = jerry_buffer_find(this_val);
    if (!buf)
    {
        return jerry_create_undefined();
    }

    // Check if the encoding string is anything other than utf8
    if (args_cnt > 3)
    {
        char *encoding = js_value_to_string(args_p[3]);
        if (!encoding)
        {
            return jerry_create_undefined();
        }

        // ask for one more char than needed to make sure not just prefix match
        const char *utf8_encoding = "utf8";
        int utf8_len = strlen(utf8_encoding);
        int rval = strncmp(encoding, utf8_encoding, utf8_len + 1);
		art_free(encoding);
        if (rval != 0)
        {
            return jerry_create_undefined();
        }
    }

    uint32_t offset = 0;
    if (args_cnt > 1)
        offset = (uint32_t)jerry_get_number_value(args_p[1]);

    uint32_t end = buf->bufsize;
    if (args_cnt > 2)
    {
        end = (uint32_t)jerry_get_number_value(args_p[2]);
        if (end > buf->bufsize)
        {
            end = buf->bufsize;
        }
    }

    if (offset >= end)
    {
        // nothing to do
        return jerry_acquire_value(this_val);
    }

    if (jerry_value_is_string(args_p[0]))
    {
        char *str = js_value_to_string(args_p[0]);
        if (!str)
        {
            return jerry_create_undefined();
        }
        source = str;
        srclen = strlen(str);
    }

    uint32_t bytes_left = end - offset;
    while (bytes_left > 0)
    {
        uint32_t bytes = srclen;
        if (bytes > bytes_left)
        {
            bytes = bytes_left;
        }
        memcpy(buf->buffer + offset, source, bytes);
        offset += bytes;
        bytes_left -= bytes;
    }

	art_free(str);
    return jerry_acquire_value(this_val);
}

jerry_value_t jerry_buffer_create(uint32_t size, js_buffer_t **ret_buf)
{
    // follow Node's Buffer.kMaxLength limits though we don't expose that
    uint32_t maxLength = (1UL << 31) - 1;
    if (sizeof(size_t) == 4)
    {
        // detected 32-bit architecture
        maxLength = (1 << 30) - 1;
    }

    if (size > maxLength)
    {
        JS_DEBUG_LOG("size: %d\n", (int)size);
        return jerry_create_undefined();
    }

    void *buf = art_malloc(size);
    js_buffer_t *buf_item = (js_buffer_t *)art_malloc(sizeof(js_buffer_t));

    if (!buf || !buf_item)
    {
		art_free(buf);
		art_free(buf_item);
        if (ret_buf)
        {
            *ret_buf = NULL;
        }
        return jerry_create_undefined();
    }

    jerry_value_t buf_obj = jerry_create_object();
    buf_item->buffer = buf;
    buf_item->bufsize = size;

    jerry_set_prototype(buf_obj, jerry_buffer_prototype);
    jerry_value_t length = jerry_create_number(size);
    jerry_jval_set_property_jval(buf_obj, "length", length);
    jerry_release_value(length);

    // watch for the object getting garbage collected, and clean up
    jerry_set_object_native_pointer(buf_obj, buf_item, &buffer_type_info);
    if (ret_buf)
    {
        *ret_buf = buf_item;
    }

    return buf_obj;
}

#define ENCODING_UTF8   0
#define ENCODING_ASCII  1
#define ENCODING_HEX    2
#define ENCODING_BASE64 3

int buffer_encoding_type(const char* encoding)
{
    int ret = ENCODING_UTF8;

    if (strequal(encoding, "hex")) ret = ENCODING_HEX;
    else if (strequal(encoding, "ascii")) ret = ENCODING_ASCII;
    else if (strequal(encoding, "utf8")) ret = ENCODING_UTF8;
    else if (strequal(encoding, "base64")) ret = ENCODING_BASE64;

    return ret;
}

/*
 * Buffer(number);
 * Buffer(string, encoding[opt]);
 * Buffer(array);
 * Buffer(object);
 */
DECLARE_HANDLER(Buffer)
{
    int encoding_type = ENCODING_UTF8;

    if (args_cnt > 1)
    {
        char *encoding = js_value_to_string(args_p[1]);
        if (encoding)
        {
            encoding_type = buffer_encoding_type(encoding);

			art_free(encoding);
        }
    }

    if (jerry_value_is_number(args_p[0]))
    {
        double dnum = jerry_get_number_value(args_p[0]);
        uint32_t unum;

        if (dnum < 0)
        {
            unum = 0;
        }
        else if (dnum > 0xffffffff)
        {
            unum = 0xffffffff;
        }
        else
        {
            // round to the nearest integer
            unum = (uint32_t)(dnum + 0.5);
        }

        // treat a number argument as a length
        return jerry_buffer_create(unum, NULL);
    }
    else if (jerry_value_is_string(args_p[0]))
    {
        uint8_t *data = NULL;
        char *str = js_value_to_string(args_p[0]);
        if (!str)
        {
            return jerry_create_undefined();
        }

        js_buffer_t *buf;
        jerry_size_t size = strlen(str);

        if (encoding_type == ENCODING_HEX)
        {
            data = art_malloc(size / 2);
            if (data)
            {
                int index;
                for (index = 0; index < size / 2; index++)
                {
                    data[index] = hex2int(&str[index * 2]);
                }
            }

            size = size / 2;
        }
        else
        {
            data = (uint8_t *)str;
        }

        jerry_value_t new_buf = jerry_buffer_create(size, &buf);
        if (buf)
        {
            memcpy(buf->buffer, data, size);
        }

        if ((char*)data != str) art_free(data);
		art_free(str);

        return new_buf;
    }
    else if (jerry_value_is_array(args_p[0]))
    {
        // treat array argument as byte initializers
        jerry_value_t array = args_p[0];
        uint32_t len = jerry_get_array_length(array);

        js_buffer_t *buf;
        jerry_value_t new_buf = jerry_buffer_create(len, &buf);
        if (buf)
        {
      int i;
            for (i = 0; i < len; i++)
            {
                jerry_value_t item = jerry_get_property_by_index(array, i);
                if (jerry_value_is_number(item))
                {
                    buf->buffer[i] = (uint8_t)jerry_get_number_value(item);
                }
                else
                {
                    JS_DEBUG_LOG("non-numeric value in array, treating as 0\n");
                    buf->buffer[i] = 0;
                }
            }
        }
        return new_buf;
    }
    else if (jerry_value_is_object(args_p[0]))
    {
        // jerry_value_t stringified = jerry_json_stringfy(args_p[0]);
        // if (!jerry_value_is_error(stringified))
        // {
        //     char *json_string = js_value_to_string(stringified);
        //     if (json_string)
        //     {
        //         js_buffer_t *buf;
        //         jerry_value_t new_buf = jerry_buffer_create(strlen(json_string), &buf);
        //         if (buf)
        //         {
        //             memcpy(buf->buffer, json_string, strlen(json_string));
        //         }
        //         free(json_string);
        //         jerry_release_value(stringified);
        //         return new_buf;
        //     }
        // }
        // jerry_release_value(stringified);
    }

    return jerry_create_undefined();
}

int js_buffer_cleanup(void)
{
    jerry_release_value(jerry_buffer_prototype);
    return 0;
}

int jerry_init_buffer(void)
{
    REGISTER_GLOBAL_HANDLER(Buffer);

    //jerry_buffer_prototype = jerry_create_object();
    jerry_buffer_prototype = jerry_get_global_object();

    REGISTER_METHOD(jerry_buffer_prototype, readUInt8);
    REGISTER_METHOD(jerry_buffer_prototype, readInt8);
    REGISTER_METHOD(jerry_buffer_prototype, writeUInt8);
    REGISTER_METHOD(jerry_buffer_prototype, readUInt16BE);
    REGISTER_METHOD(jerry_buffer_prototype, readInt16BE);
    REGISTER_METHOD(jerry_buffer_prototype, writeUInt16BE);
    REGISTER_METHOD(jerry_buffer_prototype, readUInt16LE);
    REGISTER_METHOD(jerry_buffer_prototype, readInt16LE);
    REGISTER_METHOD(jerry_buffer_prototype, writeUInt16LE);
    REGISTER_METHOD(jerry_buffer_prototype, readUInt32BE);
    REGISTER_METHOD(jerry_buffer_prototype, readInt32BE);
    REGISTER_METHOD(jerry_buffer_prototype, writeUInt32BE);
    REGISTER_METHOD(jerry_buffer_prototype, readUInt32LE);
    REGISTER_METHOD(jerry_buffer_prototype, readInt32LE);
    REGISTER_METHOD(jerry_buffer_prototype, writeUInt32LE);
    REGISTER_METHOD(jerry_buffer_prototype, readFloatBE);
    REGISTER_METHOD(jerry_buffer_prototype, readDoubleBE);
    REGISTER_METHOD(jerry_buffer_prototype, copy);
    REGISTER_METHOD(jerry_buffer_prototype, fill);
    REGISTER_METHOD(jerry_buffer_prototype, toString);
    REGISTER_METHOD(jerry_buffer_prototype, jsonParse);
    REGISTER_METHOD(jerry_buffer_prototype, write);
    REGISTER_METHOD(jerry_buffer_prototype, concat);
	jerry_release_value(jerry_buffer_prototype);
    return 0;
}
