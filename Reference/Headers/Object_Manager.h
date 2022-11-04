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
	HRESULT	Reserve_Manager(_uint iNumLevels); // ������ �޾ƿ´�.

public:
	// ���� ��ü�� �߰��ϴ� ���� 
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	// �纻 ��ü�� �߰��ϴ� ����
	HRESULT	Clone_GameObject(_uint iLevelIndex, const _tchar* pLayerTag,
		const _tchar* pPrototypeTag, void* pArg = nullptr);
	// ��� ��������, � ���� Key�� ����, � ���� Key�� ����, � �����͸� ����

private:
	// ���� ��ü���� ��Ƴ���
	map<const _tchar*, class CGameObject*>	m_Prototypes;
	typedef map<const _tchar*, class CGameObject*> PROTOTYPES;

private:
	// �纻 ��ü���� ��Ƴ���
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