
#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)

#include "file.h"
#include <memory.h>
#include <sys/stat.h>
#include <wchar.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#undef NULL
#define NULL 0

#if defined(SPADAS_ENV_MACOS)
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace file_internal
{
	using namespace spadas;

	// paths
	Char getSeparatorChar()
	{
		return '/';
	}

#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_NILRT)
	String getAppParentPathString(Bool asExecutable)
	{
		Char procname[FILENAME_MAX];
		int len = readlink("/proc/self/exe", procname, FILENAME_MAX - 1);
		if (len <= 0)  return String();

		procname[len] = '\0';
		String executablePathString = procname;

		Array<UInt> slashLocations = executablePathString.search('/');
		if (slashLocations.isEmpty()) return String();

		return executablePathString.span(0, slashLocations.last() + 1).clone();
	}

	String getHomePathString()
	{
		struct passwd *pwuid = getpwuid(getuid());
		if (pwuid == 0) return String();

		String homePath(pwuid->pw_dir);
		Char separator = getSeparatorChar();
		if (homePath.bytes()[homePath.length()-1] != (Byte)separator) return homePath + separator;
		else return homePath;
	}
#endif
#if defined(SPADAS_ENV_MACOS)
	String getAppParentPathString(Bool asExecutable)
	{
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		if (asExecutable)
		{
			CFURLRef url = CFBundleCopyExecutableURL(mainBundle);
			CFStringRef filePathString = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
			CFRelease(url);

			char buffer[1024];
			bool ok = CFStringGetCString(filePathString, buffer, 1024, kCFStringEncodingUTF8);
			CFRelease(filePathString);

			if (!ok) return String();

			String executablePathString(buffer);
			Array<UInt> slashLocations = executablePathString.search('/');
			if (slashLocations.isEmpty()) return String();

			return executablePathString.span(0, slashLocations.last() + 1).clone();
		}
		else
		{
			CFURLRef url = CFBundleCopyBundleURL(mainBundle);
			CFStringRef filePathString = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
			CFRelease(url);

			char buffer[1024];
			bool ok = CFStringGetCString(filePathString, buffer, 1024, kCFStringEncodingUTF8);
			CFRelease(filePathString);

			if (!ok) return String();
			
			String bundlePathString(buffer);
			if (bundlePathString.endsWith(".app"))
			{
				Array<UInt> slashLocations = bundlePathString.search('/');
				if (slashLocations.isEmpty()) return String();

				return bundlePathString.span(0, slashLocations.last() + 1).clone();
			}
			else
			{
				return bundlePathString + "/";
			}
		}
	}

	String getHomePathString()
	{
		return String(getenv("HOME")) + "/";
	}
#endif

	String getCurrentPathString()
	{
		Char pathChars[1024];
		getcwd(pathChars, 1024);
		pathChars[1023] = 0;

		String output(pathChars);
		if (output.endsWith("/")) return output;
		else return output + "/";
	}

	void setCurrentPathString(String pathString)
	{
		chdir(pathString.chars().data());
	}

	Bool isPathStringValid(String pathString, UInt& rootLength, Bool& isAbsolute)
	{
		UInt len = pathString.length();
		const Byte* bytes = pathString.bytes();

		if (len >= 2 && bytes[1] == (Byte)':') return FALSE; // windows style

		if (len >= 1 && (bytes[0] == '/' || bytes[0] == '\\'))
		{
			rootLength = 0;
			isAbsolute = TRUE;
			return TRUE;
		}
		else
		{
			rootLength = 0;
			isAbsolute = FALSE;
			return TRUE;
		}
	}

	// file I/O operations
	Pointer fileOpen(String file, Bool outputMode, Bool appendMode)
	{
		return (Pointer)fopen(file.chars().data(), outputMode ? (appendMode ? "a+" : "w+") : "r");
	}

	void fileClose(Pointer file)
	{
		fclose((FILE*)file);
	}

	void filePrint(Pointer file, String text, Bool isUtf8)
	{
		if (!text.isEmpty()) fwrite(text.bytes(), 1, text.length(), (FILE*)file);

		const unsigned char enter = '\n';
		fwrite(&enter, 1, 1, (FILE*)file);
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
			if (len >= 2 && buf[len - 2] == 13) buf[len - 2] = 0;
			else buf[len - 1] = 0;
		}
		else if (buf[len - 1] == 13) buf[len - 1] = 0;

		return buf;
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
		fseek((FILE*)file, 0, SEEK_END);
		return (ULong)ftell((FILE*)file);
	}

	ULong filePos(Pointer file)
	{
		return (ULong)ftell((FILE*)file);
	}

	ULong fileSeek(Pointer file, ULong pos)
	{
		fseek((FILE*)file, pos, SEEK_SET);
		return (ULong)ftell((FILE*)file);
	}

	Bool fileExist(String file)
	{
		struct stat buf;
		if (stat(file.chars().data(), &buf) != 0) return FALSE;
		return (buf.st_mode & S_IFMT) == S_IFREG;
	}

	void fileCopy(String srcFile, String dstFile)
	{
		FILE *in = fopen(srcFile.chars().data(), "r");
		FILE *out = fopen(dstFile.chars().data(), "w+" );
		if (!in || !out) return;

		fseek(in , 0 , SEEK_END);
		UInt fileSize = (UInt)ftell(in);

		Binary buf(fileSize);

		fseek(in, 0, SEEK_SET);
		if (fread(buf.data(), 1, fileSize, in)) {};

		fwrite(buf.data(), 1, fileSize, out);
		fflush(out);

		fclose(in);
		fclose(out);
	}

	void fileMove(String srcFile, String dstFile)
	{
		rename(srcFile.chars().data(), dstFile.chars().data());
	}

	void fileRemove(String file)
	{
		remove(file.chars().data());
	}

	Bool folderExist(String folder)
	{
		struct stat buf;
		if (stat(folder.chars().data(), &buf) != 0) return FALSE;
		return (buf.st_mode & S_IFMT) == S_IFDIR;
	}

	void folderCreate(String folder)
	{
		mkdir(folder.chars().data(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	}

	void folderMove(String srcFolder, String dstFolder)
	{
		rename(srcFolder.chars().data(), dstFolder.chars().data());
	}

	void folderRemove(String folder)
	{
		rmdir(folder.chars().data());
	}

	Array<String> folderGetContents(String targetFolder)
	{
		DIR *folder = opendir(targetFolder.chars().data());
		if (folder == NULL) return Array<String>();

		String separator = getSeparatorChar();
		ArrayX<String> out;
		dirent *content;
		while ((content = readdir(folder)) != NULL)
		{
			String fileName = content->d_name;
			if (fileName.isEmpty() || fileName == "." || fileName == "..") continue;

			Bool isFolder = FALSE;
			if ((Int)content->d_type == (Int)DT_UNKNOWN)
			{
				struct stat fileStat;
				String contentPath = targetFolder + fileName;
				stat(contentPath.chars().data(), &fileStat);
				isFolder = S_ISDIR(fileStat.st_mode);
			}
			else isFolder = ((Int)content->d_type & (Int)DT_DIR) != 0;

			if (isFolder) out[out.size()] = fileName + separator;
			else out[out.size()] = fileName;
		}
		closedir(folder);

		return out.toArray(Region(0, out.size()));
	}
}

#endif
