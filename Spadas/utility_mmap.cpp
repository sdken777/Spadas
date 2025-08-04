
// MemoryMap //////////////////////////////////////////////////

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>
#include "spadas.h"

namespace spadas
{
	class MemoryMapVars : public Vars
	{
	public:
		SPADAS_VARS(MemoryMap, Vars)

		HANDLE fileHandle;
		HANDLE mmapHandle;
		Pointer virtualPointer;
		
		void unmap()
		{
			if (virtualPointer != 0)
			{
				UnmapViewOfFile(virtualPointer);
				virtualPointer = 0;
			}
			if (mmapHandle != (HANDLE)-1)
			{
				CloseHandle(mmapHandle);
				mmapHandle = (HANDLE)-1;
			}
			if (fileHandle != (HANDLE)-1)
			{
				CloseHandle(fileHandle);
				fileHandle = (HANDLE)-1;
			}
		}
		
		MemoryMapVars()
		{
			fileHandle = (HANDLE)-1;
			mmapHandle = (HANDLE)-1;
			virtualPointer = 0;
		}
		~MemoryMapVars()
		{
			unmap();
		}
	};
}

using namespace spadas;

MemoryMap::MemoryMap()
{
}

MemoryMap::MemoryMap(Path file, PointerInt offset, PointerInt size)
{
	SPADAS_ERROR_RETURN(file.isNull());
	SPADAS_ERROR_RETURN(!file.exist());
	SPADAS_ERROR_RETURN(file.isFolder());

	String fileFullPath = file.fullPath();

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	SPADAS_ERROR_RETURN(offset % systemInfo.dwAllocationGranularity != 0);

	HANDLE fileHandle = CreateFileW(fileFullPath.wchars().data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	SPADAS_ERROR_RETURN(fileHandle == (HANDLE)-1);

	HANDLE mmapHandle = CreateFileMappingW(fileHandle, 0, PAGE_READWRITE, 0, (DWORD)(offset + size), 0);
	if (mmapHandle == (HANDLE)-1)
	{
		SPADAS_ERROR_MSG("mmapHandle == (HANDLE)-1");
		CloseHandle(fileHandle);
		return;
	}

	Pointer mmapPtr = MapViewOfFile(mmapHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, (DWORD)offset, size);
	if (mmapPtr == 0)
	{
		SPADAS_ERROR_MSG("mmapPtr == 0");
		CloseHandle(mmapHandle);
		CloseHandle(fileHandle);
		return;
	}

	setVars(new MemoryMapVars(), TRUE);
	var()->fileHandle = fileHandle;
	var()->mmapHandle = mmapHandle;
	var()->virtualPointer = mmapPtr;
}

void MemoryMap::unmap()
{
	if (vars) var()->unmap();
}

Pointer MemoryMap::getPointer()
{
	SPADAS_ERROR_RETURNVAL(!vars, 0);
	return var()->virtualPointer;
}

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#undef NULL

#include "spadas.h"

namespace spadas
{
	class MemoryMapVars : public Vars
	{
	public:
		SPADAS_VARS(MemoryMap, Vars)

		UInt fileDescriptor;
		Pointer virtualPointer;
		PointerInt size;
		
		void unmap()
		{
			if (virtualPointer != 0)
			{
				if (munmap(virtualPointer, size))
				{
					SPADAS_ERROR_MSG("munmap(virtualPointer, size)");
				}
				virtualPointer = 0;
				size = 0;
			}
			if (fileDescriptor != UINF)
			{
				close(fileDescriptor);
				fileDescriptor = UINF;
			}
		}

		MemoryMapVars()
		{
			fileDescriptor = UINF;
			virtualPointer = 0;
			size = 0;
		}
		~MemoryMapVars()
		{
			unmap();
		}
	};
}

using namespace spadas;

MemoryMap::MemoryMap()
{
}

MemoryMap::MemoryMap(Path file, PointerInt offset, PointerInt size)
{
	SPADAS_ERROR_RETURN(file.isNull());
	SPADAS_ERROR_RETURN(!file.exist());
	SPADAS_ERROR_RETURN(file.isFolder());
	SPADAS_ERROR_RETURN(offset % 4096 != 0);

	String fileFullPath = file.fullPath();

	UInt fileDescriptor = (UInt)open(fileFullPath.chars().data(), O_RDWR);
	SPADAS_ERROR_RETURN(fileDescriptor == UINF);

	Pointer ptr = (Byte*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, (off_t)offset);
	if (ptr == MAP_FAILED)
	{
		SPADAS_ERROR_MSG("ptr == MAP_FAILED");
		close(fileDescriptor);
		return;
	}

	setVars(new MemoryMapVars(), TRUE);
	var()->fileDescriptor = fileDescriptor;
	var()->virtualPointer = ptr;
	var()->size = size;
}

void MemoryMap::unmap()
{
	if (vars) var()->unmap();
}

Pointer MemoryMap::getPointer()
{
	SPADAS_ERROR_RETURNVAL(!vars, 0);
	return var()->virtualPointer;
}

#endif

// MemoryMapStream //////////////////////////////////////////////////

namespace spadas
{
    class MemoryMapStreamVars : public Vars
    {
    public:
		SPADAS_VARS(MemoryMapStream, Vars)

        MemoryMap mm; // 8<SendCount> + 8<ReceiveCount> + (4<DataSize> + slotSize) * slotCount(Slots)
        Path path;
        UInt slotSize;
        UInt slotCount;
        Bool host;
        Bool mode;

		Bool isSpinLockManaged() override
		{
			return TRUE;
		}
    };
}

using namespace spadas;

MemoryMapStream::MemoryMapStream() : BaseObject(new MemoryMapStreamVars)
{

}
Bool MemoryMapStream::isOpen()
{
    var()->spinEnter();
    Bool res = var()->mm.isValid();
	var()->spinLeave();
	return res;
}
Bool MemoryMapStream::open(Path path, UInt slotSize, UInt slotCount, Bool mode, Bool host)
{
    close();

    if (path.isNull() || path.isFolder()) return FALSE;
    if (!host && !path.exist()) return FALSE;
    if (slotSize < 16 || slotSize > 1048576) return FALSE;
    if (slotCount < 1 || slotCount > 1024) return FALSE;

    var()->spinEnter();

    var()->path = path;
    var()->slotSize = slotSize;
    var()->slotCount = slotCount;
    var()->mode = mode;
    var()->host = host;

    UInt mmFileSize = 8 + 8 + (4 + var()->slotSize) * var()->slotCount;

    if (var()->host)
    {
        var()->path.fileCreate(mmFileSize);
        if (!var()->path.exist())
		{
			var()->spinLeave();
			return FALSE;
		}
    }

    var()->mm = MemoryMap(path, 0, mmFileSize);

	var()->spinLeave();
    return TRUE;
}
void MemoryMapStream::close()
{
    var()->spinEnter();

    if (!var()->mm.isValid())
	{
		var()->spinLeave();
		return;
	}

    var()->mm.unmap();
    var()->mm = MemoryMap();
    if (var()->host) var()->path.remove();

	var()->spinLeave();
}
Enum<MemoryMapSendResult> MemoryMapStream::send(Pointer dataPtr, UInt byteCount)
{
    if (!var()->mode) return MemoryMapSendResult::Value::WrongMode;
    if (byteCount == 0) return MemoryMapSendResult::Value::WrongSize;
    if (byteCount > var()->slotSize * var()->slotCount) return MemoryMapSendResult::Value::WrongSize;

    var()->spinEnter();
    if (!var()->mm.isValid())
	{
		var()->spinLeave();
		return MemoryMapSendResult::Value::NotOpen;
	}

    UInt slotNeeded = (byteCount + var()->slotSize - 1) / var()->slotSize;
    UInt finalSlotByteCount = byteCount % var()->slotSize;
    if (finalSlotByteCount == 0) finalSlotByteCount = var()->slotSize;

    Byte *ptr = (Byte*)var()->mm.getPointer();
    ULong sendCount = *(ULong*)&ptr[0];
    ULong receiveCount = *(ULong*)&ptr[8];
    if (sendCount + slotNeeded > receiveCount + var()->slotCount)
	{
		var()->spinLeave();
		return MemoryMapSendResult::Value::QueueFull;
	}

	Byte* dataBytePtr = (Byte*)dataPtr;
    for (UInt i = 0; i < slotNeeded; i++)
    {
        Byte *dstPtr = &ptr[8 + 8 + (4 + var()->slotSize) * ((sendCount + i) % var()->slotCount)];
        *(UInt*)&dstPtr[0] = i == slotNeeded - 1 ? finalSlotByteCount : UINF;
        utility::memoryCopy(&dataBytePtr[i * var()->slotSize], &dstPtr[4], i == slotNeeded - 1 ? finalSlotByteCount : var()->slotSize);
    }

    *(ULong*)&ptr[0] = (sendCount + slotNeeded); // 一次性修改，保证接收时的完整性(最后一个packet肯定是一帧的末尾packet)

	var()->spinLeave();
    return MemoryMapSendResult::Value::OK;
}
Enum<MemoryMapSendResult> MemoryMapStream::send(Binary data)
{
    return send(data.data(), data.size());
}
Array<Binary> MemoryMapStream::receive()
{
    if (var()->mode) return Array<Binary>();
    
    var()->spinEnter();
    if (!var()->mm.isValid())
	{
		var()->spinLeave();
		return Array<Binary>();
	}

    Byte *ptr = (Byte*)var()->mm.getPointer();
    ULong sendCount = *(ULong*)&ptr[0];
    ULong receiveCount = *(ULong*)&ptr[8];

    ArrayX<Binary> output;
    if (sendCount - receiveCount <= var()->slotCount)
    {
        UInt packetCount = (UInt)(sendCount - receiveCount);

        ArrayX<Binary> buffer;
        for (UInt i = 0; i < packetCount; i++)
        {
            Byte *srcPtr = &ptr[8 + 8 + (4 + var()->slotSize) * ((i + receiveCount) % var()->slotCount)];

            UInt packetSize = *(UInt*)&srcPtr[0];
            Bool frameEnder = packetSize <= var()->slotSize;
            packetSize = math::min(var()->slotSize, packetSize);

            Binary packet(packetSize);
            utility::memoryCopy(&srcPtr[4], packet.data(), packet.size());

            if (frameEnder)
            {
                if (buffer.isEmpty())
                {
                    output.append(packet);
                }
                else
                {
                    buffer.append(packet);
                    output.append(Binary::merge(buffer.toArray()));
                    buffer.clear();
                }
            }
            else buffer.append(packet);
        }
    }
    
    *(ULong*)&ptr[8] = sendCount;

	var()->spinLeave();
    return output.toArray();
}