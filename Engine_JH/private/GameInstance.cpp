#include "..\public\GameInstance.h"
#include "Graphic_Device.h"
#include "Imgui_Manager.h"
#include "Level_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Timer_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

_uint			CGameInstance::m_iStaticLevelIndex = 0;
const wstring	CGameInstance::m_wstrPrototypeTransformTag = L"Prototype_Component_Transform"; 

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())

	, m_pImgui_Manager(CImgui_Manager::GetInstance())

{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device);

	Safe_AddRef(m_pImgui_Manager);

}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	NULL_CHECK_RETURN(m_pGraphic_Device, E_FAIL);
	NULL_CHECK_RETURN(m_pImgui_Manager, E_FAIL);
	NULL_CHECK_RETURN(m_pInput_Device, E_FAIL);
	NULL_CHECK_RETURN(m_pObject_Manager, E_FAIL);
	NULL_CHECK_RETURN(m_pComponent_Manager, E_FAIL);

	m_hWnd = GraphicDesc.hWnd;

	/* 그래픽 디바이스 초기화. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWindowMode, GraphicDesc.iViewportSizeX, GraphicDesc.iViewportSizeY, ppDeviceOut, ppContextOut)))
		return E_FAIL;

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	/* imgui 초기화 */
	m_pImgui_Manager->Ready_Imgui(GraphicDesc.hWnd, *ppDeviceOut, *ppContextOut);

	/* 입력 디바이스 초기화. */
	if (FAILED(m_pInput_Device->Ready_Input_Device(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* +1개로 예약하는 이유 : 엔진에서 Level_Static을 추가로 제공하기 위해서. */
	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels + 1)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels + 1)))
		return E_FAIL;
	/* 엔진에서 제공하는 스태틱레벨의 인덱스를 저장해준다. */
	/* 클라이언트 개발자가 스태틱 레벨에 컴포넌트 원형을 추가하고싶은 경우에 스태틱레벨인덱스를
	클랑리언트에 보여주기 위해서. */
	m_iStaticLevelIndex = iNumLevels;
	/* 엔진에서 제공하는 CGameObject를 상속받는 객체들이 기본적으로 CTransform컴포넌트를 기본으로 가지고 있게 만들어주기위해
	복제할 수 있는 CTransform의 원형객체를 생성한다. */
	/* 실제 이 원형을 복제하는 루틴 CGameObject의 Initialize함수에서 복제를 담당한다. */
	FAILED_CHECK_RETURN(m_pComponent_Manager->Add_Prototype(m_iStaticLevelIndex, m_wstrPrototypeTransformTag, CTransform::Create(*ppDeviceOut, *ppContextOut)), E_FAIL);
	// if (FAILED(m_pComponent_Manager->Add_Prototype(m_iStaticLevelIndex, m_wstrPrototypeTransformTag, CTransform::Create(*ppDeviceOut, *ppContextOut))))
	// 	return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_double TimeDelta)
{
	NULL_CHECK(m_pInput_Device);
	NULL_CHECK(m_pLevel_Manager);
	NULL_CHECK(m_pObject_Manager);

	/* 입력장치의 상태를 갱신받아온다. */
	m_pInput_Device->Invalidate_Input_Device();

	m_pImgui_Manager->Tick_Imgui();
	
	m_pObject_Manager->Tick(TimeDelta);
	m_pLevel_Manager->Tick(TimeDelta);

	m_pPipeLine->Tick();

	m_pObject_Manager->Late_Tick(TimeDelta);
	m_pLevel_Manager->Late_Tick(TimeDelta);

	m_pInput_Device->Reset_EveryKey();


}

void CGameInstance::Clear_Level(_uint iLevelIndex)
{
	NULL_CHECK(m_pObject_Manager);

	m_pObject_Manager->Clear(iLevelIndex);

}

HRESULT CGameInstance::Clear_Graphic_Device(const _float4 * pColor)
{
	NULL_CHECK_RETURN(m_pGraphic_Device, E_FAIL);

	HRESULT			hr = 0;

	hr = m_pGraphic_Device->Clear_BackBuffer_View(*pColor);
	hr = m_pGraphic_Device->Clear_DepthStencil_View();

	return hr;
}


