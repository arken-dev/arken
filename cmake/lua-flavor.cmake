set(LUA_FLAVOR "luajit" CACHE STRING "lua 5.1 flavor luajit or lua standard")

set_property(
  CACHE LUA_FLAVOR PROPERTY STRINGS luajit lua
)

message("LUA_FLAVOR..................: ${LUA_FLAVOR} (luajit|lua)")
