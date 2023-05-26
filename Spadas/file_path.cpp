
#include "file.h"

namespace file_internal
{
	Bool/* isValid */ getContentsString(String pathString, Bool& isAbsolute, String& rootString, String& contentsString, Char separator)
	{
		const Byte *pathStringData = pathString.bytes();
		if ((Byte)separator == 0x2f) // Linux style
		{
			if (pathString.length() >= 2 && pathStringData[1] == (Byte)':') return FALSE;

			rootString = String();
			if (pathString.isEmpty())
			{
				isAbsolute = FALSE;
				contentsString = String();
			}
			else if (pathStringData[0] == (Byte)separator)
			{
				isAbsolute = TRUE;
				contentsString = pathString.subString(1);
			}
			else
			{
				isAbsolute = FALSE;
				contentsString = pathString;
			}
		}
		else // Window style
		{
			if (pathString.length() >= 1 && pathStringData[0] == (Byte)separator) return FALSE;

			isAbsolute = pathString.length() >= 2 && pathStringData[1] == (Byte)':';
			if (isAbsolute)
			{
				rootString = pathString.subString(0, 2);
				contentsString = pathString.subString(3);
			}
			else
			{
				rootString = String();
				contentsString = pathString;
			}
		}
		return TRUE;
	}

	ListNode<String> parseWorkPath(String workPathString)
	{
		ListNode<String> workPathComponents;
		workPathComponents.joinNext(workPathComponents);

		Char separator = getSeparatorChar();

		if (workPathString.isEmpty() || workPathString.bytes()[workPathString.length() - 1] != (Byte)separator) return ListNode<String>();
		workPathString = workPathString.subString(0, workPathString.length() - 1);

		Bool dummy;
		String rootString, contentsString;
		getContentsString(workPathString, dummy, rootString, contentsString, separator);
		Array<StringSpan> contents = contentsString.split(separator);

		workPathComponents.value() = rootString;
		for (UInt i = 0; i < contents.size(); i++)
		{
			workPathComponents.insertPrevious(contents[i]);
		}
		return workPathComponents;
	}

    String generateFullPath(ListNode<String> root, Bool isFolder)
    {
        String separatorString = getSeparatorChar();
        String out = String::createWithSize(1024);
        out += root.value();
        ListNode<String> currentNode = root.next();
		while (currentNode != root)
		{
			out += separatorString;
			out += currentNode.value();
			currentNode.goNext();
		}
        if (isFolder) out += separatorString;
        return out;
    }

	void addFolderContents(ArrayX<Path> contents, Path folderPath, String folderString)
	{
		Char separator = getSeparatorChar();
		Array<String> folderContents = folderGetContents(folderString);
		for (UInt i = 0; i < folderContents.size(); i++)
		{
			String contentString = folderContents[i];
			Path contentPath = folderPath.childPath(contentString);
			contents[contents.size()] = contentPath;
			if (contentString.bytes()[contentString.length() - 1] == (Byte)separator) // folder
			{
				addFolderContents(contents, contentPath, folderString + contentString);
			}
		}
	}

	PathsInfo::PathsInfo()
	{
		executableFolderPath = getExecutableFolderPathString();
		workPath = getWorkPathString();
		homePath = getHomePathString();
		spadasFilesPath = getSpadasFilesPathString();

		if (!workPath.isEmpty()) workPathComponents = parseWorkPath(workPath);
		if (!spadasFilesPath.isEmpty() && !folderExist(spadasFilesPath)) folderCreate(spadasFilesPath);
	}
	void PathsInfo::setWorkPath(String path)
	{
		workPath = path;
		if (!workPath.isEmpty()) workPathComponents = parseWorkPath(workPath);
	}
	void PathsInfo::setExecutableFolderPath(String path)
	{
		executableFolderPath = path;
	}
	PathsInfo::~PathsInfo()
	{
		workPathComponents.collapse();
	}
	PathsInfo pathsInfo;
}

namespace spadas
{
	class PathVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(Path, Vars)

		Bool isFolder;
		ListNode<String> components;
		PathVars()
		{
			isFolder = FALSE;
			components.joinNext(components);
		}
		~PathVars()
		{
			components.collapse();
		}
		String toString() override
		{
			return file_internal::generateFullPath(components, isFolder);
		}
	};
}

