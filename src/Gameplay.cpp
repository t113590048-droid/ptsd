#include "Gameplay.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

Gameplay::Gameplay() {
    // === 1. 載入背景圖 ===
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(std::make_shared<Util::Image>("Resources/material/gameplaybg.png"));
    m_Background->SetZIndex(0);
    m_Background->m_Transform.translation = {0.0f, 0.0f};
    m_Background->m_Transform.scale = {1.0f, 1.0f};

    // === 2. 載入容器盒 ===
    m_Container = std::make_shared<Util::GameObject>();
    m_Container->SetDrawable(std::make_shared<Util::Image>("Resources/material/container.png"));
    m_Container->SetZIndex(1);
    m_Container->m_Transform.translation = {0.0f, -30.0f};
    m_Container->m_Transform.scale = {0.65f, 0.65f};

    // === 3. 載入上方的雲 ===
    m_Cloud = std::make_shared<Util::GameObject>();
    m_Cloud->SetDrawable(std::make_shared<Util::Image>("Resources/material/cloud.png"));
    m_Cloud->SetZIndex(10);
    m_Cloud->m_Transform.translation = {48.0f, 310.0f};
    m_Cloud->m_Transform.scale = {1.5f, 1.5f};

    // 👇 新增：=== 3.5. 載入瞄準線 ===
    m_AimLine = std::make_shared<Util::GameObject>();
    m_AimLine->SetDrawable(std::make_shared<Util::Image>("Resources/material/aimline.png"));
    m_AimLine->SetZIndex(2);

    // 👇 1. 調整 Y 座標：把線的「中心點」往下放，大約在容器中間的位置
    m_AimLine->m_Transform.translation = {48.0f, 21.0f};

    // 👇 2. 調整 Y 縮放比例：把線大幅拉長 (因為不知道你原圖多長，先設 20.0f 測試)
    m_AimLine->m_Transform.scale = {1.0f, 1.95f};

    // === 4. 載入 UI 文字與框框 ===

    // 左上角 Score Label
    m_ScoreLabel = std::make_shared<Util::GameObject>();
    m_ScoreLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/score.png"));
    m_ScoreLabel->SetZIndex(1);
    m_ScoreLabel->m_Transform.translation = {-350.0f, 250.0f};
    m_ScoreLabel->m_Transform.scale = {1.0f, 1.0f};

    // 右上角 Next Label
    m_NextLabel = std::make_shared<Util::GameObject>();
    m_NextLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/next.png"));
    m_NextLabel->SetZIndex(2);
    m_NextLabel->m_Transform.translation = {425.0f, 280.0f};
    m_NextLabel->m_Transform.scale = {0.3f, 0.3f};

    // 右上角 泡泡框
    m_NextBubble = std::make_shared<Util::GameObject>();
    m_NextBubble->SetDrawable(std::make_shared<Util::Image>("Resources/material/nextfruitbubble.png"));
    m_NextBubble->SetZIndex(1);
    m_NextBubble->m_Transform.translation = {425.0f, 150.0f};
    m_NextBubble->m_Transform.scale = {0.7f, 0.7f};

    // 右側中間 進化圈 Label
    m_EvolutionLabel = std::make_shared<Util::GameObject>();
    m_EvolutionLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/circleofevolution.png"));
    m_EvolutionLabel->SetZIndex(2);
    m_EvolutionLabel->m_Transform.translation = {425.0f, -15.0f};
    m_EvolutionLabel->m_Transform.scale = {0.8f, 0.8f};

    // 右下角 進化圈圖
    m_EvolutionCircle = std::make_shared<Util::GameObject>();
    m_EvolutionCircle->SetDrawable(std::make_shared<Util::Image>("Resources/material/fruitcircle.png"));
    m_EvolutionCircle->SetZIndex(1);
    m_EvolutionCircle->m_Transform.translation = {425.0f, -138.0f};
    m_EvolutionCircle->m_Transform.scale = {0.7f, 0.7f};
}

void Gameplay::Update() {
    // === 1. 處理雲的移動邏輯 ===
    float moveSpeed = 7.0f;

    float leftLimit = -175.0f;
    float rightLimit = 245.0f;

    // 偵測 A 鍵 (向左)
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        m_Cloud->m_Transform.translation.x -= moveSpeed;
    }

    // 偵測 D 鍵 (向右)
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        m_Cloud->m_Transform.translation.x += moveSpeed;
    }

    // === 2. 邊界檢查 (分開檢查左右) ===
    if (m_Cloud->m_Transform.translation.x > rightLimit) {
        m_Cloud->m_Transform.translation.x = rightLimit;
    }
    if (m_Cloud->m_Transform.translation.x < leftLimit) {
        m_Cloud->m_Transform.translation.x = leftLimit;
    }

    // 👇 新增：=== 2.5 讓瞄準線跟著雲 ===
    if (m_AimLine) {
        m_AimLine->m_Transform.translation.x = m_Cloud->m_Transform.translation.x - 45;
    }

    // === 3. 執行繪製 ===
    if (m_Background) m_Background->Draw();
    if (m_Container) m_Container->Draw();

    // 👇 新增：畫出瞄準線
    if (m_AimLine) m_AimLine->Draw();

    // 畫 UI 元件
    if (m_ScoreLabel) m_ScoreLabel->Draw();
    if (m_NextLabel) m_NextLabel->Draw();
    if (m_NextBubble) m_NextBubble->Draw();
    if (m_EvolutionLabel) m_EvolutionLabel->Draw();
    if (m_EvolutionCircle) m_EvolutionCircle->Draw();

    // 雲要在最後畫
    if (m_Cloud) m_Cloud->Draw();
}