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


