#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_DYNAMIC_SHADOWDEPTH, RENDER_NONALPHABLEND, RENDER_OUTLINE,  RENDERER_LIGHTACC, RENDER_NONLIGHT, RENDER_BLUR,RENDER_FADE, RENDER_ALPHABLEND,RENDER_EFFECT, RENDER_UI, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize_Clone(class CGameObject* pOwner, void* pArg) override;

public:
	void				Switch_Collider_Render() { m_bRender_Collider = !m_bRender_Collider; }
	HRESULT				Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT				Add_DebugRenderGroup(class CComponent* pComponent);
	HRESULT				Draw_RenderGroup();

private:
	list<class CGameObject*>			m_RenderObjectList[RENDER_END];
	typedef list<class CGameObject*>	RENDEROBJECTS;

private:
	list<class CComponent*>			m_DebugObject;
	typedef list<class CComponent*>	DEBUGOBJECTS;

private:
	class CTarget_Manager*				m_pTarget_Manager = nullptr;
	class CLight_Manager*				m_pLight_Manager = nullptr;
	class CVIBuffer_Rect*				m_pVIBuffer = nullptr;
	class CShader*						m_pShader = nullptr;
	_float4x4							m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
	_bool								m_bRender_Collider;
private:
	HRESULT				Render_Priority();
	HRESULT				Render_ShadowDepth_Dynamic();
	HRESULT				Render_NonAlphaBlend();
	HRESULT				Render_Effect();
	HRESULT				Render_LightAcc();
	HRESULT				Render_DOF();
	HRESULT				Render_Blend();
	HRESULT				Render_NonLight();
	HRESULT				Render_AlphaBlend();
	HRESULT				Render_UI();
	HRESULT				Render_FADE();


private:
	HRESULT				Render_Blur(const _tchar* pTargetTag);
	HRESULT				Render_Bloom(const _tchar* pTargetTag);
	HRESULT				Render_OutLine();
	HRESULT				Render_ShadowBlur();

#ifdef _DEBUG
private:
	HRESULT				Render_DebugObject();
#endif
public:
	static CRenderer*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg = nullptr) override;
	virtual void		Free() override;
};

END
