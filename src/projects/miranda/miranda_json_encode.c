// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <json.h>

#define DEFAULT_SPARSE_CONVERT 0
#define DEFAULT_SPARSE_RATIO 2
#define DEFAULT_SPARSE_SAFE 10
#define DEFAULT_ENCODE_MAX_DEPTH 1000
#define DEFAULT_DECODE_MAX_DEPTH 1000
#define DEFAULT_ENCODE_INVALID_NUMBERS 0
#define DEFAULT_DECODE_INVALID_NUMBERS 1
#define DEFAULT_ENCODE_KEEP_BUFFER 1
#define DEFAULT_ENCODE_NUMBER_PRECISION 14

#ifdef DISABLE_INVALID_NUMBERS
#undef DEFAULT_DECODE_INVALID_NUMBERS
#define DEFAULT_DECODE_INVALID_NUMBERS 0
#endif

/* TODO HEADER */

json_config_t * json_fetch_config(lua_State * l);

char * miranda_json_encode(lua_State *l)
{
    json_config_t *cfg = json_fetch_config(l);
    strbuf_t local_encode_buf;
    strbuf_t *encode_buf;
    char *json;
    char *result;
    int len;

    if (!cfg->encode_keep_buffer) {
        encode_buf = &local_encode_buf;
        strbuf_init(encode_buf, 0);
    } else {
        encode_buf = &cfg->encode_buf;
        strbuf_reset(encode_buf);
    }

    json_append_data(l, cfg, 1, encode_buf);

    json = strbuf_string(encode_buf, &len);

    result = malloc((strlen(json) + 1) * sizeof(char));
    strcpy(result, json);
    result[len] = '\0';

    if (!cfg->encode_keep_buffer)
        strbuf_free(encode_buf);

    return result;
}
