
#include "file.h"

namespace spadas
{
	const Double FILE_FLUSH_PERIOD = 1000; // [ms]
	const UInt ENCRYPTED_SCAN_SIZE = 256; // bytes
	
	enum class FileMode
	{
		InputBinary,
		InputTextDefault,
		InputTextUtf8,
		OutputBinary,
		OutputTextDefault,
		OutputTextUtf8,
		InputEncrypted,
		OutputEncrypted,
	};

	class CompatibleRandom
	{    
	public:
		CompatibleRandom(Int seed) : inext(0), inextp(21), seedArray(56)
		{
			Int num = (seed == NINF) ? INF : abs(seed);
			seedArray[55] = num;
			Int num3 = 1;
			for (Int i = 1; i < 55; i++)
			{
				Int num4 = 21 * i % 55;
				seedArray[num4] = num3;
				num3 = num - num3;
				if (num3 < 0) num3 += INF;
				num = seedArray[num4];
			}
			for (Int j = 1; j < 5; j++)
			{
				for (Int k = 1; k < 56; k++)
				{
					seedArray[k] -= seedArray[1 + (k + 30) % 55];
					if (seedArray[k] < 0) seedArray[k] += INF;
				}
			}
		}
		
		Int next()
		{
			Int num = inext;
			Int num2 = inextp;
			if (++num >= 56) num = 1;
			if (++num2 >= 56) num2 = 1;
			Int num3 = seedArray[num] - seedArray[num2];
			if (num3 == INF) num3--;
			if (num3 < 0) num3 += INF;
			seedArray[num] = num3;
			inext = num;
			inextp = num2;
			return num3;
		}
		
		void nextBytes(Binary buffer)
		{
			for (UInt i = 0; i < buffer.size(); i++)
			{
				buffer[i] = (Byte)(next() & 0xFF);
			}
		}

	private:
		Int inext;
		Int inextp;
		Array<Int> seedArray;
	};

