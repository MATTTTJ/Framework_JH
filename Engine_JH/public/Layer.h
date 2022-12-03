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
	list<class CGameObject*>			m_GameObjectList; // 레이어에 맞게 보관된 게임오브젝트들을 업데이트 돌려주는 클래스 
	typedef list<class CGameObject*>	GAMEOBJECTS; 

public:
	static CLayer*		Create();
	virtual void		Free() override;
};

END
