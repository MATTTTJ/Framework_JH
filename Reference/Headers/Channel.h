#pragma once
#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT				Initialize(aiNodeAnim* pAIChannel, class CModel* pModel);
	void				Update_TransformMatrix(_double dPlayTime);
	void				Reset_KeyFrameIndex() { m_iCurrentKeyframeIndex = 0; }
private:
	string				m_strName = "";
	class CBone*		m_pBone = nullptr;

	_uint				m_iNumKeyframes = 0;		// Ư�� �ִϸ��̼��� Ű������ ���� 
	vector<KEYFRAME>	m_vecKeyframes;				// �ִϸ��̼��� Ű ������ �����͸� ����
	typedef vector<KEYFRAME>		KEYFRAMES;

	_uint				m_iCurrentKeyframeIndex = 0; // ���� Ű ������ �ε���


public:
	static CChannel*	Create(aiNodeAnim* pAIChannel, class CModel* pModel);
	virtual void		Free() override;
};

END