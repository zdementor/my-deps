/**
 * \file   os_is64bit.c
 * \brief  Native code-side checking for a 64-bit architecture.
 * \author Copyright (c) 2011 Jason Perkins and the Premake project
 */

#include "os.h"

int os_is64bit(lua_State* L)
{
	void *ptr;
	int is64 = sizeof(ptr) == 8 ? 1 : 0;

	lua_pushboolean(L, is64);
	return 1;
}
