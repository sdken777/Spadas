
#include "spadas.h"

namespace spadas
{
	class FileConsoleVars : public Vars
	{
	public:
		SPADAS_VARS(FileConsole, Vars)

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

FileConsole::FileConsole() : BaseObject(new FileConsoleVars)
{

}

FileConsole::FileConsole(Path filePath) : BaseObject(new FileConsoleVars)
{
	SPADAS_ERROR_RETURN(filePath.isNull());
	SPADAS_ERROR_RETURN(filePath.isFolder());
	var()->file = File::createText(filePath);
}

void FileConsole::print(String text, Enum<MessageLevel> level)
{
	if (!var()->file.isValid()) return;

	switch (level.value())
	{
	case MessageLevel::Value::Error:
		var()->file.print(var()->errorPrefix + text);
		break;
	case MessageLevel::Value::Warning:
		var()->file.print(var()->warningPrefix + text);
		break;
	case MessageLevel::Value::Info:
		var()->file.print(var()->infoPrefix + text);
		break;
	case MessageLevel::Value::Debug:
		var()->file.print(var()->debugPrefix + text);
		break;
	default:
		break;
	}
}

