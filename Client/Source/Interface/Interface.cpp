#include "Interface.h"

Interface::Interface(ConsoleEngine& engine, TlsClient& tls) :
	_engine(engine),
	_menu(engine),
	_chat(engine, tls) { }

Menu& Interface::GetMenu() { return _menu; }
Chat& Interface::GetChat() { return _chat; }