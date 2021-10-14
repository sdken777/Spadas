
#include "spadas.h"

#include "file.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#undef TRUE
#undef FALSE
#undef min
#undef max

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
		FILE *file;
		_wfopen_s(&file, fileName.wchars().data(), outputMode ? L"wb+" : L"rb");
		return (Pointer)file;
	}

	void fileClose(Pointer file)
	{
		fclose((FILE*)file);
	}

	void filePrint(Pointer file, String text)
	{
		fileOutput(file, text.toBinary());
		fprintf((FILE*)file, "\n");
	}

	String fileScan(Pointer file, Char buffer[SCAN_SIZE], Bool isUtf8)
	{
		const UInt SCAN_READ_SIZE = 64;
		const UInt SCAN_READ_TIME = SCAN_SIZE >> 6;

		ULong originPos = filePos(file);

		Int textBytes = -1;
		for (UInt i = 0; i < SCAN_READ_TIME; i++)
		{
			UInt startIndex = i * SCAN_READ_SIZE;
			UInt readBytes = (UInt)fread(&buffer[startIndex], 1, SCAN_READ_SIZE, (FILE*)file);

			Bool finish = FALSE;
			for (UInt n = 0; n < readBytes; n++)
			{
				Char c = buffer[startIndex + n];
				Bool isEnter = c == 13 || c == 10;
				if (textBytes < 0)
				{
					if (isEnter) textBytes = startIndex + n;
				}
				else if (!isEnter)
				{
					fileSeek(file, originPos + startIndex + n);
					finish = TRUE;
					break;
				}
			}
			if (finish) break;

			if (readBytes < SCAN_READ_SIZE)
			{
				if (textBytes < 0) textBytes = startIndex + readBytes;
				break;
			}
		}
		if (textBytes < 0) textBytes = SCAN_READ_TIME * SCAN_READ_SIZE;

		if (isUtf8)
		{
			return Binary((Byte*)buffer, textBytes);
		}
		else
		{
			buffer[math::min((UInt)textBytes, SCAN_SIZE - 1)] = 0;
			return String(buffer);
		}
	}

	Binary fileInput(Pointer file, UInt size, ULong fileSize, ULong filePos)
	{
		if (filePos >= fileSize) return Binary();
		Binary out((UInt)math::min((ULong)size, fileSize - filePos));
		fread(out.data(), 1, out.size(), (FILE*)file);
		return out;
	}

	void fileOutput(Pointer file, Binary data)
	{
		fwrite(data.data(), 1, data.size(), (FILE*)file);
	}

	void fileFlush(Pointer file)
	{
		fflush((FILE*)file);
	}
	
	ULong fileGetSize(Pointer file)
	{
		_fseeki64((FILE*)file, 0, SEEK_END);
		return (ULong)_ftelli64((FILE*)file);
	}

	ULong filePos(Pointer file)
	{
		return (ULong)_ftelli64((FILE*)file);
	}
	
	ULong fileSeek(Pointer file, ULong pos)
	{
		_fseeki64((FILE*)file, pos, SEEK_SET);
		return (ULong)_ftelli64((FILE*)file);
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
