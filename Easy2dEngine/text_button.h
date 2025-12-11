#ifndef _TEXT_BUTTON_H_
#define _TEXT_BUTTON_H_

#include "button.h"
#include "textbox.h"

class TextButton : public Button
{
public:
	TextButton() = default;
	~TextButton() = default;

private:
	TextBox* textbox = nullptr;
};

#endif // !_TEXT_BUTTON_H_