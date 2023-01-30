#pragma once
#include "Base.h"
#include "Camera.h"

BEGIN(Engine)

class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)
private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	void Tick();

public:
	HRESULT Add_Camera(_uint iLevelIndex, const wstring pLayerTag, const wstring pPrototypeTag, CCamera::CAMERADESC& tCameraDesc);
	void Change_Camera();

	_matrix Get_Transform(_uint iIndex, _uint iState);
	_float4 Get_CamPosition(_uint iIndex);

	CCamera::CAMERADESC Get_CameraDesc();
	void	Set_Far(_float fFar) { m_fFar = fFar; }

	void Clear();

private:
	void	Set_Cascade();
private:
	vector<CCamera*> m_vecCamera;
	_uint m_iIndex = 0;

	_float		m_fFar = 6.f;

public:
	virtual void Free() override;
	
};


END