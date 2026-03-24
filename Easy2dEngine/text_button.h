#ifndef _TEXT_BUTTON_H_
#define _TEXT_BUTTON_H_

#include "button.h"
#include "text.h"

class TextButton : public Button
{
public:
	TextButton() = default;
	~TextButton() = default;

private:
	Text* text = nullptr;
};

#endif // !_TEXT_BUTTON_H_