
#include "spadas.h"

#include "file.h"

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
	};
}

namespace file_internal
{
	Bool/* isValid */ getContentsString(String pathString, Bool& isAbsolute, String& rootString, String& contentsString, String separator)
	{
		if (separator[0] == (WChar)0x2f) // Linux style
		{
			if (pathString.length() >= 2 && pathString[1] == L':') return FALSE;

			rootString = String();
			if (pathString.isEmpty())
			{
				isAbsolute = FALSE;
				contentsString = String();
			}
			else if (pathString[0] == separator[0])
			{
				isAbsolute = TRUE;
				contentsString = String(pathString, Region(1, UINF));
			}
			else
			{
				isAbsolute = FALSE;
				contentsString = pathString;
			}
		}
		else // Window style
		{
			if (pathString.length() >= 1 && pathString[0] == separator[0]) return FALSE;

			isAbsolute = pathString.length() >= 2 && pathString[1] == L':';
			if (isAbsolute)
			{
				rootString = String(pathString, Region(0, 2));
				contentsString = String(pathString, Region(3, UINF));
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

		String separator = getSeparatorChar();

		if (workPathString.isEmpty() || workPathString[workPathString.length() - 1] != separator[0]) return ListNode<String>();
		workPathString = String(workPathString, Region(0, workPathString.length() - 1));

		Bool dummy;
		String rootString, contentsString;
		getContentsString(workPathString, dummy, rootString, contentsString, separator);
		Array<String> contents = contentsString.split(separator);

		workPathComponents.value() = rootString;
		for (UInt i = 0; i < contents.size(); i++)
		{
			workPathComponents.insertPrevious(contents[i]);
		}
		return workPathComponents;
	}

    String generateFullPath(ListNode<String> root, Bool isFolder)
    {
        String separator = getSeparatorChar();
        String out = String::createWithSize(1024);
        out += root.value();
        ListNode<String> currentNode = root.next();
		while (currentNode != root)
		{
			out += separator;
			out += currentNode.value();
			currentNode.goNext();
		}
        if (isFolder) out += separator;
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
			if (contentString[contentString.length() - 1] == (WChar)separator) // folder
			{
				addFolderContents(contents, contentPath, folderString + contentString);
			}
		}
	}

	PathesInfo::PathesInfo()
	{
		workPath = getWorkPathString();
		homePath = getHomePathString();
		spadasFilesPath = getSpadasFilesPathString();

		if (!workPath.isEmpty()) workPathComponents = parseWorkPath(workPath);
		if (!spadasFilesPath.isEmpty() && !folderExist(spadasFilesPath)) folderCreate(spadasFilesPath);
	}
	void PathesInfo::setWorkPath(String path)
	{
		workPath = path;
		if (!workPath.isEmpty()) workPathComponents = parseWorkPath(workPath);
	}
	PathesInfo::~PathesInfo()
	{
		workPathComponents.collapse();
	}
	PathesInfo pathesInfo;
}

using namespace spadas;
using namespace spadas::console;
using namespace spadas_internal;
using namespace file_internal;

const String spadas::Path::TypeName = "spadas.Path";

Path::Path()
{
}

Path::Path(String pathString)
{
	/* standardize input */
	String separator = getSeparatorChar();
	
	if (pathString.isEmpty() || pathString == ".") pathString = String(".") + separator;
	if (pathString == "..") pathString = String("..") + separator;

	if (separator[0] == (WChar)0x2f) pathString = pathString.replace((WChar)0x5c, (WChar)0x2f);
	else pathString = pathString.replace((WChar)0x2f, (WChar)0x5c);
	
	/* check if pathString is a folder and remove the last separator */
	Bool isFolder = !pathString.isEmpty() && pathString[pathString.length() - 1] == separator[0];
	if (isFolder) pathString = String(pathString, Region(0, pathString.length() - 1));
	
	/* check if pathString is absolute and get root and contents */
	Bool isAbsolute;
	String rootString, contentsString;
	Bool isValid = getContentsString(pathString, isAbsolute, rootString, contentsString, separator);
	if (!isValid) return;

	/* generate components loop */
	Array<String> contents = contentsString.split(separator);
	if (isAbsolute)
	{
		setVars(new PathVars(), TRUE);
		vars->components.value() = rootString;
		for (UInt i = 0; i < contents.size(); i++) vars->components.insertPrevious(contents[i]);
	}
	else
	{
		if (pathesInfo.workPath.isEmpty()) return;
		setVars(new PathVars(), TRUE);
		vars->components.collapse();
		vars->components = pathesInfo.workPathComponents.cloneList();
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
	else return String(fileFullName, Region(0, dotIndices[dotIndices.size() - 1]));
}

String Path::extension()
{
	if (!vars) return String();
	
	String fileFullName = vars->components.previous().value();
	Array<UInt> dotIndices = fileFullName.search(".");
	
	if (dotIndices.isEmpty()) return String();
	else return String(fileFullName, Region(dotIndices[dotIndices.size() - 1], UINF));
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
		WChar separator = (WChar)getSeparatorChar();
		Array<Path> contentsInFolder0 = path.folderContents(TRUE);
		for (Int i = contentsInFolder0.size() - 1; i >= 0; i--)
		{
			String contentPathString = contentsInFolder0[i].fullPath();
			if (contentPathString[contentPathString.length() - 1] == separator) folderRemove(contentPathString);
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
				String childPath = String(srcContentString, Region(srcPathLen, UINF));
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
    
    String separator = getSeparatorChar();
	SPADAS_ERROR_RETURNVAL(childName.isEmpty() || childName == "." || childName == ".." || childName[childName.length() - 1] == separator[0], Path());
	
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
    
    String separator = getSeparatorChar();
	SPADAS_ERROR_RETURNVAL(childName.isEmpty() || childName == "." || childName == ".." || childName[childName.length() - 1] == separator[0], Path());
	
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
	String separator = getSeparatorChar();
	
	if (pathString.isEmpty() || pathString == ".") pathString = String(".") + separator;
	if (pathString == "..") pathString = String("..") + separator;
	
	if (separator[0] == (WChar)0x2f) pathString = pathString.replace((WChar)0x5c, (WChar)0x2f);
	else pathString = pathString.replace((WChar)0x2f, (WChar)0x5c);
	
	/* check if pathString is a folder and remove the last separator */
	Bool isFolder = !pathString.isEmpty() && pathString[pathString.length() - 1] == separator[0];
	if (isFolder) pathString = String(pathString, Region(0, pathString.length() - 1));
	
	/* check if pathString is absolute and get root and contents */
	Bool isAbsolute;
	String rootString, contentsString;
	Bool isValid = getContentsString(pathString, isAbsolute, rootString, contentsString, separator);
	SPADAS_ERROR_RETURNVAL(!isValid || isAbsolute, Path());

	Array<String> contents = contentsString.split(separator);
	
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
	WChar *srcPathData = srcPath.data();
	WChar *dstPathData = dstPath.data();

	Bool same = TRUE;
	for (UInt i = 0; i < srcPathLen; i++)
	{
		if (srcPathData[i] != dstPathData[i])
		{
			same = FALSE;
			break;
		}
	}
	if (!same) return FALSE;

	pathString = String(dstPath, Region(srcPathLen, UINF));
	return TRUE;
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
	if (pathesInfo.workPath.isEmpty()) return Path();
	else return Path(pathesInfo.workPath);
}

Path Path::homePath()
{
	if (pathesInfo.homePath.isEmpty()) return Path();
	else return Path(pathesInfo.homePath);
}

Path Path::spadasFilesPath()
{
	if (pathesInfo.spadasFilesPath.isEmpty()) return Path();
	else return Path(pathesInfo.spadasFilesPath);
}

void Path::setWorkPath(String pathString)
{
	String separator = Path::separator();
	if (!pathString.isEmpty() && pathString.endsWith(separator)) pathesInfo.setWorkPath(pathString);
}
