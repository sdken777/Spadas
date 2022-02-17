
#include "spadas.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>
#undef max
#undef min

namespace spadas
{
	class LibraryLoaderVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(LibraryLoader, Vars)

		HMODULE module;
		
		LibraryLoaderVars() : module(NULL)
		{}
	};
}

using namespace spadas;

const String spadas::LibraryLoader::TypeName = "spadas.LibraryLoader";

LibraryLoader::LibraryLoader() : Object<LibraryLoaderVars>(new LibraryLoaderVars, TRUE)
{
}

Bool LibraryLoader::openWithName(Path libDir, String libName, String libVersion)
{
	SPADAS_ERROR_RETURNVAL(libDir.isNull() || !libDir.isFolder(), FALSE);
	SPADAS_ERROR_RETURNVAL(libName.isEmpty(), FALSE);

	return openWithPath(libDir.childFile(libName + ".dll"));
}

Bool LibraryLoader::openWithPath(Path libPath)
{
	SPADAS_ERROR_RETURNVAL(libPath.isNull() || !libPath.isFile(), FALSE);
	SPADAS_ERROR_RETURNVAL(vars->module != NULL, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!libPath.exist(), libPath.fullPath(), FALSE);

	system::addEnvironmentPath(libPath.parentFolder());

	vars->module = LoadLibraryW(libPath.fullPath().wchars().data());
	SPADAS_ERROR_RETURNVAL_MSG(vars->module == NULL, SS"LoadLibrary failed: " + libPath.fullPath(), FALSE);
    
	return TRUE;
}

void LibraryLoader::close()
{
	if (vars->module)
	{
		FreeLibrary(vars->module);
		vars->module = NULL;
	}
}

Pointer LibraryLoader::getSymbol(String symbol)
{
	SPADAS_ERROR_RETURNVAL(vars->module == NULL, NULL);
	return (Pointer)GetProcAddress(vars->module, symbol.chars().data());
}

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <dlfcn.h>

namespace spadas
{
	class LibraryLoaderVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(LibraryLoader, Vars)

		Pointer handle;
		
		LibraryLoaderVars() : handle(NULL)
		{}
	};
}

using namespace spadas;

const String spadas::LibraryLoader::TypeName = "spadas.LibraryLoader";

LibraryLoader::LibraryLoader() : Object<LibraryLoaderVars>(new LibraryLoaderVars, TRUE)
{
}

Bool LibraryLoader::openWithName(Path libDir, String libName, String libVersion)
{
	SPADAS_ERROR_RETURNVAL(libDir.isNull() || !libDir.isFolder(), FALSE);
	SPADAS_ERROR_RETURNVAL(libName.isEmpty(), FALSE);

	String versionPostfix = String(libVersion.isEmpty() ? "" : ".") + libVersion;

#if defined(SPADAS_ENV_LINUX)
	return openWithPath(libDir.childFile(SS"lib" + libName + ".so" + versionPostfix));
#endif
#if defined(SPADAS_ENV_MACOS)
	return openWithPath(libDir.childFile(SS"lib" + libName + versionPostfix + ".dylib"));
#endif
}

Bool LibraryLoader::openWithPath(Path libPath)
{
	SPADAS_ERROR_RETURNVAL(libPath.isNull() || !libPath.isFile(), FALSE);
	SPADAS_ERROR_RETURNVAL(vars->handle != NULL, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!libPath.exist(), libPath.fullPath(), FALSE);

	system::addEnvironmentPath(libPath.parentFolder());

	vars->handle = (Pointer)dlopen((Char*)libPath.fullPath().bytes(), RTLD_NOW | RTLD_GLOBAL);
	SPADAS_ERROR_RETURNVAL_MSG(vars->handle == NULL, SS"dlopen failed: " + libPath.fullPath() + ". " + dlerror(), FALSE);

	return TRUE;
}

void LibraryLoader::close()
{
	if (vars->handle)
	{
		dlclose(vars->handle);
		vars->handle = NULL;
	}
}

Pointer LibraryLoader::getSymbol(String symbol)
{
	SPADAS_ERROR_RETURNVAL(vars->handle == NULL, NULL);
	return (Pointer)dlsym(vars->handle, (Char*)symbol.bytes());
}

#endif
