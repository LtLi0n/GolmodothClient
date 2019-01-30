#include "Interface.h"

Interface::Interface(ConsoleEngine& engine) :
	_engine(engine),
	_menu(engine),
	_chat(engine) { }

Menu& Interface::GetMenu() { return _menu; }
Chat& Interface::GetChat() { return _chat; }