	class EncryptionContext
	{
	public:
		static EncryptionContext *createForOpenFile(Pointer filePtr, String password, String& errorMsg)
		{
			Binary header = file_internal::fileInput(filePtr, 16, 16, 0);
			if (header.size() < 16)
			{
				errorMsg = "Invalid SEF file size";
			}
			if (header[0] != 'S' || header[1] != 'E' || header[2] != 'F' || header[3] != '1')
			{
				errorMsg = "Invalid SEF magic number";
				return NULL;
			}

			Int rawSeed = *(Int*)&header[4];
			
			EncryptionContext *encCtx = new EncryptionContext();
			encCtx->initializeMasks(rawSeed, password);

			for (UInt i = 0; i < 8; i++)
			{
				Byte val = encCtx->mask1[i] ^ encCtx->mask2[i] ^ encCtx->mask3[i] ^ encCtx->mask4[i];
				if (header[8 + i] != val)
				{
					errorMsg = "Wrong password for SEF";
					delete encCtx;
					return NULL;
				}
			}

			encCtx->curIndex1 = 8;
			encCtx->curIndex2 = 8;
			encCtx->curIndex3 = 8;
			encCtx->curIndex4 = 8;
			return encCtx;
		}
		static EncryptionContext *createForNewFile(Pointer filePtr, String password)
		{
			Binary header = Binary(16);
			header[0] = 'S';
			header[1] = 'E';
			header[2] = 'F';
			header[3] = '1';
			
			Int rawSeed = (Int)(math::randomD(1)[0] * INF);
			*(Int*)&header[4] = rawSeed;

			EncryptionContext *encCtx = new EncryptionContext();
			encCtx->initializeMasks(rawSeed, password);

			for (UInt i = 0; i < 8; i++)
			{
				header[8 + i] = encCtx->mask1[i] ^ encCtx->mask2[i] ^ encCtx->mask3[i] ^ encCtx->mask4[i];
			}
			file_internal::fileOutput(filePtr, header);

			encCtx->curIndex1 = 8;
			encCtx->curIndex2 = 8;
			encCtx->curIndex3 = 8;
			encCtx->curIndex4 = 8;
			return encCtx;
		}
		void moveTo(ULong pos)
		{
			SPADAS_ERROR_RETURN(pos < 8);
			pos -= 8;
			curIndex1 = pos % 479;
			curIndex2 = pos % 487;
			curIndex3 = pos % 491;
			curIndex4 = pos % 499;
		}
		Byte next(Byte src)
		{
			Byte dst = src ^ mask1[curIndex1] ^ mask2[curIndex2] ^ mask3[curIndex3] ^ mask4[curIndex4];
			if (++curIndex1 >= 479) curIndex1 = 0;
			if (++curIndex2 >= 487) curIndex2 = 0;
			if (++curIndex3 >= 491) curIndex3 = 0;
			if (++curIndex4 >= 499) curIndex4 = 0;
			return dst;
		}
	private:
		EncryptionContext() : curIndex1(0), curIndex2(0), curIndex3(0), curIndex4(0)
		{
			utility::memorySet(0, mask1, 479);
			utility::memorySet(0, mask2, 487);
			utility::memorySet(0, mask3, 491);
			utility::memorySet(0, mask4, 499);
		}
		void initializeMasks(Int rawSeed, String password)
		{
			Binary sha1 = ((String)rawSeed + password + rawSeed).toString().toBinary().toSHA1();
			Int seed1 = *(Int*)&sha1[11];
			sha1 = ((String)rawSeed + password).toString().toBinary().toSHA1();
			Int seed2 = *(Int*)&sha1[7];
			sha1 = ((String)password + rawSeed).toString().toBinary().toSHA1();
			Int seed3 = *(Int*)&sha1[13];
			CompatibleRandom random1(seed1);
			CompatibleRandom random2(seed2);
			CompatibleRandom random3(seed3);

			Binary mask1Buffer = Binary(479);
			Binary mask2Buffer = Binary(487);
			Binary mask3Buffer = Binary(491);
			Binary mask4Buffer = Binary(499);

			random1.nextBytes(mask1Buffer);
			random1.nextBytes(mask2Buffer);
			random1.nextBytes(mask3Buffer);
			random1.nextBytes(mask4Buffer);

			for (UInt i = 0; i < 479; i++) mask1[i] ^= mask1Buffer[i];
			for (UInt i = 0; i < 487; i++) mask2[i] ^= mask2Buffer[i];
			for (UInt i = 0; i < 491; i++) mask3[i] ^= mask3Buffer[i];
			for (UInt i = 0; i < 499; i++) mask4[i] ^= mask4Buffer[i];
			
			random2.nextBytes(mask1Buffer);
			random2.nextBytes(mask2Buffer);
			random2.nextBytes(mask3Buffer);
			random2.nextBytes(mask4Buffer);

			for (UInt i = 0; i < 479; i++) mask1[i] ^= mask1Buffer[i];
			for (UInt i = 0; i < 487; i++) mask2[i] ^= mask2Buffer[i];
			for (UInt i = 0; i < 491; i++) mask3[i] ^= mask3Buffer[i];
			for (UInt i = 0; i < 499; i++) mask4[i] ^= mask4Buffer[i];

			random3.nextBytes(mask1Buffer);
			random3.nextBytes(mask2Buffer);
			random3.nextBytes(mask3Buffer);
			random3.nextBytes(mask4Buffer);

			for (UInt i = 0; i < 479; i++) mask1[i] ^= mask1Buffer[i];
			for (UInt i = 0; i < 487; i++) mask2[i] ^= mask2Buffer[i];
			for (UInt i = 0; i < 491; i++) mask3[i] ^= mask3Buffer[i];
			for (UInt i = 0; i < 499; i++) mask4[i] ^= mask4Buffer[i];
		}
		Byte mask1[479];
		Byte mask2[487];
		Byte mask3[491];
		Byte mask4[499];
		UInt curIndex1;
		UInt curIndex2;
		UInt curIndex3;
		UInt curIndex4;
	};

