
rootdir = ".."

dopackage(rootdir.."/deps/premake_common.lua")

local DEPS_PRJ_DIR = rootdir.."/build/"

project.name = "MyEngine_Depends"
project.path = DEPS_PRJ_DIR

package = newpackage()
InitPackage(FTYPE_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
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
		matchfiles(DEPS_DIR.."/freetypedll/src/*.cpp"),
		matchfiles(DEPS_DIR.."/freetypedll/src/*.def"),
	}, {},
	{FTYPE_INC_DIR}, BASE_LIB_PATH)

package = newpackage()
InitPackage(PCRE_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{"PCRE_STATIC"}, {}, {},
	{
		matchfiles(PCRE_SRC_DIR.."/*.c"),
		matchfiles(PCRE_SRC_DIR.."/*.h"),
	}, {PCRE_SRC_DIR.."/ucptable.c", PCRE_SRC_DIR.."/ucptypetable.c",},
	{PCRE_INC_DIR}, BASE_LIB_PATH)

package = newpackage()
InitPackage(LUA_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{"LUA_OPNAMES"}, {}, {},
	{	matchfiles(LUA_SRC_DIR.."/*.c"),
		matchfiles(LUA_SRC_DIR.."/*.h"),
		matchfiles(LUA_INC_DIR.."/*.h"),
	}, {},
	{LUA_INC_DIR}, {})

package = newpackage()
InitPackage(LUA51_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{"LUA_OPNAMES"}, {}, {},
	{	matchfiles(LUA51_SRC_DIR.."/*.c"),
		matchfiles(LUA51_SRC_DIR.."/*.h"),
		matchfiles(LUA51_INC_DIR.."/*.h"),
	}, {},
	{LUA51_INC_DIR}, {})

package = newpackage()
InitPackage(LUA_INTERPRETER_DEP, DEPS_PRJ_DIR, "c++", "exe", "",
	{LUA_DEP}, {}, {},
	{}, {}, {},
	{
		matchfiles(LUA_SRC_DIR.."/lua/*.c"),
		matchfiles(LUA_SRC_DIR.."/lua/*.h"),
	}, {},
	{LUA_INC_DIR}, BASE_LIB_PATH)

package = newpackage()
InitPackage(LUA_COMPILER_DEP, DEPS_PRJ_DIR, "c++", "exe", "",
	{LUA_DEP}, {}, {},
	{}, {}, {},
	{
		matchfiles(LUA_SRC_DIR.."/luac/*.c"),
		matchfiles(LUA_SRC_DIR.."/luac/*.h"),
	}, {},
	{LUA_INC_DIR, LUA_SRC_DIR}, BASE_LIB_PATH)

local LUA_API=""
if windows then
	LUA_API="LUA_API=__declspec(dllexport)"
end

package = newpackage()
InitPackage(LUA_DLL_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
	{}, {}, {},
	{LUA_API, "LUA_OPNAMES"}, {}, {},
	{	matchfiles(LUA_DLL_SRC_DIR.."/*.cpp"),
		matchfiles(LUA_DLL_INC_DIR.."/*.h"),
		matchfiles(LUA_SRC_DIR.."/*.c"),
		matchfiles(LUA_SRC_DIR.."/*.h"),
		matchfiles(LUA_INC_DIR.."/*.h"),
       }, {},
       {LUA_INC_DIR}, {})

package = newpackage()
InitPackage(TOLUA_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{"TOLUA_STATIC"}, {"TOLUA_RELEASE"}, {},
	{
		matchfiles(TOLUA_INC_DIR.."/*.h"),
		matchfiles(TOLUA_SRC_DIR.."/lib/*.c"),
		matchfiles(TOLUA_SRC_DIR.."/lib/*.h"),
	}, {},
	{LUA_INC_DIR, TOLUA_INC_DIR}, {})

package = newpackage()
InitPackage(TOLUA_DLL_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
       {LUA_DLL_DEP}, {}, {},
       {"TOLUA_EXPORTS"}, {"TOLUA_RELEASE"}, {},
       {
		matchfiles(TOLUA_INC_DIR.."/*.h"),
		matchfiles(TOLUA_DLL_INC_DIR.."/*.h"),
		matchfiles(TOLUA_DLL_SRC_DIR.."/*.cpp"),
		matchfiles(TOLUA_SRC_DIR.."/lib/*.c"),
		matchfiles(TOLUA_SRC_DIR.."/lib/*.h"),
       }, {},
       {LUA_INC_DIR, TOLUA_INC_DIR}, {})

