#ifndef OBERON_H
#define OBERON_H

#include <lua/lua.hpp>

class Oberon
{
  public:
  static void args(lua_State * L, int argc, char * argv[]);
  static void profile(lua_State * L, const char * path);
  static lua_State * init(int argc, char * argv[], const char * path);
};

#endif // OBERON_H
