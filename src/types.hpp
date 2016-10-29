#pragma once

#include <cstdint>

/* Numeric types */
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

#define DECLARE_TYPE_AND_PTR(Name, Type) \
using Name = Type; \
using Name##Ptr = Type*

/* XCB basic types */
DECLARE_TYPE_AND_PTR(Connection, xcb_connection_t);
DECLARE_TYPE_AND_PTR(Screen, xcb_screen_t);

/* XCB event types */
DECLARE_TYPE_AND_PTR(GenericEvent, xcb_generic_event_t);
DECLARE_TYPE_AND_PTR(ExposeEvent, xcb_expose_event_t);
DECLARE_TYPE_AND_PTR(ButtonPressEvent, xcb_button_press_event_t);
DECLARE_TYPE_AND_PTR(ButtonReleaseEvent, xcb_button_release_event_t);
DECLARE_TYPE_AND_PTR(MotionNotifyEvent, xcb_motion_notify_event_t);
DECLARE_TYPE_AND_PTR(EnterNotifyEvent, xcb_enter_notify_event_t);
DECLARE_TYPE_AND_PTR(LeaveNotifyEvent, xcb_leave_notify_event_t);
DECLARE_TYPE_AND_PTR(KeyPressEvent, xcb_key_press_event_t);
DECLARE_TYPE_AND_PTR(KeyReleaseEvent, xcb_key_release_event_t);


/* Keyboard */
enum class Key {
	Esc = 9,
	_1 = 10,
	_2 = 11,
	_3 = 12,
	_4 = 13,
	_5 = 14,
	_6 = 15,
	_7 = 16,
	_8 = 17,
	_9 = 18,
	_0 = 19,
	minus = 20,
	equal = 21,
	BackSpace = 22,
	Tab = 23,
	q = 24,
	w = 25,
	e = 26,
	r = 27,
	t = 28,
	y = 29,
	u = 30,
	i = 31,
	o = 32,
	p = 33,
	BracetLeft = 34,
	BracketRight = 35,
	Return = 36,
	Control_L = 37,
	a = 38,
	s = 39,
	d = 40,
	f = 41,
	g = 42,
	h = 43,
	j = 44,
	k = 45,
	l = 46,
	semicolon = 47,
	apostrophe = 48,
	grave = 49,
	Shift_L = 50,
	backslash = 51,
	z = 52,
	x = 53,
	c = 54,
	v = 55,
	b = 56,
	n = 57,
	m = 58,
	comma = 59,
	period = 60,
	slash = 61,
	Shift_R = 62,
	KP_Multiply = 63,
	Alt_L = 64,
	Space = 65,
	Control_L = 66,
	F1 = 67,
	F2 = 68,
	F3 = 69,
	F4 = 70,
	F5 = 71,
	F6 = 72,
	F7 = 73,
	F8 = 74,
	F9 = 75,
	F10 = 76,
	Num_Lock = 77,
	Scroll_Lock = 78,
	KP_7 = 79,
	KP_8 = 80,
	KP_9 = 81,
	KP_Subtract = 82,
	KP_4 = 83,
	KP_5 = 84,
	KP_6 = 85,
	KP_Add = 86,
	KP_1 = 87,
	KP_2 = 88,
	KP_3 = 89,
	KP_Insert = 90,
	KP_Delete = 91,
	less = 94,
	F11 = 95,
	F12 = 96,
	KP_Enter = 104,
	Control_R = 105,
	KP_Divide = 106,
	Print = 107,
	Home = 110,
	Up = 111,
	Left = 113,
	Right = 114,
	End = 115,
	Down = 116,
	Next = 117,
	Insert = 118,
	Delete = 119,
};
