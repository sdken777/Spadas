
#if defined(SPADAS_ENV_WINDOWS)

#include "file.h"
#include "string_spadas.h"
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
	using namespace string_internal;

	// paths
	String getAppParentPathString(Bool asExecutable)
	{
		WChar buffer[1024];
		GetModuleFileNameW(NULL, buffer, 1024);

		String bufferString = buffer;
		Array<UInt> slashLocations = bufferString.search('\\');
		
		if (slashLocations.isEmpty()) return String();
		else return bufferString.span(0, slashLocations.last() + 1).clone();
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

	String getCurrentPathString()
	{
		WChar pathChars[1024];
		UInt validChars = GetCurrentDirectoryW(1024, pathChars);
		pathChars[math::min(1023u, validChars)] = 0;
		
		String output(pathChars);
		if (output.endsWith("\\")) return output;
		else return output + "\\";
	}

	void setCurrentPathString(String pathString)
	{
		SetCurrentDirectoryW(pathString.wchars().data());
	}

	Char getSeparatorChar()
	{
		return '\\';
	}

	Bool isPathStringValid(String pathString, UInt& rootLength, Bool& isAbsolute)
	{
		UInt len = pathString.length();
		const Byte* bytes = pathString.bytes();

		if (len >= 1 && (bytes[0] == (Byte)'/' || bytes[0] == (Byte)'\\')) return FALSE; // unix style

		if (len >= 3 && bytes[1] == (Byte)':' && (bytes[2] == '/' || bytes[2] == '\\'))
		{
			Byte drive = bytes[0];
			Bool validDrive = FALSE;
			if (drive >= (Byte)'A' && drive <= (Byte)'Z') validDrive = TRUE;
			else if (drive >= (Byte)'a' && drive <= (Byte)'z') validDrive = TRUE;
			if (validDrive)
			{
				rootLength = 2;
				isAbsolute = TRUE;
				return TRUE;
			}
			else return FALSE;
		}
		else
		{
			rootLength = 0;
			isAbsolute = FALSE;
			return TRUE;
		}
	}

	// file I/O operations
	Pointer fileOpen(String fileName, Bool outputMode, Bool appendMode)
	{
		FILE *file;
		_wfopen_s(&file, fileName.wchars().data(), outputMode ? (appendMode ? L"ab+" : L"wb+") : L"rb");
		return (Pointer)file;
	}

	void fileClose(Pointer file)
	{
		if (file) fclose((FILE*)file);
	}

	void filePrint(Pointer file, String text, Bool isUtf8)
	{
		if (isUtf8)
		{
			if (!text.isEmpty()) fwrite(text.bytes(), sizeof(Byte), text.length(), (FILE*)file);

			const Byte enter = '\n';
			fwrite(&enter, sizeof(Byte), 1, (FILE*)file);
		}
		else
		{
			if (!text.isEmpty())
			{
				Array<WChar> wchars = text.wchars();
				Array<Char> chars(wchars.size() * 3);
				UInt charsLength = wCharToAnsi(wchars.data(), wchars.size() - 1, (Byte*)chars.data(), chars.size());
				chars[charsLength] = 0;

				fwrite(chars.data(), sizeof(Char), charsLength, (FILE*)file);
			}

			const Byte enter[2] = { '\r', '\n' };
			fwrite(enter, sizeof(Byte) * 2, 1, (FILE*)file);
		}
	}

	String fileScan(Pointer file, Char buffer[SCAN_SIZE], Bool isUtf8)
	{
		Char *buf = fgets(buffer, SCAN_SIZE, (FILE*)file);
		if (!buf) return String();

		buf[SCAN_SIZE - 1] = 0;
		UInt len = (UInt)strlen(buf);
		if (len == 0) return String();

		// remove LF, CR or CRLF
		if (buf[len - 1] == 10)
		{
			if (len >= 2 && buf[len - 2] == 13) len -= 2;
			else len -= 1;
		}
		else if (buf[len - 1] == 13) len -= 1;
		buf[len] = 0;

		if (isUtf8)
		{
			return buf;
		}
		else
		{
			Array<WChar> wchars(len + 1);
			UInt wcharsLength = ansiToWChar((Byte*)buf, len, wchars.data(), wchars.size());
			wchars[wcharsLength] = 0;
			return wchars;
		}
	}

	Binary fileInput(Pointer file, UInt size, ULong fileSize, ULong filePos)
	{
		if (filePos >= fileSize) return Binary();
		Binary out((UInt)math::min((ULong)size, fileSize - filePos));
		fread(out.data(), sizeof(Byte), out.size(), (FILE*)file);
		return out;
	}

	void fileOutput(Pointer file, Binary data)
	{
		fwrite(data.data(), sizeof(Byte), data.size(), (FILE*)file);
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
			if (fileName.isEmpty() || fileName == "." || fileName == "..") continue;

			if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) out[out.size()] = fileName + separator;
			else out[out.size()] = fileName;
		}
		if (file) FindClose(file);

		return out.toArray(Region(0, out.size()));
	}
}

#endif
