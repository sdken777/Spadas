
#define SPADAS_DEPRECATED_HIDE
#include "spadas.h"

using namespace spadas;

void exampleFile()
{
	// 创建文本文件（统一输出带BOM头的UTF8编码文件，换行符为\n）
	Path outPath = "sample.txt";
	File outfile = File::createText(outPath);
	outfile.print("新年好");
	outfile.print("恭喜发财");
	outfile.close();

	// 读取文本文件（通过endOfFile判断文件是否结束）
	File infile = File::openText("sample.txt");
	while (!infile.endOfFile())
	{
		console::print(infile.scan());
	}

	// 可调用path和size方法获取文件信息
	console::print(SS"Path: " + infile.path().fullPath());
	console::print(SS"Size: " + infile.size());
	infile.close();

	// 移除文件
	outPath.remove();
}