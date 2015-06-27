#include "TextUI.h"

namespace horizon {
namespace ui {

TextUI::TextUI() {
	cout << "PolySTAR: Horizon version 2.0\n\n";
}

void TextUI::show() {
	cout << "Press enter to continue...";
	getchar();
}

}} // end namespace horizon::ui
/*
int main(int argc, char** argv) {
	horizon::ui::TextUI ui;
	ui.show();
	return EXIT_SUCCESS;
}
*/