#include "pch.h"
#include "AnimationEditor.h"
#include <numeric>
#include "ImGui/imgui_neo_internal.h"
#include "ImGui/imgui_neo_sequencer.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CEditorMgr.h"
#include "SpriteEditor.h"
#include "TreeUI.h"

AnimationEditor::AnimationEditor()
	: m_Animation(nullptr)
    , m_Play(false)
{
}

void AnimationEditor::Update()
{
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
        if (payload)
        {
            TreeNode** ppNode = (TreeNode**)payload->Data;
            TreeNode* pNode = *ppNode;

            Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
            if (ASSET_TYPE::ANIMATION == pAsset->GetAssetType())
            {
                m_Animation = (CAnimation*)pAsset.Get();
                Refresh();
            }
            else if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
            {
                if (m_Animation != nullptr)
                {
                    m_Animation->GetSpritesRef().push_back((CSprite*)pAsset.Get());
                    Refresh();
                }
            }
        }

        ImGui::EndDragDropTarget();
    }

    if (m_Animation.Get() != nullptr)
    {
        if (m_Play)
        {
            m_Acc += EngineDT;
            if (m_Acc > (1 / m_FPS))
            {
                ++m_CurrentFrame;
                m_Acc = 0.0f;
            }
            if (m_CurrentFrame >= m_EndFrame)
                m_CurrentFrame = 0;
        }

        static float deltaUV = 0.1f;
        Ptr<CSprite>& curSprite = m_Animation->GetSpriteRef(m_CurrentFrame);
        Vec2& offsetUV = curSprite->GetOffsetUVRef();
        Vec2 LeftTop = curSprite->GetLeftTopUV() - offsetUV;
        Vec2 Slice = curSprite->GetSliceUV() - offsetUV;
        vector<Ptr<CSprite>>& vecSprite = m_Animation->GetSpritesRef();
        
        if (ImGui::Button("Atlas Texture " ICON_FA_PICTURE_O, { 150, 30 }))
        {
            SpriteEditor* spriteEditor = static_cast<SpriteEditor*>(CEditorMgr::GetInst()->FindEditorUI("SpriteEditor"));
            spriteEditor->SetAtlas(curSprite->GetAtlasTexture());
            spriteEditor->SetActive(true);
        }
        ImGui::SameLine(160);
        if (ImGui::Button(ICON_FA_FLOPPY_O, { 30, 30 }))
        {
            if (std::strlen(m_AnimName) < 1)
            {
                wstring savePath = L"animation\\" + m_Animation->GetKey();
                m_Animation->Save(savePath);
            }
            else
            {
                string strPath(m_AnimName);
                strPath += ".anim";
                wstring wstrPath(strPath.begin(), strPath.end());

                m_Animation->Save(L"animation\\" + wstrPath);
            }
        }
        ImGui::SetItemTooltip(u8"애니메이션을 저장합니다");

        ImGui::SameLine(210);
        ImGui::SetNextItemWidth(100);
        ImGui::InputText("##SaveAs", m_AnimName, sizeof(m_AnimName));
        ImGui::SetItemTooltip(u8"저장할 이름\n다른이름으로 저장할 때만 작성");

        ImGui::SameLine(800);
        if (ImGui::Button(ICON_FA_FILE_IMAGE_O "NEW", { 100, 30 }))
        {
            Ptr<CAnimation> temp = new CAnimation;
            Ptr<CSprite> tempSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"DefaultSprite");
            temp->AddSprite(tempSprite);

            m_Animation = temp;
            Refresh();
        }
        // 스프라이트 벡터
        ImGui::NewLine();

        int vecOffset = 10;
        
        for (size_t i = 0; i < vecSprite.size(); ++i)
        {
            Vec2 tempLT = vecSprite[i]->GetLeftTopUV();
            Vec2 tempRB = vecSprite[i]->GetSliceUV();
            ImGui::SameLine(vecOffset);
            if (ImGui::ImageButton(vecSprite[i]->GetAtlasTexture()->GetSRV().Get(), {40, 50}, {tempLT.x, tempLT.y}, {tempLT.x + tempRB.x, tempLT.y + tempRB.y}, -1, {0, 0, 0, 0}, {1, 1, 1, 1}))
            {
                vecSprite.erase(vecSprite.begin() + i);
                Refresh();
            }
            ImGui::SetItemTooltip(u8"스프라이트 제거");

            vecOffset += 50;
            if (vecOffset > 640)
            {
                vecOffset = 10;
                ImGui::NewLine();
            }
        }

        ImGui::SameLine(700);
        ImGui::Image(curSprite->GetAtlasTexture().Get()->GetSRV().Get(), { 150, 150 }, { LeftTop.x, LeftTop.y }, { LeftTop.x + Slice.x, LeftTop.y + Slice.y });

        ImGui::NewLine();
        ImGui::SameLine(700);
        ImGui::TextColored({ 0.3f, 0.5f, 0.7f, 1.0f }, "<%f , %f>", offsetUV.x, offsetUV.y);

        ImGui::NewLine();

        ImGui::SameLine(770);
        if (ImGui::Button(ICON_FA_ARROW_UP, { 25, 25 }))
        {
            offsetUV.y += deltaUV;
        }
        ImGui::NewLine();
        ImGui::SameLine(500);
        ImGui::SetNextItemWidth(200.f);
        ImGui::InputFloat("##OffsetDelta", &deltaUV, 0.001f, 0.005f, "%.4f");
        ImGui::SetItemTooltip(u8"얼만큼 움직일지 정한 후 \n화살표로 Offset을 조절합니다");
        
        ImGui::SameLine(740);
        if (ImGui::Button(ICON_FA_ARROW_LEFT, { 25, 25 }))
        {
            offsetUV.x -= deltaUV;
        }
        ImGui::SameLine(800);
        if (ImGui::Button(ICON_FA_ARROW_RIGHT, {25, 25}))
        {
            offsetUV.x += deltaUV;
        }
        ImGui::NewLine();

        ImGui::SameLine(770);
        if (ImGui::Button(ICON_FA_ARROW_DOWN, { 25, 25 }))
        {
            offsetUV.y -= deltaUV;
        }
        ImGui::NewLine();
        if (ImGui::Button(m_Play ? ICON_FA_PAUSE : ICON_FA_PLAY, { 50, 30 }))
        {
            m_Play = !m_Play;
        }
        ImGui::SameLine(60);
        ImGui::SetNextItemWidth(100.f);
        ImGui::DragFloat("FPS", &m_FPS, 1.0f, 0.1f, 60.f, "%.1f", 0);
        ImGui::SetItemTooltip(u8"애니메이션 재생 속도 조절");

        if (ImGui::BeginNeoSequencer(u8"애니메이션", &m_CurrentFrame, &m_StartFrame, &m_EndFrame, {0, 0})) {
            
                std::vector<ImGui::FrameIndexType> keys(m_EndFrame);
                std::iota(keys.begin(), keys.end(), 0);

                if (ImGui::BeginNeoTimeline(u8"키프레임", keys)) {
                    ImGui::EndNeoTimeLine();
            }

            ImGui::EndNeoSequencer();
        }

	}
    else
    {
        ImGui::SetWindowFontScale(1.18f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.222f, 0.222f, 0.222f, 0.7f });


        ImGui::SameLine(160);
        if (ImGui::Button(ICON_FA_ARROW_LEFT, { 25, 25 }))
        {
        }
        ImGui::SameLine(200);
        if (ImGui::Button(ICON_FA_ARROW_RIGHT, { 25, 25 }))
        {
        }

        ImGui::SameLine(400);
        if (ImGui::Button(ICON_FA_PENCIL_SQUARE_O, { 30, 30 }))
        {
        }
       
        ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, u8"애니메이션을 선택해주세요!!!");
        ImGui::PopStyleColor(3);

        ImGui::SameLine(800);
        if (ImGui::Button(ICON_FA_FILE_IMAGE_O "NEW", { 100, 30 }))
        {
            Ptr<CAnimation> temp = new CAnimation;
            Ptr<CSprite> tempSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"DefaultSprite");
            temp->AddSprite(tempSprite);

            m_Animation = temp;
            Refresh();
        }
    }
}

void AnimationEditor::Refresh()
{
    m_CurrentFrame = 0;
    m_StartFrame = 0;
    m_EndFrame = m_Animation->GetMaxFrameCount();
    m_Play = false;
    m_FPS = 1.0f;
}
