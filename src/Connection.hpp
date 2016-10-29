#pragma once

#include "Types.hpp"

class Connection
{
public:
	static Connection& Instance() {
		static Connection instance;
		return instance;
	}
	
	xcb_connection_t* Get() { return _connection; }

private:
	Connection() {
		_connection = xcb_connect(nullptr, nullptr);
		if (xcb_connection_has_error(_connection)) {
			throw std::logic_error("Couldn't open display");
		}
	}

	xcb_connection_t*    _connection;
};
