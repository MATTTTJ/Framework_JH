#pragma once
#include "Base.h"
BEGIN(Engine)

class CAnimation final : public CBase
{
public:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT			Save_Animation(HANDLE& hFile, DWORD& dwByte);
	HRESULT			Load_Animation(HANDLE& hFile, DWORD& dwByte);
	string&			Get_AnimationName() { return m_strName; }
	_double&		Get_AnimationTickPerSecond() { return m_dTickPerSecond; }
	_bool			Get_AnimFinish(void) { return m_bIsFinished; }
	_bool&			Get_AnimationLoop() { return m_bIsLooping; }
	_float			Get_AnimationProgress() { return _float(m_dPlayTime / m_dDuration); }
	void			Reset_Animation();
public:
	HRESULT			Initialize(aiAnimation* pAIAnimation, class CModel* pModel);

	void			Update_Bones(_double dTimeDelta, const wstring& wstrRootBoneName);
	void			Update_Lerp(_double dTimeDelta, _float fRatio, const wstring& wstrRootBoneName);

public:
	

private:
	class CModel*				m_pModel = nullptr;

	string						m_strName = "";
	_double						m_dDuration = 0.0; // �ִϸ��̼��� ����
	_double						m_dTickPerSecond; // �ִϸ��̼��� ����Ǵ� �ӵ� (�������� ����)
	_double						m_dPlayTime = 0.0; // �ִϸ��̼��� ����� �ð��� ����
	_bool						m_bIsFinished = false;
	_bool						m_bIsLooping = false;
	_bool						m_bIsLerpEnd = false;
	_uint						m_iNumChannels = 0; // �� �ִϸ��̼��� ����ϱ� ���� �����ؾ��ϴ� ����
	vector<class CChannel*>		m_vecChannels;

public:
	static CAnimation*	Create(aiAnimation* pAIAnimation, class CModel* pModel);
	virtual void		Free() override;
};

END