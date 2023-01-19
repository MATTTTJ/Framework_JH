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
	const string&		Get_ChannelName() const { return m_strName; }
public:
	HRESULT				Initialize(aiNodeAnim* pAIChannel, class CModel* pModel);
	void				Update_TransformMatrix(_double dPlayTime, const wstring& wstrRootBoneName);
	void				Update_TransformLerpMatrix(_double dPlayTime, _float fRatio, const wstring & wstrRootBoneName);


	// void				Update_Blend(_double dPlayTime, _float fRatio);
	// void				Update_Additive(_double dPlayTime, _float fRatio);
	void				Reset_KeyFrameIndex() { m_iCurrentKeyframeIndex = 0; }
	void				Reset_LerpIndex() { m_iLerpFrameIndex = -1; }

private:
	class CModel*		m_pModel = nullptr;

	string				m_strName = "";
	class CBone*		m_pBone = nullptr;

	_uint				m_iNumKeyframes = 0;		// 특정 애니메이션의 키프레임 갯수 
	vector<KEYFRAME>	m_vecKeyframes;				// 애니메이션의 키 프레임 데이터를 보관
	typedef vector<KEYFRAME>		KEYFRAMES;

	_uint				m_iCurrentKeyframeIndex = 0; // 현재 키 프레임 인덱스
	_vector				m_vLastScale;
	_vector				m_vLastRotation;
	_vector				m_vLastPosition;
	_matrix				m_vLastTransformMatrix;

private:
	_double				m_dLerpRatio = 0.0;
	_int				m_iLerpFrameIndex = -1;

public:
	static CChannel*	Create(aiNodeAnim* pAIChannel, class CModel* pModel);
	virtual void		Free() override;
};

END