	class FileVars : public Vars
	{
    public:
		SPADAS_VARS(File, Vars)

		Path path;
		Pointer file;
		FileMode mode;
		ULong size;
		ULong pos;
		Timer timer;
		Array<Char> scanBuffer;
		EncryptionContext *encCtx;
		FileVars(Path path0, Pointer file0, FileMode mode0, ULong size0, ULong pos0) : path(path0), file(file0), mode(mode0), size(size0), pos(pos0), encCtx(NULL)
        {
        }
        ~FileVars()
        {
			if (file)
			{
				if (isOutputMode()) file_internal::fileFlush(file);
				file_internal::fileClose(file);
			}
			if (encCtx)
			{
				delete encCtx;
			}
        }
		Bool isOutputMode()
		{
			return mode == FileMode::OutputBinary || mode == FileMode::OutputTextDefault || mode == FileMode::OutputTextUtf8 || mode == FileMode::OutputEncrypted;
		}
		Bool isInputTextMode()
		{
			return mode == FileMode::InputTextDefault || mode == FileMode::InputTextUtf8 || mode == FileMode::InputEncrypted;
		}
		Bool isOutputTextMode()
		{
			return mode == FileMode::OutputTextDefault || mode == FileMode::OutputTextUtf8 || mode == FileMode::OutputEncrypted;
		}
	};
}

using namespace spadas;
using namespace file_internal;

File::File()
{
}

File File::openBinary(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());
	SPADAS_ERROR_RETURNVAL_MSG(!filePath.exist(), filePathString, File());

	Pointer filePtr = fileOpen(filePathString, FALSE, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	ULong fileSize = fileGetSize(filePtr);
	fileSeek(filePtr, 0);

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::InputBinary, fileSize, 0), TRUE);
	return file;
}

File File::openText(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());
	SPADAS_ERROR_RETURNVAL_MSG(!filePath.exist(), filePathString, File());

	Pointer filePtr = fileOpen(filePathString, FALSE, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

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
	file.setVars(new FileVars(filePath, filePtr, isUtf8 ? FileMode::InputTextUtf8 : FileMode::InputTextDefault, fileSize, pos), TRUE);
	return file;
}

File File::openEncrypted(Path filePath, String password)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());

	if (!filePath.extension().endsWith("sef"))
	{
		filePathString += ".sef";
		filePath = filePathString;
	}
	SPADAS_ERROR_RETURNVAL_MSG(!filePath.exist(), filePathString, File());
	
	Pointer filePtr = fileOpen(filePathString, FALSE, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	ULong fileSize = fileGetSize(filePtr);
	if (fileSize < 16)
	{
		fileClose(filePtr);
		SPADAS_ERROR_MSG("fileSize < 16");
		return File();
	}
	fileSeek(filePtr, 0);

	String errorMsg;
	EncryptionContext *encCtx = EncryptionContext::createForOpenFile(filePtr, password, errorMsg);
	if (!encCtx)
	{
		fileClose(filePtr);
		SPADAS_ERROR_MSG(errorMsg);
		return File();
	}

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::InputEncrypted, fileSize, 16), TRUE);
	file.vars->encCtx = encCtx;
	return file;
}

File File::createBinary(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());

	if (filePath.exist()) fileRemove(filePathString);

	Path parent = filePath.parentFolder();
	if (!parent.exist()) parent.folderMake();

	Pointer filePtr = fileOpen(filePathString, TRUE, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::OutputBinary, 0, 0), TRUE);
	return file;
}

File File::createText(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());

	if (filePath.exist()) fileRemove(filePathString);

	Path parent = filePath.parentFolder();
	if (!parent.exist()) parent.folderMake();

	Pointer filePtr = fileOpen(filePathString, TRUE, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	fileOutput(filePtr, Binary::create(3, 0xEF, 0xBB, 0xBF));

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::OutputTextUtf8, 3, 3), TRUE);
	return file;
}

