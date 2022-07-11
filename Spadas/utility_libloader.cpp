
#include "spadas.h"

#if defined(SPADAS_ENV_WINDOWS)
#include <windows.h>
#undef max
#undef min
#else
#include <dlfcn.h>
#include <unistd.h>
#endif

namespace spadas
{
	class LibraryLoaderVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(LibraryLoader, Vars)

#if defined(SPADAS_ENV_WINDOWS)
		HMODULE module;
		LibraryLoaderVars() : module(NULL)
		{}
#else
		Pointer handle;
		LibraryLoaderVars() : handle(NULL)
		{}
#endif
	};
}

using namespace spadas;

const String spadas::LibraryLoader::TypeName = "spadas.LibraryLoader";

LibraryLoader::LibraryLoader() : Object<LibraryLoaderVars>(new LibraryLoaderVars, TRUE)
{
}

Bool LibraryLoader::openWithName(Path libDir, String libName, String libVersion)
{
	String dummy;
	return openWithName(libDir, libName, libVersion, dummy);
}

Bool LibraryLoader::openWithName(Path libDir, String libName, String libVersion, String& errorMessage)
{
	SPADAS_ERROR_RETURNVAL(libDir.isNull() || !libDir.isFolder(), FALSE);
	SPADAS_ERROR_RETURNVAL(libName.isEmpty(), FALSE);

	String versionPostfix = String(libVersion.isEmpty() ? "" : ".") + libVersion;

#if defined(SPADAS_ENV_WINDOWS)
	return openWithPath(libDir.childFile(libName + ".dll"), errorMessage);
#endif
#if defined(SPADAS_ENV_LINUX)
	return openWithPath(libDir.childFile(SS"lib" + libName + ".so" + versionPostfix), errorMessage);
#endif
#if defined(SPADAS_ENV_MACOS)
	return openWithPath(libDir.childFile(SS"lib" + libName + versionPostfix + ".dylib"), errorMessage);
#endif
}

Bool LibraryLoader::openWithPath(Path libPath)
{
	String dummy;
	return openWithPath(libPath, dummy);
}

Bool LibraryLoader::openWithPath(Path libPath, String& errorMessage)
{
	SPADAS_ERROR_RETURNVAL(libPath.isNull() || !libPath.isFile(), FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!libPath.exist(), libPath.fullPath(), FALSE);

#if defined(SPADAS_ENV_WINDOWS)
	SPADAS_ERROR_RETURNVAL(vars->module != NULL, FALSE);

	system::addEnvironmentPath(libPath.parentFolder());

	vars->module = LoadLibraryW(libPath.fullPath().wchars().data());
	if (vars->module == NULL)
	{
        DWORD err = GetLastError();
        WChar buffer[4096];
        DWORD len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, buffer, 4096, NULL);
        if (len > 0)
        {
            buffer[len] = 0;
            errorMessage = buffer;
        }
		return FALSE;
	}    
#else
	SPADAS_ERROR_RETURNVAL(vars->handle != NULL, FALSE);

	Path rootDir = libPath.parentFolder();
	system::addEnvironmentPath(rootDir);
	chdir((Char*)rootDir.fullPath().bytes());

	vars->handle = (Pointer)dlopen((Char*)libPath.fullPath().bytes(), RTLD_NOW | RTLD_GLOBAL);
	if (vars->handle == NULL)
	{
		errorMessage = dlerror();
		return FALSE;
	}
#endif

	return TRUE;
}

void LibraryLoader::close()
{
#if defined(SPADAS_ENV_WINDOWS)
	if (vars->module)
	{
		FreeLibrary(vars->module);
		vars->module = NULL;
	}
#else
	if (vars->handle)
	{
		dlclose(vars->handle);
		vars->handle = NULL;
	}
#endif
}

Pointer LibraryLoader::getSymbol(String symbol)
{
#if defined(SPADAS_ENV_WINDOWS)
	SPADAS_ERROR_RETURNVAL(vars->module == NULL, NULL);
	return (Pointer)GetProcAddress(vars->module, symbol.chars().data());
#else
	SPADAS_ERROR_RETURNVAL(vars->handle == NULL, NULL);
	return (Pointer)dlsym(vars->handle, (Char*)symbol.bytes());
#endif
}