using namespace spadas;
using namespace spadas::console;
using namespace file_internal;

const String spadas::Path::TypeName = "spadas.Path";

Path::Path()
{
}

Path::Path(String pathString)
{
	/* standardize input */
	Char separator = getSeparatorChar();
	String separatorString(separator);
	
	if (pathString.isEmpty() || pathString == ".") pathString = String(".") + separatorString;
	if (pathString == "..") pathString = String("..") + separatorString;

	if ((Byte)separator == 0x2f) pathString = pathString.replace((Char)0x5c, (Char)0x2f);
	else pathString = pathString.replace((Char)0x2f, (Char)0x5c);
	
	/* check if pathString is a folder and remove the last separator */
	Bool isFolder = !pathString.isEmpty() && pathString.bytes()[pathString.length() - 1] == (Byte)separator;
	if (isFolder) pathString = pathString.subString(0, pathString.length() - 1);
	
	/* check if pathString is absolute and get root and contents */
	Bool isAbsolute;
	String rootString, contentsString;
	Bool isValid = getContentsString(pathString, isAbsolute, rootString, contentsString, separator);
	if (!isValid) return;

	/* generate components loop */
	Array<StringSpan> contents = contentsString.split(separator);
	if (isAbsolute)
	{
		setVars(new PathVars(), TRUE);
		vars->components.value() = rootString;
		for (UInt i = 0; i < contents.size(); i++) vars->components.insertPrevious(contents[i]);
	}
	else
	{
		if (pathsInfo.workPath.isEmpty()) return;
		setVars(new PathVars(), TRUE);
		vars->components.collapse();
		vars->components = pathsInfo.workPathComponents.cloneList();
		for (UInt i = 0; i < contents.size(); i++) vars->components.insertPrevious(contents[i]);
	}
	
	/* remove components "." and ".." */
	ListNode<String> currentNode = vars->components.next();
	while (currentNode != vars->components)
	{
		if (currentNode.value() == ".")
		{
			currentNode = currentNode.previous();
			currentNode.removeNext();
		}
		else if (currentNode.value() == "..")
		{
			currentNode = currentNode.next();
			currentNode.removePrevious();
			if (currentNode.previous() != vars->components) currentNode.removePrevious();
			currentNode = currentNode.previous();
		}
		currentNode = currentNode.next();
	}
	
	/* set isFolder and valid */
	vars->isFolder = isFolder;
}

String Path::name()
{
	if (!vars) return String();
	
	String fileFullName = vars->components.previous().value();
	Array<UInt> dotIndices = fileFullName.search(".");
	
	if (dotIndices.isEmpty()) return fileFullName;
	else return fileFullName.subString(0, dotIndices[dotIndices.size() - 1]);
}

String Path::extension()
{
	if (!vars) return String();
	
	String fileFullName = vars->components.previous().value();
	Array<UInt> dotIndices = fileFullName.search(".");
	
	if (dotIndices.isEmpty()) return String();
	else return fileFullName.subString(dotIndices[dotIndices.size() - 1]);
}

String Path::fullName()
{
	SPADAS_ERROR_RETURNVAL(!vars, String());
	return vars->components.previous().value().clone();
}

String Path::fullPath()
{
	SPADAS_ERROR_RETURNVAL(!vars, String());
	return generateFullPath(vars->components, vars->isFolder);
}

Bool Path::operator ==(Path path)
{
	return fullPath() == path.fullPath();
}

Bool Path::operator !=(Path path)
{
	return fullPath() != path.fullPath();
}

Word Path::getHash()
{
	return fullPath().getHash();
}

Bool Path::exist()
{
	SPADAS_ERROR_RETURNVAL(!vars, FALSE);
	String fullPathString = generateFullPath(vars->components, vars->isFolder);
	if (vars->isFolder) return folderExist(fullPathString);
	else return fileExist(fullPathString);
}

Bool Path::isFile()
{
	SPADAS_ERROR_RETURNVAL(!vars, FALSE);
	return !vars->isFolder;
}

Bool Path::isFolder()
{
	SPADAS_ERROR_RETURNVAL(!vars, FALSE);
	return vars->isFolder;
}

