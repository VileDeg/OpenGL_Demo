namespace Crave
{
	namespace WinUtils
	{
		std::string GetExecutableDirectory();
		std::optional<std::string> SaveFile(const char* filter);
		std::optional<std::string> OpenFile(const char* filter);
	}
}