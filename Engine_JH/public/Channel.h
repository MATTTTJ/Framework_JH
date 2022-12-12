#pragma once
#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT				Save_Channel(HANDLE& hFile, DWORD& dwByte);
	HRESULT				Load_Channel(HANDLE& hFile, DWORD& dwByte);

public:
	HRESULT				Initialize(aiNodeAnim* pAIChannel, class CModel* pModel);
	void				Update_TransformMatrix(_double dPlayTime);
	void				Reset_KeyFrameIndex() { m_iCurrentKeyframeIndex = 0; }
private:
	class CModel*		m_pModel = nullptr;

	string				m_strName = "";
	class CBone*		m_pBone = nullptr;

	_uint				m_iNumKeyframes = 0;		// 특정 애니메이션의 키프레임 갯수 
	vector<KEYFRAME>	m_vecKeyframes;				// 애니메이션의 키 프레임 데이터를 보관
	typedef vector<KEYFRAME>		KEYFRAMES;

	_uint				m_iCurrentKeyframeIndex = 0; // 현재 키 프레임 인덱스


public:
	static CChannel*	Create(aiNodeAnim* pAIChannel, class CModel* pModel);
	virtual void		Free() override;
};

END