
#include "file.h"

namespace file_internal
{
	String dot = ".";
	String doubleDot = "..";

	Bool/* isValid */ parsePathString(String pathString, Bool absoluteLimited, Array<StringSpan>& components, Bool& isFolder)
	{
		if (pathString.isEmpty()) return FALSE;

		UInt rootLength = 0;
		Bool isAbsolute = FALSE;
		if (!isPathStringValid(pathString, rootLength, isAbsolute)) return FALSE;

		UInt curIndex = 0;
		ArrayX<StringSpan> buf;
		if (isAbsolute)
		{
			curIndex = rootLength + 1;
			if (rootLength > 0) buf.append(StringSpan(pathString, 0, rootLength));
			else buf.append(StringSpan());
		}
		else
		{
			if (absoluteLimited) return FALSE;
			buf.append(pathsInfo.workPathComponents);
		}

		UInt len = pathString.length();
		const Byte* bytes = pathString.bytes();

		UInt bufValidSize = buf.size();
		for (UInt i = curIndex; i < len; i++)
		{
			if (bytes[i] != '/' && bytes[i] != '\\') continue;

			if (i == curIndex)
			{
				curIndex = i + 1;
				continue;
			}

			StringSpan span(pathString, curIndex, i - curIndex);
			curIndex = i + 1;
			
			if (span == dot) continue;
			else if (span == doubleDot)
			{
				if (bufValidSize < 2) return FALSE;
				else bufValidSize--;
			}
			else
			{
				buf[bufValidSize++] = span;
			}
		}

		isFolder = curIndex == len;
		if (!isFolder)
		{
			buf[bufValidSize++] = StringSpan(pathString, curIndex, len - curIndex);
		}

		components = buf.toArray(Region(0, bufValidSize));
		return TRUE;
	}

	void addFolderContents(ArrayX<Path>& contents, Path folderPath, String folderString)
	{
		Char separator = getSeparatorChar();
		for (auto e = folderGetContents(folderString).firstElem(); e.valid(); ++e)
		{
			String& contentString = e.value();
			Path contentPath = folderPath.childPath(contentString);
			contents.append(contentPath);
			if (contentString.bytes()[contentString.length() - 1] == (Byte)separator) // folder
			{
				addFolderContents(contents, contentPath, folderString + contentString);
			}
		}
	}

