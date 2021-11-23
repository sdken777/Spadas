
#include "spadas.h"

#include "file.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>
#undef TRUE
#undef FALSE
#undef min
#undef max

#include "spadas_support.h"

namespace file_internal
{
	using namespace spadas;

	// pathes
	String getWorkPathString()
	{
		WChar buffer[1024];
		GetModuleFileNameW(NULL, buffer, 1024);

		String bufferString = buffer;
		Array<UInt> slashLocations = bufferString.search('\\');
		
		if (slashLocations.isEmpty()) return String();
		else return String(bufferString, Region(0, slashLocations.last() + 1));
	}
	String getHomePathString()
	{
		WChar path[1024];
		GetEnvironmentVariableW(L"USERPROFILE", path, 1024);
		
		String pathString(path);
		if (pathString.isEmpty()) return String();
		
		Char separator = getSeparatorChar();
		if (pathString.bytes()[pathString.length()-1] != (Byte)separator) return pathString + separator;
		else return pathString;
	}
	String getSpadasFilesPathString()
	{
		String homePath = getHomePathString();
		if (homePath.isEmpty()) return String();
		else return homePath + "SpadasFiles\\";
	}

	Char getSeparatorChar()
	{
		return '\\';
	}

	// file I/O operations
	Pointer fileOpen(String fileName, Bool outputMode)
	{
		return msvc_fopen(fileName.wchars().data(), outputMode ? L"wb+" : L"rb");
	}

	void fileClose(Pointer file)
	{
		msvc_fclose(file);
	}

	void filePrint(Pointer file, String text)
	{
		fileOutput(file, text.toBinary());

		const unsigned char enter = '\n';
		msvc_fwrite(file, &enter, 1);
	}

	String fileScan(Pointer file, Char buf[SCAN_SIZE], Bool isUtf8)
	{
		Bool ok = msvc_fgets(file, buf, SCAN_SIZE);

		String out;
		if (ok)
		{
			buf[SCAN_SIZE - 1] = 0;
			UInt len = (UInt)strlen(buf);
			UInt lenDec = 0;

            // remove LF, CR or CRLF
			if (len >= 1)
			{
				if (buf[len - 1] == 10)
				{
					if (len >= 2 && buf[len - 2] == 13)
					{
						buf[len - 2] = 0;
						lenDec = 2;
					}
					else
					{
						buf[len - 1] = 0;
						lenDec = 1;
					}

				}
				else if (buf[len - 1] == 13)
				{
					buf[len - 1] = 0;
					lenDec = 1;
				}
			}

			if (isUtf8)
			{
				out = Binary((Byte*)buf, len - lenDec);
			}
			else
			{
				out = buf;
			}
		}

		return out;
	}

	Binary fileInput(Pointer file, UInt size, ULong fileSize, ULong filePos)
	{
		if (filePos >= fileSize) return Binary();
		Binary out((UInt)math::min((ULong)size, fileSize - filePos));
		msvc_fread(file, out.data(), out.size());
		return out;
	}

	void fileOutput(Pointer file, Binary data)
	{
		msvc_fwrite(file, data.data(), data.size());
	}

	void fileFlush(Pointer file)
	{
		msvc_fflush(file);
	}
	
	ULong fileGetSize(Pointer file)
	{
		msvc_fseek(file, 0, 2);
		return msvc_ftell(file);
	}

	ULong filePos(Pointer file)
	{
		return msvc_ftell(file);
	}
	
	ULong fileSeek(Pointer file, ULong pos)
	{
		msvc_fseek(file, pos, 0);
		return msvc_ftell(file);
	}

	// file operations
	Bool fileExist(String name)
	{
		UInt res = GetFileAttributesW(name.wchars().data());
		return res != UINF && (res & FILE_ATTRIBUTE_DIRECTORY) == 0;
	}

	void fileMove(String srcFile, String dstFile)
	{
		MoveFileW(srcFile.wchars().data(), dstFile.wchars().data());
	}

	void fileCopy(String srcFile, String dstFile)
	{
		CopyFileW(srcFile.wchars().data(), dstFile.wchars().data(), FALSE);
	}

	void fileRemove(String fileName)
	{
		DeleteFileW(fileName.wchars().data());
	}

	Bool folderExist(String name)
	{
		UInt res = GetFileAttributesW(name.wchars().data());
		return res != UINF && (res & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}

	void folderCreate(String folderName)
	{
		CreateDirectoryW(folderName.wchars().data(), 0);
	}

	void folderMove(String srcFolderName, String dstFolderName)
	{
		MoveFileW(srcFolderName.wchars().data(), dstFolderName.wchars().data());
	}

	void folderRemove(String folderName)
	{
		RemoveDirectoryW(folderName.wchars().data());
	}

	Array<String> folderGetContents(String targetFolder)
	{
		String separator = getSeparatorChar();
		String contentsString = targetFolder + "*";

		WIN32_FIND_DATAW fileInfo;
		HANDLE file = NULL;
		BOOL fileExist = TRUE;

		ArrayX<String> out;
		while (TRUE)
		{
			if (!file) file = FindFirstFileW(contentsString.wchars().data(), &fileInfo);
			else fileExist = FindNextFileW(file, &fileInfo);

			if (!file || !fileExist) break;

			String fileName = fileInfo.cFileName;
			if (fileName == "." || fileName == "..") continue;

			if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) out[out.size()] = fileName + separator;
			else out[out.size()] = fileName;
		}
		if (file) FindClose(file);

		return out.toArray(Region(0, out.size()));
	}
}

#endif
