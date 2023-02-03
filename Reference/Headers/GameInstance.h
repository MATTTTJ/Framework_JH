#pragma once

/* 싱글톤.  */
/* 클라이언트와 엔진의 소통을 위해 만들어진 클래스. */

#include "Base.h"
#include "Input_Device.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "Graphic_Device.h"
#include "FontMgr.h"
#include "Camera_Manager.h"

#define CONTEXT_LOCK CContext_LockGuard _CtxLock_(CGraphic_Device::GetInstance()->GetContexMtx());

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	static _uint	Get_StaticLevelIndex() { return m_iStaticLevelIndex; }
	HWND GetHWND() { return m_hWnd; }

public:
	static const wstring m_wstrPrototypeTransformTag;

public: /* For.GameInstance */
	HRESULT				Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	void				Tick_Engine(_double TimeDelta);
	void				Clear_Level(_uint iLevelIndex);

public: /* For.Graphic_Device */
	HRESULT				Clear_Graphic_Device(const _float4* pColor);
	HRESULT				Present();
	HRESULT				Update_SwapChain(HWND hWnd, _uint iWinCX, _uint iWinCY, _bool bIsFullScreen, _bool bNeedUpdate);
	D3D11_VIEWPORT	    Get_ViewPort();
public: /* For.Input_Device */
	_byte				Get_DIKeyState(_ubyte byKeyID);
	_byte				Get_DIMouseState(MOUSEKEYSTATE byMouseID);
	_long				Get_DIMouseMove(MOUSEMOVESTATE eMoveState);
	_bool				Mouse_Down(MOUSEKEYSTATE MouseButton);
	_bool				Mouse_Up(MOUSEKEYSTATE MouseButton);
	_bool				Mouse_DoubleClick(MOUSEKEYSTATE MouseButton);
	_bool				Mouse_Pressing(MOUSEKEYSTATE MouseButton);
	_bool				Key_Pressing(_ubyte byKeyID);
	_bool				Key_Down(_ubyte byKeyID);
	_bool				Key_DoubleDown(_ubyte byKeyID);
	_bool				Key_Up(_ubyte byKeyID);
	_bool				Key_Charge(_ubyte byKeyID, _double dTime);
	void				Reset_EveryKey(_double dTimeDelta);
public: /* For.Level_Manager */
	HRESULT				Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	HRESULT				Render_Level();
	const _uint&		Get_CurLevelIndex();

public: /* For.Object_Manager */
	map<const wstring, class CGameObject*>*	Get_Prototypes();
	map<const wstring, class CLayer*>*		Get_Layers(_uint iLevelIndex);
	list<class CGameObject*>*				Get_CloneObjectList(_uint iLevelIndex, const wstring& szLayerTag);
	vector<class CGameObject*>*				Get_AnimObject();
	HRESULT				Add_Prototype(const wstring& pPrototypeTag, class CGameObject* pPrototype);
	HRESULT				Add_Layer(const _uint iLevelIndex, const wstring& wstrLayerTag);
	HRESULT				Add_AnimObject(class CGameObject* pAnimObject);
	HRESULT				Clone_GameObject(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pPrototypeTag, void* pArg = nullptr);
	HRESULT				Clone_GameObject(_uint iLevelIndex, const wstring& wstrLayerTag, const wstring& wstrPrototypeTag, _float4x4 matWorld, void* pArg = nullptr);
	CGameObject*		Clone_GameObject(const wstring& wstrPrototypeTag, void* pArg = nullptr);
	class CComponent*	Get_ComponentPtr(_uint iLevelIndex, const wstring& wstrLayerTag, const wstring& wstrComponentTag, _uint iIndex = 0);
	class CGameObject*	Clone_GameObjectReturnPtr_M(_uint iLevelIndex, const wstring& wstrLayerTag, const wstring& wstrPrototypeTag, _float4x4 matWorld = XMMatrixIdentity(),void* pArg = nullptr);
	class CGameObject*	Clone_GameObjectReturnPtr(_uint iLevelIndex, const wstring& wstrLayerTag, const wstring& wstrPrototypeTag,  void* pArg = nullptr);