void file_internal::removeTree(Path path)
{
	if (path.isFolder())
	{
		Char separator = getSeparatorChar();
		Array<Path> contentsInFolder0 = path.folderContents(TRUE);
		for (Int i = contentsInFolder0.size() - 1; i >= 0; i--)
		{
			String contentPathString = contentsInFolder0[i].fullPath();
			if (contentPathString.bytes()[contentPathString.length() - 1] == (Byte)separator) folderRemove(contentPathString);
			else fileRemove(contentPathString);
		}
		folderRemove(path.fullPath());
	}
	else
	{
		fileRemove(path.fullPath());
	}
}

void Path::remove()
{
	SPADAS_ERROR_RETURN(!vars);

	if (!exist()) return;

	removeTree(*this);
}

void file_internal::moveOrCopy(Path srcPath, Path dstPath, Bool isCopy, Bool mergeDst)
{
	if (srcPath == dstPath) return;
	SPADAS_ERROR_RETURN(srcPath.isFolder() != dstPath.isFolder());

	String srcPathFullString = srcPath.fullPath(), dstPathFullString = dstPath.fullPath();
	SPADAS_ERROR_RETURN(!srcPath.exist());
	SPADAS_ERROR_RETURN(srcPath.isFolder() && !dstPathFullString.search(srcPathFullString).isEmpty());

	if (srcPath.isFolder())
	{
		String dstPathFullString = dstPath.fullPath();
		if (!dstPath.exist()) dstPath.folderMake();
		else if (!mergeDst)
		{
			folderRemove(dstPathFullString);
			folderCreate(dstPathFullString);
		}

		if (!mergeDst && !isCopy)
		{
			folderMove(srcPath.fullPath(), dstPathFullString);
		}
		else // mergeDst  || isCopy
		{
			Array<Path> srcContents = srcPath.folderContents(TRUE);
			UInt srcPathLen = srcPath.fullPath().length();
			for (UInt i = 0; i < srcContents.size(); i++)
			{
				String srcContentString = srcContents[i].fullPath();
				String childPath = srcContentString.subString(srcPathLen);
				Path dstContent = dstPath.childPath(childPath);
				String dstContentString = dstContent.fullPath();
				if (dstContent.isFolder())
				{
					folderCreate(dstContentString);
					continue;
				}

				if (mergeDst && fileExist(dstContentString))
				{
					fileRemove(dstContentString);
				}
				if (isCopy) fileCopy(srcContentString, dstContentString);
				else fileMove(srcContentString, dstContentString);
			}

			if (!isCopy)
			{
				for (Int i = srcContents.size() - 1; i >= 0; i--)
				{
					if (srcContents[i].isFolder()) folderRemove(srcContents[i].fullPath());
				}
				folderRemove(srcPathFullString);
			}

		} // !mergeDst && !isCopy
	}
	else // is file
	{
		Path dstParentPath = dstPath.parentFolder();
		if (dstParentPath.isValid())
		{
			if (!dstParentPath.exist())
			{
				dstParentPath.folderMake();
			}
			else if (dstPath.exist())
			{
				fileRemove(dstPathFullString);
			}

			if (isCopy) fileCopy(srcPathFullString, dstPathFullString);
			else fileMove(srcPathFullString, dstPathFullString);
		}
	}
}

void Path::moveTo(Path dstPath)
{
	SPADAS_ERROR_RETURN(!vars || !dstPath.vars || vars->isFolder);
	moveOrCopy(*this, dstPath, FALSE, FALSE);
}

void Path::moveTo(Path dstPath, Bool mergeDst)
{
	SPADAS_ERROR_RETURN(!vars || !dstPath.vars || !vars->isFolder);
	moveOrCopy(*this, dstPath, FALSE, mergeDst);
}

void Path::copyTo(Path dstPath)
{
	SPADAS_ERROR_RETURN(!vars || !dstPath.vars || vars->isFolder);
	moveOrCopy(*this, dstPath, TRUE, FALSE);
}

void Path::copyTo(Path dstPath, Bool mergeDst)
{
	SPADAS_ERROR_RETURN(!vars || !dstPath.vars || !vars->isFolder);
	moveOrCopy(*this, dstPath, TRUE, mergeDst);
}

