﻿
#include "binary.h"
#include <bitset>

namespace des
{
	using namespace std;

	// 初始置换表
	int IP[] = { 58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7 };

	// 结尾置换表
	int IP_1[] = { 40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25 };

	/*------------------下面是生成密钥所用表-----------------*/

	// 密钥置换表，将64位密钥变成56位
	int PC_1[] = { 57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4 };

	// 压缩置换，将56位密钥压缩成48位子密钥
	int PC_2[] = { 14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32 };

	// 每轮左移的位数
	int shiftBits[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

	/*------------------下面是密码函数 f 所用表-----------------*/

	// 扩展置换表，将 32位 扩展至 48位
	int E[] = { 32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1 };

	// S盒，每个S盒是4x16的置换表，6位 -> 4位
	int S_BOX[8][4][16] = {
		{
			{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
			{ 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
			{ 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
			{ 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
		},
		{
			{ 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
			{ 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
			{ 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
			{ 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }
		},
		{
			{ 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
			{ 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
			{ 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
			{ 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
		},
		{
			{ 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
			{ 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
			{ 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
			{ 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
		},
		{
			{ 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
			{ 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
			{ 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
			{ 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
		},
		{
			{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
			{ 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
			{ 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
			{ 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
		},
		{
			{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
			{ 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
			{ 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
			{ 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
		},
		{
			{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
			{ 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
			{ 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
			{ 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
		}
	};

	// P置换，32位 -> 32位
	int P[] = { 16, 7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2, 8, 24, 14,
	32, 27, 3, 9,
	19, 13, 30, 6,
	22, 11, 4, 25 };

	/**********************************************************************/
	/*                                                                    */
	/*                            下面是DES算法实现                         */
	/*                                                                    */
	/**********************************************************************/

	/**
	*  密码函数f，接收32位数据和48位子密钥，产生一个32位的输出
	*/
	bitset<32> f(bitset<32> R, bitset<48> k)
	{
		bitset<48> expandR;
		// 第一步：扩展置换，32 -> 48
		for (int i = 0; i<48; ++i)
			expandR[47 - i] = R[32 - E[i]];
		// 第二步：异或
		expandR = expandR ^ k;
		// 第三步：查找S_BOX置换表
		bitset<32> output;
		int x = 0;
		for (int i = 0; i<48; i = i + 6)
		{
			int row = expandR[47 - i] * 2 + expandR[47 - i - 5];
			int col = expandR[47 - i - 1] * 8 + expandR[47 - i - 2] * 4 + expandR[47 - i - 3] * 2 + expandR[47 - i - 4];
			int num = S_BOX[i / 6][row][col];
			bitset<4> binary(num);
			output[31 - x] = binary[3];
			output[31 - x - 1] = binary[2];
			output[31 - x - 2] = binary[1];
			output[31 - x - 3] = binary[0];
			x += 4;
		}
		// 第四步：P-置换，32 -> 32
		bitset<32> tmp = output;
		for (int i = 0; i<32; ++i)
			output[31 - i] = tmp[32 - P[i]];
		return output;
	}

	/**
	*  对56位密钥的前后部分进行左移
	*/
	bitset<28> leftShift(bitset<28> k, int shift)
	{
		bitset<28> tmp = k;
		for (int i = 27; i >= 0; --i)
		{
			if (i - shift<0)
				k[i] = tmp[i - shift + 28];
			else
				k[i] = tmp[i - shift];
		}
		return k;
	}

	/**
	*  工具函数：将char字符数组转为二进制
	*/
	bitset<64> charToBitset(const char s[8])
	{
		bitset<64> bits;
		for (int i = 0; i<8; ++i)
		for (int j = 0; j<8; ++j)
			bits[i * 8 + j] = ((s[i] >> j) & 1);
		return bits;
	}

	/**
	*  DES加密
	*/
	bitset<64> encrypt(bitset<64>& plain, bitset<48> subKey[16])
	{
		bitset<64> cipher;
		bitset<64> currentBits;
		bitset<32> left;
		bitset<32> right;
		bitset<32> newLeft;
		// 第一步：初始置换IP
		for (int i = 0; i<64; ++i)
			currentBits[63 - i] = plain[64 - IP[i]];
		// 第二步：获取 Li 和 Ri
		for (int i = 32; i<64; ++i)
			left[i - 32] = currentBits[i];
		for (int i = 0; i<32; ++i)
			right[i] = currentBits[i];
		// 第三步：共16轮迭代
		for (int round = 0; round<16; ++round)
		{
			newLeft = right;
			right = left ^ f(right, subKey[round]);
			left = newLeft;
		}
		// 第四步：合并L16和R16，注意合并为 R16L16
		for (int i = 0; i<32; ++i)
			cipher[i] = left[i];
		for (int i = 32; i<64; ++i)
			cipher[i] = right[i - 32];
		// 第五步：结尾置换IP-1
		currentBits = cipher;
		for (int i = 0; i<64; ++i)
			cipher[63 - i] = currentBits[64 - IP_1[i]];
		// 返回密文
		return cipher;
	}

	/**
	*  DES解密
	*/
	bitset<64> decrypt(bitset<64>& cipher, bitset<48> subKey[16])
	{
		bitset<64> plain;
		bitset<64> currentBits;
		bitset<32> left;
		bitset<32> right;
		bitset<32> newLeft;
		// 第一步：初始置换IP
		for (int i = 0; i<64; ++i)
			currentBits[63 - i] = cipher[64 - IP[i]];
		// 第二步：获取 Li 和 Ri
		for (int i = 32; i<64; ++i)
			left[i - 32] = currentBits[i];
		for (int i = 0; i<32; ++i)
			right[i] = currentBits[i];
		// 第三步：共16轮迭代（子密钥逆序应用）
		for (int round = 0; round<16; ++round)
		{
			newLeft = right;
			right = left ^ f(right, subKey[15 - round]);
			left = newLeft;
		}
		// 第四步：合并L16和R16，注意合并为 R16L16
		for (int i = 0; i<32; ++i)
			plain[i] = left[i];
		for (int i = 32; i<64; ++i)
			plain[i] = right[i - 32];
		// 第五步：结尾置换IP-1
		currentBits = plain;
		for (int i = 0; i<64; ++i)
			plain[63 - i] = currentBits[64 - IP_1[i]];
		// 返回明文
		return plain;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////

using namespace spadas;

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

Binary binary_internal::desEncode(Binary sha1, const Byte* data, UInt rawsize)
{
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

	UInt size = (rawsize % 8 == 0) ? rawsize : ((rawsize / 8 + 1) * 8);

	Array<std::bitset<64> > input(size / 8);
	for (UInt i = 0; i < rawsize; i++)
	{
		byteToBitset(data[i], input[i / 8], (i % 8) * 8);
	}

	Array<std::bitset<64> > output(size / 8);
	for (UInt i = 0; i < input.size(); i++)
	{
		output[i] = des::encrypt(input[i], subKey);
	}

	Binary ret(size);
	for (UInt i = 0; i < size; i++)
	{
		bitsetToByte(output[i / 8], (i % 8) * 8, ret[i]);
	}
	return ret;
}

Optional<Binary> binary_internal::desDecode(Binary encrypted, Binary sha1)
{
	if (encrypted.isEmpty()) return Binary();
	if (encrypted.size() % 8 != 0) return Optional<Binary>();

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
		output[i] = des::decrypt(input[i], subKey);
	}

	Binary ret(size);
	for (UInt i = 0; i < size; i++)
	{
		bitsetToByte(output[i / 8], (i % 8) * 8, ret[i]);
	}

	return ret;
}