	PathsInfo::PathsInfo()
	{
		Bool dummy = FALSE;

		String executableFolderPath = getExecutableFolderPathString();
		if (!executableFolderPath.isEmpty()) parsePathString(executableFolderPath, TRUE, executableFolderPathComponents, dummy);
		
		String workPath = getWorkPathString();
		if (!workPath.isEmpty()) parsePathString(workPath, TRUE, workPathComponents, dummy);
		
		String homePath = getHomePathString();
		if (!homePath.isEmpty()) parsePathString(homePath, TRUE, homePathComponents, dummy);
		
		String spadasFilesPath = getSpadasFilesPathString();
		if (!spadasFilesPath.isEmpty()) parsePathString(spadasFilesPath, TRUE, spadasFilesPathComponents, dummy);
		if (!spadasFilesPath.isEmpty() && !folderExist(spadasFilesPath)) folderCreate(spadasFilesPath);
	}
	void PathsInfo::setWorkPath(String path)
	{
		Array<StringSpan> components;
		Bool isFolder = FALSE;
		if (parsePathString(path, TRUE, components, isFolder) && isFolder && folderExist(path)) workPathComponents = components;
	}
	void PathsInfo::setExecutableFolderPath(String path)
	{
		Array<StringSpan> components;
		Bool isFolder = FALSE;
		if (parsePathString(path, TRUE, components, isFolder) && isFolder && folderExist(path)) executableFolderPathComponents = components;
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
		Array<StringSpan> components; // 首个为root
		PathVars()
		{
			isFolder = FALSE;
		}
		String toString() override
		{
			Char separator = file_internal::getSeparatorChar();
			String out = String::createWithSize(256);
			for (auto e = components.firstElem(); e.valid(); ++e)
			{
				if (e.index() > 0) out += separator;
				out += e.value();
			}
			if (isFolder) out += separator;
			return out;
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
	if (pathString.isEmpty()) return;

	Bool isFolder = FALSE;
	Array<StringSpan> components;
	if (parsePathString(pathString, FALSE, components, isFolder))
	{
		PathVars *vars = new PathVars();
		vars->isFolder = isFolder;
		vars->components = components;
		setVars(vars, TRUE);
	}
	else
	{
		SPADAS_ERROR_MSG(SS"Invalid path: " + pathString);
	}
}

String Path::name()
{
	if (!vars || vars->components.size() < 2) return String();
	
	StringSpan& fileFullName = vars->components.last();
	Array<UInt> dotIndices = fileFullName.search(dot);
	
	if (dotIndices.isEmpty()) return fileFullName.clone();
	else return fileFullName.sub(0, dotIndices[dotIndices.size() - 1]).clone();
}

String Path::extension()
{
	if (!vars || vars->components.size() < 2) return String();
	
	StringSpan& fileFullName = vars->components.last();
	Array<UInt> dotIndices = fileFullName.search(dot);
	
	if (dotIndices.isEmpty()) return String();
	else return fileFullName.sub(dotIndices[dotIndices.size() - 1]).clone();
}

String Path::fullName()
{
	SPADAS_ERROR_RETURNVAL(!vars || vars->components.size() < 2, String());
	return vars->components.last().clone();
}

String Path::fullPath()
{
	SPADAS_ERROR_RETURNVAL(!vars, String());
	return vars->toString();
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
	String fullPathString = vars->toString();
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

void Path::remove()
{
	SPADAS_ERROR_RETURN(!vars);

	String fullPathString = vars->toString();
	if (vars->isFolder)
	{
		if (!folderExist(fullPathString)) return;

		for (auto e = folderContents(TRUE).lastElem(); e.valid(); --e)
		{
			String contentPathString = e->fullPath();
			if (e->isFolder()) folderRemove(contentPathString);
			else fileRemove(contentPathString);
		}

		folderRemove(fullPathString);
	}
	else
	{
		if (fileExist(fullPathString)) fileRemove(fullPathString);
	}
}

void file_internal::moveOrCopy(Path srcPath, Path dstPath, Bool isCopy, Bool mergeDst)
{
	SPADAS_ERROR_RETURN(srcPath.isFolder() != dstPath.isFolder());

	String srcPathFullString = srcPath.fullPath(), dstPathFullString = dstPath.fullPath();
	if (srcPath == dstPath) return;

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
				String childPath = srcContentString.sub(srcPathLen).clone();
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
	
	Pointer filePtr = fileOpen(vars->toString(), FALSE);
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

	String parentFolderString = parentFolder.fullPath();
	SPADAS_ERROR_RETURN(fileExist(parentFolderString));

	if (!folderExist(parentFolderString)) parentFolder.folderMake();
	
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
	
	String folderPath = fullPath();
	SPADAS_ERROR_RETURN(fileExist(folderPath));
	if (folderExist(folderPath)) return;
	
	Path parent = parentFolder();
	if (parent.isValid()) parent.folderMake();
	
	folderCreate(folderPath);
}

Path Path::childFile(String childName)
{
	SPADAS_ERROR_RETURNVAL(!vars || !vars->isFolder, Path());

	UInt len = childName.length();
	const Byte* bytes = childName.bytes();
	SPADAS_ERROR_RETURNVAL(len == 0, Path());
	SPADAS_ERROR_RETURNVAL(len == 1 && bytes[0] == (Byte)'.', Path());
	SPADAS_ERROR_RETURNVAL(len == 2 && bytes[0] == (Byte)'.' && bytes[1] == (Byte)'.', Path());
	SPADAS_ERROR_RETURNVAL(bytes[len - 1] == (Byte)getSeparatorChar(), Path());

	PathVars *newVars = new PathVars();
	newVars->isFolder = FALSE;
	newVars->components = Array<StringSpan>::createUninitialized(vars->components.size() + 1);
	for (UInt i = 0; i < vars->components.size(); i++) newVars->components.initialize(i, vars->components[i]);
	newVars->components.initialize(vars->components.size(), childName.sub(0));

	Path path;
	path.setVars(newVars, TRUE);
    return path;
}

Path Path::childFolder(String childName)
{
	SPADAS_ERROR_RETURNVAL(!vars || !vars->isFolder, Path());

	UInt len = childName.length();
	const Byte* bytes = childName.bytes();
	SPADAS_ERROR_RETURNVAL(len == 0, Path());
	SPADAS_ERROR_RETURNVAL(len == 1 && bytes[0] == (Byte)'.', Path());
	SPADAS_ERROR_RETURNVAL(len == 2 && bytes[0] == (Byte)'.' && bytes[1] == (Byte)'.', Path());
	SPADAS_ERROR_RETURNVAL(bytes[len - 1] == (Byte)getSeparatorChar(), Path());
	
	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = Array<StringSpan>::createUninitialized(vars->components.size() + 1);
	for (UInt i = 0; i < vars->components.size(); i++) newVars->components.initialize(i, vars->components[i]);
	newVars->components.initialize(vars->components.size(), childName.sub(0));

	Path path;
	path.setVars(newVars, TRUE);
    return path;
}

Path Path::childPath(String pathString)
{
	SPADAS_ERROR_RETURNVAL(!vars || !vars->isFolder, Path());

	pathString = fullPath() + pathString;

	Bool isFolder = FALSE;
	Array<StringSpan> components;
	if (parsePathString(pathString, TRUE, components, isFolder))
	{
		PathVars *vars = new PathVars();
		vars->isFolder = isFolder;
		vars->components = components;

		Path path;
		path.setVars(vars, TRUE);
		return path;
	}
	else
	{
		SPADAS_ERROR_MSG(SS"Invalid path: " + pathString);
		return Path();
	}
}

Bool Path::contain(Path path)
{
	String dummy;
	return contain(path, dummy);
}

Bool Path::contain(Path path, String& pathString)
{
	SPADAS_ERROR_RETURNVAL(!vars || !vars->isFolder, FALSE);
	SPADAS_ERROR_RETURNVAL(!path.vars, FALSE);

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
		pathString = dstPath.sub(srcPathLen).clone();
		return TRUE;
	}
}

Path Path::parentFolder()
{
	SPADAS_ERROR_RETURNVAL(!vars, Path());
    
	if (vars->components.size() < 2) return Path();

	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = Array<StringSpan>::createUninitialized(vars->components.size() - 1);
	for (UInt i = 0; i < newVars->components.size(); i++) newVars->components.initialize(i, vars->components[i]);
    
    Path path;
	path.setVars(newVars, TRUE);
    return path;
}

Array<Path> Path::folderContents(Bool includeAllLevels)
{
	SPADAS_ERROR_RETURNVAL(!vars || !vars->isFolder, Array<Path>());

	String targetFolder = fullPath();
	if (!folderExist(targetFolder)) return Array<Path>();

	if (includeAllLevels)
	{
		ArrayX<Path> buf(256, Path());
		addFolderContents(buf, *this, targetFolder);
		return buf.toArray();
	}
	else
	{
		Array<String> contents = folderGetContents(targetFolder);
		Array<Path> out = Array<Path>::createUninitialized(contents.size());
		for (UInt i = 0; i < out.size(); i++)
		{
			out.initialize(i, childPath(contents[i]));
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
	if (pathsInfo.workPathComponents.isEmpty()) return Path();

	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = pathsInfo.workPathComponents.clone();
    
    Path path;
	path.setVars(newVars, TRUE);
    return path;
}

Path Path::executableFolderPath()
{
	if (pathsInfo.executableFolderPathComponents.isEmpty()) return Path();

	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = pathsInfo.executableFolderPathComponents.clone();
    
    Path path;
	path.setVars(newVars, TRUE);
    return path;
}

Path Path::homePath()
{
	if (pathsInfo.homePathComponents.isEmpty()) return Path();

	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = pathsInfo.homePathComponents.clone();
    
    Path path;
	path.setVars(newVars, TRUE);
    return path;
}

Path Path::spadasFilesPath()
{
	if (pathsInfo.spadasFilesPathComponents.isEmpty()) return Path();

	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = pathsInfo.spadasFilesPathComponents.clone();
    
    Path path;
	path.setVars(newVars, TRUE);
    return path;
}

void Path::setWorkPath(String pathString)
{
	pathsInfo.setWorkPath(pathString);
}

void Path::setExecutableFolderPath(String pathString)
{
	pathsInfo.setExecutableFolderPath(pathString);
}