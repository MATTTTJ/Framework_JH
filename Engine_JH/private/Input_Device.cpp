#include "stdafx.h"
#include "Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
	: m_pInputSDK(nullptr), m_pKeyBoard(nullptr), m_pMouse(nullptr)
{
	ZeroMemory(m_byKeyState, sizeof(m_byKeyState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
	ZeroMemory(m_bKeyState, sizeof(_bool) * 256);
	ZeroMemory(m_bPressThisFrame, sizeof(_bool) * 256);
	ZeroMemory(m_dChargeTime, sizeof(_bool) * 256);
	ZeroMemory(m_bMouseState, sizeof(_bool) * 3);
}

_bool CInput_Device::Mouse_Down(MOUSEKEYSTATE MouseButton)
{
	if(!m_bMouseState[MouseButton] && (m_MouseState.rgbButtons[MouseButton] & 0x80))
	{
		m_bMouseState[MouseButton] = true;
		return true;
	}

	return false;
}

_bool CInput_Device::Mouse_Up(MOUSEKEYSTATE MouseButton)
{
	if(m_bMouseState[MouseButton] && !(m_MouseState.rgbButtons[MouseButton] & 0x80))
	{
		m_bMouseState[MouseButton] = true;
		return true;
	}

	return false;
}

_bool CInput_Device::Mouse_DoubleClick(MOUSEKEYSTATE MouseButton)
{
	return _bool();
}

_bool CInput_Device::Key_Pressing(_ubyte byKeyID)
{
	if (Get_DIKeyState(byKeyID) & 0x80)
	{
		m_bKeyState[byKeyID] = true;
		m_bPressThisFrame[byKeyID] = true;
		return true;
	}

	return false;
}

_bool CInput_Device::Key_DoubleDown(_ubyte byKeyID)
{
	if (!m_bPressThisFrame[byKeyID] && !m_bKeyState[byKeyID] && (Get_DIKeyState(byKeyID) & 0x80))
		return true;

	return false;
}

_bool CInput_Device::Key_Down(_ubyte byKeyID)
{
	if (m_bPressThisFrame[byKeyID])
		return true;

	if (!m_bPressThisFrame[byKeyID] && !m_bKeyState[byKeyID] && (Get_DIKeyState(byKeyID) & 0x80))
	{
		m_bKeyState[byKeyID] = true;
		m_bPressThisFrame[byKeyID] = true;
		m_dChargeTime[byKeyID] = 0.0;
		return true;
	}

	return false;
}

_bool CInput_Device::Key_Up(_ubyte byKeyID)
{
	if (!m_bKeyState[byKeyID] && !(Get_DIKeyState(byKeyID) & 0x80))
		return true;

	if (m_bKeyState[byKeyID] && !(Get_DIKeyState(byKeyID) & 0x80))
	{
		m_bKeyState[byKeyID] = false;
		m_bPressThisFrame[byKeyID] = false;
		return true;
	}

	return false;
}

_bool CInput_Device::Key_Charge(_ubyte byKeyID, _double dTime)
{
	if (m_dChargeTime[byKeyID] > dTime)
	{
		m_dChargeTime[byKeyID] = 0.0;

		return true;
	}

	return false;
}

void CInput_Device::Reset_EveryKey(_double dTimeDelta)
{
	/* Reset MouseState */
	for (int i = 0; i < DIM_END; ++i)
	{
		if (m_bMouseState[i] && !(m_MouseState.rgbButtons[i] & 0x80))
			m_bMouseState[i] = false;

		else if (!m_bMouseState[i] && (m_MouseState.rgbButtons[i] & 0x80))
			m_bMouseState[i] = true;
	}

	/* Reset KeyState */
	for (int i = 0; i < 256; ++i)
	{
		if (m_bKeyState[i] && !(Get_DIKeyState(i) & 0x80))
			m_bKeyState[i] = false;

		else if (!m_bKeyState[i] && (Get_DIKeyState(i) & 0x80))
			m_bKeyState[i] = true;

		if (m_bKeyState[i] == true)
			m_dChargeTime[i] += dTimeDelta;
		else
			m_dChargeTime[i] == 0.0;

		m_bPressThisFrame[i] = false;
	}
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;


	// 키보드 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	return S_OK;
}

void CInput_Device::Invalidate_Input_Device(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof m_MouseState, &m_MouseState);
}

void Engine::CInput_Device::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
