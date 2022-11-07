#pragma once
#include "Base.h"

// ���� ���� �ʿ��� ��ü���� ����(Layer)�� ���� ����
// ��ü���� ������ �����ϰ� �ִ� Layer�� �����ϴ� Ŭ�����̴�.
// ��� ��ü���� ������Ʈ�� ����Ѵ�. 

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT		Reserve_Manager(_uint iNumLevels); // ������ �޾ƿ´�.
	HRESULT		Clear(_uint iLevelIndex);
public:
	HRESULT		Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT		Clone_GameObject(_uint iLevelIndex, const _tchar* pLayerTag,
		const _tchar* pPrototypeTag, void* pArg = nullptr);

	void		Tick(_double TimeDelta);
	void		Late_Tick(_double TimeDelta);

private:
	// ���� ��ü���� ��Ƴ���
	map<const _tchar*, class CGameObject*>			m_Prototypes;
	typedef map<const _tchar*, class CGameObject*>	PROTOTYPES;

private:
	// �纻 ��ü���� ��Ƴ���
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