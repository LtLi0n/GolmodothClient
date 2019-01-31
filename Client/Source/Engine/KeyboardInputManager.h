#pragma once

#include <string>
#include <chrono>

class KeyboardInputManager
{
public:
	std::wstring input;
	std::chrono::time_point<std::chrono::system_clock> last_blink;
	bool receive_input;
	KeyboardInputManager();
};

