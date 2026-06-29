#include "Cloud.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <string>
#include <cstdlib>

Cloud::Cloud() {
    const float npcScale = 0.67f; // ✨ 縮小成三分之二

    // 1. 初始化組合式雲朵 NPC 各部位
    m_NpcBoard = std::make_shared<Util::GameObject>();
    m_NpcBoard->SetDrawable(std::make_shared<Util::Image>("Resources/material/npc/npc_board.png"));
    m_NpcBoard->SetZIndex(10);
    m_NpcBoard->m_Transform.translation = {48.0f, 310.0f}; // ✨ 雲朵整體往下移 5 單位 (從 315 變 310)
    m_NpcBoard->m_Transform.scale = {npcScale, npcScale};

    // 腮紅 (跟隨雲朵下移)
    m_NpcBlush = std::make_shared<Util::GameObject>();
    m_NpcBlush->SetDrawable(std::make_shared<Util::Image>("Resources/material/npc/npc_blush.png"));
    m_NpcBlush->SetZIndex(11);
    m_NpcBlush->m_Transform.translation = {48.0f, std::round(310.0f - 7.5f * npcScale)};
    m_NpcBlush->m_Transform.scale = {npcScale, npcScale};

    // 嘴巴 (跟隨雲朵下移)
    m_NpcFace = std::make_shared<Util::GameObject>();
    m_NpcFace->SetDrawable(std::make_shared<Util::Image>("Resources/material/npc/npc_mouth.png"));
    m_NpcFace->SetZIndex(11);
    m_NpcFace->m_Transform.translation = {48.0f, std::round(310.0f - 21.5f * npcScale)};
    m_NpcFace->m_Transform.scale = {npcScale, npcScale};

    m_EyeOpenImage = std::make_shared<Util::Image>("Resources/material/npc/npc_eye1.png");
    m_EyeClosedImage = std::make_shared<Util::Image>("Resources/material/npc/npc_eye2.png");

    // 眼睛 (跟隨雲朵下移)
    m_NpcEyes = std::make_shared<Util::GameObject>();
    m_NpcEyes->SetDrawable(m_EyeOpenImage);
    m_NpcEyes->SetZIndex(12);
    m_NpcEyes->m_Transform.translation = {48.0f, std::round(310.0f - 3.0f * npcScale)};
    m_NpcEyes->m_Transform.scale = {npcScale, npcScale};

    // 2. 初始化瞄準線 (置於雲朵左下角，偏移 -55.0f)
    m_AimLine = std::make_shared<Util::GameObject>();
    m_AimLine->SetDrawable(std::make_shared<Util::Image>("Resources/material/aimline.png"));
    m_AimLine->SetZIndex(2);
    m_AimLine->m_Transform.translation = {48.0f - 55.0f, 20.0f};
    m_AimLine->m_Transform.scale = {1.0f, 1.95f};

    // 3. 初始化手上的水果空殼 (圖片會在 SetCurrentFruit 給予，置於左下角，偏移 -55.0f，Y 設為 267.0f)
    m_CurrentFruitUI = std::make_shared<Util::GameObject>();
    m_CurrentFruitUI->SetZIndex(11);
    m_CurrentFruitUI->m_Transform.translation = {48.0f - 55.0f, 267.0f};

    // 4. 初始化眨眼計時器
    m_BlinkTimer = GetRandomBlinkInterval();
    m_BlinkDurationTimer = 0.0f;
    m_IsBlinking = false;
}

float Cloud::GetRandomBlinkInterval() {
    // 隨機眨眼間隔為 2.0 秒 到 6.0 秒
    return 2.0f + static_cast<float>(std::rand()) / RAND_MAX * 4.0f;
}

void Cloud::SetCurrentFruit(FruitLevel level) {
    std::string path = "Resources/material/fruit/";
    float scale = 0.6f;
    switch (level) {
        case FruitLevel::Cherry: path += "cherry.png"; break;
        case FruitLevel::Strawberry: path += "strawberry.png"; break;
        case FruitLevel::Grape: path += "grape.png"; break;
        case FruitLevel::tangerine: path += "tangerine.png"; break;
        case FruitLevel::Orange: path += "orange.png"; break;
        case FruitLevel::Apple: path += "apple.png"; break;
        case FruitLevel::Pear: path += "pear.png"; break;
        case FruitLevel::Peach: path += "peach.png"; break;
        case FruitLevel::Pineapple: path += "pineapple.png"; break;
        case FruitLevel::Melon: path += "melon.png"; break;
        case FruitLevel::Watermelon: path += "watermelon.png"; break;
        default: path += "cherry.png"; break;
    }
    m_CurrentFruitUI->SetDrawable(std::make_shared<Util::Image>(path));
    m_CurrentFruitUI->m_Transform.scale = {scale, scale};
}

