/*
    json.h - public domain - Micha Mettke

ABOUT:
    This is a single header JSON parser header and implementation without
    any dependencies (even the standard library),
    string memory allocation or complex tree generation.
    Instead this library focuses on parsing tokens from a previously
    loaded in memory JSON file. Each token thereby references the JSON file
    string and limits the allocation to the initial JSON file instead of
    allocating a new string for each token.

QUICK:
    To use this file do:
    #define JSON_IMPLEMENTATION
    before you include this file in *one* C or C++ file to create the implementation

    If you want to keep the implementation in that file you have to do
    #define JSON_STATIC before including this file

    If you want to use asserts to add validation add
    #define JSON_ASSERT before including this file

    To overwrite the default seperator character used inside
    the query functions
    #define JSON_DELIMITER (character) before including this file

USAGE:
    This file behaves differently depending on what symbols you define
    before including it.

    Header-File mode:
    If you do not define JSON_IMPLEMENTATION before including this file, it
    will operate in header only mode. In this mode it declares all used structs
    and the API of the library without including the implementation of the library.

    Implementation mode:
    If you define JSON_IMPLEMENTATIOn before including this file, it will
    compile the implementation of the JSON parser. To specify the visibility
    as private and limit all symbols inside the implementation file
    you can define JSON_STATIC before including this file.
    Make sure that you only include this file implementation in *one* C or C++ file
    to prevent collisions.

EXAMPLES:*/
#if 0
    /* Parser example */
    const char *json = "{...}";
    size_t len = strlen(json);

    /* load content into token array */
    struct json_parser p = {0};
    while (json_load(&p, json, len))
        p.toks = realloc(p.toks, (size_t)p.cap * sizeof(struct json_token));

    /* query token */
    struct json_token *t0 = json_query(p.toks, p.cnt, "map.entity[4].position");

    /* query string */
    size_t size;
    char buffer[64];
    json_query_string(buffer, 64, &size, p.toks, p.cnt, "map.entity[4].name");

    /* query number */
    json_number num;
    json_query_number(&num, p.toks, p.cnt, "map.soldier[2].position.x");

    /* query type */
    int type0 = json_query_number(p.toks, p.cnt, "map.soldier[2]");

    /* sub-queries */
    json_token *entity = json_query(p.toks, p.cnt, "map.entity[4]");
    json_token *position = json_query(entity, entity->sub, "position");
    json_token *rotation = json_query(entity, entity->sub, "rotation");

    /* iterate root */
    struct json_token *elm = p.toks;
    while (elm < p.toks + p.cnt) {
        if (json_cmp_internal(&elm[0], "name") == 0)
            ret = json_convert_internal(&num, &elm[1]);
        elm = json_obj_next(elm);
    }

    /* iterate over map pairs */
    struct json_token *m = json_query(p.toks, p.cnt, "map.soldier[2]");
    struct json_token *elm = json_obj_begin(m);
    for (i = 0; i < m->children && elm; ++i) {
        if (json_cmp_internal(&elm[0], "a") == 0) {
            ret = json_convert_internal(&num, &elm[1]);
            /* ... */
        }
        elm = json_obj_next(elm);
    }

    /* iterate array elements */
    struct json_token *a = json_query(p.toks, p.cnt, "map.entities");
    struct json_token *ent = json_array_begin(a);
    for (i = 0; i < a->children && ent; ++i) {
        struct json_token *pos = json_query(ent, ent->sub, "position");
        /*... */
        ent = json_array_next(ent);}
    }
#endif
 /* ===============================================================
 *
 *                          HEADER
 *
 * =============================================================== */
//#pragma once
#ifndef JSON_H_
#define JSON_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef JSON_STATIC
#define JSON_API static
#else
#define JSON_API extern
#endif

/*--------------------------------------------------------------------------
                                INTERNAL
  -------------------------------------------------------------------------*/
typedef double json_number;
enum json_token_type {
    JSON_NONE,      /* invalid token */
    JSON_OBJECT,    /* sub-object */
    JSON_ARRAY,     /* sub-array */
    JSON_NUMBER,    /* floating point number token */
    JSON_STRING,    /* string text token */
    JSON_TRUE,      /* true constant token */
    JSON_FALSE,     /* false constant token*/
    JSON_NULL,      /* null constant token */
    JSON_MAX
};
struct json_token {
    const char *str;
    enum json_token_type type;
    int len;
    int children; /* number of direct child tokens */
    int sub; /* total number of subtokens (note: not pairs)*/
};
struct json_pair {
    struct json_token name;
    struct json_token value;
};
struct json_iter {
    int len;
    unsigned short err;
    unsigned short depth;
    const char *go;
    const char *src;
};
/* tokenizer */
JSON_API struct json_iter   json_begin_internal(const char *json, int length);
JSON_API struct json_iter   json_read_internal(struct json_token*, const struct json_iter*);
JSON_API struct json_iter   json_parse_internal(struct json_pair*, const struct json_iter*);

/* utility */
JSON_API int                json_cmp_internal(const struct json_token*, const char*);
JSON_API int                json_cpy_internal(char*, int, const struct json_token*);
JSON_API int                json_convert_internal(json_number *, const struct json_token*);

/*-------------------------------------------------------------------------
                                API
  -------------------------------------------------------------------------*/
#ifndef JSON_DELIMITER
#define JSON_DELIMITER '.'
#endif

#ifndef JSON_INITIAL_CAPACITY
#define JSON_INITIAL_CAPACITY 128
#endif

#ifndef JSON_MAX_DEPTH
#define JSON_MAX_DEPTH 256
#endif

enum json_status {
    JSON_OK = 0,
    JSON_INVAL,
    JSON_OUT_OF_TOKEN,
    JSON_STACK_OVERFLOW,
    JSON_PARSING_ERROR
};
struct json_parser {
    enum json_status err;
    struct json_token *toks;
    int cnt, cap;

    /* internal */
    struct json_iter *iter;
    struct json_iter stk[JSON_MAX_DEPTH];
    struct json_token tok;
};

JSON_API enum json_token_type json_type(const struct json_token *tok);

/* parse JSON into token array tree */
JSON_API int                json_num(const char *json, int length);
JSON_API int                json_load(struct json_parser *p, const char *str, int len);

/* iterate over array/object content */
JSON_API struct json_token* json_array_begin(struct json_token *tok);
JSON_API struct json_token* json_array_next(struct json_token *tok);

JSON_API struct json_token* json_obj_begin(struct json_token *tok);
JSON_API struct json_token* json_obj_next(struct json_token *toks);

/* access nodes inside token array */
JSON_API struct json_token *json_query(struct json_token *toks, int count, const char *path);
JSON_API int                json_query_number(json_number*, struct json_token *toks, int count, const char *path);
JSON_API int                json_query_string(char*, int max, int *size, struct json_token*, int count, const char *path);
JSON_API int                json_query_type(struct json_token *toks, int count, const char *path);

#ifdef __cplusplus
}
#endif
#endif /* JSON_H_ */

