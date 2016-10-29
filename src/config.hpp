#pragma once

static const char* dmenucmd[] = {"dmenu_run", NULL};
static const char* urxvt[] = {"urxvt", NULL};
static const char* xrdb[] = {"xrdb","-merge", "/home/vitriol/.Xresources", NULL};

static HotKey hotkeys[] = {
	/*modifier				key			function					arguments*/
	{KeyModifier::Ctrl,		Key::c,		WindowManager::quit,		{0}},
	{KeyModifier::Super,    Key::r,		WindowManager::restart,		{0}},
	{KeyModifier::Super,	Key::Return,		WindowManager::spawn,		{.v = urxvt}},
	{KeyModifier::Super,	Key::l,		WindowManager::spawn,		{.v = xrdb}},
	{KeyModifier::Super,	Key::p,		WindowManager::spawn,		{.v = dmenucmd}}
};
