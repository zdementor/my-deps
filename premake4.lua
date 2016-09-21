
ROOT_DIR = ".."

dofile(ROOT_DIR.."/deps/premake_common.lua")

local DEPS_PRJ_NAME = "MyEngine_Depends"
local DEPS_PRJ_DIR  = ROOT_DIR.."/build"

local DEPS_LIB_DIR = { LIBS_DIR }

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	FTYPE_DEP, "c++", "dll", "",
	{}, {}, {},
	{}, {}, {},
	{
		FTYPE_SRC_DIR.."/autohint/autohint.c",
		FTYPE_SRC_DIR.."/bdf/bdf.c",
		FTYPE_SRC_DIR.."/cff/cff.c",
		FTYPE_SRC_DIR.."/base/ftbase.c",
		FTYPE_SRC_DIR.."/cache/ftcache.c",
		FTYPE_SRC_DIR.."/base/ftglyph.c",
		FTYPE_SRC_DIR.."/gzip/ftgzip.c",
		FTYPE_SRC_DIR.."/base/ftinit.c",
		FTYPE_SRC_DIR.."/lzw/ftlzw.c",
		FTYPE_SRC_DIR.."/base/ftmm.c",
		FTYPE_SRC_DIR.."/base/ftsystem.c",
		FTYPE_SRC_DIR.."/pcf/pcf.c",
		FTYPE_SRC_DIR.."/pfr/pfr.c",
		FTYPE_SRC_DIR.."/psaux/psaux.c",
		FTYPE_SRC_DIR.."/pshinter/pshinter.c",
		FTYPE_SRC_DIR.."/psnames/psmodule.c",
		FTYPE_SRC_DIR.."/raster/raster.c",
		FTYPE_SRC_DIR.."/sfnt/sfnt.c",
		FTYPE_SRC_DIR.."/smooth/smooth.c",
		FTYPE_SRC_DIR.."/truetype/truetype.c",
		FTYPE_SRC_DIR.."/type1/type1.c",
		FTYPE_SRC_DIR.."/cid/type1cid.c",
		FTYPE_SRC_DIR.."/type42/type42.c",
		FTYPE_SRC_DIR.."/winfonts/winfnt.c",
		FTYPE_INC_DIR.."/ft2build.h",
		FTYPE_INC_DIR.."/freetype/config/ftconfig.h",
		FTYPE_INC_DIR.."/freetype/config/ftheader.h",
		FTYPE_INC_DIR.."/freetype/config/ftmodule.h",
		FTYPE_INC_DIR.."/freetype/config/ftoption.h",
		FTYPE_INC_DIR.."/freetype/config/ftstdlib.h",
		DEPS_DIR.."/freetypedll/src/*.cpp",
		DEPS_DIR.."/freetypedll/src/*.def",
	}, {},
	{ FTYPE_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	PCRE_DEP, "c++", "lib", "",
	{}, {}, {},
	{"PCRE_STATIC"}, {}, {},
	{
		PCRE_SRC_DIR.."/*.c",
		PCRE_SRC_DIR.."/*.h",
	},
		{ PCRE_SRC_DIR.."/ucptable.c", PCRE_SRC_DIR.."/ucptypetable.c", },
	{ PCRE_INC_DIR },
		DEPS_LIB_DIR
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	LUA_DEP, "c++", "lib", "",
	{}, {}, {},
	{"LUA_OPNAMES"}, {}, {},
	{
		LUA_SRC_DIR.."/*.c",
		LUA_SRC_DIR.."/*.h",
		LUA_INC_DIR.."/*.h",
		LUA_ADD_SRC_DIR.."/*.c",
		LUA_ADD_INC_DIR.."/*.h",
	},
		{ LUA_SRC_DIR.."/lua.c", LUA_SRC_DIR.."/luac.c", LUA_SRC_DIR.."/print.c" },
	{ LUA_INC_DIR, LUA_ADD_INC_DIR },
		{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	LUA_INTERPRETER_DEP, "c++", "exe", "",
	{LUA_DEP}, {}, {},
	{}, {}, {},
	{
		LUA_SRC_DIR.."/lua.c",
	},
		{},
	{ LUA_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	LUA_COMPILER_DEP, "c++", "exe", "",
	{LUA_DEP}, {}, {},
	{}, {}, {},
	{
		LUA_SRC_DIR.."/luac.c",
		LUA_SRC_DIR.."/print.c",
	},
		{},
	{ LUA_INC_DIR, LUA_SRC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	LUA_DLL_DEP, "c++", "dll", "",
	{}, {}, {},
	{"LUA_BUILD_AS_DLL",}, {}, {},
	{
		LUA_DLL_SRC_DIR.."/*.cpp",
		LUA_DLL_INC_DIR.."/*.h",
		LUA_SRC_DIR.."/*.c",
		LUA_SRC_DIR.."/*.h",
		LUA_INC_DIR.."/*.h",
		LUA_ADD_SRC_DIR.."/*.c",
		LUA_ADD_INC_DIR.."/*.h",
 	},
		{ LUA_SRC_DIR.."/lua.c", LUA_SRC_DIR.."/luac.c", LUA_SRC_DIR.."/print.c" },
	{ LUA_INC_DIR, LUA_ADD_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	TOLUA_DEP, "c++", "lib", "",
	{}, {}, {},
	{"TOLUA_STATIC"}, {"TOLUA_RELEASE"}, {},
	{
		TOLUA_INC_DIR.."/*.h",
		TOLUA_SRC_DIR.."/lib/*.c",
		TOLUA_SRC_DIR.."/lib/*.h",
	},
		{},
	{ LUA_INC_DIR, TOLUA_INC_DIR },
		{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	TOLUA_DLL_DEP, "c++", "dll", "",
	{LUA_DLL_DEP}, {}, {},
	{"TOLUA_EXPORTS"}, {"TOLUA_RELEASE"}, {},
	{
		TOLUA_INC_DIR.."/*.h",
		TOLUA_DLL_INC_DIR.."/*.h",
		TOLUA_DLL_SRC_DIR.."/*.cpp",
		TOLUA_SRC_DIR.."/lib/*.c",
		TOLUA_SRC_DIR.."/lib/*.h",
	},
		{},
	{ LUA_INC_DIR, TOLUA_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	TOLUA_APP_DEP, "c++", "exe", "",
	{LUA_DEP}, {}, {},
	{"TOLUA_STATIC"}, {"TOLUA_RELEASE"}, {},
	{
		TOLUA_INC_DIR.."/*.h",
		TOLUA_SRC_DIR.."/bin/*.c",
		TOLUA_SRC_DIR.."/bin/*.h",
		TOLUA_SRC_DIR.."/lib/*.c",
		TOLUA_SRC_DIR.."/lib/*.h",
	},
		{},
	{ LUA_INC_DIR, TOLUA_INC_DIR },
		{ DEPS_LIB_DIR }
)

local jpegexclude = {}
if os.is("windows") then
	jpegexclude = {
		JPEG_SRC_DIR.."/jmemansi.c",
		JPEG_SRC_DIR.."/jmemdos.c",
		JPEG_SRC_DIR.."/jmemmac.c",
		JPEG_SRC_DIR.."/jmemname.c"}
end

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	JPEG_DEP, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		JPEG_SRC_DIR.."/*.c",
		JPEG_SRC_DIR.."/*.h",
	}, 
		{
			jpegexclude,
			JPEG_SRC_DIR.."/rdjpgcom.c",
			JPEG_SRC_DIR.."/jpegtran.c",
			JPEG_SRC_DIR.."/djpeg.c",
			JPEG_SRC_DIR.."/ckconfig.c",
			JPEG_SRC_DIR.."/cjpeg.c",
			JPEG_SRC_DIR.."/ansi2knr.c",
		},
	{ JPEG_INC_DIR },
		{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	ZLIB_DEP, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		ZLIB_SRC_DIR.."/*.c",
		ZLIB_SRC_DIR.."/*.h",
	},
		{},
	{ ZLIB_INC_DIR },
		{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	PNG_DEP, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		PNG_INC_DIR.."/*.c",
		PNG_INC_DIR.."/*.h",
	}, 
		{
			PNG_INC_DIR.."/png2pnm.c",
			PNG_INC_DIR.."/pnm2png.c",
			PNG_INC_DIR.."/rpng-win.c",
			PNG_INC_DIR.."/rpng-x.c",
			PNG_INC_DIR.."/rpng2-win.c",
			PNG_INC_DIR.."/rpng2-x.c",
			PNG_INC_DIR.."/VisualPng.c",
		},
	{ PNG_INC_DIR, ZLIB_INC_DIR, DEPS_DIR },
		{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	MINI_DEP, "c++", "lib", "",
	{}, {}, {},
	{"NOOGL"}, {}, {},
	{
		MINI_SRC_DIR.."/*.cpp",
		MINI_SRC_DIR.."/*.hpp",
		MINI_SRC_DIR.."/*.h",
	},
		{ MINI_SRC_DIR.."/example.cpp", MINI_SRC_DIR.."/viewer.cpp" },
	{ MINI_INC_DIR },
		{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	OGG_DEP, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		OGG_INC_DIR.."/**.h",
		OGG_SRC_DIR.."/*.c",
		OGG_SRC_DIR.."/*.h",
	},
		 {},
	{ OGG_INC_DIR },
	{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	VORB_DEP, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		VORB_INC_DIR.."/**.h",
		VORB_SRC_DIR.."/*.c",
		VORB_SRC_DIR.."/*.h",
	},
		{
			VORB_SRC_DIR.."/psytune.c",
			VORB_SRC_DIR.."/barkmel.c",
			VORB_SRC_DIR.."/tone.c"
		},
	{ OGG_INC_DIR, VORB_INC_DIR },
		{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	OGGENC_DEP, "c++", "exe", "",
	{OGG_DEP, VORB_DEP}, {}, {},
	{}, {}, {},
	{
		VORB_TOOLS_DIR.."/share/getopt.c",
		VORB_TOOLS_DIR.."/share/getopt1.c",
		VORB_TOOLS_DIR.."/share/utf8.c",
		VORB_TOOLS_DIR.."/oggenc/*.c",
		VORB_TOOLS_DIR.."/oggenc/*.h",
		VORB_TOOLS_DIR.."/include/*.h",
	},
		{
			VORB_TOOLS_DIR.."/oggenc/flac.c",
			VORB_TOOLS_DIR.."/oggenc/easyflac.c",
		},
	{ VORB_INC_DIR, OGG_INC_DIR, VORB_TOOLS_DIR.."/include" },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	OGGDEC_DEP, "c++", "exe", "",
	{OGG_DEP, VORB_DEP}, {}, {},
	{}, {}, {},
	{
		VORB_TOOLS_DIR.."/share/getopt.c",
		VORB_TOOLS_DIR.."/share/getopt1.c",
		VORB_TOOLS_DIR.."/oggdec/*.c",
		VORB_TOOLS_DIR.."/oggdec/*.h",
		VORB_TOOLS_DIR.."/include/*.h",
	},
		{},
	{ VORB_INC_DIR, OGG_INC_DIR, VORB_TOOLS_DIR.."/include" },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	CAL3D_DEP, "c++", "dll", "",
	{}, {}, {},
	{"CAL3D_EXPORTS"}, {}, {},
	{
		CAL3D_SRC_DIR.."/cal3d/*.cpp",
		CAL3D_SRC_DIR.."/cal3d/*.h",
	},
		{},
	{ CAL3D_INC_DIR },
		{}
)

if os.is("windows") then
	InitPackage32(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
		CAL3D_MAX7EXP_DEP, "c++", "dll", "",
		{CAL3D_DEP}, {}, {"core", "geom", "gfx", "mesh", "maxutil", "maxscrpt", "paramblk2"},
		{"_AFXDLL"}, {}, {},
		{
			CAL3D_PLUGINS_DIR.."/src/**.cpp",
			CAL3D_PLUGINS_DIR.."/src/**.h",
			CAL3D_PLUGINS_DIR.."/src/**.bmp",
			CAL3D_PLUGINS_DIR.."/src/**.ico",
			CAL3D_MAXEXP_SRC_DIR.."/*.cpp",
			CAL3D_MAXEXP_SRC_DIR.."/*.h",
			CAL3D_MAXEXP_SRC_DIR.."/*.def",
			CAL3D_MAXEXP_SRC_DIR.."/*.rc",
		},
			{ CAL3D_PLUGINS_DIR.."/src/win32/SelectionDialog.*" },
		{
			CAL3D_INC_DIR,
			CAL3D_MAXEXP_INC_DIR,
			CAL3D_MAXEXP_SRC_DIR.."/../src",
			CAL3D_MAXEXP_SRC_DIR.."/../src/win32",
			MAX7SDK_INC_DIR,
			MAX7SDK_INC_DIR.."/CS"
		},
			{ MAX7SDK_LIBRARIES_DIR, DEPS_LIB_DIR }
	)
end

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	ODE_DEP, "c++", "lib", "",
	{}, {}, {},
	{
		"ODE_LIB", "_OU_NAMESPACE=odeou",
	},
		{},
			{},
	{
		ODE_INC_DIR.."/ode/*.h",
		ODE_SRC_DIR.."/joints/*.h", 
		ODE_SRC_DIR.."/joints/*.cpp", 
		ODE_SRC_DIR.."/*.h", 
		ODE_SRC_DIR.."/*.c", 
		ODE_SRC_DIR.."/*.cpp",
	    ODE_OU_DIR.."/**.h",
		ODE_OU_DIR.."/**.cpp",
		ODE_OPCODE_DIR.."/**.h",
		ODE_OPCODE_DIR.."/**.cpp",
	}, 
		{
			ODE_SRC_DIR.."/collision_std.cpp",
			ODE_SRC_DIR.."/collision_libccd.cpp",
			ODE_SRC_DIR.."/collision_libccd.h"
		},
	{
		ODE_INC_DIR,
		ODE_SRC_DIR,
		ODE_SRC_DIR.."/joints",
		ODE_GIMPACT_DIR.."/include",
		ODE_OPCODE_DIR,
		ODE_OU_DIR.."/include",
	},
		{}
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	CEGUIBASE071_DEP, "c++", "dll", "",
	{PCRE_DEP, FTYPE_DEP}, {}, {"winmm"},
	{"HAVE_CONFIG_H", "PCRE_STATIC", "CEGUIBASE_EXPORTS"}, {}, {},
	{
		CEGUI071_SRC_DIR.."/*.cpp",
		CEGUI071_INC_DIR.."/*.h",
		CEGUI071_SRC_DIR.."/elements/*.cpp",
		CEGUI071_INC_DIR.."/elements/*.h",
		CEGUI071_SRC_DIR.."/falagard/*.cpp",
		CEGUI071_INC_DIR.."/falagard/*.h",
	},
		{CEGUI071_SRC_DIR.."/minibidi.cpp", },
	{ CEGUI071_INC_DIR, FTYPE_INC_DIR, PCRE_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	CEGUILUA071_DEP, "c++", "dll", "",
	{CEGUIBASE071_DEP, TOLUA_DLL_DEP, LUA_DLL_DEP}, {}, {},
	{"HAVE_CONFIG_H", "CEGUILUA_EXPORTS", "TOLUA_RELEASE"}, {}, {},
	{
		CEGUI071_LUA_SRC_DIR.."/*.cpp",
		CEGUI071_LUA_SRC_DIR.."/*.h",
		CEGUI071_LUA_INC_DIR.."/*.h",
	},
		{},
	{ CEGUI071_INC_DIR, CEGUI071_LUA_INC_DIR, LUA_INC_DIR, TOLUA_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	CEGUIXML071_DEP, "c++", "dll", "",
	{CEGUIBASE071_DEP}, {}, {},
	{"HAVE_CONFIG_H", "CEGUITINYXMLPARSER_EXPORTS"}, {}, {},
	{
		CEGUI071_XML_INC_DIR.."/*.h",
		CEGUI071_XML_SRC_DIR.."/*.cpp",
		CEGUI071_XML_INC_DIR.."/ceguitinyxml/*.h",
		CEGUI071_XML_SRC_DIR.."/ceguitinyxml/*.cpp",
	},
		{},
	{ CEGUI071_INC_DIR, CEGUI071_XML_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	CEGUIFAL071_DEP, "c++", "dll", "",
	{CEGUIBASE071_DEP}, {}, {},
	{"HAVE_CONFIG_H", "FALAGARDWRBASE_EXPORTS", "CEGUIWRMODULE_EXPORTS"}, {}, {},
	{
		CEGUI071_WR_SRC_DIR.."/*.cpp",
		CEGUI071_WR_INC_DIR.."/*.h",
	},
		{},
	{ CEGUI071_INC_DIR, CEGUI071_WR_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	CEGUIIMG071_DEP, "c++", "dll", "",
	{CEGUIBASE071_DEP}, {}, {},
	{"HAVE_CONFIG_H", "CEGUITGAIMAGECODEC_EXPORTS"}, {}, {},
	{
		CEGUI071_CODEC_SRC_DIR.."/*.cpp",
		CEGUI071_CODEC_INC_DIR.."/*.h",
	},
		{},
	{ CEGUI071_INC_DIR, CEGUI071_CODEC_INC_DIR },
		{ DEPS_LIB_DIR }
)

InitPackage(DEPS_PRJ_NAME, DEPS_PRJ_DIR,
	DEVIL_DEP, "c++", "lib", "",
	{}, {}, {},
	{"IL_STATIC_LIB"}, {}, {},
	{
		DEVIL_INC_DIR.."/**.h",
		DEVIL_IL_INC_DIR.."/*.h",
		DEVIL_IL_SRC_DIR.."/*.c",
		DEVIL_ILU_INC_DIR.."/*.h",
		DEVIL_ILU_SRC_DIR.."/*.c",
	},
		{},
	{ DEVIL_INC_DIR, DEVIL_IL_INC_DIR, DEVIL_ILU_INC_DIR, PNG_INC_DIR, ZLIB_INC_DIR, JPEG_INC_DIR },
		{ DEPS_LIB_DIR }
)
