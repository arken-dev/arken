// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <lua/lua.hpp>
#include <arken/net/smtp.h>

using arken::net::SMTP;

/**
 * checkSMTP
 */

SMTP *
checkSMTP( lua_State *L ) {
  return *(SMTP **) luaL_checkudata(L, 1, "arken.net.SMTP.metatable");
}

/**
 * ClassMethods
 */

static int
arken_net_SMTP_new( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  SMTP **ptr = (SMTP **)lua_newuserdata(L, sizeof(SMTP*));
  *ptr = new SMTP(url);
  luaL_getmetatable(L, "arken.net.SMTP.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_net_SMTP_rfc2822Date( lua_State *L ) {
  string result = SMTP::rfc2822Date();
  lua_pushstring(L, result);
  return 1;
}

static const luaL_reg arken_net_SMTP[] = {
  {"new",         arken_net_SMTP_new},
  {"rfc2822Date", arken_net_SMTP_rfc2822Date},
  {NULL, NULL}
};

void static
register_arken_net_SMTP( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.SMTP");
  luaL_register(L, NULL, arken_net_SMTP);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_net_SMTP_setVerbose( lua_State *L ) {
  SMTP *udata  = checkSMTP( L );
  bool verbose = lua_toboolean(L, 2);
  udata->setVerbose(verbose);
  return 0;
}

static int
arken_net_SMTP_setSsl( lua_State *L ) {
  SMTP *udata  = checkSMTP( L );
  bool ssl = lua_toboolean(L, 2);
  udata->setSsl(ssl);
  return 0;
}

static int
arken_net_SMTP_setFrom( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * mail = luaL_checkstring(L, 2);
  if(lua_gettop(L) == 3) { /* número de argumentos */
    const char * from = luaL_checkstring(L, 3);
    udata->setFrom(mail, from);
  } else {
    udata->setFrom(mail);
  }
  return 0;
}

static int
arken_net_SMTP_setUsername( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * username = luaL_checkstring(L, 2);
  udata->setUsername(username);
  return 0;
}

static int
arken_net_SMTP_setPassword( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * password = luaL_checkstring(L, 2);
  udata->setPassword(password);
  return 0;
}

static int
arken_net_SMTP_setContentType( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * contentType = luaL_checkstring(L, 2);
  udata->setContentType(contentType);
  return 0;
}

static int
arken_net_SMTP_setTo( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * mail = luaL_checkstring(L, 2);
  if(lua_gettop(L) == 3) { /* número de argumentos */
    const char * to = luaL_checkstring(L, 3);
    udata->setTo(mail, to);
  } else {
    udata->setTo(mail);
  }
  return 0;
}

static int
arken_net_SMTP_setReplyTo( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * mail = luaL_checkstring(L, 2);
  udata->setReplyTo(mail);
  return 0;
}

static int
arken_net_SMTP_setCopy( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * mail = luaL_checkstring(L, 2);
  if(lua_gettop(L) == 3) { /* número de argumentos */
    const char * copy = luaL_checkstring(L, 3);
    udata->setCopy(mail, copy);
  } else {
    udata->setCopy(mail);
  }
  return 0;
}

static int
arken_net_SMTP_setPort( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  int port = luaL_checkinteger(L, 2);
  udata->setPort(port);
  return 0;
}

static int
arken_net_SMTP_setSubject( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * subject = luaL_checkstring(L, 2);
  udata->setSubject(subject);
  return 0;
}

static int
arken_net_SMTP_setDomain( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * domain = luaL_checkstring(L, 2);
  udata->setDomain(domain);
  return 0;
}

static int
arken_net_SMTP_setBody( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * body  = luaL_checkstring(L, 2);
  udata->setBody(body);
  return 0;
}

static int
arken_net_SMTP_perform( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  bool result = udata->perform();
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_net_SMTP_message( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * message = udata->message();
  lua_pushstring(L, message);
  return 1;
}

static int
arken_net_SMTP_raw( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  string raw = udata->raw();
  lua_pushlstring(L, raw.data(), raw.size());
  return 1;
}

static int
arken_net_SMTP_from( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * from = udata->from();
  lua_pushstring(L, from);
  return 1;
}

static int
arken_net_SMTP_to( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * to = udata->to();
  lua_pushstring(L, to);
  return 1;
}

static int
arken_net_SMTP_copy( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * copy = udata->copy();
  lua_pushstring(L, copy);
  return 1;
}

static int
arken_net_SMTP_replyTo( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * replayTo = udata->replyTo();
  lua_pushstring(L, replayTo);
  return 1;
}

static int
arken_net_SMTP_subject( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * subject = udata->subject();
  lua_pushstring(L, subject);
  return 1;
}

static int
arken_net_SMTP_domain( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * domain = udata->domain();
  lua_pushstring(L, domain);
  return 1;
}

static int
arken_net_SMTP_body( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * body = udata->body();
  lua_pushstring(L, body);
  return 1;
}

static int
arken_net_SMTP_username( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * username = udata->username();
  lua_pushstring(L, username);
  return 1;
}

static int
arken_net_SMTP_password( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * password = udata->password();
  lua_pushstring(L, password);
  return 1;
}

static int
arken_net_SMTP_contentType( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * contentType = udata->contentType();
  lua_pushstring(L, contentType);
  return 1;
}

static int
arken_net_SMTP_port( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  int port = udata->port();
  lua_pushinteger(L, port);
  return 1;
}

static int
arken_net_SMTP_ssl( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  bool ssl = udata->ssl();
  lua_pushboolean(L, ssl);
  return 1;
}

static int
arken_net_SMTP_verbose( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  bool verbose = udata->verbose();
  lua_pushboolean(L, verbose);
  return 1;
}

static int
arken_net_SMTP_gc( lua_State *L ) {
  SMTP *udata = checkSMTP( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_net_SMTP_metatable[] = {
  {"setVerbose",     arken_net_SMTP_setVerbose},
  {"setSsl",         arken_net_SMTP_setSsl},
  {"setBody",        arken_net_SMTP_setBody},
  {"setSubject",     arken_net_SMTP_setSubject},
  {"setDomain",      arken_net_SMTP_setDomain},
  {"setFrom",        arken_net_SMTP_setFrom},
  {"setTo",          arken_net_SMTP_setTo},
  {"setCopy",        arken_net_SMTP_setCopy},
  {"setPort",        arken_net_SMTP_setPort},
  {"setUsername",    arken_net_SMTP_setUsername},
  {"setPassword",    arken_net_SMTP_setPassword},
  {"setContentType", arken_net_SMTP_setContentType},
  {"setReplyTo",     arken_net_SMTP_setReplyTo},
  {"perform",        arken_net_SMTP_perform},
  {"message",        arken_net_SMTP_message},
  {"raw",            arken_net_SMTP_raw},
  {"from",           arken_net_SMTP_from},
  {"to",             arken_net_SMTP_to},
  {"copy",           arken_net_SMTP_copy},
  {"replayTo",       arken_net_SMTP_replyTo},
  {"subject",        arken_net_SMTP_subject},
  {"domain",         arken_net_SMTP_domain},
  {"body",           arken_net_SMTP_body},
  {"username",       arken_net_SMTP_username},
  {"password",       arken_net_SMTP_password},
  {"contentType",    arken_net_SMTP_contentType},
  {"port",           arken_net_SMTP_port},
  {"ssl",            arken_net_SMTP_ssl},
  {"verbose",        arken_net_SMTP_verbose},
  {"__gc",           arken_net_SMTP_gc},
  {NULL, NULL}
};

void static
register_arken_net_SMTP_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.SMTP.metatable");
  luaL_register(L, NULL, arken_net_SMTP_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_SMTP( lua_State *L ) {
    register_arken_net_SMTP_metatable(L);
    register_arken_net_SMTP(L);
    return 1;
  }
}
