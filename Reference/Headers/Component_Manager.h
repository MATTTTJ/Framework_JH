#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Collider.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"	

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	map<const wstring, class CComponent*>* Get_PrototypeComponents() { if (m_mapPrototypes) return m_mapPrototypes; else return nullptr; }
public:
	HRESULT				Reserve_Manager(_uint iNumLevels);
	HRESULT				Add_Prototype(_uint iLevelIndex, const wstring& pPrototypeTag, class CComponent* pPrototype);
	class CComponent*	Clone_Component(_uint iLevelIndex, const wstring& pPrototypeTag, class CGameObject* pOwner, void* pArg = nullptr);
	
private:
	_uint												m_iNumLevels = 0;

	// 레벨별로 원형 컴포넌트(텍스쳐, 매쉬 등)을 보관하려 함.
	map<const wstring, class CComponent*>*				m_mapPrototypes = nullptr;
	typedef map<const wstring, class CComponent*>		PROTOTYPES;

private:
	CComponent* Find_Prototype(_uint iLevelIndex, const wstring& pPrototypeTag);

public:
	virtual void Free() override;
};

END
