#pragma once
#include "Client_Defines.h"
#include "Base.h"
#include "Human_Sword.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
class CState;
class CModel;
class CNavigation;
class CTransform;
END

BEGIN(Client)
class CHuman_Sword_State final : public CBase
{
public:

	struct ExampleAppLog
	{
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
		bool                AutoScroll;  // Keep scrolling if already at the bottom.

		ExampleAppLog()
		{
			AutoScroll = true;
			Clear();
		}

		void    Clear()
		{
			Buf.clear();
			LineOffsets.clear();
			LineOffsets.push_back(0);
		}

		void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			int old_size = Buf.size();
			va_list args;
			va_start(args, fmt);
			Buf.appendfv(fmt, args);
			va_end(args);
			for (int new_size = Buf.size(); old_size < new_size; old_size++)
				if (Buf[old_size] == '\n')
					LineOffsets.push_back(old_size + 1);
		}

		void    Draw(const char* title, bool* p_open = NULL)
		{
			if (!ImGui::Begin(title, p_open))
			{
				ImGui::End();
				return;
			}

			// Options menu
			if (ImGui::BeginPopup("Options"))
			{
				ImGui::Checkbox("Auto-scroll", &AutoScroll);
				ImGui::EndPopup();
			}

			// Main window
			if (ImGui::Button("Options"))
				ImGui::OpenPopup("Options");
			ImGui::SameLine();
			bool clear = ImGui::Button("Clear");
			ImGui::SameLine();
			bool copy = ImGui::Button("Copy");
			ImGui::SameLine();
			Filter.Draw("Filter", -100.0f);

			ImGui::Separator();
			ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

			if (clear)
				Clear();
			if (copy)
				ImGui::LogToClipboard();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			const char* buf = Buf.begin();
			const char* buf_end = Buf.end();
			if (Filter.IsActive())
			{
				// In this example we don't use the clipper when Filter is enabled.
				// This is because we don't have random access to the result of our filter.
				// A real application processing logs with ten of thousands of entries may want to store the result of
				// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
				for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					if (Filter.PassFilter(line_start, line_end))
						ImGui::TextUnformatted(line_start, line_end);
				}
			}
			else
			{
				// The simplest and easy way to display the entire buffer:
				//   ImGui::TextUnformatted(buf_begin, buf_end);
				// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
				// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
				// within the visible area.
				// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
				// on your side is recommended. Using ImGuiListClipper requires
				// - A) random access into your data
				// - B) items all being the  same height,
				// both of which we can handle since we have an array pointing to the beginning of each line of text.
				// When using the filter (in the block of code above) we don't have random access into the data to display
				// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
				// it possible (and would be recommended if you want to search through tens of thousands of entries).
				ImGuiListClipper clipper;
				clipper.Begin(LineOffsets.Size);
				while (clipper.Step())
				{
					for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
					{
						const char* line_start = buf + LineOffsets[line_no];
						const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
						ImGui::TextUnformatted(line_start, line_end);
					}
				}
				clipper.End();
			}
			ImGui::PopStyleVar();

			if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndChild();
			ImGui::End();
		}
	};

	enum DAMAGEDTYPE { HITHEAD, HITBODY, HIT, HIT_END};

	enum ANIMATION
	{
		SWORD_INCOMBAT_IDLE,
		SWORD_RUN_A, SWORD_RUN_B,
		SWORD_JUSTSTAND,
		SWORD_HITHEAD,
		SWORD_DECTED, SWORD_NODETECTED,
		SWORD_GROUNDSPAWN,
		SWORD_JUMP, SWORD_LAND,
		SWORD_HITBODY,
		SWORD_HIDE_RIGHT, SWORD_HIDE_LEFT,
		SWORD_ATTACK_A, SWORD_ATTACK_B,
		SWORD_ANIMEND 
	};


public:
	CHuman_Sword_State();
	virtual ~CHuman_Sword_State() = default;

