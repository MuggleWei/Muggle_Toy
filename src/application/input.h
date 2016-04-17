#ifndef __MUGGLE_INPUT_H__
#define __MUGGLE_INPUT_H__

#include "base/base.h"
#include "math/vector2.h"

NS_MUGGLE_BEGIN

class System;

enum class eKeyCode
{
	// null
	Null = 0,
	// mouse
	Mouse0,	// left
	Mouse1,	// right
	Mouse2,	// middle
	Mouse3,
	Mouse4,
	Mouse5,
	// alphabet
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	// number
	Key0,		// number on the top of the alphanumeric keyboard
	Key1,
	Key2,
	Key3,
	Key4,
	Key5,
	Key6,
	Key7,
	Key8,
	Key9,
	Number0,	// number in Numeric keypad
	Number1,
	Number2,
	Number3,
	Number4,
	Number5,
	Number6,
	Number7,
	Number8,
	Number9,
	// F1~F9
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	// axis
	Up,
	Down,
	Left,
	Right,
	// function
	LShift,
	RShift,
	LCtrl,
	RCtrl,
	LAlt,
	RAlt,
	Tab,
	Clear,
	Enter,
	Escape,
	Space,
	Backspace,
	Pause,
	CapsLock,
	PageUp,
	PageDown,
	End,
	Home,
	Insert,
	Delete,
	Help,
	Select,
	Paint,
	Excute,
	PaintScreen,
	LNatural, // win key board in pc
	RNatural, // win key board in pc
	Sleep,
	Multiply,
	Add,
	Separator,
	Subtract,
	Decimal,
	Divide,
	NumberLock,
	ScrollLock,
	Semicolon,
	Plus,
	Comma,
	Minus,
	Period,
	LBracket,
	RBracket,
	Slash,
	Backslash,
	Quote,
	BackQuote,
	// IME
	IME_Kana,
	IME_Junja,
	IME_final,
	IME_Hanja,
	IME_Kanji,
	IME_convert,
	IME_nonconvert,
	IME_accept,
	IME_mode_change_request,
	Max,
};

enum class eMouseButton
{
	Left = 0,
	Right,
	Middle,
	Max,
};

enum class eButtonStatus
{
	Null = 0,
	Release,
	Press,
	Max,
};

class Input
{
public:
	static void Update();
	static bool Initialize();

	static bool GetKey(eKeyCode key_code);
	static bool GetKeyDown(eKeyCode key_code);
	static bool GetKeyUp(eKeyCode key_code);

	static bool GetMouse(eMouseButton button);
	static bool GetMouseDown(eMouseButton button);
	static bool GetMouseUp(eMouseButton button);

	static vec2i GetMousePos();
	static int16_t GetMouseWheel();

public:
	static void KeyDown(eKeyCode key_code);
	static void KeyUp(eKeyCode key_code);

	static void SetMousePos(const vec2i& pos);
	static void SetMouseWheel(const int16_t& val);

private:
	static eButtonStatus s_key[(int)eKeyCode::Max];
	static eButtonStatus s_key_cur_frame[(int)eKeyCode::Max];
	static vec2i s_mouse_pos;
	static int16_t s_mouse_wheel;

#if MG_PLATFORM_WINDOWS
public:
	// respond for convert windows virtual key code
	static void SetKeyCode_Win(uint16_t vk, const eButtonStatus& status);
	static bool VerifyVirtualKey();
#endif
};

NS_MUGGLE_END

#endif