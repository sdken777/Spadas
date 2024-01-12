
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
			if (pathsInfo.entryFolderPathComponents.isEmpty()) return FALSE;
			buf.append(pathsInfo.entryFolderPathComponents);
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

	String componentsToString(Array<StringSpan>& components, Bool isFolder)
	{
		Char separator = getSeparatorChar();
		String out = String::createWithSize(256);
		for (auto e = components.firstElem(); e.valid(); ++e)
		{
			if (e.index() > 0) out += separator;
			out += e.value();
		}
		if (isFolder) out += separator;
		return out;
	}

	PathsInfo::PathsInfo()
	{
		Bool dummy = FALSE;

		String entryFolderPath = getEntryFolderPathString(FALSE);
		if (!entryFolderPath.isEmpty() && parsePathString(entryFolderPath, TRUE, entryFolderPathComponents, dummy))
		{
			entryFolderPathString = componentsToString(entryFolderPathComponents, TRUE);
		}

		String appFolderPath = getEntryFolderPathString(TRUE);
		if (!appFolderPath.isEmpty() && parsePathString(appFolderPath, TRUE, appFolderPathComponents, dummy))
		{
			appFolderPathString = componentsToString(appFolderPathComponents, TRUE);
		}

		String homePath = getHomePathString();
		if (!homePath.isEmpty() && parsePathString(homePath, TRUE, homePathComponents, dummy))
		{
			homePathString = componentsToString(homePathComponents, TRUE);
		}

		String spadasFilesPath = homePath.isEmpty() ? String() : (homePath + "SpadasFiles" + Path::separator()).toString();
		if (!spadasFilesPath.isEmpty() && parsePathString(spadasFilesPath, TRUE, spadasFilesPathComponents, dummy))
		{
			spadasFilesPathString = componentsToString(spadasFilesPathComponents, TRUE);
		}
		if (!spadasFilesPath.isEmpty() && !folderExist(spadasFilesPath)) folderCreate(spadasFilesPath);
	}

	void PathsInfo::setEntryFolderPath(Array<StringSpan> components, String pathString)
	{
		entryFolderPathComponents = components;
		entryFolderPathString = pathString;

		Bool inAppBundle = FALSE;
		if (system::getEnv() == Environment::Value::MacOS)
		{
			if (components.size() >= 4 && components.last() == "MacOS" && components[components.size() - 2] == "Contents" && components[components.size() - 3].endsWith(".app"))
			{
				appFolderPathComponents = components.span(0, components.size() - 3).clone();
				appFolderPathString = componentsToString(appFolderPathComponents, TRUE);
				inAppBundle = TRUE;
			}
		}
		if (!inAppBundle)
		{
			appFolderPathComponents = components;
			appFolderPathString = pathString;
		}
	}

	PathsInfo pathsInfo;
}

namespace spadas
{
	class PathVars : public Vars
	{
    public:
		SPADAS_VARS(Path, Vars)

		Bool isFolder;
		Array<StringSpan> components; // 首个为root
		String pathString; // 仅用于set

		PathVars() : isFolder(FALSE)
		{}
		PathVars(Array<StringSpan>& dirComponent, String dirPathString, StringSpan childFullName, Bool isFolder) : isFolder(isFolder)
		{
			components = Array<StringSpan>::createUninitialized(dirComponent.size() + 1);
			for (auto e = dirComponent.firstElem(); e.valid(); ++e) components.initialize(e.index(), e.value());
			components.initialize(dirComponent.size(), childFullName);
			if (!dirPathString.isEmpty())
			{
				pathString = String::createWithSize(dirPathString.length() + childFullName.length() + 1);
				pathString += dirPathString;
				pathString += childFullName;
				if (isFolder) pathString += file_internal::getSeparatorChar();
			}
		}
		String getPathString()
		{
			if (pathString.isEmpty()) pathString = file_internal::componentsToString(components, isFolder);
			return pathString;
		}
		String toString() override
		{
			return getPathString().clone();
		}
	};
}

using namespace spadas;
using namespace spadas::console;
using namespace file_internal;

Path::Path()
{
}

Path::Path(const Char pathStringChars[])
{
	String pathString(pathStringChars);
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
		SPADAS_ERROR_MSG("Invalid path: " cat pathString);
	}
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
		SPADAS_ERROR_MSG("Invalid path: " cat pathString);
	}
}

