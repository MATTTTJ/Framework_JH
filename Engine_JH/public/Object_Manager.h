#pragma once

#include "Base.h"
#include "Layer.h"
/* 게임내에 필요한 객체들을 내 기준(CLayer)에 따라 나누어 보관한다. */
/* 객체들을 나누어 저장하고 있는 CLayer들을 보관하는 클래스이다. */
/* 모든 객체들의 갱신(Tick, Late_Tick)을 담당한다. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	map<const wstring, class CGameObject*>*	Get_Prototypes() { return &m_mapPrototypes; }
	map<const wstring, class CLayer*>*		Get_Layers(_uint iLevelIndex) { return &m_pLayers[iLevelIndex]; }
	list<class CGameObject*>*				Get_CloneObjectList(_uint iLevelIndex, const wstring& wstrLayerTag) { return Find_Layer(iLevelIndex, wstrLayerTag)->Get_GameObject(); }

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Clear(_uint iLevelIndex);
public:
	HRESULT	Add_Layer(_uint iLevelIndex, const wstring& wstrLayerTag);
	HRESULT Add_Prototype(const wstring& pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Clone_GameObject(_uint iLevelIndex, const wstring& pLayerTag, const wstring& wstrPrototypeTag, void* pArg = nullptr);
	HRESULT Clone_GameObject(_uint iLevelIndex, const wstring& pLayerTag, const wstring& wstrPrototypeTag, _float4x4 WorldMatrix, void* pArg = nullptr);
	void	Tick(_double TimeDelta);
	void	Late_Tick(_double TimeDelta);

public:
	void Imgui_ProtoViewer(OUT const _tchar*& szSelectedProto);

	void Imgui_ObjectViewer(_uint iLevel, OUT CGameObject*& pSelectedObject);

private: /* For Prototype Objects. */
	map<const wstring, class CGameObject*>			m_mapPrototypes;
	typedef map<const wstring, class CGameObject*>	PROTOTYPES;

private: /* For Cloned Objects. */
	map<const wstring, class CLayer*>*			m_pLayers = nullptr;
	typedef map<const wstring, class CLayer*>	LAYERS;
	_uint										m_iNumLevels = 0;

private:
	class CGameObject*	Find_Prototype(const wstring& pPrototypeTag);
	class CLayer*		Find_Layer(_uint iLevelIndex, const wstring& pLayerTag);

public:
	virtual void Free() override;
};

END