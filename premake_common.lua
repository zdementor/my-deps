
DEPS_DIR = rootdir.."/deps"
SRC_DIR  = rootdir.."/base/src"

-- CEGUI (Crazy Eddie's GUI)

CEGUIBASE071_DEP	= "CEGUIBase-{v.0.7.1}"
CEGUILUA071_DEP	= "CEGUILuaScriptModule-{v.0.7.1}"
CEGUIXML071_DEP	= "CEGUITinyXMLParser-{v.0.7.1}"
CEGUIFAL071_DEP	= "CEGUIFalagardWRBase-{v.0.7.1}"
CEGUIIMG071_DEP	= "CEGUITGAImageCodec-{v.0.7.1}"
CEGUI071_DIR			= DEPS_DIR.."/cegui/cegui-0.7.1/cegui"
CEGUI071_INC_DIR		= CEGUI071_DIR.."/include"
CEGUI071_SRC_DIR		= CEGUI071_DIR.."/src"
CEGUI071_LUA_INC_DIR	= CEGUI071_INC_DIR.."/ScriptingModules/LuaScriptModule"
CEGUI071_LUA_SRC_DIR	= CEGUI071_SRC_DIR.."/ScriptingModules/LuaScriptModule"
CEGUI071_XML_INC_DIR	= CEGUI071_INC_DIR.."/XMLParserModules/TinyXMLParser"
CEGUI071_XML_SRC_DIR	= CEGUI071_SRC_DIR.."/XMLParserModules/TinyXMLParser"
CEGUI071_WR_INC_DIR		= CEGUI071_INC_DIR.."/WindowRendererSets/Falagard"
CEGUI071_WR_SRC_DIR		= CEGUI071_SRC_DIR.."/WindowRendererSets/Falagard"
CEGUI071_CODEC_INC_DIR	= CEGUI071_INC_DIR.."/ImageCodecModules/TGAImageCodec"
CEGUI071_CODEC_SRC_DIR	= CEGUI071_SRC_DIR.."/ImageCodecModules/TGAImageCodec"

CEGUIBASE_DEP		= CEGUIBASE071_DEP
CEGUILUA_DEP		= CEGUILUA071_DEP
CEGUIXML_DEP		= CEGUIXML071_DEP
CEGUIFAL_DEP		= CEGUIFAL071_DEP
CEGUIIMG_DEP		= CEGUIIMG071_DEP
CEGUI_DIR			= CEGUI071_DIR
CEGUI_INC_DIR		= CEGUI071_INC_DIR
CEGUI_LUA_INC_DIR 	= CEGUI071_LUA_INC_DIR

-- Mini (terrain rendering library)
MINI_DEP		= "libmini-{v.8.2}"
MINI_DIR		= DEPS_DIR.."/mini/mini-8.2"
MINI_SRC_DIR	= MINI_DIR
MINI_INC_DIR	= MINI_DIR

-- Freetype (Free True Type fonts library)
FTYPE_DEP		= "FreeType-{v.2.1.9}"
FTYPE_DIR		= DEPS_DIR.."/freetype/freetype-2.1.9"
FTYPE_SRC_DIR	= FTYPE_DIR.."/src"
FTYPE_INC_DIR	= FTYPE_DIR.."/include"

-- ODE (Open Dynamic Engine)
ODE_DEP			= "libode-{v.0.6}"
ODE_DIR			= DEPS_DIR.."/ode/ode-0.6"
ODE_SRC_DIR		= ODE_DIR.."/ode/src"
ODE_INC_DIR		= ODE_DIR.."/include"
ODE_OPCODE_DIR	= ODE_DIR.."/OPCODE"
ODE_GIMPACT_DIR	= ODE_DIR.."/GIMPACT"

-- pcre
PCRE_DEP		= "libpcre-{v.5.0}"
PCRE_DIR		= DEPS_DIR.."/pcre/pcre-5.0"
PCRE_INC_DIR	= PCRE_DIR
PCRE_SRC_DIR	= PCRE_DIR

-- Lua tools
LUA_INTERPRETER_DEP = "lua"
LUA_COMPILER_DEP	= "luac"

-- Lua
LUA_DEP		= "liblua-{v.5.1.4}"
LUA_DIR		= DEPS_DIR.."/lua/lua-5.1.4"
LUA_INC_DIR	= LUA_DIR.."/src"
LUA_SRC_DIR	= LUA_DIR.."/src"

-- Lua dll
LUA_DLL_DEP		= "Lua-{v.5.1.4}"
LUA_DLL_DIR		= DEPS_DIR.."/luadll"
LUA_DLL_INC_DIR	= LUA_DLL_DIR.."/src"
LUA_DLL_SRC_DIR	= LUA_DLL_DIR.."/src"

-- tolua++
TOLUA_DEP		= "libtolua++-{v.1.0.92}"
TOLUA_DIR		= DEPS_DIR.."/tolua++/tolua++-1.0.92"
TOLUA_INC_DIR	= TOLUA_DIR.."/include"
TOLUA_SRC_DIR	= TOLUA_DIR.."/src"

-- tolua++dll
TOLUA_DLL_DEP		= "ToLua++-{v.1.0.92}"
TOLUA_DLL_DIR		= DEPS_DIR.."/tolua++dll"
TOLUA_DLL_INC_DIR	= TOLUA_DLL_DIR.."/include"
TOLUA_DLL_SRC_DIR	= TOLUA_DLL_DIR.."/src"

TOLUA_APP_DEP	= "tolua++"

-- jpeg
JPEG_DEP		= "libjpeg-{v.6b}"
JPEG_DIR		= DEPS_DIR.."/jpeglib/jpeglib-6b"
JPEG_INC_DIR	= JPEG_DIR
JPEG_SRC_DIR	= JPEG_DIR

-- zlib
ZLIB_DEP		= "libzlib-{v.1.1.4}"
ZLIB_DIR		= DEPS_DIR.."/zlib/zlib-1.1.4"
ZLIB_INC_DIR	= ZLIB_DIR
ZLIB_SRC_DIR	= ZLIB_DIR.."/zlib"

-- png
PNG_DEP		= "libpng-{v.1.2.8}"
PNG_DIR		= DEPS_DIR.."/libpng/libpng-1.2.8"
PNG_INC_DIR	= PNG_DIR
PNG_SRC_DIR	= PNG_DIR

-- DevIL
DEVIL_DEP		= "libdevil-{v.1.7.8}"
DEVIL_DIR		= DEPS_DIR.."/devil/devil-1.7.8"
DEVIL_INC_DIR	= DEVIL_DIR.."/include"
DEVIL_IL_DIR		= DEVIL_DIR.."/src-IL"
DEVIL_IL_INC_DIR	= DEVIL_IL_DIR.."/include"
DEVIL_IL_SRC_DIR	= DEVIL_IL_DIR.."/src"
DEVIL_ILU_DIR		= DEVIL_DIR.."/src-ILU"
DEVIL_ILU_INC_DIR	= DEVIL_ILU_DIR.."/include"
DEVIL_ILU_SRC_DIR	= DEVIL_ILU_DIR.."/src"

-- Cal3d (Core Animation Library)
CAL3D_DEP		= "Cal3D-{v.0.11.0}"
CAL3D_DIR		= DEPS_DIR.."/cal3d/cal3d-0.11.0"
CAL3D_INC_DIR	= CAL3D_DIR.."/src"
CAL3D_SRC_DIR	= CAL3D_DIR.."/src"

-- Cal3d plugins
CAL3D_PLUGINS_DIR		= CAL3D_DIR.."/plugins"
CAL3D_MAX7EXP_DEP		= "Cal3D_Max7_Exporter-{v.0.11.0}"
CAL3D_MAXEXP_INC_DIR	= CAL3D_PLUGINS_DIR.."/cal3d_max_exporter"
CAL3D_MAXEXP_SRC_DIR	= CAL3D_PLUGINS_DIR.."/cal3d_max_exporter"

-- ogg-vorbis
OGGVORB_DIR = DEPS_DIR.."/ogg-vorbis"

VORB_DEP		= "libvorbis-{v.1.1.1}"
VORB_DIR		= OGGVORB_DIR.."/libvorbis-1.1.1"
VORB_INC_DIR	= VORB_DIR.."/include"
VORB_SRC_DIR	= VORB_DIR.."/lib"

OGG_DEP		= "libogg-{v.1.1.2}"
OGG_DIR		= OGGVORB_DIR.."/libogg-1.1.2"
OGG_INC_DIR	= OGG_DIR.."/include"
OGG_SRC_DIR	= OGG_DIR.."/src"

OGGDEC_DEP = "oggdec-{v.1.0.1}"
OGGENC_DEP = "oggenc-{v.1.0.1}"
VORB_TOOLS_DIR	= OGGVORB_DIR.."/vorbis-tools-1.0.1"

MAX7SDK_DIR				= DEPS_DIR.."/maxsdk/7"
MAX7SDK_INC_DIR			= MAX7SDK_DIR.."/include"
MAX7SDK_LIBRARIES_DIR	= MAX7SDK_DIR.."/lib"

OAL_INC_DIR	= DEPS_DIR.."/openal/include/al"

function CollectStrings(tbl, res_tbl)
	for key, value in pairs(tbl) do
		if type(value) == "string" then
			table.insert(res_tbl, value)
		end
		if type(value) == "table" then
			CollectStrings(value, res_tbl)
		end
	end
end

local function InitPackage4Arch(_arch, _solname, _solpath,
	_prjname, _lang, _kind, _name_suffix,
	_prjdeps, _deps, _sysdeps,
	_common_defines, _release_defines, _debug_defines,
	_files, _excludes, _inc_paths, _lib_paths,
	_build_opts, _link_opts, _dbg_build_opts, _dbg_link_opts,
	_main, _dbg_main)

	local archstr = "x32"

	if _arch == "x64" then
		archstr = "x64"
	end

	io.write(string.format("Creating %s package %s ->%s ...\n", archstr, _solname, _prjname))

	_solpath = _solpath.."/"..archstr
	_solname = 	_solname.."_"..archstr

	solution(_solname)
	basedir(_solpath)
	configurations {"Release", "Debug"}

	if _arch == "x64" then
		platforms { _arch }
	end

	local kind4 = {
		["exe"] = "WindowedApp",
		["dll"] = "SharedLib",
		["lib"] = "StaticLib",
		}

	local fullname = _prjname
	if _name_suffix then
		fullname = _prjname.._name_suffix
	end

	project(fullname)
		kind(kind4[_kind])
		basedir(_solpath)
		language(_lang)
		targetdir(rootdir.."/bin/"..archstr)

		files(_files)
		excludes(_excludes)

		includedirs(_inc_paths)
		libdirs({_lib_paths, rootdir.."/bin/"..archstr})

		local rel_links = {}
		local dbg_links = {}
		for key, value in pairs(_deps) do
			table.insert(rel_links, value)
			table.insert(dbg_links, value.."_d")
		end
		table.insert(rel_links, _sysdeps)
		table.insert(dbg_links, _sysdeps)
		for key, value in pairs(_prjdeps) do
			table.insert(rel_links, value)
			table.insert(dbg_links, value.."_d")
		end

		local rel_linkoptions = {}
		local dbg_linkoptions = {}
		table.insert(rel_linkoptions, _link_opts or {})
		table.insert(dbg_linkoptions, _dbg_link_opts or (_link_opts or {}))
		-- flags for VTune
		--table.insert(rel_linkoptions, {"/FIXED:NO"})
		--table.insert(dbg_linkoptions, {"/FIXED:NO"})

		local rel_buildoptions = {}
		local dbg_buildoptions = {}
		table.insert(rel_buildoptions, _build_opts or {})
		table.insert(dbg_buildoptions, _dbg_build_opts or (_build_opts or {}))

		local rel_flags = {}
		local dbg_flags = {}
		table.insert(dbg_flags, "NoPCH")
		table.insert(rel_flags, "NoPCH")
		table.insert(rel_flags,
			"OptimizeSpeed"
			--"OptimizeSize"
			)
		table.insert(rel_flags, "No64BitChecks")
		if os.is("windows") then
			if _main == "WinMain"  then
				table.insert(rel_flags, "WinMain")
				table.insert(rel_linkoptions, "/SUBSYSTEM:WINDOWS")
			else
				table.insert(rel_linkoptions, "/SUBSYSTEM:CONSOLE")
			end
			if _dbg_main == "WinMain" then
				table.insert(dbg_flags, "WinMain")
				table.insert(dbg_linkoptions, "/SUBSYSTEM:WINDOWS")
			else
				table.insert(dbg_linkoptions, "/SUBSYSTEM:CONSOLE")
			end
		end
		table.insert(dbg_flags, "Symbols")

		local rel_defines = {}
		local dbg_defines = {}
		table.insert(rel_defines, _common_defines)
		table.insert(dbg_defines, _common_defines)
		table.insert(rel_defines, _release_defines)
		table.insert(dbg_defines, _debug_defines)
		if os.is("windows") then
			table.insert(dbg_defines, {"_WIN32", "WIN32"})
			table.insert(dbg_defines, {"_DEBUG"})
			table.insert(rel_defines, {"_WIN32", "WIN32"})
			table.insert(rel_defines, {"_NDEBUG", "NDEBUG"})
			table.insert(rel_defines, {"_CRT_SECURE_NO_DEPRECATE", "_SCL_SECURE_NO_WARNINGS"})
		elseif os.is("linux") then
			table.insert(rel_defines, {"_UNIX"})
			table.insert(dbg_defines, {"_UNIX"})
		end

		configuration("Release")
			targetname(fullname)
			objdir(rootdir.."/obj/"..fullname.."/Release/"..archstr)
			defines(rel_defines)
			flags(rel_flags)
			buildoptions(rel_buildoptions)
			linkoptions(rel_linkoptions)
			links(rel_links)

		configuration("Debug")
			targetname(fullname.."_d")
			objdir(rootdir.."/obj/"..fullname.."/Debug/"..archstr)
			defines(dbg_defines)
			flags(dbg_flags)
			buildoptions(dbg_buildoptions)
			linkoptions(dbg_linkoptions)
			links(dbg_links)
end

function InitPackage4(_solname, _solpath,
	_prjname, _lang, _kind, _name_suffix,
	_prjdeps, _deps, _sysdeps,
	_common_defines, _release_defines, _debug_defines,
	_files, _excludes, _inc_paths, _lib_paths,
	_build_opts, _link_opts, _dbg_build_opts, _dbg_link_opts,
	_main, _dbg_main)

	local arch = {
		[1] = nil,
		[2] = "x64"
	}

	for i = 1, 2 do
		InitPackage4Arch(
			arch[i],
			_solname, _solpath,
			_prjname, _lang, _kind, _name_suffix,
			_prjdeps, _deps, _sysdeps,
			_common_defines, _release_defines, _debug_defines,
			_files, _excludes, _inc_paths, _lib_paths,
			_build_opts, _link_opts, _dbg_build_opts, _dbg_link_opts,
			_main, _dbg_main)
	end
end

BASE_LIB_PATH =
{ DEPS_DIR.."/lib/static", DEPS_DIR.."/lib/dynamic", rootdir.."/bin"}

BASE_INC_PATH =
{ SRC_DIR.."/inc", DEPS_DIR, SRC_DIR.."/vid/gl", }