public:
	HRESULT						Initialize(CHuman_Sword* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	void						Tick(_double dTimeDelta);
	void						Late_Tick(_double dTimeDelta);
	void						Reset_Damaged();
private:
	HRESULT						SetUp_State_Idle();
	HRESULT						SetUp_State_Run();
	HRESULT						SetUp_State_JustStand();
	HRESULT						SetUp_State_HitBody();
	HRESULT						SetUp_State_No_Detected();
	HRESULT						SetUp_State_Detected();
	HRESULT						SetUp_State_GroundSpawn();
	HRESULT						SetUp_State_Attack_A();
	HRESULT						SetUp_State_Hide();
	HRESULT						SetUp_State_Death();

private:
	class CGameInstance*		m_pGameInstance = nullptr;


private:
	CPlayer*					m_pPlayer = nullptr;
	class CWeapon_State*				m_pPlayerState = nullptr;


	class CHuman_Sword*			m_pMonster = nullptr;
	CState*						m_pState = nullptr;
	CModel*						m_pModelCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CNavigation*				m_pNavigationCom = nullptr;

	_bool						m_bPlayAnimation = true ;
	_bool						m_bDeadOnce = false;
	_bool						m_bFirstState[CHuman_Sword::FIRSTSTATE_END] = { false, false, false };

	_bool						m_bDamaged[HIT_END] = { false, false, false };

	vector<class CBullet*>*		m_vecLiveBullet;
private:
	// vector<class CGameObject*>	m_vecBullet;

private:
	void						Start_Idle(_double dTimeDelta);
	void						Start_Run(_double dTimeDelta);
	void						Start_JustStand(_double dTimeDelta);
	void						Start_Damaged(_double dTimeDelta);
	void						Start_No_Detected(_double dTimeDelta);
	void						Start_Detected(_double dTimeDelta);
	void						Start_GroundSpawn(_double dTimeDelta);
	void						Start_Attack_A(_double dTimeDelta);
	void						Start_Hide(_double dTimeDelta);
	void						Start_Death(_double dTimeDelta);

private:
	void						Tick_Idle(_double dTimeDelta);
	void						Tick_Run(_double dTimeDelta);
	void						Tick_JustStand(_double dTimeDelta);
	void						Tick_Damaged(_double dTimeDelta);
	void						Tick_No_Detected(_double dTimeDelta);
	void						Tick_Detected(_double dTimeDelta);
	void						Tick_GroundSpawn(_double dTimeDelta);
	void						Tick_Attack_A(_double dTimeDelta);
	void						Tick_Hide(_double dTimeDelta);
	void						Tick_Death(_double dTimeDelta);

private:
	void						End_Common(_double TimeDelta);
	void						End_Idle(_double dTimeDelta);
	void						End_Run(_double dTimeDelta);
	void						End_JustStand(_double dTimeDelta);
	void						End_Damaged(_double dTimeDelta);
	void						End_No_Detected(_double dTimeDelta);
	void						End_Detected(_double dTimeDelta);
	void						End_GroundSpawn(_double dTimeDelta);
	void						End_Attack_A(_double dTimeDelta);
	void						End_Hide(_double dTimeDelta);
	void						End_Death(_double dTimeDelta);

private:
	_bool						Animation_Finish();
	_bool						Player_Detected(); // NoDetected -> Detected Anim Play
	_bool						Player_DetectedAndFar(); // Stand State -> Player Detected (Follow)
	// _bool						Player_DetectedAndFar_AnimFinish(); 
	_bool						Player_DetectedAndClose(); // Ground Spawn
	_bool						Player_ShootOnHead();
	_bool						Player_NotDetected(); // When NoDetected (Collider)
	_bool						Is_Damaged();
	_bool						IS_Dead();
	// _bool					Check_IdleFinishCount();
	// _bool					Check_WalkTime();

public:
	static CHuman_Sword_State*	Create(CHuman_Sword* pOwner, CState* pStateMachineCom, CModel* pModel, CTransform* pTransform, CNavigation* pNavigation);
	virtual void				Free() override;
};




END
