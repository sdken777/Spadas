
#include "spadas_support.h"

#include <stdio.h>
#include <wchar.h>

void* msvc_fopen(const wchar_t* path, const wchar_t* mode)
{
	FILE* file;
	_wfopen_s(&file, path, mode);
	return file;
}

void msvc_fclose(void* file)
{
	fclose((FILE*)file);
}

bool msvc_fgets(void* file, char* buffer, unsigned int size)
{
	return fgets(buffer, size, (FILE*)file) != 0;
}

void msvc_fread(void* file, unsigned char* buffer, unsigned int size)
{
	fread(buffer, 1, size, (FILE*)file);
}

void msvc_fwrite(void* file, const unsigned char* data, unsigned int size)
{
	fwrite(data, 1, size, (FILE*)file);
}

void msvc_fflush(void* file)
{
	fflush((FILE*)file);
}

void msvc_fseek(void* file, long long offset, int mode)
{
	_fseeki64((FILE*)file, offset, mode);
}

unsigned long long msvc_ftell(void* file)
{
	return _ftelli64((FILE*)file);
}