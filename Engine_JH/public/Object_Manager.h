#pragma once
#include "Base.h"

// 게임 내에 필요한 객체들을 기준(Layer)에 따라 나눔
// 객체들을 나누어 저장하고 있는 Layer를 보관하는 클래스이다.
// 모든 객체들의 업데이트를 담당한다. 

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT		Reserve_Manager(_uint iNumLevels); // 레벨을 받아온다.
	HRESULT		Clear(_uint iLevelIndex);
public:
	HRESULT		Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT		Clone_GameObject(_uint iLevelIndex, const _tchar* pLayerTag,
		const _tchar* pPrototypeTag, void* pArg = nullptr);

	void		Tick(_double TimeDelta);
	void		Late_Tick(_double TimeDelta);

private:
	// 원본 객체들을 모아놓음
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;

private:
	// 사본 객체들을 모아놓음
	map<const _tchar*, class CLayer*>*				m_pLayers = nullptr;
	typedef map<const _tchar*, class CLayer*>		LAYERS;
	_uint											m_iNumLevels = 0;
	
private:
	class CGameObject*		Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer*			Find_Layer(_uint iLevelIndex, const _tchar* pLayer);

public:
	virtual void	Free() override;
};

END