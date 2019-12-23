#pragma once
#ifndef _GURU_KEYBOARD_EYE_H_
#define _GURU_KEYBOARD_EYE_H_

#include "../gvals.h"
#include "event_base.h"
#include "eye_base.h"

#define KEY_CURRENTLY_PRESSED_MASK (1 << 15)

_GURU_BEGIN

class keyboard_event : public event_base
{
public:
	keyboard_event(short vkey_code) noexcept :
		event_base{},
		_vkey_code{ vkey_code }
	{
		_type = EVENT_TYPE::EVENT_KEYBOARD;
	}

	keyboard_event(const keyboard_event& other) noexcept
		: event_base{ other },
		_vkey_code{ other._vkey_code }
	{
		_type = other._type;
	}

	~keyboard_event() noexcept override
	{
	};

	char character() const noexcept
	{
		return MapVirtualKey(_vkey_code, MAPVK_VK_TO_CHAR);
	}

	short vkey_code() const noexcept
	{
		return _vkey_code;
	}

	std::string vkey_str() const noexcept
	{
		switch (_vkey_code)
		{
		case 0x01: return "Left Mouse";
		case 0x02: return "Right Mouse";
		case 0x03: return "Control-break processing";
		case 0x04: return "Middle Mouse";
		case 0x05: return "X1 Mouse";
		case 0x06: return "X2 Mouse";
		case 0x07: return "Undefined";
		case 0x08: return "Backspace";
		case 0x09: return "Tab";
			// Skip
		case 0x0C: return "Clear";
		case 0x0D: return "Enter";
		case 0x10: return "Shift";
		case 0x11: return "Ctrl";
		case 0x12: return "Alt";
		case 0x13: return "Pause";
		case 0x14: return "Capslock";
			// SKip
		case 0x1B: return "Esc";
		case 0x20: return "Spacebar";
		case 0x21: return "Page Up";
		case 0x22: return "Page Down";
		case 0x23: return "End";
		case 0x24: return "Home";
		case 0x25: return "Left Arrow";
		case 0x26: return "Up Arrow";
		case 0x27: return "Right Arrow";
		case 0x28: return "Down Arrow";
		case 0x29: return "Select";
		case 0x2A: return "Print";
		case 0x2B: return "Execute";
		case 0x2C: return "Print Screen";
		case 0x2D: return "INS";
		case 0x2E: return "Del";
		case 0x2F: return "Help";
			// Skip
		case 0x5B: return "Left Windows";
		case 0x5C: return "Right Windows";
			// Skip
		case 0xA0: return "Left Shift";
		case 0xA1: return "Right Shift";
		case 0xA2: return "Left Control";
		case 0xA3: return "Right Control";

		default:
		{
			std::stringstream stream;
			stream << character();
			return stream.str();
		}
		}
	}

public:
	std::string to_string() const noexcept override
	{
		std::ostringstream stream;
		stream << event_base::to_string();
		stream << "[Keyboard Event] " << " KeyCode: 0x" << std::hex 
			<< std::setw(2) << std::setfill('0') << _vkey_code << " ";
		stream << "Character: " << vkey_str();

		return stream.str();
	}

private:
	short _vkey_code;
};

class keyboard_eye : public eye_base
{
public:
	void operator()() noexcept override
	{
		while (true)
		{
			for (int virtualKeyCode = 0x01; virtualKeyCode <= 0xFE; ++virtualKeyCode)
			{
				short result = GetAsyncKeyState(virtualKeyCode);

				if (result & (KEY_CURRENTLY_PRESSED_MASK > 0))
				{
					keyboard_event ke(virtualKeyCode);
					_signal(ke.to_string());
				}
			}
		}
	}
};

_GURU_END

#endif /* _GURU_KEYBOARD_EYE_H_ */