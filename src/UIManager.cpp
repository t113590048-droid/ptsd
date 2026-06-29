#include "UIManager.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include <cmath> 

UIManager::UIManager() {
    // 1. 背景 (包含容器)
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(std::make_shared<Util::Image>("Resources/material/bg_game.png"));
    m_Background->SetZIndex(0);
    m_Background->m_Transform.scale = {0.67f, 0.67f};

    // ✨ 這裡已刪除 m_Container 的載入與設定

    // ==========================================
    // 2. 分數區
    // ==========================================
    m_ScoreBubble = std::make_shared<Util::GameObject>();
    m_ScoreBubble->SetDrawable(std::make_shared<Util::Image>("Resources/material/foam_score.png"));
    m_ScoreBubble->m_Transform.translation = {-405.0f, 195.0f};
    m_ScoreBubble->m_Transform.scale = {0.6f, 0.6f};
    m_ScoreBubble->SetZIndex(1);

    m_ScoreText = std::make_shared<Util::GameObject>();
    UpdateScore(0);
    m_ScoreText->m_Transform.translation = {-400.0f, 160.0f};
    m_ScoreText->m_Transform.scale = {0.7f, 0.7f};
    m_ScoreText->SetZIndex(3);

    // ==========================================
    // 3. 預告區
    // ==========================================
    m_NextBubble = std::make_shared<Util::GameObject>();
    m_NextBubble->SetDrawable(std::make_shared<Util::Image>("Resources/material/foam_next.png"));
    m_NextBubble->m_Transform.translation = {425.0f, 150.0f};
    m_NextBubble->m_Transform.scale = {0.6f, 0.6f};
    m_NextBubble->SetZIndex(1);

    m_NextFruitUI = std::make_shared<Util::GameObject>();
    m_NextFruitUI->m_Transform.translation = {425.0f, 160.0f};
    m_NextFruitUI->SetZIndex(2);

    // ==========================================
    // 4. 操作提示文字
    // ==========================================
    m_ControlsText = std::make_shared<Util::GameObject>();
    m_ControlsText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/FOT-OEDKTRSTD-E.otf",
        24,
        "[ P : Pause    A/D : Move    Space : Drop    C : Cheat    UP/DOWN : Volume ]",
        Util::Color(0, 0, 0, 255)
    ));
    m_ControlsText->SetZIndex(89);
    m_ControlsText->m_Transform.translation = {100.0f, -345.0f};
}

void UIManager::UpdateScore(int score) {
    m_ScoreText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/FOT-OEDKTRSTD-E.otf", 60,
        std::to_string(score), Util::Color(255, 255, 255, 255)));
}

void UIManager::UpdateNextFruit(FruitLevel level) {
    std::string path = "Resources/material/fruit/";
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
    m_NextFruitUI->SetDrawable(std::make_shared<Util::Image>(path));
    m_NextFruitUI->m_Transform.scale = {0.5f, 0.5f};
}

void UIManager::Draw() {
    static float floatTimer = 0.0f;
    floatTimer += 1.0f / 60.0f;

    float scoreOffset = std::sin(floatTimer * 3.0f) * 8.0f;
    float nextOffset = std::sin(floatTimer * 3.0f + 1.0f) * 8.0f;

    if (m_ScoreBubble) m_ScoreBubble->m_Transform.translation.y = 195.0f + scoreOffset;
    if (m_ScoreText) m_ScoreText->m_Transform.translation.y = 160.0f + scoreOffset;

    if (m_NextBubble) m_NextBubble->m_Transform.translation.y = 167.0f + nextOffset;
    if (m_NextFruitUI) m_NextFruitUI->m_Transform.translation.y = 170.0f + nextOffset;

    // ==========================================
    // 繪圖邏輯 (✨ 已移除 m_Container)
    // ==========================================
    if (m_Background) m_Background->Draw();

    if (m_ScoreBubble) m_ScoreBubble->Draw();
    if (m_ScoreText) m_ScoreText->Draw();

    if (m_NextBubble) m_NextBubble->Draw();
    if (m_NextFruitUI) m_NextFruitUI->Draw();

    if (m_ControlsText) m_ControlsText->Draw();
}