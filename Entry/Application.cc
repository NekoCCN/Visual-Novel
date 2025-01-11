#include <iostream>
#include <fstream>
#include "Application.h"

int32_t entry(const std::string& title = "NULL", uint32_t width = 1920, uint32_t height = 1080,
	const std::string& theme_pack_path = "basic_asset.vntp")
{
	vn::entry::Application app("Test", width, height, R"(E:\TestFile\basic_theme.vntp)");
	return app();
}
int main()
{
	using namespace std;
	cout << "Hello World!" << '\n';

	// vn::asset::theme::ThemePackCreator creator(R"(E:\TestFile\basic_theme.vntp)", R"(E:\TestFile\)");
	// creator();


	entry("Test", 1920, 1080, R"(E:\TestFile\basic_theme.vntp)");
	entry("Test", 1920 / 3, 1080 / 3, R"(E:\TestFile\basic_theme.vntp)");
	// vn::asset::theme::ThemePack theme_pack(R"(E:\TestFile\basic_theme.vntp)");
	// ofstream file("E:\\basic_font.ttf", ios::out | ios::binary);
	// file.write(theme_pack[0].get(), theme_pack.getFileSize(0));
}