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

public:
	HRESULT			Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	void			Update_Bones(_double TimeDelta);
private:
	class CModel*				m_pModel = nullptr;

	string						m_strName = "";
	_double						m_Duration = 0.0; // �ִϸ��̼��� ����
	_double						m_dTIckPerSecond; // �ִϸ��̼��� ����Ǵ� �ӵ� (�������� ����)
	_double						m_dPlayTime = 0.0; // �ִϸ��̼��� ����� �ð��� ����
	_bool						m_bIsFinished = false;
	_bool						m_bIsLooping = false; 
	_uint						m_iNumChannels = 0; // �� �ִϸ��̼��� ����ϱ� ���� �����ؾ��ϴ� ����
	vector<class CChannel*>		m_vecChannels;

public:
	static CAnimation*	Create(aiAnimation* pAIAnimation, class CModel* pModel);
	virtual void		Free() override;
};

END