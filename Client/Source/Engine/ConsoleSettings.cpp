#include "ConsoleSettings.h"

#include "wtypes.h"

ConsoleSettings::ConsoleSettings(const int& render_width, const int& render_height, const int& font_width, const int& font_height) :
	_render_w(render_width),
	_render_h(render_height),
	_font_w(font_width),
	_font_h(font_height),
	fullscreen(false) { }

ConsoleSettings::ConsoleSettings(const int& font_width, const int& font_height) :
	_font_w(font_width),
	_font_h(font_height)
{
	int raw_width, raw_height;
	GetDesktopResolution(raw_width, raw_height);

	_render_w = raw_width / font_width + 1;
	_render_h = raw_height / font_height - 1;

	fullscreen = true;
}

int ConsoleSettings::RenderWidth() const { return _render_w; }
int ConsoleSettings::RenderHeight() const { return _render_h; }
int ConsoleSettings::FontWidth() const { return _font_w; }
int ConsoleSettings::FontHeight() const { return _font_h; }

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