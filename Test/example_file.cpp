
#include "spadas.h"

using namespace spadas;

void exampleFile()
{
	// Create text file (Always output UTF8 encoding file with BOM header, line break as "\n") / 创建文本文件（统一输出带BOM头的UTF8编码文件，换行符为\n）
	Path outPath = "sample.txt";
	File outfile = File::createText(outPath);
	outfile.print("Hello world 新年好");
	outfile.print("Have a nice day 恭喜发财");
	outfile.close();

	// Read the text file (using "endOfFile" to get whether the file is finished) / 读取文本文件（通过endOfFile判断文件是否结束）
	File infile = File::openText("sample.txt");
	while (!infile.endOfFile())
	{
		console::print(infile.scan());
	}

	// You can call the "path" and "size" methods to obtain file information / 可调用path和size方法获取文件信息
	console::print("Path: " cat infile.path());
	console::print("Size: " cat infile.size());
	infile.close();

	// Delete file / 移除文件
	outPath.remove();
}