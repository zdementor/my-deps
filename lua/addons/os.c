/**
 * \file   premake.c
 * \brief  Program entry point.
 * \author Copyright (c) 2002-2013 Jason Perkins and the Premake project
 */

#include <stdlib.h>
#include <string.h>
#include "os.h"

#if PLATFORM_MACOSX
#include <CoreFoundation/CFBundle.h>
#endif

/* Built-in functions */
static const luaL_Reg os_functions[] = {
	{ "chdir",       os_chdir       },
	{ "copyfile",    os_copyfile    },
	{ "_is64bit",    os_is64bit     },
	{ "isdir",       os_isdir       },
	{ "getcwd",      os_getcwd      },
// os.getversion()
// Return Value:
// A table, containing the following key-value pairs:
//     majorversion	- The major version number
//     minorversion	- The minor version number
//     revision     - The bug fix release or service pack number
//     description  - A human-readable description of the OS version
	{ "getversion",  os_getversion  },
	{ "isfile",      os_isfile      },
	{ "matchdone",   os_matchdone   },
	{ "matchisfile", os_matchisfile },
	{ "matchname",   os_matchname   },
	{ "matchnext",   os_matchnext   },
	{ "matchstart",  os_matchstart  },
	{ "mkdir",       os_mkdir       },
	{ "pathsearch",  os_pathsearch  },
	{ "rmdir",       os_rmdir       },
	{ "stat",        os_stat        },
	{ "uuid",        os_uuid        },
	{ NULL, NULL }
};

int luainit_os_addon(lua_State* L)
{
	luaL_register(L, "os", os_functions);

	return OKAY;
}