ULong Path::fileSize()
{
	SPADAS_ERROR_RETURNVAL(!vars, 0);
	SPADAS_ERROR_RETURNVAL(!exist(), 0);
	SPADAS_ERROR_RETURNVAL(vars->isFolder, 0);
	
	Pointer filePtr = fileOpen(fullPath(), FALSE);
	SPADAS_ERROR_RETURNVAL(filePtr == NULL, 0);
	
	ULong size = fileGetSize(filePtr);
	fileClose(filePtr);
	
	return size;
}

void Path::fileCreate(UInt size)
{
	SPADAS_ERROR_RETURN(!vars);
	SPADAS_ERROR_RETURN(vars->isFolder);

	Path parentFolder = this->parentFolder();
	SPADAS_ERROR_RETURN(parentFolder.isNull());

	if (!parentFolder.exist()) parentFolder.folderMake();
	
	String filePathString = fullPath();
	Pointer file = fileOpen(filePathString, TRUE);
	SPADAS_ERROR_RETURN(file == NULL);

	if (size != 0)
	{
		fileOutput(file, Binary(size, 0));
		fileFlush(file);
	}

	fileClose(file);
}

void Path::folderMake()
{
	SPADAS_ERROR_RETURN(!vars);
	SPADAS_ERROR_RETURN(!vars->isFolder);
	
	if (exist()) return;
	
	Path parent = parentFolder();
	if (parent.isValid() && !parent.exist())
	{
		parent.folderMake();
	}
	
	folderCreate(fullPath());
}

Path Path::childFile(String childName)
{
	SPADAS_ERROR_RETURNVAL(!vars, Path());
	SPADAS_ERROR_RETURNVAL(!isFolder(), Path());
    
    Char separator = getSeparatorChar();
	SPADAS_ERROR_RETURNVAL(childName.isEmpty() || childName == "." || childName == ".." || childName.bytes()[childName.length() - 1] == (Byte)separator, Path());
	
	Path path;
	path.setVars(new PathVars(), TRUE);
    path.vars->isFolder = FALSE;
	path.vars->components.collapse();
    path.vars->components = vars->components.cloneList();
	path.vars->components.insertPrevious(childName);
    return path;
}

Path Path::childFolder(String childName)
{
	SPADAS_ERROR_RETURNVAL(!vars, Path());
	SPADAS_ERROR_RETURNVAL(!isFolder(), Path());
    
	Char separator = getSeparatorChar();
	SPADAS_ERROR_RETURNVAL(childName.isEmpty() || childName == "." || childName == ".." || childName.bytes()[childName.length() - 1] == (Byte)separator, Path());
	
	Path path;
	path.setVars(new PathVars(), TRUE);
    path.vars->isFolder = TRUE;
	path.vars->components.collapse();
    path.vars->components = vars->components.cloneList();
    path.vars->components.insertPrevious(childName);
    return path;
}

Path Path::childPath(String pathString)
{
	SPADAS_ERROR_RETURNVAL(!vars, Path());
	SPADAS_ERROR_RETURNVAL(!isFolder(), Path());

	/* standardize input */
	Char separator = getSeparatorChar();
	String separatorString(separator);
	
	if (pathString.isEmpty() || pathString == ".") pathString = String(".") + separatorString;
	if (pathString == "..") pathString = String("..") + separatorString;
	
	if ((Byte)separator == 0x2f) pathString = pathString.replace((Char)0x5c, (Char)0x2f);
	else pathString = pathString.replace((Char)0x2f, (Char)0x5c);
	
	/* check if pathString is a folder and remove the last separator */
	Bool isFolder = !pathString.isEmpty() && pathString.bytes()[pathString.length() - 1] == (Byte)separator;
	if (isFolder) pathString = pathString.subString(0, pathString.length() - 1);
	
	/* check if pathString is absolute and get root and contents */
	Bool isAbsolute;
	String rootString, contentsString;
	Bool isValid = getContentsString(pathString, isAbsolute, rootString, contentsString, separator);
	SPADAS_ERROR_RETURNVAL(!isValid || isAbsolute, Path());

	Array<StringSpan> contents = contentsString.split(separator);
	
	/* generate components loop */
	Path out;
	out.setVars(new PathVars(), TRUE);
	out.vars->components.collapse();
	out.vars->components = vars->components.cloneList();
	for (UInt i = 0; i < contents.size(); i++) out.vars->components.insertPrevious(contents[i]);
	
	/* remove components "." and ".." */
	ListNode<String> currentNode = out.vars->components.next();
	while (currentNode != out.vars->components)
	{
		if (currentNode.value() == ".")
		{
			currentNode = currentNode.previous();
			currentNode.removeNext();
		}
		else if (currentNode.value() == "..")
		{
			currentNode = currentNode.next();
			currentNode.removePrevious();
			if (currentNode.previous() != out.vars->components) currentNode.removePrevious();
			currentNode = currentNode.previous();
		}
		currentNode = currentNode.next();
	}
	
	/* set isFolder and valid */
	out.vars->isFolder = isFolder;
	return out;
}

