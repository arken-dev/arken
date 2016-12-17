#include <QMutex>

extern "C" {
#include <json.h>
}

static QMutex s_mutex;

char * json_lock_encode(lua_State *l)
{

  QMutexLocker ml(&s_mutex);

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

  result = (char *) malloc((strlen(json) + 1) * sizeof(char));
  strcpy(result, json);
  result[len] = '\0';

  if (!cfg->encode_keep_buffer)
    strbuf_free(encode_buf);

  return result;
}

void json_lock_decode(lua_State *l, const char * data)
{

  QMutexLocker ml(&s_mutex);

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

static int
json_decode_data(lua_State *L) {
  const char * data = luaL_checkstring(L, 1);
  json_lock_decode(L, data);
  return 1;
}

static int
json_encode_data(lua_State *L) {
  char * data = json_lock_encode(L);
  lua_pushstring(L, data);
  delete[] data;
  return 1;
}

extern "C" {
  int luaopen_charon_json( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"decode", json_decode_data},
      {"encode", json_encode_data},
      {NULL, NULL}
    };
    luaL_register(L, "json", Map);
    return 1;
  }
}
