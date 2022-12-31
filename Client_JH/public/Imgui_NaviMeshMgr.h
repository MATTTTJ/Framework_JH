#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
END


BEGIN(Client)
class CImgui_NaviMeshMgr final : public CImguiObject
{
public:
	CImgui_NaviMeshMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_NaviMeshMgr() = default;

public:
		virtual HRESULT Initialize(void* pArg) override;
		virtual void	Imgui_RenderWindow() override;

		void			Picking_On_Mesh(void);

private:
	void				Cell_Sorting(void);

private:
	ID3D11Device*						m_pDevice = nullptr;
	ID3D11DeviceContext*				m_pContext = nullptr;
	CGameInstance*						m_pGameInstance = nullptr;

	_float4								m_vPickingPos = _float4(0.f, 0.f, 0.f, 1.f);
	_int								m_iCubeIndex = -1;

private:
	_uint								m_iCurPoint = 0;
	_float3								m_vPoint[3];
	list<_float3>						m_PointList;
	vector<CGameObject*>				m_vecPointCube;

public:
	static CImgui_NaviMeshMgr*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
	virtual void						Free() override;
};

END