#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CState;
class CModel;
END

BEGIN(Client)

class CMonster abstract : public CGameObject
{

public:
	enum MONSTERCOLLTYPE { COLLTYPE_DETECTED, COLLTYPE_HITHEAD, COLLTYPE_HITBODY, COLLTYPE_ATTPOS, COLLTYPE_END };

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	void					Set_Player(class CPlayer* pPlayer) { m_pPlayer = pPlayer; }

public:
	_matrix					Get_BoneMatrix(const string& strBoneName);
	_vector					Get_TransformState(CTransform::STATE eState);
	_matrix					Get_CombindMatrix(const string& strBoneName);

	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void			Collider_Tick(_double TimeDelta) {}
	virtual void			Collider_Late_Tick(_double TimeDelta) {} 
	virtual void			Set_On_NaviMesh() {}

public:
	virtual void			Collision_Event(class CPlayer* pPlayer) {} // 구현해도되고 안해도되고 

protected:
	CModel*					m_pModelCom = nullptr;
	class CPlayer*			m_pPlayer = nullptr;
	CState*					m_pState = nullptr;
	CCollider*				m_pColliderCom[COLLTYPE_END] = { nullptr };

	_bool					m_bPlayerDetected = false;
	

protected:
	vector<CGameObject*>	m_vecMonsterUI;

public:
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE; // 필수 구현
	virtual void			Free() override;
};

END