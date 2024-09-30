#include "spch.h"
#include "CNPCBehavior.h"
#include "CPlayerScript.h"
#include "CNPCUIScript.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>


#include "../Client/CLevelSaveLoad.h"

#ifdef _DEBUG
#include "../Client/Inspector.h"
#include "../Client/CEditorMgr.h"
#endif
#include "CBlurControl.h"

CNPCBehavior::CNPCBehavior()
	: CScript(SCRIPT_TYPE::NPCBEHAVIOR)
{
	AddScriptParam(SCRIPT_PARAM::INT, u8"NPC", &m_Type, 0, 0, u8"0: YuhaCat, 1: Dora, 2: Oliveria,\n3: Grandpa, 4: Cereza, 5: Matriarch, 6: AB");
}

void CNPCBehavior::Begin()
{
	if (m_Type == NPCType::AFTERBOSS)
	{
		CBlurControl* GodRay = static_cast<CBlurControl*>(CLevelMgr::GetInst()->FindObjectByName(L"GodRay")->FindScript((UINT)SCRIPT_TYPE::BLURCONTROL));
		if (GodRay != nullptr)
		{
			GodRay->SetEffect(BlurEffect::TurnOff);
			GodRay->Activate();
		}
	}
}

void CNPCBehavior::Tick()
{
	if (m_Activated)
	{
		switch (m_Type)
		{
		case NPCType::YUHACAT:
		{
			tRenderText tText = {};
			tText.Detail = L"\"야오옹\" ...▼";
			tText.FontSize = 20.f;
			tText.Pos = Vec2(330, 450);
			tText.RGBA = FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText);

			tRenderText tText2 = {};
			tText2.Detail = L"☾ 쓰다듬는다";
			tText2.FontSize = 18.f;
			tText2.Pos = Vec2(330, 490);
			tText2.RGBA = m_SelectIdx == 0 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText2);

			tRenderText tText3 = {};
			tText3.Detail = L"☾ 가던길 간다";
			tText3.FontSize = 18.f;
			tText3.Pos = Vec2(330, 518);
			tText3.RGBA = m_SelectIdx == 1 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText3);

			if (KEY_TAP(KEY::UP))
			{
				--m_SelectIdx;
				if (m_SelectIdx < 0)
					m_SelectIdx = 1;
			}

			if (KEY_TAP(KEY::DOWN))
			{
				++m_SelectIdx;
				m_SelectIdx %= 2;
			}

			if (KEY_TAP(KEY::A))
			{
				Thaw();
			}

			break;
		}
		case NPCType::DORA:
		{
			tRenderText tText = {};
			tText.Detail = L"말시키지 말고 가라...";
			tText.FontSize = 20.f;
			tText.Pos = Vec2(530, 390);
			tText.RGBA = FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText);

			tRenderText tText2 = {};
			tText2.Detail = L"☾ 넵..!";
			tText2.FontSize = 18.f;
			tText2.Pos = Vec2(530, 420);
			tText2.RGBA = m_SelectIdx == 0 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText2);

			tRenderText tText3 = {};
			tText3.Detail = L"☾ 싫어";
			tText3.FontSize = 18.f;
			tText3.Pos = Vec2(530, 439);
			tText3.RGBA = m_SelectIdx == 1 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText3);

			if (KEY_TAP(KEY::UP))
			{
				--m_SelectIdx;
				if (m_SelectIdx < 0)
					m_SelectIdx = 1;
			}

			if (KEY_TAP(KEY::DOWN))
			{
				++m_SelectIdx;
				m_SelectIdx %= 2;
			}

			if (KEY_TAP(KEY::A))
			{
				Vec3& doraPos = Transform()->GetRelativePosRef();
				
				doraPos.z = 3.7f;
				Animator2D()->Play(L"Dora_Look", 10.f, false);
				Animator2D()->Reset();
				
				Thaw();
			}
			
			break;
		}
		case NPCType::OLIVERIA:
		{
			switch (m_DialogIdx)
			{
			case 0:
			{
				tRenderText tText = {};
				tText.Detail = L"\"또 오셨군요...\n이번엔 어떤 보스에게\n도전하시겠습니까?\" ...▼";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(680, 460);
				tText.RGBA = FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				if (KEY_TAP(KEY::A))
				{
					++m_DialogIdx;
				}
				break;
			}
			case 1:
			{
				if (KEY_TAP(KEY::UP))
				{
					--m_SelectIdx;
					if (m_SelectIdx < 0)
						m_SelectIdx = 2;
				}

				if (KEY_TAP(KEY::DOWN))
				{
					++m_SelectIdx;
					m_SelectIdx %= 3;
				}

				tRenderText tText = {};
				tText.Detail = L"보스선택 :";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(700, 450);
				tText.RGBA = FONT_RGBA(255, 55, 55, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				tRenderText tText2 = {};
				tText2.Detail = L"☾ 가리세 악마";
				tText2.FontSize = 20.f;
				tText2.Pos = Vec2(700, 480);
				tText2.RGBA = m_SelectIdx == 0 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText2);

				tRenderText tText3 = {};
				tText3.Detail = L"☾ 검은 고양이";
				tText3.FontSize = 20.f;
				tText3.Pos = Vec2(700, 500);
				tText3.RGBA = m_SelectIdx == 1 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText3);

				tRenderText tText4 = {};
				tText4.Detail = L"☾ 좀 더 생각해보고 올게요";
				tText4.FontSize = 20.f;
				tText4.Pos = Vec2(700, 520);
				tText4.RGBA = m_SelectIdx == 2 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText4);


				if (KEY_TAP(KEY::A))
				{	
					++m_DialogIdx;
				}
				break;
			}
			case 2:
			{

				switch (m_SelectIdx)
				{
				case 0: // 악마
				{
					tRenderText tText = {};
					tText.Detail = L"✨가리세 악마✨";
					tText.FontSize = 20.f;
					tText.Pos = Vec2(700, 450);
					tText.RGBA = FONT_RGBA(255, 55, 55, 255);
					CRenderMgr::GetInst()->AddRenderText(tText);

					tRenderText tText2 = {};
					tText2.Detail = L"\"이녀석이 공격을 시전하기 전\n모이는 이펙트 색깔을\n관찰해보세요\"";
					tText2.FontSize = 20.f;
					tText2.Pos = Vec2(680, 480);
					tText2.RGBA = FONT_RGBA(222, 222, 222, 255);
					CRenderMgr::GetInst()->AddRenderText(tText2);

					break;
				}
				case 1: // 리네스
				{
					tRenderText tText = {};
					tText.Detail = L"✨검정 고양이✨";
					tText.FontSize = 20.f;
					tText.Pos = Vec2(700, 450);
					tText.RGBA = FONT_RGBA(255, 55, 55, 255);
					CRenderMgr::GetInst()->AddRenderText(tText);

					tRenderText tText2 = {};
					tText2.Detail = L"\"보통 고양이가 아니니\n단단히 각오 하셔야 할거에요..\"";
					tText2.FontSize = 20.f;
					tText2.Pos = Vec2(680, 490);
					tText2.RGBA = FONT_RGBA(222, 222, 222, 255);
					CRenderMgr::GetInst()->AddRenderText(tText2);

					break;
				}
				case 2:
				{
					tRenderText tText = {};
					tText.Detail = L"\"언제든 오셔서\n무시무시한 보스들에게\n도전해 주세요\"";
					tText.FontSize = 20.f;
					tText.Pos = Vec2(710, 470);
					tText.RGBA = FONT_RGBA(222, 222, 222, 255);
					CRenderMgr::GetInst()->AddRenderText(tText);
					break;
				}
				}

				if (KEY_TAP(KEY::A))
				{
#ifdef _DEBUG
					Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
					pInspector->SetTargetObject(nullptr);
					pInspector->SetTargetAsset(nullptr);
#endif
					switch (m_SelectIdx)
					{
					case 0:
					case 1:
					{
						CBlurControl* GodRay = static_cast<CBlurControl*>(CLevelMgr::GetInst()->FindObjectByName(L"GodRay")->FindScript((UINT)SCRIPT_TYPE::BLURCONTROL));
						if (GodRay != nullptr)
						{
							GodRay->Activate();
						}
						CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
						if (pPlayer != nullptr)
						{
							CPlayerScript* playerScript = static_cast<CPlayerScript*>(pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
							if (playerScript != nullptr)
								playerScript->ChangeState(PlayerState::IDLE);
						}

						CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
						if (textBox != nullptr)
						{
							CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
							if (uiScript != nullptr)
							{
								uiScript->Deactivate();
							}
						}

						++m_DialogIdx;
						break;
					}
					case 2:
					{
						Thaw();
						break;
					}
					}
				}
				break;
			}
			case 3:
			{
				CBlurControl* GodRay = static_cast<CBlurControl*>(CLevelMgr::GetInst()->FindObjectByName(L"GodRay")->FindScript((UINT)SCRIPT_TYPE::BLURCONTROL));
				if (GodRay != nullptr)
				{
					if (GodRay->IsFinished())
					{
						switch (m_SelectIdx)
						{
						case 0:
						{
							CPlayerManager::GetInst()->SetNextPos(Vec3(608.4f, -255.f, 1.8f));
							CPlayerManager::GetInst()->SetNextCamPos(Vec3(192.52f, 60.68f, 0.f));
							CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\DemonBossTEST.lv");
							ChangeLevel(pLevel, LEVEL_STATE::PLAY);

							Ptr<CSound> curBGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"kohovillage");
							curBGM->Stop();
							break;
						}
						case 1:
						{
							CPlayerManager::GetInst()->SetNextPos(Vec3(-98.97f, -322.f, 1.8f));
							CPlayerManager::GetInst()->SetNextCamPos(Vec3(-142.f, -56.12f, 0.f));
							CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\LinethTEST.lv");
							ChangeLevel(pLevel, LEVEL_STATE::PLAY);

							Ptr<CSound> curBGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"kohovillage");
							curBGM->Stop();
							break;
						}
						}
						m_DialogIdx = 0;
						m_SelectIdx = 0;
						m_Activated = false;
					}
				}
				break;
			}
			}
			
			break;
		}
		case NPCType::GRANDPA:
		{
			switch (m_DialogIdx)
			{
			case 0:
			{
				tRenderText tText = {};
				tText.Detail = L"\"콜록!!콜록! 야오옹...\"\n             ...▼";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(320, 390);
				tText.RGBA = FONT_RGBA(222, 222, 0, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				if (KEY_TAP(KEY::A))
				{
					CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
					if (pPlayer != nullptr)
					{
						CPlayerScript* playerScript = static_cast<CPlayerScript*>(pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
						playerScript->ChangeState(PlayerState::SURPRISED);
					}
					++m_DialogIdx;
				}
				break;
			}
			case 1:
			{
				tRenderText tText = {};
				tText.Detail = L".... ???";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(320, 390);
				tText.RGBA = FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				tRenderText tText2 = {};
				tText2.Detail = L"☾ 쓰다듬는다";
				tText2.FontSize = 18.f;
				tText2.Pos = Vec2(330, 425);
				tText2.RGBA = m_SelectIdx == 0 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText2);

				tRenderText tText3 = {};
				tText3.Detail = L"☾ 가던길 간다";
				tText3.FontSize = 18.f;
				tText3.Pos = Vec2(330, 444);
				tText3.RGBA = m_SelectIdx == 1 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText3);

				if (KEY_TAP(KEY::UP))
				{
					--m_SelectIdx;
					if (m_SelectIdx < 0)
						m_SelectIdx = 1;
				}

				if (KEY_TAP(KEY::DOWN))
				{
					++m_SelectIdx;
					m_SelectIdx %= 2;
				}

				if (KEY_TAP(KEY::A))
				{
					Thaw();
				}
				break;
			}
			}
			break;
		}
		case NPCType::CEREZA:
		{
			switch (m_DialogIdx)
			{
			case 0:
			{
				tRenderText tText = {};
				tText.Detail = L"\"모모! 왔구나!!\n내 앞에 종을 좀 봐!\n수상하지 않아?\" ...▼";
				tText.FontSize = 18.f;
				tText.Pos = Vec2(370, 460);
				tText.RGBA = FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				if (KEY_TAP(KEY::A))
				{
					Animator2D()->Play(L"Cereza_Stretch", 8.f, false);
					Animator2D()->Reset();
					++m_DialogIdx;
				}
				break;
			}
			case 1:
			{
				tRenderText tText = {};
				tText.Detail = L"\"이 종을 치면\n엄청난 일이 일어날지도...?\"";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(360, 460);
				tText.RGBA = FONT_RGBA(255, 55, 55, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				if (Animator2D()->IsFinished())
					Animator2D()->Reset();

				if (KEY_TAP(KEY::A))
				{
					Animator2D()->Play(0, 8.0f, true);
					Thaw();
				}
				break;
			}
			}
			break;
		}
		case NPCType::MATRIARCH:
		{
			switch (m_DialogIdx)
			{
			case 0:
			{
				tRenderText tText = {};
				tText.Detail = L"\"뛰어난 여사제 모모여..\" ...▼";
				tText.FontSize = 18.f;
				tText.Pos = Vec2(530, 360);
				tText.RGBA = FONT_RGBA(255, 255, 0, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				if (KEY_TAP(KEY::A))
				{
					++m_DialogIdx;
				}
				break;
			}
			case 1:
			{
				tRenderText tText = {};
				tText.Detail = L"\"그대의 여정에 축복을 ...▼\"";
				tText.FontSize = 18.f;
				tText.Pos = Vec2(530, 360);
				tText.RGBA = FONT_RGBA(255, 255, 0, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				if (KEY_TAP(KEY::A))
				{
					Thaw();
				}
				break;
			}
			}
			break;
		}
		case NPCType::AFTERBOSS:
		{
			switch (m_DialogIdx)
			{
			case 0:
			{
				tRenderText tText = {};
				tText.Detail = L"\"보스를 물리치고 오셨군요!!\n이번엔 누구에게 도전하시겠습니까?\" \n...▼";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(680, 460);
				tText.RGBA = FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				if (KEY_TAP(KEY::A))
				{
					++m_DialogIdx;
				}
				break;
			}
			case 1:
			{
				if (KEY_TAP(KEY::UP))
				{
					--m_SelectIdx;
					if (m_SelectIdx < 0)
						m_SelectIdx = 2;
				}

				if (KEY_TAP(KEY::DOWN))
				{
					++m_SelectIdx;
					m_SelectIdx %= 3;
				}

				tRenderText tText = {};
				tText.Detail = L"보스선택 :";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(700, 450);
				tText.RGBA = FONT_RGBA(255, 55, 55, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				tRenderText tText2 = {};
				tText2.Detail = L"☾ 가리세 악마";
				tText2.FontSize = 20.f;
				tText2.Pos = Vec2(700, 480);
				tText2.RGBA = m_SelectIdx == 0 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText2);

				tRenderText tText3 = {};
				tText3.Detail = L"☾ 검은 고양이";
				tText3.FontSize = 20.f;
				tText3.Pos = Vec2(700, 500);
				tText3.RGBA = m_SelectIdx == 1 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText3);

				tRenderText tText4 = {};
				tText4.Detail = L"☾ 좀 더 생각해보고 올게요";
				tText4.FontSize = 20.f;
				tText4.Pos = Vec2(700, 520);
				tText4.RGBA = m_SelectIdx == 2 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText4);


				if (KEY_TAP(KEY::A))
				{
					++m_DialogIdx;
				}
				break;
			}
			case 2:
			{

				switch (m_SelectIdx)
				{
				case 0: // 악마
				{
					tRenderText tText = {};
					tText.Detail = L"✨가리세 악마✨";
					tText.FontSize = 20.f;
					tText.Pos = Vec2(700, 450);
					tText.RGBA = FONT_RGBA(255, 55, 55, 255);
					CRenderMgr::GetInst()->AddRenderText(tText);

					tRenderText tText2 = {};
					tText2.Detail = L"이녀석은 근거리 공격\n불뿜기, 그리고 샤우팅(?)\n공격을 한다고 알려져 있어요\n공격 시전 전 모이는 기의 색깔을 보면\n어떤 공격인지 알 수 있답니다.\"";
					tText2.FontSize = 20.f;
					tText2.Pos = Vec2(680, 480);
					tText2.RGBA = FONT_RGBA(222, 222, 222, 255);
					CRenderMgr::GetInst()->AddRenderText(tText2);

					break;
				}
				case 1: // 리네스
				{
					tRenderText tText = {};
					tText.Detail = L"✨검정 고양이✨";
					tText.FontSize = 20.f;
					tText.Pos = Vec2(700, 450);
					tText.RGBA = FONT_RGBA(255, 55, 55, 255);
					CRenderMgr::GetInst()->AddRenderText(tText);

					tRenderText tText2 = {};
					tText2.Detail = L"\"보통 고양이가 아니니\n단단히 각오 하셔야 할거에요..\"";
					tText2.FontSize = 20.f;
					tText2.Pos = Vec2(680, 490);
					tText2.RGBA = FONT_RGBA(222, 222, 222, 255);
					CRenderMgr::GetInst()->AddRenderText(tText2);

					break;
				}
				case 2:
				{
					tRenderText tText = {};
					tText.Detail = L"\"언제든지 환영합니다\"";
					tText.FontSize = 20.f;
					tText.Pos = Vec2(710, 470);
					tText.RGBA = FONT_RGBA(222, 222, 222, 255);
					CRenderMgr::GetInst()->AddRenderText(tText);
					break;
				}
				}

				if (KEY_TAP(KEY::A))
				{
#ifdef _DEBUG
					Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
					pInspector->SetTargetObject(nullptr);
					pInspector->SetTargetAsset(nullptr);
#endif
					switch (m_SelectIdx)
					{
					case 0:
					case 1:
					{
						CBlurControl* GodRay = static_cast<CBlurControl*>(CLevelMgr::GetInst()->FindObjectByName(L"GodRay")->FindScript((UINT)SCRIPT_TYPE::BLURCONTROL));
						if (GodRay != nullptr)
						{
							GodRay->SetEffect(BlurEffect::TurnOn);
							GodRay->Activate();
						}
						CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
						if (pPlayer != nullptr)
						{
							CPlayerScript* playerScript = static_cast<CPlayerScript*>(pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
							if (playerScript != nullptr)
								playerScript->ChangeState(PlayerState::IDLE);
						}

						CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
						if (textBox != nullptr)
						{
							CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
							if (uiScript != nullptr)
							{
								uiScript->Deactivate();
							}
						}

						++m_DialogIdx;
						break;
					}
					case 2:
					{
						Thaw();
						break;
					}
					}
				}
				break;
			}
			case 3:
			{
				CBlurControl* GodRay = static_cast<CBlurControl*>(CLevelMgr::GetInst()->FindObjectByName(L"GodRay")->FindScript((UINT)SCRIPT_TYPE::BLURCONTROL));
				if (GodRay != nullptr)
				{
					if (GodRay->IsFinished())
					{
						switch (m_SelectIdx)
						{
						case 0:
						{
							CPlayerManager::GetInst()->SetNextPos(Vec3(608.4f, -255.f, 1.8f));
							CPlayerManager::GetInst()->SetNextCamPos(Vec3(192.52f, 60.68f, 0.f));
							CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\DemonBossTEST.lv");
							ChangeLevel(pLevel, LEVEL_STATE::PLAY);

							CPlayerManager::GetInst()->StopCurBGM();
							break;
						}
						case 1:
						{
							CPlayerManager::GetInst()->SetNextPos(Vec3(-98.97f, -322.f, 1.8f));
							CPlayerManager::GetInst()->SetNextCamPos(Vec3(-142.f, -56.12f, 0.f));
							CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\LinethTEST.lv");
							ChangeLevel(pLevel, LEVEL_STATE::PLAY);

							CPlayerManager::GetInst()->StopCurBGM();
							break;
						}
						}
						m_DialogIdx = 0;
						m_SelectIdx = 0;
						m_Activated = false;
					}
				}
				break;
			}
			}

			break;
		}
		case NPCType::BOY:
		{
			tRenderText tText = {};
			tText.Detail = L"\"일어나셨군요!!\n마을로 나가시겠습니까?\" \n...▼";
			tText.FontSize = 20.f;
			tText.Pos = Vec2(730, 450);
			tText.RGBA = FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText);

			tRenderText tText2 = {};
			tText2.Detail = L"🚪 나간다";
			tText2.FontSize = 18.f;
			tText2.Pos = Vec2(730, 490);
			tText2.RGBA = m_SelectIdx == 0 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText2);

			tRenderText tText3 = {};
			tText3.Detail = L"☾ 좀 더 머무른다";
			tText3.FontSize = 18.f;
			tText3.Pos = Vec2(730, 518);
			tText3.RGBA = m_SelectIdx == 1 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText3);

			if (KEY_TAP(KEY::UP))
			{
				--m_SelectIdx;
				if (m_SelectIdx < 0)
					m_SelectIdx = 1;
			}

			if (KEY_TAP(KEY::DOWN))
			{
				++m_SelectIdx;
				m_SelectIdx %= 2;
			}

			if (KEY_TAP(KEY::A))
			{
				
				switch (m_SelectIdx)
				{
				case 0:
				{
#ifdef _DEBUG
					Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
					pInspector->SetTargetObject(nullptr);
					pInspector->SetTargetAsset(nullptr);
#endif
					CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\KohoVillageTEST.lv");
					CPlayerManager::GetInst()->SetNextPos(Vec3(-20.83f, -150.94f, 1.8f));
					CPlayerManager::GetInst()->SetNextCamPos(Vec3(-92.5, 83.f, 0.f));
					ChangeLevel(pLoadedLevel, LEVEL_STATE::PLAY);

					break;
				}
				case 1:
				{
					Animator2D()->Play(L"Boy_Idle", 1.f, true);
					break;
				}
				}

				Thaw();
			}
			break;
		}
		case NPCType::NONE:
		{
			break;
		}
		}

	}
}

