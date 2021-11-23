
#ifndef SPADAS_SUPPORT_H
#define SPADAS_SUPPORT_H

#if defined(SPADAS_ENV_WINDOWS) && !defined(SPADAS_SUPPORT_EXPORTS)
#define SPADAS_SUPPORT_API __declspec(dllimport)
#else
#define SPADAS_SUPPORT_API __declspec(dllexport)
#endif

extern "C"
{
	SPADAS_SUPPORT_API void* msvc_fopen(const wchar_t* path, const wchar_t* mode);
	SPADAS_SUPPORT_API void msvc_fclose(void* file);
	SPADAS_SUPPORT_API bool msvc_fgets(void* file, char* buffer, unsigned int size);
	SPADAS_SUPPORT_API void msvc_fread(void* file, unsigned char* buffer, unsigned int size);
	SPADAS_SUPPORT_API void msvc_fwrite(void* file, const unsigned char* data, unsigned int size);
	SPADAS_SUPPORT_API void msvc_fflush(void* file);
	SPADAS_SUPPORT_API void msvc_fseek(void* file, long long offset, int mode);
	SPADAS_SUPPORT_API unsigned long long msvc_ftell(void* file);
}

#endif