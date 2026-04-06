#include "Gameplay.hpp"
#include "Util/Image.hpp"

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
    m_Container->m_Transform.scale = {0.65f, 0.65f}; // 容器大小維持這樣看起來很剛好

    // === 3. 載入上方的雲 ===
    m_Cloud = std::make_shared<Util::GameObject>();
    m_Cloud->SetDrawable(std::make_shared<Util::Image>("Resources/material/cloud.png"));
    m_Cloud->SetZIndex(10);
    m_Cloud->m_Transform.translation = {0.0f, 310.0f}; // 稍微往上移
    m_Cloud->m_Transform.scale = {1.5f, 1.5f}; // 👇 大幅放大

    // === 4. 載入 UI 文字與框框 ===

    // 左上角 Score Label
    m_ScoreLabel = std::make_shared<Util::GameObject>();
    m_ScoreLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/score.png"));
    m_ScoreLabel->SetZIndex(1);
    m_ScoreLabel->m_Transform.translation = {-350.0f, 250.0f};
    m_ScoreLabel->m_Transform.scale = {1.0f, 1.0f}; // 👇 放大

    // 右上角 Next Label
    m_NextLabel = std::make_shared<Util::GameObject>();
    m_NextLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/next.png"));
    m_NextLabel->SetZIndex(2);
    m_NextLabel->m_Transform.translation = {425.0f, 280.0f}; // 統一 X 座標
    m_NextLabel->m_Transform.scale = {0.3f, 0.3f}; // 👇 大幅縮小！

    // 右上角 泡泡框
    m_NextBubble = std::make_shared<Util::GameObject>();
    m_NextBubble->SetDrawable(std::make_shared<Util::Image>("Resources/material/nextfruitbubble.png"));
    m_NextBubble->SetZIndex(1);
    m_NextBubble->m_Transform.translation = {425.0f, 150.0f}; // 放在 Next 下方
    m_NextBubble->m_Transform.scale = {0.7f, 0.7f}; // 👇 縮小一點來配 Next

    // 右側中間 進化圈 Label
    m_EvolutionLabel = std::make_shared<Util::GameObject>();
    m_EvolutionLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/circleofevolution.png"));
    m_EvolutionLabel->SetZIndex(2);
    m_EvolutionLabel->m_Transform.translation = {425.0f, -20.0f}; // 往上拉一點
    m_EvolutionLabel->m_Transform.scale = {0.8f, 0.8f}; // 👇 放大

    // 右下角 進化圈圖
    m_EvolutionCircle = std::make_shared<Util::GameObject>();
    m_EvolutionCircle->SetDrawable(std::make_shared<Util::Image>("Resources/material/fruitcircle.png"));
    m_EvolutionCircle->SetZIndex(1);
    m_EvolutionCircle->m_Transform.translation = {425.0f, -160.0f}; // 對齊泡泡跟文字
    m_EvolutionCircle->m_Transform.scale = {0.55f, 0.55f}; // 稍微放大一點點
}

void Gameplay::Update() {
    // 繪製順序：背景先畫，然後畫上面的元件
    if (m_Background) m_Background->Draw();
    if (m_Container) m_Container->Draw();

    // 畫 UI 元件
    if (m_ScoreLabel) m_ScoreLabel->Draw();
    if (m_NextLabel) m_NextLabel->Draw();
    if (m_NextBubble) m_NextBubble->Draw();
    if (m_EvolutionLabel) m_EvolutionLabel->Draw();
    if (m_EvolutionCircle) m_EvolutionCircle->Draw();

    // 雲放最後畫，確保它蓋在盒子上面
    if (m_Cloud) m_Cloud->Draw();
}