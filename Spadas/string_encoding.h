
namespace string_internal
{
    int cvtUTF8toUTF32(const char src[], int srcLength, wchar_t dst[], unsigned int dstSize); // return dstLength. src/dstLength = -1 while unknown
    int cvtUTF32toUTF8(const wchar_t src[], int srcLength, char dst[], unsigned int dstSize); // return dstLength. src/dstLength = -1 while unknown
    int cvtASCIItoUTF32(const char src[], int srcLength, wchar_t dst[], unsigned int dstSize); // return dstLength. src/dstLength = -1 while unknown
    int cvtUTF32toASCII(const wchar_t src[], int srcLength, char dst[], unsigned int dstSize); // return dstLength. src/dstLength = -1 while unknown
}