package = newpackage()
InitPackage(TOLUA_APP_DEP, DEPS_PRJ_DIR, "c++", "exe", "",
	{LUA51_DEP}, {}, {},
	{"TOLUA_STATIC"}, {"TOLUA_RELEASE"}, {},
	{
		matchfiles(TOLUA_INC_DIR.."/*.h"),
		matchfiles(TOLUA_SRC_DIR.."/bin/*.c"),
		matchfiles(TOLUA_SRC_DIR.."/bin/*.h"),
		matchfiles(TOLUA_SRC_DIR.."/lib/*.c"),
		matchfiles(TOLUA_SRC_DIR.."/lib/*.h"),
	}, {},
	{LUA51_INC_DIR, TOLUA_INC_DIR}, BASE_LIB_PATH)

local jpegexclude = {}
if windows then
	jpegexclude = {
		JPEG_SRC_DIR.."/jmemansi.c",
		JPEG_SRC_DIR.."/jmemdos.c",
		JPEG_SRC_DIR.."/jmemmac.c",
		JPEG_SRC_DIR.."/jmemname.c"}
end

package = newpackage()
InitPackage(JPEG_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		matchfiles(JPEG_SRC_DIR.."/*.c"),
		matchfiles(JPEG_SRC_DIR.."/*.h"),
	}, 
	{	jpegexclude,
		JPEG_SRC_DIR.."/rdjpgcom.c",
		JPEG_SRC_DIR.."/jpegtran.c",
		JPEG_SRC_DIR.."/djpeg.c",
		JPEG_SRC_DIR.."/ckconfig.c",
		JPEG_SRC_DIR.."/cjpeg.c",
		JPEG_SRC_DIR.."/ansi2knr.c",
	},
	{JPEG_INC_DIR}, {})

package = newpackage()
InitPackage(ZLIB_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		matchfiles(ZLIB_SRC_DIR.."/*.c"),
		matchfiles(ZLIB_SRC_DIR.."/*.h"),
	}, {},
	{ZLIB_INC_DIR}, {})

package = newpackage()
InitPackage(PNG_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		matchfiles(PNG_INC_DIR.."/*.c"),
		matchfiles(PNG_INC_DIR.."/*.h"),
	}, 
	{
		PNG_INC_DIR.."/png2pnm.c",
		PNG_INC_DIR.."/pnm2png.c",
		PNG_INC_DIR.."/rpng-win.c",
		PNG_INC_DIR.."/rpng-x.c",
		PNG_INC_DIR.."/rpng2-win.c",
		PNG_INC_DIR.."/rpng2-x.c",
		PNG_INC_DIR.."/VisualPng.c",},
	{PNG_INC_DIR, ZLIB_INC_DIR, DEPS_DIR}, {})

package = newpackage()
InitPackage(MINI_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{"NOOGL"}, {}, {},
	{
		matchfiles(MINI_SRC_DIR.."/*.cpp"),
		matchfiles(MINI_SRC_DIR.."/*.hpp"),
		matchfiles(MINI_SRC_DIR.."/*.h"),
	}, {MINI_SRC_DIR.."/example.cpp", MINI_SRC_DIR.."/viewer.cpp"},
	{MINI_INC_DIR}, {})

package = newpackage()
InitPackage(OGG_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		matchrecursive(OGG_INC_DIR.."/*.h"),
		matchfiles(OGG_SRC_DIR.."/*.c"),
		matchfiles(OGG_SRC_DIR.."/*.h"),
	}, {},
	{OGG_INC_DIR}, {})

package = newpackage()
InitPackage(VORB_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{}, {}, {},
	{
		matchrecursive(VORB_INC_DIR.."/*.h"),
		matchfiles(VORB_SRC_DIR.."/*.c"),
		matchfiles(VORB_SRC_DIR.."/*.h"),
	},
	{
		VORB_SRC_DIR.."/psytune.c",
		VORB_SRC_DIR.."/barkmel.c",
		VORB_SRC_DIR.."/tone.c"
	},
	{OGG_INC_DIR, VORB_INC_DIR}, {})

