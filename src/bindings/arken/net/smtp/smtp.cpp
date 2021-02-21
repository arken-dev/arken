// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/net/smtp.h>

using arken::net::SMTP;
using arken::string;

/**
 * checkSMTP
 */

SMTP *
checkSMTP( lua_State *L ) {
  return *(SMTP **) luaL_checkudata(L, 1, "SMTP.metatable");
}

/**
 * ClassMethods
 */

static int
arken_SMTPClassMethodNew( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  SMTP **ptr = (SMTP **)lua_newuserdata(L, sizeof(SMTP*));
  *ptr = new SMTP(url);
  luaL_getmetatable(L, "SMTP.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_SMTPClassMethodRfc2822Date( lua_State *L ) {
  string result = SMTP::rfc2822Date();
  lua_pushstring(L, result);
  return 1;
}

static const luaL_reg SMTPClassMethods[] = {
  {"new",         arken_SMTPClassMethodNew},
  {"rfc2822Date", arken_SMTPClassMethodRfc2822Date},
  {NULL, NULL}
};

void static
registerSMTPClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "SMTP");
  luaL_register(L, NULL, SMTPClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_SMTPInstanceMethodSetVerbose( lua_State *L ) {
  SMTP *udata  = checkSMTP( L );
  bool verbose = lua_toboolean(L, 2);
  udata->setVerbose(verbose);
  return 0;
}

static int
arken_SMTPInstanceMethodSetSsl( lua_State *L ) {
  SMTP *udata  = checkSMTP( L );
  bool ssl = lua_toboolean(L, 2);
  udata->setSsl(ssl);
  return 0;
}

static int
arken_SMTPInstanceMethodSetFrom( lua_State *L ) {
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
arken_SMTPInstanceMethodSetUsername( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * username = luaL_checkstring(L, 2);
  udata->setUsername(username);
  return 0;
}

static int
arken_SMTPInstanceMethodSetPassword( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * password = luaL_checkstring(L, 2);
  udata->setPassword(password);
  return 0;
}

static int
arken_SMTPInstanceMethodSetContentType( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * contentType = luaL_checkstring(L, 2);
  udata->setContentType(contentType);
  return 0;
}


static int
arken_SMTPInstanceMethodSetTo( lua_State *L ) {
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
arken_SMTPInstanceMethodSetReplyTo( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * mail = luaL_checkstring(L, 2);
  udata->setReplyTo(mail);
  return 0;
}

static int
arken_SMTPInstanceMethodSetCopy( lua_State *L ) {
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
arken_SMTPInstanceMethodSetPort( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  int port = luaL_checkinteger(L, 2);
  udata->setPort(port);
  return 0;
}

static int
arken_SMTPInstanceMethodSetSubject( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * subject = luaL_checkstring(L, 2);
  udata->setSubject(subject);
  return 0;
}

static int
arken_SMTPInstanceMethodSetDomain( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * domain = luaL_checkstring(L, 2);
  udata->setDomain(domain);
  return 0;
}

static int
arken_SMTPInstanceMethodSetBody( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  const char * body  = luaL_checkstring(L, 2);
  udata->setBody(body);
  return 0;
}

static int
arken_SMTPInstanceMethodPerform( lua_State *L ) {
  SMTP * udata = checkSMTP( L );
  bool result = udata->perform();
  lua_pushboolean(L, result);
  return 1;
}

static int
arken_SMTPInstanceMethodMessage( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * message = udata->message();
  lua_pushstring(L, message);
  return 1;
}

static int
arken_SMTPInstanceMethodRaw( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  string raw = udata->raw();
  lua_pushlstring(L, raw.data(), raw.size());
  return 1;
}

static int
arken_SMTPInstanceMethodFrom( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * from = udata->from();
  lua_pushstring(L, from);
  return 1;
}

static int
arken_SMTPInstanceMethodTo( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * to = udata->to();
  lua_pushstring(L, to);
  return 1;
}

static int
arken_SMTPInstanceMethodCopy( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * copy = udata->copy();
  lua_pushstring(L, copy);
  return 1;
}

static int
arken_SMTPInstanceMethodReplyTo( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * replayTo = udata->replyTo();
  lua_pushstring(L, replayTo);
  return 1;
}

static int
arken_SMTPInstanceMethodSubject( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * subject = udata->subject();
  lua_pushstring(L, subject);
  return 1;
}

static int
arken_SMTPInstanceMethodDomain( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * domain = udata->domain();
  lua_pushstring(L, domain);
  return 1;
}

static int
arken_SMTPInstanceMethodBody( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * body = udata->body();
  lua_pushstring(L, body);
  return 1;
}

static int
arken_SMTPInstanceMethodUsername( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * username = udata->username();
  lua_pushstring(L, username);
  return 1;
}

static int
arken_SMTPInstanceMethodPassword( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * password = udata->password();
  lua_pushstring(L, password);
  return 1;
}

static int
arken_SMTPInstanceMethodContentType( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  const char * contentType = udata->contentType();
  lua_pushstring(L, contentType);
  return 1;
}

static int
arken_SMTPInstanceMethodPort( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  int port = udata->port();
  lua_pushinteger(L, port);
  return 1;
}

static int
arken_SMTPInstanceMethodSsl( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  bool ssl = udata->ssl();
  lua_pushboolean(L, ssl);
  return 1;
}

static int
arken_SMTPInstanceMethodVerbose( lua_State *L ) {
  SMTP * udata   = checkSMTP( L );
  bool verbose = udata->verbose();
  lua_pushboolean(L, verbose);
  return 1;
}

static int
arken_SMTPInstanceMethodDestruct( lua_State *L ) {
  SMTP *udata = checkSMTP( L );
  delete udata;
  return 0;
}

static const
luaL_reg SMTPInstanceMethods[] = {
  {"setVerbose",     arken_SMTPInstanceMethodSetVerbose},
  {"setSsl",         arken_SMTPInstanceMethodSetSsl},
  {"setBody",        arken_SMTPInstanceMethodSetBody},
  {"setSubject",     arken_SMTPInstanceMethodSetSubject},
  {"setDomain",      arken_SMTPInstanceMethodSetDomain},
  {"setFrom",        arken_SMTPInstanceMethodSetFrom},
  {"setTo",          arken_SMTPInstanceMethodSetTo},
  {"setCopy",        arken_SMTPInstanceMethodSetCopy},
  {"setPort",        arken_SMTPInstanceMethodSetPort},
  {"setUsername",    arken_SMTPInstanceMethodSetUsername},
  {"setPassword",    arken_SMTPInstanceMethodSetPassword},
  {"setContentType", arken_SMTPInstanceMethodSetContentType},
  {"setReplyTo",     arken_SMTPInstanceMethodSetReplyTo},
  {"perform",        arken_SMTPInstanceMethodPerform},
  {"message",        arken_SMTPInstanceMethodMessage},
  {"raw",            arken_SMTPInstanceMethodRaw},
  {"from",           arken_SMTPInstanceMethodFrom},
  {"to",             arken_SMTPInstanceMethodTo},
  {"copy",           arken_SMTPInstanceMethodCopy},
  {"replayTo",       arken_SMTPInstanceMethodReplyTo},
  {"subject",        arken_SMTPInstanceMethodSubject},
  {"domain",         arken_SMTPInstanceMethodDomain},
  {"body",           arken_SMTPInstanceMethodBody},
  {"username",       arken_SMTPInstanceMethodUsername},
  {"password",       arken_SMTPInstanceMethodPassword},
  {"contentType",    arken_SMTPInstanceMethodContentType},
  {"port",           arken_SMTPInstanceMethodPort},
  {"ssl",            arken_SMTPInstanceMethodSsl},
  {"verbose",        arken_SMTPInstanceMethodVerbose},
  {"__gc",           arken_SMTPInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerSMTPInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "SMTP.metatable");
  luaL_register(L, NULL, SMTPInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_SMTP( lua_State *L ) {
    registerSMTPInstanceMethods(L);
    registerSMTPClassMethods(L);
    return 1;
  }
}
