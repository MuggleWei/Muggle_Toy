#include "input.h"
#if MG_PLATFORM_WINDOWS
#include <windows.h>
#endif

NS_MUGGLE_BEGIN

eButtonStatus Input::s_key[(int)eKeyCode::Max];
eButtonStatus Input::s_key_cur_frame[(int)eKeyCode::Max];
vec2i Input::s_mouse_pos;
int16_t Input::s_mouse_wheel;

void Input::Update()
{
	for (int i = 0; i < (int)eKeyCode::Max; ++i)
	{
		s_key_cur_frame[i] = eButtonStatus::Null;
	}

	s_mouse_wheel = 0;
}
bool Input::Initialize()
{
#if MG_PLATFORM_WINDOWS
	if (!VerifyVirtualKey())
	{
		return false;
	}
#endif

	for (int i = 0; i < (int)eKeyCode::Max; ++i)
	{
		s_key[i] = eButtonStatus::Release;
		s_key_cur_frame[i] = eButtonStatus::Null;
	}

	return true;
}

bool Input::GetKey(eKeyCode key_code)
{
	return s_key[(int)key_code] == eButtonStatus::Press;
}
bool Input::GetKeyDown(eKeyCode key_code)
{
	return s_key_cur_frame[(int)key_code] == eButtonStatus::Press;
}
bool Input::GetKeyUp(eKeyCode key_code)
{
	return s_key_cur_frame[(int)key_code] == eButtonStatus::Release;
}

bool Input::GetMouse(eMouseButton button)
{
	return s_key[(int)button + (int)eKeyCode::Mouse0] == eButtonStatus::Press;
}
bool Input::GetMouseDown(eMouseButton button)
{
	return s_key_cur_frame[(int)button + (int)eKeyCode::Mouse0] == eButtonStatus::Press;
}
bool Input::GetMouseUp(eMouseButton button)
{
	return s_key_cur_frame[(int)button + (int)eKeyCode::Mouse0] == eButtonStatus::Release;
}

vec2i Input::GetMousePos()
{
	return s_mouse_pos;
}
int16_t Input::GetMouseWheel()
{
	return s_mouse_wheel;
}

void Input::KeyDown(eKeyCode key_code)
{
	s_key_cur_frame[(int)key_code] = eButtonStatus::Press;
	s_key[(int)key_code] = eButtonStatus::Press;
}
void Input::KeyUp(eKeyCode key_code)
{
	s_key_cur_frame[(int)key_code] = eButtonStatus::Release;
	s_key[(int)key_code] = eButtonStatus::Release;
}

void Input::SetMousePos(const vec2i& pos)
{
	s_mouse_pos = pos;
}
void Input::SetMouseWheel(const int16_t& val)
{
	s_mouse_wheel = val;
}

#if MG_PLATFORM_WINDOWS

enum
{
	VK_MAX = 256,
};

struct VKToKeyCode
{
	int virual_key;
	eKeyCode key_code;
};