public: // For CameraManager
	HRESULT				Add_Camera(_uint iLevelIndex, const wstring& pLayerTag, const wstring& pPrototypeTag, CCamera::CAMERADESC& tCameraDesc);
	void				Change_Camera();
	_matrix				Get_Transform(_uint iIndex, _uint iState);
	_float4				Get_CamPosition(_uint iIndex);
	void				Clear_Camera();
	CCamera::CAMERADESC Get_CameraDesc();
	void				Set_Far(_float fFar);


public: /* For.Component_Manager */
	map<const wstring, class CComponent*>* Get_PrototypeComponents();
	HRESULT				Add_Prototype(_uint iLevelIndex, const wstring& pPrototypeTag, class CComponent* pPrototype);
	class CComponent*	Clone_Component(_uint iLevelIndex, const wstring& pPrototypeTag, class CGameObject* pOwner, void* pArg = nullptr);

public:/* For.PipeLine*/
	_matrix				Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4			Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE eState);
	_matrix				Get_TransformMatrix_Inverse(CPipeLine::TRANSFORMSTATE eState);
	void				Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix);
	_float4				Get_CamPos();
	_float4				Get_CamLook();
public: /* For. Timer_Manager*/
	_double				Get_TimeDelta(const wstring pTimerTag);
	HRESULT				Ready_Timer(const wstring pTimerTag);
	void				Update_Timer(const wstring pTimaerTag);

public: /* For. Light_Manager*/
	const LIGHTDESC*	Get_LightDesc(_uint iIndex);
	HRESULT				Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	_float4x4			Get_LightTransform(_uint iIndex, _uint eState); //1. 뷰, 2. 투영
	_float4x4			Get_LightTransform_TP(_uint iIndex, _uint eState); //1. 뷰, 2. 투영
	void				Set_LightPos(_uint iIndex, _fvector vPos);
	void				Set_LightRange(_uint iIndex, _float fRange);
	void				Set_LightDirection(_uint iIndex, _float4 vDirection);
	HRESULT				Delete_Light(_uint iLightIndex);
	void				Clear_Lights();
	_bool&				Set_LightEnable(_uint iIndex);

public: /* For. Font_Manager*/
	HRESULT				Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT				Render_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPos, _float fRadian, _float2 vScale, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f));

public: // for imgui manager
	void				Render_ImGui();
	void				Render_Update_ImGui();
	void				Add_ImguiTabObject(class CImguiObject* ImguiObject);
	void				Add_ImguiWindowObject(class CImguiObject* ImguiObject);
	void				Clear_ImguiObjects();

public: /* For.Frustum */
	_bool isInFrustum_WorldSpace(_fvector vWorldPos, _float fRange = 0.f);
	_bool isInFrustum_LocalSpace(_fvector vLocalPos, _float fRange = 0.f);
private:
	class CGraphic_Device*			m_pGraphic_Device = nullptr;
	class CInput_Device*			m_pInput_Device = nullptr;
	class CLevel_Manager*			m_pLevel_Manager = nullptr;
	class CObject_Manager*			m_pObject_Manager = nullptr;
	class CComponent_Manager*		m_pComponent_Manager = nullptr;
	class CPipeLine*				m_pPipeLine = nullptr;
	class CTimer_Manager*			m_pTimer_Manager = nullptr;
	class CLight_Manager*			m_pLight_Manager = nullptr;
	class CFontMgr*					m_pFont_Manager = nullptr;
	class CFrustum*					m_pFrustum = nullptr;
	class CTarget_Manager*			m_pTarget_Manager = nullptr;
	class CImgui_Manager*			m_pImgui_Manager = nullptr;
	class CCamera_Manager*			m_pCamera_Manager = nullptr;
private:
	static _uint					m_iStaticLevelIndex;

public:
	static void Release_Engine();

private:
	HWND m_hWnd;

public:
	virtual void Free() override;
};

END