#include "UIManager.hpp"
#include "Util/Image.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

UIManager::UIManager() {
    // 1. 背景與容器
    m_Background = std::make_shared<Util::GameObject>();
    m_Background->SetDrawable(std::make_shared<Util::Image>("Resources/material/gameplaybg.png"));
    m_Background->SetZIndex(0);

    m_Container = std::make_shared<Util::GameObject>();
    m_Container->SetDrawable(std::make_shared<Util::Image>("Resources/material/container.png"));
    m_Container->SetZIndex(1);
    m_Container->m_Transform.translation = {0.0f, -30.0f};
    m_Container->m_Transform.scale = {0.65f, 0.65f};
    // 2. 分數區
    m_ScoreLabel = std::make_shared<Util::GameObject>();
    m_ScoreLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/score.png"));
    m_ScoreLabel->m_Transform.translation = {-400.0f, 250.0f};
    m_ScoreLabel->m_Transform.scale = {1.3f, 1.3f};
    m_ScoreLabel->SetZIndex(2);

    m_ScoreBubble = std::make_shared<Util::GameObject>();
    m_ScoreBubble->SetDrawable(std::make_shared<Util::Image>("Resources/material/scorebubble.png"));
    m_ScoreBubble->m_Transform.translation = {-400.0f, 150.0f};
    m_ScoreBubble->m_Transform.scale = {0.8f, 0.8f};
    m_ScoreBubble->SetZIndex(1);

    m_ScoreText = std::make_shared<Util::GameObject>();
    UpdateScore(0); // 初始分數
    m_ScoreText->m_Transform.translation = {-400.0f, 150.0f};
    m_ScoreText->SetZIndex(3);
    // 3. 預告與進化區
    m_NextLabel = std::make_shared<Util::GameObject>();
    m_NextLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/next.png"));
    m_NextLabel->m_Transform.translation = {425.0f, 255.0f};
    m_NextLabel->m_Transform.scale = {0.3f, 0.3f};
    m_NextLabel->SetZIndex(2);

    m_NextBubble = std::make_shared<Util::GameObject>();
    m_NextBubble->SetDrawable(std::make_shared<Util::Image>("Resources/material/nextfruitbubble.png"));
    m_NextBubble->m_Transform.translation = {425.0f, 150.0f};
    m_NextBubble->m_Transform.scale = {0.8f, 0.8f};
    m_NextBubble->SetZIndex(1);

    m_NextFruitUI = std::make_shared<Util::GameObject>();
    m_NextFruitUI->m_Transform.translation = {425.0f, 150.0f};
    m_NextFruitUI->SetZIndex(2);

    m_EvolutionLabel = std::make_shared<Util::GameObject>();
    m_EvolutionLabel->SetDrawable(std::make_shared<Util::Image>("Resources/material/circleofevolution.png"));
    m_EvolutionLabel->m_Transform.translation = {425.0f, -15.0f};
    m_EvolutionLabel->SetZIndex(2);

    m_EvolutionCircle = std::make_shared<Util::GameObject>();
    m_EvolutionCircle->SetDrawable(std::make_shared<Util::Image>("Resources/material/fruitcircle.png"));
    m_EvolutionCircle->m_Transform.translation = {425.0f, -138.0f};
    m_EvolutionCircle->m_Transform.scale = {0.7f, 0.7f};
    m_EvolutionCircle->SetZIndex(1);

    // 4. 操作提示文字
    m_ControlsText = std::make_shared<Util::GameObject>();
    m_ControlsText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf",
        24,
        "[ P : Pause    A/D : Move    Space : Drop ]",
        Util::Color(0, 0, 0, 255) // 黑色
    ));
    m_ControlsText->SetZIndex(100);
    m_ControlsText->m_Transform.translation = {400.0f, -345.0f};
}
void UIManager::UpdateScore(int score) {
    m_ScoreText->SetDrawable(std::make_shared<Util::Text>(
        "Resources/material/font/Roboto-Regular.ttf", 60,
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
    if (m_Background) m_Background->Draw();
    if (m_Container) m_Container->Draw();
    if (m_ScoreBubble) m_ScoreBubble->Draw();
    if (m_ScoreLabel) m_ScoreLabel->Draw();
    if (m_ScoreText) m_ScoreText->Draw();
    if (m_NextLabel) m_NextLabel->Draw();
    if (m_NextBubble) m_NextBubble->Draw();
    if (m_NextFruitUI) m_NextFruitUI->Draw();
    if (m_EvolutionLabel) m_EvolutionLabel->Draw();
    if (m_EvolutionCircle) m_EvolutionCircle->Draw();
    if (m_ControlsText) m_ControlsText->Draw();
}