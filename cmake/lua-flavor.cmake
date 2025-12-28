set(LUA_FLAVOR "luajit" CACHE STRING "lua 5.1 flavor luajit or lua standard")

set_property(
  CACHE LUA_FLAVOR PROPERTY STRINGS luajit lua
)

set(LUA_LIB_PATH  CACHE STRING "specify path for liblua(jit) path")

if(LUA_LIB_PATH)
  message("LUA_LIB_PATH................: ${LUA_LIB_PATH}")
else()
  message("LUA_FLAVOR..................: ${LUA_FLAVOR} (luajit|lua)")
endif()
