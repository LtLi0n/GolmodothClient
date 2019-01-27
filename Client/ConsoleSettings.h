#pragma once
class ConsoleSettings
{
public:
	ConsoleSettings(const int& render_width, const int& render_height, const int& font_width, const int& font_height);
	ConsoleSettings(const int& font_width, const int& font_height);

	bool fullscreen;

	int render_width;
	int render_height;
	int font_width;
	int font_height;

private:
	void GetDesktopResolution(int& width, int& height);
};

