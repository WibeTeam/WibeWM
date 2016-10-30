#pragma once

#include "Types.hpp"
#include "Client.hpp"
#include "Layouts.hpp"

class Monitor {
public:
	Monitor::Monitor(const std::forward_list<Tag>& tags)
			:_tags(tags)
	{ }

	std::string ltsymbol;

	double MasterAreaFactor;
	int    WindowInMasterArea;
	bool   BarVisible;
	int  num;

	bool   BarAtTop;
	int    BarHeight;
	Rectangle WindowRect;
	Rectangle ScreenRect;

	Window Bar;

	std::forward_list<Tag> Tags;

	unique_ptr<ILayout> _layout;
};