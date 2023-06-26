
#include "spadas.h"

namespace file_internal
{
	using namespace spadas;

	// paths
	class PathsInfo
	{
	public:
		Array<StringSpan> workPathComponents;
		String workPathString;
		Array<StringSpan> executableFolderPathComponents;
		String executableFolderPathString;
		Array<StringSpan> homePathComponents;
		String homePathString;
		Array<StringSpan> spadasFilesPathComponents;
		String spadasFilesPathString;

		PathsInfo();
		void setWorkPath(String path);
		void setExecutableFolderPath(String path);
	};
	extern PathsInfo pathsInfo;

	String getWorkPathString();
	String getExecutableFolderPathString();
	String getHomePathString();
	String getSpadasFilesPathString();
	Char getSeparatorChar();
	Bool isPathStringValid(String pathString, UInt& rootLength, Bool& isAbsolute);

	// file I/O operations
	const UInt SCAN_SIZE = 1048576; // 1024 * 1024

	Pointer fileOpen(String fileName, Bool outputMode);
	void fileClose(Pointer file);
	void filePrint(Pointer file, String text);
	String fileScan(Pointer file, Char buffer[SCAN_SIZE], Bool isUtf8);
	Binary fileInput(Pointer file, UInt inputSize, ULong fileSize, ULong filePos);
	void fileOutput(Pointer file, Binary data);
	void fileFlush(Pointer file);
	ULong fileGetSize(Pointer file); // should be called before any I/O
	ULong filePos(Pointer file);
	ULong fileSeek(Pointer file, ULong pos);

	// file functions
	Bool fileExist(String name);
	void fileMove(String srcFileName, String dstFileName);
	void fileCopy(String srcFileName, String dstFileName);
	void fileRemove(String fileName);
	Bool folderExist(String name);
	void folderCreate(String folderName);
	void folderMove(String srcFolderName, String dstFolderName);
	void folderRemove(String folderName);
	Array<String> folderGetContents(String folderPath);

	// common functions
	void moveOrCopy(Path srcPath, Path dstPath, Bool isCopy, Bool mergeDst);
	void removeTree(Path path);
}
