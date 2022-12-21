#pragma once
#include "Base.h"

BEGIN(Engine)

class CInput_Device : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	// enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	// enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

private:
	CInput_Device();
	virtual ~CInput_Device() = default;

public:
	_byte		Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; m_bLastInput = byKeyID; }
	_byte		Get_DIMouseState(MOUSEKEYSTATE byMouseID)
	{
		return m_MouseState.rgbButtons[byMouseID];
	}
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMoveState)
	{
		return *(((long*)&m_MouseState) + eMoveState);
	}

public:
	_bool		Mouse_Down(MOUSEKEYSTATE MouseButton);
	_bool		Mouse_Up(MOUSEKEYSTATE MouseButton);
	_bool		Mouse_DoubleClick(MOUSEKEYSTATE MouseButton);
	_bool		Key_Pressing(_ubyte byKeyID);
	_bool		Key_Down(_ubyte byKeyID);
	_bool		Key_Up(_ubyte byKeyID);
	void		Reset_EveryKey();

public:
	HRESULT			Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	void			Invalidate_Input_Device(void);

private:
	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;
	LPDIRECTINPUT8				m_pInputSDK;
	_ubyte			m_bLastInput;
	_byte			m_byKeyState[256];
	DIMOUSESTATE	m_MouseState;
	_bool			m_bKeyState[256];
	_bool			m_bMouseState[3];

	_int			m_iClickedCnt = 0;

public:
	virtual void		Free(void);
};

END