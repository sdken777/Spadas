
#include "spadas.h"

#include "file.h"

namespace spadas
{
	const Double FILE_FLUSH_PERIOD = 1000; // [ms]
	
	enum class FileMode
	{
		OpenBinary,
		OpenTextNormal,
		OpenTextUtf8,
		CreateBinary,
		CreateText,
	};

	class FileVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(File, Vars)

		Path path;
		Pointer file;
		FileMode mode;
		ULong size;
		ULong pos;
		Timer timer;
		Array<Char> scanBuffer;
		FileVars(Path path0, Pointer file0, FileMode mode0, ULong size0, ULong pos0) : path(path0), file(file0), mode(mode0), size(size0), pos(pos0)
        {
        }
        ~FileVars()
        {
			if (file)
			{
				if (isOutputMode()) file_internal::fileFlush(file);
				file_internal::fileClose(file);
			}
        }
		Bool isOutputMode()
		{
			return mode == FileMode::CreateBinary || mode == FileMode::CreateText;
		}
		Bool isInputTextMode()
		{
			return mode == FileMode::OpenTextNormal || mode == FileMode::OpenTextUtf8;
		}
	};
}

using namespace spadas;
using namespace spadas_internal;
using namespace file_internal;

const String spadas::File::TypeName = "spadas.File";

File::File()
{
}

File File::openBinary(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull() || filePath.isFolder() || !filePath.exist(), File());

	Pointer filePtr = fileOpen(filePath.fullPath(), FALSE);
	SPADAS_ERROR_RETURNVAL(!filePtr, File());

	ULong fileSize = fileGetSize(filePtr);
	fileSeek(filePtr, 0);

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::OpenBinary, fileSize, 0), TRUE);
	return file;
}

File File::openText(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull() || filePath.isFolder() || !filePath.exist(), File());

	Pointer filePtr = fileOpen(filePath.fullPath(), FALSE);
	SPADAS_ERROR_RETURNVAL(!filePtr, File());

	ULong fileSize = fileGetSize(filePtr);
	fileSeek(filePtr, 0);

	Bool isUtf8 = FALSE;
	ULong pos = 0;
	if (fileSize >= 3)
	{
		Binary maybeBom = fileInput(filePtr, 3, fileSize, 0);
		if (maybeBom.size() == 3 && maybeBom[0] == 0xEF && maybeBom[1] == 0xBB && maybeBom[2] == 0xBF)
		{
			isUtf8 = TRUE;
			pos = 3;
		}
		else fileSeek(filePtr, 0);
	}

	File file;
	file.setVars(new FileVars(filePath, filePtr, isUtf8 ? FileMode::OpenTextUtf8 : FileMode::OpenTextNormal, fileSize, pos), TRUE);
	return file;
}

File File::createBinary(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull() || filePath.isFolder(), File());

	String fileFullPath = filePath.fullPath();
	if (filePath.exist())
	{
		fileRemove(fileFullPath);
	}

	Path parent = filePath.parentFolder();
	if (!parent.exist()) parent.folderMake();

	Pointer filePtr = fileOpen(fileFullPath, TRUE);
	SPADAS_ERROR_RETURNVAL(!filePtr, File());

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::CreateBinary, 0, 0), TRUE);
	return file;
}

File File::createText(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull() || filePath.isFolder(), File());

	String fileFullPath = filePath.fullPath();
	if (filePath.exist())
	{
		fileRemove(fileFullPath);
	}

	Path parent = filePath.parentFolder();
	if (!parent.exist()) parent.folderMake();

	Pointer filePtr = fileOpen(fileFullPath, TRUE);
	SPADAS_ERROR_RETURNVAL(!filePtr, File());

	fileOutput(filePtr, Binary::create(3, 0xEF, 0xBB, 0xBF));

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::CreateText, 3, 3), TRUE);
	return file;
}

void File::close()
{
	if (!isClosed())
	{
		if (vars->isOutputMode()) fileFlush(vars->file);
		fileClose(vars->file);
		vars->file = NULL;
	}
}

Bool File::isClosed()
{
	return !vars || !vars->file;
}

ULong File::position()
{
	if (isClosed()) return ULINF;
	else return vars->pos;
}

ULong File::size()
{
	if (isClosed()) return ULINF;
	else return vars->size;
}

Path File::path()
{
	return vars ? vars->path : Path();
}
	
void File::print(String text)
{
	SPADAS_ERROR_RETURN(isClosed() || vars->mode != FileMode::CreateText);

	filePrint(vars->file, text);
	
	ULong curPos = filePos(vars->file);
	vars->pos = vars->size = curPos;
	
	if (vars->timer.check() > FILE_FLUSH_PERIOD)
	{
		vars->timer.start();
		fileFlush(vars->file);
	}
}

void File::output(Binary data)
{
	SPADAS_ERROR_RETURN(isClosed() || !vars->isOutputMode());

	fileOutput(vars->file, data);

	vars->size += data.size();
	vars->pos = vars->size;
	
	if (vars->timer.check() > FILE_FLUSH_PERIOD)
	{
		vars->timer.start();
		fileFlush(vars->file);
	}
}

void File::flush()
{
	SPADAS_ERROR_RETURN(isClosed() || !vars->isOutputMode());

	fileFlush(vars->file);
}

String File::scan()
{
	SPADAS_ERROR_RETURNVAL(isClosed() || !vars->isInputTextMode(), String());

	if (vars->scanBuffer.isEmpty()) vars->scanBuffer = Array<Char>(SCAN_SIZE);

	String output = fileScan(vars->file, vars->scanBuffer.data(), vars->mode == FileMode::OpenTextUtf8);
	vars->pos = filePos(vars->file);
	return output;
}

Binary File::input(UInt size)
{
	if (size == 0) return Binary();
	
	SPADAS_ERROR_RETURNVAL(isClosed() || vars->isOutputMode(), Binary());

	Binary output = fileInput(vars->file, size, vars->size, vars->pos);
	vars->pos += output.size();
	return output;
}

ULong File::seek(ULong pos)
{
	SPADAS_ERROR_RETURNVAL(isClosed() || vars->isOutputMode(), ULINF);

	if (vars->mode == FileMode::OpenTextUtf8) pos = math::max(3ull, pos);

	vars->pos = fileSeek(vars->file, pos);
	return vars->pos;
}

Bool File::endOfFile()
{
	SPADAS_ERROR_RETURNVAL(isClosed() || vars->isOutputMode(), TRUE);

	return vars->pos >= vars->size;
}
