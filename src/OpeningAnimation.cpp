#include "OpeningAnimation.hpp"
#include "Util/Image.hpp"
#include <cmath>
#include <string>

OpeningAnimation::OpeningAnimation() {
    // 1. 載入 Ready 與 Go 圖片 (使用 static 緩存避免每次載入與上傳紋理)
    static auto readyImage = std::make_shared<Util::Image>("Resources/material/first page/ready.png");
    m_ReadyText = std::make_shared<Util::GameObject>();
    m_ReadyText->SetDrawable(readyImage);
    m_ReadyText->SetZIndex(99);
    m_ReadyText->m_Transform.scale = {0.4f, 0.4f};

    static auto goImage = std::make_shared<Util::Image>("Resources/material/first page/go.png");
    m_GoText = std::make_shared<Util::GameObject>();
    m_GoText->SetDrawable(goImage);
    m_GoText->SetZIndex(99);
    m_GoText->m_Transform.scale = {0.4f, 0.4f};

    // 2. 載入 0.png 到 10.png 的泡泡水果 (使用 static 緩存)
    static std::vector<std::shared_ptr<Util::Image>> bubbleImages;
    if (bubbleImages.empty()) {
        for (int i = 0; i < 11; i++) {
            std::string path = "Resources/material/fruit/" + std::to_string(i) + ".png";
            bubbleImages.push_back(std::make_shared<Util::Image>(path));
        }
    }
    for (int i = 0; i < 11; i++) {
        auto bubble = std::make_shared<Util::GameObject>();
        bubble->SetDrawable(bubbleImages[i]);
        bubble->SetZIndex(98);
        bubble->m_Transform.scale = {0.4f, 0.4f};
        m_BubbleFruits.push_back(bubble);
    }

    // 3. 預先載入布幕圖片 cloth1.png 到 cloth5.png (使用 static 緩存)
    static std::vector<std::shared_ptr<Util::Image>> curtainImages;
    if (curtainImages.empty()) {
        for (int i = 1; i <= 5; i++) {
            std::string path = "Resources/material/npc/cloth" + std::to_string(i) + ".png";
            curtainImages.push_back(std::make_shared<Util::Image>(path));
        }
    }
    m_CurtainImages = curtainImages;

    // 4. 左右布幕設定
    m_LeftCurtain = std::make_shared<Util::GameObject>();
    m_LeftCurtain->SetDrawable(m_CurtainImages[4]);
    m_LeftCurtain->SetZIndex(90);
    m_LeftCurtain->m_Transform.scale = {0.72f, 0.72f};
    m_LeftCurtain->m_Transform.translation = {-1200.0f, 0.0f}; // ✨ 初始化起點，防止第一幀出現在中間 (0, 0)

    m_RightCurtain = std::make_shared<Util::GameObject>();
    m_RightCurtain->SetDrawable(m_CurtainImages[4]);
    m_RightCurtain->SetZIndex(90);
    m_RightCurtain->m_Transform.scale = {-0.72f, 0.72f}; // 水平鏡像翻轉
    m_RightCurtain->m_Transform.translation = {1200.0f, 0.0f}; // ✨ 初始化起點，防止第一幀出現在中間 (0, 0)
}

void OpeningAnimation::Update(float deltaTime) {
    if (m_State == State::FINISHED) return;

    m_Timer += deltaTime;
    m_RotationAngle -= 2.0f * deltaTime; // 泡泡旋轉速度

    float radius = 200.0f; // 水果繞圈的半徑

    // ==========================================
    // 階段 1：左右布幕滑入 (0.0s ~ 0.5s)
    // ==========================================
    if (m_Timer < 0.5f) {
        m_State = State::CURTAIN_IN;
        float progress = m_Timer / 0.5f;

        m_LeftCurtain->m_Transform.translation = {-1200.0f + (900.0f * progress), 0.0f};
        m_RightCurtain->m_Transform.translation = {1200.0f - (900.0f * progress), 0.0f};
    }
    // ==========================================
    // 階段 2：布幕動畫 cloth5 -> cloth1 (0.5s ~ 1.0s)
    // ==========================================
    else if (m_Timer < 1.0f) {
        m_State = State::CURTAIN_ANIM;
        m_LeftCurtain->m_Transform.translation = {-300.0f, 0.0f};
        m_RightCurtain->m_Transform.translation = {300.0f, 0.0f};

        m_CurtainAnimTimer += deltaTime;
        if (m_CurtainAnimTimer > 0.1f && m_CurtainFrame > 1) {
            m_CurtainAnimTimer = 0.0f;
            m_CurtainFrame--;
            m_LeftCurtain->SetDrawable(m_CurtainImages[m_CurtainFrame - 1]);
            m_RightCurtain->SetDrawable(m_CurtainImages[m_CurtainFrame - 1]);
        }
    }
    // ==========================================
    // 階段 3：Ready? 旋轉 (1.0s ~ 2.5s)
    // ==========================================
    else if (m_Timer < 2.5f) {
        m_State = State::READY;
    }
    // ==========================================
    // 階段 4：Go!! 飛散 (2.5s ~ 4.5s)
    // ==========================================
    else if (m_Timer < 4.5f) {
        m_State = State::GO;
        float goProgress = m_Timer - 2.5f;

        radius = 200.0f + std::pow(goProgress, 3.0f) * 1500.0f;

        float scale = 0.4f + (goProgress * 0.2f);
        if (scale > 0.6f) scale = 0.6f;
        m_GoText->m_Transform.scale = {scale, scale};
    }
    // ==========================================
    // 階段 5：左右布幕滑出 (4.5s ~ 5.0s)
    // ==========================================
    else if (m_Timer < 5.0f) {
        m_State = State::CURTAIN_OUT;
        float progress = (m_Timer - 4.5f) / 0.5f;

        m_LeftCurtain->m_Transform.translation = {-300.0f - (900.0f * progress), 0.0f};
        m_RightCurtain->m_Transform.translation = {300.0f + (900.0f * progress), 0.0f};
    }
    // ==========================================
    // 階段 6：動畫結束 (5.0s)
    // ==========================================
    else {
        m_State = State::FINISHED;
        return;
    }

    // 更新泡泡位置
    if (m_State == State::READY || m_State == State::GO) {
        for (size_t i = 0; i < m_BubbleFruits.size(); i++) {
            float angle = m_RotationAngle + (i * (6.28318f / 11.0f));
            m_BubbleFruits[i]->m_Transform.translation.x = std::cos(angle) * radius;
            m_BubbleFruits[i]->m_Transform.translation.y = std::sin(angle) * radius;
        }
    }
}

void OpeningAnimation::Draw() {
    if (m_State == State::FINISHED) return;

    // 1. 最下層：先畫布幕 (隨時都要畫)
    if (m_LeftCurtain) m_LeftCurtain->Draw();
    if (m_RightCurtain) m_RightCurtain->Draw();

    // 2. 最上層：畫泡泡和文字 (布幕關上後才顯示)
    if (m_State == State::READY || m_State == State::GO) {
        for (auto& bubble : m_BubbleFruits) bubble->Draw();

        if (m_State == State::READY) m_ReadyText->Draw();
        else if (m_State == State::GO) m_GoText->Draw();
    }
}