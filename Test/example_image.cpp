#define SPADAS_ENABLE_MACROS
#include "spadas.h"
#include "resource.h"

using namespace spadas;

Double diffImage(Image image1, Image image2) // Calculate difference of two images / 计算两图像数据差别
{
    if (image1.isNull() || image2.isNull()) return -1;
    if (image1.size() != image2.size() || image1.format() != image2.format()) return -1;

    UInt rowBytes = image1.rowBytesValid();
    Double diffSum = 0.0;
    for (UInt v = 0; v < image1.height(); v++)
    {
        Byte *row1 = image1[v].b;
        Byte *row2 = image2[v].b;
        for (UInt n = 0; n < rowBytes; n++)
        {
            diffSum += math::abs((Int)row1[n] - (Int)row2[n]);
        }
    }

    return diffSum / (image1.height() * rowBytes);
}

void exampleImage()
{
	// Decode images (png4 is with alpha channel) / 解码图像数据（其中png4含alpha通道）
    Image srcJpg(ImageFileFormat::Value::JPG, resource_input_jpg);
    Image srcPng3(ImageFileFormat::Value::PNG, resource_input3_png);
    Image srcPng4(ImageFileFormat::Value::PNG, resource_input4_png);

	// Encode images / 编码图像数据
    Binary dstPng3Data = srcPng3.toBinary(ImageFileFormat::Value::PNG);
    Binary dstPng4Data = srcPng4.toBinary(ImageFileFormat::Value::PNG);
    Binary dstJpgData = srcJpg.toBinary(ImageFileFormat::Value::JPG, 80);

	// Decode again and compare / 再次解码并比较数据
    Image dstPng3(ImageFileFormat::Value::PNG, dstPng3Data);
    Image dstPng4(ImageFileFormat::Value::PNG, dstPng4Data);
    Image dstJpg(ImageFileFormat::Value::JPG, dstJpgData);

    console::print("Difference of jpg: " cat diffImage(srcJpg, dstJpg));
    console::print("Difference of png3: " cat diffImage(srcPng3, dstPng3));
    console::print("Difference of png4: " cat diffImage(srcPng4, dstPng4));
}