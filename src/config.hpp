#pragma once

static const char* dmenucmd[] = {"dmenu_run", NULL};

static HotKey hotkeys[] = {
	/*modifier				key			function					arguments*/
	{KeyModifier::Ctrl,		Key::c,		WindowManager::quit,		{0}},
	{KeyModifier::Ctrl,		Key::r,		WindowManager::restart,		{0}},
	{KeyModifier::Super,	Key::r,		WindowManager::spawn,		{.v = dmenucmd}}
};