Bool Path::contain(Path path)
{
	String dummy;
	return contain(path, dummy);
}

Bool Path::contain(Path path, String& pathString)
{
	SPADAS_ERROR_RETURNVAL(!vars, FALSE);
	SPADAS_ERROR_RETURNVAL(!path.vars, FALSE);
	SPADAS_ERROR_RETURNVAL(!isFolder(), FALSE);

	String srcPath = fullPath();
	String dstPath = path.fullPath();
	UInt srcPathLen = srcPath.length();
	UInt dstPathLen = dstPath.length();

	if (dstPathLen < srcPathLen) return FALSE;
	if (!dstPath.startsWith(srcPath)) return FALSE;

	if (dstPathLen == srcPathLen)
	{
		pathString = String();
		return TRUE;
	}
	else
	{
		pathString = dstPath.subString(srcPathLen);
		return TRUE;
	}
}

Path Path::parentFolder()
{
	SPADAS_ERROR_RETURNVAL(!vars, Path());
    
	if (vars->components.next() == vars->components) return Path();
    
    Path path;
	path.setVars(new PathVars(), TRUE);
    path.vars->isFolder = TRUE;
	path.vars->components.collapse();
    path.vars->components = vars->components.cloneList();
    path.vars->components.removePrevious();
    return path;
}

Array<Path> Path::folderContents(Bool includeAllLevels)
{
	SPADAS_ERROR_RETURNVAL(!vars, Array<Path>());
	SPADAS_ERROR_RETURNVAL(!isFolder(), Array<Path>());

	if (!exist()) return Array<Path>();

	String targetFolder = fullPath();
	if (includeAllLevels)
	{
		ArrayX<Path> buf(256, Path());
		addFolderContents(buf, *this, targetFolder);
		return buf.toArray(Region(0, buf.size()));
	}
	else
	{
		Array<String> contents = folderGetContents(targetFolder);
		Array<Path> out(contents.size());
		for (UInt i = 0; i < out.size(); i++)
		{
			out[i] = childPath(contents[i]);
		}
		return out;
	}
}

String Path::separator()
{
	return getSeparatorChar();
}

Path Path::workPath()
{
	if (pathsInfo.workPath.isEmpty()) return Path();
	else return Path(pathsInfo.workPath);
}

Path Path::executableFolderPath()
{
	if (pathsInfo.executableFolderPath.isEmpty()) return Path();
	else return Path(pathsInfo.executableFolderPath);
}

Path Path::homePath()
{
	if (pathsInfo.homePath.isEmpty()) return Path();
	else return Path(pathsInfo.homePath);
}

Path Path::spadasFilesPath()
{
	if (pathsInfo.spadasFilesPath.isEmpty()) return Path();
	else return Path(pathsInfo.spadasFilesPath);
}

void Path::setWorkPath(String pathString)
{
	String separator = Path::separator();
	if (!pathString.isEmpty() && pathString.endsWith(separator)) pathsInfo.setWorkPath(pathString);
}

void Path::setExecutableFolderPath(String pathString)
{
	String separator = Path::separator();
	if (!pathString.isEmpty() && pathString.endsWith(separator)) pathsInfo.setExecutableFolderPath(pathString);
}