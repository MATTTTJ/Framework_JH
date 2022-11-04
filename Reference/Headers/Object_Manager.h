#pragma once
#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)

private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT	Reserve_Manager(_uint iNumLevels); // 레벨을 받아온다.

public:
	// 원본 객체를 추가하는 행위 
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	// 사본 객체를 추가하는 행위
	HRESULT	Clone_GameObject(_uint iLevelIndex, const _tchar* pLayerTag,
		const _tchar* pPrototypeTag, void* pArg = nullptr);
	// 어느 레벨에서, 어떤 레벨 Key를 가진, 어떤 원본 Key를 가진, 어떤 포인터를 가진

private:
	// 원본 객체들을 모아놓음
	map<const _tchar*, class CGameObject*>	m_Prototypes;
	typedef map<const _tchar*, class CGameObject*> PROTOTYPES;

private:
	// 사본 객체들을 모아놓음
	map<const _tchar*, class CLayer*>*				m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>		LAYERS;
	_uint									m_iNumLevels = 0;

private:
	class CGameObject*		Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer*			Find_Layer(const _tchar* pLayer);

public:
	virtual void	Free() override;
};

END