package = newpackage()
InitPackage(OGGENC_DEP, DEPS_PRJ_DIR, "c++", "exe", "",
	{OGG_DEP, VORB_DEP}, {}, {},
	{}, {}, {},
	{
		VORB_TOOLS_DIR.."/share/getopt.c",
		VORB_TOOLS_DIR.."/share/getopt1.c",
		VORB_TOOLS_DIR.."/share/utf8.c",
		matchfiles(VORB_TOOLS_DIR.."/oggenc/*.c"),
		matchfiles(VORB_TOOLS_DIR.."/oggenc/*.h"),
		matchfiles(VORB_TOOLS_DIR.."/include/*.h"),
	},
	{
		VORB_TOOLS_DIR.."/oggenc/flac.c",
		VORB_TOOLS_DIR.."/oggenc/easyflac.c",
	},
	{VORB_INC_DIR, OGG_INC_DIR, VORB_TOOLS_DIR.."/include"}, {})

package = newpackage()
InitPackage(OGGDEC_DEP, DEPS_PRJ_DIR, "c++", "exe", "",
	{OGG_DEP, VORB_DEP}, {}, {},
	{}, {}, {},
	{
		VORB_TOOLS_DIR.."/share/getopt.c",
		VORB_TOOLS_DIR.."/share/getopt1.c",
		matchfiles(VORB_TOOLS_DIR.."/oggdec/*.c"),
		matchfiles(VORB_TOOLS_DIR.."/oggdec/*.h"),
		matchfiles(VORB_TOOLS_DIR.."/include/*.h"),
	}, {},
	{VORB_INC_DIR, OGG_INC_DIR, VORB_TOOLS_DIR.."/include"}, {})

package = newpackage()
InitPackage(CAL3D_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
	{}, {}, {},
	{"CAL3D_EXPORTS"}, {}, {},
	{
		matchfiles(CAL3D_SRC_DIR.."/cal3d/*.cpp"),
		matchfiles(CAL3D_SRC_DIR.."/cal3d/*.h"),
	}, {},
	{CAL3D_INC_DIR}, {})

package = newpackage()
InitPackage(CAL3D_MAX7EXP_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
	{CAL3D_DEP}, {}, {"core", "geom", "gfx", "mesh", "maxutil", "maxscrpt", "paramblk2"},
	{"_AFXDLL"}, {}, {},
	{
		matchrecursive(CAL3D_PLUGINS_DIR.."/src/*.cpp"),
		matchrecursive(CAL3D_PLUGINS_DIR.."/src/*.h"),
		matchrecursive(CAL3D_PLUGINS_DIR.."/src/*.bmp"),
		matchrecursive(CAL3D_PLUGINS_DIR.."/src/*.ico"),
		matchfiles(CAL3D_MAXEXP_SRC_DIR.."/*.cpp"),
		matchfiles(CAL3D_MAXEXP_SRC_DIR.."/*.h"),
		matchfiles(CAL3D_MAXEXP_SRC_DIR.."/*.def"),
		matchfiles(CAL3D_MAXEXP_SRC_DIR.."/*.rc"),
	},
	{	matchfiles(CAL3D_PLUGINS_DIR.."/src/win32/SelectionDialog.*") },
	{	CAL3D_INC_DIR,
		CAL3D_MAXEXP_INC_DIR,
		CAL3D_MAXEXP_SRC_DIR.."/../src",
		CAL3D_MAXEXP_SRC_DIR.."/../src/win32",
		MAX7SDK_INC_DIR,
		MAX7SDK_INC_DIR.."/CS"},
	{	MAX7SDK_LIBRARIES_DIR} )

package = newpackage()
InitPackage(ODE_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{"ODE_LIB"}, {}, {},
	{
		matchfiles(ODE_SRC_DIR.."/*.cpp"),
		matchfiles(ODE_SRC_DIR.."/*.c"),
		matchfiles(ODE_SRC_DIR.."/*.h"),
		matchfiles(ODE_INC_DIR.."/ode/*.h"),
		matchrecursive(ODE_OPCODE_DIR.."/*.h"),
		matchrecursive(ODE_OPCODE_DIR.."/*.cpp"),
		matchrecursive(ODE_GIMPACT_DIR.."/*.h"),
		matchrecursive(ODE_GIMPACT_DIR.."/*.cpp"),
	}, 
	{
		ODE_SRC_DIR.."/collision_std.cpp",
		ODE_SRC_DIR.."/scrapbook.cpp",
		ODE_SRC_DIR.."/stack.cpp",
	},
	{ODE_INC_DIR, ODE_DIR, ODE_GIMPACT_DIR.."/include", ODE_OPCODE_DIR}, {})

