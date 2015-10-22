#ifndef TEXTUI_H
#define TEXTUI_H

#include "UI.h"
#include <iostream>

namespace horizon {
namespace ui {

	using std::cout;
/**
 * A text-based user interface to the application, operating on an input stream
 * and an output stream.
 * @author Einar Pehrson
 */
class TextUI : public UI {

public:
	// StreamUI(inputstream in, outputstream out, bool loopback = false);
	TextUI();
	void show();
};

}} // end namespace horizon::ui

#endif /*TEXTUI_H_*/
