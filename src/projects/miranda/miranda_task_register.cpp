#include <luajit-2.0/lua.hpp>
#include <OByteArray>
#include <QDebug>
#include <QThread>
#include <oberon/helper>
#include "mirandastate.h"

extern "C" {
#include <json.h>
char * miranda_json_encode(lua_State *l);
}

static int
miranda_task_value(lua_State *l) {

  const char * uuid = luaL_checkstring(l, 1);
  const char * data = MirandaState::value(uuid);

  json_parse_t json;
  json_token_t token;
  size_t json_len = strlen(data);

  json.cfg = json_fetch_config(l);
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

  return 1;
}

static int
miranda_task_start(lua_State *L) {
  const char * file_name = luaL_checkstring(L, 1);
  const char * uuid = os::uuid();
  /*
  if ( lua_isstring(L, 2) ) {
    MirandaState::insert(uuid, lua_tostring(L, 2));
  }
  */
  MirandaState::insert(uuid, miranda_json_encode(L));
  MirandaState::createTask( file_name, uuid );
  lua_pushstring(L, uuid);
  delete[] uuid;
  return 1;
}

static int
miranda_task_pool(lua_State *L) {
  const char * file_name = luaL_checkstring(L, 1);
  const char * uuid = os::uuid();
  /*
  if ( lua_isstring(L, 2) ) {
    MirandaState::insert(uuid, lua_tostring(L, 2));
  }
  */
  MirandaState::insert(uuid, miranda_json_encode(L));
  MirandaState::taskPool( file_name, uuid );
  lua_pushstring(L, uuid);
  delete[] uuid;
  return 1;
}


static int
miranda_task_insert(lua_State *L) {
  const char * uuid = luaL_checkstring(L, 1);
  MirandaState::insert(uuid, miranda_json_encode(L));
  return 0;
}

void
miranda_task_register(lua_State * L) {
  static const         luaL_reg Map[] = {
    {"pool",    miranda_task_pool},
    {"value",   miranda_task_value},
    {"start",   miranda_task_start},
    {"insert",  miranda_task_insert},
    {NULL, NULL}
  };

  luaL_register(L, "task", Map);
}
