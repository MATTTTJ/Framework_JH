#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
END


BEGIN(Client)
class CEffect abstract : public CGameObject
{
public:
	enum EffectType { EFFECT_PARTICLE_POINT, EFFECT_PARTICLE_RECT, EFFECT_PARTICLE_MESH, EFFECT_POINT, EFFECT_RECT, EFFECT_MESH, EFFECT_END };

public:
	typedef struct EffectInfo
	{
		_tchar pEffectTag[MAX_PATH] = TEXT("");
		//_uint iTagNum; // ���� ����� �� ����� ����
		_tchar pMeshPrototypeTag[MAX_PATH] = TEXT("");
		//_int iEffectTag;
		//_int iMeshPrototypeTag;

		_float3 vScale; //���� ũ�Ⱚ
		_float3 vRotation; //���� ȸ����
		_float3 vOffsetPos; //���� ��ġ��

		_float fAppearTime; //��Ÿ���� �ð�
		_float fDelTime; //������� �ð�

		_float fFadeInStartTime; //���̵� �� ���۽ð�
		_float fFadeInRatio; //���̵� �� ����
		_float fFadeOutStartTime; //���̵� �ƿ� ���۽ð�
		_float fFadeOutRatio; //���̵� �ƿ� ����

							  //_bool bIsChange; //��ȭ ����
		_float fStartChangeTime;
		_float fEndChangeTime;
		_float fChangeRatio; //��ȭ ����(�ð��� ���� �󸶸�ŭ ���ߴ���)

		_bool bIsSprite; //��������Ʈ �ؽ��� ����
		_float2 vMaxUV; //�ִ� ����(����)
		_float fUVchangeTime; //��������Ʈ ���� �ð�
		_float fUVchangeStartTime; //��������Ʈ ���� ���� �ð�

		_bool bIsBillBoard; //������ ����
		_bool bIsRotation; //ȸ�� ����
		_bool bIsParentRotation; //�θ�� ���� ȸ���ϴ��� ����
		_float3 vRotationToTime; //�ð��� ���� ȸ��

		_float4 vColor; //����Ʈ Į��

		_float2 vUVScale; //������ ũ��(�������� ��)
		_float2 vUVPos; //������ ��ġ(�������� ��)

		EffectType eType; //����Ʈ ����
						  //EffectDetailType eDetailType; //����Ʈ �� ����

		_uint iTextureIndex;
		_uint iPassIndex; //���̴� �н��ε���

		_bool bIsBlur;
		_bool bIsBloom;
		_bool bIsGlow;

		PARTICLEDESC tParticleDesc;
	}EFFECTINFO;


public:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize_Clone(const wstring& wstrPrototypeTag, void* pArg) override;
	virtual void			Tick(_double dTimeDelta) override;
	virtual void			Late_Tick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;



public:
	virtual CEffect*		Start(CTransform* vTransform);
	virtual void			End();
	void					ResetAlphaRatio() { m_fAlphaRatio = 0.f; }

public:
	EFFECTINFO				GetEffectInfo() { return m_tEffect; }

protected:
	CRenderer*						m_pRendererCom = nullptr;
	CTransform*						m_pOwnerTransform = nullptr;

	EFFECTINFO						m_tEffect;

	_float4x4						m_LocalMatrix;
	_float4x4						m_WorldMatrix;
	_float4x4						m_WorldMatrixTP;

	_float							m_fTimeAcc = 0.f;
	_float							m_fRatio = 0.f;

	_float							m_fAlphaRatio = 0.f;

	_float2							m_vSpriteUVPos;

private:
	virtual HRESULT			SetUp_Components();

public:
	virtual CGameObject*	Clone(const wstring& wstrPrototypeTag, void* pArg = nullptr) PURE;
	virtual void			Free() override;
};

END;