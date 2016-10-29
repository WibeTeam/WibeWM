#pragma once

#include "Types.hpp"
#include "Client.hpp"
#include "Layouts.hpp"


class Monitor {
public:
	Monitor::Monitor(const std::forward_list<Tag>& tags)
			:_tags(tags)
	{ }

private:
	bool _showBar;
	bool _barAtTop;
	int barHeight;

	Rectangle _windowRect;
	Rectangle _screenRect;

	Window _bar;

	std::forward_list<Tag>& _tags;

	unique_ptr<ILayout> _layout;
};