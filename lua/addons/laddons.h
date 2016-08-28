#ifndef addons_h
#define addons_h

#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LUA_BITLIBNAME	"bit"
LUA_API int luaopen_bit(lua_State *L);

LUA_API int luainit_os_addon(lua_State* L);

#ifdef __cplusplus
} // extern "C" {
#endif

#endif //#ifndef addons_h