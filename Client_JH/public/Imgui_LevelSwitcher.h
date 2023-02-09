#pragma once
#include "Client_Defines.h"
#include "ImguiObject.h"
#include "Effect.h"

class CImgui_LevelSwitcher final : public CImguiObject
{
public:
	enum TAB { TAB_LEVELSWITCH, TAB_SETTING, TAB_EFFECT, TAB_END };

	friend class CEffect;

public:
	CImgui_LevelSwitcher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImgui_LevelSwitcher() = default;

	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Imgui_RenderWindow() override;
	virtual void					Imgui_GotoLogo() override;
	void							Render_LevelSwitch();
	void							Render_ImguiSetting();
	void							Render_EffectTool();
	void							LoadData();
	void							SaveData();


public:
	// 디버그용 
	void							Set_TestInt(_uint iTemp) { m_iTemp = iTemp; }
	void							Set_TestFloat(_float fTemp) { m_fTemp = fTemp; }
	void							Set_TestVector(_float3 vTemp) { m_vTemp = vTemp; }
	// _float 배열 변경 함수 
	void							InputFloat2(_float2* pVector, const char* str);
	void							InputFloat3(_float3* pVector, const char* str);
	void							InputColor(_float4* pVector, const char* str);

	_float m_fRatioX = 335.f;
	_float m_fRatioY = 380.f;
	_float m_fMiniX = 0.24f;
	_float m_fMiniY = 0.52f;

	/* 값 테스트용 변수 */
	_uint m_iTemp;
	_float m_fTemp;
	_float3 m_vTemp;

	// 이펙트 변수
	_uint							m_iEffectTagCnt = 0;
	vector<CEffect::EFFECTINFO>		m_vecEffectInfo;
	vector<string>					m_PrototypeTagList;
	vector<string>					m_EffectTagList;

private:
	ID3D11Device*					m_pDevice;
	ID3D11DeviceContext*			m_pContext;

	_int							m_iCurrentLevel = 0;
	_int							m_iNextLevel = 0;
	char*							m_pLevelName[LEVEL_END] = { "Loading", "Logo", "GamePlay" };

private:
	_float*							m_pAlpha = nullptr;
	TAB								m_eTabId = TAB_END; // 탭 저장용 변수 
public:
	static CImgui_LevelSwitcher*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
	virtual void					Free() override;
};