String Path::name()
{
	if (!vars || vars->components.size() < 2) return String();
	
	StringSpan& fileFullName = vars->components.last();
	Array<UInt> dotIndices = fileFullName.search(dot);
	
	if (dotIndices.isEmpty()) return fileFullName.clone();
	else return fileFullName.span(0, dotIndices[dotIndices.size() - 1]).clone();
}

String Path::extension()
{
	if (!vars || vars->components.size() < 2) return String();
	
	StringSpan& fileFullName = vars->components.last();
	Array<UInt> dotIndices = fileFullName.search(dot);
	
	if (dotIndices.isEmpty()) return String();
	else return fileFullName.span(dotIndices[dotIndices.size() - 1]).clone();
}

String Path::fullName()
{
	SPADAS_ERROR_RETURNVAL(!vars || vars->components.size() < 2, String());
	return vars->components.last().clone();
}

String Path::fullPath()
{
	SPADAS_ERROR_RETURNVAL(!vars, String());
	return vars->getPathString().clone();
}

Bool Path::operator ==(Path path)
{
	if (vars)
	{
		if (path.vars)
		{
			if (vars->isFolder != path.vars->isFolder) return FALSE;
			if (vars->components.size() != path.vars->components.size()) return FALSE;
			StringSpan *srcSpans = vars->components.data();
			StringSpan *targetSpans = path.vars->components.data();
			UInt nComponents = vars->components.size();
			for (UInt i = 0; i < nComponents; i++)
			{
				if (srcSpans[i] != targetSpans[i]) return FALSE;
			}
			return TRUE;
		}
		else return FALSE;
	}
	else return path.vars == NULL;
}

Bool Path::operator !=(Path path)
{
	if (vars)
	{
		if (path.vars)
		{
			if (vars->isFolder != path.vars->isFolder) return TRUE;
			if (vars->components.size() != path.vars->components.size()) return TRUE;
			StringSpan *srcSpans = vars->components.data();
			StringSpan *targetSpans = path.vars->components.data();
			UInt nComponents = vars->components.size();
			for (UInt i = 0; i < nComponents; i++)
			{
				if (srcSpans[i] != targetSpans[i]) return TRUE;
			}
			return FALSE;
		}
		else return TRUE;
	}
	else return path.vars != NULL;
}

Word Path::getHash()
{
	if (!vars) return 0;
	else return vars->components.last().getHash();
}

