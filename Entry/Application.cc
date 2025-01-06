#include <iostream>
#include <fstream>
#include "Application.h"

int main()
{
	using namespace std;
	cout << "Hello World!" << '\n';

	// vn::asset::theme::ThemePackCreator creator(R"(E:\TestFile\basic_theme.vntp)", R"(E:\TestFile\)");
	// creator();

	vn::entry::Application app("Test", 1920, 1080, R"(E:\TestFile\basic_theme.vntp)");
	app();

	// vn::asset::theme::ThemePack theme_pack(R"(E:\TestFile\basic_theme.vntp)");
	// ofstream file("E:\\basic_font.ttf", ios::out | ios::binary);
	// file.write(theme_pack[0].get(), theme_pack.getFileSize(0));
}