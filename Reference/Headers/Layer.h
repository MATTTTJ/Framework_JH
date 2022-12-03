#pragma once
#include "Base.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;
public:
	list<class CGameObject*>*		Get_GameObject() { return &m_GameObjectList; }
public:
	HRESULT		Add_GameObject(class CGameObject* pGameObject);
	HRESULT		Initialize();
	void		Tick(_double TimeDelta);
	void		Late_Tick(_double TimeDelta);
	list<class CGameObject*>& GetGameObjects() { return m_GameObjectList; }

private:
	list<class CGameObject*>			m_GameObjectList; // ���̾ �°� ������ ���ӿ�����Ʈ���� ������Ʈ �����ִ� Ŭ���� 
	typedef list<class CGameObject*>	GAMEOBJECTS; 

public:
	static CLayer*		Create();
	virtual void		Free() override;
};

END