Bool Path::exist()
{
	SPADAS_ERROR_RETURNVAL(!vars, FALSE);
	if (vars->isFolder) return folderExist(vars->getPathString());
	else return fileExist(vars->getPathString());
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

	if (vars->isFolder)
	{
		if (!folderExist(vars->getPathString())) return;

		for (auto e = folderContents(TRUE).lastElem(); e.valid(); --e)
		{
			if (!e->vars) continue;
			if (e->vars->isFolder) folderRemove(e->vars->getPathString());
			else fileRemove(e->vars->getPathString());
		}

		folderRemove(vars->getPathString());
	}
	else
	{
		if (fileExist(vars->getPathString())) fileRemove(vars->getPathString());
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
				String childPath = srcContentString.span(srcPathLen).clone();
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
	SPADAS_ERROR_RETURNVAL(vars->isFolder, 0);
	SPADAS_ERROR_RETURNVAL(!exist(), 0);
	
	Pointer filePtr = fileOpen(vars->getPathString(), FALSE, FALSE);
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
	SPADAS_ERROR_RETURN(!parentFolder.vars);
	SPADAS_ERROR_RETURN(fileExist(parentFolder.vars->getPathString()));

	if (!folderExist(parentFolder.vars->getPathString())) parentFolder.folderMake();
	
	Pointer file = fileOpen(vars->getPathString(), TRUE, FALSE);
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
	
	SPADAS_ERROR_RETURN(fileExist(vars->getPathString()));
	if (folderExist(vars->getPathString())) return;
	
	Path parent = parentFolder();
	if (parent.isValid()) parent.folderMake();
	
	folderCreate(vars->getPathString());
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

	Path path;
	path.setVars(new PathVars(vars->components, String(), StringSpan(childName, 0, childName.length()), FALSE), TRUE);
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

	Path path;
	path.setVars(new PathVars(vars->components, String(), StringSpan(childName, 0, childName.length()), TRUE), TRUE);
    return path;
}

Path Path::childPath(String pathString)
{
	SPADAS_ERROR_RETURNVAL(!vars || !vars->isFolder, Path());

	pathString = vars->getPathString() + pathString;

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
		SPADAS_ERROR_MSG("Invalid path: " cat pathString);
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

	String srcPathString = vars->getPathString();
	String dstPathString = path.vars->getPathString();
	UInt srcPathLen = srcPathString.length();
	UInt dstPathLen = dstPathString.length();

	if (dstPathLen < srcPathLen) return FALSE;
	if (!dstPathString.startsWith(srcPathString)) return FALSE;

	if (dstPathLen == srcPathLen)
	{
		pathString = String();
		return TRUE;
	}
	else
	{
		pathString = dstPathString.span(srcPathLen).clone();
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

void Path::addFolderContents(ArrayX<Path>& contents, String folderPathString, Array<StringSpan>& folderComponents)
{
	for (auto e = folderGetContents(folderPathString).firstElem(); e.valid(); ++e)
	{
		String& content = e.value();
		Char lastChar = (Char)content.bytes()[content.length() - 1];
		Bool contentIsFolder = lastChar == '/' || lastChar == '\\';
		StringSpan contentFullName(content, 0, contentIsFolder ? (content.length() - 1) : content.length());

		Path path;
		path.setVars(new PathVars(folderComponents, folderPathString, contentFullName, contentIsFolder), TRUE);
		contents.append(path);

		if (contentIsFolder)
		{
			addFolderContents(contents, path.vars->getPathString(), path.vars->components);
		}
	}
}

Array<Path> Path::folderContents(Bool includeAllLevels)
{
	SPADAS_ERROR_RETURNVAL(!vars || !vars->isFolder, Array<Path>());

	if (!folderExist(vars->getPathString())) return Array<Path>();

	if (includeAllLevels)
	{
		ArrayX<Path> buf(256, Path());
		addFolderContents(buf, vars->getPathString(), vars->components);
		return buf.toArray();
	}
	else
	{
		Array<String> contents = folderGetContents(vars->getPathString());
		Array<Path> out = Array<Path>::createUninitialized(contents.size());
		for (auto e = contents.firstElem(); e.valid(); ++e)
		{
			String& content = e.value();
			Char lastChar = (Char)content.bytes()[content.length() - 1];
			Bool contentIsFolder = lastChar == '/' || lastChar == '\\';
			StringSpan contentFullName(content, 0, contentIsFolder ? (content.length() - 1) : content.length());

			Path path;
			path.setVars(new PathVars(vars->components, vars->getPathString(), contentFullName, contentIsFolder), TRUE);
			out.initialize(e.index(), path);
		}
		return out;
	}
}

String Path::separator()
{
	return getSeparatorChar();
}

Path Path::entryFolderPath(Bool forApp)
{
	Array<StringSpan> components = forApp ? pathsInfo.appFolderPathComponents : pathsInfo.entryFolderPathComponents;
	String pathString = forApp ? pathsInfo.appFolderPathString : pathsInfo.entryFolderPathString;

	if (components.isEmpty()) return Path();

	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = components;
	newVars->pathString = pathString;
    
    Path path;
	path.setVars(newVars, TRUE);
    return path;
}

Path Path::currentPath()
{
	return Path(getCurrentPathString());
}

Path Path::homePath()
{
	if (pathsInfo.homePathComponents.isEmpty()) return Path();

	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = pathsInfo.homePathComponents;
	newVars->pathString = pathsInfo.homePathString;
    
    Path path;
	path.setVars(newVars, TRUE);
    return path;
}

Path Path::spadasFilesPath()
{
	if (pathsInfo.spadasFilesPathComponents.isEmpty()) return Path();

	PathVars *newVars = new PathVars();
	newVars->isFolder = TRUE;
	newVars->components = pathsInfo.spadasFilesPathComponents;
	newVars->pathString = pathsInfo.spadasFilesPathString;
    
    Path path;
	path.setVars(newVars, TRUE);
    return path;
}

void Path::setCurrentPath(Path path)
{
	SPADAS_ERROR_RETURN(path.isNull());
	SPADAS_ERROR_RETURN(!path.isFolder());
	SPADAS_ERROR_RETURN(!path.exist());

	setCurrentPathString(path.fullPath());
}

void Path::setEntryFolderPath(Path path)
{
	SPADAS_ERROR_RETURN(path.isNull());
	SPADAS_ERROR_RETURN(!path.isFolder());
	SPADAS_ERROR_RETURN(!path.exist());

	pathsInfo.setEntryFolderPath(path.vars->components.clone(), path.fullPath());
}