#include <iostream>
#include "Application.h"

int main()
{
	using namespace std;
	cout << "Hello World!" << '\n';

	// vn::asset::theme::ThemePackCreator creator(R"(E:\TestFile\basic_theme.vntp)", R"(E:\TestFile\)");
	// creator();

	vn::entry::Application app("Test", 1920, 1080, R"(E:\TestFile\basic_theme.vntp)");
	app();
}