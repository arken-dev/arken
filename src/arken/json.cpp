// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <mutex>
#include <arken/string.h>
#include <arken/os.h>
#include <arken/json.h>

extern "C" {
#include <json.h>
}

static std::mutex mtx;

namespace arken {

char * json::encode(lua_State *l)
{

  std::unique_lock<std::mutex> lck(mtx);

  json_config_t *cfg = json_fetch_config();
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

  size_t size = strlen(json);
  result = new char[size+1];

  strncpy(result, json, size);
  result[len] = '\0';

  if (!cfg->encode_keep_buffer)
    strbuf_free(encode_buf);

  return result;
}

void json::decode(lua_State *l, const char * data)
{

  std::unique_lock<std::mutex> lck(mtx);

  json_parse_t json;
  json_token_t token;
  size_t json_len = strlen(data);

  json.cfg = json_fetch_config();
  json.data = data; //luaL_checklstring(l, 1, &json_len);
  json.current_depth = 0;
  json.ptr = json.data;

  /* Detect Unicode other than UTF-8 (see RFC 4627, Sec 3)
   *
   * CJSON can support any simple data type, hence only the first
   * character is guaranteed to be ASCII (at worst: '"'). This is
   * still enough to detect whether the wrong encoding is in use. */
  if (json_len >= 2 && (!json.data[0] || !json.data[1]))
      luaL_error(l, "JSON parser does not support UTF-16 or UTF-32");

  /* Ensure the temporary buffer can hold the entire string.
   * This means we no longer need to do length checks since the decoded
   * string must be smaller than the entire json string */
  json.tmp = strbuf_new(json_len);

  json_next_token(&json, &token);
  json_process_value(l, &json, &token);

  /* Ensure there is no more input left */
  json_next_token(&json, &token);

  if (token.type != T_END)
      json_throw_parse_error(l, &json, "the end", &token);

  strbuf_free(json.tmp);
}

} // namespace arken