HRESULT CGameInstance::Present()
{
	NULL_CHECK_RETURN(m_pGraphic_Device, E_FAIL);

	return m_pGraphic_Device->Present();
}

HRESULT CGameInstance::Update_SwapChain(HWND hWnd, _uint iWinCX, _uint iWinCY, _bool bIsFullScreen, _bool bNeedUpdate)
{
	NULL_CHECK_RETURN(m_pGraphic_Device, E_FAIL);

	return m_pGraphic_Device->Update_SwapChain(hWnd, iWinCX, iWinCY, bIsFullScreen, bNeedUpdate);
}

D3D11_VIEWPORT CGameInstance::Get_ViewPort()
{
	return m_pGraphic_Device->Get_ViewPort();
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	NULL_CHECK_RETURN(m_pInput_Device, 0);

	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(CInput_Device::MOUSEKEYSTATE byMouseID)
{
	NULL_CHECK_RETURN(m_pInput_Device, 0);

	return m_pInput_Device->Get_DIMouseState(byMouseID);
}

_long CGameInstance::Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE eMoveState)
{
	NULL_CHECK_RETURN(m_pInput_Device, 0);

	return m_pInput_Device->Get_DIMouseMove(eMoveState);
}

_bool CGameInstance::Mouse_Down(CInput_Device::MOUSEKEYSTATE MouseButton)
{
	NULL_CHECK_RETURN(m_pInput_Device, false);

	return m_pInput_Device->Mouse_Down(MouseButton);
}

_bool CGameInstance::Mouse_Up(CInput_Device::MOUSEKEYSTATE MouseButton)
{
	NULL_CHECK_RETURN(m_pInput_Device, false);

	return m_pInput_Device->Mouse_Up(MouseButton);
}

_bool CGameInstance::Mouse_DoubleClick(CInput_Device::MOUSEKEYSTATE MouseButton)
{
	NULL_CHECK_RETURN(m_pInput_Device, false);

	return m_pInput_Device->Mouse_DoubleClick(MouseButton);
}

_bool CGameInstance::Key_Down(_ubyte byKeyID)
{
	NULL_CHECK_RETURN(m_pInput_Device, false);

	return m_pInput_Device->Key_Down(byKeyID);
}

_bool CGameInstance::Key_Up(_ubyte byKeyID)
{
	NULL_CHECK_RETURN(m_pInput_Device, false);

	return m_pInput_Device->Key_Up(byKeyID);
}

