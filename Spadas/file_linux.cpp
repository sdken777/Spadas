
#include "spadas.h"

#include "file.h"

#if defined(SPADAS_ENV_LINUX)

#include <memory.h>
#include <sys/stat.h>
#include <wchar.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

namespace file_internal
{
	using namespace spadas;

	Char getSeparatorChar()
	{
		return '/';
	}

	String getWorkPathString()
	{
		Char procname[FILENAME_MAX];
		int len = readlink("/proc/self/exe", procname, FILENAME_MAX - 1);
		if (len <= 0)  return String();

		procname[len] = '\0';
		String executablePathString = procname;

		Array<UInt> slashLocations = executablePathString.search(L'/');
		if (slashLocations.isEmpty()) return String();

		if (slashLocations.size() >= 4) // 调试时判定
		{
			UInt rootSlash = slashLocations[slashLocations.size() - 3];
			UInt releaseBeginSlash = slashLocations[slashLocations.size() - 2];
			UInt releaseEndSlash = slashLocations[slashLocations.size() - 1];

			String releaseString = String(executablePathString, Region(releaseBeginSlash + 1, releaseEndSlash - releaseBeginSlash - 1)).toLower();
			String realFolderString = String(executablePathString, Region(0, rootSlash)) + "/binx/";
			String realFileString = realFolderString + String(executablePathString, Region(releaseEndSlash + 1, UINF));
			if (releaseString == "release" && fileExist(realFileString)) return realFolderString;
		}

		return String(executablePathString, Region(0, slashLocations.last() + 1));
	}

	String getHomePathString()
	{
		struct passwd *pwuid = getpwuid(getuid());
		if (pwuid == 0) return String();

		String homePath(pwuid->pw_dir);
		String separator = getSeparatorChar();
		if (homePath[homePath.length()-1] != separator[0]) return homePath + separator;
		else return homePath;
	}

	String getSpadasFilesPathString()
	{
		String homePath = getHomePathString();
		if (homePath.isEmpty()) return String();
		else return homePath + "SpadasFiles/";
	}

	Pointer fileOpen(String file, Bool outputMode)
	{
		return (Pointer)fopen(file.dataA(), outputMode ? "w+" : "r");
	}

	void fileClose(Pointer file)
	{
		fclose((FILE*)file);
	}

	void filePrint(Pointer file, String text)
	{
		fprintf((FILE*)file, "%s\n", text.dataA());
	}

	String fileScan(Pointer file, Char buffer[SCAN_SIZE], Bool isUtf8)
	{
		Char *buf = fgets(buffer, SCAN_SIZE, (FILE*)file);

		String out;
		if (buf)
		{
			buf[SCAN_SIZE - 1] = 0;
			UInt len = (UInt)strlen(buf);

			// remove LF, CR or CRLF
			if (len >= 1)
			{
				if (buf[len - 1] == 10)
				{
					if (len >= 2 && buf[len - 2] == 13) buf[len - 2] = 0;
					else buf[len - 1] = 0;
				}
				else if (buf[len - 1] == 13) buf[len - 1] = 0;
			}

			out = buf;
		}

		return out;
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
		if (stat(file.dataA(), &buf) != 0) return FALSE;
		return (buf.st_mode & S_IFMT) == S_IFREG;
	}

	void fileCopy(String srcFile, String dstFile)
	{
		FILE *in = fopen(srcFile.dataA(), "r");
		FILE *out = fopen(dstFile.dataA(), "w+" );
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
		rename(srcFile.dataA(), dstFile.dataA());
	}

	void fileRemove(String file)
	{
		remove(file.dataA());
	}

	Bool folderExist(String folder)
	{
		struct stat buf;
		if (stat(folder.dataA(), &buf) != 0) return FALSE;
		return (buf.st_mode & S_IFMT) == S_IFDIR;
	}

	void folderCreate(String folder)
	{
		mkdir(folder.dataA(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	}

	void folderMove(String srcFolder, String dstFolder)
	{
		rename(srcFolder.dataA(), dstFolder.dataA());
	}

	void folderRemove(String folder)
	{
		rmdir(folder.dataA());
	}

	Array<String> folderGetContents(String targetFolder)
	{
		DIR *folder = opendir(targetFolder.dataA());
		if (folder == NULL) return Array<String>();

		String separator = getSeparatorChar();
		ArrayX<String> out;
		dirent *content;
		while ((content = readdir(folder)) != NULL)
		{
			String fileName = content->d_name;
			if (fileName == "." || fileName == "..") continue;

			Bool isFolder = ((Int)content->d_type & (Int)DT_DIR) != 0;
			if (isFolder) out[out.size()] = fileName + separator;
			else out[out.size()] = fileName;
		}
		closedir(folder);

		return out.toArray(Region(0, out.size()));
	}
}

#endif
