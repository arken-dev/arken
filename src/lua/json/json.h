#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <lua.h>
#include <lauxlib.h>
#include <stdio.h>

#include "strbuf.h"
#include "fpconv.h"

#ifndef _JSON_HEADER_
#define _JSON_HEADER_

typedef enum {
    T_OBJ_BEGIN,
    T_OBJ_END,
    T_ARR_BEGIN,
    T_ARR_END,
    T_STRING,
    T_NUMBER,
    T_BOOLEAN,
    T_NULL,
    T_COLON,
    T_COMMA,
    T_END,
    T_WHITESPACE,
    T_ERROR,
    T_UNKNOWN
} json_token_type_t;

typedef struct {
    json_token_type_t ch2token[256];
    char escape2char[256];  /* Decoding */

    /* encode_buf is only allocated and used when
     * encode_keep_buffer is set */
    strbuf_t encode_buf;

    int encode_sparse_convert;
    int encode_sparse_ratio;
    int encode_sparse_safe;
    int encode_max_depth;
    int encode_invalid_numbers;     /* 2 => Encode as "null" */
    int encode_number_precision;
    int encode_keep_buffer;

    int decode_invalid_numbers;
    int decode_max_depth;
} json_config_t;

typedef struct {
    const char *data;
    const char *ptr;
    strbuf_t *tmp;    /* Temporary storage for strings */
    json_config_t *cfg;
    int current_depth;
} json_parse_t;

typedef struct {
    json_token_type_t type;
    int index;
    union {
        const char *string;
        double number;
        int boolean;
    } value;
    int string_len;
} json_token_t;

json_config_t *json_fetch_config(lua_State *l);
void json_next_token(json_parse_t *json, json_token_t *token);
void json_append_data(lua_State *l, json_config_t *cfg,
                             int current_depth, strbuf_t *json);
void json_process_value(lua_State *l, json_parse_t *json,
                               json_token_t *token);
void json_throw_parse_error(lua_State *l, json_parse_t *json,
                                   const char *exp, json_token_t *token);

#endif