void CGameInstance::Reset_EveryKey()
{
	NULL_CHECK_RETURN(m_pInput_Device,);

	return m_pInput_Device->Reset_EveryKey();
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	NULL_CHECK_RETURN(m_pLevel_Manager, E_FAIL);

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

HRESULT CGameInstance::Render_Level()
{
	NULL_CHECK_RETURN(m_pLevel_Manager, E_FAIL);

	return m_pLevel_Manager->Render();
}

const _uint& CGameInstance::Get_CurLevelIndex()
{
	NULL_CHECK_RETURN(m_pLevel_Manager, 9999)

	return m_pLevel_Manager->GetCurLevelIdx();
}

map<const wstring, CGameObject*>* CGameInstance::Get_Prototypes()
{
	NULL_CHECK_RETURN(m_pObject_Manager, nullptr)

	return m_pObject_Manager->Get_Prototypes();
}

map<const wstring, CLayer*>* CGameInstance::Get_Layers(_uint iLevelIndex)
{
	NULL_CHECK_RETURN(m_pObject_Manager, nullptr)

	return m_pObject_Manager->Get_Layers(iLevelIndex);
}

list<CGameObject*>* CGameInstance::Get_CloneObjectList(_uint iLevelIndex, const wstring& szLayerTag)
{
	NULL_CHECK_RETURN(m_pObject_Manager, nullptr)

	return m_pObject_Manager->Get_CloneObjectList(iLevelIndex, szLayerTag);
}

HRESULT CGameInstance::Add_Prototype(const wstring& pPrototypeTag, CGameObject * pPrototype)
{
	NULL_CHECK_RETURN(m_pObject_Manager, E_FAIL);

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_Layer(const _uint iLevelIndex, const wstring& wstrLayerTag)
{
	NULL_CHECK_RETURN(m_pObject_Manager, E_FAIL);

	return m_pObject_Manager->Add_Layer(iLevelIndex, wstrLayerTag);
}

HRESULT CGameInstance::Clone_GameObject(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pPrototypeTag, void * pArg)
{
	NULL_CHECK_RETURN(m_pObject_Manager, E_FAIL);

	return m_pObject_Manager->Clone_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

map<const wstring, CComponent*>* CGameInstance::Get_PrototypeComponents()
{
	NULL_CHECK_RETURN(m_pComponent_Manager, nullptr);

	return m_pComponent_Manager->Get_PrototypeComponents();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& pPrototypeTag, CComponent * pPrototype)
{
	NULL_CHECK_RETURN(m_pComponent_Manager, E_FAIL);

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring& pPrototypeTag, void * pArg)
{
	NULL_CHECK_RETURN(m_pComponent_Manager, nullptr);

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState)
{
	NULL_CHECK_RETURN(m_pPipeLine, XMMatrixIdentity());

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE eState)
{
	NULL_CHECK_RETURN(m_pPipeLine, _float4x4());

	return m_pPipeLine->Get_TransformFloat4x4(eState);
}

_matrix CGameInstance::Get_TransformMatrix_Inverse(CPipeLine::TRANSFORMSTATE eState)
{
	NULL_CHECK_RETURN(m_pPipeLine, XMMatrixIdentity());

	return m_pPipeLine->Get_TransformMatrix_Inverse(eState);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	NULL_CHECK(m_pPipeLine);

	return m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_float4 CGameInstance::Get_CamPos()
{
	NULL_CHECK_RETURN(m_pPipeLine, _float4(0.0f, 0.0f, 0.0f, 0.0f));

	return m_pPipeLine->Get_CamPosition();
}

_double CGameInstance::Get_TimeDelta(const wstring pTimerTag)
{
	NULL_CHECK_RETURN(m_pTimer_Manager, 0.0f);

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Ready_Timer(const wstring pTimerTag)
{
	NULL_CHECK_RETURN(m_pTimer_Manager, E_FAIL);

	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

void CGameInstance::Update_Timer(const wstring pTimaerTag)
{
	NULL_CHECK(m_pTimer_Manager);

	return m_pTimer_Manager->Update_Timer(pTimaerTag);
}

const LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	NULL_CHECK_RETURN(m_pLight_Manager, nullptr);

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	NULL_CHECK_RETURN(m_pLight_Manager, E_FAIL);

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);
}

void CGameInstance::Render_ImGui()
{
	NULL_CHECK(m_pImgui_Manager);

	m_pImgui_Manager->Render_Imgui();
}

void CGameInstance::Render_Update_ImGui()
{
	NULL_CHECK(m_pImgui_Manager);

	m_pImgui_Manager->Render_Update_ImGui();
}

void CGameInstance::Add_ImguiTabObject(CImguiObject* ImguiObject)
{
	NULL_CHECK(m_pImgui_Manager);

	m_pImgui_Manager->Add_ImguiTabObject(ImguiObject);
}

void CGameInstance::Add_ImguiWindowObject(CImguiObject* ImguiObject)
{
	NULL_CHECK(m_pImgui_Manager);

	m_pImgui_Manager->Add_ImguiWindowObject(ImguiObject);
}

void CGameInstance::Clear_ImguiObjects()
{
	NULL_CHECK(m_pImgui_Manager);

	m_pImgui_Manager->Clear_ImguiObjects();
}

void CGameInstance::Release_Engine()
{

	CImgui_Manager::GetInstance()->DestroyInstance();

	CGameInstance::GetInstance()->DestroyInstance();

	CObject_Manager::GetInstance()->DestroyInstance();

	CComponent_Manager::GetInstance()->DestroyInstance();

	CLevel_Manager::GetInstance()->DestroyInstance();

	CInput_Device::GetInstance()->DestroyInstance();

	CPipeLine::GetInstance()->DestroyInstance();

	CLight_Manager::GetInstance()->DestroyInstance();

	CGraphic_Device::GetInstance()->DestroyInstance();

	CTimer_Manager::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{

	Safe_Release(m_pImgui_Manager);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);

}

