
#if defined(SPADAS_ENV_WINDOWS)
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#undef NULL
#endif

#define SPADAS_ENABLE_MACROS
#include "spadas.h"

namespace spadas
{
	class LibraryLoaderVars : public Vars
	{
	public:
		SPADAS_VARS(LibraryLoader, Vars)

#if defined(SPADAS_ENV_WINDOWS)
		HMODULE module;
		LibraryLoaderVars() : module(0)
		{}
#else
		Pointer handle;
		LibraryLoaderVars() : handle(0)
		{}
#endif
	};
}

using namespace spadas;

LibraryLoader::LibraryLoader() : BaseObject(new LibraryLoaderVars)
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
	Path libPath = libDir.childFile(libName + ".dll");
#endif
#if defined(SPADAS_ENV_LINUX)
	Path libPath = libDir.childFile(catAll("lib", libName, ".so", versionPostfix));
#endif
#if defined(SPADAS_ENV_MACOS)
	Path libPath = libDir.childFile(catAll("lib", libName, versionPostfix, ".dylib"));
#endif

	if (!libPath.exist())
	{
		errorMessage = "Library file doesn't exist.";
		return FALSE;
	}

	return openWithPath(libPath, errorMessage);
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
	SPADAS_ERROR_RETURNVAL(var()->module != 0, FALSE);

	system::addEnvironmentPath(libPath.parentFolder());

	var()->module = LoadLibraryW(libPath.fullPath().wchars().data());
	if (var()->module == 0)
	{
        DWORD err = GetLastError();
        WChar buffer[4096];
        DWORD len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 0, err, 0, buffer, 4096, 0);
        if (len > 0)
        {
            buffer[len] = 0;
            errorMessage = buffer;
        }
		return FALSE;
	}    
#else
	SPADAS_ERROR_RETURNVAL(var()->handle != 0, FALSE);

	Path rootDir = libPath.parentFolder();
	system::addEnvironmentPath(rootDir);
	chdir(rootDir.fullPath().chars().data());

	var()->handle = (Pointer)dlopen(libPath.fullPath().chars().data(), RTLD_NOW);
	if (var()->handle == 0)
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
	if (var()->module)
	{
		FreeLibrary(var()->module);
		var()->module = 0;
	}
#else
	if (var()->handle)
	{
		dlclose(var()->handle);
		var()->handle = 0;
	}
#endif
}

Pointer LibraryLoader::getSymbol(String symbol)
{
#if defined(SPADAS_ENV_WINDOWS)
	SPADAS_ERROR_RETURNVAL(var()->module == 0, 0);
	return (Pointer)GetProcAddress(var()->module, symbol.chars().data());
#else
	SPADAS_ERROR_RETURNVAL(var()->handle == 0, 0);
	return (Pointer)dlsym(var()->handle, symbol.chars().data());
#endif
}
