
namespace file_internal
{
	using namespace spadas;

	// pathes
	class PathesInfo
	{
	public:
		String workPath;
		ListNode<String> workPathComponents;
		String executableFolderPath;
		String homePath;
		String spadasFilesPath;
		PathesInfo();
		void setWorkPath(String path);
		~PathesInfo();
	};
	extern PathesInfo pathesInfo;

	String getWorkPathString();
	String getExecutableFolderPathString();
	String getHomePathString();
	String getSpadasFilesPathString();
	Char getSeparatorChar();

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