package = newpackage()
InitPackage(CEGUIBASE071_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
	{PCRE_DEP, FTYPE_DEP}, {}, {"winmm"},
	{"HAVE_CONFIG_H", "PCRE_STATIC", "CEGUIBASE_EXPORTS"}, {}, {},
	{
		matchfiles(CEGUI071_SRC_DIR.."/*.cpp"),
		matchfiles(CEGUI071_INC_DIR.."/*.h"),
		matchfiles(CEGUI071_SRC_DIR.."/elements/*.cpp"),
		matchfiles(CEGUI071_INC_DIR.."/elements/*.h"),
		matchfiles(CEGUI071_SRC_DIR.."/falagard/*.cpp"),
		matchfiles(CEGUI071_INC_DIR.."/falagard/*.h"),
	},
		{CEGUI071_SRC_DIR.."/minibidi.cpp",},
	{CEGUI071_INC_DIR, FTYPE_INC_DIR, PCRE_INC_DIR},
		BASE_LIB_PATH)

package = newpackage()
InitPackage(CEGUILUA071_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
	{CEGUIBASE071_DEP, TOLUA_DLL_DEP, LUA_DLL_DEP}, {}, {},
	{"HAVE_CONFIG_H", "CEGUILUA_EXPORTS", "TOLUA_RELEASE"}, {}, {},
	{	matchfiles(CEGUI071_LUA_SRC_DIR.."/*.cpp"),
		matchfiles(CEGUI071_LUA_SRC_DIR.."/*.h"),
		matchfiles(CEGUI071_LUA_INC_DIR.."/*.h"),
	}, {},
	{CEGUI071_INC_DIR, CEGUI071_LUA_INC_DIR, LUA_INC_DIR, TOLUA_INC_DIR},
		BASE_LIB_PATH)

package = newpackage()
InitPackage(CEGUIXML071_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
	{CEGUIBASE071_DEP}, {}, {},
	{"HAVE_CONFIG_H", "CEGUITINYXMLPARSER_EXPORTS"}, {}, {},
	{
		matchfiles(CEGUI071_XML_INC_DIR.."/*.h"),
		matchfiles(CEGUI071_XML_SRC_DIR.."/*.cpp"),
		matchfiles(CEGUI071_XML_INC_DIR.."/ceguitinyxml/*.h"),
		matchfiles(CEGUI071_XML_SRC_DIR.."/ceguitinyxml/*.cpp"),
	}, {},
	{CEGUI071_INC_DIR, CEGUI071_XML_INC_DIR},
		BASE_LIB_PATH)

package = newpackage()
InitPackage(CEGUIFAL071_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
	{CEGUIBASE071_DEP}, {}, {},
	{"HAVE_CONFIG_H", "FALAGARDWRBASE_EXPORTS", "CEGUIWRMODULE_EXPORTS"}, {}, {},
	{
		matchfiles(CEGUI071_WR_SRC_DIR.."/*.cpp"),
		matchfiles(CEGUI071_WR_INC_DIR.."/*.h"),
	}, {},
	{CEGUI071_INC_DIR, CEGUI071_WR_INC_DIR},
		BASE_LIB_PATH)

package = newpackage()
InitPackage(CEGUIIMG071_DEP, DEPS_PRJ_DIR, "c++", "dll", "",
	{CEGUIBASE071_DEP}, {}, {},
	{"HAVE_CONFIG_H", "CEGUITGAIMAGECODEC_EXPORTS"}, {}, {},
	{
		matchfiles(CEGUI071_CODEC_SRC_DIR.."/*.cpp"),
		matchfiles(CEGUI071_CODEC_INC_DIR.."/*.h"),
	}, {},
	{CEGUI071_INC_DIR, CEGUI071_CODEC_INC_DIR},
		BASE_LIB_PATH)

package = newpackage()
InitPackage(DEVIL_DEP, DEPS_PRJ_DIR, "c++", "lib", "",
	{}, {}, {},
	{"IL_STATIC_LIB"}, {}, {},
	{
		matchrecursive(DEVIL_INC_DIR.."/*.h"),
		matchfiles(DEVIL_IL_INC_DIR.."/*.h"),
		matchfiles(DEVIL_IL_SRC_DIR.."/*.c"),
		matchfiles(DEVIL_ILU_INC_DIR.."/*.h"),
		matchfiles(DEVIL_ILU_SRC_DIR.."/*.c"),
	}, {},
	{DEVIL_INC_DIR, DEVIL_IL_INC_DIR, DEVIL_ILU_INC_DIR, PNG_INC_DIR, ZLIB_INC_DIR, JPEG_INC_DIR},
		{BASE_LIB_PATH})