File File::createEncrypted(Path filePath, String password)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());
	
	if (!filePath.extension().endsWith("sef"))
	{
		filePathString += ".sef";
		filePath = filePathString;
	}
	if (filePath.exist()) fileRemove(filePathString);

	Path parent = filePath.parentFolder();
	if (!parent.exist()) parent.folderMake();

	Pointer filePtr = fileOpen(filePathString, TRUE, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::OutputEncrypted, 16, 16), TRUE);
	file.vars->encCtx = EncryptionContext::createForNewFile(filePtr, password);
	return file;
}

File File::appendBinary(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());

	if (!filePath.exist()) return createBinary(filePath);

	Pointer filePtr = fileOpen(filePathString, TRUE, TRUE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	ULong fileSize = fileGetSize(filePtr);

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::OutputBinary, fileSize, fileSize), TRUE);
	return file;
}

File File::appendText(Path filePath)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());

	if (!filePath.exist()) return createText(filePath);

	Pointer filePtr = fileOpen(filePathString, FALSE, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	ULong fileSize = fileGetSize(filePtr);
	fileSeek(filePtr, 0);

	Bool isUtf8 = FALSE;
	if (fileSize >= 3)
	{
		Binary maybeBom = fileInput(filePtr, 3, fileSize, 0);
		if (maybeBom.size() == 3 && maybeBom[0] == 0xEF && maybeBom[1] == 0xBB && maybeBom[2] == 0xBF)
		{
			isUtf8 = TRUE;
		}
	}

	fileClose(filePtr);

	filePtr = fileOpen(filePathString, TRUE, TRUE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	fileSize = fileGetSize(filePtr);

	File file;
	file.setVars(new FileVars(filePath, filePtr, isUtf8 ? FileMode::OutputTextUtf8 : FileMode::OutputTextDefault, fileSize, fileSize), TRUE);
	return file;
}

File File::appendEncrypted(Path filePath, String password)
{
	SPADAS_ERROR_RETURNVAL(filePath.isNull(), File());

	String filePathString = filePath.fullPath();
	SPADAS_ERROR_RETURNVAL_MSG(filePath.isFolder(), filePathString, File());

	if (!filePath.extension().endsWith("sef"))
	{
		filePathString += ".sef";
		filePath = filePathString;
	}
	if (!filePath.exist()) return createEncrypted(filePath, password);

	Pointer filePtr = fileOpen(filePathString, FALSE, FALSE);
	SPADAS_ERROR_RETURNVAL_MSG(!filePtr, filePathString, File());

	ULong fileSize = fileGetSize(filePtr);
	if (fileSize < 16)
	{
		fileClose(filePtr);
		SPADAS_ERROR_MSG("fileSize < 16");
		return File();
	}
	fileSeek(filePtr, 0);

	String errorMsg;
	EncryptionContext *encCtx = EncryptionContext::createForOpenFile(filePtr, password, errorMsg);
	if (!encCtx)
	{
		fileClose(filePtr);
		SPADAS_ERROR_MSG(errorMsg);
		return File();
	}

	fileClose(filePtr);

	filePtr = fileOpen(filePathString, TRUE, TRUE);
	if (!filePtr)
	{
		SPADAS_ERROR_MSG((String)"!filePtr. " + filePathString);
		delete encCtx;
		return File();
	}

	fileSize = fileGetSize(filePtr);
	encCtx->moveTo(fileSize);

	File file;
	file.setVars(new FileVars(filePath, filePtr, FileMode::OutputEncrypted, fileSize, fileSize), TRUE);
	file.vars->encCtx = encCtx;
	return file;
}

void File::close()
{
	if (!isClosed())
	{
		if (vars->isOutputMode()) fileFlush(vars->file);
		fileClose(vars->file);
		vars->file = NULL;
		if (vars->encCtx)
		{
			delete vars->encCtx;
			vars->encCtx = NULL;
		}
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
	SPADAS_ERROR_RETURN(isClosed() || !vars->isOutputTextMode());

	if (vars->mode == FileMode::OutputEncrypted)
	{
		Binary data = text.toBinary();
		for (UInt i = 0; i < data.size(); i++)
		{
			data[i] = vars->encCtx->next(data[i]);
		}

		Binary enter(1);
		enter[0] = vars->encCtx->next('\n');

		fileOutput(vars->file, data);
		fileOutput(vars->file, enter);
	}
	else
	{
		filePrint(vars->file, text, vars->mode == FileMode::OutputTextUtf8);
	}
	
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
	if (data.isEmpty()) return;

	if (vars->mode == FileMode::OutputEncrypted)
	{
		Binary localData(data.size());
		for (UInt i = 0; i < data.size(); i++)
		{
			localData[i] = vars->encCtx->next(data[i]);
		}
		fileOutput(vars->file, localData);
	}
	else
	{
		fileOutput(vars->file, data);
	}

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

	if (vars->mode == FileMode::InputEncrypted)
	{
		ULong originPos = vars->pos;

		ArrayX<BinarySpan> stringBuffer;
		UInt bufferedLength = 0;
		UInt enderLength = 0;
		while (TRUE)
		{
			Binary data = fileInput(vars->file, ENCRYPTED_SCAN_SIZE, vars->size, originPos + bufferedLength);
			if (data.isEmpty()) break;

			Int validLength = -1;
			for (UInt i = 0; i < data.size(); i++)
			{
				Char c = (Char)vars->encCtx->next(data[i]);
				data[i] = (Byte)c;
				if (c == 0)
				{
					validLength = i;
					break;
				}
				else if (c == '\n')
				{
					validLength = i;
					enderLength = 1;
					break;
				}
				else if (c == '\r')
				{
					validLength = i;
					enderLength = 2; // 默认为"\r\n"
					break;
				}
			}

			if (validLength == -1 && data.size() < ENCRYPTED_SCAN_SIZE)
			{
				validLength = data.size();
			}

			if (validLength == -1)
			{
				stringBuffer.append(data.span());
				bufferedLength += data.size();
			}
			else
			{
				if (validLength > 0)
				{
					stringBuffer.append(data.span(0, validLength));
					bufferedLength += validLength;
				}
				break;
			}
		}

		String output;
		for (auto e = stringBuffer.toArray().firstElem(); e.valid(); ++e)
		{
			output += e.value();
		}

		ULong targetPos = math::min(vars->size, originPos + bufferedLength + enderLength);
		vars->pos = fileSeek(vars->file, targetPos);
		vars->encCtx->moveTo(targetPos);

		return output;
	}
	else
	{
		if (vars->scanBuffer.isEmpty()) vars->scanBuffer = Array<Char>(SCAN_SIZE);

		String output = fileScan(vars->file, vars->scanBuffer.data(), vars->mode == FileMode::InputTextUtf8);
		vars->pos = filePos(vars->file);
		return output;
	}
}

Binary File::input(UInt size)
{
	if (size == 0) return Binary();
	
	SPADAS_ERROR_RETURNVAL(isClosed() || vars->isOutputMode(), Binary());

	Binary output = fileInput(vars->file, size, vars->size, vars->pos);

	if (vars->mode == FileMode::InputEncrypted)
	{
		for (UInt i = 0; i < output.size(); i++)
		{
			output[i] = vars->encCtx->next(output[i]);
		}
	}

	vars->pos += output.size();
	return output;
}

ULong File::seek(ULong pos)
{
	SPADAS_ERROR_RETURNVAL(isClosed() || vars->isOutputMode(), ULINF);

	if (vars->mode == FileMode::InputTextUtf8) pos = math::max(3ull, pos);
	else if (vars->mode == FileMode::InputEncrypted) pos = math::max(16ull, pos);

	vars->pos = fileSeek(vars->file, pos);
	if (vars->mode == FileMode::InputEncrypted) vars->encCtx->moveTo(vars->pos);

	return vars->pos;
}

Bool File::endOfFile()
{
	SPADAS_ERROR_RETURNVAL(isClosed() || vars->isOutputMode(), TRUE);

	return vars->pos >= vars->size;
}