void Cloud::Update() {
    // 1. 偵測左右移動
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) m_NpcBoard->m_Transform.translation.x -= m_MoveSpeed;
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) m_NpcBoard->m_Transform.translation.x += m_MoveSpeed;
    // 2. 限制不出界
    if (m_NpcBoard->m_Transform.translation.x > m_RightLimit) m_NpcBoard->m_Transform.translation.x = m_RightLimit;
    if (m_NpcBoard->m_Transform.translation.x < m_LeftLimit) m_NpcBoard->m_Transform.translation.x = m_LeftLimit;

    // 3. 讓瞄準線與手上的水果以及五官跟著雲朵走 (水果/瞄準線對齊左下角偏移 -55)
    if (m_AimLine) m_AimLine->m_Transform.translation.x = m_NpcBoard->m_Transform.translation.x - 55.0f;
    if (m_CurrentFruitUI) {
        m_CurrentFruitUI->m_Transform.translation.x = m_NpcBoard->m_Transform.translation.x - 55.0f;
        m_CurrentFruitUI->m_Transform.translation.y = 267.0f;
    }

    const float npcScale = 0.67f;
    if (m_NpcBlush) {
        m_NpcBlush->m_Transform.translation.x = std::round(m_NpcBoard->m_Transform.translation.x);
        m_NpcBlush->m_Transform.translation.y = std::round(m_NpcBoard->m_Transform.translation.y - 7.5f * npcScale);
    }
    if (m_NpcFace) {
        m_NpcFace->m_Transform.translation.x = std::round(m_NpcBoard->m_Transform.translation.x);
        m_NpcFace->m_Transform.translation.y = std::round(m_NpcBoard->m_Transform.translation.y - 21.5f * npcScale);
    }
    if (m_NpcEyes) {
        m_NpcEyes->m_Transform.translation.x = std::round(m_NpcBoard->m_Transform.translation.x);
        m_NpcEyes->m_Transform.translation.y = std::round(m_NpcBoard->m_Transform.translation.y - 3.0f * npcScale);
    }

    // 4. 減少發射冷卻計時器
    if (m_CooldownTimer > 0.0f) {
        m_CooldownTimer -= 1.0f / 60.0f;
    }

    // 5. 偵測發射鍵 (SPACE)
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE) && m_CooldownTimer <= 0.0f) {
        m_IsDropping = true;
        m_CooldownTimer = m_CooldownTime;
    } else {
        m_IsDropping = false;
    }

    // 6. 隨機眨眼動畫控制
    if (m_IsBlinking) {
        m_BlinkDurationTimer -= 1.0f / 60.0f;
        if (m_BlinkDurationTimer <= 0.0f) {
            m_IsBlinking = false;
            m_NpcEyes->SetDrawable(m_EyeOpenImage);
            m_BlinkTimer = GetRandomBlinkInterval();
        }
    } else {
        m_BlinkTimer -= 1.0f / 60.0f;
        if (m_BlinkTimer <= 0.0f) {
            m_IsBlinking = true;
            m_BlinkDurationTimer = 0.15f; // 眨眼閉眼持續 0.15 秒
            m_NpcEyes->SetDrawable(m_EyeClosedImage);
        }
    }
}

bool Cloud::IsDroppingFruit() const {
    return m_IsDropping;
}

float Cloud::GetDropX() const {
    return m_CurrentFruitUI->m_Transform.translation.x;
}

void Cloud::Draw() {
    if (m_AimLine) m_AimLine->Draw();
    if (m_CurrentFruitUI) m_CurrentFruitUI->Draw();
    if (m_NpcBoard) m_NpcBoard->Draw();
    if (m_NpcBlush) m_NpcBlush->Draw();
    if (m_NpcFace) m_NpcFace->Draw();
    if (m_NpcEyes) m_NpcEyes->Draw();
}