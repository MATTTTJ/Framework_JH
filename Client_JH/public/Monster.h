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
	enum MONSTERCOLLTYPE { COLLTYPE_DETECTED, COLLTYPE_HITHEAD, COLLTYPE_HITBODY, COLLTYPE_ATTPOS,	COLLTYPE_ATTRANGE, COLLTYPE_END };
	enum FIRSTSTATE { STATE_GROUNDSPAWN, STATE_NODETECTED, STATE_ALREADYSPAWN, FIRSTSTATE_END };

	typedef struct tagMonsterOption
	{
		CGameObject::GAMEOBJECTDESC MonsterDesc;
		_bool		m_bFirstSpawnType[FIRSTSTATE_END] = { false, false, false };
	}MONSTEROPTION;


protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual CCollider*		Get_CollPtr(MONSTERCOLLTYPE eType) { return m_pColliderCom[eType]; }
	void					Set_HideColl(_bool bHide) {	 m_bHideCollision = bHide; }
	void					Set_Player(class CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void					Set_PlayAnimation(_bool bPlayAnim) { m_bPlayAnimation = bPlayAnim; }
	


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


	virtual void			Set_On_NaviMesh() {}

public:
	virtual void			Collision_Body(class CBullet* pBullet) {} // �����ص��ǰ� ���ص��ǰ� 
	virtual void			Collision_Head(class CBullet* pBullet) {} // �����ص��ǰ� ���ص��ǰ� 
	virtual void			Collision_Hide(class CBullet* pBullet) {} // �����ص��ǰ� ���ص��ǰ� 


protected:
	CModel*					m_pModelCom = nullptr;
	class CPlayer*			m_pPlayer = nullptr;
	CState*					m_pState = nullptr;
	CCollider*				m_pColliderCom[COLLTYPE_END] = { nullptr };

	_bool					m_bPlayAnimation = true;
	MONSTEROPTION			m_tMonsterOption;

	// State �Ǵ� �Һ���
	_bool					m_bPlayerDetected = false;
	_bool					m_bPlayerDetectedFar = false;
	_bool					m_bPlayerDetectedClose = false;
	_bool					m_bHideCollision = false;
protected:
	vector<CGameObject*>	m_vecMonsterUI;

public:
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE; // �ʼ� ����
	virtual void			Free() override;
};

END