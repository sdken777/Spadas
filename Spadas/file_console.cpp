
#include "spadas.h"

namespace spadas
{
	class FileConsoleVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(FileConsole, Vars)

		File file;
		String errorPrefix;
		String warningPrefix;
		String infoPrefix;
		String debugPrefix;

		FileConsoleVars()
		{
			errorPrefix = "[ERRO] ";
			warningPrefix = "[WARN] ";
			infoPrefix = "[INFO] ";
			debugPrefix = "[DBUG] ";
		}
	};
}

using namespace spadas;

const String spadas::FileConsole::TypeName = "spadas.FileConsole";

FileConsole::FileConsole() : Object<FileConsoleVars>(new FileConsoleVars, TRUE)
{

}

FileConsole::FileConsole(Path filePath) : Object<FileConsoleVars>(new FileConsoleVars, TRUE)
{
	SPADAS_ERROR_RETURN(filePath.isNull());
	SPADAS_ERROR_RETURN(filePath.isFolder());
	vars->file = File::createText(filePath);
}

void FileConsole::print(String text, Enum<MessageLevel> level)
{
	if (!vars->file.isValid()) return;

	switch (level.value())
	{
	case MessageLevel::Error:
		vars->file.print(vars->errorPrefix + text);
		break;
	case MessageLevel::Warning:
		vars->file.print(vars->warningPrefix + text);
		break;
	case MessageLevel::Info:
		vars->file.print(vars->infoPrefix + text);
		break;
	case MessageLevel::Debug:
		vars->file.print(vars->debugPrefix + text);
		break;
	default:
		break;
	}
}

