#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CDynamic_Camera final : public CCamera
{
public:
	typedef struct tagCamKeyFrame
	{
		_float		dTime;
	}CAMFRAME;

public:
	CDynamic_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic_Camera(const CDynamic_Camera& rhs);
	virtual ~CDynamic_Camera() = default;

	void					Set_Boss_IntroCam(_matrix camWorld);
	void					Set_LobbyCam(_bool trueIsLobby) { m_bLobby = trueIsLobby; }
	virtual HRESULT			Initialize_Prototype()		 override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta)		 override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render()					 override;

public:
	HRESULT					SetUp_Components();


private:
	_bool					m_bLobby = false;
	_float					m_fCurPlayTime = 0.f;
	vector<CAMFRAME>		m_vecCamKeyFrame;
	_uint				m_iCurrentKeyframeIndex = 0; // 현재 키 프레임 인덱스

	_float3					m_vFinalScale;
	_float3					m_vFinalPos;
	_float4					m_vFinalRotation;

	_float3					m_vInitScale;
	_float3					m_vInitPos;
	_float4					m_vInitRotation;

	_bool					m_bArrivedCam = false;

public:
	static CDynamic_Camera*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) override;
	virtual void			Free() override;
};

END