void CNPCBehavior::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(NPCType), 1, _File);
}

void CNPCBehavior::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(NPCType), 1, _File);
}

void CNPCBehavior::Activate()
{
	m_Activated = true;

	switch (m_Type)
	{
	case NPCType::YUHACAT:
	{
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(-300.f, -150.5f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(335.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::DORA:
	{
		Animator2D()->Play(L"Dora_Popdown", 10.f, false);
		Animator2D()->Reset();
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(-9.f, -67.5f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(335.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::OLIVERIA:
	{
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(200.f, -150.f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(400.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::GRANDPA:
	{
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(-300.f, -67.5f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(335.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::CEREZA:
	{
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(-200.f, -150.f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(400.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::MATRIARCH:
	{
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(400.f, 160.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::AFTERBOSS:
	{
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(200.f, -150.f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(400.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::BOY:
	{
		Animator2D()->Play(L"Boy_Surprised", 11.f, true);
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(200.f, -150.f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(400.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::NONE:
	{
		break;
	}
	}
}

void CNPCBehavior::Thaw()
{
	CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
	if (textBox != nullptr)
	{
		CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
		if (uiScript != nullptr)
		{
			uiScript->Deactivate();
		}
	}

	CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	if (pPlayer != nullptr)
	{
		CPlayerScript* playerScript = static_cast<CPlayerScript*>(pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
		if (playerScript != nullptr)
			playerScript->ChangeState(PlayerState::IDLE);
	}
	m_DialogIdx = 0;
	m_SelectIdx = 0;
	m_Activated = false;
}
