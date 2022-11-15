#pragma once
#include "Shader.h"
#include "Renderer.h"
#include "VIBuffer_Rect.h"
#include "Transform.h"
#include "Texture.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT				Reserve_Manager(_uint iNumLevels);
	HRESULT				Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent*	Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);


private:
	_uint												m_iNumLevels = 0;

	// �������� ���� ������Ʈ(�ؽ���, �Ž� ��)�� �����Ϸ� ��.
	map<const _tchar*, class CComponent*>*				m_pPrototypes = nullptr;
	typedef map<const _tchar*, class CComponent*>		PROTOTYPES;

private:
	CComponent* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END