// map windows virtual key to key code
static VKToKeyCode s_map_vk_keycode[VK_MAX] = {
	{ 0x00, eKeyCode::Null },
	{ VK_LBUTTON, eKeyCode::Mouse0 },
	{ VK_RBUTTON, eKeyCode::Mouse1 },
	{ VK_CANCEL, eKeyCode::Null },
	{ VK_MBUTTON, eKeyCode::Mouse2 },
	{ VK_XBUTTON1, eKeyCode::Mouse3 },
	{ VK_XBUTTON2, eKeyCode::Mouse4 },
	{ 0x07, eKeyCode::Null },
	{ VK_BACK, eKeyCode::Backspace },
	{ VK_TAB, eKeyCode::Tab },
	{ 0x0A, eKeyCode::Null },
	{ 0x0B, eKeyCode::Null },
	{ VK_CLEAR, eKeyCode::Clear },
	{ VK_RETURN, eKeyCode::Enter },
	{ 0x0E, eKeyCode::Null },
	{ 0x0F, eKeyCode::Null },
	{ VK_SHIFT, eKeyCode::LShift },
	{ VK_CONTROL, eKeyCode::LCtrl },
	{ VK_MENU, eKeyCode::LAlt },
	{ VK_PAUSE, eKeyCode::Pause },
	{ VK_CAPITAL, eKeyCode::CapsLock },
	{ VK_KANA, eKeyCode::IME_Kana },
	{ 0x16, eKeyCode::Null },
	{ VK_JUNJA, eKeyCode::IME_Junja},
	{ VK_FINAL, eKeyCode::IME_final },
	{ VK_HANJA, eKeyCode::IME_Hanja },
	{ 0x1A, eKeyCode::Null },
	{ VK_ESCAPE, eKeyCode::Escape },
	{ VK_CONVERT, eKeyCode::IME_convert },
	{ VK_NONCONVERT, eKeyCode::IME_nonconvert },
	{ VK_ACCEPT, eKeyCode::IME_accept },
	{ VK_MODECHANGE, eKeyCode::IME_mode_change_request },
	{ VK_SPACE, eKeyCode::Space },
	{ VK_PRIOR, eKeyCode::PageUp },
	{ VK_NEXT, eKeyCode::PageDown },
	{ VK_END, eKeyCode::End },
	{ VK_HOME, eKeyCode::Home },
	{ VK_LEFT, eKeyCode::Left },
	{ VK_UP, eKeyCode::Up },
	{ VK_RIGHT, eKeyCode::Right },
	{ VK_DOWN, eKeyCode::Down },
	{ VK_SELECT, eKeyCode::Select },
	{ VK_PRINT, eKeyCode::Paint },
	{ VK_EXECUTE, eKeyCode::Excute },
	{ VK_SNAPSHOT, eKeyCode::PaintScreen },
	{ VK_INSERT, eKeyCode::Insert },
	{ VK_DELETE, eKeyCode::Delete },
	{ VK_HELP, eKeyCode::Help },
	{ 0x30, eKeyCode::Key0 },
	{ 0x31, eKeyCode::Key1 },
	{ 0x32, eKeyCode::Key2 },
	{ 0x33, eKeyCode::Key3 },
	{ 0x34, eKeyCode::Key4 },
	{ 0x35, eKeyCode::Key5 },
	{ 0x36, eKeyCode::Key6 },
	{ 0x37, eKeyCode::Key7 },
	{ 0x38, eKeyCode::Key8 },
	{ 0x39, eKeyCode::Key9 },
	{ 0x3A, eKeyCode::Null },
	{ 0x3B, eKeyCode::Null },
	{ 0x3C, eKeyCode::Null },
	{ 0x3D, eKeyCode::Null },
	{ 0x3E, eKeyCode::Null },
	{ 0x3F, eKeyCode::Null },
	{ 0x40, eKeyCode::Null },
	{ 0x41, eKeyCode::A },
	{ 0x42, eKeyCode::B },
	{ 0x43, eKeyCode::C },
	{ 0x44, eKeyCode::D },
	{ 0x45, eKeyCode::E },
	{ 0x46, eKeyCode::F },
	{ 0x47, eKeyCode::G },
	{ 0x48, eKeyCode::H },
	{ 0x49, eKeyCode::I },
	{ 0x4A, eKeyCode::J },
	{ 0x4B, eKeyCode::K },
	{ 0x4C, eKeyCode::L },
	{ 0x4D, eKeyCode::M },
	{ 0x4E, eKeyCode::N },
	{ 0x4F, eKeyCode::O },
	{ 0x50, eKeyCode::P },
	{ 0x51, eKeyCode::Q },
	{ 0x52, eKeyCode::R },
	{ 0x53, eKeyCode::S },
	{ 0x54, eKeyCode::T },
	{ 0x55, eKeyCode::U },
	{ 0x56, eKeyCode::V },
	{ 0x57, eKeyCode::W },
	{ 0x58, eKeyCode::X },
	{ 0x59, eKeyCode::Y },
	{ 0x5A, eKeyCode::Z },
	{ VK_LWIN, eKeyCode::LNatural },
	{ VK_RWIN, eKeyCode::RNatural },
	{ VK_APPS, eKeyCode::LNatural },
	{ 0x5E, eKeyCode::Null },
	{ VK_SLEEP, eKeyCode::Sleep },
	{ VK_NUMPAD0, eKeyCode::Number0 },
	{ VK_NUMPAD1, eKeyCode::Number1 },
	{ VK_NUMPAD2, eKeyCode::Number2 },
	{ VK_NUMPAD3, eKeyCode::Number3 },
	{ VK_NUMPAD4, eKeyCode::Number4 },
	{ VK_NUMPAD5, eKeyCode::Number5 },
	{ VK_NUMPAD6, eKeyCode::Number6 },
	{ VK_NUMPAD7, eKeyCode::Number7 },
	{ VK_NUMPAD8, eKeyCode::Number8 },
	{ VK_NUMPAD9, eKeyCode::Number9 },
	{ VK_MULTIPLY, eKeyCode::Multiply },
	{ VK_ADD, eKeyCode::Add },
	{ VK_SEPARATOR, eKeyCode::Separator },
	{ VK_SUBTRACT, eKeyCode::Subtract },
	{ VK_DECIMAL, eKeyCode::Decimal },
	{ VK_DIVIDE, eKeyCode::Divide },
	{ VK_F1, eKeyCode::F1 },
	{ VK_F2, eKeyCode::F2 },
	{ VK_F3, eKeyCode::F3 },
	{ VK_F4, eKeyCode::F4 },
	{ VK_F5, eKeyCode::F5 },
	{ VK_F6, eKeyCode::F6 },
	{ VK_F7, eKeyCode::F7 },
	{ VK_F8, eKeyCode::F8 },
	{ VK_F9, eKeyCode::F9 },
	{ VK_F10, eKeyCode::F10 },
	{ VK_F11, eKeyCode::F11 },
	{ VK_F12, eKeyCode::F12 },
	{ VK_F13, eKeyCode::F13 },
	{ VK_F14, eKeyCode::F14 },
	{ VK_F15, eKeyCode::F15 },
	{ VK_F16, eKeyCode::F16 },
	{ VK_F17, eKeyCode::F17 },
	{ VK_F18, eKeyCode::F18 },
	{ VK_F19, eKeyCode::F19 },
	{ VK_F20, eKeyCode::F20 },
	{ VK_F21, eKeyCode::F21 },
	{ VK_F22, eKeyCode::F22 },
	{ VK_F23, eKeyCode::F23 },
	{ VK_F24, eKeyCode::F24 },
	{ 0x88, eKeyCode::Null },
	{ 0x89, eKeyCode::Null },
	{ 0x8A, eKeyCode::Null },
	{ 0x8B, eKeyCode::Null },
	{ 0x8C, eKeyCode::Null },
	{ 0x8D, eKeyCode::Null },
	{ 0x8E, eKeyCode::Null },
	{ 0x8F, eKeyCode::Null },
	{ VK_NUMLOCK, eKeyCode::NumberLock },
	{ VK_SCROLL, eKeyCode::ScrollLock },
	{ 0x92, eKeyCode::Null },
	{ 0x93, eKeyCode::Null },
	{ 0x94, eKeyCode::Null },
	{ 0x95, eKeyCode::Null },
	{ 0x96, eKeyCode::Null },
	{ 0x97, eKeyCode::Null },
	{ 0x98, eKeyCode::Null },
	{ 0x99, eKeyCode::Null },
	{ 0x9A, eKeyCode::Null },
	{ 0x9B, eKeyCode::Null },
	{ 0x9C, eKeyCode::Null },
	{ 0x9D, eKeyCode::Null },
	{ 0x9E, eKeyCode::Null },
	{ 0x9F, eKeyCode::Null },
	{ VK_LSHIFT, eKeyCode::LShift },
	{ VK_RSHIFT, eKeyCode::RShift },
	{ VK_LCONTROL, eKeyCode::LCtrl },
	{ VK_RCONTROL, eKeyCode::RCtrl },
	{ VK_LMENU, eKeyCode::LAlt },
	{ VK_RMENU, eKeyCode::RAlt },
	{ VK_BROWSER_BACK, eKeyCode::Null },
	{ VK_BROWSER_FORWARD, eKeyCode::Null },
	{ VK_BROWSER_REFRESH, eKeyCode::Null },
	{ VK_BROWSER_STOP, eKeyCode::Null },
	{ VK_BROWSER_SEARCH, eKeyCode::Null },
	{ VK_BROWSER_FAVORITES, eKeyCode::Null },
	{ VK_BROWSER_HOME, eKeyCode::Null },
	{ VK_VOLUME_MUTE, eKeyCode::Null },
	{ VK_VOLUME_DOWN, eKeyCode::Null },
	{ VK_VOLUME_UP, eKeyCode::Null },
	{ VK_MEDIA_NEXT_TRACK, eKeyCode::Null },
	{ VK_MEDIA_PREV_TRACK, eKeyCode::Null },
	{ VK_MEDIA_STOP, eKeyCode::Null },
	{ VK_MEDIA_PLAY_PAUSE, eKeyCode::Null },
	{ VK_LAUNCH_MAIL, eKeyCode::Null },
	{ VK_LAUNCH_MEDIA_SELECT, eKeyCode::Null },
	{ VK_LAUNCH_APP1, eKeyCode::Null },
	{ VK_LAUNCH_APP2, eKeyCode::Null },
	{ 0xB8, eKeyCode::Null },
	{ 0xB9, eKeyCode::Null },
	{ VK_OEM_1, eKeyCode::Semicolon },
	{ VK_OEM_PLUS, eKeyCode::Plus },
	{ VK_OEM_COMMA, eKeyCode::Comma },
	{ VK_OEM_MINUS, eKeyCode::Minus },
	{ VK_OEM_PERIOD, eKeyCode::Period },
	{ VK_OEM_2, eKeyCode::Slash },
	{ VK_OEM_3, eKeyCode::BackQuote },
	{ 0xC1, eKeyCode::Null },
	{ 0xC2, eKeyCode::Null },
	{ 0xC3, eKeyCode::Null },
	{ 0xC4, eKeyCode::Null },
	{ 0xC5, eKeyCode::Null },
	{ 0xC6, eKeyCode::Null },
	{ 0xC7, eKeyCode::Null },
	{ 0xC8, eKeyCode::Null },
	{ 0xC9, eKeyCode::Null },
	{ 0xCA, eKeyCode::Null },
	{ 0xCB, eKeyCode::Null },
	{ 0xCC, eKeyCode::Null },
	{ 0xCD, eKeyCode::Null },
	{ 0xCE, eKeyCode::Null },
	{ 0xCF, eKeyCode::Null },
	{ 0xD0, eKeyCode::Null },
	{ 0xD1, eKeyCode::Null },
	{ 0xD2, eKeyCode::Null },
	{ 0xD3, eKeyCode::Null },
	{ 0xD4, eKeyCode::Null },
	{ 0xD5, eKeyCode::Null },
	{ 0xD6, eKeyCode::Null },
	{ 0xD7, eKeyCode::Null },
	{ 0xD8, eKeyCode::Null },
	{ 0xD9, eKeyCode::Null },
	{ 0xDA, eKeyCode::Null },
	{ VK_OEM_4, eKeyCode::LBracket },
	{ VK_OEM_5, eKeyCode::Backslash },
	{ VK_OEM_6, eKeyCode::RBracket },
	{ VK_OEM_7, eKeyCode::Quote },
	{ VK_OEM_8, eKeyCode::Null },
	{ 0xE0, eKeyCode::Null },
	{ 0xE1, eKeyCode::Null },
	{ VK_OEM_102, eKeyCode::Null },
	{ 0xE3, eKeyCode::Null },
	{ 0xE4, eKeyCode::Null },
	{ VK_PROCESSKEY, eKeyCode::Null },
	{ 0xE6, eKeyCode::Null },
	{ VK_PACKET, eKeyCode::Null },
	{ 0xE8, eKeyCode::Null },
	{ 0xE9, eKeyCode::Null },
	{ 0xEA, eKeyCode::Null },
	{ 0xEB, eKeyCode::Null },
	{ 0xEC, eKeyCode::Null },
	{ 0xED, eKeyCode::Null },
	{ 0xEE, eKeyCode::Null },
	{ 0xEF, eKeyCode::Null },
	{ 0xF0, eKeyCode::Null },
	{ 0xF1, eKeyCode::Null },
	{ 0xF2, eKeyCode::Null },
	{ 0xF3, eKeyCode::Null },
	{ 0xF4, eKeyCode::Null },
	{ 0xF5, eKeyCode::Null },
	{ VK_ATTN, eKeyCode::Null },
	{ VK_CRSEL, eKeyCode::Null },
	{ VK_EXSEL, eKeyCode::Null },
	{ VK_EREOF, eKeyCode::Null },
	{ VK_PLAY, eKeyCode::Null },
	{ VK_ZOOM, eKeyCode::Null },
	{ VK_NONAME, eKeyCode::Null },
	{ VK_PA1, eKeyCode::Null },
	{ VK_OEM_CLEAR, eKeyCode::Null },
	{ 0xFF, eKeyCode::Null },
};

void Input::SetKeyCode_Win(uint16_t vk, const eButtonStatus& status)
{
	MASSERT_MSG(vk < VK_MAX, "virtual key in the range [1, 0xFE]");
	if (status == eButtonStatus::Press)
	{
		KeyDown(s_map_vk_keycode[vk].key_code);
	}
	else
	{
		KeyUp(s_map_vk_keycode[vk].key_code);
	}
}
bool Input::VerifyVirtualKey()
{
	size_t num = sizeof(s_map_vk_keycode) / sizeof(s_map_vk_keycode[0]);
	if (num != VK_MAX)
	{
		MASSERT_MSG(0, "Error in virtual key number");
		return false;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (s_map_vk_keycode[i].virual_key != i)
		{
			MASSERT_MSG(0, "Error in virtual key index 0x%x", i);
			return false;
		}
	}

	return true;
}

#endif

NS_MUGGLE_END