#include "Inputclass.h"

InputClass::InputClass()
{
    m_directInput = 0;
    m_keyboard = 0;
    m_mouse = 0;
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
    HRESULT result;


    // Store the screen size which will be used for positioning the mouse cursor.
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Initialize the location of the mouse on the screen.
    m_mouseX = 0;
    m_mouseY = 0;

    // Initialize the main direct input interface.
    result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
    if (FAILED(result))
    {
        return false;
    }
#pragma region Keyboard
    // Initialize the direct input interface for the keyboard.
    result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Set the data format.  In this case since it is a keyboard we can use the predefined data format.
    result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(result))
    {
        return false;
    }

    // Set the cooperative level of the keyboard to not share with other programs.
    result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
    if (FAILED(result))
    {
        return false;
    }

    // Now acquire the keyboard.
    result = m_keyboard->Acquire();
    if (FAILED(result))
    {
        return false;
    }
#pragma endregion
#pragma region Mouse
    // Initialize the direct input interface for the mouse.
    result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
    if (FAILED(result))
    {
        return false;
    }

    // Set the data format for the mouse using the pre-defined mouse data format.
    result = m_mouse->SetDataFormat(&c_dfDIMouse);
    if (FAILED(result))
    {
        return false;
    }
    // Set the cooperative level of the mouse to share with other programs.
    result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(result))
    {
        return false;
    }
    // Acquire the mouse.
    result = m_mouse->Acquire();
    if (FAILED(result))
    {
        return false;
    }
#pragma endregion

    return true;
}

void InputClass::Shutdown()
{
    // Release the mouse.
    if (m_mouse)
    {
        m_mouse->Unacquire();
        m_mouse->Release();
        m_mouse = 0;
    }

    // Release the keyboard.
    if (m_keyboard)
    {
        m_keyboard->Unacquire();
        m_keyboard->Release();
        m_keyboard = 0;
    }

    // Release the main interface to direct input.
    if (m_directInput)
    {
        m_directInput->Release();
        m_directInput = 0;
    }

    return;
}

bool InputClass::Frame()
{
    bool result;


    // Read the current state of the keyboard.
    result = ReadKeyboard();
    if (!result)
    {
        return false;
    }

    // Read the current state of the mouse.
    result = ReadMouse();
    if (!result)
    {
        return false;
    }

    // Process the changes in the mouse and keyboard.
    ProcessInput();

    return true;
}

bool InputClass::ReadKeyboard()
{
    HRESULT result;


    // Read the keyboard device.
    result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
    if (FAILED(result))
    {
        // If the keyboard lost focus or was not acquired then try to get control back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_keyboard->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}
bool InputClass::ReadMouse()
{
    HRESULT result;


    // Read the mouse device.
    result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
    if (FAILED(result))
    {
        // If the mouse lost focus or was not acquired then try to get control back.
        if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
        {
            m_mouse->Acquire();
        }
        else
        {
            return false;
        }
    }

    return true;
}

void InputClass::ProcessInput()
{
#pragma region Mosue
    // Update the location of the mouse cursor based on the change of the mouse location during the frame.
    m_mouseX += m_mouseState.lX;
    m_mouseY += m_mouseState.lY;

    // Ensure the mouse location doesn't exceed the screen width or height.
    if (m_mouseX < 0) { m_mouseX = 0; }
    if (m_mouseY < 0) { m_mouseY = 0; }

    if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
    if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
#pragma endregion

    return;
}

void InputClass::GetMouseLocation(OUT int& mouseX, OUT int& mouseY)
{
    mouseX = m_mouseX;
    mouseY = m_mouseY;
    return;
}

unsigned char InputClass::GetPressedKeyCode()
{
    for (int i = 0; i < sizeof(m_keyboardState); ++i) {
        if (IsKeyPressed(i)) {
            return static_cast<unsigned char>(i);
        }
    }
    return 0;
}

char InputClass::ConvertKeyToAscii(unsigned char keycode) {
    static std::unordered_map<unsigned char, char> keyMap = {
        { DIK_A, 'a' }, { DIK_B, 'b' }, { DIK_C, 'c' }, { DIK_D, 'd' },
        { DIK_E, 'e' }, { DIK_F, 'f' }, { DIK_G, 'g' }, { DIK_H, 'h' },
        { DIK_I, 'i' }, { DIK_J, 'j' }, { DIK_K, 'k' }, { DIK_L, 'l' },
        { DIK_M, 'm' }, { DIK_N, 'n' }, { DIK_O, 'o' }, { DIK_P, 'p' },
        { DIK_Q, 'q' }, { DIK_R, 'r' }, { DIK_S, 's' }, { DIK_T, 't' },
        { DIK_U, 'u' }, { DIK_V, 'v' }, { DIK_W, 'w' }, { DIK_X, 'x' },
        { DIK_Y, 'y' }, { DIK_Z, 'z' }, { DIK_0, '0' }, { DIK_1, '1' },
        { DIK_2, '2' }, { DIK_3, '3' }, { DIK_4, '4' }, { DIK_5, '5' },
        { DIK_6, '6' }, { DIK_7, '7' }, { DIK_8, '8' }, { DIK_9, '9' },
        { DIK_SPACE, ' ' }, { DIK_RETURN, '\n' }, { DIK_TAB, '\t' }
        // 필요한 다른 키들을 추가
    };

    bool shiftPressed = IsKeyPressed(DIK_LSHIFT) || IsKeyPressed(DIK_RSHIFT);

    if (keyMap.find(keycode) != keyMap.end()) {
        char asciiChar = keyMap[keycode];
        if (shiftPressed && asciiChar >= 'a' && asciiChar <= 'z') {
            asciiChar = asciiChar - 'a' + 'A'; // 소문자를 대문자로 변환
        }
        return asciiChar;
    }

    return '\0'; // 매핑되지 않은 키는 널 문자 반환
}
