#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <unordered_map>
#include <dinput.h>

class InputClass
{
public:
    InputClass();
    InputClass(const InputClass&);
    ~InputClass();

    bool Initialize(HINSTANCE, HWND, int, int);
    void Shutdown();
    bool Frame();

    bool IsEscapePressed() { return IsKeyPressed(DIK_ESCAPE); }
    bool IsMousePressed() { return IsMouseKeyPressed(0); }
    bool IsLeftArrowPressed() { return IsKeyPressed(DIK_LEFT);}
    bool IsRightArrowPressed() { return IsKeyPressed(DIK_RIGHT);}

    void GetMouseLocation(OUT int&, OUT int&);
    unsigned char GetPressedKeyCode();
    char ConvertKeyToAscii(unsigned char keycode);


private:
    __forceinline bool IsKeyPressed(unsigned char keycode) { return m_keyboardState[keycode] & 0x80; }
    __forceinline bool IsMouseKeyPressed(unsigned char idx) { return m_mouseState.rgbButtons[idx] & 0x80; };

    bool ReadKeyboard();
    bool ReadMouse();
    void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};
