
#include "spadas.h"
#include "resource.h"

using namespace spadas;

String spadas::system::getSpadasVersion()
{
	return (String)SPADAS_VERSION_MAJOR + "." + SPADAS_VERSION_MINOR + "." + SPADAS_VERSION_BUILD;
}

Dictionary<String> spadas::system::getSpadasThirdPartyNotices()
{
	Dictionary<String> table;
	if (getEnv() == Environment::Value::Windows) table["MinGW-w64"] = resource_MinGW_w64_LICENSE;
	table["ConvertUTF"] = resource_ConvertUTF_LICENSE;
	table["C++ 11 BigInteger Library"] = resource_C___11_BigInteger_Library_LICENSE;
	table["base64.h base64.c"] = resource_base64_h_base64_c_LICENSE;
	table["svd.h svd.c pythag.h"] = resource_svd_h_svd_c_pythag_h_LICENSE;
	return table;
}