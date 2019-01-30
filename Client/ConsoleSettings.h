#pragma once
class ConsoleSettings
{
public:
	ConsoleSettings(const int& render_width, const int& render_height, const int& font_width, const int& font_height);
	ConsoleSettings(const int& font_width, const int& font_height);

	int RenderWidth() const;
	int RenderHeight() const;
	int FontWidth() const;
	int FontHeight() const;

	bool fullscreen;

private:
	void GetDesktopResolution(int& width, int& height);
	int _render_w;
	int _render_h;
	const int _font_w;
	const int _font_h;
};

