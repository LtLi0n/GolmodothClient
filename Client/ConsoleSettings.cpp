#include "ConsoleSettings.h"

#include "wtypes.h"

ConsoleSettings::ConsoleSettings(const int& render_width, const int& render_height, const int& font_width, const int& font_height)
{
	this->render_width = render_width;
	this->render_height = render_height;
	this->font_width = font_width;
	this->font_height = font_height;

	fullscreen = false;
}

ConsoleSettings::ConsoleSettings(const int& font_width, const int& font_height)
{
	this->font_width = font_width;
	this->font_height = font_height;

	int raw_width = 0, raw_height = 0;
	GetDesktopResolution(raw_width, raw_height);

	render_width = raw_width / font_width + 1;
	render_height = raw_height / font_height - 1;

	fullscreen = true;
}

// Get the horizontal and vertical screen sizes in pixel
void ConsoleSettings::GetDesktopResolution(int& width, int& height)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	width = desktop.right;
	height = desktop.bottom;
}