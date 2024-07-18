
#include <bitset>
#include "spadas.h"
#include "binary.h"

std::bitset<64> encrypt(std::bitset<64>& plain, std::bitset<48> subKey[16]);
std::bitset<64> decrypt(std::bitset<64>& cipher, std::bitset<48> subKey[16]);

using namespace binary_internal;

void byteToBitset(Byte input, std::bitset<48>& bits, int offset)
{
	bits[offset] = (input & 0x01) != 0;
	bits[offset + 1] = (input & 0x02) != 0;
	bits[offset + 2] = (input & 0x04) != 0;
	bits[offset + 3] = (input & 0x08) != 0;
	bits[offset + 4] = (input & 0x10) != 0;
	bits[offset + 5] = (input & 0x20) != 0;
	bits[offset + 6] = (input & 0x40) != 0;
	bits[offset + 7] = (input & 0x80) != 0;
}

void byteToBitset(Byte input, std::bitset<64>& bits, int offset)
{
	bits[offset] = (input & 0x01) != 0;
	bits[offset + 1] = (input & 0x02) != 0;
	bits[offset + 2] = (input & 0x04) != 0;
	bits[offset + 3] = (input & 0x08) != 0;
	bits[offset + 4] = (input & 0x10) != 0;
	bits[offset + 5] = (input & 0x20) != 0;
	bits[offset + 6] = (input & 0x40) != 0;
	bits[offset + 7] = (input & 0x80) != 0;
}

void bitsetToByte(std::bitset<64>& bits, int offset, Byte& ret)
{
	ret = 0;
	ret |= (bits[offset] ? 1 : 0) << 0;
	ret |= (bits[offset+1] ? 1 : 0) << 1;
	ret |= (bits[offset+2] ? 1 : 0) << 2;
	ret |= (bits[offset+3] ? 1 : 0) << 3;
	ret |= (bits[offset+4] ? 1 : 0) << 4;
	ret |= (bits[offset+5] ? 1 : 0) << 5;
	ret |= (bits[offset+6] ? 1 : 0) << 6;
	ret |= (bits[offset+7] ? 1 : 0) << 7;
}

void genSubKey(Binary sha1, int offset, std::bitset<48>& bits)
{
	byteToBitset(sha1[offset], bits, 0);
	byteToBitset(sha1[offset + 1], bits, 8);
	byteToBitset(sha1[offset + 2], bits, 16);
	byteToBitset(sha1[offset + 3], bits, 24);
	byteToBitset(sha1[offset + 4], bits, 32);
	byteToBitset(sha1[offset + 5], bits, 40);
}

Binary BinaryCommon::toDES(const Byte* bytes, UInt rawSize, String& key)
{
	if (rawSize == 0) return Binary();
	if (key.isEmpty()) key = "spadas";

	Binary sha1 = key.toBinary().toSHA1();
	Binary sha1ex(21, 0);
	for (UInt i = 0; i < sha1.size(); i++) sha1ex[i] = sha1[i];

	std::bitset<48> subKey[16];
	genSubKey(sha1ex, 0, subKey[0]);
	genSubKey(sha1ex, 6, subKey[1]);
	genSubKey(sha1ex, 12, subKey[2]);
	genSubKey(sha1ex, 2, subKey[3]);
	genSubKey(sha1ex, 8, subKey[4]);
	genSubKey(sha1ex, 14, subKey[5]);
	genSubKey(sha1ex, 4, subKey[6]);
	genSubKey(sha1ex, 10, subKey[7]);
	genSubKey(sha1ex, 1, subKey[8]);
	genSubKey(sha1ex, 7, subKey[9]);
	genSubKey(sha1ex, 13, subKey[10]);
	genSubKey(sha1ex, 3, subKey[11]);
	genSubKey(sha1ex, 9, subKey[12]);
	genSubKey(sha1ex, 15, subKey[13]);
	genSubKey(sha1ex, 5, subKey[14]);
	genSubKey(sha1ex, 11, subKey[15]);

	UInt size = (rawSize % 8 == 0) ? rawSize : ((rawSize / 8 + 1) * 8);

	Array<std::bitset<64> > input(size / 8);
	for (UInt i = 0; i < rawSize; i++)
	{
		byteToBitset(bytes[i], input[i / 8], (i % 8) * 8);
	}

	Array<std::bitset<64> > output(size / 8);
	for (UInt i = 0; i < input.size(); i++)
	{
		output[i] = encrypt(input[i], subKey);
	}

	Binary ret(size);
	for (UInt i = 0; i < size; i++)
	{
		bitsetToByte(output[i / 8], (i % 8) * 8, ret[i]);
	}

	return ret;
}

Optional<Binary> Binary::createFromDES(Binary encrypted, String key)
{
	if (encrypted.isEmpty()) return Binary();
	if (encrypted.size() % 8 != 0) return Optional<Binary>();
	if (key.isEmpty()) key = "spadas";

	Binary sha1 = key.toBinary().toSHA1();
	Binary sha1ex(21, 0);
	for (UInt i = 0; i < sha1.size(); i++) sha1ex[i] = sha1[i];

	std::bitset<48> subKey[16];
	genSubKey(sha1ex, 0, subKey[0]);
	genSubKey(sha1ex, 6, subKey[1]);
	genSubKey(sha1ex, 12, subKey[2]);
	genSubKey(sha1ex, 2, subKey[3]);
	genSubKey(sha1ex, 8, subKey[4]);
	genSubKey(sha1ex, 14, subKey[5]);
	genSubKey(sha1ex, 4, subKey[6]);
	genSubKey(sha1ex, 10, subKey[7]);
	genSubKey(sha1ex, 1, subKey[8]);
	genSubKey(sha1ex, 7, subKey[9]);
	genSubKey(sha1ex, 13, subKey[10]);
	genSubKey(sha1ex, 3, subKey[11]);
	genSubKey(sha1ex, 9, subKey[12]);
	genSubKey(sha1ex, 15, subKey[13]);
	genSubKey(sha1ex, 5, subKey[14]);
	genSubKey(sha1ex, 11, subKey[15]);

	UInt size = encrypted.size();
	Byte *data = encrypted.data();

	Array<std::bitset<64> > input(size / 8);
	for (UInt i = 0; i < size; i++)
	{
		byteToBitset(data[i], input[i / 8], (i % 8) * 8);
	}

	Array<std::bitset<64> > output(size / 8);
	for (UInt i = 0; i < input.size(); i++)
	{
		output[i] = decrypt(input[i], subKey);
	}

	Binary ret(size);
	for (UInt i = 0; i < size; i++)
	{
		bitsetToByte(output[i / 8], (i % 8) * 8, ret[i]);
	}

	return ret;
}