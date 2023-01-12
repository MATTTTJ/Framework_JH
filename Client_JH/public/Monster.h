#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
	class CCollider;
class CState;
class CModel;
class CTexture;
END

BEGIN(Client)

class CMonster abstract : public CGameObject
{

public:
	enum TEXTURETYPE { TEXTURE_GLOW, TEXTURE_NORMAL, TEXTURETYPE_END };
	enum MONSTERCOLLTYPE { COLLTYPE_DETECTED, COLLTYPE_HITHEAD, COLLTYPE_HITBODY, COLLTYPE_ATTPOS,	COLLTYPE_ATTRANGE, COLLTYPE_ONAIM, COLLTYPE_END };
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
	virtual void						Get_Damaged() { m_tMonsterOption.MonsterDesc.m_iHP -= 30; }
	virtual MONSTEROPTION	Get_MonsterDesc() { return m_tMonsterOption; }
	virtual CModel*			Get_ModelCom() { return m_pModelCom; }
	virtual	void			Set_HitColor() { m_bHitColor = true; }
	virtual _bool			Get_IsAimOmPlayer() { return m_bIsOnPlayerEyes; }
	virtual CCollider*		Get_CollPtr(MONSTERCOLLTYPE eType) { return m_pColliderCom[eType]; }
	void					Set_HideColl(_bool bHide) {	 m_bHideCollision = bHide; }
	void					Set_Player(class CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void					Set_PlayAnimation(_bool bPlayAnim) { m_bPlayAnimation = bPlayAnim; }
	


public:
	_matrix					Get_BoneMatrix(const string& strBoneName);
	_vector					Get_TransformState(CTransform::STATE eState);
	_matrix					Get_CombindMatrix(const string& strBoneName);
	HRESULT					Ready_UI();



	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double TimeDelta) override;
	virtual void			Late_Tick(_double TimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void			Collider_Tick(_double TimeDelta) {}


	virtual void			Set_On_NaviMesh() {}

public:
	virtual void			Collision_Body(class CBullet* pBullet) {} // 구현해도되고 안해도되고 
	virtual void			Collision_Head(class CBullet* pBullet) {} // 구현해도되고 안해도되고 
	virtual void			Collision_Hide(class CBullet* pBullet) {} // 구현해도되고 안해도되고 
	virtual void			Collision_PlayerEyes() {} // 구현해도되고 안해도되고 


protected:
	CModel*					m_pModelCom = nullptr;
	class CPlayer*			m_pPlayer = nullptr;
	CState*					m_pState = nullptr;
	CCollider*				m_pColliderCom[COLLTYPE_END] = { nullptr };
	CTexture*				m_pTextureCom[TEXTURETYPE_END] = {nullptr};
	_bool					m_bPlayAnimation = true;
	MONSTEROPTION			m_tMonsterOption;

	// State 판단 불변수
	_bool					m_bPlayerDetected = false;
	_bool					m_bPlayerDetectedFar = false;
	_bool					m_bPlayerDetectedClose = false;
	_bool					m_bHideCollision = false;

protected:
	_bool					m_bIsOnPlayerEyes = false;
	_float					m_fOutLineOffset = 0.02f;
	_float4					m_vDefaultOutLineColor = { 0.f, 0.f, 0.f, 1.f };
	_float4					m_vOnAimOutLineColor = { 0.98f, 0.35f, 0.15f, 1.f };
	// _float4					m_vDefaultLimLightColor = }
	// _float4				m_vHitLimLightColor = {}
	_float					m_fCurHitColorOnTime = 0.f;
	_float					m_fHitColorOnTime = 0.3f;

	_bool					m_bHitColor = false; 
protected:
	vector<CGameObject*>	m_vecMonsterUI;



public:
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE; // 필수 구현
	virtual void			Free() override;
};

END